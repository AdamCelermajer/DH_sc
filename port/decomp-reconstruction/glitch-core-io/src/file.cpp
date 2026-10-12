#include "glitch-core-io/io/file.hpp"

#include <stdexcept>

namespace glitch::io {

// 0x56c9e8 _ZN6glitch2io5CFileC1EP7__sFILEPKcb
// 0x56c960 _ZN6glitch2io5CFileC2EP7__sFILEPKcb (same body)
// PARTIAL: the separator passed to find_last_of is not recovered (the call is a GCC clone
// with the literal baked in). '/' and '\\' are used, as in CPakReader::deletePathFromFilename.
// When no separator is found the base name is the whole name (the original leaves +0x20
// unset). When the separator is the last character the original throws out_of_range.
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
