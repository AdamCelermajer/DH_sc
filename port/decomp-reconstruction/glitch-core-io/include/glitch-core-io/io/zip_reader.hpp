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

    std::string path;     // +0x00: the raw name from the local header (lower-cased with flagA)
    std::string name;     // +0x18: the file name (base name with flagB, else the raw name)
    std::string extra;    // +0x30
    std::int32_t f48 = 0;   // +0x48: offset of the file data (set from getPos after the local header)
    std::int32_t f4c = 0;   // +0x4c
    std::int32_t f50 = 0;   // +0x50
    std::int32_t f54 = 0;   // +0x54: compression method (0 stored, 8 deflate)
    std::int32_t f58 = 0;   // compressed size (port slot; the pseudocode reads it at +0x5e)
    std::int32_t f5c = 0;   // uncompressed size (port slot; the pseudocode reads it at +0x62)
    std::int32_t f60 = 0;   // +0x60
    std::int32_t f64 = 0;   // +0x64
    std::int16_t f68 = 0;   // +0x68
};

// Order of entries for heapsink (0x5775a4), heapsort (0x5777ac) and binary_search (0x5783c4): the
// name (+0x18) compared byte by byte as signed chars (LDRSB), then by length, as for SPakFileEntry.
bool operator<(const SZipFileEntry& a, const SZipFileEntry& b);

class CZipReader : public glitch::IReferenceCounted {
public:
    CZipReader() = default;  // used by the tests; the original always has a file or a name
    // 0x577fa4 (C1), 0x578050 (C2): reads the directory from an open file (referenced), then sorts
    // the entries by name with glitch::core::heapsort, as the original does.
    CZipReader(IReadFile* file, bool flagA, bool flagB);
    // 0x57826c (C1), 0x578318 (C2): opens the file by name, then as above.
    CZipReader(const char* fileName, bool flagA, bool flagB);
    ~CZipReader() override;  // 0x577488 (D1), 0x5774ec (D2): drops the file

    int getFileCount();                                     // 0x576cf0: entries / 0x6c
    // 0x576f18: PARTIAL. Stored entries (method 0) are a limit read file over the archive (the
    // five-argument form when bit 0 of unk_10 is set). Deflate entries (method 8) need zlib, which
    // is not vendored in this tree, so they give null here.
    IReadFile* openFile(int index);
    // 0x578624: findFile, then openFile(index); null when the name is not in the archive.
    IReadFile* openFile(const char* name);
    // 0x578520: the key's name is the argument, lower-cased when flagA is set and with its path
    // removed when flagB is set; the search is the shared binary search over the entries.
    int findFile(const char* name);
    const SZipFileEntry* getFileInfo(int index) const;      // 0x576d1c: no bounds check, as the original

    // 0x576d2c: true when the file starts with the local file header signature 0x04034b50.
    // The position is restored afterwards.
    static bool isValid(IReadFile* file);
    // 0x576ef4: opens the file by name and tests it. PARTIAL: a missing file gives false here
    // (the original would test a null file).
    static bool isValid(const char* fileName);

    // 0x577b50: keeps the text after the last '/' or '\\' (not at index 0); the string is NUL
    // terminated, as the pseudocode copies up to strlen.
    void deletePathFromFilename(std::string& fileName);

    // 0x577bc8: splits the raw name (path, +0x00) into the directory part (extra, up to and
    // including the last '/') and the base name (name). A name with no '/', or whose only '/' is
    // the first character, keeps the whole name and an empty directory. flagA lower-cases the raw
    // name first. With flagB the name is the base name; without it the raw name is kept.
    void extractFilename(SZipFileEntry& entry);

    // 0x577db4: reads the next local header (30 bytes, name, extra field) and records the entry
    // with its data offset. Returns false at the end of the local headers. The 12 bytes read when
    // the data-descriptor flag (bit 3) is set overwrite the crc and sizes, as the pseudocode does.
    bool scanLocalHeader();

    // +0x08: the archive file (referenced by the reader, released in the destructor)
    IReadFile* readFile = nullptr;
    // +0x0c: flagA, lower-cases names; +0x0d: flagB, keeps only the base name (see extractFilename)
    bool flagA = false;
    bool flagB = false;
    // +0x10: bit 0 selects the five-argument CLimitReadFile form in openFile. The constructors store
    // 0 and no code in this class sets it; its writer is not recovered.
    std::int32_t unk_10 = 0;
    // +0x14 .. +0x1c: entry vector
    std::vector<SZipFileEntry> entries;
};

}  // namespace glitch::io
