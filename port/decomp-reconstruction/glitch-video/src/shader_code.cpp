#include "glitch-video/shader_code.hpp"

#include <cstring>

namespace glitch::video {

// 0x6e2130 _ZN6glitch5video11IShaderCodeC1EPKc
// 0x6e2178 _ZN6glitch5video11IShaderCodeC2EPKc
IShaderCode::IShaderCode(const char* name) : m_name(name) {}

// 0x6df690 _ZN6glitch5video15CGLSLShaderCodeC1EPNS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEE
// 0x6df6e4 _ZN6glitch5video15CGLSLShaderCodeC2EPNS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEE
// Partial: the driver is stored; the two fields after it are not identified and start at zero.
CGLSLShaderCode::CGLSLShaderCode(CProgrammableGLDriver* driver)
    : IShaderCode(""), m_driver(driver) {}

// 0x6e207c _ZN6glitch5video11IShaderCodeD1Ev
// 0x6e20e4 _ZN6glitch5video11IShaderCodeD2Ev
// The string member releases its heap buffer (the original calls GlitchFree
// when the buffer is not the inline one); the vtable store is implicit.

uint16_t CShaderCodeManager::getId(const char* name) const {
    auto it = m_ids.find(name);
    return it == m_ids.end() ? kInvalidId : it->second;
}

void CShaderCodeManager::insert(const char* name, const boost::intrusive_ptr<IShaderCode>& code) {
    m_ids[name] = static_cast<uint16_t>(m_codes.size());
    m_codes.push_back(code);
}

boost::intrusive_ptr<IShaderCode> CShaderCodeManager::at(uint16_t id) const {
    if (id < m_codes.size()) return m_codes[id];
    return boost::intrusive_ptr<IShaderCode>();
}

void CShaderCodeManager::removeAll() {
    m_ids.clear();
    m_codes.clear();
}

// 0x6e1b08 _ZNK6glitch5video18ICodeShaderManager13getShaderCodeEPKc
// Returns a null pointer when the name is unknown, as the original does.
boost::intrusive_ptr<IShaderCode> ICodeShaderManager::getShaderCode(const char* name) const {
    uint16_t id = m_shaderCodes.getId(name);
    if (id == CShaderCodeManager::kInvalidId) return boost::intrusive_ptr<IShaderCode>();
    return m_shaderCodes.at(id);
}

// 0x6e1d70 _ZN6glitch5video18ICodeShaderManager13addShaderCodeERKN5boost13intrusive_ptrINS0_11IShaderCodeEEE
void ICodeShaderManager::addShaderCode(const boost::intrusive_ptr<IShaderCode>& code) {
    m_shaderCodes.insert(code->m_name.c_str(), code);
}

// 0x6e2028 _ZN6glitch5video18ICodeShaderManager23removeUnusedShaderCodesEv
void ICodeShaderManager::removeUnusedShaderCodes() {
    m_shaderCodes.removeAll();
}

// 0x6e2034 _ZN6glitch5video18ICodeShaderManager19removeUnusedShadersEv
// The original calls the IShaderManager base version first (not modelled here).
void ICodeShaderManager::removeUnusedShaders() {
    removeUnusedShaderCodes();
}

// 0x6e0a3c _ZN6glitch5video18ICodeShaderManager20initAdditionalConfigEPKc
// Partial: the file read through the driver (vtable slots 12 and 32 of the file object) is not
// ported. The "not found" warning is not logged. The loaded text would replace '^' by newline.
void ICodeShaderManager::initAdditionalConfig(const char* /*name*/) {
    if (m_additionalConfigLength != -1) {
        return;
    }
}

// 0x6e0b64 _ZN6glitch5video18ICodeShaderManager18makeShaderCodeNameEPKcjS3_jS3_jPj
// Partial: the original returns a process buffer; the buffer's heap-excess flag is not modelled.
std::string ICodeShaderManager::makeShaderCodeName(const char* a, uint32_t aLength, const char* b,
                                                   uint32_t bLength, const char* c, uint32_t cLength,
                                                   uint32_t* outLength) const {
    std::string result;
    result.reserve(aLength + bLength + cLength +
                   (m_hasAdditionalConfig ? m_additionalConfig.size() : 0));
    result.append(a, aLength);
    result.append(b, bLength);
    result.append(c, cLength);
    if (m_hasAdditionalConfig) {
        result.append(m_additionalConfig);
    }
    if (outLength) {
        *outLength = static_cast<uint32_t>(result.size());
    }
    return result;
}

// 0x6e0c10 _ZN6glitch5video18ICodeShaderManager18makeShaderCodeNameEPKcS3_S3_Pj
// Partial: see the overload above.
std::string ICodeShaderManager::makeShaderCodeName(const char* a, const char* b, const char* c,
                                                   uint32_t* outLength) const {
    const char* bText = b ? b : "";
    const char* cText = c ? c : "";
    return makeShaderCodeName(a, static_cast<uint32_t>(std::char_traits<char>::length(a)), bText,
                              static_cast<uint32_t>(std::char_traits<char>::length(bText)), cText,
                              static_cast<uint32_t>(std::char_traits<char>::length(cText)), outLength);
}

// 0x6e0ca4 _ZN6glitch5video18ICodeShaderManagerC1Ev
// 0x6e0cf0 _ZN6glitch5video18ICodeShaderManagerC2Ev
// 0x6e176c _ZN6glitch5video18ICodeShaderManagerD1Ev
// 0x6e17d8 _ZN6glitch5video18ICodeShaderManagerD2Ev
// Base IShaderManager ctor and dtor are not modelled.

}  // namespace glitch::video
