//
// Created by Owen on 2023/12/2.
//
#include "Utils.h"
#include <stdexcept>
#include <fstream>
#include <string>
#include <sstream>

unsigned int fileReader::readLittleUInt(int n) {
    if (n > 32) {
        throw std::runtime_error("n should be less than 32 (fileReader::readLittleUInt)");
    }

    while (inputBufferLength < n) {
        unsigned int temp = input.get(); // one byte
        updateCRC8(temp);
        updateCRC16(temp);
        if (temp == EOF) {
            throw std::runtime_error("Reached end of file");
        }
        inputBuffer = (temp << inputBufferLength) | inputBuffer;
        inputBufferLength += 8;
    }
    inputBufferLength -= n;
    unsigned int result = (unsigned int) (inputBuffer & ((1ull << n) - 1));
    inputBuffer = inputBuffer >> n;
    if (n < 32) {
        result &= (1 << n) - 1;
    }
    return result;
}

int fileReader::readLittleSInt(int n) {
    return ((int) readLittleUInt(n) << (32 - n)) >> (32 - n);
}

unsigned int fileReader::readBigUInt(int n) {
    if (n > 32) {
        throw std::runtime_error("n should be less than 32 (fileReader::readBigUInt)");
    }

    while (inputBufferLength < n) {
        unsigned int temp = input.get(); // one byte
        updateCRC8(temp);
        updateCRC16(temp);
        if (temp == EOF) {
            throw std::runtime_error("Reached end of file");
        }
        inputBuffer = (inputBuffer << 8) | temp;
        inputBufferLength += 8;
    }
    inputBufferLength -= n;
    unsigned int result = (unsigned int) (inputBuffer >> inputBufferLength);
    inputBuffer &= (1 << inputBufferLength) - 1;
    if (n < 32) {
        result &= (1 << n) - 1;
    }
    return result;
}

int fileReader::readBigSInt(int n) {
    return ((int) readBigUInt(n) << (32 - n)) >> (32 - n);
}

void fileReader::alignByte() {
    inputBufferLength = 0;
    inputBuffer = 0;
}

void fileReader::closeReader() {
    input.close();
}

std::string fileReader::intToHex(int num) {
    std::stringstream stream;
    stream << std::hex << num;  // Convert decimal to hexadecimal
    return stream.str();  // Return the hexadecimal string
}

int fileReader::hexToInt(const std::string &str) {
    int ans;
    sscanf(str.c_str(), "%x", &ans);
    return ans;
}

void fileReader::updateCRC8(unsigned int data) {
    const unsigned int CRC8_POLY = 0x107;
    CRC8 ^= data;
    for (int i = 0; i < 8; ++i) {
        if (CRC8 & 0x80) {
            CRC8 = (CRC8 << 1) ^ CRC8_POLY;
        } else {
            CRC8 <<= 1;
        }
    }
}

bool fileReader::checkCRC8() {
    return CRC8 == 0;
}

void fileReader::resetCRC8() {
    CRC8 = 0;
}

void fileReader::updateCRC16(unsigned int data) {
    const unsigned int CRC16_POLY = 0x18005;
    CRC16 ^= data << 8;
    for (int i = 0; i < 8; ++i) {
        if (CRC16 & 0x8000) {
            CRC16 = (CRC16 << 1) ^ CRC16_POLY;
        } else {
            CRC16 <<= 1;
        }
    }
}

bool fileReader::checkCRC16() {
    return CRC16 == 0;
}

void fileReader::resetCRC16() {
    CRC16 = 0;
}

void fileWriter::writeLittleInt(unsigned int data, int n) {
    if (n % 8 != 0) {
        throw std::runtime_error("n should be a multiple of 8 (fileWriter::writeLittleInt)");
    }

    for (int i = 0; i < n / 8; ++i) {
        output.put((char) (data >> (i * 8)));
    }
}

void fileWriter::writeBigInt(unsigned int data, int n) {
    if (n > 32) {
        throw std::runtime_error("n should be less than 32 (fileWriter::writeBigInt)");
    }

    outputBuffer = (outputBuffer << n) | (data & ((1ull << n) - 1));
    outputBufferLength += n;
    while (outputBufferLength >= 8) {
        outputBufferLength -= 8;
        output.put((char) (outputBuffer >> outputBufferLength));
        outputBuffer &= (1 << outputBufferLength) - 1;
    }
}

void fileWriter::alignByte() {
    // This function should only be used in wav2flac
    if (outputBufferLength > 0) {
        writeBigInt(0, 8 - outputBufferLength);
    }
}

void fileWriter::closeWriter() {
    alignByte();
    output.flush();
    output.close();
}

void fileCopier::copyFile() {
    char temp;
    while (true){
        temp = (char)input.get();
        if (temp == EOF) {
            break;
        }
        output.put(temp);
    }
}

void fileCopier::closeCopier() {
    input.close();
    output.close();
}

void fileWriter::writeStr(std::string str) {
    for(int i = 0; i < str.length(); i++){
        fileWriter::writeBigInt((unsigned int)str[i], 8);
    }
}
