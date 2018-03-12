/**
  * @file <src/modules/AutoMoDeBehaviourRepulsion.cpp>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#include "AutoMoDeBehaviourRepulsion.h"


namespace argos {

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourRepulsion::AutoMoDeBehaviourRepulsion() {
		m_strLabel = "Repulsion";
	}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourRepulsion::AutoMoDeBehaviourRepulsion(AutoMoDeBehaviourRepulsion* pc_behaviour) {
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

	AutoMoDeBehaviourRepulsion::~AutoMoDeBehaviourRepulsion() {}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourRepulsion* AutoMoDeBehaviourRepulsion::Clone() {
		return new AutoMoDeBehaviourRepulsion(this);
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourRepulsion::ControlStep() {
        CCI_EPuckVirtualCamrabSensor::TReadings sLastPackets = m_pcRobotDAO->GetCamRabMessages();

        CVector2 sCamRabVectorSum(0,CRadians::ZERO);
        CVector2 sProxVectorSum(0,CRadians::ZERO);
        CVector2 sResultVector(0,CRadians::ZERO);

        if (!sLastPackets.empty()) {
            for(size_t i = 0; i < sLastPackets.size(); ++i) {
                if ((sLastPackets[i]->Data[1] != (UInt8) m_pcRobotDAO->GetRobotIdentifier())) {

                    CRadians anguloR;

                    anguloR.FromValueInDegrees(sLastPackets[i]->Angle);

                    sCamRabVectorSum += CVector2(m_unRepulsionParameter / (sLastPackets[i]->Distance + 1),anguloR.SignedNormalize());
                }
            }
        }

        sProxVectorSum = SumProximityReadings(m_pcRobotDAO->GetProximityInput());
        sResultVector = -sCamRabVectorSum - 5*sProxVectorSum;

        if (sResultVector.Length() < 0.1) {
            sResultVector = CVector2(1, CRadians::ZERO);
        }

        m_pcRobotDAO->SetWheelsVelocity(MILowLevelController(sResultVector));

        m_bLocked = false;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourRepulsion::Init() {
		std::map<std::string, Real>::iterator it = m_mapParameters.find("rep");
		if (it != m_mapParameters.end()) {
			m_unRepulsionParameter = it->second;
		} else {
			LOGERR << "[FATAL] Missing parameter for the following behaviour:" << m_strLabel << std::endl;
			THROW_ARGOSEXCEPTION("Missing Parameter");
		}
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourRepulsion::Reset() {
		m_bOperational = false;
		ResumeStep();
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourRepulsion::ResumeStep() {
		m_bOperational = true;
	}
}
