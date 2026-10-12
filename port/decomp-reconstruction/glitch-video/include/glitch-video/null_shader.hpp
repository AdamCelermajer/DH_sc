// glitch::video::CNullShader: shader stub used by the null driver. Only the
// layout pieces the hash and attribute code touch are modelled; the IShader
// base class is not reconstructed, so base-class calls are noted at each use.
#pragma once

#include <cstdint>

namespace glitch::io {
class IAttributes;
}

namespace glitch::video {

class CNullDriver;

class CNullShader {
public:
    CNullShader(CNullDriver* driver);                                    // 0x6e0670 (C1), 0x6e06b8 (C2)
    CNullShader(uint16_t id, const char* name, CNullDriver* driver);     // 0x6e0700 (C1), 0x6e0744 (C2)
    ~CNullShader();                                                       // 0x6e05ec (D1), 0x6e063c (D2)

    void releaseDriverSpecificResources();  // 0x6e0554
    void bindShader() const;                // 0x6e0558
    void updateHash() const;                // 0x6e055c
    uint32_t getShaderSize() const;         // 0x6e0570
    uint32_t getHashValue() const;          // 0x6e0590
    void deserializeAttributes(io::IAttributes* attributes);  // 0x6e05d0

    uint16_t m_id = 0;                      // +0x40, set from the id argument of the named ctor
    mutable uint32_t m_hashValue = 0;       // +0x44, written by updateHash
    mutable bool m_hashDirty = false;       // +0x48, cleared by updateHash
    mutable uint32_t m_shaderSize = 0;      // +0x4c, returned by getShaderSize
    uint32_t m_vertexAttributeMask = 0;     // +0x38, set to 1 by the named ctor
};

}  // namespace glitch::video
