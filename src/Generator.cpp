
#include <Generator.h>

#include <stdexcept>

const std::string Generator::GENERATOR_FILE = "generator.exe";
const std::string Generator::OUTPUT_FILE = "graphs.bin";
bool Generator::squelchStderr = false;
//std::string Generator::graphTypes = "-p -c1 -m1"; // min connectivity = 1, min degree = 1;
std::string Generator::graphTypes = "-h -p";

void Generator::generate(int nVert, int maxGraphs) {
//    if (nVert > MAX_N_VERT){
//        throw std::runtime_error("Max possible number of vertices is: " + std::to_string(MAX_N_VERT));
//    }

    std::string squelcher = "";
    if (squelchStderr) {
#ifdef _WIN32
        squelcher = " 2 > nul";
#else
        squelcher = " 2 > dev/null";
#endif
    }


    auto s_nVert = std::to_string(nVert);
    auto s_maxGraphs = std::to_string(maxGraphs);
    std::string generatorCommand(
            GENERATOR_FILE + " mylimit " + s_maxGraphs +
            " " + s_nVert + " " + graphTypes + " " + OUTPUT_FILE);
    system(generatorCommand.c_str());
}
