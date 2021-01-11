#ifndef MAP_COLORING_LINEAR5_H
#define MAP_COLORING_LINEAR5_H
#include <utility>
#include "../Graph.h"


template <typename T>
class Linear5;

// Returns:
// pair.first - number of colors used
// pair.second - vector mapping vertex:color
template <typename T>
inline std::pair<int, std::vector<int>> colorLinear5(const Graph<T> &graph) {
    Linear5 coloringClass(graph);
    return std::pair<int, std::vector<int>>(1,std::vector<int>(graph.getVertices().size()));
}


template <typename T>
class Linear5 {

public:
    std::vector<int> coloring;

    Linear5(const Graph<T> & g): graph(g) {}


    void reduce();
    void color();


private:
    Graph<T> & graph;

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
