cmake_minimum_required(VERSION 3.17)
project(map_coloring)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(map_coloring main.cpp coloring/Graph.h coloring/Coloring.h coloring/Coloring.cpp)
add_executable(test_main testMain.cpp coloring/Graph.h coloring/Coloring.h src/CommandLineParser.cpp)
add_subdirectory(unittest)


