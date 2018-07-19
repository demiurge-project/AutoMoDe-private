add_library(stop_mtc_loopfunc SHARED StopMtcLoopFunc.h StopMtcLoopFunc.cpp)
target_link_libraries(stop_mtc_loopfunc argos3plugin_${ARGOS_BUILD_FOR}_epuck)



