// glitch::io::SPakFileEntry and glitch::io::CPakReader: the directory of a pak archive.
// Each entry holds three glitch strings and two integers. The strings sit 0x18 bytes
// apart in the original (+0x00, +0x18, +0x30); std::string stands in for them here, so
// the offsets below are the original ones, kept for reference only.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "glitch-core-io/deps/glitch_core.hpp"
#include "glitch-core-io/io/read_file.hpp"

namespace glitch::io {

struct SPakFileEntry {
    SPakFileEntry();                                        // 0x56f494 (C1)
    SPakFileEntry(const SPakFileEntry& other);              // 0x56f500 (C1, copy)
    SPakFileEntry& operator=(const SPakFileEntry& other);   // 0x56f5e0 (copy assignment)
    // 0x56f650 (D1): compiler-generated; the three strings release their buffers.
    ~SPakFileEntry() = default;

    std::string path;   // +0x00: name as stored in the pak directory
    std::string name;   // +0x18: file name the limit reader is opened with (openFile)
    std::string extra;  // +0x30: third string; its use is not recovered
    std::int32_t offset = 0;  // +0x48: position of the file data in the pak (seek target)
    std::int32_t size = 0;    // +0x4c: size of the file data (limit of the reader)
};

// 0x56fa04 / 0x56fc0c use operator< on entries. PARTIAL: the decompiled comparator reads a
// string field at +0x28..+0x2c; the port compares the name (+0x18), the field that findFile
// keys on. Order of equal-prefix names follows std::string, not the original's signed char
// compare.
bool operator<(const SPakFileEntry& a, const SPakFileEntry& b);

class CPakReader : public glitch::IReferenceCounted {
public:
    // The original constructor (0x57002c, 0x5700c4) reads the directory through
    // scanLocalHeader and sorts it; neither is reconstructed. This default constructor
    // gives an empty directory (used by the tests).
    CPakReader() = default;
    // 0x57002c (C1), 0x5700c4 (C2): references the file and scans its directory once.
    // PARTIAL: the scan (scanLocalHeader) and the entry sort are not reconstructed.
    CPakReader(IReadFile* file, bool flagA, bool flagB);
    ~CPakReader() override;  // 0x56f6f4 (D1), 0x56f73c (D2): drops the read file

    // 0x56f784: seeks the archive file to the entry's offset and returns a limit reader over the
    // entry's size, named after the entry.
    IReadFile* openFile(int index);
    // 0x5703b8: the reader for the named entry; null when there is no such entry.
    IReadFile* openFile(const char* name);
    // 0x5702b4: PARTIAL. Index of the entry whose name matches (the key's name is lower-cased
    // when flagA is set), or -1. The lookup is a binary search over the sorted entries.
    int findFile(const char* name);
    // 0x56fcec: PARTIAL. Lower-cases the path when flagA is set; the separator trimming that
    // follows in the original is not recovered (the pseudocode indexes a fixed offset).
    void extractFilename(SPakFileEntry* entry);
    // Directory scan, not reconstructed: records nothing.
    void scanLocalHeader();

    CPakReader(const CPakReader&) = delete;
    CPakReader& operator=(const CPakReader&) = delete;

    int getFileCount();                           // 0x56f43c: (end - begin) / 0x50
    const SPakFileEntry* getFileInfo(int index) const;  // 0x56f464: begin + index * 0x50

    // 0x56f568: keeps the text after the last '/' or '\\' in name. A separator at index 0
    // or no separator at all leaves name unchanged. The original works on a glitch string
    // whose end pointer holds the terminator; for std::string the same scan is done from
    // the last character. Partial: see src/pak_reader.cpp.
    void deletePathFromFilename(std::string& name);

    // +0x08: the archive's read file (reference counted; released in the destructor).
    glitch::io::IReadFile* readFile = nullptr;
    // +0x18 .. +0x20: entry vector (begin, end, capacity).
    std::vector<SPakFileEntry> entries;
    // +0x24, +0x25: flags passed to the constructor (their uses are not recovered).
    bool flagA = false;  // +0x24
    bool flagB = false;  // +0x25
};

}  // namespace glitch::io
