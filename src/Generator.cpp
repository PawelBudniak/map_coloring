// Analysis of map coloring algorithms
// Authors: Pawel Budniak, Michal Swiatek

#include <Generator.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <numeric>
#include <algorithm>
#include <chrono>

#include <random>
#include <ctime>


Graph<LinkedVertex, LinkedVertexList> Generator::getGraph(int nVert, int batchSize = 0){
    using namespace boost;
    typedef adjacency_list<vecS, vecS, undirectedS> MyGraphType;

    // initialize the random number generator with time-dependent seed
    static std::mt19937_64 rng(std::time(nullptr));
    // initialize a uniform distribution between 0 and 1
    static std::uniform_real_distribution<double> unif(0, 1);

    MyGraphType g;

    // if the function was called with the same nVert last time, don't recalculate all the edge combinations
    if (prev_nVert != nVert) {
        possible_edges = combinations_k2(nVert);
        prev_nVert = nVert;
    }

    std::shuffle(possible_edges.begin(), possible_edges.end(), std::default_random_engine(std::time(nullptr)));
    int count = 0;

    if (!batchSize) {
        for (const auto &edge: possible_edges) {
            // increase probability of choosing an edge in the earlier iterations
            if (unif(rng) < 1 - (std::min((double)(count++)/possible_edges.size(),0.9)) ) {

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

    Graph<LinkedVertex, LinkedVertexList> graph(nVert);

    auto t = edges(g);
    for (auto& it = t.first; it != t.second; ++it){
        int u = source(*it,g);
        int v = target(*it,g);
        graph.addEdge(u,v);
    }

    return graph;
}

//
std::vector<std::pair<int,int>> Generator::combinations_k2(int nVert) {
    std::vector<std::pair<int,int>> result(n_combinations_k2(nVert));
    int count = 0;
    for (int i =0; i < nVert; ++i){
        for (int j =i+1; j < nVert; ++j)
            result[count++] = std::pair<int,int>(i,j);
    }
    return result;
}
