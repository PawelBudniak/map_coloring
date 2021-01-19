/*
 *  Created by michal-swiatek on 14.01.2021.
 *
 *  Github: https://github.com/michal-swiatek
 */

#include <set>
#include <fstream>

#include "CommandLineParser.h"
#include "coloring/Graph.h"
#include "coloring/Coloring.h"
#include "Generator.h"
#include "Timer.h"

void manualMode(Graph<LinkedVertex, LinkedVertexList>& graph);
void generatorMode(Graph<LinkedVertex, LinkedVertexList>& graph, int nVert);
void testMode(int start, int max, int step, int maxGraphs);

int main(int argc, char** argv)
{
    CommandLineParser commandParser(argc, argv);
//    commandParser.printParams();

    auto mode = commandParser.getMode();
    auto algorithm = commandParser.getAlgorithm();

    Graph<LinkedVertex, LinkedVertexList> graph(commandParser.getParam(CommandLineParser::N));

    switch (mode) {
        case CommandLineParser::MANUAL:     manualMode(graph);      break;
        case CommandLineParser::GENERATOR:  generatorMode(graph, CommandLineParser::N);   break;
        case CommandLineParser::TEST:       testMode(0,0,0,0);        break;
        default:                            exit(1);
    }

    std::cout << graph << "\n\n\n";

    int maxColor;
    std::vector<int> colors;

    switch (algorithm) {
        case CommandLineParser::GREEDY:
        {
            auto [a, b] = greedyColoring(graph);
            maxColor = a;
            colors = std::move(b);
            break;
        }
        case CommandLineParser::DSATUR:
        {
            auto[a, b] = dsaturColoring(graph);
            maxColor = a;
            colors = std::move(b);
            break;
        }
        case CommandLineParser::LINEAR5:
        {
            auto [a, b] = colorLinear5(graph);
            maxColor = a;
            colors = std::move(b);
            break;
        }
    }

    std::cout << maxColor << '\n';
    for (const auto& color : colors)
        std::cout << color << ' ';

    return 0;
}


void manualMode(Graph<LinkedVertex, LinkedVertexList>& graph)
{
    int n;  //  Number of vertices
    std::cin >> n;

    graph.resize(n);

    int k;  //  Number of edges
    std::cin >> k;

    while (k-- > 0)
    {
        int a, b;   //  Vertices of edge
        std::cin >> a >> b;

        graph.addEdge(a, b);
    }
}

void generatorMode(Graph<LinkedVertex, LinkedVertexList>& graph, int nVert)
{
    Generator::generate(nVert, 1);

    std::ifstream fp(Generator::OUTPUT_FILE);
    std::string line;
    std::getline(fp, line);

    graph.fromAscii(line);
}

void testMode(int start, int max, int step, int maxGraphs)
{
    using namespace timer;

    Timer<milliseconds> timer;

    auto algorithm1 = &greedyColoring<LinkedVertex, LinkedVertexList>;
    auto algorithm2 = &dsaturColoring<LinkedVertex, LinkedVertexList>;
    auto algorithm3 = colorLinear5;

    std::vector<double> times1;
    std::vector<double> times2;
    std::vector<double> times3;

    for (int nVert = start; nVert <= max; nVert += step) {
        Generator::generate(nVert, maxGraphs);

        std::ifstream fp(Generator::OUTPUT_FILE);
        std::string line;

        double avg1 = 0, avg2 = 0, avg3 = 0;

        while (std::getline(fp, line)) {
            Graph<LinkedVertex, LinkedVertexList> g;
            g.fromAscii(line);

            avg1 += timer.time(algorithm1, g, false);
            avg2 += timer.time(algorithm2, g);
            avg3 += timer.time(algorithm3, g);

//            std::cout << g << std::endl;
//            testAllAlgorithmsCorrectness(g);
//            std::cout << "################################### " << std::endl;
        }

        times1.push_back(avg1 / maxGraphs);
        times2.push_back(avg2 / maxGraphs);
        times3.push_back(avg3 / maxGraphs);
    }

    remove(Generator::OUTPUT_FILE.c_str());
}

// nie wiem gdzie dac ta funkcje xD w unittestach tak raczej nie pasuje
