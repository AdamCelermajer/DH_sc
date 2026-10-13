#include "glitch-core-io/io/file.hpp"

#include <stdexcept>

namespace glitch::io {

// 0x56c9e8 _ZN6glitch2io5CFileC1EP7__sFILEPKcb
// 0x56c960 _ZN6glitch2io5CFileC2EP7__sFILEPKcb (same body)
// The find_last_of call is a GCC clone (0x56c844) with the separator set folded in. Its bitmap
// holds '/' (0x2f) and '\\' (0x5c), and the search runs from the end of the name. When the last
// character is a separator the original throws out_of_range.
// PARTIAL: when the name has no separator the original stores nothing at +0x20, so the base name
// offset keeps whatever the object was allocated with. The port default-initialises the offset to 0.
CFile::CFile(std::FILE* handle, const char* fileName, bool obfuscatedFlag)
    : file(handle), name(fileName ? fileName : "") {
    obfuscated = obfuscatedFlag;
    std::size_t sep = name.find_last_of("/\\");
    if (sep != std::string::npos) {
        if (name.size() <= sep + 1) throw std::out_of_range("basic_string");
        baseNameOffset = sep + 1;
    }
}

// 0x56d108 _ZN6glitch2io5CFileD1Ev (D2 at 0x56d144 is the same body)
CFile::~CFile() {
    if (file) std::fclose(file);
}

}  // namespace glitch::io
