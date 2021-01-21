
#include <Generator.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <numeric>
#include <algorithm>
#include <chrono>

#include <stdexcept>
#include <random>

const std::string Generator::GENERATOR_FILE = "generator.exe";
const std::string Generator::OUTPUT_FILE = "graphs.bina";
bool Generator::squelchStderr = false;
//std::string Generator::graphTypes = "-p -c1 -m1"; // min connectivity = 1, min degree = 1;
std::string Generator::graphTypes = "";

void Generator::generateToFile(int nVert, int maxGraphs) {
    if (nVert > MAX_N_VERT){
        throw std::runtime_error("Max possible number of vertices is: " + std::to_string(MAX_N_VERT));
    }

    std::string squelcher = "";
    if (squelchStderr){
#ifdef _WIN32
        squelcher = " 2 > nul";
#else
        squelcher = "2 > dev/null";
#endif
    }


    auto s_nVert = std::to_string(nVert);
    auto s_maxGraphs = std::to_string(maxGraphs);
    std::string generatorCommand(
            GENERATOR_FILE + " mylimit " + s_maxGraphs +
            " " + s_nVert + " " + graphTypes + " " + OUTPUT_FILE + squelcher);
    system(generatorCommand.c_str());
}

Graph<LinkedVertex, LinkedVertexList> Generator::getGraph(int nVert, int batchSize=0) {
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

    std::vector<std::pair<int,int>> possible_edges(n_combinations_k2(nVert));
    combinations_k2(nVert, possible_edges);

    std::shuffle(possible_edges.begin(), possible_edges.end(), std::default_random_engine(timeSeed));
    int count = 0;

    if (!batchSize) {
        for (const auto &edge: possible_edges) {
            if (unif(rng) < 1 - ( std::min(    (double)(count++)/possible_edges.size(),0.9))     ) {
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
void Generator::combinations_k2(int nVert, std::vector<std::pair<int, int>> &result) {
    int count = 0;
    for (int i =0; i < nVert; ++i){
        for (int j =i+1; j < nVert; ++j)
            result[count++] = std::pair<int,int>(i,j);
    }
}
