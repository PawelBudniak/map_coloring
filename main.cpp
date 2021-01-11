#include <iostream>
#include "Graph.h"


int main() {
    Graph<std::list<int>> graph(2);
//    graph.addVertex(1);
//    graph.addVertex(2);
    graph.addEdge(0,1);
    std::cout << graph;
    graph.removeNeighbour(0, 1);
    std::cout << graph;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
