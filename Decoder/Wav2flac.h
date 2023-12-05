//
// Created by Roderick Qiu on 2023/12/2.
//

#ifndef GTWD_WAV2FLAC_H
#define GTWD_WAV2FLAC_H

#include<string>
#include<fstream>
#include "../Utils/Utils.h"

using namespace std;

class Wav2flac {
public:
    static string hello() {
        return "Hello from Wav2fLaC";
    }

    static void encodeFile(fileReader &in, fileWriter &out);

};


#endif //GTWD_WAV2FLAC_H
