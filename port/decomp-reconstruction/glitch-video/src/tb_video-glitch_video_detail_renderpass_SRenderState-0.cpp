#include "glitch-video/tb/renderpass_srenderstate.hpp"

#include <cstddef>

namespace glitch::video::detail::renderpass {

// Layout checks for the offsets named in the header.
static_assert(offsetof(SRenderState, m_word00) == 0x00, "SRenderState +0x00");
static_assert(offsetof(SRenderState, m_word04) == 0x04, "SRenderState +0x04");
static_assert(offsetof(SRenderState, m_byte08) == 0x08, "SRenderState +0x08");
static_assert(offsetof(SRenderState, m_byte09) == 0x09, "SRenderState +0x09");
static_assert(offsetof(SRenderState, m_byte0a) == 0x0a, "SRenderState +0x0a");
static_assert(offsetof(SRenderState, m_byte0b) == 0x0b, "SRenderState +0x0b");
static_assert(offsetof(SRenderState, m_float0c) == 0x0c, "SRenderState +0x0c");
static_assert(offsetof(SRenderState, m_float10) == 0x10, "SRenderState +0x10");
static_assert(offsetof(SRenderState, m_word14) == 0x14, "SRenderState +0x14");
static_assert(offsetof(SRenderState, m_float18) == 0x18, "SRenderState +0x18");
static_assert(offsetof(SRenderState, m_float1c) == 0x1c, "SRenderState +0x1c");

// 0x588ddc _ZN6glitch5video6detail10renderpass12SRenderStateC1Ev
// Writes the default words in the order of the original. Fields after +0x1c are not
// written here and are not named.
SRenderState::SRenderState() {
    m_word00 = 0x18ff0001u;
    m_byte0b = 0;
    m_word14 = 0;
    m_float1c = 1.0f;
    m_word04 = 0x00180007u;
    m_byte08 = 0;
    m_byte09 = 0;
    m_byte0a = 0;
    m_float0c = 1.0f;
    m_float10 = 1.0f;
    m_float18 = 1.0f;
}

}  // namespace glitch::video::detail::renderpass
