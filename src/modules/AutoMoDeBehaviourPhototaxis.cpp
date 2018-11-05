/**
  * @file <src/modules/AutoMoDeBehaviourPhototaxis.cpp>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#include "AutoMoDeBehaviourPhototaxis.h"


namespace argos {

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourPhototaxis::AutoMoDeBehaviourPhototaxis() {
		m_strLabel = "Phototaxis";
	}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourPhototaxis::AutoMoDeBehaviourPhototaxis(AutoMoDeBehaviourPhototaxis* pc_behaviour) {
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

	AutoMoDeBehaviourPhototaxis::~AutoMoDeBehaviourPhototaxis() {}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourPhototaxis* AutoMoDeBehaviourPhototaxis::Clone() {
		return new AutoMoDeBehaviourPhototaxis(this);
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourPhototaxis::ControlStep() {

        m_pcRobotDAO->SetLEDsColor(CColor::BLACK);

        CVector2 sResultVector(0,CRadians::ZERO);
        CVector2 sLightVector(0,CRadians::ZERO);
        CVector2 sProxVector(0,CRadians::ZERO);

        /* Compute the light vector */
        CCI_EPuckLightSensor::SReading cLightReading = m_pcRobotDAO->GetLightReading();
        sLightVector = CVector2(cLightReading.Value, cLightReading.Angle);

        /* Compute the proximity vector */
        CCI_EPuckProximitySensor::SReading cProxReading = m_pcRobotDAO->GetProximityReading();
        sProxVector = CVector2(cProxReading.Value, cProxReading.Angle);

        /* if robot alone, go straight ahead */
        if (sLightVector.Length() < 0.1) {
            sLightVector = CVector2(1, CRadians::ZERO);
        }

        /* Compute the result vector */
        sResultVector = sLightVector - 5*sProxVector;

        /* Compute the velocity of the wheels */
        m_pcRobotDAO->SetWheelsVelocity(MILowLevelController(sResultVector, 1.0, 0.7));
        //m_pcRobotDAO->SetWheelsVelocity(ComputeWheelsVelocityFromVector(sResultVector));

        m_bLocked = false;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourPhototaxis::Init() {}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourPhototaxis::Reset() {
		m_bOperational = false;
		ResumeStep();
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourPhototaxis::ResumeStep() {
		m_bOperational = true;
	}
}
