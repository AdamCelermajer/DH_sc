// glitch::io::SPakFileEntry and glitch::io::CPakReader (directory accessors and the
// destructor). Reconstructed from libDungeonHunter2.so.
#include "glitch-core-io/io/pak_reader.hpp"

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

}  // namespace glitch::io
