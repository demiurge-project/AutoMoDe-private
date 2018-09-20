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
        CVector2 sCamVector(0,CRadians::ZERO);
        CVector2 sProxVector(0,CRadians::ZERO);
        CVector2 sResultVector(0,CRadians::ZERO);

        /* Compute the interaction vector */
        CCI_EPuckOmnidirectionalCameraSensor::SBlob cCamReading = m_pcRobotDAO->GetNeighborsDirection();
        sCamVector = CVector2(cCamReading.Distance, cCamReading.Angle);

        /* Compute the proximity vector */
        CCI_EPuckProximitySensor::SReading cProxReading = m_pcRobotDAO->GetProximityReading();
        sProxVector = CVector2(cProxReading.Value, cProxReading.Angle);

        /* if robot alone, go straight ahead */
        if (sCamVector.Length() < 0.1) {
            sCamVector = -CVector2(1, CRadians::ZERO);
        }

        /* Compute the result vector, prox is an order the magnitude bigger */
        sResultVector = -m_unRepulsionParameter*sCamVector - 5*sProxVector;

        /* Compute the velocity of the wheels */
        m_pcRobotDAO->SetWheelsVelocity(MILowLevelController(sResultVector, 1.0, 0.7));
        //m_pcRobotDAO->SetWheelsVelocity(ComputeWheelsVelocityFromVector(sResultVector));

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
