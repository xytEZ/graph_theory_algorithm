cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)

project(graphTheoryAlgo)

add_definitions(-g -W -Wall -Wextra -std=c++17)

set(SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ProgramOptionsHandler.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/AbstractGraph.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/FactoryGraph.cpp)

add_executable(graphTheoryAlgo ${SRCS})

find_package(Boost COMPONENTS program_options REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dijkstra)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/bellman_ford)

target_link_libraries(graphTheoryAlgo PRIVATE
  ${Boost_LIBRARIES}
  dijkstra_lib
  bellman_ford_lib)

target_include_directories(graphTheoryAlgo PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include)
