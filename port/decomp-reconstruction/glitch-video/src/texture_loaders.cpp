#include "glitch-video/texture_loaders.hpp"

namespace glitch::video {

// 0x602c84 _ZN6glitch5video12IImageLoader23hasTextureLoadInterfaceEv
bool IImageLoader::hasTextureLoadInterface() { return false; }

// 0x602c8c _ZNK6glitch5video12IImageLoader17loadTextureHeaderEPNS_2io9IReadFileERNS0_12STextureDescE
bool IImageLoader::loadTextureHeader(io::IReadFile*, STextureDesc&) const { return false; }

// 0x602c9c _ZN6glitch5video15CImageLoaderATC23hasTextureLoadInterfaceEv
bool CImageLoaderATC::hasTextureLoadInterface() { return true; }

// 0x602ca4 _ZNK6glitch5video15CImageLoaderATC9CDataInfo15getFileDataSizeEv
int32_t CImageLoaderATC::CDataInfo::getFileDataSize() const { return m_fileDataSize; }

// 0x602cac _ZNK6glitch5video15CImageLoaderATC9CDataInfo12getFilePitchEh
uint32_t CImageLoaderATC::CDataInfo::getFilePitch(uint8_t) const { return 0; }

// 0x602cb4 _ZNK6glitch5video15CImageLoaderATC9CDataInfo14isLittleEndianEv
bool CImageLoaderATC::CDataInfo::isLittleEndian() const { return true; }

// 0x602cbc _ZNK6glitch5video15CImageLoaderATC9CDataInfo9needsFlipEv
bool CImageLoaderATC::CDataInfo::needsFlip() const { return false; }

// 0x6041c4 _ZN6glitch5video15CImageLoaderDDS23hasTextureLoadInterfaceEv
bool CImageLoaderDDS::hasTextureLoadInterface() { return true; }

// 0x60422c _ZNK6glitch5video15CImageLoaderDDS9CDataInfo15getFileDataSizeEv
int32_t CImageLoaderDDS::CDataInfo::getFileDataSize() const { return m_fileDataSize; }

// 0x604258 _ZNK6glitch5video15CImageLoaderDDS9CDataInfo14isLittleEndianEv
bool CImageLoaderDDS::CDataInfo::isLittleEndian() const { return true; }

// 0x604260 _ZNK6glitch5video15CImageLoaderDDS9CDataInfo9needsFlipEv
bool CImageLoaderDDS::CDataInfo::needsFlip() const { return false; }

// 0x605718 _ZN6glitch5video15CImageLoaderPVR23hasTextureLoadInterfaceEv
bool CImageLoaderPVR::hasTextureLoadInterface() { return true; }

// 0x605720 _ZNK6glitch5video15CImageLoaderPVR9CDataInfo15getFileDataSizeEv
int32_t CImageLoaderPVR::CDataInfo::getFileDataSize() const { return m_fileDataSize; }

// 0x605728 _ZNK6glitch5video15CImageLoaderPVR9CDataInfo12getFilePitchEh
uint32_t CImageLoaderPVR::CDataInfo::getFilePitch(uint8_t) const { return 0; }

// 0x605730 _ZNK6glitch5video15CImageLoaderPVR9CDataInfo14isLittleEndianEv
bool CImageLoaderPVR::CDataInfo::isLittleEndian() const { return true; }

// 0x605738 _ZNK6glitch5video15CImageLoaderPVR9CDataInfo9needsFlipEv
bool CImageLoaderPVR::CDataInfo::needsFlip() const { return false; }

// 0x604adc _ZN6glitch5video15CImageLoaderJPG11init_sourceEP22jpeg_decompress_struct
void CImageLoaderJPG::init_source(jpeg_decompress_struct*) {}

// 0x604ae0 _ZN6glitch5video15CImageLoaderJPG17fill_input_bufferEP22jpeg_decompress_struct
bool CImageLoaderJPG::fill_input_buffer(jpeg_decompress_struct*) { return true; }

// 0x604b08 _ZN6glitch5video15CImageLoaderJPG11term_sourceEP22jpeg_decompress_struct
void CImageLoaderJPG::term_source(jpeg_decompress_struct*) {}

}  // namespace glitch::video
