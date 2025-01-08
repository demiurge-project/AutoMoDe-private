/*
 * @file <src/core/AutoMoDePatchController.cpp>
 *
 * @author Jeanne Szpirer - <jszpirer@ulb.be>
 *
 * @package ARGoS3-AutoMoDe
 *
 * @license MIT License
 */

#include "AutoMoDePatchController.h"

namespace argos {

	/****************************************/
	/****************************************/

	AutoMoDePatchController::AutoMoDePatchController() {
		m_pcRobotState = new ReferenceModel4Dot0();
		m_unTimeStep = 0;
	}

	/****************************************/
	/****************************************/

	AutoMoDePatchController::~AutoMoDePatchController() {
		delete m_pcRobotState;
	}

	/****************************************/
	/****************************************/

	void AutoMoDePatchController::Init(TConfigurationNode& t_node) {
		m_unPatchID = atoi(GetId().substr(11).c_str());
		m_pcRobotState->SetRobotIdentifier(m_unPatchID);

		/*
		 * Initializing the sensor
		 */

		try{
			 m_pcRabSensor = GetSensor<CCI_EPuckRangeAndBearingSensor>("epuck_range_and_bearing");
		} catch (CARGoSException ex) {
			LOGERR<<"Error while initializing the Rab sensor!\n";
		}

		/*
		 *  Initializing the actuator
		*/

		try{
			m_pcRabActuator = GetActuator<CCI_EPuckRangeAndBearingActuator>("epuck_range_and_bearing");
		} catch (CARGoSException ex) {
			LOGERR<<"Error while initializing the Rab actuator!\n";
		}


		/*
		 * Starts actuation.
		 */
		 InitializeActuation();
	}

	/****************************************/
	/****************************************/

	void AutoMoDePatchController::ControlStep() {
		/*
		 * 1. Update RobotDAO
		 */
		if(m_pcRabSensor != NULL){
			const CCI_EPuckRangeAndBearingSensor::TPackets& packets = m_pcRabSensor->GetPackets();
			//m_pcRobotState->SetNumberNeighbors(packets.size());
			m_pcRobotState->SetRangeAndBearingMessages(packets);
		}

		/*
		 * 4. Update variables and sensors
		 */
		if (m_pcRabSensor != NULL) {
			m_pcRabSensor->ClearPackets();
		}
		/*
		 * Just the time step because the fake epuck is not supposed to do anything
		 */
		m_unTimeStep++;
	}

	/****************************************/
	/****************************************/

	void AutoMoDePatchController::Destroy() {}

	/****************************************/
	/****************************************/

	void AutoMoDePatchController::Reset() {
		m_pcRobotState->Reset();
		// Restart actuation.
		InitializeActuation();
	}

	/****************************************/
	/****************************************/

	void AutoMoDePatchController::SetColor(UInt8 color){
		LOG << "In the SetColor of the Patch Controller " << color << std::endl;
		m_color = color;

		InitializeActuation();
	}

	/****************************************/
	/****************************************/

	void AutoMoDePatchController::InitializeActuation() {
		/*
		 * Constantly send range-and-bearing messages containing the robot integer identifier.
		 */
		if (m_pcRabActuator != NULL) {
			UInt8 data[4];
			data[0] = m_unPatchID;
			data[1] = m_color;
			data[2] = 0;
			data[3] = 0;
			m_pcRabActuator->SetData(data);
		}
		LOG << "Message " << m_color << " sent by robot " << m_unPatchID << std::endl;
	}

	REGISTER_CONTROLLER(AutoMoDePatchController, "patchautomode_controller");
}
