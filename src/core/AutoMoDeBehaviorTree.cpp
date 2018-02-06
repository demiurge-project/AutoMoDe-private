 /*
  * @file <src/core/AutoMoDeBehaviorTree.cpp>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#include "AutoMoDeBehaviorTree.h"

namespace argos {

	/****************************************/
	/****************************************/

	AutoMoDeBehaviorTree::AutoMoDeBehaviorTree() {
	}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviorTree::~AutoMoDeBehaviorTree() {

	}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviorTree::AutoMoDeBehaviorTree(const AutoMoDeBehaviorTree* pc_fsm) {

	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviorTree::ControlStep(){

	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviorTree::SetRootNode(Node* pc_root_node) {
		m_pcRootNode = pc_root_node;
	}

	/****************************************/
	/****************************************/

	const std::string AutoMoDeBehaviorTree::GetReadableFormat() {
		std::ostringstream ssUrl;
		ssUrl << "http://chart.googleapis.com/chart?cht=gv:dot&chl=digraph G{" ;
		m_pcRootNode->FillDOTDescription(ssUrl);
		ssUrl << "}" ;
		std::string strUrl = ssUrl.str();
		std::replace(strUrl.begin(), strUrl.end(), ' ', '+');
		return strUrl;
	}
}
