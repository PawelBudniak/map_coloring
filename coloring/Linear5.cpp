#include "Linear5.h"
#include <cassert>

auto colorLinear5(Graph<LinkedVertex,LinkedVertexList> &graph) -> std::tuple<int, std::vector<int>>
{
    Linear5 coloringClass(graph);
    coloringClass.reduce();
    coloringClass.color();
    return {1, std::vector<int>(graph.getVertices().size())};
}

Linear5::Linear5(Graph<LinkedVertex, LinkedVertexList> &g): graph(g)
{

    auto & vertices = graph.getVertices();

    // setup helper variables and containers

    n_vertices = vertices.size();
    qPointers = std::vector<std::optional<Qtype::iterator>>(n_vertices, std::nullopt);
    degrees = std::vector<int>();
    marks = std::vector<bool>(n_vertices, false);

    for (int i = 0; i <n_vertices; ++i){

        int degree = vertices[i].size();
        degrees.push_back(degree);

        if (degree == 5) {
            qDeg5.push_back(i);
            qPointers[i] = qDeg5.end()--;
        }
        else if(degree <= 4) {
            qDegLte4.push_back(i);
            qPointers[i] = qDegLte4.end()--;
        }
    }

}

void Linear5::reduce() {

}

void Linear5::color() {

}

void Linear5::check(int vertex) {
    if (degrees[vertex] == 5) {
        qDeg5.push_back(vertex);
        qPointers[vertex] = qDeg5.end()--;
    }
    else if(degrees[vertex] == 4){
        assert(qPointers[vertex].has_value() && "This should never fail, before the check the vertex must've had deg=5 "
                                                "so it must've had a valid qPointers value");
        qDeg5.erase(qPointers[vertex].value());
        qDegLte4.push_back(vertex);
        qPointers[vertex] = qDegLte4.end()--;
    }

}

void Linear5::removeVertex(int vertex) {
    auto & vertices = graph.getVertices();
    for (const LinkedVertex& v: vertices[vertex]){
        graph.removeNeighbour(v);
        degrees[v] -= 1;
        check(v);
    }
    removed.push(RemoveInfo(vertex, -1));

    auto optQptr = qPointers[vertex];
    if (optQptr.has_value()){ // useless check i think
        if (degrees[vertex] == 5)
            qDeg5.erase(optQptr.value());
        else if (degrees[vertex] <= 4)
            qDegLte4.erase(optQptr.value());
    }
    n_vertices -= 1;

}
