// Analysis of map coloring algorithms
// Authors: Pawel Budniak, Michal Swiatek

#include "CommandLineParser.h"

#include <iostream>

CommandLineParser::CommandLineParser(int argc, char **argv)
{
    if (argc < 2 || !matches(std::string(argv[1]), "-m1", "-m2", "-m3"))
    {
        printHelp();
        exit(0);
    }

    setMode(argv[1]);
    parseCommands(argc, argv);
}

void CommandLineParser::setMode(std::string &&param)
{
    if (param == "-m1")
        mode = MANUAL;
    else if (param == "-m2")
        mode = GENERATOR;
    else if (param == "-m3")
        mode = TEST;
}

void CommandLineParser::parseCommands(int argc, char** argv)
{
    for (int i = 2; i < argc; ++i)
    {
        auto cmd = std::string(argv[i]);

        if (cmd == "-a" && ++i < argc)
            readParam(A, argv[i]);
        else if (cmd == "-n" && ++i < argc)
            readParam(N, argv[i]);
        else if (cmd == "-r" && ++i < argc)
            readParam(R, argv[i]);
        else if (cmd == "-k" && ++i < argc)
            readParam(K, argv[i]);
        else if (cmd == "-step" && ++i < argc)
            readParam(STEP, argv[i]);
        else if (cmd == "-p")
            print = true;
        else
        {
            printHelp();
            exit(0);
        }
    }
}

void CommandLineParser::readParam(Param param, const std::string& str)
{
    if (param == A)
    {
        if (str == "greedy")
            algorithm = GREEDY;
        else if (str == "dsatur")
            algorithm = DSATUR;
        else if (str == "linear5")
            algorithm = LINEAR5;
        else
        {
            printHelp();
            exit(0);
        }

        return;
    }

    int temp;
    try {
        temp = std::stoi(str);
    }
    catch (std::exception& e) {
        printHelp();
        exit(0);
    }

    switch (param) {
        case N:     n = temp;     break;
        case K:     k = temp;     break;
        case R:     r = temp;     break;
        case STEP:  step = temp;  break;
        case A:                   break;
    }
}

int CommandLineParser::getParam(Param param) const
{
    switch (param) {
        case N:     return n;
        case K:     return k;
        case R:     return r;
        case STEP:  return step;
        case P:     return print;
        default:    return -1;
    }
}

void CommandLineParser::printParams() const
{
    std::cout << "Program parameters:\n";

    switch (mode) {
        case MANUAL:        std::cout << "Mode: manual, ";      break;
        case GENERATOR:     std::cout << "Mode: generator, ";   break;
        case TEST:          std::cout << "Mode: test, ";        break;
        case INVALID:       printHelp();    exit(0);
    }

    switch (algorithm) {
        case GREEDY:        std::cout << "a: greedy, ";         break;
        case DSATUR:        std::cout << "a: dsatur, ";         break;
        case LINEAR5:       std::cout << "a: linear5, ";        break;
    }

    std::cout << "n: " << n << ", k: " << k << ", r: " << r << ", step: " << step << '\n';
}

void CommandLineParser::printHelp() const
{
    std::cout << "\nProgram usage:\n";
    std::cout << "program_name [-m1|-m2|-m3] [params...]\n\n";

    std::cout << "Mode flag must be present and has to be first flag in command line after program name.\n";
    std::cout << "Supported modes:\n\n";
    std::cout << "-m1\tMANUAL mode, take input from standard input in form: n k\n";
    std::cout << "   \tk pairs of vertices defining unique edge (vertices start at 0)\n";
    std::cout << "-m2\tGENERATOR mode, generate instance of problem and solve it\n";
    std::cout << "-m3\tTEST mode, generate k instances of problem starting at n and increasing number of vertices by step\n";
    std::cout << "   \teach instance of a problem has a batch of r graphs, results are an averge over single batch, time given in microseconds.\n";
    std::cout << "   \tRequires n,k,r,step flags set (see below for explanation)\n";

    std::cout << "\n\n";

    std::cout << "Following flags can be given in an arbitrary order, parameters should be separated with whitespace.\n";
    std::cout << "Supported flags:\n\n";
    std::cout << "-n\tnumber of vertices used in generated problem\n";
    std::cout << "-k\tnumber of iterations used in test mode\n";
    std::cout << "-r\tnumber of graphs in single test batch for test mode\n";
    std::cout << "-step\tnumber of vertices is increased by that value in each iteration of test mode\n";
    std::cout << "-a\talgorithm used in MANUAL or GENERATOR mode, by default set to greedy. Possible values: greedy dsatur linear5\n";
    std::cout << "-p\tprints graph to standard output, doesn't take parameter\n";
}
