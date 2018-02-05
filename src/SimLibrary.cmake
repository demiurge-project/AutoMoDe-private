# Set up ARGoS compilation information
include_directories(${CMAKE_SOURCE_DIR} ${ARGOS_INCLUDE_DIRS})
link_directories(${ARGOS_LIBRARY_DIRS})

# Headers
set(AUTOMODE_HEADERS
	core/AutoMoDeController.h
	core/AutoMoDeRabBuffer.h
	core/AutoMoDeFiniteStateMachine.h
	core/AutoMoDeFsmBuilder.h
	core/AutoMoDeFsmHistory.h
	core/AutoMoDeRobotDAO.h
	core/AutoMoDeLoopFunctions.h
	# Behaviours
	modules/AutoMoDeBehaviour.h
	modules/AutoMoDeBehaviourAntiPhototaxis.h
	modules/AutoMoDeBehaviourPhototaxis.h
	modules/AutoMoDeBehaviourAttraction.h
	modules/AutoMoDeBehaviourRepulsion.h
	modules/AutoMoDeBehaviourStop.h
	modules/AutoMoDeBehaviourExploration.h
	# Conditions
	modules/AutoMoDeCondition.h
	modules/AutoMoDeConditionBlackFloor.h
	modules/AutoMoDeConditionWhiteFloor.h
	modules/AutoMoDeConditionGrayFloor.h
	modules/AutoMoDeConditionNeighborsCount.h
	modules/AutoMoDeConditionInvertedNeighborsCount.h
	modules/AutoMoDeConditionFixedProbability.h)

# Sources
set(AUTOMODE_SOURCES
	core/AutoMoDeController.cpp
	core/AutoMoDeRabBuffer.cpp
	core/AutoMoDeFiniteStateMachine.cpp
	core/AutoMoDeFsmBuilder.cpp
	core/AutoMoDeRobotDAO.cpp
	core/AutoMoDeFsmHistory.cpp
	core/AutoMoDeLoopFunctions.cpp
	# Behaviours
	modules/AutoMoDeBehaviour.cpp
	modules/AutoMoDeBehaviourAntiPhototaxis.cpp
	modules/AutoMoDeBehaviourPhototaxis.cpp
	modules/AutoMoDeBehaviourAttraction.cpp
	modules/AutoMoDeBehaviourRepulsion.cpp
	modules/AutoMoDeBehaviourStop.cpp
	modules/AutoMoDeBehaviourExploration.cpp
	# Conditions
	modules/AutoMoDeCondition.cpp
	modules/AutoMoDeConditionBlackFloor.cpp
	modules/AutoMoDeConditionWhiteFloor.cpp
	modules/AutoMoDeConditionGrayFloor.cpp
	modules/AutoMoDeConditionNeighborsCount.cpp
	modules/AutoMoDeConditionInvertedNeighborsCount.cpp
	modules/AutoMoDeConditionFixedProbability.cpp)


set(BEHAVIOR_TREE_HEADERS
	core/AutoMoDeBehaviorTree.h
	core/AutoMoDeBehaviorTreeBuilder.h
	core/bt/Node.h
	core/bt/Selector.h
	core/bt/SequenceStar.h
	#AutoMoDe
	core/AutoMoDeRobotDAO.h
	core/AutoMoDeController.h
	core/AutoMoDeRabBuffer.h
	core/AutoMoDeLoopFunctions.h
	# Behaviours
	modules/AutoMoDeBehaviour.h
	modules/AutoMoDeBehaviourAntiPhototaxis.h
	modules/AutoMoDeBehaviourPhototaxis.h
	modules/AutoMoDeBehaviourAttraction.h
	modules/AutoMoDeBehaviourRepulsion.h
	modules/AutoMoDeBehaviourStop.h
	modules/AutoMoDeBehaviourExploration.h
	# Conditions
	modules/AutoMoDeCondition.h
	modules/AutoMoDeConditionBlackFloor.h
	modules/AutoMoDeConditionWhiteFloor.h
	modules/AutoMoDeConditionGrayFloor.h
	modules/AutoMoDeConditionNeighborsCount.h
	modules/AutoMoDeConditionInvertedNeighborsCount.h
	modules/AutoMoDeConditionFixedProbability.h)


set(BEHAVIOR_TREE_SOURCES
	core/AutoMoDeBehaviorTree.cpp
	core/AutoMoDeBehaviorTreeBuilder.cpp
	core/bt/Node.cpp
	core/bt/Selector.cpp
	core/bt/SequenceStar.cpp
	#AutoMoDe
	core/AutoMoDeRobotDAO.cpp
	core/AutoMoDeController.cpp
	core/AutoMoDeRabBuffer.cpp
	core/AutoMoDeLoopFunctions.cpp
	# Behaviours
	modules/AutoMoDeBehaviour.cpp
	modules/AutoMoDeBehaviourAntiPhototaxis.cpp
	modules/AutoMoDeBehaviourPhototaxis.cpp
	modules/AutoMoDeBehaviourAttraction.cpp
	modules/AutoMoDeBehaviourRepulsion.cpp
	modules/AutoMoDeBehaviourStop.cpp
	modules/AutoMoDeBehaviourExploration.cpp
	# Conditions
	modules/AutoMoDeCondition.cpp
	modules/AutoMoDeConditionBlackFloor.cpp
	modules/AutoMoDeConditionWhiteFloor.cpp
	modules/AutoMoDeConditionGrayFloor.cpp
	modules/AutoMoDeConditionNeighborsCount.cpp
	modules/AutoMoDeConditionInvertedNeighborsCount.cpp
	modules/AutoMoDeConditionFixedProbability.cpp)



add_library(automode SHARED ${AUTOMODE_HEADERS} ${AUTOMODE_SOURCES})
target_link_libraries(automode argos3plugin_${ARGOS_BUILD_FOR}_epuck)

add_library(automode_bt SHARED ${BEHAVIOR_TREE_HEADERS} ${BEHAVIOR_TREE_SOURCES})
target_link_libraries(automode argos3plugin_${ARGOS_BUILD_FOR}_epuck)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
add_executable(automode_main AutoMoDeMain.cpp)
target_link_libraries(automode_main automode argos3core_${ARGOS_BUILD_FOR} argos3plugin_${ARGOS_BUILD_FOR}_epuck)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
add_executable(automode_main_bt AutoMoDeMainBT.cpp)
target_link_libraries(automode_main_bt automode_bt automode argos3core_${ARGOS_BUILD_FOR} argos3plugin_${ARGOS_BUILD_FOR}_epuck)
