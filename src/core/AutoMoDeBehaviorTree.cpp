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

	const std::string AutoMoDeBehaviorTree::GetReadableFormat() {
		std::stringstream ssUrl;
		ssUrl << "http://chart.googleapis.com/chart?cht=gv:dot&chl=digraph finite_state_machine{rankir=LR;" ;

		ssUrl << "}" ;
		std::string strUrl = ssUrl.str();
		std::replace(strUrl.begin(), strUrl.end(), ' ', '+');
		return strUrl;
	}
}
