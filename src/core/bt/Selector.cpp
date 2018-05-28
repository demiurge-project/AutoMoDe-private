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

  Selector::~Selector() {
		for (UInt8 i = 0; i < m_vecChilds.size(); i++) {
			delete m_vecChilds.at(i);
		}
	}

  /****************************************/
  /****************************************/


	Selector::Selector(Selector* pc_selector) {
		std::vector<Node*> vecChildNodes = pc_selector->GetChildNodes();
		m_vecChilds.clear();
		for (std::vector<Node*>::iterator it = vecChildNodes.begin(); it != vecChildNodes.end(); ++it) {
			m_vecChilds.push_back((*it)->Clone());
		}
	}

	/****************************************/
	/****************************************/

	Node::ReturnState Selector::Tick() {
		Node::ReturnState eCurrentState;
		for (UInt8 i = 0; i < m_vecChilds.size(); i++) {
			eCurrentState = m_vecChilds.at(i)->Tick();
			if (eCurrentState == Node::RUNNING) {
				return Node::RUNNING;
			} else if (eCurrentState == Node::SUCCESS) {
        return Node::SUCCESS;
      }
		}
		return Node::FAILURE;
	}

	/****************************************/
	/****************************************/

	void Selector::Reset() {}

	/****************************************/
	/****************************************/

	Selector* Selector::Clone() {
		return new Selector(this);
	}

  /****************************************/
  /****************************************/

  void Selector::FillDOTDescription(std::ostringstream& ss_dot_string){
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

	std::string Selector::GetLabel() {
	 return m_strLabel;
	}

	/****************************************/
	/****************************************/

	void Selector::AddChildNode(Node* pc_new_child_node) {
		m_vecChilds.push_back(pc_new_child_node);
	}

	/****************************************/
	/****************************************/

	void Selector::ShareRobotDAO(EpuckDAO* pc_robot_dao) {
		for (UInt8 i = 0; i < m_vecChilds.size(); i++) {
			m_vecChilds.at(i)->ShareRobotDAO(pc_robot_dao);
		}
	}
}
