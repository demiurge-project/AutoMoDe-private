/**
  * @file <src/modules/AutoMoDeConditionColor.cpp>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

 #include "AutoMoDeConditionColor.h"

 namespace argos {

  /****************************************/
  /****************************************/

    AutoMoDeConditionColor::AutoMoDeConditionColor() {
        m_strLabel = "Color";
	}

  /****************************************/
  /****************************************/

    AutoMoDeConditionColor::~AutoMoDeConditionColor() {}

  /****************************************/
  /****************************************/

    AutoMoDeConditionColor::AutoMoDeConditionColor(AutoMoDeConditionColor* pc_condition) {
		m_strLabel = pc_condition->GetLabel();
		m_unIndex = pc_condition->GetIndex();
		m_unIdentifier = pc_condition->GetIndex();
		m_unFromBehaviourIndex = pc_condition->GetOrigin();
		m_unToBehaviourIndex = pc_condition->GetExtremity();
		m_mapParameters = pc_condition->GetParameters();
    Init();
	}

  /****************************************/
  /****************************************/

  void AutoMoDeConditionColor::Init() {
      std::map<std::string, Real>::iterator it = m_mapParameters.find("e");
    if (it != m_mapParameters.end()) {
      m_fParameterEta = it->second;
    } else {
      LOGERR << "[FATAL] Missing parameter for the following condition:" << m_strLabel << std::endl;
      THROW_ARGOSEXCEPTION("Missing Parameter");
	  }
    it = m_mapParameters.find("i");
    if (it != m_mapParameters.end()) {
      m_unParameterXi = it->second;
    } else {
      LOGERR << "[FATAL] Missing parameter for the following condition:" << m_strLabel << std::endl;
      THROW_ARGOSEXCEPTION("Missing Parameter");
      }
    it = m_mapParameters.find("l");
    if (it != m_mapParameters.end()) {
      m_cColorParameter = GetColorParameter(it->second);
    } else {
      LOGERR << "[FATAL] Missing parameter for the following condition:" << m_strLabel << std::endl;
      THROW_ARGOSEXCEPTION("Missing Parameter");
      }
    it = m_mapParameters.find("d");
    if (it != m_mapParameters.end()) {
      m_fDistance = it->second;
    } else {
      LOGERR << "[FATAL] Missing parameter for the following condition:" << m_strLabel << std::endl;
      THROW_ARGOSEXCEPTION("Missing Parameter");
      }
  }

  /****************************************/
  /****************************************/

    AutoMoDeConditionColor* AutoMoDeConditionColor::Clone() {
        return new AutoMoDeConditionColor(this);
	}

  /****************************************/
  /****************************************/

    bool AutoMoDeConditionColor::Verify() {
        CCI_EPuckOmnidirectionalCameraSensor::SReadings sReadings = m_pcRobotDAO->GetCameraInput();
        CCI_EPuckOmnidirectionalCameraSensor::TBlobList::iterator it;
        SInt32 sBlobCounter = 0;

        LOG << "ROBOT:" << m_pcRobotDAO->GetRobotIdentifier() << "---------" << std::endl;

        for (it = sReadings.BlobList.begin(); it != sReadings.BlobList.end(); it++) {

            if ((*it)->Color == m_cColorParameter) {
                if ((*it)->Distance <= m_fDistance){
                    sBlobCounter++;
                }
            }
        }

        if (sBlobCounter > 0){
            Real fProbability = (1/(1 + exp(m_fParameterEta * ((int)m_unParameterXi - sBlobCounter))));
            LOG << "Eta: " << m_fParameterEta << std::endl;
            LOG << "Xi: " << m_unParameterXi << std::endl;
            LOG << "Blobs: " << sBlobCounter << std::endl;
            LOG << "Probability: " << fProbability << std::endl;
            return EvaluateBernoulliProbability(fProbability);
        }

    return false;
 }

  /****************************************/
  /****************************************/

    void AutoMoDeConditionColor::Reset() {
    Init();
  }

 }
