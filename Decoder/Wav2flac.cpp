//
// Created by Roderick Qiu on 2023/12/2.
//

#include "Wav2flac.h"
#include<fstream>

void Wav2flac::encodeFile(fileReader &in, fileWriter &out) {
    cout << "Wav2flac::encodeFile: called" << endl;
    if(in.readBigUInt(32)!= 0x52494646){
        throw runtime_error("Invalid RIFF file header (Wav2flac::encodeFile)");
    }
    cout << "Wav2flac::encodeFile: valid WAV file header" <<endl;
    unsigned int sampleRate = 0;
    unsigned int channels = 0;
    unsigned int blockAlign = 0;
    unsigned int bitsPerSample = 0;
    unsigned int bps = 0;
    unsigned int totalSampleLength = 0;
    unsigned int dataLength = in.readLittleUInt(32);
    cout << "Wav2flac::encodeFile: data length = " << dataLength << " bits" <<  endl;
    if(in.readBigUInt(32)!= 0x57415645){
        throw runtime_error("Invalid WAV file header 'WAVE' (Wav2flac::encodeFile)");
    }
    cout << "Wav2flac::encodeFile: valid WAV file header 'WAVE'" <<endl;
    if(in.readBigUInt(32) != 0x666d7420){
        throw runtime_error("Unrecognized WAV file chunk 'fmt ' (Wav2flac::encodeFile)");
    }
    cout << "Wav2flac::encodeFile: valid WAV file chunk 'fmt '" <<endl;
    if(in.readLittleUInt(32)!= 16){
        throw runtime_error("Invalid WAV file chunk size (Wav2flac::encodeFile)");
    }
    cout << "Wav2flac::encodeFile: valid WAV file chunk size" <<endl;
    if(in.readLittleUInt(16)!= 1){
        throw runtime_error("Invalid WAV file format (Wav2flac::encodeFile)");
    }
    cout << "Wav2flac::encodeFile: valid WAV file format" <<endl;
    channels = in.readLittleUInt(16);
    if(channels < 0 || channels > 2){
        throw runtime_error("Invalid WAV file channel count (Wav2flac::encodeFile)");
    }
    cout << "Wav2flac::encodeFile: WAV file channel count = " << channels << endl;
    sampleRate = in.readLittleUInt(32);
    if(sampleRate <= 0 || sampleRate >= (1 << 20)){
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
    if(in.readBigUInt(32)!= 0x64617461){
        throw runtime_error("Invalid WAV file data chunk header 'data' (Wav2flac::encodeFile)");
    }
    cout << "Wav2flac::encodeFile: valid WAV file data chunk header 'data'" <<endl;
    totalSampleLength = in.readLittleUInt(32);
    cout << "Wav2flac::encodeFile: WAV file total sample length = " << totalSampleLength << " bits" << endl;
    for(int i = 0; i < totalSampleLength; i += blockAlign * channels){
        cout << "Wav2flac::encodeFile: #" << i / (blockAlign * channels) + 1;
        for(int j = 0; j < blockAlign * channels; j += blockAlign){
            cout << " channel: *" << j / blockAlign + 1 << " data = " << in.readLittleUInt(bitsPerSample);
        }
        cout << endl;
    }
}
