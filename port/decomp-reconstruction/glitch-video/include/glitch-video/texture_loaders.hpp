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
    // 0x602cc4: reads the ATC magic (16-bit value at offset 4 is 0x8c92 or 0x8c93).
    bool isALoadableFileFormat(io::IReadFile* file) const;
    // 0x603254: ".atc" or ".ATC" in the name.
    bool isALoadableFileExtension(const char* filename) const;
};

class CImageLoaderDDS : public IImageLoader {
public:
    class CDataInfo {
    public:
        int32_t getFileDataSize() const;     // 0x60422c
        // 0x604234: pitch from the DDS header when mip 0 is asked and the pitch flag is set.
        uint32_t getFilePitch(uint8_t mip) const;
        bool isLittleEndian() const;         // 0x604258
        bool needsFlip() const;              // 0x604260
    private:
        int32_t m_fileDataSize = 0;          // +0x0c
        const uint32_t* m_header = nullptr;  // +0x04, DDS header as 32-bit words
    };
    bool hasTextureLoadInterface() override;  // 0x6041c4
    // 0x6042c0: reads the first four bytes and checks for "DDS ".
    bool isALoadableFileFormat(io::IReadFile* file) const;
    // 0x60490c: ".dds" in the name.
    bool isALoadableFileExtension(const char* filename) const;
};

class CImageLoaderPVR : public IImageLoader {
public:
    // 0x6057e0: a 52-byte header with "PVR!" at offset 44, or "BTEXpvr" at offset 0.
    bool isALoadableFileFormat(io::IReadFile* file) const;
    // 0x6061b4: ".pvr" or ".btex" in the name.
    bool isALoadableFileExtension(const char* filename) const;
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
    // 0x604b0c: "JFIF" (or the byte-swapped form) at offset 6.
    bool isALoadableFileFormat(io::IReadFile* file) const;
    // 0x604f6c: ".jpg" or ".JPG" in the name.
    bool isALoadableFileExtension(const char* filename) const;
    virtual ~CImageLoaderJPG() = default;
    void init_source(jpeg_decompress_struct*);       // 0x604adc
    bool fill_input_buffer(jpeg_decompress_struct*); // 0x604ae0
    void term_source(jpeg_decompress_struct*);       // 0x604b08
};

}  // namespace glitch::video
