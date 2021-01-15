#include "catch.hpp"
#include "TestColoring.h"
#include "../coloring/Linear5.h"

TEST_CASE("TestIsCorrectColoring", "[isCorrectColoring]"){
    Graph<int> graph(4);
    std::vector<int> coloring(4, 0);
    REQUIRE(isCorrectColoring(graph,coloring));
}
TEST_CASE("TestIsCorrectColoring2", "[isCorrectColoring]"){
    Graph<int> graph(4);
    graph.addEdge(0,1);
    std::vector<int> coloring(4, 0);
    REQUIRE_FALSE(isCorrectColoring(graph,coloring));
}
TEST_CASE("TestIsCorrectColoringThrow", "[isCorrectColoring]"){
    Graph<int> graph(4);
    std::vector<int> coloring(5, 0);
    REQUIRE_THROWS(isCorrectColoring(graph,coloring));
}


TEST_CASE("TestLinear5_1", "[Linear5]"){
    Graph<LinkedVertex, LinkedVertexList> graph(5);
    graph.addEdge(0,1);
    graph.addEdge(3,4);
    graph.addEdge(1,3);
    Graph<LinkedVertex, LinkedVertexList> graphCopy = graph;

    auto [n_colors, coloring] = colorLinear5(graph);

    std::cout << "colors:" << n_colors;
    CHECK(isCorrectColoring(graphCopy, coloring));
    CHECK(n_colors <= 5);

}

TEST_CASE("Graph-4 is correctly colored", "[Linear5]"){
    Graph<LinkedVertex, LinkedVertexList> graph(4);
    graph.addEdge(0,1);
    graph.addEdge(3,2);
    graph.addEdge(1,3);
    Graph<LinkedVertex, LinkedVertexList> graphCopy = graph;

    auto [n_colors, coloring] = colorLinear5(graph);

    std::cout << "colors:" << n_colors;
    CHECK(isCorrectColoring(graphCopy, coloring));
    CHECK(n_colors <= 5);

}
TEST_CASE("Medium sized graph-8 is correctly colored", "[Linear5]"){
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

    Graph<LinkedVertex, LinkedVertexList> graphCopy = graph;


    auto [n_colors, coloring] = colorLinear5(graph);

    std::cout << "colors:" << n_colors;

    CHECK(isCorrectColoring(graphCopy, coloring));
    CHECK(n_colors <= 5);
}