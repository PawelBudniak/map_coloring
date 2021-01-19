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
#include <utility>
#include <stack>
#include <optional>

#include "Graph.h"

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
auto dsaturColoring(const Graph<Vertex, NeighbourList>& graph) -> std::tuple<int, std::vector<int>>
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
        auto best = std::max_element(unassignedVertices.begin(), unassignedVertices.end(),
                    [&](const auto& a, const auto& b)
                    {
                     return saturation[a] < saturation[b] || (saturation[a] == saturation[b] && degree[a] < degree[b]);
                    });

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
            auto index = static_cast<int>(neighbour);   //  Get neighbour index by converting it to int

            //  Update only non-colored, non-visited vertices
            if (result[index] == -1)
            {
                std::unordered_set<int> adjacentColors;     //  Used to count unique adjacent colors

                for (const auto& vertex : vertices[neighbour])
                {
                    auto color = result[static_cast<int>(vertex)];    //  Get vertex index by converting it to int
                    if (color != -1)
                        adjacentColors.insert(color);
                }

                saturation[index] = adjacentColors.size();
            }
        }

        //  Remove vertex from unassignedVertices
        std::swap(*best, unassignedVertices.back());    //  Swap with last element
        unassignedVertices.pop_back();                        //  Constant time removal from end
    }

    //  Shift color numbers to 1, 2, 3...
    std::transform(result.begin(), result.end(), result.begin(), [](auto a){ return ++a; });

    return {maxColor + 1, result};
}

// Implementation of an algorithm published by Howard Williams in The Computer Journal in 1985
// Colors a planar graph with max 5 colors in linear time
// Returns:
// a tuple {int: n of colors used, vector<int>: vector mapping vertex:color}
auto colorLinear5(Graph<LinkedVertex,LinkedVertexList> &graph) -> std::tuple<int, std::vector<int>>;



class Linear5 {
private:

    static const int K_THRESHHOLD = 13;
    static const int NO_COLOR = -1;
    static const int NOT_IDENTIFIED = -2;
    static const int N_COLORS = 5;

    Graph<LinkedVertex,LinkedVertexList> & graph;
    const Graph<LinkedVertex,LinkedVertexList>::VertexList & vertices;

    using Qtype = std::list<int>;
    // holds vertices of degree 5
    Qtype qDeg5;
    // holds vertices of degree <= 4
    Qtype qDegLte4;

    struct RemoveInfo{
        int removedVertex;
        // a vertex which removedVertex was identified with, NOT_IDENTIFIED if no such vertex
        int identifiedVertex;
        RemoveInfo(int rm, int id): removedVertex(rm), identifiedVertex(id) {}
    };

    std::stack<RemoveInfo> removed;

    // helper vector used to merge 2 vertex neighbour sets
    // every vertex of v1 is marked as true
    std::vector<bool> marks;

    // holds the current degree of every vertex
    std::vector<int> degrees;
    // contains an iterator pointing to vertex v in qDeg5 or qDegLte4 if v is present in either of them
    std::vector<std::optional<Qtype::iterator>> qPointers;

    // keeps track of current number of vertices, this is necessary because some get temporarily removed during the algorithm's runtime
    int n_vertices;



public:
    std::vector<int> coloring;

    explicit Linear5(Graph<LinkedVertex,LinkedVertexList> & g);


    void reduce();
    auto color() -> std::tuple<int, std::vector<int>>;


private:
    void check(int vertex);
    void identify(int u, int v);
    void removeVertex(int vertex);
    void removeFromQ(Qtype & q, int vertex);
    void removeFromUnkownQ(int vertex);
    void addToQ(Qtype & q, int vertex);
    void moveToQ(Qtype & from, Qtype & to, int vertex);
};


// testing functions:

template <typename T, typename V>
inline bool isCorrectColoring(const Graph<T,V> & graph, std::vector<int> coloring){
    auto vertices = graph.getVertices();
    if (vertices.size() != coloring.size()){
        throw std::invalid_argument("Number of vertices colored doesn't equal the number of vertices in graph");
    }
    for (int i=0; i < vertices.size(); ++i){
        auto & neighbourList = vertices[i];
        for (int neighbour: neighbourList){
            if (coloring[i] == coloring[neighbour])
                return false;
        }
    }
    return true;
}

void testAllAlgorithmsCorrectness(Graph<LinkedVertex,LinkedVertexList> & graph);


#endif //MAP_COLORING_COLORING_H
