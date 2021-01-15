#include <iostream>
#include "coloring/Graph.h"


int main() {
    Graph<LinkedVertex, LinkedVertexList> graph(2);
//    graph.addVertex(1);
//    graph.addVertex(2);
    graph.addEdge(0,1);
    std::cout << graph;
    graph.removeNeighbour(*graph.getVertices()[0].begin());
    //graph.removeNeighbour(0, 1);
    std::cout << graph;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
