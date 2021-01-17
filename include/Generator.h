#ifndef MAP_COLORING_GENERATOR_H
#define MAP_COLORING_GENERATOR_H

#include <string>
#include <vector>

class Generator{


public:

    static const std::string GENERATOR_FILE;
    static const std::string OUTPUT_FILE;
    static bool squelchStderr;
    static std::string graphTypes;
    static const int MAX_N_VERT = 90;


    static void generate(int nVert, int maxGraphs);

};

#endif //MAP_COLORING_GENERATOR_H
