/*
 * @file <src/core/bt/Condition.cpp>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * @package ARGoS3-AutoMoDe-BT
 *
 * @license MIT License
 */

#include "Condition.h"

namespace argos {

  /****************************************/
  /****************************************/

	Condition::Condition() {
    m_strLabel = "Condition";
  }

  /****************************************/
	/****************************************/

  Condition::~Condition() {
		delete m_pcCondition;
	}

  /****************************************/
  /****************************************/


	Condition::Condition(Condition* pc_action) {
		m_pcCondition = pc_action->GetCondition()->Clone();
	}

	/****************************************/
	/****************************************/

	Node::ReturnState Condition::Tick() {
		Node::ReturnState eCurrentState;
		if (m_pcCondition->Verify()) {
			eCurrentState = Node::SUCCESS;
		} else {
			eCurrentState = Node::FAILURE;
		}
		return eCurrentState;
	}

	/****************************************/
	/****************************************/

	void Condition::Reset() {}

	/****************************************/
	/****************************************/

	Condition* Condition::Clone() {
		return new Condition(this);
	}

  /****************************************/
  /****************************************/

  void Condition::FillDOTDescription(std::ostringstream& ss_dot_string){
		// Creation of graphical nodes
		// for (UInt8 i = 0; i < m_vecConditions.size(); i++) {
		// 	ss_dot_string << "cond" << m_unBranchId << "x" << i << " [shape=diamond;label=\"" << m_vecConditions.at(i)->GetDOTDescription() << "\";];";
		// }
		// for (UInt8 i = 0; i < m_vecConditions.size(); i++) {
		// 	ss_dot_string << "act" << m_unBranchId << "x" << i << " [shape=circle;label=\"" << m_vecConditions.at(i)->GetDOTDescription() << "\";];";
		// }
		//
		// // Linking nodes
		// for (UInt8 i = 0; i < m_vecConditions.size(); i++) {
		// 	ss_dot_string << m_strLabel << m_unBranchId << " -> " << "cond" << m_unBranchId << "x" << i <<  ";";
		// }
		// for (UInt8 i = 0; i < m_vecConditions.size(); i++) {
		// 	ss_dot_string << m_strLabel << m_unBranchId << " -> " << "act" << m_unBranchId << "x" << i <<  ";";
		// }
  }

	/****************************************/
	/****************************************/

	std::string Condition::GetLabel() {
	 return m_strLabel;
	}

	/****************************************/
	/****************************************/

	void Condition::AddChildNode(Node* pc_new_child_node) {
		THROW_ARGOSEXCEPTION("An Condition node should not have children");
	}

	/****************************************/
	/****************************************/

	void Condition::ShareRobotDAO(EpuckDAO* pc_robot_dao) {
		m_pcCondition->SetRobotDAO(pc_robot_dao);
	}

	/****************************************/
	/****************************************/

	AutoMoDeCondition* Condition::GetCondition() {
		return m_pcCondition;
	}
}
