#include <iostream>
#include "Decoder/Pcm2wav.h"
#include "Decoder/Wav2flac.h"
#include "Encoder/Flac2wav.h"
#include "Encoder/Wav2pcm.h"
#include "Libraries/clipp.h"

using namespace std;
using namespace clipp;

int main(int argc, char **argv) {
    string mode, input, output;
    auto cli = option("-m", "-M", "--mode") & (value("mode", mode),
            option("-i", "-I", "--input") & value("input", input),
            option("-o", "-O", "--output") & value("output", output));
    if (parse(argc, const_cast<char **>(argv), cli)) {
        cout << "Mode: " << mode << endl;
        cout << "Input: " << input << endl;
        cout << "Output: " << output << endl;
        if (mode == "w2f") { // wav to flac
            Wav2flac wav2flac;
            cout << wav2flac.hello() << endl;
        } else if (mode == "f2w") {// flac to wav
            Flac2wav flac2wav;
            cout << flac2wav.hello() << endl;
        } else if (mode == "w2p") {// wav to pcm
            Wav2pcm wav2pcm;
            cout << wav2pcm.hello() << endl;
        } else if (mode == "p2w") {// pcm to wav
            Pcm2wav pcm2wav;
            cout << pcm2wav.hello() << endl;
        } else if (mode == "f2p") {// flac to pcm
            Flac2wav flac2wav;
            cout << flac2wav.hello() << endl;
            Wav2pcm wav2pcm;
            cout << wav2pcm.hello() << endl;
        } else if (mode == "p2f") {// pcm to flac
            Pcm2wav pcm2wav;
            cout << pcm2wav.hello() << endl;
            Wav2flac wav2flac;
            cout << wav2flac.hello() << endl;
        } else {
            cout << "Invalid mode" << endl;
            return 1;
        }
    }
    return 0;
}