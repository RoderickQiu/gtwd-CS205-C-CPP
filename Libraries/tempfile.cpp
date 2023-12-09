#include "tempfile.h"
#include <errno.h>
#include <filesystem>
#include <vector>
#include <unistd.h>

namespace fs = std::filesystem;

const std::string &TempFile::string() const { return filename; }

TempFile::TempFile(const std::string &mask_) {
    auto dir = fs::temp_directory_path(ec);
    if (ec) return;
    std::string mask = (dir / mask_).string();
    std::vector<char> vfilename(mask.c_str(), mask.c_str() + mask.size() + 1);
    int fd = mkstemp(&vfilename[0]);
    if (fd == -1) {
        ec = std::error_code(errno, std::generic_category());
        return;
    }
    close(fd);
    filename = (char *) &vfilename[0];
}

TempFile::~TempFile() {
    bool keepCache = false;
    const char *keepCacheValue = getenv("KEEP_CACHE");
    if (keepCacheValue) keepCache = atoi(keepCacheValue);
    if (!keepCache) unlink(filename.c_str());
}