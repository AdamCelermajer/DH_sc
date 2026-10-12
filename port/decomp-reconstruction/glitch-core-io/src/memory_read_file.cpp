#include "glitch-core-io/io/memory_read_file.hpp"

namespace glitch::io {

// 0x56ed1c _ZN6glitch2io15CMemoryReadFileC1Ev
CMemoryReadFile::CMemoryReadFile() = default;

// 0x56ed8c _ZN6glitch2io15CMemoryReadFileC2Ev (same body as the constructor above)
// Kept as the same constructor: the two symbols are the C1 and C2 copies.

// 0x56edfc _ZN6glitch2io15CMemoryReadFile3setEPvlPKc
void CMemoryReadFile::set(void* data, long length, const char* fileName) {
    buffer = data;
    size = length;
    position = 0;
    name = fileName ? fileName : "";
}

// 0x56f150 _ZNK6glitch2io15CMemoryReadFile5cloneEv
CMemoryReadFile* CMemoryReadFile::clone() const {
    CMemoryReadFile* copy = new CMemoryReadFile();
    copy->buffer = buffer;
    copy->owned = owned;
    copy->size = size;
    copy->position = position;
    copy->name = name;
    return copy;
}

// 0x56f2ec _ZN6glitch2io15CMemoryReadFileC1EPvlPKcb
// 0x56f3b8 _ZN6glitch2io15CMemoryReadFileC2EPvlPKcb (same body)
CMemoryReadFile::CMemoryReadFile(void* data, long length, const char* fileName, bool takeOwnership)
    : buffer(data), size(length), name(fileName ? fileName : "") {
    if (takeOwnership) owned.reset(static_cast<char*>(data));
}

}  // namespace glitch::io
