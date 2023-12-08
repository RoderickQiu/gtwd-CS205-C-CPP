//
// Created by Roderick Qiu on 2023/12/2.
//

#ifndef GTWD_PCM2WAV_H
#define GTWD_PCM2WAV_H

#include<string>

using namespace std;

typedef struct pcmConfig {
    int sample_rate;
    int depth;
    int channels;
} PcmConfig;

class Pcm2wav {
public:
    static string hello() {
        return "Hello from Pcm2wav";
    }
};


#endif //GTWD_PCM2WAV_H
