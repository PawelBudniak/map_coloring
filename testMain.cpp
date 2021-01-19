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
#include <windows.h>

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

Graph<LinkedVertex, LinkedVertexList> getGraph(int nVert = 10){
    using namespace boost;
    typedef adjacency_list<vecS, vecS, undirectedS> MyGraphType;



    std::mt19937_64 rng;
    // initialize the random number generator with time-dependent seed
    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
    rng.seed(ss);
    // initialize a uniform distribution between 0 and 1
    std::uniform_real_distribution<double> unif(0, 1);

    MyGraphType g;



    std::vector<int> temp;
    std::vector<std::vector<int>> possible_edges;
    combinations(0,2,nVert, temp,possible_edges);

    std::shuffle(possible_edges.begin(), possible_edges.end(), std::default_random_engine(timeSeed));


    for (const auto & edge: possible_edges){
        //if (unif(rng) >  0.2) {
            add_edge(edge[0], edge[1], g);
            if (!boyer_myrvold_planarity_test(g))
                remove_edge(edge[0], edge[1], g);
        //}
    }

    Graph<LinkedVertex, LinkedVertexList> myg(nVert);

    auto t = edges(g);
    for (auto& it = t.first; it != t.second; ++it){
        int u = source(*it,g);
        int v = target(*it,g);
        //std::cout <<"to: " <<  target(*it,g)  << " " << source(*it,g) << std::endl;
        myg.addEdge(u,v);
    }
    return myg;


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

//
//    auto time = std::chrono::system_clock::now().time_since_epoch();
//    Graph<LinkedVertex, LinkedVertexList> g = getGraph(10);
//    std::cout << g;
//    auto diff = std::chrono::system_clock::now().time_since_epoch() - time;
//    std::cout << "ms: " << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();

    int step = 10;
    int max = 70;
    int start = 10;
    int maxGraphs = 30;

    std::vector<double> times;

    for (int n_vert = start; n_vert <= max; n_vert += step) {
        //Generator::generate(n_vert, maxGraphs);

        //Sleep(10 * 1000);


        double avg = 0;

        //std::ifstream fp(Generator::OUTPUT_FILE);
        //std::string line;
        //while (std::getline(fp, line)) {
        for (int i = 0; i < maxGraphs; ++i) {
            Graph<LinkedVertex, LinkedVertexList> g = getGraph(n_vert);
            //g.fromAscii(line);

            //testAllAlgorithmsCorrectness(g);
            auto time = std::chrono::steady_clock::now().time_since_epoch();
            colorLinear5(g);
            auto diff = std::chrono::steady_clock::now().time_since_epoch() - time;

            avg += std::chrono::duration_cast<std::chrono::microseconds>(diff).count();


//            std::cout << g << std::endl;
//            std::cout << "################################### " << std::endl;
            //}
        }

        times.push_back(avg/maxGraphs);

    }
    std::cout << "times: new york times taka gazeta" << std::endl;

    for (auto& v: times){
        std::cout << v << " ";
    }

    //remove(Generator::OUTPUT_FILE.c_str());
//
//
//    return 0;
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
