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
		for (UInt8 i = 0; i < m_vecConditions.size(); i++) {
			delete m_vecConditions.at(i);
		}
		for (UInt8 i = 0; i < m_vecActions.size(); i++) {
			delete m_vecActions.at(i);
		}
	}

  /****************************************/
  /****************************************/


	Selector::Selector(Selector* pc_selector) {
		std::vector<AutoMoDeCondition*> vecConditions = pc_selector->GetConditions();
		m_vecConditions.clear();
		for (std::vector<AutoMoDeCondition*>::iterator it = vecConditions.begin(); it != vecConditions.end(); ++it) {
			m_vecConditions.push_back((*it)->Clone());
		}

		std::vector<AutoMoDeBehaviour*> vecActions = pc_selector->GetActions();
		m_vecActions.clear();
		for (std::vector<AutoMoDeBehaviour*>::iterator it = vecActions.begin(); it != vecActions.end(); ++it) {
			m_vecActions.push_back((*it)->Clone());
		}
	}

	/****************************************/
	/****************************************/

	Node::ReturnState Selector::Tick() {
		for (UInt8 i = 0; i < m_vecConditions.size(); i++) {
			if (m_vecConditions.at(i)->Verify()) {
				LOG << "[" << m_unRobotID << "]\t" << "Condition true: " << m_vecConditions.at(i)->GetLabel() << std::endl;
				return Node::SUCCESS;
			}
		}
		LOG << "[" << m_unRobotID << "]\t"<< m_vecActions.at(0)->GetLabel() << std::endl;
		m_vecActions.at(0)->ControlStep();
		return Node::RUNNING;
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
		for (UInt8 i = 0; i < m_vecConditions.size(); i++) {
			ss_dot_string << "cond" << m_unBranchId << "x" << i << " [shape=diamond;label=\"" << m_vecConditions.at(i)->GetDOTDescription() << "\";];";
		}
		for (UInt8 i = 0; i < m_vecActions.size(); i++) {
			ss_dot_string << "act" << m_unBranchId << "x" << i << " [shape=circle;label=\"" << m_vecActions.at(i)->GetDOTDescription() << "\";];";
		}

		// Linking nodes
		for (UInt8 i = 0; i < m_vecConditions.size(); i++) {
			ss_dot_string << m_strLabel << m_unBranchId << " -> " << "cond" << m_unBranchId << "x" << i <<  ";";
		}
		for (UInt8 i = 0; i < m_vecActions.size(); i++) {
			ss_dot_string << m_strLabel << m_unBranchId << " -> " << "act" << m_unBranchId << "x" << i <<  ";";
		}
  }

	/****************************************/
	/****************************************/

	std::string Selector::GetLabel() {
	 return m_strLabel;
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

	/****************************************/
	/****************************************/

	void Selector::ShareRobotDAO(EpuckDAO* pc_robot_dao) {
		m_unRobotID = pc_robot_dao->GetRobotIdentifier();
		for (UInt8 i = 0; i < m_vecConditions.size(); i++) {
			m_vecConditions.at(i)->SetRobotDAO(pc_robot_dao);
		}
		for (UInt8 i = 0; i < m_vecActions.size(); i++) {
			m_vecActions.at(i)->SetRobotDAO(pc_robot_dao);
		}
	}

}
