// Small GL driver pieces: the GLSL shader handler and the GLES2 driver's
// constant hooks. The full driver classes are not reconstructed yet.
#pragma once

#include <cstdint>

#include "glitch-video/deps/glitch_video_types.hpp"
#include "glitch-video/shader_attributes.hpp"
#include "glitch-video/shader_code.hpp"

namespace glitch::io {
class IAttributes;
}  // namespace glitch::io

namespace glitch::video {

class IShaderManager;

class CGLSLShaderHandler {
public:
    CGLSLShaderHandler();                                    // 0x6df9b8 / 0x6df9c4
    bool initShaderHandler(IShaderManager* manager);          // 0x6df9d0

private:
    uint32_t m_unknown00 = 0;  // +0x00, cleared by the constructor; purpose not identified
};

// GL program wrapper. The original derives from IShader; the GL program name is
// IShader::m_glProgram (+0x4c).
class CProgrammableGLDriver;

class CGLSLShader : public IShader {
public:
    CGLSLShader() = default;
    // 0x6df0d4 (C1) / 0x6df130 (C2). Partial: the IShader base constructor (driver, empty name,
    // id 0) is not ported, so the driver is not stored. The words at +0x44, +0x48 and the byte
    // at +0x50 are zeroed here; +0x4c is IShader::m_glProgram and starts at zero.
    explicit CGLSLShader(CProgrammableGLDriver* driver);
    void serializeAttributes(io::IAttributes* attributes) const;  // 0x6de878, empty
    void deserializeAttributes(io::IAttributes* attributes);      // 0x6de87c, empty
    bool createProgram();                                        // 0x6de8e0: glCreateProgram into +0x4c; returns true

private:
    uint32_t m_unknown44 = 0;  // +0x44, not identified
    uint32_t m_unknown48 = 0;  // +0x48, not identified
    uint8_t m_unknown50 = 0;   // +0x50, not identified
};

class COpenGLES2Driver {
public:
    E_DRIVER_TYPE getDriverType() const;     // 0x5aefa4
    bool swapBuffersImpl(int32_t);           // 0x5aefac
};

}  // namespace glitch::video
