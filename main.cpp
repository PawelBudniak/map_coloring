/*
 *  Created by michal-swiatek on 14.01.2021.
 *
 *  Github: https://github.com/michal-swiatek
 */

#include <set>

#include "CommandLineParser.h"
#include "coloring/Graph.h"
#include "coloring/Coloring.h"

void manualMode(Graph<int, std::set<int>>& graph);
void generatorMode(Graph<int, std::set<int>>& graph);
void testMode(Graph<int, std::set<int>>& graph);

int main(int argc, char** argv)
{
    CommandLineParser commandParser(argc, argv);
//    commandParser.printParams();

    auto mode = commandParser.getMode();
    auto algorithm = commandParser.getAlgorithm();

    Graph<int, std::set<int>> graph(commandParser.getParam(CommandLineParser::N));

    switch (mode) {
        case CommandLineParser::MANUAL:     manualMode(graph);      break;
        case CommandLineParser::GENERATOR:  generatorMode(graph);   break;
        case CommandLineParser::TEST:       testMode(graph);        break;
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
//            auto [a, b] = colorLinear5(graph);
//            maxColor = a;
//            colors = std::move(b);
            break;
        }
    }

    std::cout << maxColor << '\n';
    for (const auto& color : colors)
        std::cout << color << ' ';

    return 0;
}

void manualMode(Graph<int, std::set<int>>& graph)
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
