/*  
 *  Created by michal-swiatek on 14.01.2021.
 *
 *  Github: https://github.com/michal-swiatek
 */

#include <set>

#include "CommandLineParser.h"
#include "coloring/Graph.h"
#include "coloring/Coloring.h"
#include <fstream>
#include <sstream>
#include <string>
#include "Generator.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <numeric>
#include <algorithm>
#include <chrono>

void combinations(int offset, int k, int nVert, std::vector<int> & curr, std::vector<std::vector<int>> & result) {
    if (k == 0) {
        result.push_back(curr);
        return;
    }
    for (int i = offset; i <= nVert - k; ++i) {
        curr.push_back(i);
        combinations(i+1, k-1, nVert, curr, result);
        curr.pop_back();
    }
}

void getGraph(int nVert = 10){
    using namespace boost;
    typedef adjacency_list<vecS, vecS, undirectedS> MyGraphType;

    MyGraphType g;



    std::vector<int> temp;
    std::vector<std::vector<int>> possible_edges;
    combinations(0,2,nVert, temp,possible_edges);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(possible_edges.begin(), possible_edges.end(), std::default_random_engine(seed));


    for (const auto & edge: possible_edges){
        add_edge(edge[0], edge[1], g);
        if (!boyer_myrvold_planarity_test(g))
            remove_edge(edge[0],edge[1],g);
    }


    auto t = edges(g);
    for (auto& it = t.first; it != t.second; ++it){
        int u = source(*it,g);
        auto v = source(*it,g);
        std::cout << *it << std::endl;

    }



}

void manualMode(Graph<int, std::set<int>>& graph);
void generatorMode(Graph<int, std::set<int>>& graph);
void testMode(Graph<int, std::set<int>>& graph);

int main(int argc, char** argv)
{

    // UWAGA: trzeba najpierw odpalic konfiguracje "generator" zeby sie skompilowal generator
    // a pozniej dopiero mozna test_main

//    std::vector<int> temp;
//    std::vector<std::vector<int>> result;
//    combinations(0,2,10, temp,result);
//
//    for (auto & vec: result){
//        for (auto& v: vec){
//            std::cout << v << " ";
//        }
//        std:: cout << std:: endl;
//    }


    getGraph(100);

//    int step = 5;
//    int max = 40;
//    int start = 15;
//    int maxGraphs = 30;
//
//
//    for (int n_vert = start; n_vert <= max; n_vert += step) {
//        Generator::generate(n_vert, maxGraphs);
//
//        std::ifstream fp(Generator::OUTPUT_FILE);
//        std::string line;
//        while (std::getline(fp, line)) {
//            Graph<LinkedVertex, LinkedVertexList> g;
//            g.fromAscii(line);
//            testAllAlgorithmsCorrectness(g);
//            std::cout << g << std::endl;
//            std::cout << "################################### " << std::endl;
//        }
//    }
//
//    remove(Generator::OUTPUT_FILE.c_str());


    return 0;
}

void manualMode(Graph<int, std::set<int>>& graph)
{
    int n;  //  Number of edges
    std::cin >> n;

    while (n-- > 0)
    {
        int a, b;   //  Vertices of edge
        std::cin >> a >> b;

        graph.addEdge(a, b);
    }
}

void generatorMode(Graph<int, std::set<int>>& graph)
{

}

void testMode(Graph<int, std::set<int>>& graph)
{
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
}
