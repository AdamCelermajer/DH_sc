#include "glitch-video/null_shader.hpp"

namespace glitch::video {

// 0x6e0670 _ZN6glitch5video11CNullShaderC1EPNS0_11CNullDriverE
// 0x6e06b8 _ZN6glitch5video11CNullShaderC2EPNS0_11CNullDriverE
// Base IShader(0, "", driver) is not modelled; the vtable store is implicit.
CNullShader::CNullShader(CNullDriver*) {}

// 0x6e0700 _ZN6glitch5video11CNullShaderC1EtPKcPNS0_11CNullDriverE
// 0x6e0744 _ZN6glitch5video11CNullShaderC2EtPKcPNS0_11CNullDriverE
// Base IShader(id, name, driver) is not modelled.
CNullShader::CNullShader(uint16_t id, const char*, CNullDriver*) : m_id(id) {
    updateHash();
    m_vertexAttributeMask = 1;
}

// 0x6e05ec _ZN6glitch5video11CNullShaderD1Ev
// 0x6e063c _ZN6glitch5video11CNullShaderD2Ev
// Base IShader destructor is not modelled.
CNullShader::~CNullShader() = default;

// 0x6e0554 _ZN6glitch5video11CNullShader30releaseDriverSpecificResourcesEv
void CNullShader::releaseDriverSpecificResources() {}

// 0x6e0558 _ZNK6glitch5video11CNullShader10bindShaderEv
void CNullShader::bindShader() const {}

// 0x6e055c _ZNK6glitch5video11CNullShader10updateHashEv
void CNullShader::updateHash() const {
    m_hashDirty = false;
    m_hashValue = m_id;
}

// 0x6e0570 _ZNK6glitch5video11CNullShader13getShaderSizeEv
uint32_t CNullShader::getShaderSize() const {
    if (m_hashDirty) updateHash();
    return m_shaderSize;
}

// 0x6e0590 _ZNK6glitch5video11CNullShader12getHashValueEv
uint32_t CNullShader::getHashValue() const {
    if (m_hashDirty) updateHash();
    return m_hashValue;
}

// 0x6e05d0 _ZN6glitch5video11CNullShader21deserializeAttributesEPNS_2io11IAttributesE
// The original first calls IShader::deserializeAttributes (base, not modelled).
void CNullShader::deserializeAttributes(io::IAttributes*) {
    updateHash();
}

}  // namespace glitch::video
