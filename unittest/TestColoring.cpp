// Analysis of map coloring algorithms
// Authors: Pawel Budniak, Michal Swiatek

#include "catch.hpp"
#include "../coloring/Graph.h"
#include "../coloring/Coloring.h"

void testAllAlgorithms(Graph<LinkedVertex,LinkedVertexList> & graph){
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

TEST_CASE("Graph with no edges can be colored with one color", "[isCorrectColoring]"){
    Graph<int> graph(4);
    std::vector<int> coloring(4, 0);
    REQUIRE(isCorrectColoring(graph,coloring));
}
TEST_CASE("Graph with one edge can't be colored with one color", "[isCorrectColoring]"){
    Graph<int> graph(4);
    graph.addEdge(0,1);
    std::vector<int> coloring(4, 0);
    REQUIRE_FALSE(isCorrectColoring(graph,coloring));
}
TEST_CASE("Can't pass a coloring of different length than number of vertices ", "[isCorrectColoring]"){
    Graph<int> graph(4);
    std::vector<int> coloring(5, 0);
    REQUIRE_THROWS(isCorrectColoring(graph,coloring));
}


TEST_CASE("Graph with vertices of degree 0 is correctly colored", "[color]"){
    Graph<LinkedVertex, LinkedVertexList> graph(5);
    graph.addEdge(0,1);
    graph.addEdge(3,4);
    graph.addEdge(1,3);

    testAllAlgorithms(graph);

}

TEST_CASE("Graph-4 is correctly colored", "[color]"){
    Graph<LinkedVertex, LinkedVertexList> graph(4);
    graph.addEdge(0,1);
    graph.addEdge(3,2);
    graph.addEdge(1,3);

    testAllAlgorithms(graph);

}

TEST_CASE("Medium sized graph-8 is correctly colored", "[color]"){
    Graph<LinkedVertex, LinkedVertexList> graph(8);


    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 3);
    graph.addEdge(1, 3);
    graph.addEdge(2, 3);
    graph.addEdge(3, 4);
    graph.addEdge(4, 5);
    graph.addEdge(4, 6);
    graph.addEdge(5, 6);
    graph.addEdge(5, 7);
    graph.addEdge(6, 7);

    testAllAlgorithms(graph);
}

TEST_CASE("Graph with a vertex of degree > 5 is correctly colored", "[color]"){
    Graph<LinkedVertex, LinkedVertexList> graph(9);

    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 3);
    graph.addEdge(2, 3);
    graph.addEdge(3, 4);
    graph.addEdge(2, 5);
    graph.addEdge(3, 5);
    graph.addEdge(3, 6);
    graph.addEdge(5, 6);
    graph.addEdge(3, 7);
    graph.addEdge(6, 7);
    graph.addEdge(3, 8);
    graph.addEdge(7, 8);
    graph.addEdge(4, 8);

    testAllAlgorithms(graph);

}
