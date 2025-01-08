/**
  * @file <src/modules/AutoMoDeBehaviourGoAwayFromPatch.cpp>
  *
  * @author Jeanne Szpirer - <jszpirer@ulb.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#include "AutoMoDeBehaviourGoAwayFromPatch.h"


namespace argos {

	/****************************************/
	/****************************************/

    AutoMoDeBehaviourGoAwayFromPatch::AutoMoDeBehaviourGoAwayFromPatch() {
        m_strLabel = "GoAwayFromPatch";
	}

	/****************************************/
	/****************************************/

    AutoMoDeBehaviourGoAwayFromPatch::AutoMoDeBehaviourGoAwayFromPatch(AutoMoDeBehaviourGoAwayFromPatch* pc_behaviour) {
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

    AutoMoDeBehaviourGoAwayFromPatch::~AutoMoDeBehaviourGoAwayFromPatch() {}

	/****************************************/
	/****************************************/

    AutoMoDeBehaviourGoAwayFromPatch* AutoMoDeBehaviourGoAwayFromPatch::Clone() {
        return new AutoMoDeBehaviourGoAwayFromPatch(this);
	}

	/****************************************/
	/****************************************/

    void AutoMoDeBehaviourGoAwayFromPatch::ControlStep() {
	    CVector2 sRabVector(0,CRadians::ZERO);
	    CVector2 sProxVector(0,CRadians::ZERO);
	    CVector2 sResultVector(0,CRadians::ZERO);
	    CCI_EPuckRangeAndBearingSensor::SReceivedPacket cRabReading = m_pcRobotDAO->GetAttractionVectorToPatch(m_unRepulsionParameter, m_unColor);
	    
	    if (cRabReading.Range > 0.0f) {
		    sRabVector = CVector2(cRabReading.Range, cRabReading.Bearing);
	    }

	    sProxVector = CVector2(m_pcRobotDAO->GetRABReading().Value, m_pcRobotDAO->GetRABReading().Angle);
	    sResultVector = -m_unRepulsionParameter*sRabVector - 5*sProxVector;

	    if (sResultVector.Length() < 0.1) {
		    sResultVector = CVector2(1, CRadians::ZERO);
	    }

	    m_pcRobotDAO->SetWheelsVelocity(ComputeWheelsVelocityFromVector(sResultVector));

	    m_bLocked = false;
	}

	/****************************************/
	/****************************************/

    void AutoMoDeBehaviourGoAwayFromPatch::Init() {
        std::map<std::string, Real>::iterator it = m_mapParameters.find("rep");
		if (it != m_mapParameters.end()) {
			m_unRepulsionParameter = it->second;
		} else {
			LOGERR << "[FATAL] Missing parameter for the following behaviour:" << m_strLabel << std::endl;
			THROW_ARGOSEXCEPTION("Missing Parameter");
		}
        it = m_mapParameters.find("gaf");
        if (it != m_mapParameters.end()) {
            m_unColor = it->second;
        } else {
            LOGERR << "[FATAL] Missing parameter for the following behaviour:" << m_strLabel << std::endl;
            THROW_ARGOSEXCEPTION("Missing Parameter");
        }
	}

	/****************************************/
	/****************************************/

    void AutoMoDeBehaviourGoAwayFromPatch::Reset() {
		m_bOperational = false;
		ResumeStep();
	}

	/****************************************/
	/****************************************/

    void AutoMoDeBehaviourGoAwayFromPatch::ResumeStep() {
		m_bOperational = true;
	}
}
