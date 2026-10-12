// Small GL driver pieces: the GLSL shader handler and the GLES2 driver's
// constant hooks. The full driver classes are not reconstructed yet.
#pragma once

#include <cstdint>

#include "glitch-video/deps/glitch_video_types.hpp"

namespace glitch::video {

class IShaderManager;

class CGLSLShaderHandler {
public:
    CGLSLShaderHandler();                                    // 0x6df9b8 / 0x6df9c4
    bool initShaderHandler(IShaderManager* manager);          // 0x6df9d0

private:
    uint32_t m_unknown00 = 0;  // +0x00, cleared by the constructor; purpose not identified
};

class COpenGLES2Driver {
public:
    E_DRIVER_TYPE getDriverType() const;     // 0x5aefa4
    bool swapBuffersImpl(int32_t);           // 0x5aefac
};

}  // namespace glitch::video
