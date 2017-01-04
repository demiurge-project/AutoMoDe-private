# Set up ARGoS compilation information
include_directories(${CMAKE_SOURCE_DIR} ${ARGOS_INCLUDE_DIRS})
link_directories(${ARGOS_LIBRARY_DIRS})

add_library(aggregation_loopfunc SHARED AggregationLoopFunc.h AggregationLoopFunc.cpp)
add_library(gianduja_aggregation_loopfunc SHARED GiandujaAggregationLoopFunc.h GiandujaAggregationLoopFunc.cpp)
target_link_libraries(aggregation_loopfunc argos3plugin_${ARGOS_BUILD_FOR}_epuck)
