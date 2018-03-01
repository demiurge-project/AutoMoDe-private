# Set up ARGoS compilation information
include_directories(${CMAKE_SOURCE_DIR} ${ARGOS_INCLUDE_DIRS})
link_directories(${ARGOS_LIBRARY_DIRS})

add_library(log_robot_position_loopfunc SHARED LogRobotPositionLoopFunc.h LogRobotPositionLoopFunc.cpp)
target_link_libraries(log_robot_position_loopfunc argos3plugin_${ARGOS_BUILD_FOR}_epuck)


add_subdirectory(example)
add_subdirectory(chocolate)
