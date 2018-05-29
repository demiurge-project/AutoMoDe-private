/*
 * @file <src/core/bt/Action.cpp>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * @package ARGoS3-AutoMoDe-BT
 *
 * @license MIT License
 */

#include "Action.h"

namespace argos {

  /****************************************/
  /****************************************/

	Action::Action() {
    m_strLabel = "Action";
  }

  /****************************************/
	/****************************************/

  Action::~Action() {
		delete m_pcBehaviour;
	}

  /****************************************/
  /****************************************/


	Action::Action(Action* pc_action) {
		m_pcBehaviour = pc_action->GetBehaviour()->Clone();
	}

	/****************************************/
	/****************************************/

	Node::ReturnState Action::Tick() {
		Node::ReturnState eCurrentState;
		if (m_pcBehaviour->Failed()) {
			eCurrentState = Node::FAILURE;
		} else if (m_pcBehaviour->Succeeded()) {
			eCurrentState = Node::SUCCESS;
		} else {
			m_pcBehaviour->ControlStep();
			eCurrentState = Node::RUNNING;
		}
		return eCurrentState;
	}

	/****************************************/
	/****************************************/

	void Action::Reset() {}

	/****************************************/
	/****************************************/

	Action* Action::Clone() {
		return new Action(this);
	}

  /****************************************/
  /****************************************/

  void Action::FillDOTDescription(std::ostringstream& ss_dot_string){
		// Creation of graphical nodes
		// for (UInt8 i = 0; i < m_vecConditions.size(); i++) {
		// 	ss_dot_string << "cond" << m_unBranchId << "x" << i << " [shape=diamond;label=\"" << m_vecConditions.at(i)->GetDOTDescription() << "\";];";
		// }
		// for (UInt8 i = 0; i < m_vecActions.size(); i++) {
		// 	ss_dot_string << "act" << m_unBranchId << "x" << i << " [shape=circle;label=\"" << m_vecActions.at(i)->GetDOTDescription() << "\";];";
		// }
		//
		// // Linking nodes
		// for (UInt8 i = 0; i < m_vecConditions.size(); i++) {
		// 	ss_dot_string << m_strLabel << m_unBranchId << " -> " << "cond" << m_unBranchId << "x" << i <<  ";";
		// }
		// for (UInt8 i = 0; i < m_vecActions.size(); i++) {
		// 	ss_dot_string << m_strLabel << m_unBranchId << " -> " << "act" << m_unBranchId << "x" << i <<  ";";
		// }
  }

	/****************************************/
	/****************************************/

	std::string Action::GetLabel() {
	 return m_strLabel;
	}

	/****************************************/
	/****************************************/

	void Action::AddChildNode(Node* pc_new_child_node) {
		THROW_ARGOSEXCEPTION("An Action node should not have children");
	}

	/****************************************/
	/****************************************/

	void Action::ShareRobotDAO(EpuckDAO* pc_robot_dao) {
		m_pcBehaviour->SetRobotDAO(pc_robot_dao);
	}

	/****************************************/
	/****************************************/

	void Action::SetBehaviour(AutoMoDeBehaviour* pc_behaviour) {
		m_pcBehaviour = pc_behaviour;
	}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviour* Action::GetBehaviour() {
		return m_pcBehaviour;
	}
}
