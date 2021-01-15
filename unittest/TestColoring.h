#ifndef MAP_COLORING_TESTCOLORING_H
#define MAP_COLORING_TESTCOLORING_H

#include "../coloring/Graph.h"

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

#endif //MAP_COLORING_TESTCOLORING_H
