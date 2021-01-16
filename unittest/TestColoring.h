#ifndef MAP_COLORING_TESTCOLORING_H
#define MAP_COLORING_TESTCOLORING_H

#include "../coloring/Graph.h"
#include "catch.hpp"

template <typename T, typename V>
inline bool isCorrectColoring(const Graph<T,V> & graph, std::vector<int> coloring){
    auto vertices = graph.getVertices();
    if (vertices.size() != coloring.size()){
        throw std::invalid_argument("Number of vertices colored doesn't equal the number of vertices in graph");
    }
    for (int i=0; i < vertices.size(); ++i){
        auto & neighbourList = vertices[i];
        for (int neighbour: neighbourList){
            if (coloring[i] == coloring[neighbour])
                return false;
        }
    }
    return true;
}
template <typename T, typename V>
void testAllAlgorithms(Graph<T,V> & graph){
    SECTION("GreedyColoring"){
        auto [n_colors, coloring] = greedyColoring(graph, true);
        CHECK(isCorrectColoring(graph, coloring));
        std::cout << "colors: " << n_colors << std::endl;
    }
    SECTION("DSatur"){
        auto [n_colors, coloring] = dsaturColoring(graph);
        CHECK(isCorrectColoring(graph, coloring));
        std::cout << "colors: " << n_colors << std::endl;
    }
    SECTION("Linear5"){
        Graph<LinkedVertex, LinkedVertexList> graph_copy = graph;
        auto [n_colors, coloring] = colorLinear5(graph);
        CHECK(isCorrectColoring(graph_copy, coloring));
        CHECK(n_colors <= 5);
        std::cout << "colors: " << n_colors << std::endl;
    }
}

#endif //MAP_COLORING_TESTCOLORING_H
