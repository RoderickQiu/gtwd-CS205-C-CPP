#include <iostream>
#include "Decoder/Pcm2wav.h"
#include "Decoder/Wav2flac.h"
#include "Encoder/Flac2wav.h"
#include "Encoder/Wav2pcm.h"
#include "Libraries/clipp.h"
#include "SimpleIni.h"

using namespace std;
using namespace clipp;

int main(int argc, char **argv) {
    string mode, input, output, config;
    auto cli = option("-m", "-M", "--mode") & (value("mode", mode),
            option("-i", "-I", "--input") & value("input", input),
            option("-o", "-O", "--output") & value("output", output),
            option("-c", "-C", "--config") & value("config", config));
    if (parse(argc, const_cast<char **>(argv), cli)) {
        cout << "Mode: " << mode << endl;
        cout << "Input: " << input << endl;
        cout << "Output: " << output << endl;
        if (mode == "w2f") { // wav to flac
            cout << Wav2flac::hello() << endl;
        } else if (mode == "f2w") {// flac to wav
            cout << Flac2wav::hello() << endl;
        } else if (mode == "w2p") {// wav to pcm
            cout << Wav2pcm::hello() << endl;
        } else if (mode == "p2w") {// pcm to wav
            cout << Pcm2wav::hello() << endl;
            PcmConfig pcmConfig;
            CSimpleIniA ini;
            ini.SetUnicode();
            SI_Error rc = ini.LoadFile(config.c_str());
            if (rc < 0) {
                throw "Error loading pcmConfig file";
            } else {
                pcmConfig.channels = (int) ini.GetLongValue("STREAM INFO", "channels");
                pcmConfig.depth = (int) ini.GetLongValue("STREAM INFO", "depth");
                pcmConfig.sample_rate = (int) ini.GetLongValue("STREAM INFO", "sample_rate");
            }
            cout << "CHANNEL: " << pcmConfig.channels << endl;
            cout << "DEPTH: " << pcmConfig.depth << endl;
            cout << "SAMPLE RATE: " << pcmConfig.sample_rate << endl;
        } else if (mode == "f2p") {// flac to pcm
            cout << Flac2wav::hello() << endl;
            cout << Wav2pcm::hello() << endl;
        } else if (mode == "p2f") {// pcm to flac
            cout << Pcm2wav::hello() << endl;
            cout << Wav2flac::hello() << endl;
        } else {
            cout << "Invalid mode" << endl;
            return 1;
        }
    }
    return 0;
}