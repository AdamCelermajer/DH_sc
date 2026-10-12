#include "glitch-video/texture_loaders.hpp"

#include <cstring>

namespace glitch::video {

// 0x602c84 _ZN6glitch5video12IImageLoader23hasTextureLoadInterfaceEv
bool IImageLoader::hasTextureLoadInterface() { return false; }

// 0x602c8c _ZNK6glitch5video12IImageLoader17loadTextureHeaderEPNS_2io9IReadFileERNS0_12STextureDescE
bool IImageLoader::loadTextureHeader(io::IReadFile*, STextureDesc&) const { return false; }

// 0x602c9c _ZN6glitch5video15CImageLoaderATC23hasTextureLoadInterfaceEv
bool CImageLoaderATC::hasTextureLoadInterface() { return true; }

// 0x602ca4 _ZNK6glitch5video15CImageLoaderATC9CDataInfo15getFileDataSizeEv
int32_t CImageLoaderATC::CDataInfo::getFileDataSize() const { return m_fileDataSize; }

// 0x602cc4 _ZNK6glitch5video15CImageLoaderATC21isALoadableFileFormatEPNS_2io9IReadFileE
// Files shorter than 8 bytes are rejected. The 16-bit value at offset 4 is read
// as b0 | b1 << 8 and accepted when (value + 0x736e) & 0xffff < 2.
bool CImageLoaderATC::isALoadableFileFormat(io::IReadFile* file) const {
    if (file == nullptr || file->getSize() < 8) return false;
    uint8_t b0 = 0, b1 = 0;
    file->seek(4, false);
    file->read(&b0, 1);
    file->read(&b1, 1);
    uint16_t value = static_cast<uint16_t>(b0 | (b1 << 8));
    return static_cast<uint16_t>(value + 0x736e) < 2;
}

// 0x603254 _ZNK6glitch5video15CImageLoaderATC24isALoadableFileExtensionEPKc
bool CImageLoaderATC::isALoadableFileExtension(const char* filename) const {
    return std::strstr(filename, ".atc") != nullptr || std::strstr(filename, ".ATC") != nullptr;
}

// 0x602cac _ZNK6glitch5video15CImageLoaderATC9CDataInfo12getFilePitchEh
uint32_t CImageLoaderATC::CDataInfo::getFilePitch(uint8_t) const { return 0; }

// 0x602cb4 _ZNK6glitch5video15CImageLoaderATC9CDataInfo14isLittleEndianEv
bool CImageLoaderATC::CDataInfo::isLittleEndian() const { return true; }

// 0x602cbc _ZNK6glitch5video15CImageLoaderATC9CDataInfo9needsFlipEv
bool CImageLoaderATC::CDataInfo::needsFlip() const { return false; }

// 0x6041c4 _ZN6glitch5video15CImageLoaderDDS23hasTextureLoadInterfaceEv
bool CImageLoaderDDS::hasTextureLoadInterface() { return true; }

// 0x6042c0 _ZNK6glitch5video15CImageLoaderDDS21isALoadableFileFormatEPNS_2io9IReadFileE
bool CImageLoaderDDS::isALoadableFileFormat(io::IReadFile* file) const {
    if (file == nullptr) return false;
    uint32_t magic = 0;
    file->read(&magic, 4);
    return magic == 0x20534444u;  // "DDS " read little-endian
}

// 0x60490c _ZNK6glitch5video15CImageLoaderDDS24isALoadableFileExtensionEPKc
bool CImageLoaderDDS::isALoadableFileExtension(const char* filename) const {
    return std::strstr(filename, ".dds") != nullptr;
}

// 0x604234 _ZNK6glitch5video15CImageLoaderDDS9CDataInfo12getFilePitchEh
uint32_t CImageLoaderDDS::CDataInfo::getFilePitch(uint8_t mip) const {
    if (mip == 0 && m_header != nullptr && (m_header[1] & 8) != 0) {
        return m_header[4];
    }
    return 0;
}

// 0x60422c _ZNK6glitch5video15CImageLoaderDDS9CDataInfo15getFileDataSizeEv
int32_t CImageLoaderDDS::CDataInfo::getFileDataSize() const { return m_fileDataSize; }

// 0x604258 _ZNK6glitch5video15CImageLoaderDDS9CDataInfo14isLittleEndianEv
bool CImageLoaderDDS::CDataInfo::isLittleEndian() const { return true; }

// 0x604260 _ZNK6glitch5video15CImageLoaderDDS9CDataInfo9needsFlipEv
bool CImageLoaderDDS::CDataInfo::needsFlip() const { return false; }

// 0x605718 _ZN6glitch5video15CImageLoaderPVR23hasTextureLoadInterfaceEv
bool CImageLoaderPVR::hasTextureLoadInterface() { return true; }

// 0x6057e0 _ZNK6glitch5video15CImageLoaderPVR21isALoadableFileFormatEPNS_2io9IReadFileE
// Reads a 52-byte header, then seeks back to the saved position. Accepted when
// the header is complete and either the size word is 0x34 and "PVR!" sits at
// offset 44, or the first 8 bytes are "BTEXpvr".
bool CImageLoaderPVR::isALoadableFileFormat(io::IReadFile* file) const {
    if (file == nullptr) return false;
    int32_t saved = file->getPos();
    uint8_t header[0x34] = {};
    int32_t got = file->read(header, 0x34);
    file->seek(saved, false);
    uint32_t size = 0;
    std::memcpy(&size, header, 4);
    if (got == 0x34 && size == 0x34 && std::memcmp(header + 44, "PVR!", 4) == 0) {
        return true;
    }
    return std::memcmp(header, "BTEXpvr", 8) == 0;
}

// 0x6061b4 _ZNK6glitch5video15CImageLoaderPVR24isALoadableFileExtensionEPKc
bool CImageLoaderPVR::isALoadableFileExtension(const char* filename) const {
    return std::strstr(filename, ".pvr") != nullptr || std::strstr(filename, ".btex") != nullptr;
}

// 0x605720 _ZNK6glitch5video15CImageLoaderPVR9CDataInfo15getFileDataSizeEv
int32_t CImageLoaderPVR::CDataInfo::getFileDataSize() const { return m_fileDataSize; }

// 0x605728 _ZNK6glitch5video15CImageLoaderPVR9CDataInfo12getFilePitchEh
uint32_t CImageLoaderPVR::CDataInfo::getFilePitch(uint8_t) const { return 0; }

// 0x605730 _ZNK6glitch5video15CImageLoaderPVR9CDataInfo14isLittleEndianEv
bool CImageLoaderPVR::CDataInfo::isLittleEndian() const { return true; }

// 0x605738 _ZNK6glitch5video15CImageLoaderPVR9CDataInfo9needsFlipEv
bool CImageLoaderPVR::CDataInfo::needsFlip() const { return false; }

// 0x604b0c _ZNK6glitch5video15CImageLoaderJPG21isALoadableFileFormatEPNS_2io9IReadFileE
bool CImageLoaderJPG::isALoadableFileFormat(io::IReadFile* file) const {
    if (file == nullptr || file->getSize() < 6) return false;
    file->seek(6, false);
    uint32_t magic = 0;
    file->read(&magic, 4);
    return magic == 0x4a464946u || magic == 0x4649464au;
}

// 0x604f6c _ZNK6glitch5video15CImageLoaderJPG24isALoadableFileExtensionEPKc
bool CImageLoaderJPG::isALoadableFileExtension(const char* filename) const {
    return std::strstr(filename, ".jpg") != nullptr || std::strstr(filename, ".JPG") != nullptr;
}

// 0x604adc _ZN6glitch5video15CImageLoaderJPG11init_sourceEP22jpeg_decompress_struct
void CImageLoaderJPG::init_source(jpeg_decompress_struct*) {}

// 0x604ae0 _ZN6glitch5video15CImageLoaderJPG17fill_input_bufferEP22jpeg_decompress_struct
bool CImageLoaderJPG::fill_input_buffer(jpeg_decompress_struct*) { return true; }

// 0x604b08 _ZN6glitch5video15CImageLoaderJPG11term_sourceEP22jpeg_decompress_struct
void CImageLoaderJPG::term_source(jpeg_decompress_struct*) {}

}  // namespace glitch::video
