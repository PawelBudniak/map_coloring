#include <cassert>
#include <set>

#include "Coloring.h"

auto colorLinear5(Graph<LinkedVertex,LinkedVertexList> &graph) -> std::tuple<int, std::vector<int>>
{
    Linear5 coloringClass(graph);
    coloringClass.reduce();
    return coloringClass.color();
}

Linear5::Linear5(Graph<LinkedVertex, LinkedVertexList> &g):
        graph(g),
        vertices(g.getVertices())
{

    //auto & vertices = graph.getVertices();

    // setup helper variables and containers

    n_vertices = vertices.size();
    qPointers = std::vector<std::optional<Qtype::iterator>>(n_vertices, std::nullopt);
    degrees = std::vector<int>(n_vertices);
    marks = std::vector<bool>(n_vertices, false);

    for (int i = 0; i <n_vertices; ++i){

        int degree = vertices[i].size();
        degrees[i] = degree;

        if (degree == 5)
            addToQ(qDeg5, i);
        else if(degree <= 4)
            addToQ(qDegLte4, i);
    }

}

void Linear5::reduce() {
    while (n_vertices > 5){
        if (!qDegLte4.empty()) {
            // "delete top entry from Q4" -- idk if back or front
            removeVertex(qDegLte4.front());
        }
        else{
            int v = qDeg5.front();

            // try and find two non adj x,y can be found in N(v) such that deg[x] < k and deg[y] < k
            int y = -1, x = -1;

            // candidates for x,y are these with deg smaller than K
            std::vector<int> candidates;
            for (const LinkedVertex& temp: vertices[v]){
                if (degrees[temp] < K_THRESHHOLD)
                    candidates.push_back(temp);
            }
            // now check if we can find 2 non adjacent candidates, this takes at most O(5*5*K) => it's done in constant time
            // candidates size <= vertices[v] size == 5; since it's in qDeg5
            // TODO: mozna przechodzic po (5 choose 2) kombinacjach a nie po 5*5 wszystkich z powtorzeniami jak teraz
            for (int i=0; i < candidates.size(); ++i){
                for (int j=i; i < candidates.size(); ++j){
                    bool areAdj = false;
                    for (const LinkedVertex& k: vertices[j]){
                        if (k.index == i){
                            areAdj = true;
                            break;
                        }
                    }
                    if (!areAdj){
                        y = i;
                        x = j;
                    }
                }
            }
            // vertices x,y that satisfy the conditions described above have been found
            if (y != -1){
                removeVertex(v);
                identify(x,y);
            }
            else{
                qDeg5.pop_front();
                qDeg5.push_back(v);
            }

        }
    }
}

auto Linear5::color() -> std::tuple<int, std::vector<int>> {
    std::vector<int> coloring(vertices.size(), -1);

    // color at most 5 remaining vertices (after reduction) i.e those on qDegLte4
    auto it = qDegLte4.begin();
    for (int i = 0; i < qDegLte4.size(); ++i){
        coloring[*it] = i;
        ++it;
    }


    while (!removed.empty()){
        RemoveInfo rInfo = removed.top();
        removed.pop();
        if (rInfo.identifiedVertex == NOT_IDENTIFIED){
            //color rInfo.removedVertex with color different than vertices in vertices[rInfo.removedVertex]
            bool colors_used[N_COLORS] = { false };
            for (const LinkedVertex& v: vertices[rInfo.removedVertex]){
                if (coloring[v] != NO_COLOR)
                    colors_used[coloring[v]] = true;
            }
            for (int i=0; i < N_COLORS; ++i){
                if (!colors_used[i])
                    coloring[rInfo.removedVertex] = i;
            }

        }
        else{
            assert(coloring[rInfo.identifiedVertex] != NO_COLOR);
            coloring[rInfo.removedVertex] = coloring[rInfo.identifiedVertex];
        }

    }
    // TODO: xD
    int n_colors = std::set<int>(coloring.begin(), coloring.end()).size();
    if (n_colors > 5)
        throw std::runtime_error("Beka " + std::to_string(n_colors));
    assert(n_colors <= 5);
    return {n_colors, coloring};
}

void Linear5::check(int vertex) {

    if (degrees[vertex] == 5) {
        addToQ(qDeg5, vertex);
    }
    else if(degrees[vertex] == 4){
        moveToQ(qDegLte4, qDeg5, vertex);
    }

}

void Linear5::removeVertex(int vertex) {
    for (const LinkedVertex& v: vertices[vertex]){
        graph.removeNeighbour(v);
        degrees[v] -= 1;
        check(v);
    }
    removed.push(RemoveInfo(vertex, NOT_IDENTIFIED));

    removeFromUnkownQ(vertex);
    n_vertices -= 1;

}

void Linear5::identify(int u, int v) {

    for (const LinkedVertex& v_adj: vertices[v])
        marks[v_adj] = true;

    for (const LinkedVertex& u_adj: vertices[u]){
        graph.removeNeighbour(u_adj);
        if (!marks[u_adj]){
            // u_adj is adjacent to u but not v
            graph.addEdge(u_adj, v);
            degrees[v] += 1;
            if (degrees[v] == 6)
                removeFromQ(qDeg5, v);
            else if (degrees[v] == 5)
                moveToQ(qDeg5, qDegLte4, v);
        }
        else{
            degrees[u_adj] -= 1;
            check(u_adj);
        }
    }
    for (const LinkedVertex& v_adj: vertices[v])
        marks[v_adj] = false;
    removeFromUnkownQ(u);
    removed.push(RemoveInfo(u,v));
    n_vertices -= 1;
}

void Linear5::removeFromQ(Linear5::Qtype& q, int vertex) {
    assert(qPointers[vertex].has_value());

    q.erase(qPointers[vertex].value());
    qPointers[vertex] = std::nullopt;

}

void Linear5::addToQ(Linear5::Qtype &q, int vertex) {
    q.push_back(vertex);
    qPointers[vertex] = --q.end();
}

void Linear5::moveToQ(Linear5::Qtype &to, Linear5::Qtype &from, int vertex) {
    assert(qPointers[vertex].has_value());

    from.erase(qPointers[vertex].value());
    to.push_back(vertex);
    qPointers[vertex] = --to.end();
}

void Linear5::removeFromUnkownQ(int vertex) {
    if (degrees[vertex] == 5)
        removeFromQ(qDeg5, vertex);
    else if (degrees[vertex] <= 4)
        removeFromQ(qDegLte4, vertex);
}


void testAllAlgorithmsCorrectness(Graph<LinkedVertex,LinkedVertexList> & graph){

    auto [n_colors, coloring] = greedyColoring(graph, true);
    assert(isCorrectColoring(graph, coloring));

    auto [n_colors2, coloring2] = dsaturColoring(graph);
    assert(isCorrectColoring(graph, coloring2));

    Graph<LinkedVertex, LinkedVertexList> graph_copy = graph;
    auto [n_colors3, coloring3] = colorLinear5(graph);
    assert(isCorrectColoring(graph, coloring3));
}



