#include <iostream>
#include "Decoder/Pcm2wav.h"
#include "Decoder/Wav2flac.h"
#include "Encoder/Flac2wav.h"
#include "Encoder/Wav2pcm.h"
#include "Encoder/FlacMetadata.h"
#include "Libraries/clipp.h"
#include "Libraries/SimpleIni.h"

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
            ifstream input("./ProvidedDocuments/ovs.wav", ios::in | ios::binary);
            ofstream output("./ProvidedDocuments/out.flac", ios::out | ios::binary);
            if(!input.is_open()) {
                cout<<"Error opening input file"<<endl;
                return 1;
            }
            if(!output.is_open()) {
                cout<<"Error opening output file"<<endl;
                return 1;
            }
            fileReader in = fileReader(input);
            fileWriter out = fileWriter(output);
            Wav2flac::encodeFile(in, out);
        } else if (mode == "f2w") { // flac to wav
            ifstream inputFile(input, ios::in | ios::binary);
            ofstream outputFile(output, ios::out | ios::trunc | ios::binary);
            if (!inputFile.is_open()) {
                cout << "Error opening input file" << endl;
                return 1;
            }
            if (!outputFile.is_open()) {
                cout << "Error opening output file" << endl;
                return 1;
            }
            fileReader reader(inputFile);
            fileWriter writer(outputFile);
            try {
                Flac2wav::decodeFile(reader, writer);
            } catch (exception &e) {
                cout << e.what() << endl;
            }
            reader.closeReader();
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
        } else if (mode == "fmeta") {
            ifstream inputFile(input, ios::in | ios::binary);
            if (inputFile.is_open()) {
                fileReader inStream = fileReader(inputFile);
                FlacMetadata::interpretFile(inStream);
                inputFile.close();
            } else {
                throw runtime_error("Error opening file (main::main)");
            }
        } else {
            cout << "Invalid mode" << endl;
            return 1;
        }
    }
    return 0;
}