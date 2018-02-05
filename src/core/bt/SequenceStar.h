/**
  * @file <src/core/bt/SequenceStar.h>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe-BT
  *
  * @license MIT License
  */

#ifndef SEQUENCESTAR_H
#define SEQUENCESTAR_H

#include "Node.h"

namespace argos {
	class SequenceStar: public Node {
		public:
			SequenceStar();
			SequenceStar(SequenceStar* pc_behaviour);
			virtual ~SequenceStar();

			virtual ReturnState Tick();

			virtual SequenceStar* Clone();

      virtual const std::string GetDOTDescription();

			virtual void AddCondition(AutoMoDeCondition* pc_condition);

			virtual void AddAction(AutoMoDeBehaviour* pc_action);

      void AddChildNode(Node* pc_new_child_node);

    private:
      std::vector<Node*> m_vecChilds;

      UInt8 m_unIndexRunningChild;
	};
}

#endif
