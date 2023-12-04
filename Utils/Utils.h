//
// Created by Owen on 2023/12/2.
//
#ifndef GTWD_UTILS_H
#define GTWD_UTILS_H
#include<iostream>
class fileReader{
    private:
        std::istream& input;
        unsigned long long inputBuffer;
        int inputBufferLength;

    public:
        fileReader(std::istream& inStream) : input(inStream) {
            inputBuffer = 0;
            inputBufferLength = 0;
        }
        int readLittleUInt(int n);
        int readBigUInt(int n);
};

class fileWriter{
    private:
        std::ostream& out;
    public:
        fileWriter(std::ostream& outStream) : out(outStream) {};
        int write();
};
#endif //GTWD_UTILS_H
