#include "core-platform/filesystem_base.hpp"

#include <cstring>

#ifdef _WIN32
#include <direct.h>
#define DH2_CHDIR _chdir
#else
#include <unistd.h>
#define DH2_CHDIR chdir
#endif

namespace glitch::io {
char CFileSystem_WorkingDirectory[512] = {};
}  // namespace glitch::io

// 0x34e0fc FileSystemBase::decodeObfuscatedData(int, void*, int, void*)
// Partial: the pseudocode has three arguments; the fourth mangled parameter is
// not visible. Each of the first (4 - index) bytes gets the running key added,
// where the key starts at ~index and decrements by one per byte.
void FileSystemBase::decodeObfuscatedData(int index, void* data, int size) {
    if (index < 4) {
        int count = size;
        if (4 - index <= count) {
            count = 4 - index;
        }
        auto* bytes = static_cast<unsigned char*>(data);
        unsigned int key = ~static_cast<unsigned int>(index);
        for (int i = 0; i < count; ++i) {
            bytes[i] = static_cast<unsigned char>(bytes[i] + static_cast<unsigned char>(key));
            key = (key & 0xffu) - 1;
        }
    }
}

// 0x34e1d4 FileSystemBase::changeWorkingDirectoryTo(char const*)
bool FileSystemBase::changeWorkingDirectoryTo(const char* path) {
    bool ok = DH2_CHDIR(path) == 0;
    if (ok) {
        std::strcpy(glitch::io::CFileSystem_WorkingDirectory, path);
    }
    return ok;
}
