#include "catch.hpp"
#include "TestColoring.h"
#include "../coloring/Linear5.h"

TEST_CASE("TestIsCorrectColoring", "[isCorrectColoring]"){
    Graph graph(4);
    std::vector<int> coloring(4, 0);
    REQUIRE(isCorrectColoring(graph,coloring));
}
TEST_CASE("TestIsCorrectColoring2", "[isCorrectColoring]"){
    Graph graph(4);
    graph.addEdge(0,1);
    std::vector<int> coloring(4, 0);
    REQUIRE_FALSE(isCorrectColoring(graph,coloring));
}
TEST_CASE("TestIsCorrectColoringThrow", "[isCorrectColoring]"){
    Graph graph(4);
    std::vector<int> coloring(5, 0);
    REQUIRE_THROWS(isCorrectColoring(graph,coloring));
}


// waiting for colorLinear5 implementation
TEST_CASE("TestLinear5_1", "[Linear5]"){
    Graph graph(5);
    graph.addEdge(0,1);
    graph.addEdge(3,4);
    graph.addEdge(1,3);
    auto result = colorLinear5(graph);
    auto coloring = std::get<1>(result);
    CHECK(isCorrectColoring(graph, coloring));
}