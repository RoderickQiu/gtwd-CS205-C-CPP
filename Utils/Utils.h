//
// Created by Owen on 2023/12/2.
//
#ifndef GTWD_UTILS_H
#define GTWD_UTILS_H
#include<iostream>
class fileReader{
    private:
        std::ifstream& input;
        unsigned long long inputBuffer;
        int inputBufferLength;

    public:
        fileReader(std::ifstream& inStream) : input(inStream) {
            inputBuffer = 0;
            inputBufferLength = 0;
        }
        unsigned int readLittleUInt(int n);
        unsigned int readBigUInt(int n);
        int readLittleSInt(int n);
        int readBigSInt(int n);
        void alignByte();
        void closeReader();
};

class fileWriter{
    private:
        std::ostream& out;
    public:
        fileWriter(std::ostream& outStream) : out(outStream) {};
        int write();
};
#endif //GTWD_UTILS_H
