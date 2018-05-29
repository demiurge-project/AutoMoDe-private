/*
 * @file <src/core/AutoMoDeBehaviorTreeBuilder.cpp>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * @package ARGoS3-AutoMoDe
 *
 * @license MIT License
 */

#include "AutoMoDeBehaviorTreeBuilder.h"

namespace argos {

	/****************************************/
	/****************************************/

	AutoMoDeBehaviorTreeBuilder::AutoMoDeBehaviorTreeBuilder() {}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviorTreeBuilder::~AutoMoDeBehaviorTreeBuilder() {
		delete cBehaviorTree;
	}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviorTree* AutoMoDeBehaviorTreeBuilder::BuildBehaviorTree(const std::string& str_bt_config) {
		std::istringstream iss(str_bt_config);
		std::vector<std::string> tokens;
		copy(std::istream_iterator<std::string>(iss),
			std::istream_iterator<std::string>(),
			std::back_inserter(tokens));
		return BuildBehaviorTree(tokens);
	}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviorTree* AutoMoDeBehaviorTreeBuilder::BuildBehaviorTree(std::vector<std::string>& vec_bt_config) {
		cBehaviorTree = new AutoMoDeBehaviorTree();
		Node* cRootNode;

		// Get root node type and create root node.
		std::vector<std::string>::iterator it;
		try {
			it = std::find(vec_bt_config.begin(), vec_bt_config.end(), "--rootnode");
			UInt8 unRootNodeType = atoi((*(it+1)).c_str());

			// Create root node.
			switch(unRootNodeType) {
				case 0:
					cRootNode = new SequenceStar();
					break;
			}
		}
		catch (std::exception e) {
			THROW_ARGOSEXCEPTION("[Error while parsing]: Could not instanciate Root Node");
		}

		// Identifying root's childs
		UInt8 unNumberChilds;
		try {
			it = std::find(vec_bt_config.begin(), vec_bt_config.end(), "--nchildsroot");
			unNumberChilds = atoi((*(it+1)).c_str());
			std::vector<std::string>::iterator first_child;
			std::vector<std::string>::iterator second_child;
			for (UInt32 i = 0; i < unNumberChilds; i++) {
				std::ostringstream oss;
				oss << "--n" << i;
				first_child = std::find(vec_bt_config.begin(), vec_bt_config.end(), oss.str());
				if (i+1 < unNumberChilds) {
					std::ostringstream oss;
					oss << "--n" << i+1;
					second_child = std::find(vec_bt_config.begin(), vec_bt_config.end(), oss.str());
				} else {
					second_child = vec_bt_config.end();
				}
				std::vector<std::string> vecChildConfig(first_child, second_child);
				// Debug
				// std::cout << "Child " << i << std::endl;
				// for (it=vecChildConfig.begin(); it!=vecChildConfig.end(); it++) {
				// 	std::cout << *(it) << std::endl;
				// }
				HandleChild(cRootNode, vecChildConfig);
			}
		}
		catch (std::exception e) {
			THROW_ARGOSEXCEPTION("[Error while parsing]: Error while parsing Root's childs");
		}

		cBehaviorTree->SetRootNode(cRootNode);
		return cBehaviorTree;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviorTreeBuilder::HandleChild(Node* pc_parent_node, std::vector<std::string>& vec_bt_child_config) {
		Node* cChildNode;

		std::vector<std::string>::iterator it;
		UInt8 unBranchIndex =  atoi((*vec_bt_child_config.begin()).substr(3,4).c_str());
		UInt8 unChildType = atoi((*(vec_bt_child_config.begin()+1)).c_str());

		// Selector case
		if (unChildType == 0) {
			cChildNode = new Selector();
			cChildNode->SetBranchId(unBranchIndex);

			try {
				// Conditions
				std::ostringstream oss;
				oss << "--nc" << unBranchIndex;
				it = std::find(vec_bt_child_config.begin(), vec_bt_child_config.end(), oss.str());
				UInt8 unNbrConditions = atoi((*(it+1)).c_str());
				std::vector<std::string>::iterator first_condition;
				std::vector<std::string>::iterator second_condition;
				for (UInt8 unConditionIndex = 0; unConditionIndex < unNbrConditions; unConditionIndex++) {
					std::ostringstream ssConditionVariable;
					ssConditionVariable << "--c" << unBranchIndex << "x" << unConditionIndex ;
					first_condition = std::find(vec_bt_child_config.begin(), vec_bt_child_config.end(), ssConditionVariable.str());
					if (unConditionIndex+1 < unNbrConditions) {
						std::ostringstream ssNextConditionVariable;
						ssNextConditionVariable << "--c" << unBranchIndex << "x" << unConditionIndex+1;
						second_condition = std::find(vec_bt_child_config.begin(), vec_bt_child_config.end(), ssNextConditionVariable.str());
					} else {
						std::ostringstream ssActionVariable;
						ssActionVariable << "--a" << unBranchIndex;
						second_condition = std::find(vec_bt_child_config.begin(), vec_bt_child_config.end(), ssActionVariable.str());
					}
					std::vector<std::string> vecConditionConfig(first_condition, second_condition);
					// Debug
					// std::cout << "\tCond " << unConditionIndex << std::endl;
					// for (it=vecConditionConfig.begin(); it!=vecConditionConfig.end(); it++) {
					//  	std::cout << "\t" << *(it) << std::endl;
					// }
					HandleCondition(cChildNode, vecConditionConfig, unBranchIndex, unConditionIndex);
				}

				// Action
				std::ostringstream ssActionVariable;
				ssActionVariable << "--a" << unBranchIndex;
				std::vector<std::string>::iterator action_start;
				std::vector<std::string>::iterator action_end;
				action_start = std::find(vec_bt_child_config.begin(), vec_bt_child_config.end(), ssActionVariable.str());
				action_end = vec_bt_child_config.end();
				std::vector<std::string> vecActionConfig(action_start, action_end);
				// Debug
				// std::cout << "\tAction " << std::endl;
				// for (it=vecActionConfig.begin(); it!=vecActionConfig.end(); it++) {
				// 	std::cout << "\t" << *(it) << std::endl;
				// }
				HandleAction(cChildNode, vecActionConfig);

				pc_parent_node->AddChildNode(cChildNode);
			} catch(std::exception e) {
				LOGERR << "[Error while parsing]: Error in Selector node" << std::endl;
				THROW_ARGOSEXCEPTION("");
			}

		} else { // Unknown case: throw exception
 			LOGERR << "[Error while parsing]: Node's type unidentified" << std::endl;
			THROW_ARGOSEXCEPTION("");
		}
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviorTreeBuilder::HandleAction(Node* pc_parent_node, std::vector<std::string>& vec_bt_action_config) {
		Action* cActionNode = new Action();
		AutoMoDeBehaviour* cNewBehaviour;
		std::vector<std::string>::iterator it;
		// Extraction of the index of the action
		UInt8 unActionIndex =  atoi((*vec_bt_action_config.begin()).substr(3,4).c_str());
		// Extraction of the identifier of the behaviour
		UInt8 unBehaviourIdentifier =  atoi((*(vec_bt_action_config.begin()+1)).c_str());

		// Creation of the Behaviour object
		switch(unBehaviourIdentifier) {
			case 0:
				//cNewBehaviour = new AutoMoDeBehaviourExploration();
				break;
			case 1:
				cNewBehaviour = new AutoMoDeBehaviourStop();
				break;
			case 2:
				cNewBehaviour = new AutoMoDeBehaviourPhototaxis();
				break;
			case 3:
				cNewBehaviour = new AutoMoDeBehaviourAntiPhototaxis();
				break;
			case 4:
				cNewBehaviour = new AutoMoDeBehaviourAttraction();
				break;
			case 5:
				cNewBehaviour = new AutoMoDeBehaviourRepulsion();
				break;
		}
		cNewBehaviour->SetIndex(unActionIndex);
		cNewBehaviour->SetIdentifier(unBehaviourIdentifier);

		// Checking for parameters
		std::string vecPossibleParameters[] = {"rwm", "att", "rep", "b"};
		UInt8 unNumberPossibleParameters = sizeof(vecPossibleParameters) / sizeof(vecPossibleParameters[0]);
		for (UInt8 i = 0; i < unNumberPossibleParameters; i++) {
			std::string strCurrentParameter = vecPossibleParameters[i];
			std::ostringstream oss;
			oss << "--" << strCurrentParameter << unActionIndex;
			it = std::find(vec_bt_action_config.begin(), vec_bt_action_config.end(), oss.str());
			if (it != vec_bt_action_config.end()) {
				Real fCurrentParameterValue = strtod((*(it+1)).c_str(), NULL);
				cNewBehaviour->AddParameter(strCurrentParameter, fCurrentParameterValue);
			}
		}
		cNewBehaviour->Init();
		cActionNode->SetBehaviour(cNewBehaviour);
		// Add the constructed Behaviour to the FSM
		pc_parent_node->AddChildNode(cActionNode);
	}

	/****************************************/
	/****************************************/

void AutoMoDeBehaviorTreeBuilder::HandleCondition(Node* pc_parent_node, std::vector<std::string>& vec_bt_condition_config, const UInt32& un_branch_index, const UInt32& un_condition_index){
		Condition* cConditionNode = new Condition();
		AutoMoDeCondition* cNewCondition;

		// Extract Condition identifier
		UInt32 unConditionIdentifier = atoi((*(vec_bt_condition_config.begin()+1)).c_str());

		switch(unConditionIdentifier) {
			case 0:
				cNewCondition = new AutoMoDeConditionBlackFloor();
				break;
			case 1:
				cNewCondition = new AutoMoDeConditionGrayFloor();
				break;
			case 2:
				cNewCondition = new AutoMoDeConditionWhiteFloor();
				break;
			case 3:
				cNewCondition = new AutoMoDeConditionNeighborsCount();
				break;
			case 4:
				cNewCondition = new AutoMoDeConditionInvertedNeighborsCount();
				break;
			case 5:
				cNewCondition = new AutoMoDeConditionFixedProbability();
				break;
		}

		cNewCondition->SetOriginAndExtremity(0, 0);  // No need of origin and extremity in cas of Behavior Trees. Set them to random value.
		cNewCondition->SetIndex(0); // Same here, no need of index.
		cNewCondition->SetIdentifier(unConditionIdentifier);


		// Checking for parameters
		std::vector<std::string>::iterator it;
		std::string vecPossibleParameters[] = {"p", "w"};
		UInt8 unNumberPossibleParameters = sizeof(vecPossibleParameters) / sizeof(vecPossibleParameters[0]);
		for (UInt8 i = 0; i < unNumberPossibleParameters; i++) {
			std::string strCurrentParameter = vecPossibleParameters[i];
			std::stringstream ssParamVariable;
			ssParamVariable << "--" << strCurrentParameter << un_branch_index << "x" << un_condition_index;
			it = std::find(vec_bt_condition_config.begin(), vec_bt_condition_config.end(), ssParamVariable.str());
			if (it != vec_bt_condition_config.end()) {
				Real fCurrentParameterValue = strtod((*(it+1)).c_str(), NULL);
				cNewCondition->AddParameter(strCurrentParameter, fCurrentParameterValue);
			}
		}
		cNewCondition->Init();

		cConditionNode->SetCondition(cNewCondition);
		pc_parent_node->AddChildNode(cConditionNode);
	}
}
