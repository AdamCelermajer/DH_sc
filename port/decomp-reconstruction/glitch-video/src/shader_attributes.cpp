#include "glitch-video/shader_attributes.hpp"

namespace glitch::video {

// 0x5e4940 _ZNK6glitch5video7IShader26getVertexAttributeDefIndexENS0_18E_VERTEX_ATTRIBUTEE
// The original multiplies the element byte offset by 0x200000 and shifts right
// by 24, which is the element index. Returns 0xff when no definition matches.
uint32_t IShader::getVertexAttributeDefIndex(uint32_t attribute) const {
    for (uint32_t i = 0; i < m_vertexAttributeDefCount; ++i) {
        if (m_vertexAttributeDefs[i].attribute == attribute) {
            return i;
        }
    }
    return 0xff;
}

}  // namespace glitch::video
