/*
 *  Created by michal-swiatek on 14.01.2021.
 *
 *  Github: https://github.com/michal-swiatek
 */

#include <set>
#include <fstream>
#include <windows.h>

#include "CommandLineParser.h"
#include "coloring/Graph.h"
#include "coloring/Coloring.h"
#include "Generator.h"
#include "Timer.h"

void manualMode(Graph<LinkedVertex, LinkedVertexList>& graph);
void generatorMode(Graph<LinkedVertex, LinkedVertexList>& graph, int nVert);
void testMode(int start, int maxVertices, int step, int nGraphs);

int main(int argc, char** argv)
{
    CommandLineParser commandParser(argc, argv);
//    commandParser.printParams();

    auto mode = commandParser.getMode();
    auto algorithm = commandParser.getAlgorithm();

    Graph<LinkedVertex, LinkedVertexList> graph(commandParser.getParam(CommandLineParser::N));
    
    auto n = commandParser.getParam(CommandLineParser::N);
    auto k = commandParser.getParam(CommandLineParser::K);
    auto r = commandParser.getParam(CommandLineParser::R);
    auto step = commandParser.getParam(CommandLineParser::STEP);

    switch (mode) {
        case CommandLineParser::MANUAL:     manualMode(graph);                                 break;
        case CommandLineParser::GENERATOR:  generatorMode(graph, CommandLineParser::N);   break;
        case CommandLineParser::TEST:       testMode(n, n + step * k, step, r);         break;
        default:                            exit(1);
    }

//    std::cout << graph << "\n\n\n";

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

//    std::cout << maxColor << '\n';
//    for (const auto& color : colors)
//        std::cout << color << ' ';

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

void testMode(int start, int maxVertices, int step, int nGraphs)
{
    using namespace timer;

    Timer<microseconds> timer;

    auto algorithm1 = &greedyColoring<LinkedVertex, LinkedVertexList>;
    auto algorithm2 = &dsaturColoring<LinkedVertex, LinkedVertexList>;
    auto algorithm3 = colorLinear5;

    std::vector<double> times1;
    std::vector<double> times2;
    std::vector<double> times3;

    for (int nVert = start; nVert <= maxVertices; nVert += step) {
        Generator::generate(nVert, nGraphs + 1);

        std::ifstream fp(Generator::OUTPUT_FILE, std::ios::binary);
        std::vector<char> graphsData((std::istreambuf_iterator<char>(fp)), std::istreambuf_iterator<char>());

        Graph<LinkedVertex, LinkedVertexList> g;
        int dataStart = 0;

        double avg1 = 0, avg2 = 0, avg3 = 0;

        //  Umie czytac z binarnego pliku juz
        //  Paramwtry do map_coloring(najwieksze dla jakiego dziala): -m3 -n 109 -k 0 -r 100 -step 10
        while ((dataStart = g.fromBinary(graphsData, dataStart)) < graphsData.size() - 1)
        {
//            std::cout << g << std::endl;

            //  Timer dziala zajebiscie prosze z neigo korzystac :)
            avg1 += timer.time(algorithm1, g, false);
            avg2 += timer.time(algorithm2, g);
            avg3 += timer.time(algorithm3, g);
        }

        times1.push_back(avg1 / nGraphs);
        times2.push_back(avg2 / nGraphs);
        times3.push_back(avg3 / nGraphs);
    }

    for (auto avg : times1)
        std::cout << avg << ' ';

    std::cout << "\n\n";

    for (auto avg : times2)
        std::cout << avg << ' ';

    std::cout << "\n\n";

    for (auto avg : times3)
        std::cout << avg << ' ';

    remove(Generator::OUTPUT_FILE.c_str());
}

// nie wiem gdzie dac ta funkcje xD w unittestach tak raczej nie pasuje
