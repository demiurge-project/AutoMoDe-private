add_library(icra_stop_loopfunc SHARED IcraStopLoopFunc.h IcraStopLoopFunc.cpp)
target_link_libraries(icra_stop_loopfunc argos3plugin_${ARGOS_BUILD_FOR}_epuck)

add_library(icra_agg_loopfunc SHARED IcraAggLoopFunc.h IcraAggLoopFunc.cpp)
target_link_libraries(icra_agg_loopfunc argos3plugin_${ARGOS_BUILD_FOR}_epuck)

add_library(icra_des_loopfunc SHARED IcraDesLoopFunc.h IcraDesLoopFunc.cpp)
target_link_libraries(icra_des_loopfunc argos3plugin_${ARGOS_BUILD_FOR}_epuck)

