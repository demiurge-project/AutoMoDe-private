/**
  * @file <src/modules/AutoMoDeBehaviourRepulsion.cpp>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#include "AutoMoDeBehaviourRepulsionColor.h"


namespace argos {

	/****************************************/
	/****************************************/

    AutoMoDeBehaviourRepulsionColor::AutoMoDeBehaviourRepulsionColor() {
        m_strLabel = "RepulsionColor";
	}

	/****************************************/
	/****************************************/

    AutoMoDeBehaviourRepulsionColor::AutoMoDeBehaviourRepulsionColor(AutoMoDeBehaviourRepulsionColor* pc_behaviour) {
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

    AutoMoDeBehaviourRepulsionColor::~AutoMoDeBehaviourRepulsionColor() {}

	/****************************************/
	/****************************************/

    AutoMoDeBehaviourRepulsionColor* AutoMoDeBehaviourRepulsionColor::Clone() {
        return new AutoMoDeBehaviourRepulsionColor(this);
	}

	/****************************************/
	/****************************************/

    void AutoMoDeBehaviourRepulsionColor::ControlStep() {
        CCI_EPuckOmnidirectionalCameraSensor::SReadings sReadings = m_pcRobotDAO->GetCameraInput();
        CCI_EPuckOmnidirectionalCameraSensor::TBlobList::iterator it;
        CVector2 sColVectorSum(0,CRadians::ZERO);
		CVector2 sProxVectorSum(0,CRadians::ZERO);
		CVector2 sResultVector(0,CRadians::ZERO);

        for (it = sReadings.BlobList.begin(); it != sReadings.BlobList.end(); it++) {
            if ((*it)->Color == m_cColorReceiverParameter) {
                sColVectorSum += CVector2(m_unRepulsionParameter * (((*it)->Distance)/50),
                                          (*it)->Angle);  // 172.5 -> max(DISTANCE)
            }
            // TODO Check sColVectorSum function
		}

		sProxVectorSum = SumProximityReadings(m_pcRobotDAO->GetProximityInput());
        sResultVector = -sColVectorSum - 5*sProxVectorSum;

		if (sResultVector.Length() < 0.1) {
			sResultVector = CVector2(1, CRadians::ZERO);
		}

		m_pcRobotDAO->SetWheelsVelocity(ComputeWheelsVelocityFromVector(sResultVector));
        m_pcRobotDAO->SetLEDsColor(m_cColorEmiterParameter);

		m_bLocked = false;
	}

	/****************************************/
	/****************************************/

    void AutoMoDeBehaviourRepulsionColor::Init() {
		std::map<std::string, Real>::iterator it = m_mapParameters.find("rep");
		if (it != m_mapParameters.end()) {
			m_unRepulsionParameter = it->second;
		} else {
			LOGERR << "[FATAL] Missing parameter for the following behaviour:" << m_strLabel << std::endl;
			THROW_ARGOSEXCEPTION("Missing Parameter");
		}
        it = m_mapParameters.find("cle");
        if (it != m_mapParameters.end()) {
            m_cColorEmiterParameter = GetColorParameter(it->second);
        } else {
            LOGERR << "[FATAL] Missing parameter for the following behaviour:" << m_strLabel << std::endl;
            THROW_ARGOSEXCEPTION("Missing Parameter");
        }
        it = m_mapParameters.find("clr");
        if (it != m_mapParameters.end()) {
            m_cColorReceiverParameter = GetColorParameter(it->second);
        } else {
            LOGERR << "[FATAL] Missing parameter for the following behaviour:" << m_strLabel << std::endl;
            THROW_ARGOSEXCEPTION("Missing Parameter");
        }
	}

	/****************************************/
	/****************************************/

    void AutoMoDeBehaviourRepulsionColor::Reset() {
		m_bOperational = false;
		ResumeStep();
	}

	/****************************************/
	/****************************************/

    void AutoMoDeBehaviourRepulsionColor::ResumeStep() {
		m_bOperational = true;
	}
}
