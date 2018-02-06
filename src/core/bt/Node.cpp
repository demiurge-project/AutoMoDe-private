/*
 * @file <src/core/bt/Node.cpp>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * @package ARGoS3-AutoMoDe-BT
 *
 * @license MIT License
 */

#include "Node.h"

namespace argos {

  /****************************************/
  /****************************************/

	Node::~Node() {}

	/****************************************/
	/****************************************/

	UInt8 Node::GetBranchId(){
		return m_unBranchId;
	}

	/****************************************/
	/****************************************/

	void Node::SetBranchId(UInt8 un_branch_id) {
		m_unBranchId = un_branch_id;
	}

	/****************************************/
	/****************************************/

	std::string Node::GetDOTLabel() {
		return m_strDOTLabel;
	}

}
