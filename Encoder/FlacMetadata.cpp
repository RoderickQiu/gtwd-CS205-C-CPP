//
// Created by Roderick Qiu on 2023/12/5.
//

#include "FlacMetadata.h"
#include <iostream>
#include <vector>

using namespace std;

void FlacMetadata::interpretFile(fileReader &in) {
    if (in.readBigUInt(32) != 0x664c6143) {
        throw runtime_error("Not a flac file (Flac2wav::decodeFile)");
    }

    unsigned int maxBlockSize = -1;
    unsigned int minBlockSize = -1;
    unsigned int maxFrameSize = -1;
    unsigned int minFrameSize = -1;
    unsigned int sampleRate = -1;
    unsigned int numChannels = -1;
    unsigned int sampleDepth = -1;
    unsigned long long numSamples = -1;
    string md5[4];
    bool isLastBlock = false;
    while (!isLastBlock) {
        isLastBlock = in.readBigUInt(1);
        int blockType = in.readBigUInt(7);
        int blockSize = in.readBigUInt(24);
        /*
         * 0 : STREAMINFO
         * 1 : PADDING
         * 2 : APPLICATION
         * 3 : SEEKTABLE
         * 4 : VORBIS_COMMENT
         * 5 : CUESHEET
         * 6 : PICTURE
         */
        if (blockType == 0) {//streaminfo
            minBlockSize = in.readBigUInt(16);
            maxBlockSize = in.readBigUInt(16);
            minFrameSize = in.readBigUInt(24);
            maxFrameSize = in.readBigUInt(24);
            sampleRate = in.readBigUInt(20);
            numChannels = in.readBigUInt(3) + 1;
            sampleDepth = in.readBigUInt(5) + 1;
            numSamples = in.readBigUInt(20) << 20 | in.readBigUInt(16);
            md5[0] = fileReader::intToHex(in.readBigUInt(32));
            md5[1] = fileReader::intToHex(in.readBigUInt(32));
            md5[2] = fileReader::intToHex(in.readBigUInt(32));
            md5[3] = fileReader::intToHex(in.readBigUInt(32));
            cout << "minBlockSize: " << minBlockSize << endl;
            cout << "maxBlockSize: " << maxBlockSize << endl;
            cout << "minFrameSize: " << minFrameSize << endl;
            cout << "maxFrameSize: " << maxFrameSize << endl;
            cout << "sampleRate: " << sampleRate << endl;
            cout << "numChannels: " << numChannels << endl;
            cout << "sampleDepth: " << sampleDepth << endl;
            cout << "numSamples: " << numSamples << endl;
            cout << "md5: " << md5[0] << md5[1] << md5[2] << md5[3] << endl;
        } else if (blockType == 4) {//vorbis_comment
            // Note that the 32-bit field lengths are little-endian coded according to the vorbis spec
            // as opposed to the usual big-endian coding of fixed-length integers in the rest of FLAC.
            unsigned int vendorLength = in.readLittleUInt(32);
            vector<unsigned int> vendor;
            vendor.reserve(vendorLength);
            for (int i = 0; i < vendorLength; ++i) {
                vendor.push_back(in.readLittleUInt(8));
            }
            std::string vendorString(vendor.begin(), vendor.end());
            cout << "vendor: " << vendorString << endl;
            unsigned int commentListLength = in.readLittleUInt(32);
            cout << "commentListLength: " << commentListLength << endl;
            for (int i = 0; i < commentListLength; ++i) {
                unsigned int commentLength = in.readLittleUInt(32);
                vector<unsigned int> comment;
                comment.reserve(commentLength);
                for (int j = 0; j < commentLength; ++j) {
                    comment.push_back(in.readLittleUInt(8));
                }
                std::string commentString(comment.begin(), comment.end());
                cout << "comment " << i << ": " << commentString << endl;
            }
        } else {
            for (int i = 0; i < blockSize; ++i) {
                in.readBigUInt(8);
            }
        }
    }
}