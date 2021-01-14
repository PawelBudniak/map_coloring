#define CATCH_CONFIG_MAIN // provides a main() function for catch testing, should be included only once
#include "catch.hpp"
#include "../Graph.h"
#include <list>


TEST_CASE("Test1", "[Test]"){
    CHECK(1 == 0);
}

TEST_CASE("Testrm", "[Test]"){
    std::list<int> mylist = {1,2,3,4,5,6};
    auto it1 = std::next(mylist.begin(), 3);
    auto it2 = std::next(mylist.begin(), 5);
    std::cout << "it2 before" << *it2;
    mylist.erase(it1);
    std::cout << "it2 after" << *it2;
    for (auto i: mylist)
        std:: cout << i << ": ";



}
TEST_CASE("TestAddEdge", "[Graph]"){
    Graph<int,std::list<int>> graph(2);
    graph.addEdge(0,1);
    int first_neighbour = *(graph[0].begin());
    int second_neighbour = *(graph[1].begin());
    CHECK(first_neighbour == 1);
    CHECK(second_neighbour == 0);
}



