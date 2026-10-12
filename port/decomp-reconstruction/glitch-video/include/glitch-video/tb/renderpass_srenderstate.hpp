// glitch::video::detail::renderpass::SRenderState: the render-state block that a render
// pass carries. Only the words written by the default constructor (0x588ddc) are named.
// Members are named by offset because the field meanings are not identified yet. The
// total size of the original block is not recovered; more members are expected later.
#pragma once

#include <cstdint>

namespace glitch::video::detail::renderpass {

struct SRenderState {
    SRenderState();  // 0x588ddc

    uint32_t m_word00;  // +0x00, set to 0x18ff0001 by the constructor
    uint32_t m_word04;  // +0x04, set to 0x00180007 by the constructor
    uint8_t m_byte08;   // +0x08, set to 0
    uint8_t m_byte09;   // +0x09, set to 0
    uint8_t m_byte0a;   // +0x0a, set to 0
    uint8_t m_byte0b;   // +0x0b, set to 0
    float m_float0c;    // +0x0c, set to 1.0f
    float m_float10;    // +0x10, set to 1.0f
    uint32_t m_word14;  // +0x14, set to 0
    float m_float18;    // +0x18, set to 1.0f
    float m_float1c;    // +0x1c, set to 1.0f
};

}  // namespace glitch::video::detail::renderpass
