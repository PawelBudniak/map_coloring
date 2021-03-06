// Analysis of map coloring algorithms
// Authors: Pawel Budniak, Michal Swiatek

#include <iomanip>
#include <fstream>

#include "CommandLineParser.h"
#include "coloring/Graph.h"
#include "coloring/Coloring.h"
#include "Generator.h"
#include "Timer.h"

struct TestResults
{
    enum Algorithm {GREEDY, GREEDY_RANDOM, DSATUR, LINEAR5};

    int startVertices;
    int maxVertices;
    int stepVertices;

    std::vector<double> avgTimes1;
    std::vector<double> avgTimes2;
    std::vector<double> avgTimes3;
    std::vector<double> avgTimes4;

    std::vector<double> avgColor1;
    std::vector<double> avgColor2;
    std::vector<double> avgColor3;
    std::vector<double> avgColor4;

    TestResults(int startVertices, int maxVertices, int stepVertices) :
        startVertices(startVertices), maxVertices(maxVertices), stepVertices(stepVertices) { }

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

void manualMode(Graph<LinkedVertex, LinkedVertexList>& graph, CommandLineParser::Algorithm algorithm, bool print);
void generatorMode(Graph<LinkedVertex, LinkedVertexList>& graph, int nVert, CommandLineParser::Algorithm algorithm, bool print);
void testMode(int start, int maxVertices, int step, int nGraphs);

void printResults(const TestResults& results, int batchSize);
void printAlgorithmResults(const TestResults& results, TestResults::Algorithm algorithm);

int main(int argc, char** argv)
{
    CommandLineParser commandParser(argc, argv);

    auto mode = commandParser.getMode();
    auto algorithm = commandParser.getAlgorithm();

    Graph<LinkedVertex, LinkedVertexList> graph(commandParser.getParam(CommandLineParser::N));
    
    auto n = commandParser.getParam(CommandLineParser::N);
    auto k = commandParser.getParam(CommandLineParser::K);
    auto r = commandParser.getParam(CommandLineParser::R);
    auto step = commandParser.getParam(CommandLineParser::STEP);
    auto print = commandParser.getParam(CommandLineParser::P);

    switch (mode) {
        case CommandLineParser::MANUAL:     manualMode(graph, algorithm, print);        break;
        case CommandLineParser::GENERATOR:  generatorMode(graph, n, algorithm, print);  break;
        case CommandLineParser::TEST:       testMode(n, n + step * k, step, r);       break;
        default:                            exit(1);
    }

    return 0;
}

void manualMode(Graph<LinkedVertex, LinkedVertexList>& graph, CommandLineParser::Algorithm algorithm, bool print)
{
    std::cout << "Enter number of vertices:";

    int n;  //  Number of vertices
    std::cin >> n;

    graph.resize(n);

    std::cout << "Enter number of edges:";

    int k;  //  Number of edges
    std::cin >> k;

    while (k-- > 0)
    {
        std::cout << "Enter edge:";

        int a, b;   //  Vertices of edge
        std::cin >> a >> b;

        graph.addEdge(a, b);
    }

    if (print)
        std::cout << graph << "\n\n";

    switch (algorithm) {
        case CommandLineParser::GREEDY: {
            auto[colorsUsed, colors] = greedyColoring(graph);

            //  Print results
            std::cout << colorsUsed << '\n';
            for (int i = 0; i < colors.size(); ++i)
                std::cout << i + 1 << ' ' << colors[i] << '\n';

            break;
        }
        case CommandLineParser::DSATUR: {
            auto[colorsUsed, colors] = dsaturColoring(graph);

            //  Print results
            std::cout << colorsUsed << '\n';
            for (int i = 0; i < colors.size(); ++i)
                std::cout << i + 1 << ' ' << colors[i] << '\n';

            break;
        }
        case CommandLineParser::LINEAR5: {
            auto[colorsUsed, colors] = colorLinear5(graph);

            //  Print results
            std::cout << colorsUsed << '\n';
            for (int i = 0; i < colors.size(); ++i)
                std::cout << i + 1 << ' ' << colors[i] << '\n';

            break;
        }
    }
}

void generatorMode(Graph<LinkedVertex, LinkedVertexList>& graph, int nVert, CommandLineParser::Algorithm algorithm, bool print)
{
    Generator g;
    graph = g.getGraph(nVert, 0);

    if (print)
        std::cout << graph << "\n\n";

    switch (algorithm) {
        case CommandLineParser::GREEDY: {
            auto[colorsUsed, colors] = greedyColoring(graph);

            //  Print results
            std::cout << colorsUsed << '\n';
            for (int i = 0; i < colors.size(); ++i)
                std::cout << i + 1 << ' ' << colors[i] << '\n';

            break;
        }
        case CommandLineParser::DSATUR: {
            auto[colorsUsed, colors] = dsaturColoring(graph);

            //  Print results
            std::cout << colorsUsed << '\n';
            for (int i = 0; i < colors.size(); ++i)
                std::cout << i + 1 << ' ' << colors[i] << '\n';

            break;
        }
        case CommandLineParser::LINEAR5: {
            auto[colorsUsed, colors] = colorLinear5(graph);

            //  Print results
            std::cout << colorsUsed << '\n';
            for (int i = 0; i < colors.size(); ++i)
                std::cout << i + 1 << ' ' << colors[i] << '\n';

            break;
        }
    }
}

void testMode(int start, int maxVertices, int step, int nGraphs)
{
    using namespace timer;

    Timer<microseconds> timer;

    auto algorithm1 = &greedyColoring<LinkedVertex, LinkedVertexList>;
    auto algorithm2 = &dsaturColoring<LinkedVertex, LinkedVertexList>;
    auto algorithm3 = colorLinear5;

    TestResults results(start, maxVertices, step);

    for (int nVert = start; nVert < maxVertices; nVert += step)
    {
        double avgTime1 = 0, avgTime2 = 0, avgTime3 = 0, avgTime4 = 0,
               avgColor1 = 0, avgColor2 = 0, avgColor3 = 0, avgColor4 = 0;

        Generator g;

        for (int i = 0; i < nGraphs; ++i)
        {
            Graph<LinkedVertex, LinkedVertexList> graph = g.getGraph(nVert, 0);

            auto [time1, colorsUsed1, colors1] = timer.time(algorithm1, graph, false);
            auto [time2, colorsUsed2, colors2] = timer.time(algorithm1, graph, true);
            auto [time3, colorsUsed3, colors3] = timer.time(algorithm2, graph);
            auto [time4, colorsUsed4, colors4] = timer.time(algorithm3, graph);

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

    printResults(results, nGraphs);
}

void printResults(const TestResults& results, int batchSize)
{
    std::cout << "\nEach test group has been tested on batch of " << batchSize << " graphs.\n\n";
    std::cout << "n - number of vertices for each graphs in batch\n";
    std::cout << "T(n) - theoretical speed of algorithm (linear for all tested algorithms)\n";
    std::cout << "t(n) - measured speed of algorithm (average over all tests in batch, given in microseconds)\n";
    std::cout << "q(n) - estimation coefficient\n";
    std::cout << "c - average number of colors used to color graph\n";

    printAlgorithmResults(results, TestResults::GREEDY);
    printAlgorithmResults(results, TestResults::GREEDY_RANDOM);
    printAlgorithmResults(results, TestResults::DSATUR);
    printAlgorithmResults(results, TestResults::LINEAR5);
}

void printAlgorithmResults(const TestResults& results, TestResults::Algorithm algorithm)
{
    using namespace std;

    const std::vector<double>* times;
    const std::vector<double>* colors;

    std::ofstream file;

    std::cout << std::endl;

    switch (algorithm)
    {
        case TestResults::GREEDY:
            times = &results.avgTimes1;
            colors = &results.avgColor1;

            file.open("greedy_results.csv");

            std::cout << "Results of greedy algorithm\n\n";
            break;

        case TestResults::GREEDY_RANDOM:
            times = &results.avgTimes2;
            colors = &results.avgColor2;

            file.open("greedy_shuffle_results.csv");

            std::cout << "Results of greedy algorithm with random order of traversal\n\n";
            break;

        case TestResults::DSATUR:
            times = &results.avgTimes3;
            colors = &results.avgColor3;

            file.open("dsatur_results.csv");

            std::cout << "Results of dsatur algorithm\n\n";
            break;

        case TestResults::LINEAR5:
            times = &results.avgTimes4;
            colors = &results.avgColor4;

            file.open("linear5_results.csv");

            std::cout << "Results of linear5 algorithm\n\n";
            break;
    }

    auto mid = (*times).size() / 2;
    auto tMedian = (*times).size() % 2 == 0 ? ((*times)[mid] + (*times)[mid + 1]) / 2 : (*times)[mid];
    auto TMedian = (results.startVertices + results.maxVertices) / 2;

    //  Write headers to output file
    file << "n,T(n),t(n),q(n),colors\n";

    std::cout << "------------------------------------------------\n";
    std::cout << "|  n  |   T(n)   |   t(n)   |   q(n)   |   c   |\n";
    std::cout << "------------------------------------------------\n";

    for (int i = 1; i <= results.avgTimes1.size(); ++i)
    {
        auto Tn = i * results.stepVertices;
        auto tn = (*times)[i - 1];

        //  Print results on console
        std::cout << "| " << setw(3) <<  i * results.stepVertices;
        std::cout << " | " << setw(8) << Tn;
        std::cout << " | " << setw(8) << tn;
        std::cout << " | " << setw(8) << setprecision(5) << (tn * TMedian) / (Tn * tMedian);
        std::cout << " | " << setw(5) << (*colors)[i - 1];
        std::cout << " |\n";

        //  Save to csv file
        file << i * results.stepVertices << ',';
        file << Tn << ',';
        file << tn << ',';
        file << setprecision(5) << (tn * TMedian) / (Tn * tMedian) << ',';
        file << (*colors)[i - 1] << '\n';
    }
}
