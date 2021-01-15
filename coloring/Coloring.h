/*  
 *  Created by michal-swiatek on 14.01.2021.
 *
 *  Github: https://github.com/michal-swiatek
 */

#ifndef MAP_COLORING_COLORING_H
#define MAP_COLORING_COLORING_H

#include <ctime>
#include <tuple>
#include <random>
#include <unordered_set>

#include "../Graph.h"

template <typename  NeighbourList>
int assignGreedyColor(int v, const NeighbourList& neighbours, std::vector<uint8_t>& availableColors, const std::vector<int>& result)
{
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
    for (j = 0; j < availableColors.size(); ++j)
        if (availableColors[j])
            break;

    //  Reset available colors for next iteration
    for (const auto& neighbour : neighbours)
    {
        auto color = result[static_cast<int>(neighbour)];    //  Get vertex index by converting it to int
        if (color != -1)
            availableColors[color] = true;
    }

    return j;
}

template <typename Vertex, typename NeighbourList>
auto greedyColoring(const Graph<Vertex, NeighbourList>& graph, bool shuffle = false) -> std::tuple<int, std::vector<int>>
{
    const auto& vertices = graph.getVertices();
    auto maxColor = 0;

    //  Vector of colors assigned to each Vertex
    std::vector<int> result(vertices.size(), -1);

    //  A graph can be colored with at most V colors
    //  Using uint8_t because std::vector<bool> is implemented as dynamic bitset
    std::vector<uint8_t> availableColors(vertices.size(), true);

    //  Generate order of traversal over graph vertices
    std::vector<int> traversal(vertices.size());
    std::generate(traversal.begin(), traversal.end(), [number = 0] () mutable { return number++; });

    if (shuffle)
        //  Use std::time() because some compilers have faulty std::random_device implementation
        std::shuffle(traversal.begin(), traversal.end(), std::mt19937(std::time(nullptr)));

    //  Assign colors to vertices
    for (auto v : traversal)
    {
        const auto& neighbours = vertices[v];
        auto vertexColor = assignGreedyColor(v, neighbours, availableColors, result);

        result[v] = vertexColor;
        maxColor = std::max(maxColor, vertexColor);
    }

    //  Shift color numbers to 1, 2, 3...
    std::transform(result.begin(), result.end(), result.begin(), [](auto a){ return ++a; });

    return {maxColor + 1, result};
}

template <typename Vertex, typename NeighbourList>
auto dsaturColoring(const Graph<Vertex, NeighbourList>& graph, bool shuffle = false) -> std::tuple<int, std::vector<int>>
{
    const auto& vertices = graph.getVertices();
    auto maxColor = 0;

    //  Vector of colors assigned to each Vertex
    std::vector<int> result(vertices.size(), -1);

    //  A graph can be colored with at most V colors
    //  Using uint8_t because std::vector<bool> is implemented as dynamic bitset
    std::vector<uint8_t> availableColors(vertices.size(), true);

    //  Holds saturation of each vertex
    std::vector<int> saturation(vertices.size());

    //  Degree of each vertex used to break ties between equally saturated vertices
    std::vector<int> degree(vertices.size());
    for (auto i = 0; i < vertices.size(); ++i)
        degree[i] = vertices[i].size();

    //  Vertices with unassigned color
    std::vector<int> unassignedVertices(vertices.size());
    std::generate(unassignedVertices.begin(), unassignedVertices.end(), [number = 0] () mutable { return number++; });

    while (!unassignedVertices.empty())
    {
        //  Find next vertex according to criteria:
        //      1. With highest saturation, when equal
        //      2. With highest degree, when equal
        //      3. First encountered
        auto best = unassignedVertices.begin();
        for (auto it = ++unassignedVertices.begin(); it != unassignedVertices.end(); ++it)
            if (saturation[*it] > saturation[*best] || (saturation[*it] == saturation[*best] && degree[*it] > degree[*best]))
                best = it;

        //  Get vertex from iterator
        auto v = *best;

        const auto& neighbours = vertices[v];
        auto vertexColor = assignGreedyColor(v, neighbours, availableColors, result);

        result[v] = vertexColor;
        maxColor = std::max(maxColor, vertexColor);

        //  Update saturation array
        //  Only neighbours of recently assigned vertex have to be checked because only their saturation could change
        for (const auto& neighbour : neighbours)
        {
            std::unordered_set<int> adjacentColors;     //  Used to count unique adjacent colors

            for (const auto& vertex : vertices[neighbour])
            {
                auto color = result[static_cast<int>(vertex)];    //  Get vertex index by converting it to int
                if (color != -1)
                    adjacentColors.insert(color);
            }

            saturation[static_cast<int>(neighbour)] = adjacentColors.size();
        }

        //  Remove vertex from unassignedVertices
        std::swap(*best, unassignedVertices.back());    //  Swap with last element
        unassignedVertices.pop_back();                        //  Constant time removal from end
    }

    //  Shift color numbers to 1, 2, 3...
    std::transform(result.begin(), result.end(), result.begin(), [](auto a){ return ++a; });

    return {maxColor + 1, result};
}

#endif //MAP_COLORING_COLORING_H
