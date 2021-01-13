#ifndef MAP_COLORING_LINEAR5_H
#define MAP_COLORING_LINEAR5_H
#include <utility>
#include "../Graph.h"


template <typename T>
class Linear5;

// Returns:
// a tuple {int: n of colors used, vector<int>: vector mapping vertex:color}
template <typename T>
inline std::tuple<int, std::vector<int>> colorLinear5(const Graph<T> &graph) {
    Linear5 coloringClass(graph);
    return {1, std::vector<int>(graph.getVertices().size())};
}


template <typename T>
class Linear5 {

private:
    const Graph<T> & graph;
    std::vector<int> q_deg5;
    std::vector<int> q_deg_lte4;

public:
    std::vector<int> coloring;

    Linear5(const Graph<T> & g):graph(g)  {}


    void reduce();
    void color();


private:
    void check(int vertex);
    void identify(int u, int v);

};


template<typename T>
void Linear5<T>::check(int vertex) {

}

template<typename T>
void Linear5<T>::identify(int u, int v) {

}

template<typename T>
void Linear5<T>::reduce() {

}

template<typename T>
void Linear5<T>::color() {

}




#endif //MAP_COLORING_LINEAR5_H
