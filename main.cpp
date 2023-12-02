#include <iostream>
#include "Decoder/Wav2flac.h"
#include "Encoder/Flac2wav.h"

using namespace std;

int main() {
    cout << "Welcome to the FLAC2WAV converter!" << endl;
    cout << Flac2wav::hello() << endl;
    cout << Wav2flac::hello() << endl;
    return 0;
}