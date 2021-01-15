/*  
 *  Created by michal-swiatek on 15.01.2021.
 *
 *  Github: https://github.com/michal-swiatek
 */

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
}
