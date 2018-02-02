/*
 * @file <src/core/bt/SequenceStar.cpp>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * @package ARGoS3-AutoMoDe-BT
 *
 * @license MIT License
 */

#include "SequenceStar.h"

namespace argos {

  /****************************************/
  /****************************************/

	SequenceStar::SequenceStar() {
    m_strLabel = "SequenceStar";
    m_unIndexRunningChild = 0;
  }

  /****************************************/
	/****************************************/

  SequenceStar::~SequenceStar() {}

  /****************************************/
  /****************************************/


	SequenceStar::SequenceStar(SequenceStar* pc_behaviour) {
	}

	/****************************************/
	/****************************************/

	SequenceStar* SequenceStar::Clone() {
		return new SequenceStar(this);
	}

  /****************************************/
  /****************************************/

  const std::string SequenceStar::GetDOTDescription() {
    return "SequenceStar";
  }

	/****************************************/
  /****************************************/

  Node::ReturnState SequenceStar::Tick() {
    return SUCCESS;
  }

  /****************************************/
  /****************************************/

  void SequenceStar::AddChildNode(Node* pc_new_child_node) {
    m_vecChilds.push_back(pc_new_child_node);
  }

}
