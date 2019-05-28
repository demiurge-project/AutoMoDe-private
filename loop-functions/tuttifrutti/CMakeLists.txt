add_library(tutti_loopfunc SHARED TuttiLoopFunc.h TuttiLoopFunc.cpp)
target_link_libraries(tutti_loopfunc argos3plugin_${ARGOS_BUILD_FOR}_epuck)



