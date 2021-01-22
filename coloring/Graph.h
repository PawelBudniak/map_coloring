// Analysis of map coloring algorithms
// Authors: Pawel Budniak, Michal Swiatek

#ifndef MAP_COLORING_GRAPH_H
#define MAP_COLORING_GRAPH_H

#include <vector>
#include <unordered_set>
#include <list>
#include <algorithm>
#include <iostream>
#include <deque>



struct LinkedVertex;

// needs to guarantee O(1) removal of an element from the middle given an iterator pointing to it
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

    using Vertex = V;
    using NeighboursList = NeighbourList;
    using VertexList = std::vector<NeighbourList>;

    explicit Graph (int n_vertices): vertices(n_vertices) {}
    Graph () = default;

    void fromAscii(const std::string & ascii);
    int fromPlanarCode(const std::vector<char>& data, int start);

    NeighbourList& operator[] (int idx)             { return vertices[idx]; }
    const NeighbourList& operator[] (int idx) const { return vertices[idx]; }

    void addEdge(int v1, int v2);

    void removeNeighbour(LinkedVertex from);

    void resize(int newSize) { vertices.resize(newSize); }

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
    int nEdges = 0;
public:
    //const VertexList &getVertices() const;
    auto getVertices() const -> const VertexList &;
    auto getPlanarDensity() const -> double;
};

template<typename V, typename NeighbourList>
inline void Graph<V, NeighbourList>::addEdge(int v1, int v2)
{
    // insert because push_back doesn't work for sets
    vertices[v1].insert(vertices[v1].end(), v2);
    vertices[v2].insert(vertices[v2].end(), v1);
    ++nEdges;
}


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
    ++nEdges;
}

// a vertex v is removed, such that v is at the other end of an edge with vertex *from* passed to this function
// e.g.: if this method is called on vertex 3 in vertices[1] => vertex 1 in vertices[3] will be removed
template<>
inline void Graph<LinkedVertex, LinkedVertexList>::removeNeighbour(LinkedVertex from)
{
    vertices[int(from)].erase(from.edge);
}

template<typename V, typename NeighbourList>
void Graph<V, NeighbourList>::fromAscii(const std::string &ascii) {


    const char FIRST = '#';
    const char DELIM = '!';
    int i = 0;

    // find the number of vertices
    while (ascii[i] != ' '){
        ++i;
    }
    int n_vert = std::stoi(ascii.substr(0, i));
    vertices = VertexList(n_vert);

    // find edges
    int currV = 0;
    while (ascii[i] != '\n' && i < ascii.size()){
        if(ascii[i] != DELIM){
            int adj = ascii[i] - FIRST;
            // avoid duplicate edges, e.g. only add (1,3) and ignore (3,1)
            if (currV < adj)
                addEdge(currV, adj);
        }
        else{
            ++currV;
        }
        ++i;
    }
}

template<typename V, typename NeighbourList>
int Graph<V, NeighbourList>::fromPlanarCode(const std::vector<char>& data, int start) {
    vertices.clear();

    const char FIRST = 1;

    int n_vert = static_cast<unsigned char>(data[start++]);
    vertices = VertexList(n_vert);

    for (int i = 0; i < n_vert; ++i)
    {
        int temp;
        while ((temp = static_cast<unsigned char>(data[start++])) > 0)
        {
            int adj = temp - FIRST;
            if (i < adj)
                addEdge(i, adj);
        }
    }

    return start;
}

template<typename V, typename NeighbourList>
auto Graph<V, NeighbourList>::getPlanarDensity() const -> double {
    double d_nVert = vertices.size();
    double d_nEdges = nEdges;
    return (d_nEdges - d_nVert+1)/(2*d_nVert-5);
}


#endif //MAP_COLORING_GRAPH_H
