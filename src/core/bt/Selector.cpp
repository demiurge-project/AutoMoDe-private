/*
 * @file <src/core/bt/Selector.cpp>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * @package ARGoS3-AutoMoDe-BT
 *
 * @license MIT License
 */

#include "Selector.h"

namespace argos {

  /****************************************/
  /****************************************/

	Selector::Selector() {
    m_strLabel = "Selector";
		m_strDOTLabel = "?";
  }

  /****************************************/
	/****************************************/

  Selector::~Selector() {}

  /****************************************/
  /****************************************/


	Selector::Selector(Selector* pc_behaviour) {
	}

	/****************************************/
	/****************************************/

	Selector* Selector::Clone() {
		return new Selector(this);
	}

  /****************************************/
  /****************************************/

  void Selector::FillDOTDescription(std::ostringstream& ss_dot_string){
		// Creation of graphical nodes
		for (UInt8 i = 0; i < m_vecConditions.size(); i++) {
			ss_dot_string << "cond" << m_unBranchId << "x" << i << " [shape=circle;label=\"" << m_vecConditions.at(i)->GetDOTDescription() << "\";color=lightgray;style=filled];";
		}
		for (UInt8 i = 0; i < m_vecActions.size(); i++) {
			ss_dot_string << "act" << m_unBranchId << "x" << i << " [shape=square;label=\"" << m_vecActions.at(i)->GetDOTDescription() << "\";color=gray;style=filled];";
		}

		// Linking nodes
		for (UInt8 i = 0; i < m_vecConditions.size(); i++) {
			ss_dot_string << m_strLabel << m_unBranchId << " -> " << "cond" << m_unBranchId << "x" << i <<  ";";
		}
		for (UInt8 i = 0; i < m_vecActions.size(); i++) {
			ss_dot_string << m_strLabel << m_unBranchId << " -> " << "act" << m_unBranchId << "x" << i <<  ";";
		}
  }

	std::string Selector::GetLabel() {
	 return m_strLabel;
	}

	/****************************************/
  /****************************************/

  Node::ReturnState Selector::Tick() {
    return SUCCESS;
  }

  /****************************************/
  /****************************************/

  void Selector::AddCondition(AutoMoDeCondition* pc_condition) {
    m_vecConditions.push_back(pc_condition);
  }

  /****************************************/
  /****************************************/

  void Selector::AddAction(AutoMoDeBehaviour* pc_action) {
    m_vecActions.push_back(pc_action);
  }

	/****************************************/
	/****************************************/

	void Selector::AddChildNode(Node* pc_new_child_node) {
		THROW_ARGOSEXCEPTION("As of now, a Selector node should not have a node different from an Action or a Condition as child")
	}

}
