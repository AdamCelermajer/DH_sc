// glitch::io::SPakFileEntry and glitch::io::CPakReader (directory accessors and the
// destructor). Reconstructed from libDungeonHunter2.so.
#include "glitch-core-io/io/pak_reader.hpp"

#include "glitch-core-io/io/limit_read_file.hpp"
#include "glitch-core-io/core/sort.hpp"

#include <cstddef>

namespace glitch::io {

// 0x56f494 _ZN6glitch2io13SPakFileEntryC1Ev
// Partial: the three strings start empty, as in the original. The original does not set
// the two integers (+0x48, +0x4c); they are zeroed here by their default initialisers.
SPakFileEntry::SPakFileEntry() {}

// 0x56f500 _ZN6glitch2io13SPakFileEntryC1ERKS1_
SPakFileEntry::SPakFileEntry(const SPakFileEntry& other)
    : path(other.path), name(other.name), extra(other.extra), offset(other.offset), size(other.size) {}

// 0x56f5e0 _ZN6glitch2io13SPakFileEntryaSERKS1_
// Each string is assigned only when it is not the same object, as the original compares
// the member addresses before each assignment. The integers are always copied.
SPakFileEntry& SPakFileEntry::operator=(const SPakFileEntry& other) {
    if (this != &other) path = other.path;
    if (&name != &other.name) name = other.name;
    if (&extra != &other.extra) extra = other.extra;
    offset = other.offset;
    size = other.size;
    return *this;
}

// 0x56f43c _ZN6glitch2io10CPakReader12getFileCountEv
// The original computes (end - begin) / 0x50 with a multiply by the inverse of 5; that
// is the element count of the vector.
int CPakReader::getFileCount() {
    return static_cast<int>(entries.size());
}

// 0x56f464 _ZNK6glitch2io10CPakReader11getFileInfoEi
// No bounds check, as in the original: index must be a valid entry.
const SPakFileEntry* CPakReader::getFileInfo(int index) const {
    return entries.data() + index;
}

// 0x56f6f4 _ZN6glitch2io10CPakReaderD1Ev
// 0x56f73c _ZN6glitch2io10CPakReaderD2Ev
// Partial: the body releases the read file. The original also resets the vtable and
// destroys the entry vector with the glitch allocator; here the vector is destroyed by
// its own destructor and the vtable is the compiler's.
CPakReader::~CPakReader() {
    if (readFile != nullptr) readFile->drop();
}

// 0x56f568 _ZN6glitch2io10CPakReader22deletePathFromFilenameERSbIc...
// Partial: the original scans the glitch string from its end pointer and tests the byte
// there first. std::string keeps a terminator at size(), which is never a separator, so
// that first test is dropped and the scan starts at the last character. The rest matches:
// the text after the last separator is kept, and a separator at index 0 leaves the string
// unchanged (the original calls the assign helper with the text after the separator).
void CPakReader::deletePathFromFilename(std::string& name) {
    const std::size_t size = name.size();
    if (size == 0) return;
    std::size_t i = size;
    do {
        --i;
        if (name[i] == '/' || name[i] == '\\') {
            if (i != 0) name = name.substr(i + 1);
            return;
        }
    } while (i != 0);
}


// 0x57002c _ZN6glitch2io10CPakReaderC1EPNS0_9IReadFileEbb (C2 at 0x5700c4 is the same body)
// PARTIAL: the scan runs once (scanLocalHeader) and the sort of the entries is not applied.
CPakReader::CPakReader(IReadFile* file, bool a, bool b) : readFile(file), flagA(a), flagB(b) {
    if (readFile) {
        ++readFile->referenceCounter;
        scanLocalHeader();
    }
}

// 0x56f784 _ZN6glitch2io10CPakReader8openFileEi
IReadFile* CPakReader::openFile(int index) {
    const SPakFileEntry& entry = entries[static_cast<std::size_t>(index)];
    readFile->seek(entry.offset, false);
    return createLimitReadFile(entry.name.c_str(), readFile, entry.size);
}

// 0x56fcec _ZN6glitch2io10CPakReader15extractFilenameEPNS0_13SPakFileEntryE
// PARTIAL: lower-cases the path (ASCII 'A'..'Z') when flagA is set. The original's separator
// handling afterwards is not recovered.
void CPakReader::extractFilename(SPakFileEntry* entry) {
    if (!flagA) return;
    for (char& c : entry->path) {
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c + ' ');
    }
}

// 0x5703b8 _ZN6glitch2io10CPakReader8openFileEPKc
IReadFile* CPakReader::openFile(const char* name) {
    const int index = findFile(name);
    if (index != -1) return openFile(index);
    return nullptr;
}

// 0x5702b4 _ZN6glitch2io10CPakReader8findFileEPKc
// PARTIAL: the key is built from the name (the path is left empty) and lower-cased when
// flagA is set, as the decompile shows. The search is the shared binary search over the
// entries, which relies on the same name ordering as the sort.
int CPakReader::findFile(const char* name) {
    SPakFileEntry key;
    key.name = name;
    if (flagA) {
        for (char& c : key.name) {
            if (c >= 'A' && c <= 'Z') c = static_cast<char>(c + ' ');
        }
    }
    return glitch::core::binary_search(entries, key);
}

// 0x56fa04 / 0x56fc0c: see the declaration in pak_reader.hpp for the comparison used.
bool operator<(const SPakFileEntry& a, const SPakFileEntry& b) { return a.name < b.name; }

// 0x56fe44 _ZN6glitch2io10CPakReader15scanLocalHeaderEv: PARTIAL stub, records no entries.
void CPakReader::scanLocalHeader() {}

}  // namespace glitch::io
