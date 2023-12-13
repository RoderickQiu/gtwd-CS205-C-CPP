#ifndef TEMP_WAV_AIFF2WAV_H
#define TEMP_WAV_AIFF2WAV_H

#include "../Utils/Utils.h"

class Aiff2wav {
public:
    static void encodeFile(fileReader &in, fileWriter &out);
};


#endif //TEMP_WAV_AIFF2WAV_H
