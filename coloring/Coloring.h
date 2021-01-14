/*  
 *  Created by michal-swiatek on 14.01.2021.
 *
 *  Github: https://github.com/michal-swiatek
 */

#ifndef MAP_COLORING_COLORING_H
#define MAP_COLORING_COLORING_H

#include <ctime>
#include <random>
#include <utility>

#include "../Graph.h"

template <typename Vertex, typename NeighbourList>
auto greedyColoring(const Graph<Vertex, NeighbourList>& graph, bool shuffle = false) -> std::tuple<int, std::vector<int>>
{
    auto vertices = graph.getVertices();
    auto maxColor = 0;

    //  Vector of colors assigned to each Vertex
    std::vector<int> result(vertices.size(), -1);

    //  A graph can be colored with at most V colors
    //  Using uint8_t because std::vector<bool> is implemented as dynamic bitset
    std::vector<uint8_t> availableColors(vertices.size(), true);

    //  Generate order of traversal over graph vertices
    std::vector<int> traversal(vertices.size() - 1);
    std::generate(traversal.begin(), traversal.end(), [number = 0] () mutable { return ++number; });

    if (shuffle)
        //  Use std::time() because some compilers have faulty std::random_device implementation
        std::shuffle(traversal.begin(), traversal.end(), std::mt19937(std::time(nullptr)));

    //  Assign first color to first vertex
    result[0] = 0;
    for (auto v : traversal)
    {
        const auto& neighbours = vertices[v];

        //  Check all neighbours to determine used colors
        //  A value of true indicates that color can be used to color new vertex
        for (const auto& neighbour : neighbours)
        {
            auto color = result[static_cast<int>(neighbour)];    //  Get vertex index by converting it to int
            if (color != -1)
                availableColors[color] = false;
        }

        //  Use first available color
        int j;
        for (j = 0; j < vertices.size(); ++j)
            if (availableColors[j])
                break;

        result[v] = j;
        maxColor = std::max(maxColor, j);

        //  Reset available colors for next iteration
        for (const auto& neighbour : neighbours)
        {
            auto color = result[static_cast<int>(neighbour)];    //  Get vertex index by converting it to int
            if (color != -1)
                availableColors[color] = true;
        }
    }

    //  Start indexing colors from 1
    std::transform(result.begin(), result.end(), result.begin(), [](auto a){ return ++a; });

    return {maxColor + 1, result};
}

#endif //MAP_COLORING_COLORING_H
