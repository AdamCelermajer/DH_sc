// Image loader and writer format probes for BMP, TGA. Reconstructed from
// libDungeonHunter2.so. Only the format-detection entry points are rebuilt.
#pragma once

#include "glitch-video/deps/glitch_io.hpp"

namespace glitch::video {

// 0x603480: new CImageLoaderBMP, default constructed.
class CImageLoaderBMP;
CImageLoaderBMP* createImageLoaderBMP();

// 0x606990 (C2) / 0x6069bc (C1): writer with the reference word at +0x04 set to 1.
class CImageWriterJPG {
public:
    CImageWriterJPG();  // 0x606990 (C2) / 0x6069bc (C1)
    virtual ~CImageWriterJPG() = default;

private:
    uint32_t m_unknown04 = 1;  // +0x04
};

// 0x606d30 (C2) / 0x606d5c (C1): PNG writer, same layout as the JPG writer.
class CImageWriterPNG {
public:
    CImageWriterPNG();  // 0x606d30 (C2) / 0x606d5c (C1)
    virtual ~CImageWriterPNG() = default;

private:
    uint32_t m_unknown04 = 1;  // +0x04
};

// 0x606d10, 0x6072c0, 0x60762c: factories for the JPG, PNG and TGA writers.
CImageWriterJPG* createImageWriterJPG();
CImageWriterPNG* createImageWriterPNG();
class CImageWriterTGA;
CImageWriterTGA* createImageWriterTGA();

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
