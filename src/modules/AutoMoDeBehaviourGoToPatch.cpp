/**
  * @file <src/modules/AutoMoDeBehaviourGoToPatch.cpp>
  *
  * @author Jeanne Szpirer - <jszpirer@ulb.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#include "AutoMoDeBehaviourGoToPatch.h"


namespace argos {

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourGoToPatch::AutoMoDeBehaviourGoToPatch() {
		m_strLabel = "GoToPatch";
	}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourGoToPatch::AutoMoDeBehaviourGoToPatch(AutoMoDeBehaviourGoToPatch* pc_behaviour) {
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

	AutoMoDeBehaviourGoToPatch::~AutoMoDeBehaviourGoToPatch() {}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourGoToPatch* AutoMoDeBehaviourGoToPatch::Clone() {
		return new AutoMoDeBehaviourGoToPatch(this);   // todo: check without *
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourGoToPatch::ControlStep() {
		CVector2 sRabVector(0,CRadians::ZERO);
		CVector2 sProxVector(0,CRadians::ZERO);
		CVector2 sResultVector(0,CRadians::ZERO);
		CCI_EPuckRangeAndBearingSensor::SReceivedPacket cRabReading = m_pcRobotDAO->GetAttractionVectorToPatch(m_unAttractionParameter, m_unColorParameter);

		if (cRabReading.Range > 0.0f) {
			sRabVector = CVector2(cRabReading.Range, cRabReading.Bearing);
		}

		sProxVector = CVector2(m_pcRobotDAO->GetRABReading().Value, m_pcRobotDAO->GetRABReading().Angle);
		sResultVector = sRabVector - 6*sProxVector;

		if (sResultVector.Length() < 0.1) {
			sResultVector = CVector2(1, CRadians::ZERO);
		}

		m_pcRobotDAO->SetWheelsVelocity(ComputeWheelsVelocityFromVector(sResultVector));

		m_bLocked = false;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourGoToPatch::Init() {
		std::map<std::string, Real>::iterator it = m_mapParameters.find("att");
		if (it != m_mapParameters.end()) {
			m_unAttractionParameter = it->second;
		} else {
			LOGERR << "[FATAL] Missing parameter for the following behaviour:" << m_strLabel << std::endl;
			THROW_ARGOSEXCEPTION("Missing Parameter");
		}

	it = m_mapParameters.find("got");
	if (it != m_mapParameters.end()) {
		m_unColorParameter = it->second;
	} else {
		LOGERR << "[FATAL] Missing parameter for the following behaviour:" << m_strLabel << std::endl;
		THROW_ARGOSEXCEPTION("Missing Parameter");
	}
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourGoToPatch::Reset() {
		m_bOperational = false;
		ResumeStep();
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourGoToPatch::ResumeStep() {
		m_bOperational = true;
	}
}
