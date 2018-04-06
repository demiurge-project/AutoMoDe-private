# Set up ARGoS compilation information
include_directories(${CMAKE_SOURCE_DIR} ${ARGOS_INCLUDE_DIRS})
link_directories(${ARGOS_LIBRARY_DIRS})

add_subdirectory(example)
add_subdirectory(chocolate)
add_subdirectory(vanilla)
add_subdirectory(visual-add-ons)
