#define CATCH_CONFIG_MAIN // provides a main() function for catch testing, should be included only once
#include "catch.hpp"
#include "../Graph.h"
#include <list>




TEST_CASE("TestAddEdge", "[Graph]"){
    Graph<int,std::list<int>> graph(2);
    graph.addEdge(0,1);
    int first_neighbour = *(graph[0].begin());
    int second_neighbour = *(graph[1].begin());
    CHECK(first_neighbour == 1);
    CHECK(second_neighbour == 0);
}



