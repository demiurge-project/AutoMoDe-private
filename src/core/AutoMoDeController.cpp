/*
 * @file <src/core/AutoMoDeControllerBehaviorTree.cpp>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * @package ARGoS3-AutoMoDe
 *
 * @license MIT License
 */

#include "AutoMoDeController.h"

namespace argos {

	/****************************************/
	/****************************************/

	AutoMoDeControllerBehaviorTree::AutoMoDeControllerBehaviorTree() {
		m_pcRobotState = new ReferenceModel1Dot2();
		m_unTimeStep = 0;
		m_strBtConfiguration = "";
		m_bMaintainHistory = false;
		m_bPrintReadableBt = false;
		m_strHistoryFolder = "./";
		m_bBehaviorTreeGiven = false;
	}

	/****************************************/
	/****************************************/

	AutoMoDeControllerBehaviorTree::~AutoMoDeControllerBehaviorTree() {
		delete m_pcRobotState;
		delete m_pcBehaviorTreeBuilder;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeControllerBehaviorTree::Init(TConfigurationNode& t_node) {
		// Parsing parameters
		try {
			GetNodeAttributeOrDefault(t_node, "bt-config", m_strBtConfiguration, m_strBtConfiguration);
			GetNodeAttributeOrDefault(t_node, "history", m_bMaintainHistory, m_bMaintainHistory);
			GetNodeAttributeOrDefault(t_node, "hist-folder", m_strHistoryFolder, m_strHistoryFolder);
			GetNodeAttributeOrDefault(t_node, "readable", m_bPrintReadableBt, m_bPrintReadableBt);
		} catch (CARGoSException& ex) {
			THROW_ARGOSEXCEPTION_NESTED("Error parsing <params>", ex);
		}

		m_unRobotID = atoi(GetId().substr(5, 6).c_str());
		m_pcRobotState->SetRobotIdentifier(m_unRobotID);

		/*
		 * If a BT configuration is given as parameter of the experiment file, create a FSM from it
		 */
		if (m_strBtConfiguration.compare("") != 0 && !m_bBehaviorTreeGiven) {
			m_pcBehaviorTreeBuilder = new AutoMoDeBehaviorTreeBuilder();
			SetBehaviorTree(m_pcBehaviorTreeBuilder->BuildBehaviorTree(m_strBtConfiguration));
			if (m_bMaintainHistory) {
				//m_pcBehaviorTree->SetHistoryFolder(m_strHistoryFolder);
				//m_pcBehaviorTree->MaintainHistory();
			}
			if (m_bPrintReadableBt) {
				std::cout << "Behavior Tree description: " << std::endl;
				std::cout << m_pcBehaviorTree->GetReadableFormat() << std::endl;
			}
		} else {
			LOGERR << "Warning: No behavior tree configuration found in .argos" << std::endl;
		}

		/*
		 *  Initializing sensors and actuators
		 */
		try{
			m_pcProximitySensor = GetSensor<CCI_EPuckProximitySensor>("epuck_proximity");
			m_pcLightSensor = GetSensor<CCI_EPuckLightSensor>("epuck_light");
			m_pcGroundSensor = GetSensor<CCI_EPuckGroundSensor>("epuck_ground");
			 m_pcRabSensor = GetSensor<CCI_EPuckRangeAndBearingSensor>("epuck_range_and_bearing");
			 m_pcCameraSensor = GetSensor<CCI_EPuckOmnidirectionalCameraSensor>("epuck_omnidirectional_camera");
		} catch (CARGoSException ex) {
			LOGERR<<"Error while initializing a Sensor!\n";
		}

		try{
			m_pcWheelsActuator = GetActuator<CCI_EPuckWheelsActuator>("epuck_wheels");
			m_pcRabActuator = GetActuator<CCI_EPuckRangeAndBearingActuator>("epuck_range_and_bearing");
			m_pcLEDsActuator = GetActuator<CCI_EPuckRGBLEDsActuator>("epuck_rgb_leds");
		} catch (CARGoSException ex) {
			LOGERR<<"Error while initializing an Actuator!\n";
		}

		/*
		 * Starts actuation.
		 */
		 InitializeActuation();
	}

	/****************************************/
	/****************************************/

	void AutoMoDeControllerBehaviorTree::ControlStep() {
		/*
		 * 1. Update RobotDAO
		 */
		if(m_pcRabSensor != NULL){
			const CCI_EPuckRangeAndBearingSensor::TPackets& packets = m_pcRabSensor->GetPackets();
			//m_pcRobotState->SetNumberNeighbors(packets.size());
			m_pcRobotState->SetRangeAndBearingMessages(packets);
		}
		if (m_pcGroundSensor != NULL) {
			const CCI_EPuckGroundSensor::SReadings& readings = m_pcGroundSensor->GetReadings();
			m_pcRobotState->SetGroundInput(readings);
		}
		if (m_pcLightSensor != NULL) {
			const CCI_EPuckLightSensor::TReadings& readings = m_pcLightSensor->GetReadings();
			m_pcRobotState->SetLightInput(readings);
		}
		if (m_pcProximitySensor != NULL) {
			const CCI_EPuckProximitySensor::TReadings& readings = m_pcProximitySensor->GetReadings();
			m_pcRobotState->SetProximityInput(readings);
		}

		/*
		 * 2. Execute step of running Action of BT
		 */
		m_pcBehaviorTree->ControlStep();

		/*
		 * 3. Update Actuators
		 */
		if (m_pcWheelsActuator != NULL) {
			m_pcWheelsActuator->SetLinearVelocity(m_pcRobotState->GetLeftWheelVelocity(),m_pcRobotState->GetRightWheelVelocity());
		}

		/*
		 * 4. Update variables and sensors
		 */
		if (m_pcRabSensor != NULL) {
			m_pcRabSensor->ClearPackets();
		}
		m_unTimeStep++;

	}

	/****************************************/
	/****************************************/

	void AutoMoDeControllerBehaviorTree::Destroy() {}

	/****************************************/
	/****************************************/

	void AutoMoDeControllerBehaviorTree::Reset() {
		m_pcBehaviorTree->Reset();
		m_pcRobotState->Reset();
		// Restart actuation.
		InitializeActuation();
	}

	/****************************************/
	/****************************************/

	void AutoMoDeControllerBehaviorTree::SetBehaviorTree(AutoMoDeBehaviorTree* pc_behaviour_tree) {
		m_pcBehaviorTree = pc_behaviour_tree;
		m_pcBehaviorTree->SetRobotDAO(m_pcRobotState);
		m_pcBehaviorTree->Init();
		m_bBehaviorTreeGiven = true;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeControllerBehaviorTree::InitializeActuation() {
		/*
		 * Constantly send range-and-bearing messages containing the robot integer identifier.
		 */
		if (m_pcRabActuator != NULL) {
			UInt8 data[4];
			data[0] = m_unRobotID;
			data[1] = 0;
			data[2] = 0;
			data[3] = 0;
			m_pcRabActuator->SetData(data);
		}
	}

	REGISTER_CONTROLLER(AutoMoDeControllerBehaviorTree, "automode_controller_bt");
}
