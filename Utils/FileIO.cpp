//
// Created by Owen on 2023/12/2.
//
#include "Utils.h"
#include <stdexcept>
#include <fstream>


/// 参数：
/// std::stream 比特数据流
/// int 读取数据位数（不可超过32）
/// 功能：将输入流中的前n个字节的数据转化为int类型的数据
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
    unsigned int result = (unsigned int) inputBuffer & (((unsigned long long)1 << n) - 1);
    inputBuffer = inputBuffer >> n;
    if(n < 32) {
        result &= (1 << n) - 1;
    }
    return result;
}

int fileReader::readLittleSInt(int n) {
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
    unsigned int result = (unsigned int)inputBuffer >> inputBufferLength;
    inputBuffer &= (1 << inputBufferLength) - 1;
    if(n < 32) {
        result &= (1 << n) - 1;
    }
    return result;
}

int fileReader::readBigSInt(int n) {
    return ((int)readBigUInt(n) << (32 - n)) >> (32 - n);
}

void fileReader::alignByte() {
    inputBufferLength = 0;
    inputBuffer = 0;
}

void fileReader::closeReader() {
    input.close();
}
