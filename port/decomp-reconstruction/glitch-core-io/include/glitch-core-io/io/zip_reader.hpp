// glitch::io::SZipFileEntry and glitch::io::CZipReader (the directory part). An entry is
// 0x6c bytes in the original. Its three strings sit at +0x00, +0x18 and +0x30, as in the
// pak entry; the integers from +0x48 are not named by the code read so far.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "glitch-core-io/deps/glitch_core.hpp"
#include "glitch-core-io/io/read_file.hpp"

namespace glitch::io {

struct SZipFileEntry {
    SZipFileEntry() = default;                              // 0x577308 (C1)
    SZipFileEntry(const SZipFileEntry& other);              // 0x576e78 (C1, copy)
    SZipFileEntry& operator=(const SZipFileEntry& other);   // 0x577284
    // 0x5773e4 (D1): compiler generated; the strings release their buffers.
    ~SZipFileEntry() = default;

    std::string path;     // +0x00
    std::string name;     // +0x18
    std::string extra;    // +0x30
    std::int32_t f48 = 0;   // +0x48
    std::int32_t f4c = 0;   // +0x4c
    std::int32_t f50 = 0;   // +0x50
    std::int32_t f54 = 0;   // +0x54
    std::int32_t f58 = 0;   // +0x58
    std::int32_t f5c = 0;   // +0x5c
    std::int32_t f60 = 0;   // +0x60
    std::int32_t f64 = 0;   // +0x64
    std::int16_t f68 = 0;   // +0x68
};

class CZipReader : public glitch::IReferenceCounted {
public:
    CZipReader() = default;

    int getFileCount();                                     // 0x576cf0: entries / 0x6c
    const SZipFileEntry* getFileInfo(int index) const;      // 0x576d1c: no bounds check, as the original

    // 0x576d2c: true when the file starts with the local file header signature 0x04034b50.
    // The position is restored afterwards.
    static bool isValid(IReadFile* file);
    // 0x576ef4: opens the file by name and tests it. PARTIAL: a missing file gives false here
    // (the original would test a null file).
    static bool isValid(const char* fileName);

    // 0x577b50: keeps the text after the last '/' or '\\' (not at index 0). PARTIAL: the copy
    // is inferred from the tail call to strlen in the pseudocode.
    void deletePathFromFilename(std::string& fileName);

    // +0x14 .. +0x18: entry vector
    std::vector<SZipFileEntry> entries;
};

}  // namespace glitch::io
