// Analysis of map coloring algorithms
// Authors: Pawel Budniak, Michal Swiatek

#ifndef MAP_COLORING_GENERATOR_H
#define MAP_COLORING_GENERATOR_H

#include <string>
#include <vector>
#include "../coloring/Graph.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>

class Generator {


public:
    // Generates a random order of all possible edges
    // Then with a probability p it adds the edge to the graph
    // next a test is run to check if the graph remained planar - if it didn't the edge is removed
    // Returns: A random graph with nVert vertices
    Graph<LinkedVertex, LinkedVertexList> getGraph(int nVert, int batchSize);

private:

    static const int NOVAL = -1;
    int prev_nVert = NOVAL;
    std::vector<std::pair<int,int>> possible_edges;

    static std::vector<std::pair<int,int>> combinations_k2(int nVert);
    static int n_combinations_k2 (int n) { return n*(n-1)/2; }
};

#endif //MAP_COLORING_GENERATOR_H
