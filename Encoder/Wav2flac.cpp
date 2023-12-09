//
// Created by Roderick Qiu on 2023/12/2.
//

#include "Wav2flac.h"
#include "FlacMetadata.h"
#include<fstream>

void setMetadata();

void Wav2flac::encodeFile(fileReader &in, fileWriter &out) {
    cout << "Wav2flac::encodeFile: called" << endl;
    if (in.readBigUInt(32) != 0x52494646) {
        throw runtime_error("Invalid RIFF file header (Wav2flac::encodeFile)");
    }
    cout << "Wav2flac::encodeFile: valid WAV file header" << endl;
    unsigned int sampleRate = 0;
    unsigned int channels = 0;
    unsigned int blockAlign = 0;
    unsigned int bitsPerSample = 0;
    unsigned int bps = 0;
    unsigned int totalSampleLength = 0;
    unsigned int dataLength = in.readLittleUInt(32);
    cout << "Wav2flac::encodeFile: data length = " << dataLength << " bits" << endl;
    if (in.readBigUInt(32) != 0x57415645) {
        throw runtime_error("Invalid WAV file header 'WAVE' (Wav2flac::encodeFile)");
    }
    cout << "Wav2flac::encodeFile: valid WAV file header 'WAVE'" << endl;
    if (in.readBigUInt(32) != 0x666d7420) {
        throw runtime_error("Unrecognized WAV file chunk 'fmt ' (Wav2flac::encodeFile)");
    }
    cout << "Wav2flac::encodeFile: valid WAV file chunk 'fmt '" << endl;
    if (in.readLittleUInt(32) != 16) {
        throw runtime_error("Invalid WAV file chunk size (Wav2flac::encodeFile)");
    }
    cout << "Wav2flac::encodeFile: valid WAV file chunk size" << endl;
    if (in.readLittleUInt(16) != 1) {
        throw runtime_error("Invalid WAV file format (Wav2flac::encodeFile)");
    }
    cout << "Wav2flac::encodeFile: valid WAV file format" << endl;
    channels = in.readLittleUInt(16);
    if (channels <= 0 || channels > 2) {
        throw runtime_error("Invalid WAV file channel count (Wav2flac::encodeFile)");
    }
    cout << "Wav2flac::encodeFile: WAV file channel count = " << channels << endl;
    sampleRate = in.readLittleUInt(32);
    if (sampleRate <= 0 || sampleRate > 655350) {
        throw runtime_error("Invalid WAV file sample rate (Wav2flac::encodeFile)");
    }
    cout << "Wav2flac::encodeFile: WAV file sample rate = " << sampleRate << " Hz" << endl;
    bps = in.readLittleUInt(32);
    bps = bps >> 4;
    cout << "Wav2flac::encodeFile: WAV file bits per second = " << bps << " bits" << endl;
    blockAlign = in.readLittleUInt(16);
    cout << "Wav2flac::encodeFile: WAV file blockAlign = " << blockAlign << " bytes" << endl;
    bitsPerSample = in.readLittleUInt(16);
    cout << "Wav2flac::encodeFile: WAV file bits per sample = " << bitsPerSample << " bits" << endl;
    if (in.readBigUInt(32) != 0x64617461) {
        throw runtime_error("Invalid WAV file data chunk header 'data' (Wav2flac::encodeFile)");
    }
    cout << "Wav2flac::encodeFile: valid WAV file data chunk header 'data'" << endl;
    totalSampleLength = in.readLittleUInt(32);
    cout << "Wav2flac::encodeFile: WAV file total sample length = " << totalSampleLength << " bits" << endl;
    /*
    for (int i = 0; i < totalSampleLength; i += blockAlign * channels) {
        cout << "Wav2flac::encodeFile: #" << i / (blockAlign * channels) + 1;
        for (int j = 0; j < blockAlign * channels; j += blockAlign) {
            cout << " channel: *" << j / blockAlign + 1 << " data = " << in.readLittleUInt(bitsPerSample);
        }
        cout << endl;
    }*/
    // Encoding FLAC file
    // header
    out.writeBigInt(0x664c6143, 32); // "fLaC"

    // Metadata block
    // SREAMINFO
    out.writeBigInt(0, 1); // not last
    out.writeBigInt(0, 7); // stream info block
    out.writeBigInt(272, 24);// block size
    out.writeBigInt(0, 16);// minimum block size ?
    out.writeBigInt(0, 16);// maximum block size ?
    out.writeBigInt(0, 24);// minimum frame size ?
    out.writeBigInt(0, 24);// maximum frame size ?
    out.writeBigInt(sampleRate, 20);// sample rate in Hz, max = 655350, 0 is invalid
    out.writeBigInt(channels, 3); // number of channels
    out.writeBigInt(bitsPerSample, 5); // bits per sample
    out.writeBigInt(totalSampleLength, 36); // total samples per stream
    out.writeBigInt(0, 128); // MD5 signature ?

    // VORBIS_COMMENT
    out.writeBigInt(1, 1); // last
    out.writeBigInt(1, 7); // vorbis comment block
    out.writeBigInt(0, 24); // block size
    out.writeLittleInt(40, 32); // vendor length
    out.writeStr("UTF-8"); // "UTF-8"
    out.writeLittleInt(1, 32); // user comment list length
//    out.writeStr()

    out.writeBigInt(0, 1); // last block
    out.writeBigInt(0, 7);
    out.writeBigInt(0, 24);
}

risComment()