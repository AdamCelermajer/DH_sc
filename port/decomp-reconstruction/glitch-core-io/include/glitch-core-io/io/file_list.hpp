// glitch::io::CFileList: a directory listing. Each entry keeps its name, its full name,
// an id and whether it is a directory. Sorting and getFullFileName are not reconstructed.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "glitch-core-io/deps/glitch_core.hpp"

namespace glitch::io {

class CFileList : public glitch::IReferenceCounted {
public:
    struct FileEntry {
        FileEntry() = default;
        FileEntry(const FileEntry& other);                 // 0x6b4188
        FileEntry& operator=(const FileEntry& other);      // 0x6b404c
        ~FileEntry() = default;                            // 0x6b3f58

        // +0x00: entry name (glitch string; the begin pointer is read by getFileName)
        std::string Name;
        // +0x18: full name (glitch string, cached by getFullFileName in the original)
        std::string FullName;
        // +0x30
        std::uint32_t ID = 0;
        // +0x34
        bool isDirectory = false;
    };

    CFileList() = default;                             // 0x6b4418, 0x6b4480
    ~CFileList() override = default;                   // 0x6b3fe0, 0x6b4030

    int getFileCount() const;                          // 0x6b3e78
    const char* getFileName(std::uint32_t index) const;  // 0x6b3ea4; nullptr when out of range
    bool isDirectory(std::uint32_t index) const;       // 0x6b3ee4; false when out of range

    // +0x08: directory path (glitch string)
    std::string Path;
    // +0x20 .. +0x28: entry vector (begin, end, capacity)
    std::vector<FileEntry> Files;
};

}  // namespace glitch::io
