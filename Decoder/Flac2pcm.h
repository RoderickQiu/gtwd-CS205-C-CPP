//
// Created by Owen on 2024/1/15.
//

#ifndef GTWD_FLAC2PCM_H
#define GTWD_FLAC2PCM_H

#include<string>
#include<fstream>
#include"Utils.h"

class Flac2pcm {
public:
    static void decodeFile(fileReader &in, fileWriter &out);

};

#endif //GTWD_FLAC2PCM_H
