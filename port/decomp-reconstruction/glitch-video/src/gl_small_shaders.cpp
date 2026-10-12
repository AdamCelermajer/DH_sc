// CGLSLShader and CGLSLShaderCode members that the reconstructed driver uses.
#include "glitch-video/gl_small.hpp"

#include "glitch-video/deps/gl_function_table.hpp"

namespace glitch::video {

// 0x6df0d4 _ZN6glitch5video11CGLSLShaderC1EPNS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEE
// 0x6df130 _ZN6glitch5video11CGLSLShaderC2EPNS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEE
// Partial: the IShader base constructor is not ported, so the driver argument is not stored.
CGLSLShader::CGLSLShader(CProgrammableGLDriver*) {}

// 0x6de878 _ZNK6glitch5video11CGLSLShader19serializeAttributesEPNS_2io11IAttributesE
void CGLSLShader::serializeAttributes(io::IAttributes*) const {}

// 0x6de87c _ZN6glitch5video11CGLSLShader21deserializeAttributesEPNS_2io11IAttributesE
void CGLSLShader::deserializeAttributes(io::IAttributes*) {}

// 0x6df1f0 _ZN6glitch5video11CGLSLShaderC1EtPKcRKN5boost13intrusive_ptrINS0_15CGLSLShaderCodeEEES9_PNS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEEb
// 0x6df2c4 _ZN6glitch5video11CGLSLShaderC2EtPKcRKN5boost13intrusive_ptrINS0_15CGLSLShaderCodeEEES9_PNS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEEb
// Partial: the IShader base constructor is not ported (id and name are dropped). glAttachShader
// x2 and the link step are not ported, so a failed link is not undone here.
CGLSLShader::CGLSLShader(uint16_t, const char*, const boost::intrusive_ptr<CGLSLShaderCode>& first,
                         const boost::intrusive_ptr<CGLSLShaderCode>& second, CProgrammableGLDriver* driver,
                         bool link)
    : m_firstCode(first), m_secondCode(second) {
    (void)driver;
    (void)link;
    m_unknown50 = 0;
    m_glProgram = 0;
    createProgram();
}

// 0x6de880 _ZN6glitch5video11CGLSLShader17releaseShaderCodeEv
void CGLSLShader::releaseShaderCode() {
    m_firstCode = boost::intrusive_ptr<CGLSLShaderCode>();
    m_secondCode = boost::intrusive_ptr<CGLSLShaderCode>();
}

// 0x6de8f8 _ZN6glitch5video11CGLSLShader30releaseDriverSpecificResourcesEv
void CGLSLShader::releaseDriverSpecificResources() {
    if (m_glProgram != 0) {
        auto& t = deps::activeGLFunctionTable();
        if (t.deleteProgram) t.deleteProgram(m_glProgram);
        m_glProgram = 0;
    }
    releaseShaderCode();
}

// 0x6de8e0 _ZN6glitch5video11CGLSLShader13createProgramEv
bool CGLSLShader::createProgram() {
    auto& t = deps::activeGLFunctionTable();
    m_glProgram = t.createProgram ? t.createProgram() : 0;
    return true;
}

}  // namespace glitch::video
