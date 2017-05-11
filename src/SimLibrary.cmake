SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

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

set(AUTOMODE_HEADERS_PSEUDO_REALITY
	core/AutoMoDeControllerPseudoReality.h
	core/AutoMoDeRabBuffer.h
	core/AutoMoDeFiniteStateMachine.h
	core/AutoMoDeFsmBuilder.h
	core/AutoMoDeFsmHistory.h
	core/AutoMoDeRobotDAO.h
	core/AutoMoDeLoopFunctionsPseudoReality.h
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

set(AUTOMODE_SOURCES_PSEUDO_REALITY
	core/AutoMoDeControllerPseudoReality.cpp
	core/AutoMoDeRabBuffer.cpp
	core/AutoMoDeFiniteStateMachine.cpp
	core/AutoMoDeFsmBuilder.cpp
	core/AutoMoDeRobotDAO.cpp
	core/AutoMoDeFsmHistory.cpp
	core/AutoMoDeLoopFunctionsPseudoReality.cpp
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

add_library(automode_pseudo_reality SHARED ${AUTOMODE_HEADERS_PSEUDO_REALITY} ${AUTOMODE_SOURCES_PSEUDO_REALITY})
target_link_libraries(automode_pseudo_reality argos3plugin_${ARGOS_BUILD_FOR}_epuck)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
add_executable(automode_main AutoMoDeMain.cpp)
target_link_libraries(automode_main automode argos3core_${ARGOS_BUILD_FOR} argos3plugin_${ARGOS_BUILD_FOR}_epuck)

add_executable(automode_launch_pseudo_reality AutoMoDeMainPseudoReality.cpp)
target_link_libraries(automode_launch_pseudo_reality automode_pseudo_reality argos3core_${ARGOS_BUILD_FOR} argos3plugin_${ARGOS_BUILD_FOR}_epuck)
