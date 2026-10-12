// glitch::video::IShader: the part of the shader base that holds the vertex
// attribute definitions. The rest of the class is not reconstructed.
#pragma once

#include <cstdint>

namespace glitch::video {

// 8-byte entry in the original (SShaderVertexAttributeDef).
struct SShaderVertexAttributeDef {
    uint32_t field00 = 0;      // +0x00, not identified
    uint16_t attribute = 0;    // +0x04, E_VERTEX_ATTRIBUTE
    uint16_t field06 = 0;      // +0x06, not identified
};

class IShader {
public:
    // 0x5e4940: index of the first definition with the attribute, 0xff when absent.
    uint32_t getVertexAttributeDefIndex(uint32_t attribute) const;

    SShaderVertexAttributeDef* m_vertexAttributeDefs = nullptr;  // +0x24
    uint8_t m_vertexAttributeDefCount = 0;                       // +0x3c
};

}  // namespace glitch::video
