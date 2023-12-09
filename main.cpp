#include <iostream>
#include <chrono>
#include "Pcm2wav.h"
#include "Wav2flac.h"
#include "Flac2wav.h"
#include "Wav2pcm.h"
#include "Wav2aiff.h"
#include "FlacMetadata.h"
#include "clipp.h"
#include "SimpleIni.h"

using namespace std;
using namespace clipp;

string parseMode(const string &mode) {
    if (mode == "w2f") {
        return "wav to flac";
    } else if (mode == "f2w") {
        return "flac to wav";
    } else if (mode == "f2p") {
        return "flac to pcm";
    } else if (mode == "p2f") {
        return "pcm to flac";
    } else if (mode == "p2w") {
        return "pcm to wav";
    } else if (mode == "w2p") {
        return "wav to pcm";
    } else if (mode == "fm") {
        return "flac metadata";
    } else if (mode == "fe") {
        return "flac metadata edit";
    } else if (mode == "a2w") {
        return "aiff to wav";
    } else if (mode == "w2a") {
        return "wav to aiff";
    }
    return "invalid mode";
}

string generateRandomString(int length);

int main(int argc, char **argv) {
    string mode, input, output, config;
    string modifyVendor, modifyComment, modifyCommentIndex, appendComment, removeCommentIndex;
    auto cli = (
            option("-m", "-M", "--mode") & value("mode", mode),
                    option("-i", "-I", "--input") & value("input", input),
                    option("-o", "-O", "--output") & value("output", output),
                    option("-c", "-C", "--config") & value("config", config),
                    option("-mv", "--modify-vendor") & value("modifyVendor", modifyVendor),
                    option("-mc", "--modify-comment") & value("modifyComment", modifyComment)
                    & value("commentIndex", modifyCommentIndex),
                    option("-ac", "--append-comment") & value("appendComment", appendComment),
                    option("-rc", "--remove-comment") & value("removeComment", removeCommentIndex)
    );
    if (parse(argc, const_cast<char **>(argv), cli)) {
        cout << "Mode: " << parseMode(mode) << endl;
        if (!input.empty())
            cout << "Input: " << input << endl;
        if (!output.empty())
            cout << "Output: " << output << endl;
        if (mode == "w2f") { // wav to flac
            ifstream inputFile(input, ios::in | ios::binary);
            ofstream outputFile(output, ios::out | ios::trunc | ios::binary);
            if (!inputFile.is_open()) {
                throw runtime_error("Error opening input file");
            }
            if (!outputFile.is_open()) {
                throw runtime_error("Error opening output file");
            }
            fileReader reader(inputFile);
            fileWriter writer(outputFile);
            Wav2flac::encodeFile(reader, writer);
            reader.closeReader();
            writer.closeWriter();
        } else if (mode == "f2w") { // flac to wav
            char tmpname[FILENAME_MAX];
            char *tev = getenv("TMPDIR");
            if (!tev) tev = getenv("TEMP");
            strcpy(tmpname, tev);
#ifdef WIN32
            strcat(tmpname, "\\");
            strcat(tmpname, generateRandomString(7).c_str());
#else
            strcat(tmpname, "XXXXXXX");
#endif
            int fd = mkstemp(tmpname);
            cout << "Temp file: " << tmpname << fd << endl;
            ifstream inputFile(input, ios::in | ios::binary);
            ofstream tempOutputFile(tmpname, ios::out | ios::trunc | ios::binary);
            try {
                if (!inputFile.is_open()) { throw runtime_error("Error opening input file"); }
                if (!tempOutputFile.is_open()) { throw runtime_error("Error opening temp output file"); }
                fileReader reader(inputFile);
                fileWriter writer(tempOutputFile);
                Flac2wav::decodeFile(reader, writer);
                reader.closeReader();
                writer.closeWriter();
                ifstream tempInputFile(tmpname, ios::in | ios::binary);
                ofstream outputFile(output, ios::out | ios::trunc | ios::binary);
                if (!tempInputFile.is_open()) { throw runtime_error("Error opening temp input file"); }
                if (!outputFile.is_open()) { throw runtime_error("Error opening output file"); }
                fileCopier copier(tempInputFile, outputFile);
                copier.copyFile();
                copier.closeCopier();
            } catch (exception &e) {
                remove(tmpname);
                throw runtime_error(e.what());
            }
            remove(tmpname);
        } else if (mode == "w2p") {// wav to pcm
            cout << Wav2pcm::hello() << endl;
        } else if (mode == "w2a") {// wav to aiff
            ifstream inputFile(input, ios::in | ios::binary);
            ofstream outputFile(output, ios::out | ios::trunc | ios::binary);
            if (!inputFile.is_open()) {
                throw runtime_error("Error opening input file");
            }
            if (!outputFile.is_open()) {
                throw runtime_error("Error opening output file");
            }
            fileReader reader(inputFile);
            fileWriter writer(outputFile);
            Wav2aiff::encodeFile(reader, writer);
            reader.closeReader();
            writer.closeWriter();
        } else if (mode == "p2w") {// pcm to wav
            cout << Pcm2wav::hello() << endl;
            PcmConfig pcmConfig;
            CSimpleIniA ini;
            ini.SetUnicode();
            SI_Error rc = ini.LoadFile(config.c_str());
            if (rc < 0) {
                throw runtime_error("Error loading config file");
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
        } else if (mode == "fm") {
            ifstream inputFile(input, ios::in | ios::binary);
            if (inputFile.is_open()) {
                fileReader inStream = fileReader(inputFile);
                FlacMetadata::interpretFile(inStream);
                inputFile.close();
            } else {
                throw runtime_error("Error opening input file (main::main)");
            }
        } else if (mode == "fe") {
            ifstream inputFile(input, ios::in | ios::binary);
            ofstream outputFile(output, ios::out | ios::trunc | ios::binary);
            if (!inputFile.is_open()) {
                throw runtime_error("Error opening input file (main::main)");
            }
            if (!outputFile.is_open()) {
                throw runtime_error("Error opening output file (main::main)");
            }

            FlacMetadata::MetaEditInfo metaEditInfo;
            if (!modifyVendor.empty()) {
                metaEditInfo.newVendorString = modifyVendor;
                metaEditInfo.modifyVendorString = true;
            }
            if (!modifyComment.empty()) {
                metaEditInfo.newComment = modifyComment;
                metaEditInfo.modifyComment = true;
                if (!modifyCommentIndex.empty()) {
                    metaEditInfo.modifyCommentIndex = stoi(modifyCommentIndex);
                } else {
                    throw runtime_error("Error acquiring comment index for modifying (main::main)");
                }
            }
            if (!appendComment.empty()) {
                metaEditInfo.newComment = appendComment;
                metaEditInfo.appendComment = true;
            }
            if (!removeCommentIndex.empty()) {
                metaEditInfo.removeComment = true;
                if (!removeCommentIndex.empty()) {
                    metaEditInfo.removeCommentIndex = stoi(removeCommentIndex);
                } else {
                    throw runtime_error("Error acquiring comment index for removing (main::main)");
                }
            }

            fileReader reader(inputFile);
            fileWriter writer(outputFile);
            try {
                FlacMetadata::editFile(reader, writer, metaEditInfo);
            } catch (exception &e) {
                cout << e.what() << endl;
            }
            reader.closeReader();
        } else {
            // Invalid mode
            return 1;
        }
    }
    return 0;
}

string generateRandomString(int length) {
    srand(static_cast<unsigned int>(std::time(nullptr)));
    const string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string result;
    for (int i = 0; i < length; ++i) {
        result += charset[rand() % charset.size()];
    }
    return result;
}