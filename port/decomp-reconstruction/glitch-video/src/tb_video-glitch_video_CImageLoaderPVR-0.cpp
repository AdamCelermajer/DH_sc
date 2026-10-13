// Track B batch e1-glitch-video-glitch_video_CImageLoaderPVR-0 (glitch::video::CImageLoaderPVR).
#include <cstdint>

#include "glitch-video/deps/os_logging.hpp"
#include "glitch-video/pixel_format.hpp"
#include "glitch-video/texture_loaders.hpp"

namespace glitch::video {
namespace {

// Flag bits of SPVRHeader::flags that loadTextureHeader tests.
constexpr uint32_t kPVRFlagTwiddled = 0x200;
constexpr uint32_t kPVRFlagMipMap = 0x100;
constexpr uint32_t kPVRFlagCubeMap = 0x1000;
constexpr uint32_t kPVRFlagVolume = 0x4000;
constexpr uint32_t kPVRFlagAltFormat = 0x8000;

// Size of the PVR header that follows the container tag.
constexpr uint32_t kPVRHeaderBytes = 52;

}  // namespace

// 0x605b10 _ZNK6glitch5video15CImageLoaderPVR17loadTextureHeaderEPNS_2io9IReadFileERNS0_12STextureDescE
// The header comes from readPVRHeader (glitch::video::readPVRHeader, 0x6058a4, exposed in
// texture_loaders.hpp); the file name in the log messages is IReadFile::getFileName (vtable +0x28). The
// format mapping follows the Hex-Rays switch; the pixel-format table (detail::PFDTable) is read for the
// twiddle check. The log calls go to Printer::logf, whose output glitch-core-io still drops (its own row).
bool CImageLoaderPVR::loadTextureHeader(io::IReadFile* file, STextureDesc& desc) const {
    SPVRHeader header;
    bool isBTEX = false;
    // The original returns this result unchanged: 0 when the header is rejected.
    if (!readPVRHeader(file, header, isBTEX)) {
        return false;
    }

    const uint32_t flags = header.flags;

    // +0x00: 2 for a cube map, 1 for a volume, 0 otherwise.
    if ((flags & kPVRFlagCubeMap) != 0) {
        desc.type = 2;
    } else {
        desc.type = (flags & kPVRFlagVolume) != 0 ? 1 : 0;
    }

    desc.height = header.height;                                      // +0x14
    desc.width = header.width;                                        // +0x10
    desc.unknown08 = 0;                                               // +0x08
    desc.depth = desc.type == 1 ? header.surfaceCount : 1;            // +0x18
    desc.flag = static_cast<uint8_t>((flags & kPVRFlagMipMap) != 0);  // +0x1c, bit 8 of the flags

    // The file must hold exactly the header (plus the 8-byte tag for BTEX) and the data for the
    // surface count (6 faces for a cube map). Compared in 32 bits as the original does.
    const uint32_t trailer = isBTEX ? 8u : 0u;
    const uint32_t multiplier = desc.type == 2 ? 6u : 1u;
    const uint32_t fileSize = static_cast<uint32_t>(file->getSize());
    if (fileSize - kPVRHeaderBytes - trailer != header.textureDataSize * multiplier) {
        os::Printer::logf(os::ELL_ERROR, "loading %s: file size mismatches header info", file->getFileName());
        return false;
    }

    // +0x04: glitch format id from the PowerVR pixel format in the low byte of the flags.
    uint32_t format = 0;
    switch (static_cast<uint8_t>(flags & 0xff)) {
        case 0: format = 6; break;
        case 1: format = 8; break;
        case 2:
        case 19: format = 5; break;
        case 4:
        case 21: format = 10; break;
        case 5:
        case 26: format = 13; break;
        case 7:
        case 22: format = 0; break;
        case 8:
        case 23: format = 4; break;
        case 12:
        case 24: format = (flags & kPVRFlagAltFormat) != 0 ? 25 : 24; break;
        case 13:
        case 25: format = (flags & kPVRFlagAltFormat) != 0 ? 27 : 26; break;
        case 16: format = 7; break;
        case 17: format = 9; break;
        case 18: format = 14; break;
        case 32: format = (flags & kPVRFlagAltFormat) != 0 ? 18 : 17; break;
        case 33:
        case 34: format = 19; break;
        case 35:
        case 36: format = 20; break;
        case 42: format = 16; break;
        case 57: format = 2; break;
        case 59: format = 1; break;
        case 80: format = 31; break;
        case 83: format = 30; break;
        case 86: format = 29; break;
        default:
            // The original format string is "%0x02u": a hex conversion with the 0 flag, then "02u".
            os::Printer::logf(os::ELL_ERROR, "loading %s: pixel format %0x02u not supported", file->getFileName(),
                              static_cast<unsigned>(flags & 0xff));
            return false;
    }
    desc.format = format;

    // Twiddled data is only accepted when the table row for the format is marked as supported
    // (bit 8 of its first word). Rows past the end of the table are not read: all ids above are < 40.
    if ((flags & kPVRFlagTwiddled) == 0 || (pixel_format::detail::PFDTable[format].flags & 8u) != 0) {
        return true;
    }
    os::Printer::logf(os::ELL_ERROR, "loading %s: twiddled format not supported", file->getFileName());
    return false;
}

}  // namespace glitch::video
