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

int n_combinations_k2(int n){
    return n*(n-1)/2;
}

std::vector<std::pair<int,int>> combinations_k2(int nVert){
    std::vector<std::pair<int,int>> result(n_combinations_k2(nVert));
    int count = 0;
    for (int i =0; i < nVert; ++i){
        for (int j =i+1; j < nVert; ++j)
            result[count++] = std::pair<int,int>(i,j);
    }
    return result;
}


Graph<LinkedVertex, LinkedVertexList> getGraph(int nVert, std::optional<std::vector<std::pair<int,int>>> edge_combinations=std::nullopt, int batchSize = 0 ){
    using namespace boost;
    typedef adjacency_list<vecS, vecS, undirectedS> MyGraphType;

    static std::mt19937_64 rng;
    // initialize the random number generator with time-dependent seed
    static uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    static std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
    rng.seed(ss);
    // initialize a uniform distribution between 0 and 1
    static std::uniform_real_distribution<double> unif(0, 1);

    MyGraphType g;

    std::vector<std::pair<int, int>> possible_edges;
    if (edge_combinations.has_value()) {
        possible_edges = std::move(edge_combinations.value());
    }
    else {
        possible_edges = combinations_k2(nVert);
    }

    std::shuffle(possible_edges.begin(), possible_edges.end(), std::default_random_engine(timeSeed));
    int count = 0;

    if (!batchSize) {

        for (const auto &edge: possible_edges) {
            if (unif(rng) < 1 - ( std::min((double)(count++)/possible_edges.size(),0.9))) {
            // (unif(rng) > 0.5) {
                add_edge(edge.first, edge.second, g);
                if (!boyer_myrvold_planarity_test(g))
                    remove_edge(edge.first, edge.second, g);


            }

       }
    }

    // add edges in batches version

    if (batchSize > 0) {

        for (int i = 0; i < possible_edges.size(); ++i) {
            auto &edge = possible_edges[i];
            add_edge(edge.first, edge.second, g);

            if (i % batchSize == 0 && !boyer_myrvold_planarity_test(g)) {
                for (int l = 0; l < batchSize; ++l) {
                    remove_edge(possible_edges[i - l].first, possible_edges[i - l].second, g);
                }
            }

        }

        if (!boyer_myrvold_planarity_test(g)) {
            for (int l = possible_edges.size() - 1; l < batchSize; --l) {
                remove_edge(possible_edges[l].first, possible_edges[l].second, g);
            }
        }
    }

    Graph<LinkedVertex, LinkedVertexList> myg(nVert);

    auto t = edges(g);
    for (auto& it = t.first; it != t.second; ++it){
        int u = source(*it,g);
        int v = target(*it,g);
        myg.addEdge(u,v);
    }
    return myg;


}

void manualMode(Graph<int, std::set<int>>& graph);
void generatorMode(Graph<int, std::set<int>>& graph);
void testMode(Graph<int, std::set<int>>& graph);

int main(int argc, char** argv)
{

    auto time = std::chrono::system_clock::now().time_since_epoch();

    const int N = 20;
    std::vector<double> densities;
    double colors = 0;


    for (int i =0; i< N; ++i) {
        Graph<LinkedVertex, LinkedVertexList> g = getGraph(60, combinations_k2(60));
        //std::cout << g;

        densities.emplace_back(g.getPlanarDensity());
        auto [nc,c] = colorLinear5(g);
        colors += nc;




    }
    std::cout << std::accumulate(densities.begin(), densities.end(), 0.0)/N << std::endl;
    std::cout << "colors: " << colors/N << std::endl;





    auto diff = std::chrono::system_clock::now().time_since_epoch() - time;



    std::cout << "ms: " << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
    //std::cout <<" density: " << g.getPlanarDensity() << std::endl;

    return 0;

    int step = 10;
    int max = 70;
    int start = 10;
    int maxGraphs = 10;

    std::vector<double> times;

    for (int n_vert = start; n_vert <= max; n_vert += step) {
        //Generator::generateToFile(n_vert, maxGraphs);

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
