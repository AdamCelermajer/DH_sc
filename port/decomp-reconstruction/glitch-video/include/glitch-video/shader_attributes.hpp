// glitch::video::IShader: the shader base. Holds the vertex attribute definitions,
// the per-stage parameter lists, the shader id and the GL program name.
// Offsets in comments are the ARM layout of the original.
#pragma once

#include <cstdint>
#include <string>

#include "glitch-video/deps/intrusive_ptr.hpp"

namespace glitch::io {
class IAttributes;
}  // namespace glitch::io

namespace glitch::video {

class IVideoDriver;
class IBatchBaker;

// 8-byte entry in the original (SShaderVertexAttributeDef).
struct SShaderVertexAttributeDef {
    uint32_t field00 = 0;      // +0x00, not identified
    uint16_t attribute = 0;    // +0x04, E_VERTEX_ATTRIBUTE
    uint16_t field06 = 0;      // +0x06, not identified
};

// 16-byte entry in the original (SShaderParameterDef). Only the type is used here.
struct SShaderParameterDef {
    uint32_t field00 = 0;          // +0x00, not identified
    uint16_t parameterType = 0;    // +0x04, E_SHADER_PARAMETER_TYPE
    uint16_t field06 = 0;          // +0x06, not identified
    uint32_t field08 = 0;          // +0x08, not identified
    uint32_t field0c = 0;          // +0x0c, not identified
};

// Per-stage parameter list (8 bytes in the original: pointer, halfword, halfword count).
struct SShaderStageParameters {
    SShaderParameterDef* parameters = nullptr;  // +0x00 of the record
    uint16_t field04 = 0;                       // +0x04, not identified
    uint16_t count = 0;                         // +0x06, number of parameter defs
};

class IShader {
public:
    IShader() = default;
    // 0x5e4a4c (C1) / 0x5e4ae0 (C2). Base of the shader classes; the vtable store is implicit.
    IShader(uint16_t id, const char* name, IVideoDriver* driver);
    virtual ~IShader() = default;

    // 0x5e48e4: index of the parameter with the given type in stage's list, 0xffff when absent.
    // Stage 2 and above are not identified in the original and always return 0xffff here.
    uint32_t getParameterID(uint32_t parameterType, uint32_t stage, uint16_t index) const;
    // 0x5e4b74: lookup by name. Partial: the name comparison needs SSharedString, not in the port.
    uint32_t getParameterID(const char* name, uint32_t stage, uint16_t index) const;
    // 0x5e4998: partial. IShaderManager::getBatchBaker is not ported; returns null.
    boost::intrusive_ptr<IBatchBaker> getBatchBaker() const;
    // 0x5e4940: index of the first definition with the attribute, 0xff when absent.
    uint32_t getVertexAttributeDefIndex(uint32_t attribute) const;

    // 0x5e4f68 / 0x5e5188: partial. The IAttributes vtable slots are not identified, so
    // the attribute traversal is not translated.
    void deserializeAttributes(io::IAttributes* attributes);
    void serializeAttributes(io::IAttributes* attributes) const;

    IVideoDriver* m_driver = nullptr;                              // +0x08
    std::string m_name;                                           // +0x0c
    SShaderVertexAttributeDef* m_vertexAttributeDefs = nullptr;   // +0x24
    SShaderStageParameters m_stageParameters[2];                  // +0x28, +0x30 (stages 0 and 1)
    uint32_t m_vertexAttributeMask = 0;                           // +0x38, written by (de)serialize
    uint8_t m_vertexAttributeDefCount = 0;                        // +0x3c
    uint8_t m_unknown3d = 0xff;                                   // +0x3d, set to 0xff by the ctor
    uint8_t m_unknown3e = 1;                                      // +0x3e, set to 1 by the ctor
    uint16_t m_id = 0;                                            // +0x40, shader id
    uint32_t m_glProgram = 0;                                     // +0x4c, GL program name (set by CGLSLShader::createProgram)
};

}  // namespace glitch::video
