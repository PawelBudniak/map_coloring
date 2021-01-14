#ifndef MAP_COLORING_LINEAR5_H
#define MAP_COLORING_LINEAR5_H
#include <utility>
#include "../Graph.h"
#include <stack>
#include <type_traits>


struct Vertex {};

class Linear5;

// Returns:
// a tuple {int: n of colors used, vector<int>: vector mapping vertex:color}
template <typename T>
inline auto colorLinear5(const Graph<LinkedVertex,LinkedVertexList> &graph) -> std::tuple<int, std::vector<int>>{
    static_assert(std::is_same<T, std::deque<Vertex>>::value, "linear5 coloring currently only works with std::deque<Vertex>");
    Linear5 coloringClass(graph);
    return {1, std::vector<int>(graph.getVertices().size())};
}


class Linear5 {

private:

    const Graph<LinkedVertex,LinkedVertexList> & graph;

    // holds vertices of degree 5
    std::list<int> qDeg5;
    // holds vertices of degree <= 4
    std::list<int> qDegLte4;

    struct RemoveInfo{
        int removedVertex;
        // a vertex which removedVertex was identified with, -1 if no such vertex
        int identifiedVertex;
    };

    std::stack<RemoveInfo> x;

    // helper vector used to merge 2 vertex neighbour sets
    // every vertex of v1 is marked as true
    std::vector<bool> marks;

    // holds the current degree of every vertex
    std::vector<int> degrees;
    // contains an iterator pointing to vertex v in qDegLte4 if v is present in qDegLte4
    std::vector<std::list<int>::iterator> qLte4Pointers;



public:
    std::vector<int> coloring;

    Linear5(const Graph<T> & g);


    void reduce();
    void color();


private:
    void check(int vertex);
    void identify(int u, int v);
};




#endif //MAP_COLORING_LINEAR5_H
