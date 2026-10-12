// Image loader and writer format probes for BMP, TGA. Reconstructed from
// libDungeonHunter2.so. Only the format-detection entry points are rebuilt.
#pragma once

#include "glitch-video/deps/glitch_io.hpp"
#include "glitch-video/texture_loaders.hpp"

namespace glitch::video {

// 0x603480: new CImageLoaderBMP, default constructed.
class CImageLoaderBMP;
CImageLoaderBMP* createImageLoaderBMP();

// 0x606990 (C2) / 0x6069bc (C1): writer with the reference word at +0x04 set to 1.
class CImageWriterJPG {
public:
    CImageWriterJPG();  // 0x606990 (C2) / 0x6069bc (C1)
    virtual ~CImageWriterJPG() = default;

    // 0x606c78: the extension after the last dot is .jpg, .jpeg, .JPG or .JPEG.
    bool isAWriteableFileExtension(const char* filename) const;

private:
    uint32_t m_unknown04 = 1;  // +0x04
};

// 0x606d30 (C2) / 0x606d5c (C1): PNG writer, same layout as the JPG writer.
class CImageWriterPNG {
public:
    CImageWriterPNG();  // 0x606d30 (C2) / 0x606d5c (C1)
    virtual ~CImageWriterPNG() = default;

    // 0x607230: ".png" or ".PNG" appears anywhere in the name (substring match).
    bool isAWriteableFileExtension(const char* filename) const;

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

class CImageLoaderTGA : public IImageLoader {
public:
    CImageLoaderTGA() = default;
    virtual ~CImageLoaderTGA() = default;

    // 0x606778: the file is at least 0x1a bytes and the 26-byte footer, read from
    // the end, carries the "TRUEVISION-XFILE." signature at footer offset 8.
    bool isALoadableFileFormat(io::IReadFile* file) const;
    // Case-sensitive match on ".tga" or ".TGA".
    bool isALoadableFileExtension(const char* filename) const;
};

// PNG loader. Only the format and extension probes are rebuilt here.
class CImageLoaderPng : public IImageLoader {
public:
    CImageLoaderPng() = default;
    virtual ~CImageLoaderPng() = default;

    // 0x605048: reads 8 bytes and compares them with the PNG signature.
    bool isALoadableFileFormat(io::IReadFile* file) const;
    // 0x605674: the extension after the last dot is exactly ".PNG" or ".png".
    bool isALoadableFileExtension(const char* filename) const;
};

// 0x60500c: new CImageLoaderPng with the reference word set to 1.
CImageLoaderPng* createImageLoaderPNG();
// 0x60632c: new CImageLoaderTGA with the reference word set to 1.
CImageLoaderTGA* createImageLoaderTGA();

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
