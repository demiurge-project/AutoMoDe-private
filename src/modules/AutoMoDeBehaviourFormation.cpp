/**
  * @file <src/modules/AutoMoDeBehaviourFormation.cpp>
  *
  * @author Fernando Mendiburu - <fmendiburu@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#include "AutoMoDeBehaviourFormation.h"


namespace argos {

	/****************************************/
	/****************************************/

    AutoMoDeBehaviourFormation::AutoMoDeBehaviourFormation() {
        m_strLabel = "Formation";
	}

	/****************************************/
	/****************************************/

    AutoMoDeBehaviourFormation::AutoMoDeBehaviourFormation(AutoMoDeBehaviourFormation* pc_behaviour) {
		m_strLabel = pc_behaviour->GetLabel();
		m_bLocked = pc_behaviour->IsLocked();
		m_bOperational = pc_behaviour->IsOperational();
		m_unIndex = pc_behaviour->GetIndex();
		m_unIdentifier = pc_behaviour->GetIdentifier();
		m_mapParameters = pc_behaviour->GetParameters();
		Init();
	}

	/****************************************/
	/****************************************/

    AutoMoDeBehaviourFormation::~AutoMoDeBehaviourFormation() {}

	/****************************************/
	/****************************************/

    AutoMoDeBehaviourFormation* AutoMoDeBehaviourFormation::Clone() {
        return new AutoMoDeBehaviourFormation(this);   // todo: check without *
	}

	/****************************************/
	/****************************************/

    void AutoMoDeBehaviourFormation::ControlStep() {


        m_pcRobotDAO->SetLEDsColor(CColor::CYAN);

        CVector2 sCamVector(0,CRadians::ZERO);
        CVector2 sProxVector(0,CRadians::ZERO);
        CVector2 sResultVector(0,CRadians::ZERO);

        /* Compute the interaction vector */
        CCI_EPuckOmnidirectionalCameraSensor::SBlob cCamReading = m_pcRobotDAO->GetNeighborsCoesion(m_sCoesionParams.Gain,
                                                                                                    m_sCoesionParams.TargetDistance,
                                                                                                    m_sCoesionParams.Exponent);
        sCamVector = CVector2(cCamReading.Distance, cCamReading.Angle);

        /* Compute the proximity vector */
        CCI_EPuckProximitySensor::SReading cProxReading = m_pcRobotDAO->GetProximityReading();
        sProxVector = CVector2(cProxReading.Value, cProxReading.Angle);

        /* Compute the result vector, prox is an order the magnitude bigger */
        sResultVector = sCamVector - 5*sProxVector;

        /* Compute the velocity of the wheels */
        m_pcRobotDAO->SetWheelsVelocity(MILowLevelController(sResultVector));

		m_bLocked = false;
	}

    /****************************************/
    /****************************************/

    void AutoMoDeBehaviourFormation::InitializeVariables() {
        m_sCoesionParams.Exponent = 2;
        m_sCoesionParams.Gain = 10;
    }

	/****************************************/
	/****************************************/

    void AutoMoDeBehaviourFormation::Init() {

        InitializeVariables();

        std::map<std::string, Real>::iterator itDist = m_mapParameters.find("dij");
        if (itDist != m_mapParameters.end()) {
            m_sCoesionParams.TargetDistance = itDist->second;
		} else {
			LOGERR << "[FATAL] Missing parameter for the following behaviour:" << m_strLabel << std::endl;
			THROW_ARGOSEXCEPTION("Missing Parameter");
		}
	}

	/****************************************/
	/****************************************/

    void AutoMoDeBehaviourFormation::Reset() {
		m_bOperational = false;
        InitializeVariables();
		ResumeStep();
	}

	/****************************************/
	/****************************************/

    void AutoMoDeBehaviourFormation::ResumeStep() {
		m_bOperational = true;
	}

}
