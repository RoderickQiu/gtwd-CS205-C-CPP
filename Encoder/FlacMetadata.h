//
// Created by Roderick Qiu on 2023/12/5.
//

#ifndef GTWD_FLACMETADATA_H
#define GTWD_FLACMETADATA_H

#include <string>
#include <vector>
#include "Utils.h"

using namespace std;

class FlacMetadata {
public:
    typedef struct metadataFlac {
        unsigned int infoBlockSize = -1;
        unsigned int maxBlockSize = -1;
        unsigned int minBlockSize = -1;
        unsigned int maxFrameSize = -1;
        unsigned int minFrameSize = -1;
        unsigned int sampleRate = -1;
        unsigned int numChannels = -1;
        unsigned int sampleDepth = -1;
        unsigned long long numSamples = -1;
        string md5[4];
        unsigned int commentBlockSize = 0;
        string vendorString;
        vector<unsigned int> vendorStringOriginal;
        unsigned int commentListLength = 0;
        vector<string> commentList;
        vector<vector<unsigned int>> commentsOriginal;
    } FlacMetadataInfo;

    typedef struct metaEditInfo {
        bool modifyVendorString = false;
        string newVendorString;
        bool modifyComment = false;
        int modifyCommentIndex = -1;
        string newComment;
        bool appendComment = false;
        bool removeComment = false;
        int removeCommentIndex = -1;
    } MetaEditInfo;

    static FlacMetadata::FlacMetadataInfo interpretFile(fileReader &in);

    static void editFile(fileReader &in, fileWriter &out, FlacMetadata::MetaEditInfo editInfo);
};


#endif //GTWD_FLACMETADATA_H