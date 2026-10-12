// Texture-container loaders (ATC, DDS, PVR) and the IImageLoader base probes.
// Only the constant-returning probes and the CDataInfo getters are rebuilt.
#pragma once

#include <cstdint>

#include "glitch-video/deps/glitch_io.hpp"
#include "glitch-video/deps/glitch_video_types.hpp"

struct jpeg_decompress_struct;

namespace glitch::video {

class IImageLoader {
public:
    virtual ~IImageLoader() = default;
    virtual bool hasTextureLoadInterface();                               // 0x602c84
    virtual bool loadTextureHeader(io::IReadFile* file, STextureDesc& desc) const;  // 0x602c8c
};

class CImageLoaderATC : public IImageLoader {
public:
    class CDataInfo {
    public:
        int32_t getFileDataSize() const;     // 0x602ca4
        uint32_t getFilePitch(uint8_t) const;  // 0x602cac
        bool isLittleEndian() const;         // 0x602cb4
        bool needsFlip() const;              // 0x602cbc
    private:
        int32_t m_fileDataSize = 0;          // +0x0c
    };
    bool hasTextureLoadInterface() override;  // 0x602c9c
};

class CImageLoaderDDS : public IImageLoader {
public:
    class CDataInfo {
    public:
        int32_t getFileDataSize() const;     // 0x60422c
        bool isLittleEndian() const;         // 0x604258
        bool needsFlip() const;              // 0x604260
    private:
        int32_t m_fileDataSize = 0;          // +0x0c
    };
    bool hasTextureLoadInterface() override;  // 0x6041c4
};

class CImageLoaderPVR : public IImageLoader {
public:
    class CDataInfo {
    public:
        int32_t getFileDataSize() const;     // 0x605720
        uint32_t getFilePitch(uint8_t) const;  // 0x605728
        bool isLittleEndian() const;         // 0x605730
        bool needsFlip() const;              // 0x605738
    private:
        int32_t m_fileDataSize = 0;          // +0x0c
    };
    bool hasTextureLoadInterface() override;  // 0x605718
};

// libjpeg source-manager callbacks used by the JPG loader.
class CImageLoaderJPG {
public:
    virtual ~CImageLoaderJPG() = default;
    void init_source(jpeg_decompress_struct*);       // 0x604adc
    bool fill_input_buffer(jpeg_decompress_struct*); // 0x604ae0
    void term_source(jpeg_decompress_struct*);       // 0x604b08
};

}  // namespace glitch::video
