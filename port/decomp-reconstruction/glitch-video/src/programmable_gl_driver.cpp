#include "glitch-video/programmable_gl_driver.hpp"

#include "glitch-video/deps/gl_function_table.hpp"

namespace glitch::video {

// 0x5af030 _ZN6glitch5video21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEE11onSet3DModeEv
void CProgrammableGLDriver::onSet3DMode() {}

// 0x5af034 _ZNK6glitch5video21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEE18queryMaxLightCountEv
uint32_t CProgrammableGLDriver::queryMaxLightCount() const { return 8; }

// 0x5af03c _ZNK6glitch5video21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEE20getMaxUserClipPlanesEv
uint32_t CProgrammableGLDriver::getMaxUserClipPlanes() const { return 6; }

// 0x5af044 _ZNK6glitch5video21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEE16getShaderManagerEv
IShaderManager* CProgrammableGLDriver::getShaderManager() const { return m_shaderManager; }

// 0x5af04c _ZNK6glitch5video21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEE13getDriverTypeEv
E_DRIVER_TYPE CProgrammableGLDriver::getDriverType() const { return 0x78; }

// 0x5af094 _ZN6glitch5video21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEE23setTextureLODBiasTexEnvEPKNS0_8ITextureE
void CProgrammableGLDriver::setTextureLODBiasTexEnv(const ITexture*) {}

// 0x5b1a84 _ZN6glitch5video21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEE12commitShaderEPKNS0_7IShaderE
void CProgrammableGLDriver::commitShader(const IShader* shader) {
    auto& t = deps::activeGLFunctionTable();
    if (t.useProgram) t.useProgram(shader->m_glProgram);
    m_currentShader = shader;
}

// 0x5b1aa4 _ZN6glitch5video21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEE17onShaderDestroyedEPNS0_7IShaderE
void CProgrammableGLDriver::onShaderDestroyed(IShader* shader) {
    if (m_currentShader != shader) return;
    m_currentShader = nullptr;
}

}  // namespace glitch::video
