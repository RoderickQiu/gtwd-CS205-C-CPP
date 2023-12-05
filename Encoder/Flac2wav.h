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
        ifstream input("../ProvidedDocuments/ovs.wav", ios::in|ios::binary);
        if(!input.is_open()) {
            cout<<"Error opening input file"<<endl;
        }

        fileReader reader(input);
        cout<<(char)reader.readBigUInt(8)<<endl;
        cout<<(char)reader.readBigUInt(8)<<endl;
        cout<<(char)reader.readBigUInt(8)<<endl;
        cout<<(char)reader.readBigUInt(8)<<endl;
        cout<<reader.readLittleUInt(16)<<endl;
        cout<<reader.readLittleUInt(16)<<endl;

        reader.closeReader();
        return "Hello from fLaC2Wav";
    }
};


#endif //GTWD_FLAC2WAV_H
