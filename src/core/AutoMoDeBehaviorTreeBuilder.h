/*
 * @file <src/core/AutoMoDeBehaviorTreeBuilder.h>
 *
 * @author Antoine Ligot - <aligot@ulb.ac.be>
 *
 * @package ARGoS3-AutoMoDe
 *
 * @license MIT License
 *
 * @brief This class is used to parse the configuration of
 * 				the stochastic finite state machine and instanciate the
 * 				different modules (behaviours and conditions). These modules
 * 				are added to the AutoMoDeFiniStateMachine created.
 */

#ifndef AUTOMODE_BEHAVIOR_TREE_BUILDER_H
#define AUTOMODE_BEHAVIOR_TREE_BUILDER_H

#include "AutoMoDeBehaviorTree.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <iterator>
#include <stdlib.h>

namespace argos {
	class AutoMoDeBehaviorTreeBuilder {
		public:
			/*
			 * Class constructor.
			 */
			AutoMoDeBehaviorTreeBuilder();

			/**
			 * Creates an AutoMoDeBehaviorTree based on a configuration as a vector of strings.
			 * This method should be called when the BehaviorTree is created from the AutoMoDeMain.cpp.
			 */
			AutoMoDeBehaviorTree* BuildBehaviorTree(std::vector<std::string>& vec_bt_config);

			/**
			 * Creates an AutoMoDeBehaviorTree based on a configuration as a string.
			 * This method should be called when the BehaviorTree is created from the AutoMoDeController.cpp.
			 */
			AutoMoDeBehaviorTree* BuildBehaviorTree(const std::string& str_bt_config);

			/*
			 * Class destructor.
			 */
			virtual ~AutoMoDeBehaviorTreeBuilder();

		private:
			/**
			 * Creates a AutoMoDeBehaviour from an action configuration and add it to the
			 * Selector in construction.
			 */
			void HandleAction(Selector* pc_selector, std::vector<std::string>& vec_bt_state_config);

			/**
			 * Creates a AutoMoDeCondition from a transition configuration and add it to the
			 * AutoMoDeFiniStateMachine in construction.
			 */
			void HandleTransition(std::vector<std::string>& vec_BehaviorTree_transition_config,
									const UInt32& un_initial_state_index, const UInt32& un_condition_index);

			/**
			 * Creates a list containing the indexes of the behaviours reachable from a given state.
			 * Added for compatibility with irace interdependent parameters.
			 */
			const std::vector<UInt32> GetPossibleDestinationBehaviour(const UInt32& un_initial_state_index);

			UInt32 m_unNumberStates;

			AutoMoDeBehaviorTree* cBehaviorTree;

	};
}

#endif
