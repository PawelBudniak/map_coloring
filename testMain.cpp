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
#include "unittest/TestColoring.h"
#include "Generator.h"

void manualMode(Graph<int, std::set<int>>& graph);
void generatorMode(Graph<int, std::set<int>>& graph);
void testMode(Graph<int, std::set<int>>& graph);

int main(int argc, char** argv)
{

    // UWAGA: trzeba najpierw odpalic konfiguracje "generator" zeby sie skompilowal generator
    // a pozniej dopiero mozna test_main

    int step = 5;
    int max = 40;
    int start = 15;
    int maxGraphs = 30;


    for (int n_vert = start; n_vert <= max; n_vert += step) {
        Generator::generate(n_vert, maxGraphs);

        std::ifstream fp(Generator::OUTPUT_FILE);
        std::string line;
        while (std::getline(fp, line)) {
            Graph<LinkedVertex, LinkedVertexList> g;
            g.fromAscii(line);
            testAllAlgorithmsCorrectness(g);
            std::cout << g << std::endl;
            std::cout << "################################### " << std::endl;
        }
    }

    remove(Generator::OUTPUT_FILE.c_str());


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
