//
// Created by Roderick Qiu on 2023/12/2.
//

#ifndef GTWD_FLAC2WAV_H
#define GTWD_FLAC2WAV_H

#include<string>
#include<fstream>
#include"Utils.h"
using namespace std;

class Flac2wav {
    public:
        static string hello() {
            return "Hello from fLaC2Wav";
        }
        static void decodeFile(fileReader& in, fileWriter& out);

};


#endif //GTWD_FLAC2WAV_H
