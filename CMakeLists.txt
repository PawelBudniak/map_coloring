cmake_minimum_required(VERSION 3.17)
project(map_coloring)

set(BOOST_ROOT "D:/boost/boost_1_75_0/boost_1_75_0")

#find_package(Boos)
#if(NOT Boost_FOUND)
#    message(FATAL_ERROR "Could not find boost!")
#endif()

FIND_PACKAGE(Boost REQUIRED)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()


set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(map_coloring main.cpp coloring/Graph.h coloring/Coloring.h coloring/Coloring.cpp src/CommandLineParser.cpp include/Generator.h src/Generator.cpp)
add_executable(test_main testMain.cpp coloring/Graph.h coloring/Coloring.h src/CommandLineParser.cpp coloring/Coloring.cpp include/Generator.h src/Generator.cpp)
add_executable(generator generator/plantri.c)
add_subdirectory(unittest)


