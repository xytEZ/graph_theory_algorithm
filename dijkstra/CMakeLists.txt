cmake_minimum_required(VERSION 3.0)

set(DIJKSTRA_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DijkstraFileParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DijkstraAlgorithm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DijkstraResPrinter.cpp)

add_library(dijkstra_lib ${DIJKSTRA_SRCS})

target_link_libraries(dijkstra_lib PRIVATE ${Boost_LIBRARIES})

target_include_directories(dijkstra_lib PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include)
