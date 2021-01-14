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


// waiting for colorLinear5 implementation
//TEST_CASE("TestLinear5_1", "[Linear5]"){
//    Graph<std::deque<Vertex>> graph(5);
//    graph.addEdge(0,1);
//    graph.addEdge(3,4);
//    graph.addEdge(1,3);
//    auto [n_colors, coloring] = colorLinear5(graph);
//
//    CHECK(isCorrectColoring(graph, coloring));
//}