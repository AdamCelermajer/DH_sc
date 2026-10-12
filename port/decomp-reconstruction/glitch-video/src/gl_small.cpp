#include "glitch-video/gl_small.hpp"

namespace glitch::video {

// 0x6df9b8 _ZN6glitch5video18CGLSLShaderHandlerC2Ev
// 0x6df9c4 _ZN6glitch5video18CGLSLShaderHandlerC1Ev
CGLSLShaderHandler::CGLSLShaderHandler() = default;

// 0x6df9d0 _ZN6glitch5video18CGLSLShaderHandler17initShaderHandlerEPNS0_14IShaderManagerE
bool CGLSLShaderHandler::initShaderHandler(IShaderManager*) { return true; }

// 0x5aefa4 _ZNK6glitch5video16COpenGLES2Driver13getDriverTypeEv
E_DRIVER_TYPE COpenGLES2Driver::getDriverType() const { return 8; }

// 0x5aefac _ZN6glitch5video16COpenGLES2Driver15swapBuffersImplEi
bool COpenGLES2Driver::swapBuffersImpl(int32_t) { return false; }

}  // namespace glitch::video
