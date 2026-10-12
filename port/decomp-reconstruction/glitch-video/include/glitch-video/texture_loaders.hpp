// Texture-container loaders (ATC, DDS, PVR) and the IImageLoader base probes.
// Only the constant-returning probes and the CDataInfo getters are rebuilt.
#pragma once

#include <cstdint>

#include "glitch-video/deps/glitch_io.hpp"
#include "glitch-video/deps/glitch_video_types.hpp"

struct jpeg_decompress_struct;

namespace glitch::video {

struct STextureLoadInfo;
struct STextureDataInfo;

class IImageLoader {
public:
    virtual ~IImageLoader() = default;
    virtual bool hasTextureLoadInterface();                               // 0x602c84
    virtual bool loadTextureHeader(io::IReadFile* file, STextureDesc& desc) const;  // 0x602c8c

    // Stand-in for the object at +0x08 of ITextureDataLoading. Its vtable is not
    // identified; the default answers are the no-special-case values.
    class ITextureDataSource {
    public:
        virtual ~ITextureDataSource() = default;
        // Vtable +0x0c. Non-zero makes getFilePitch skip the pitch computation.
        virtual int32_t unidentifiedSlot0c() const { return 0; }
        // Vtable +0x10. Destination pointer for the byte swap in read.
        virtual void* unidentifiedSlot10() const { return nullptr; }
    };

    // Texture data reader used by the loaders (0x607654 / 0x6076a8 constructors).
    // Only the fields that the reconstructed paths touch are named; the source
    // and loader objects (+0x08, +0x10) are not modelled, see the member notes.
    class ITextureDataLoading {
    public:
        ITextureDataLoading();
        virtual ~ITextureDataLoading() = default;

        // 0x6077c4 (void in the original): the pitch value is computed and discarded
        // by the original, so only the flag at +0x22 is kept. Partial.
        void getFilePitch(uint8_t mip);
        // 0x607898: bytes of one mip level, from the file or the in-memory descriptor. Partial.
        int32_t getSourceStep(uint8_t mip) const;
        // 0x607d1c: reads size bytes and checks the count. The byte-swap path (pixel_format::swapBytes)
        // is not reconstructed. Partial.
        uint32_t read(void* buffer, int32_t size);

        io::IReadFile* m_file = nullptr;              // +0x04
        ITextureDataSource* m_source = nullptr;       // +0x08
        STextureLoadInfo* m_desc = nullptr;           // +0x0c
        const STextureDataInfo* m_info = nullptr;     // +0x10
        int32_t m_unknown14 = 0;                      // +0x14, zeroed by the constructors
        int32_t m_unknown18 = 0;                      // +0x18, zeroed by the constructors
        int32_t m_sourceStep = 0;                     // +0x1c, multiplier used by getSourceStep
        bool m_flag20 = false;                        // +0x20, set when a read fails
        bool m_flag21 = true;                         // +0x21, set by the constructors
        bool m_pitchKnown = false;                    // +0x22, set by getFilePitch
    };

    uint32_t m_refCount = 1;  // +0x04, set to 1 by the loader constructors
};

// Minimal descriptor shapes used by ITextureDataLoading. Offsets are the
// original's; the field names follow how the reconstructed code reads them.
struct STextureLoadInfo {
    uint32_t unknown00 = 0;               // +0x00
    E_PIXEL_FORMAT format = 0;            // +0x04
    uint32_t layoutType = 0;              // +0x08, a value of 1 selects the cube-face size path
    uint32_t unknown0c = 0;               // +0x0c
    uint32_t width = 0;                   // +0x10
    uint32_t height = 0;                  // +0x14
    uint32_t depth = 0;                   // +0x18
};

// Header block reached through the first word of STextureDataInfo.
struct STextureHeaderBlock {
    uint8_t unknown00[0x24] = {};         // +0x00..+0x23
    uint32_t width = 0;                   // +0x24
    uint32_t height = 0;                  // +0x28
};

struct STextureDataInfo {
    const STextureHeaderBlock* header = nullptr;  // +0x00
    uint8_t unknown04[9] = {};            // +0x04..+0x0c
    bool swapBytes = false;               // +0x0d, read uses it to choose the swap path
};

// DDS_HEADER as read after the "DDS " magic (0x7c bytes, first word is the size).
struct SDDSSurfaceFormatHeader {
    uint32_t size = 0;                    // +0x00 (0x7c)
    uint32_t flags = 0;                   // +0x04
    uint32_t height = 0;                  // +0x08
    uint32_t width = 0;                   // +0x0c
    uint32_t pitchOrLinearSize = 0;       // +0x10
    uint32_t depth = 0;                   // +0x14
    uint32_t mipMapCount = 0;             // +0x18
    uint32_t reserved1[11] = {};          // +0x1c..+0x47
    uint32_t pixelFormatSize = 0;         // +0x48, DDS_PIXELFORMAT.dwSize (0x20)
    uint32_t pixelFormat[7] = {};         // +0x4c..+0x67, the rest of DDS_PIXELFORMAT
    uint32_t caps[4] = {};                // +0x68..+0x77
    uint32_t reserved2 = 0;               // +0x78, header ends at 0x7c
};

// 0x6041cc (anonymous namespace in the original): reads the DDS header at file
// offset 4 and returns true when the size word matches the bytes read and the
// pixel-format size is 32.
bool readDDSHeader(io::IReadFile* file, SDDSSurfaceFormatHeader& header);

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
    CImageLoaderDDS();  // 0x604268 (C2) / 0x604294 (C1)
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
    CImageLoaderPVR();  // 0x605740 (C2) / 0x60576c (C1)
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
    CImageLoaderJPG();  // 0x604a7c (C2) / 0x604aa8 (C1)
    // 0x604b0c: "JFIF" (or the byte-swapped form) at offset 6.
    bool isALoadableFileFormat(io::IReadFile* file) const;
    // 0x604f6c: ".jpg" or ".JPG" in the name.
    bool isALoadableFileExtension(const char* filename) const;
    virtual ~CImageLoaderJPG() = default;
    void init_source(jpeg_decompress_struct*);       // 0x604adc
    bool fill_input_buffer(jpeg_decompress_struct*); // 0x604ae0
    void term_source(jpeg_decompress_struct*);       // 0x604b08

private:
    uint32_t m_unknown04 = 1;  // +0x04, set to 1 by the constructors
};

// Factories: new object, default constructed, returned to the caller.
CImageLoaderATC* createImageLoaderATC();  // 0x602dc4
CImageLoaderDDS* createImageLoaderDDS();  // 0x604344
CImageLoaderJPG* createImageLoaderJPG();  // 0x604bc0
CImageLoaderPVR* createImageLoaderPVR();  // 0x6057c0

}  // namespace glitch::video
