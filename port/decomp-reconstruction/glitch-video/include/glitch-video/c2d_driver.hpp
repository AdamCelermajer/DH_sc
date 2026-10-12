#pragma once

#include <cstdint>

namespace glitch::video {

class IVideoDriver;

// glitch::video::C2DDriver: 2D drawing helper that forwards to an IVideoDriver.
// Only the constructor is reconstructed here. The member names of the fields
// that are zeroed by the constructor are not identified yet, so they keep
// neutral names with their original offsets.
class C2DDriver {
public:
    // 0x59efb4 _ZN6glitch5video9C2DDriverC2EPNS0_12IVideoDriverE
    // 0x59f008 _ZN6glitch5video9C2DDriverC1EPNS0_12IVideoDriverE
    // C1 and C2 share one definition.
    explicit C2DDriver(IVideoDriver* driver);
    virtual ~C2DDriver() = default;

    uint32_t m_unknown04 = 1;        // +0x04, set to 1 by the constructor
    IVideoDriver* m_driver = nullptr; // +0x08, the driver passed to the constructor
    uint32_t m_unknown0c = 0;        // +0x0c
    uint32_t m_unknown10 = 0;        // +0x10
    uint16_t m_unknown14 = 0;        // +0x14
    uint32_t m_unknown18 = 0;        // +0x18
    uint16_t m_unknown1c = 0;        // +0x1c
    uint8_t m_unknown1e = 0;         // +0x1e
};

}  // namespace glitch::video
