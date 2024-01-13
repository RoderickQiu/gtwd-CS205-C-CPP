//
// Created by Roderick Qiu on 2023/12/2.
//

#include "Wav2flac.h"
#include "FlacMetadata.h"
#include<fstream>

static const int BLOCK_SIZE = 4096;

void encodeSubframe(int samples[], unsigned long len, int sampleDepth, fileWriter &out) {
    out.writeBigInt(0, 1);
    out.writeBigInt(1, 6);
    out.writeBigInt(0, 1);
    for (int i = 0; i < len; i++) {
        // cout <<samples[i] << " " << sampleDepth << endl;
        out.writeBigInt(samples[i], sampleDepth);
    }
}

void encodeFrame(fileReader &in, fileWriter &out, unsigned int frameIndex, unsigned int numChannels,
                 unsigned int sampleDepth, unsigned int sampleRate,
                 int blockSize) {
    int samples[numChannels][blockSize];
    unsigned int bytesPerSample = sampleDepth / 8;
    for (int i = 0; i < blockSize; i++) {
        for (int ch = 0; ch < numChannels; ch++) {
            int val = 0;
            for (int j = 0; j < bytesPerSample; j++) {
                int b = in.readLittleUInt(8);
                if (b == -1) {
                    throw exception();
                }
                val |= b << (j * 8);
            }
            if (sampleDepth == 8) {
                samples[ch][i] = val - 128;
            } else {
                samples[ch][i] = (val << (32 - sampleDepth)) >> (32 - sampleDepth);
            }
        }
    }
    out.resetCRC();
    out.writeBigInt(0x3FFE, 14); // Sync code '11111111111110'
    out.writeBigInt(0, 1); // Reserved: mandatory value
    out.writeBigInt(0, 1); // Blocking strategy: fixed-blocksize stream
    out.writeBigInt(7, 4); // Block size in inter-channel samples: reserved
    out.writeBigInt(sampleRate % 10 == 0 ? 14 : 13, 4); // Sample rate
    out.writeBigInt(numChannels - 1, 4); // Channel assignment
    // 	Sample size in bits
    switch (sampleDepth) {
        case 16:
            out.writeBigInt(4, 3);
            break;
        case 24:
            out.writeBigInt(6, 3);
            break;
        default:
            throw runtime_error("Invalid Sample Depth");
    }
    out.writeBigInt(0, 1); // Reserved
    out.writeBigInt(0xFC | (frameIndex >> 30), 8); // "UTF-8" coded frame number
    for (int i = 24; i >= 0; i -= 6) {
        out.writeBigInt(0x80 | ((frameIndex >> i) & 0x3F), 8);
    }
    out.writeBigInt(blockSize - 1, 16);
    out.writeBigInt(sampleRate / (sampleRate % 10 == 0 ? 10 : 1), 16);
    out.writeBigInt(out.CRC8, 8);
    for (int i = 0; i < sizeof(samples) / sizeof(samples[0]); i++) {
        encodeSubframe(samples[i], sizeof(samples[0]) / sizeof(samples[0][0]), sampleDepth, out);
    }
    out.alignByte();
    out.writeBigInt(out.CRC16, 16);
}

void Wav2flac::encodeFile(fileReader &in, fileWriter &out) {
    cout << "Wav2flac::encodeFile: called" << endl;
    if (in.readBigUInt(32) != 0x52494646) {
        throw runtime_error("Invalid RIFF file header (Wav2flac::encodeFile)");
    }
    unsigned int sampleRate = 0;
    unsigned int numChannels = 0;
    unsigned int blockAlign = 0;
    unsigned int sampleDepth = 0;
    unsigned int bps = 0;
    unsigned int sampleDataLen = 0;
    unsigned int dataLength = in.readLittleUInt(32);
    // String "RIFF"
    if (in.readBigUInt(32) != 0x57415645) {
        throw runtime_error("Invalid WAV file header 'WAVE' (Wav2flac::encodeFile)");
    }
    // String "WAVE"
    if (in.readBigUInt(32) != 0x666d7420) {
        throw runtime_error("Unrecognized WAV file chunk 'fmt ' (Wav2flac::encodeFile)");
    }
    // String "fmt "
    if (in.readLittleUInt(32) != 16) {
        throw runtime_error("Invalid WAV file chunk size (Wav2flac::encodeFile)");
    }
    if (in.readLittleUInt(16) != 0x0001) {
        throw runtime_error("Invalid WAV file format (Wav2flac::encodeFile)");
    }
    numChannels = in.readLittleUInt(16);
    if (numChannels <= 0 || numChannels > 8) {
        throw runtime_error("Invalid WAV file channel count (Wav2flac::encodeFile)");
    }
    sampleRate = in.readLittleUInt(32);
    if (sampleRate <= 0 || sampleRate >= (1 << 20)) {
        throw runtime_error("Invalid WAV file sample rate (Wav2flac::encodeFile)");
    }
    bps = in.readLittleUInt(32);
    bps = bps >> 4;
    blockAlign = in.readLittleUInt(16);
    sampleDepth = in.readLittleUInt(16);
    if (sampleDepth == 0 || sampleDepth > 32 || sampleDepth % 8 != 0) {
        throw runtime_error("Unsupported WAV file sample depth");
    }
    if (in.readBigUInt(32) != 0x64617461) {
        throw runtime_error("Invalid WAV file data chunk header 'data' (Wav2flac::encodeFile)");
    }
    sampleDataLen = in.readLittleUInt(32);
    if (sampleDataLen <= 0 || sampleDataLen % (numChannels * (sampleDepth / 8)) != 0) {
        throw runtime_error("Invalid length of audio sample data");
    }
    int numSamples = sampleDataLen / (numChannels * (sampleDepth / 8));
    cout << "Writing flac\n";
    // Encoding FLAC file
    // header
    out.writeBigInt(0x664c6143, 32); // "fLaC"
    // METADATA_BLOCK
    // METADATA_BLOCK_HEADER
    out.writeBigInt(1, 1); // Last-metadata-block flag: not last
    out.writeBigInt(0, 7); // BLOCK_TYPE: 0 - streaminfo
    out.writeBigInt(34, 24); // Length (in bytes) of metadata to follow (not METADATA_BLOCK_HEADER)
    //METADATA_BLOCK_STREAMINFO
    out.writeBigInt(BLOCK_SIZE, 16);// minimum block size: constant
    out.writeBigInt(BLOCK_SIZE, 16);// maximum block size: constant
    out.writeBigInt(0, 24);// minimum frame size: 0 - unknown
    out.writeBigInt(0, 24);// maximum frame size: 0 - unknown
    out.writeBigInt(sampleRate, 20);// sample rate in Hz, max = 655350, 0 is invalid
    out.writeBigInt(numChannels - 1, 3); // (number of channels) - 1
    out.writeBigInt(sampleDepth - 1, 5); // (bits per sample) - 1
    out.writeBigInt(numSamples >> 18, 18); // Total samples in stream
    out.writeBigInt(numSamples, 18);
    // MD5 signature
    for (int i = 0; i < 16; i++) {
        out.writeBigInt(0, 8);
    }
    // VORBIS_COMMENT
    // METADATA_BLOCK_HEADER
    out.writeBigInt(1, 1); // last
    out.writeBigInt(4, 7); // vorbis comment block
    out.writeBigInt(0, 24); // block size
    // METADATA_BLOCK_VORBIS_COMMENT
    // https://www.xiph.org/vorbis/doc/v-comment.html
    out.writeLittleInt(40, 32); // vendor length
    out.writeStr("UTF-8"); // "UTF-8"
    out.writeLittleInt(1, 32); // user comment list length
    for (int i = 0; i < 1; i++) {
        string str = "TITLE = test";
        out.writeBigInt(8 * str.length(), 32);
        out.writeStr(str);
    }
    // FRAME
    for (int i = 0; numSamples > 0; i++) {
        unsigned int blockSize = min(numSamples, BLOCK_SIZE);
        encodeFrame(in, out, i, numChannels, sampleDepth, sampleRate, blockSize);
        numSamples -= blockSize;
    }
}


