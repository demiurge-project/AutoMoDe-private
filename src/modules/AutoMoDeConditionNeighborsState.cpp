/**
  * @file <src/modules/AutoMoDeConditionNeighborsState.cpp>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

 #include "AutoMoDeConditionNeighborsState.h"

 namespace argos {

	/****************************************/
	/****************************************/

    AutoMoDeConditionNeighborsState::AutoMoDeConditionNeighborsState() {
        m_strLabel = "NeighborsState";
	}

	/****************************************/
	/****************************************/

    AutoMoDeConditionNeighborsState::~AutoMoDeConditionNeighborsState() {}

	/****************************************/
	/****************************************/

    AutoMoDeConditionNeighborsState::AutoMoDeConditionNeighborsState(AutoMoDeConditionNeighborsState* pc_condition) {
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

    AutoMoDeConditionNeighborsState* AutoMoDeConditionNeighborsState::Clone() {
        return new AutoMoDeConditionNeighborsState(this);
	}

	/****************************************/
	/****************************************/

    bool AutoMoDeConditionNeighborsState::Verify() {

        UInt32 unNumberNeighbors = m_pcRobotDAO->GetNumberOfRobotsInState(1);
        Real fProbability = (1/(1 + exp(10.0f * ((int)m_unParameterXi - (int)unNumberNeighbors))));

        //std::cout << "numRobParam: " << m_unParameterXi << " Vecinos: " << unNumberNeighbors << "\n";


		return EvaluateBernoulliProbability(fProbability);
	}

	/****************************************/
	/****************************************/

    void AutoMoDeConditionNeighborsState::Reset() {

	}

	/****************************************/
	/****************************************/

    void AutoMoDeConditionNeighborsState::Init() {
		std::map<std::string, Real>::iterator itXi = m_mapParameters.find("p");
        //std::map<std::string, Real>::iterator itS = m_mapParameters.find("s");

        if ((itXi != m_mapParameters.end())) {
            //m_fParameterS = itS->second;
			m_unParameterXi = itXi->second;
		} else {
			LOGERR << "[FATAL] Missing parameter for the following condition:" << m_strLabel << std::endl;
			THROW_ARGOSEXCEPTION("Missing Parameter");
		}
	}

 }
