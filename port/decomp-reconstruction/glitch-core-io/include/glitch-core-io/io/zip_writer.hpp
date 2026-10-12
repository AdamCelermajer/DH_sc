// glitch::io::CZipWriter: writes a zip archive to a write file. Layout from the original
// offsets: +0x08 the write file (not reference counted by this class), +0x0c flag byte,
// +0x24 the names of the entries added so far (vector of strings).
#pragma once

#include <string>
#include <vector>

#include "glitch-core-io/deps/glitch_core.hpp"
#include "glitch-core-io/io/interfaces.hpp"

namespace glitch::io {

class CZipWriter : public glitch::IReferenceCounted {
public:
    explicit CZipWriter(IWriteFile* file);  // 0x6b4f34 (C2), 0x6b4f90 (C1)
    // Both destructors call close() in the original, which is not reconstructed (PARTIAL).
    ~CZipWriter() override;                 // 0x6b5238 (D1), 0x6b5288 (D2)

    CZipWriter(const CZipWriter&) = delete;
    CZipWriter& operator=(const CZipWriter&) = delete;

    IWriteFile* writeFile = nullptr;   // +0x08
    bool flag = false;                 // +0x0c
    std::vector<std::string> names;    // +0x24
};

}  // namespace glitch::io
