// glitch::video::CMaterialRendererManager. Only the two lookups that take a temporary id are
// reconstructed here. The technique and renderer tables of the original are not in the port.
#pragma once

#include <cstdint>

namespace glitch::video {

class CMaterialRendererManager {
public:
    // Temporary id, passed by value as one word in the original.
    struct STemporaryID {
        uint32_t value = 0;
    };

    // 0x5d7cf4: returns the id when the flag word at +0x90 is set and the id is not zero,
    // otherwise 0.
    int32_t getTechnique(STemporaryID id) const;
    // 0x5d7d14: the same test and result as getTechnique.
    int32_t getParameterDef(STemporaryID id) const;

    uint32_t m_unknown90 = 0;  // +0x90, flag word; the name is not identified
};

}  // namespace glitch::video
