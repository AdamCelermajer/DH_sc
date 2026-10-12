#include "glitch-video/image_loaders.hpp"

#include <cstring>

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

// 0x6062f4 _ZN6glitch5video15CImageLoaderTGAD1Ev
// 0x606318 _ZN6glitch5video15CImageLoaderTGAD0Ev is the compiler generated deleting variant.

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

}  // namespace glitch::video
