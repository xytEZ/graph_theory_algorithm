cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)

project(graphTheoryAlgo)

add_definitions(-g -W -Wall -Wextra -std=c++17)

set(SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ProgramOptionsHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/AbstractGraph.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FactoryGraph.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DijkstraFileParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DijkstraAlgorithm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DijkstraResPrinter.cpp)

add_executable(graphTheoryAlgo ${SRCS})

find_package(Boost COMPONENTS program_options REQUIRED)

Target_link_libraries(graphTheoryAlgo ${Boost_LIBRARIES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
