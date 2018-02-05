/*
 * @file <src/core/AutoMoDeBehaviorTree.h>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * @package ARGoS3-AutoMoDe
 *
 * @license MIT License
 *
 * @brief
 */

#ifndef AUTOMODE_BEHAVIOR_TREE_H
#define AUTOMODE_BEHAVIOR_TREE_H

#include "bt/Node.h"
#include "bt/Selector.h"
#include "bt/SequenceStar.h"
#include "../modules/AutoMoDeBehaviour.h"
#include "../modules/AutoMoDeBehaviourAttraction.h"
#include "../modules/AutoMoDeBehaviourAntiPhototaxis.h"
#include "../modules/AutoMoDeBehaviourExploration.h"
#include "../modules/AutoMoDeBehaviourPhototaxis.h"
#include "../modules/AutoMoDeBehaviourRepulsion.h"
#include "../modules/AutoMoDeBehaviourStop.h"
#include "../modules/AutoMoDeCondition.h"
#include "../modules/AutoMoDeConditionBlackFloor.h"
#include "../modules/AutoMoDeConditionGrayFloor.h"
#include "../modules/AutoMoDeConditionWhiteFloor.h"
#include "../modules/AutoMoDeConditionFixedProbability.h"
#include "../modules/AutoMoDeConditionNeighborsCount.h"
#include "../modules/AutoMoDeConditionInvertedNeighborsCount.h"

#include <string>
#include <ctime>
#include <algorithm>

namespace argos {
	class AutoMoDeBehaviorTree {

		public:

			/*
			 * Class constructor.
			 */
			AutoMoDeBehaviorTree();

			/*
			 * Class destructor.
			 */
			virtual ~AutoMoDeBehaviorTree();

			/*
			 * Copy constructor.
			 * Intensively used in the main to duplicate the BT to all robots.
			 */
			AutoMoDeBehaviorTree(const AutoMoDeBehaviorTree* pc_fsm);


			void ControlStep();


			const std::string GetReadableFormat();

			void SetRootNode(Node* pc_root_node);

		private:

			Node* m_pcRootNode;

	};
}

#endif
