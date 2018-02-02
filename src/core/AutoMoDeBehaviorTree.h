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

		private:

			Node* m_cRootNode;

	};
}

#endif
