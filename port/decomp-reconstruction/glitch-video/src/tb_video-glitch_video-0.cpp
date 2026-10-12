// Track B batch e1-glitch-video-glitch_video-0 (glitch::video, PVR header probe).
#include <algorithm>
#include <cstdint>
#include <cstring>

#include "glitch-video/deps/glitch_io.hpp"
#include "glitch-video/deps/os_logging.hpp"

namespace glitch::video {
namespace {

// Header size and the flag bits the PVR probe tests.
constexpr uint32_t kPVRHeaderSize = 52;
constexpr uint32_t kPVRFlagMipMap = 0x100;
constexpr uint32_t kPVRFlagCubeMap = 0x1000;
constexpr uint32_t kPVRFlagVolume = 0x4000;  // the surface-count word is the depth then

// The 52-byte PowerVR (version 2) header. Each field is decoded from the 32-bit little-endian
// file layout; the struct is never cast from file bytes. Field names follow the PowerVR SDK
// header layout, which the original's offsets match. The names are not in the binary.
struct SPVRHeader {
    uint32_t headerSize = 0;       // +0x00, must be 52
    uint32_t height = 0;           // +0x04
    uint32_t width = 0;            // +0x08
    uint32_t mipMapCount = 0;      // +0x0c
    uint32_t flags = 0;            // +0x10
    uint32_t textureDataSize = 0;  // +0x14
    uint32_t bitsPerPixel = 0;     // +0x18
    uint32_t redBitMask = 0;       // +0x1c
    uint32_t greenBitMask = 0;     // +0x20
    uint32_t blueBitMask = 0;      // +0x24
    uint32_t alphaBitMask = 0;     // +0x28
    char pvrTag[4] = {};           // +0x2c, "PVR!"
    uint32_t surfaceCount = 0;     // +0x30 (surface or cube-face count, or volume depth)
};
static_assert(kPVRHeaderSize == 52, "PVR header is 52 bytes");

uint32_t readLE32(const uint8_t* p) {
    return static_cast<uint32_t>(p[0]) | (static_cast<uint32_t>(p[1]) << 8) |
           (static_cast<uint32_t>(p[2]) << 16) | (static_cast<uint32_t>(p[3]) << 24);
}

// Index of the highest set bit, as the original's shift loop computes it. Zero gives
// 0xFFFFFFFF (the loop starts at -1). The original compares these unsigned, so the value
// stays unsigned here.
uint32_t highestBitIndex(uint32_t value) {
    uint32_t index = 0xFFFFFFFFu;
    if (value != 0) {
        do {
            value >>= 1;
            ++index;
        } while (value != 0);
    }
    return index;
}

// 0x6058a4 _ZN6glitch5video12_GLOBAL__N_113readPVRHeaderEPNS_2io9IReadFileERNS1_10SPVRHeaderERb
// Reads the header. A plain header is the 8 bytes of size and height followed by 44 more,
// with "PVR!" at +0x2c. A "BTEXpvr" wrapper is 8 bytes followed by the 52-byte header.
// Returns false on a short read, a wrong tag or size, a bad flag combination, or a mip
// chain that does not match the dimensions. isBTEX is set once the wrapper tag is read.
[[maybe_unused]] bool readPVRHeader(io::IReadFile* file, SPVRHeader& header, bool& isBTEX) {
    file->seek(0, false);
    isBTEX = false;

    uint8_t first[8] = {};
    file->read(first, 8);

    uint8_t raw[kPVRHeaderSize] = {};
    if (std::memcmp(first, "BTEXpvr", 8) != 0) {
        std::memcpy(raw, first, 8);
        if (file->read(raw + 8, 44) != 44) {
            return false;
        }
    } else {
        const int32_t got = file->read(raw, kPVRHeaderSize);
        isBTEX = true;
        if (got != static_cast<int32_t>(kPVRHeaderSize)) {
            return false;
        }
    }

    // Decode the fields from the file layout. On a short read this is skipped. The
    // original leaves the raw bytes in the caller's struct; that difference is noted.
    header.headerSize = readLE32(raw + 0x00);
    header.height = readLE32(raw + 0x04);
    header.width = readLE32(raw + 0x08);
    header.mipMapCount = readLE32(raw + 0x0c);
    header.flags = readLE32(raw + 0x10);
    header.textureDataSize = readLE32(raw + 0x14);
    header.bitsPerPixel = readLE32(raw + 0x18);
    header.redBitMask = readLE32(raw + 0x1c);
    header.greenBitMask = readLE32(raw + 0x20);
    header.blueBitMask = readLE32(raw + 0x24);
    header.alphaBitMask = readLE32(raw + 0x28);
    std::memcpy(header.pvrTag, raw + 0x2c, 4);
    header.surfaceCount = readLE32(raw + 0x30);

    if (std::memcmp(header.pvrTag, "PVR!", 4) != 0) {
        return false;
    }
    if (header.headerSize != kPVRHeaderSize) {
        return false;
    }
    const uint32_t flags = header.flags;
    if ((flags & kPVRFlagMipMap) != 0 && header.mipMapCount == 0) {
        return false;
    }
    if ((flags & kPVRFlagCubeMap) != 0 && header.surfaceCount != 6) {
        return false;
    }
    if ((flags & kPVRFlagMipMap) == 0) {
        return true;
    }

    // Levels expected from the dimensions. The original picks the larger of the width and
    // height logs (ties go to width), then the larger of that and the volume depth log. Its
    // unsigned compares give the same result as std::max over the three values.
    const uint32_t widthLog = highestBitIndex(header.width);
    const uint32_t heightLog = highestBitIndex(header.height);
    const uint32_t depthLog = (flags & kPVRFlagVolume) != 0 ? highestBitIndex(header.surfaceCount)
                                                            : highestBitIndex(1);
    const uint32_t expectedLevels = std::max(std::max(widthLog, heightLog), depthLog);
    if (header.mipMapCount != expectedLevels) {
        const char* name = file->getFileNameText();
        os::Printer::logf(os::ELL_ERROR, "loading %s: incomplete mipmap chain not supported", name);
        return false;
    }
    return true;
}

}  // namespace
}  // namespace glitch::video
