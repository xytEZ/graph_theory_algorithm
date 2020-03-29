cmake_minimum_required(VERSION 3.0)

set(ASTAR_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/AStarFileParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/AStarAlgorithm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/AStarAlgoReport.cpp)

add_library(astar_lib ${ASTAR_SRCS})

target_link_libraries(astar_lib PRIVATE ${Boost_LIBRARIES})

target_include_directories(astar_lib PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include)
