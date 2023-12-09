#ifndef TEMP_FILE_H
#define TEMP_FILE_H

#include <string>
#include <system_error>

class TempFile {
    std::error_code ec;
    std::string filename;
public :
    const std::string &string() const;

    TempFile(const std::string &mask);

    ~TempFile();
};

#endif // TEMP_FILE_H