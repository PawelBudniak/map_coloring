// Analysis of map coloring algorithms
// Authors: Pawel Budniak, Michal Swiatek

#ifndef MAP_COLORING_COMMANDLINEPARSER_H
#define MAP_COLORING_COMMANDLINEPARSER_H

#include <string>

class CommandLineParser
{
public:
    enum Mode {MANUAL, GENERATOR, TEST, INVALID};
    enum Algorithm {GREEDY, DSATUR, LINEAR5};
    enum Param {A, N, K, R, STEP, P};

public:
    CommandLineParser(int argc, char** argv);

    [[nodiscard]] Mode getMode() const { return mode; }
    [[nodiscard]] Algorithm getAlgorithm() const { return algorithm; }

    [[nodiscard]] int getParam(Param param) const;

    void printParams() const;

private:
    template<class T, class... Args>
    bool matches(const T& test, Args&&... match)
    {
        return ((test == match) || ...);
    }

    void setMode(std::string&& param);
    void parseCommands(int argc, char** argv);
    void readParam(Param param, const std::string& str);

    void printHelp() const;

    Mode mode{INVALID};
    Algorithm algorithm{GREEDY};

    int n{};
    int k{};
    int r{};
    int step{};

    bool print = false;
};

#endif //MAP_COLORING_COMMANDLINEPARSER_H
