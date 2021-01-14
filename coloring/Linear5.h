#ifndef MAP_COLORING_LINEAR5_H
#define MAP_COLORING_LINEAR5_H
#include <utility>
#include "../Graph.h"
#include <stack>
#include <type_traits>
#include <optional>



// Returns:
// a tuple {int: n of colors used, vector<int>: vector mapping vertex:color}
auto colorLinear5(Graph<LinkedVertex,LinkedVertexList> &graph) -> std::tuple<int, std::vector<int>>;



class Linear5 {
private:

    Graph<LinkedVertex,LinkedVertexList> & graph;

    using Qtype = std::list<int>;
    // holds vertices of degree 5
    Qtype qDeg5;
    // holds vertices of degree <= 4
    Qtype qDegLte4;

    struct RemoveInfo{
        int removedVertex;
        // a vertex which removedVertex was identified with, -1 if no such vertex
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
    void color();


private:
    void check(int vertex);
    void identify(int u, int v);
    void removeVertex(int vertex);
};




#endif //MAP_COLORING_LINEAR5_H
