#ifndef MAP_COLORING_GENERATOR_H
#define MAP_COLORING_GENERATOR_H

#include <string>
#include <vector>
#include "../coloring/Graph.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>

class Generator {


public:



    static const std::string GENERATOR_FILE;
    static const std::string OUTPUT_FILE;
    static bool squelchStderr;
    static std::string graphTypes;
    static const int MAX_N_VERT = 90;

    static void generateToFile(int nVert, int maxGraphs);

    Graph<LinkedVertex, LinkedVertexList> getGraph(int nVert, int batchSize);

private:

    static const int NOVAL = -1;
    int possibleEdgesCache = NOVAL;
    std::vector<std::pair<int,int>> possible_edges;

    static std::vector<std::pair<int,int>> combinations_k2(int nVert);
    static int n_combinations_k2 (int n) { return n*(n-1)/2; }
};

#endif //MAP_COLORING_GENERATOR_H
