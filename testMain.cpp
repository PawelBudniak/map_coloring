/*  
 *  Created by michal-swiatek on 14.01.2021.
 *
 *  Github: https://github.com/michal-swiatek
 */

#include "Graph.h"
#include "coloring/Coloring.h"

int main(int argc, char** argv)
{
    Graph<int> graph(8);

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

    std::cout << graph << "\n\n\n";

    auto [maxColor, colors] = greedyColoring(graph);

    std::cout << maxColor << '\n';
    for (const auto& color : colors)
        std::cout << color << ' ';

    return 0;
}
