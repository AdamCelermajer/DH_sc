// glitch::video::CLight: light record with a shared-count word at +0x00.
// Layout from the constructors and the copy constructor (0x59fc40, 0x59fe08, 0x59feb8).
// The float defaults are as the original stores them. The STransformationSource
// handle at +0x50 is a slot that the source object owns; its class is not reconstructed.
#pragma once

#include <cstdint>

#include "glitch-video/deps/intrusive_ptr.hpp"

namespace glitch::video {

// Stand-in for glitch::video::STransformationSource (not reconstructed here).
// detach() releases the source slot; the body lives in another category.
struct STransformationSource {
    static void detach(uint32_t* slot);
};

class CLight {
public:
    // Copy constructor (0x59fc40 C1, 0x59fd48 C2). The source handle is not copied: it is
    // reset, and *out (when given) receives the address of the new handle slot.
    CLight(const CLight& source, STransformationSource** out);
    // Default constructor (0x59fe08 C1, 0x59fef8 C2).
    explicit CLight(STransformationSource** out);

    // 0x59fd00: heap copy, returned with the count set to 1.
    boost::intrusive_ptr<CLight> clone(STransformationSource** out) const;
    // 0x59feb8: heap allocation, returned with the count set to 1.
    static boost::intrusive_ptr<CLight> allocate(STransformationSource** out);

    uint32_t m_refCount = 0;          // +0x00
    uint32_t m_word04[3] = {};        // +0x04..+0x0c
    float m_float10[10] = {};         // +0x10..+0x34, default 1.0
    uint32_t m_word38[2] = {};        // +0x38, +0x3c
    float m_float40 = 0.0f;           // +0x40, default FLT_MAX (0x7f7fffff)
    uint32_t m_word44 = 0;            // +0x44
    float m_float48 = 0.0f;           // +0x48, default 40.0
    float m_float4c = 0.0f;           // +0x4c, default 2.0
    uint32_t m_sourceHandle = 0;      // +0x50, STransformationSource slot
    uint8_t m_flag54 = 0;             // +0x54, set to 1 by both constructors
    uint16_t m_word58 = 0;            // +0x58
    uint8_t m_flag5a = 0;             // +0x5a, set to 1 by the default constructor
};

}  // namespace glitch::video
