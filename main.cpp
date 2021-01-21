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

struct TestResults
{
    std::vector<double> avgTimes1;
    std::vector<double> avgTimes2;
    std::vector<double> avgTimes3;
    std::vector<double> avgTimes4;

    std::vector<double> avgColor1;
    std::vector<double> avgColor2;
    std::vector<double> avgColor3;
    std::vector<double> avgColor4;

    void updateResults(double time1, double time2, double time3, double time4,
                       double color1, double color2, double color3, double color4, int nTests)
    {
        avgTimes1.push_back(time1 / nTests);
        avgTimes2.push_back(time2 / nTests);
        avgTimes3.push_back(time3 / nTests);
        avgTimes4.push_back(time4 / nTests);

        avgColor1.push_back(color1 / nTests);
        avgColor2.push_back(color2 / nTests);
        avgColor3.push_back(color3 / nTests);
        avgColor4.push_back(color4 / nTests);
    }
};

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
    Generator::generateToFile(nVert, 1);

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

    TestResults results;

    for (int nVert = start; nVert <= maxVertices; nVert += step)
    {
        double avgTime1 = 0, avgTime2 = 0, avgTime3 = 0, avgTime4 = 0,
               avgColor1 = 0, avgColor2 = 0, avgColor3 = 0, avgColor4 = 0;

        Generator g;

        for (int i = 0; i < nGraphs; ++i)
        {
            Graph<LinkedVertex, LinkedVertexList> graph = g.getGraph(nVert, 0);

            auto copy = graph;

//            std::cout << graph << std::endl;

            auto [time1, colorsUsed1, colors1] = timer.time(algorithm1, graph, false);
            auto [time2, colorsUsed2, colors2] = timer.time(algorithm1, graph, true);

            std::cout << graph << '\n';

            auto [time3, colorsUsed3, colors3] = timer.time(algorithm2, graph);

            std::cout << graph << '\n';

            auto [time4, colorsUsed4, colors4] = timer.time(algorithm3, graph);

            std::cout << colorsUsed1 << ' ' << colorsUsed2 << ' ' <<colorsUsed3 << ' ' <<colorsUsed4 << '\n';

            if (!isCorrectColoring(graph, colors1))
                std::cout << "Dupa1\n";
            if (!isCorrectColoring(graph, colors2))
                std::cout << "Dupa2\n";
            if (!isCorrectColoring(graph, colors3))
                std::cout << "Dupa3\n";
            if (!isCorrectColoring(graph, colors4))
                std::cout << "Dupa4\n";

            avgTime1 += time1;
            avgTime2 += time2;
            avgTime3 += time3;
            avgTime4 += time4;

            avgColor1 += colorsUsed1;
            avgColor2 += colorsUsed2;
            avgColor3 += colorsUsed3;
            avgColor4 += colorsUsed4;
        }

        results.updateResults(avgTime1, avgTime2, avgTime3, avgTime4, avgColor1, avgColor2, avgColor3, avgColor4, nGraphs);
    }

    for (auto avg : results.avgTimes1)
        std::cout << avg << ' ';

    std::cout << "\n";

    for (auto avg : results.avgTimes2)
        std::cout << avg << ' ';

    std::cout << "\n";

    for (auto avg : results.avgTimes3)
        std::cout << avg << ' ';

    std::cout << "\n";

    for (auto avg : results.avgTimes4)
        std::cout << avg << ' ';

    std::cout << "\n\n";

    for (auto avg : results.avgColor1)
        std::cout << avg << ' ';

    std::cout << "\n";

    for (auto avg : results.avgColor2)
        std::cout << avg << ' ';

    std::cout << "\n";

    for (auto avg : results.avgColor3)
        std::cout << avg << ' ';

    std::cout << "\n";

    for (auto avg : results.avgColor4)
        std::cout << avg << ' ';

    remove(Generator::OUTPUT_FILE.c_str());
}

// nie wiem gdzie dac ta funkcje xD w unittestach tak raczej nie pasuje
