#include "glitch-video/image_loaders.hpp"

#include <cstring>

namespace {
// PNG file signature (the bytes checked by the original through png_sig_cmp).
const unsigned char kPngSignature[8] = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a};
}  // namespace

namespace glitch::video {

namespace {

bool hasExtension(const char* name, const char* lower, const char* upper) {
    return std::strstr(name, lower) != nullptr || std::strstr(name, upper) != nullptr;
}

}  // namespace

// 0x603380 _ZN6glitch5video15CImageLoaderBMPC2Ev
// 0x6033ac _ZN6glitch5video15CImageLoaderBMPC1Ev
CImageLoaderBMP::CImageLoaderBMP() = default;

// 0x6033d8 _ZNK6glitch5video15CImageLoaderBMP21isALoadableFileFormatEPNS_2io9IReadFileE
// The original compares the 16-bit value 0x4d42 read in little-endian order,
// which is bytes 'B' then 'M'. The byte compare below is endian independent.
bool CImageLoaderBMP::isALoadableFileFormat(io::IReadFile* file) const {
    unsigned char magic[2] = {0, 0};
    file->read(magic, 2);
    return magic[0] == 'B' && magic[1] == 'M';
}

// 0x6034a0 _ZNK6glitch5video15CImageLoaderBMP24isALoadableFileExtensionEPKc
bool CImageLoaderBMP::isALoadableFileExtension(const char* filename) const {
    return hasExtension(filename, ".bmp", ".BMP");
}

// 0x605048 _ZNK6glitch5video15CImageLoaderPng21isALoadableFileFormatEPNS_2io9IReadFileE
// Reads the 8-byte signature and compares it with the PNG signature. A short read fails.
bool CImageLoaderPng::isALoadableFileFormat(io::IReadFile* file) const {
    if (file == nullptr) return false;
    unsigned char header[8] = {};
    if (file->read(header, 8) != 8) return false;
    return std::memcmp(header, kPngSignature, 8) == 0;
}

// 0x605674 _ZNK6glitch5video15CImageLoaderPng24isALoadableFileExtensionEPKc
// Only the text after the last dot is compared, so "a.png.bak" is rejected.
bool CImageLoaderPng::isALoadableFileExtension(const char* filename) const {
    const char* dot = std::strrchr(filename, '.');
    if (dot == nullptr) return false;
    return std::strcmp(dot, ".PNG") == 0 || std::strcmp(dot, ".png") == 0;
}

// 0x6062f4 _ZN6glitch5video15CImageLoaderTGAD1Ev
// 0x606318 _ZN6glitch5video15CImageLoaderTGAD0Ev is the compiler generated deleting variant.

// 0x606778 _ZNK6glitch5video15CImageLoaderTGA21isALoadableFileFormatEPNS_2io9IReadFileE
// The original reads the file size, seeks to size - 0x1a and reads the 26-byte footer.
// The signature is compared at footer offset 8 with "TRUEVISION-XFILE.".
bool CImageLoaderTGA::isALoadableFileFormat(io::IReadFile* file) const {
    if (file == nullptr || file->getSize() < 0x1a) {
        return false;
    }
    file->seek(static_cast<int32_t>(file->getSize()) - 0x1a, false);
    unsigned char footer[0x1a] = {};
    file->read(footer, 0x1a);
    return std::strcmp(reinterpret_cast<const char*>(footer + 8), "TRUEVISION-XFILE.") == 0;
}

// 0x60686c _ZNK6glitch5video15CImageLoaderTGA24isALoadableFileExtensionEPKc
bool CImageLoaderTGA::isALoadableFileExtension(const char* filename) const {
    return hasExtension(filename, ".tga", ".TGA");
}

// 0x6072e0 _ZN6glitch5video15CImageWriterTGAC2Ev
// 0x60730c _ZN6glitch5video15CImageWriterTGAC1Ev
CImageWriterTGA::CImageWriterTGA() = default;

// 0x6075e0 _ZNK6glitch5video15CImageWriterTGA25isAWriteableFileExtensionEPKc
bool CImageWriterTGA::isAWriteableFileExtension(const char* filename) const {
    return hasExtension(filename, ".tga", ".TGA");
}

// 0x606c78 _ZNK6glitch5video15CImageWriterJPG25isAWriteableFileExtensionEPKc
// Matches the text after the last dot exactly against .jpg, .jpeg, .JPG and .JPEG.
bool CImageWriterJPG::isAWriteableFileExtension(const char* filename) const {
    const char* dot = std::strrchr(filename, '.');
    if (dot == nullptr) return false;
    return std::strcmp(dot, ".jpg") == 0 || std::strcmp(dot, ".jpeg") == 0 ||
           std::strcmp(dot, ".JPG") == 0 || std::strcmp(dot, ".JPEG") == 0;
}

// 0x607230 _ZNK6glitch5video15CImageWriterPNG25isAWriteableFileExtensionEPKc
// Substring match: ".png" or ".PNG" anywhere in the name.
bool CImageWriterPNG::isAWriteableFileExtension(const char* filename) const {
    return std::strstr(filename, ".png") != nullptr || std::strstr(filename, ".PNG") != nullptr;
}

}  // namespace glitch::video
