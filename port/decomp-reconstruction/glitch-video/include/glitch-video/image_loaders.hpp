// Image loader and writer format probes for BMP, TGA. Reconstructed from
// libDungeonHunter2.so. Only the format-detection entry points are rebuilt.
#pragma once

#include "glitch-video/deps/glitch_io.hpp"

namespace glitch::video {

class CImageLoaderBMP {
public:
    CImageLoaderBMP();
    virtual ~CImageLoaderBMP() = default;

    // Reads the first two bytes and checks for the 'BM' signature.
    bool isALoadableFileFormat(io::IReadFile* file) const;
    // Case-sensitive match on ".bmp" or ".BMP".
    bool isALoadableFileExtension(const char* filename) const;

private:
    uint32_t m_unknown04 = 1;  // +0x04, set to 1 by both constructors
};

class CImageLoaderTGA {
public:
    CImageLoaderTGA() = default;
    virtual ~CImageLoaderTGA() = default;

    // Case-sensitive match on ".tga" or ".TGA".
    bool isALoadableFileExtension(const char* filename) const;
};

class CImageWriterTGA {
public:
    CImageWriterTGA();
    virtual ~CImageWriterTGA() = default;

    // Case-sensitive match on ".tga" or ".TGA".
    bool isAWriteableFileExtension(const char* filename) const;

private:
    uint32_t m_unknown04 = 1;  // +0x04, set to 1 by both constructors
};

}  // namespace glitch::video
