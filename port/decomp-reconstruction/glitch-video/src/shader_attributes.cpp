#include "glitch-video/shader_attributes.hpp"

#include "glitch-video/video_driver.hpp"

namespace glitch::video {

// 0x5e4a4c _ZN6glitch5video7IShaderC1EtPKcPNS0_12IVideoDriverE
// 0x5e4ae0 _ZN6glitch5video7IShaderC2EtPKcPNS0_12IVideoDriverE
IShader::IShader(uint16_t id, const char* name, IVideoDriver* driver)
    : m_driver(driver), m_name(name) {
    // Stage records, attribute list and mask start at zero.
    m_vertexAttributeDefs = nullptr;
    m_vertexAttributeMask = 0;
    m_vertexAttributeDefCount = 0;
    m_unknown3d = 0xff;
    m_unknown3e = 1;
    m_id = id;
}

// 0x5e4940 _ZNK6glitch5video7IShader26getVertexAttributeDefIndexENS0_18E_VERTEX_ATTRIBUTEE
uint32_t IShader::getVertexAttributeDefIndex(uint32_t attribute) const {
    for (uint32_t i = 0; i < m_vertexAttributeDefCount; ++i) {
        if (m_vertexAttributeDefs[i].attribute == attribute) {
            return i;
        }
    }
    return 0xff;
}

// 0x5e48e4 _ZNK6glitch5video7IShader14getParameterIDENS0_23E_SHADER_PARAMETER_TYPEENS0_14E_SHADER_STAGEEt
// Stage 2 and above have no record in the port (see the header).
uint32_t IShader::getParameterID(uint32_t parameterType, uint32_t stage, uint16_t index) const {
    if (stage >= 2) {
        return 0xffff;
    }
    const SShaderStageParameters& list = m_stageParameters[stage];
    const uint32_t count = list.count;
    for (uint32_t i = index; i < count; ++i) {
        if (list.parameters[i].parameterType == parameterType) {
            return i;
        }
    }
    return 0xffff;
}

// 0x5e4b74 _ZNK6glitch5video7IShader14getParameterIDEPKcNS0_14E_SHADER_STAGEEt
// Partial: the original resolves the name through SSharedString and compares the shared
// string data pointers. The port has no SSharedString, so no name is ever matched here.
// The original's not-found return is shown by the decompiler as a bogus address; the
// port returns 0xffff, the not-found value used by the index lookup above.
uint32_t IShader::getParameterID(const char* /*name*/, uint32_t /*stage*/, uint16_t /*index*/) const {
    return 0xffff;
}

// 0x5e4998 _ZNK6glitch5video7IShader13getBatchBakerEv
// Partial: the original forwards to IShaderManager::getBatchBaker(driver->shaderManager, m_id),
// and IShaderManager is not ported.
boost::intrusive_ptr<IBatchBaker> IShader::getBatchBaker() const {
    return boost::intrusive_ptr<IBatchBaker>();
}

// 0x5e4f68 _ZN6glitch5video7IShader21deserializeAttributesEPNS_2io11IAttributesE
// Partial: the traversal uses IAttributes vtable slots that are not identified
// (begin/end group, read int, read string). Nothing is read here yet.
void IShader::deserializeAttributes(io::IAttributes* /*attributes*/) {}

// 0x5e5188 _ZNK6glitch5video7IShader19serializeAttributesEPNS_2io11IAttributesE
// Partial: the same IAttributes vtable slots as deserializeAttributes; nothing is written here.
void IShader::serializeAttributes(io::IAttributes* /*attributes*/) const {}

}  // namespace glitch::video
