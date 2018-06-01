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
		CVector2 sLightVector(0,CRadians::ZERO);
		CCI_EPuckLightSensor::SReading cLightReading = m_pcRobotDAO->GetLightReading();

		sLightVector = CVector2(cLightReading.Value, cLightReading.Angle);

		// LOG << cLightReading.Value << " " << cLightReading.Angle << std::endl;
		// LOG << sLightVector << std::endl;

		if (sLightVector.Length() < 0.1) {
			sLightVector = CVector2(1.0, CRadians::ZERO);
		}

		m_pcRobotDAO->SetWheelsVelocity(ComputeWheelsVelocityFromVector(sLightVector));

		m_bLocked = false;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourPhototaxis::Init() {
		// Success probability
	  std::map<std::string, Real>::iterator it = m_mapParameters.find("p");
		if (it != m_mapParameters.end()) {
			m_fSuccessProbabilityParameter = it->second;
		} else {
			LOGERR << "[FATAL] Missing probability parameter for the following behaviour:" << m_strLabel << std::endl;
			THROW_ARGOSEXCEPTION("Missing Parameter");
		}
	}

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

	/****************************************/
	/****************************************/

	bool AutoMoDeBehaviourPhototaxis::Succeeded() {
		return EvaluateBernoulliProbability(m_fSuccessProbabilityParameter);
	}

	/****************************************/
	/****************************************/

	bool AutoMoDeBehaviourPhototaxis::Failed() {
		return (ObstacleInFront() || !LightPerceived());
	}
}
