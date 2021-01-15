#ifndef MAP_COLORING_GRAPH_H
#define MAP_COLORING_GRAPH_H

#include <vector>
#include <unordered_set>
#include <list>
#include <algorithm>
#include <iostream>
#include <deque>



struct LinkedVertex;

using LinkedVertexList = std::list<LinkedVertex>;

struct LinkedVertex{
    int index;
    LinkedVertexList::iterator edge;

    LinkedVertex(int idx, const LinkedVertexList::iterator &edge) :index(idx), edge(edge) {}

    operator int () const { return index; }
};

template <typename V, typename NeighbourList = std::vector<V>>
class Graph {
public:

    using VertexList = std::vector<NeighbourList>;

    Graph (int n_vertices): vertices(n_vertices) {}

    NeighbourList& operator[] (int idx)             { return vertices[idx]; }
    const NeighbourList& operator[] (int idx) const { return vertices[idx]; }

    void addEdge(int v1, int v2);

    void removeNeighbour(LinkedVertex from);



    //friend std::ostream& operator<<(std::ostream& os, const Graph& g);

    friend std::ostream &operator<<(std::ostream &os, const Graph &g){
        for (int i = 0; i < g.vertices.size(); ++i) {
            os << i << " : ";
            for (auto &neighbour: g.vertices[i]) {
                os << neighbour << " ";
            }
            os << std::endl;
        }
        return os;
    }



private:
    VertexList vertices;
public:
    //const VertexList &getVertices() const;
    auto getVertices() const -> const VertexList &;
};

template<typename V, typename NeighbourList>
inline void Graph<V, NeighbourList>::addEdge(int v1, int v2)
{
    // insert because push_back doesn't work for sets
    vertices[v1].insert(vertices[v1].end(), v2);
    vertices[v2].insert(vertices[v2].end(), v1);
}

//template<typename NeighbourList>
//inline void Graph<NeighbourList>::removeNeighbour(int from, int neighbour) {
//    NeighbourList & neighbours = vertices[from];
//    neighbours.erase(std::remove(neighbours.begin(), neighbours.end(), neighbour), neighbours.end());
//}
//
template<typename V, typename NeighbourList>
inline auto Graph<V,NeighbourList>::getVertices() const -> const typename Graph<V,NeighbourList>::VertexList &
{
    return vertices;
}

template<>
inline void Graph<LinkedVertex, LinkedVertexList>::addEdge(int v1, int v2)
{
    auto temp = vertices[v2].end();
    vertices[v1].push_back(LinkedVertex(v2, temp));
    auto it1 = --vertices[v1].end();
    vertices[v2].push_back(LinkedVertex(v1, it1));
    (--vertices[v1].end())->edge = --vertices[v2].end();
}

// a vertex v is removed, such that v is at the other end of an edge with vertex *from* passed to this function
// e.g.: this method is called on vertex 3 in vertices[1] => vertex 1 in vertices[3] will be removed
template<>
inline void Graph<LinkedVertex, LinkedVertexList>::removeNeighbour(LinkedVertex from)
{
    vertices[from].erase(from.edge);
}

//template<>
//inline void Graph<std::unordered_set<int>>::removeNeighbour(int from, int neighbour) {
//    std::unordered_set<int> & neighbours = vertices[from];
//    std::cout << "hello specka" << std::endl;
//    neighbours.erase(neighbour);
//}
//template<>
//inline void Graph<std::list<int>>::removeNeighbour(int from, int neighbour) {
//    std::cout << "list specka" << std::endl;
//    std::list<int> & neighbours = vertices[from];
//    neighbours.remove(neighbour);
//}

//template<typename int>
//inline void Graph<int, std::vector<int>>::removeNeighbour(int from, int neighbour) {
//    std::vector<int> & neighbours = vertices[from];
//    std::cout << "hello specka" << std::endl;
//    neighbours.erase(std::remove(neighbours.begin(), neighbours.end(), neighbour), neighbours.end());
//}


/* nie dziala u mnie mimo ze ustawilem standard c++20 przy robieniu nowego projektu w clion */
//template<typename int, typename NeighbourList>
//void Graph<int, NeighbourList>::removeNeighbour(int from, int neighbour) {
//    std::erase(vertices[from], neighbour);
//}




#endif //MAP_COLORING_GRAPH_H
