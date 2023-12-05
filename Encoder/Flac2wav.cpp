//
// Created by Roderick Qiu on 2023/12/2.
//

#include "Flac2wav.h"

void Flac2wav::decodeFile(fileReader &in, fileWriter &out) {
    out.writeLittleInt(0x46464952, 32); // RIFF
    if(in.readBigUInt(32) != 0x664c6143) {
        throw std::runtime_error("Not a flac file (Flac2wav::decodeFile)");
    }

    unsigned int maxBlockSize = -1;
    unsigned int minBlockSize = -1;
    unsigned int maxFrameSize = -1;
    unsigned int minFrameSize = -1;
    unsigned int sampleRate = -1;
    unsigned int numChannels = -1;
    unsigned int sampleDepth = -1;
    unsigned long long numSamples = -1;
    bool isLastBlock = false;
    while(!isLastBlock) {
        isLastBlock = in.readBigUInt(1);
        int blockType = in.readBigUInt(7);
        int blockSize = in.readBigUInt(24);
        if(blockType == 0) {
            // streamInfo block
            minBlockSize = in.readBigUInt(16);
            maxBlockSize = in.readBigUInt(16);
            minFrameSize = in.readBigUInt(24);
            maxFrameSize = in.readBigUInt(24);
            sampleRate = in.readBigUInt(20);
            numChannels = in.readBigUInt(3) + 1;
            sampleDepth = in.readBigUInt(5) + 1;
            numSamples = in.readBigUInt(20) << 20 | in.readBigUInt(16);
            in.readBigUInt(32); // MD5 -- ignore
            in.readBigUInt(32);
            in.readBigUInt(32);
            in.readBigUInt(32);
            cout<<"minBlockSize: "<<minBlockSize<<endl;
            cout<<"maxBlockSize: "<<maxBlockSize<<endl;
            cout<<"minFrameSize: "<<minFrameSize<<endl;
            cout<<"maxFrameSize: "<<maxFrameSize<<endl;
            cout<<"sampleRate: "<<sampleRate<<endl;
            cout<<"numChannels: "<<numChannels<<endl;
            cout<<"sampleDepth: "<<sampleDepth<<endl;
            cout<<"numSamples: "<<numSamples<<endl;
        } else {
            // discard metadata
            for (int i = 0; i < blockSize; ++i) {
                in.readBigUInt(8);
            }
        }
    }
}
