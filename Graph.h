#ifndef MAP_COLORING_GRAPH_H
#define MAP_COLORING_GRAPH_H

#include <vector>
#include <unordered_set>
#include <list>
#include <algorithm>
#include <iostream>



template <typename NeighbourList = std::vector<int>>
class Graph {
public:

    using VertexList = std::vector<NeighbourList>;

    Graph (int n_vertices): vertices(n_vertices) {}

    NeighbourList& operator[] (int idx)             { return vertices[idx]; }
    const NeighbourList& operator[] (int idx) const { return vertices[idx]; }

    void addEdge(int v1, int v2);
    void removeNeighbour(int from, int neighbour);



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
    const VertexList &getVertices() const;
};

template<typename NeighbourList>
inline void Graph<NeighbourList>::addEdge(int v1, int v2) {
    // insert because push_back doesn't work for sets
    vertices[v1].insert(vertices[v1].end(), v2);
    vertices[v2].insert(vertices[v2].end(), v1);
}

template<typename NeighbourList>
inline void Graph<NeighbourList>::removeNeighbour(int from, int neighbour) {
    NeighbourList & neighbours = vertices[from];
    neighbours.erase(std::remove(neighbours.begin(), neighbours.end(), neighbour), neighbours.end());
}

template<typename NeighbourList>
inline const typename Graph<NeighbourList>::VertexList &Graph<NeighbourList>::getVertices() const {
    return vertices;
}

template<>
inline void Graph<std::unordered_set<int>>::removeNeighbour(int from, int neighbour) {
    std::unordered_set<int> & neighbours = vertices[from];
    std::cout << "hello specka" << std::endl;
    neighbours.erase(neighbour);
}
template<>
inline void Graph<std::list<int>>::removeNeighbour(int from, int neighbour) {
    std::cout << "list specka" << std::endl;
    std::list<int> & neighbours = vertices[from];
    neighbours.remove(neighbour);
}

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
