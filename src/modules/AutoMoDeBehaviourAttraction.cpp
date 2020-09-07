/**
  * @file <src/modules/AutoMoDeBehaviourAttraction.cpp>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#include "AutoMoDeBehaviourAttraction.h"


namespace argos {

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourAttraction::AutoMoDeBehaviourAttraction() {
		m_strLabel = "Attraction";
	}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourAttraction::AutoMoDeBehaviourAttraction(AutoMoDeBehaviourAttraction* pc_behaviour) {
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

	AutoMoDeBehaviourAttraction::~AutoMoDeBehaviourAttraction() {}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourAttraction* AutoMoDeBehaviourAttraction::Clone() {
		return new AutoMoDeBehaviourAttraction(this);   // todo: check without *
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourAttraction::ControlStep() {

        m_pcRobotDAO->SetLEDsColor(CColor::BLACK);

        CVector2 sRabVector(0,CRadians::ZERO);
        CVector2 sProxVector(0,CRadians::ZERO);
        CVector2 sResultVector(0,CRadians::ZERO);

        /* Compute the interaction vector */
        CCI_EPuckRangeAndBearingSensor::SReceivedPacket cRabReading = m_pcRobotDAO->GetNeighborsCenterOfMass();
        sRabVector = CVector2(cRabReading.Range, cRabReading.Bearing);

        /* Compute the proximity vector */
        CCI_EPuckProximitySensor::SReading cProxReading = m_pcRobotDAO->GetProximityReading();
        sProxVector = CVector2(cProxReading.Value, cProxReading.Angle);

        /* if robot alone, go straight ahead */
        if (sRabVector.Length() < 0.1) {
            sRabVector = CVector2(1, CRadians::ZERO);
        }

        /* Compute the result vector, prox is an order the magnitude bigger */
        sResultVector = m_unAttractionParameter*sRabVector - 6*sProxVector;

        /* Compute the velocity of the wheels */
        m_pcRobotDAO->SetWheelsVelocity(MILowLevelController(sResultVector));

        m_bLocked = false;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourAttraction::Init() {
		std::map<std::string, Real>::iterator it = m_mapParameters.find("att");
		if (it != m_mapParameters.end()) {
			m_unAttractionParameter = it->second;
		} else {
			LOGERR << "[FATAL] Missing parameter for the following behaviour:" << m_strLabel << std::endl;
			THROW_ARGOSEXCEPTION("Missing Parameter");
		}
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourAttraction::Reset() {
		m_bOperational = false;
		ResumeStep();
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourAttraction::ResumeStep() {
		m_bOperational = true;
	}
}
