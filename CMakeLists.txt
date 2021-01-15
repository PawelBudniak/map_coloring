cmake_minimum_required(VERSION 3.17)
project(map_coloring)

set(CMAKE_CXX_STANDARD 20)

add_executable(map_coloring main.cpp Graph.h coloring/Linear5.cpp coloring/Linear5.h coloring/Coloring.h)
add_executable(test_main testMain.cpp Graph.h coloring/Coloring.h)
add_subdirectory(unittest)


