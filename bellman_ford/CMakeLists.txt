cmake_minimum_required(VERSION 3.0)

set(BELLMAN_FORD_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BellmanFordFileParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BellmanFordAlgorithm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BellmanFordAlgoReport.cpp)

add_library(bellman_ford_lib ${BELLMAN_FORD_SRCS})

target_link_libraries(bellman_ford_lib PRIVATE ${Boost_LIBRARIES})

target_include_directories(bellman_ford_lib PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include)
