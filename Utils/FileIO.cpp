//
// Created by Owen on 2023/12/2.
//
#include "Utils.h"
#include <stdexcept>



/// 参数：
/// std::stream 比特数据流
/// int 读取数据位数
/// 功能：将输入流中的前n个字节的数据转化为int类型的数据
int fileReader::readLittleInt(int n) {
    int result = 0;
    for (int i = 0; i < n; i++) {
        int b = input.get(); // 从输入流中读取一个字节
        if (b == EOF) {
            throw std::runtime_error("Reached end of file");
        }
        result |= b << (i * 8); // 取in的前8 bit信息，
    }
    return result;
}