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

void manualMode(Graph<int, std::set<int>>& graph);
void generatorMode(Graph<int, std::set<int>>& graph);
void testMode(Graph<int, std::set<int>>& graph);

int main(int argc, char** argv)
{

    // UWAGA: trzeba najpierw odpalic konfiguracje "generator" zeby sie skompilowal generator
    // a pozniej dopiero mozna test_main

    const std::string GENERATOR_FILE("generator.exe");
    const std::string OUTPUT_FILE("graphs.txt");
    const int MAX_N_VERT = 90;

    int step = 5;
    int max = 90;
    int start = 80;
    int limit = 2;

    if (max > MAX_N_VERT){
        throw std::runtime_error("Max vertices is: " + std::to_string(MAX_N_VERT));
    }

    for (int n_vert = start; n_vert <= max; n_vert += step) {

        auto n_vertS = std::to_string(n_vert);
        auto limitS = std::to_string(limit);
        std::string generatorCommand(GENERATOR_FILE + " mylimit " + limitS + " " + n_vertS + " -a > " + OUTPUT_FILE);
        system(generatorCommand.c_str());


        std::ifstream fp(OUTPUT_FILE);
        std::string line;
        while (std::getline(fp, line)) {
            Graph<LinkedVertex, LinkedVertexList> g(line);
            std::cout << g << std::endl;
            std::cout << "################################### " << std::endl;
        }
    }



    //Graph<LinkedVertex, LinkedVertexList> g("10 bcdefghi,aihjdc,abd,acbjhgfe,adf,aedg,afdh,agdjbi,ahb,bhd");
    //std::cout << g;



//    CommandLineParser commandParser(argc, argv);
//    commandParser.printParams();
//
//    auto mode = commandParser.getMode();
//    auto algorithm = commandParser.getAlgorithm();
//
//    Graph<int, std::set<int>> graph(commandParser.getParam(CommandLineParser::N));
//
//    switch (mode) {
//        case CommandLineParser::MANUAL:     manualMode(graph);      break;
//        case CommandLineParser::GENERATOR:  generatorMode(graph);   break;
//        case CommandLineParser::TEST:       testMode(graph);        break;
//        default:                            exit(1);
//    }
//
//    std::cout << graph << "\n\n\n";
//
//    int maxColor;
//    std::vector<int> colors;
//
//    switch (algorithm) {
//        case CommandLineParser::GREEDY:
//        {
//            auto [a, b] = greedyColoring(graph);
//            maxColor = a;
//            colors = std::move(b);
//            break;
//        }
//        case CommandLineParser::DSATUR:
//        {
//            auto[a, b] = dsaturColoring(graph);
//            maxColor = a;
//            colors = std::move(b);
//            break;
//        }
//        case CommandLineParser::LINEAR5:
//        {
////            auto [a, b] = colorLinear5(graph);
////            maxColor = a;
////            colors = std::move(b);
//            break;
//        }
//    }
//
//    std::cout << maxColor << '\n';
//    for (const auto& color : colors)
//        std::cout << color << ' ';
//
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
