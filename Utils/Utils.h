//
// Created by Owen on 2023/12/2.
//
#ifndef GTWD_UTILS_H
#define GTWD_UTILS_H
#include<iostream>
class fileReader
{
public:
    std::istream input;

    fileReader(): input(nullptr)
    {
        throw std::runtime_error("Initializing Failed: no argument for fileReader");
    };
    int write();
    int readLittleInt(int n);
};

class fileWriter
{
public:
    std::ostream out;

    fileWriter(): out(nullptr)
    {
        throw std::runtime_error("Initializing Failed: no argument for fileWriter");
    };
};
#endif //GTWD_UTILS_H
