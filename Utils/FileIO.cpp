//
// Created by Owen on 2023/12/2.
//
#include "Utils.h"
#include <stdexcept>



/// 参数：
/// std::stream 比特数据流
/// int 读取数据位数（不可超过32）
/// 功能：将输入流中的前n个字节的数据转化为int类型的数据
int fileReader::readLittleUInt(int n) {
    if (n > 32) {
        throw std::runtime_error("n should be less than 32 (fileReader::readLittleUInt)");
    }

    while(inputBufferLength < n) {
        int temp = input.get(); // 从输入流中读取一个字节
        if (temp == EOF) {
            throw std::runtime_error("Reached end of file");
        }
        inputBuffer = (inputBuffer << 8) | temp;
        inputBufferLength += 8;
    }
    inputBufferLength -= n;
    int result = (int)inputBuffer >> inputBufferLength;
    if(n < 32) {
        result &= (1 << n) - 1;
    }
    return result;
}

int fileReader::readBigUInt(int n) {
    if (n > 32) {
        throw std::runtime_error("n should be less than 32 (fileReader::readBigUInt)");
    }

    while(inputBufferLength < n) {
        int temp = input.get(); // 从输入流中读取一个字节
        if (temp == EOF) {
            throw std::runtime_error("Reached end of file");
        }
        inputBuffer = (temp << inputBufferLength) | inputBuffer;
        inputBufferLength += 8;
    }
    inputBufferLength -= n;
    int result = (int)inputBuffer >> inputBufferLength;
    if(n < 32) {
        result &= (1 << n) - 1;
    }
    return result;
}
