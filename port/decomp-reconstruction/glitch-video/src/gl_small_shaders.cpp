// CGLSLShader and CGLSLShaderCode members that the reconstructed driver uses.
#include "glitch-video/gl_small.hpp"

#include "glitch-video/deps/gl_function_table.hpp"

namespace glitch::video {

// 0x6de878 _ZNK6glitch5video11CGLSLShader19serializeAttributesEPNS_2io11IAttributesE
void CGLSLShader::serializeAttributes(io::IAttributes*) const {}

// 0x6de87c _ZN6glitch5video11CGLSLShader21deserializeAttributesEPNS_2io11IAttributesE
void CGLSLShader::deserializeAttributes(io::IAttributes*) {}

// 0x6de8e0 _ZN6glitch5video11CGLSLShader13createProgramEv
bool CGLSLShader::createProgram() {
    auto& t = deps::activeGLFunctionTable();
    m_glProgram = t.createProgram ? t.createProgram() : 0;
    return true;
}

}  // namespace glitch::video
