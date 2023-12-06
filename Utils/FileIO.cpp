//
// Created by Owen on 2023/12/2.
//
#include "Utils.h"
#include <stdexcept>
#include <fstream>

unsigned int fileReader::readLittleUInt(int n) {
    if (n > 32) {
        throw std::runtime_error("n should be less than 32 (fileReader::readLittleUInt)");
    }

    while(inputBufferLength < n) {
        unsigned int temp = input.get(); // 从输入流中读取一个字节
        if (temp == EOF) {
            throw std::runtime_error("Reached end of file");
        }
        inputBuffer = (temp << inputBufferLength) | inputBuffer;
        inputBufferLength += 8;
    }
    inputBufferLength -= n;
    unsigned int result = (unsigned int) (inputBuffer & ((1ull << n) - 1));
    inputBuffer = inputBuffer >> n;
    if(n < 32) {
        result &= (1 << n) - 1;
    }
    return result;
}

int fileReader::readLittleSInt(unsigned int n) {
    return ((int)readLittleUInt(n) << (32 - n)) >> (32 - n);
}

unsigned int fileReader::readBigUInt(int n) {
    if (n > 32) {
        throw std::runtime_error("n should be less than 32 (fileReader::readBigUInt)");
    }

    while(inputBufferLength < n) {
        unsigned int temp = input.get(); // 从输入流中读取一个字节
        if (temp == EOF) {
            throw std::runtime_error("Reached end of file");
        }
        inputBuffer = (inputBuffer << 8) | temp;
        inputBufferLength += 8;
    }
    inputBufferLength -= n;
    unsigned int result = (unsigned int) (inputBuffer >> inputBufferLength);
    inputBuffer &= (1 << inputBufferLength) - 1;
    if(n < 32) {
        result &= (1 << n) - 1;
    }
    return result;
}

int fileReader::readBigSInt(unsigned int n) {
    return ((int)readBigUInt(n) << (32 - n)) >> (32 - n);
}

void fileReader::alignByte() {
    inputBufferLength = 0;
    inputBuffer = 0;
}

void fileReader::closeReader() {
    input.close();
}

void fileWriter::writeLittleInt(unsigned int data, int n) {
    if(n % 8 != 0) {
        throw std::runtime_error("n should be a multiple of 8 (fileWriter::writeLittleInt)");
    }

    for (int i = 0; i < n / 8; ++i) {
        output.put((char)(data >> (i * 8)));
    }
}

void fileWriter::writeBigInt(unsigned int data, int n) {
    if(n > 32){
        throw std::runtime_error("n should be less than 32 (fileWriter::writeBigInt)");
    }

    outputBuffer = (outputBuffer << n) | (data & ((1ull << n) - 1));
    outputBufferLength += n;
    while(outputBufferLength >= 8) {
        outputBufferLength -= 8;
        output.put((char)(outputBuffer >> outputBufferLength));
        outputBuffer &= (1 << outputBufferLength) - 1;
    }
}

void fileWriter::alignByte() {
    // This function should only be used in wav2flac
    if(outputBufferLength > 0) {
        writeBigInt(0, 8 - outputBufferLength);
    }
}

void fileWriter::closeWriter() {
    alignByte();
    output.flush();
    output.close();
}
