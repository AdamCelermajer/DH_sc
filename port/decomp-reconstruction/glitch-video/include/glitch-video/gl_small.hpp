// Small GL driver pieces: the GLSL shader handler and the GLES2 driver's
// constant hooks. The full driver classes are not reconstructed yet.
#pragma once

#include <cstdint>

#include "glitch-video/deps/intrusive_ptr.hpp"

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
    // id 0) is not ported, so the driver is not stored. +0x4c is IShader::m_glProgram.
    explicit CGLSLShader(CProgrammableGLDriver* driver);
    // 0x6df1f0 (C1) / 0x6df2c4 (C2). Partial: the IShader base constructor is not ported, so the
    // id and name are not stored. The codes are kept, the program is created, and the two
    // glAttachShader calls and the link step are not ported. The link result is ignored.
    CGLSLShader(uint16_t id, const char* name, const boost::intrusive_ptr<CGLSLShaderCode>& first,
                const boost::intrusive_ptr<CGLSLShaderCode>& second, CProgrammableGLDriver* driver,
                bool link);
    void serializeAttributes(io::IAttributes* attributes) const;  // 0x6de878, empty
    void deserializeAttributes(io::IAttributes* attributes);      // 0x6de87c, empty
    bool createProgram();                                        // 0x6de8e0: glCreateProgram into +0x4c; returns true
    // 0x6de880: drops both shader code references (+0x44 and +0x48). Reference counting is
    // not modelled, so the references are cleared.
    void releaseShaderCode();
    // 0x6de8f8: deletes the GL program (+0x4c) when set, then releaseShaderCode().
    void releaseDriverSpecificResources();

private:
    boost::intrusive_ptr<CGLSLShaderCode> m_firstCode;   // +0x44, first code argument
    boost::intrusive_ptr<CGLSLShaderCode> m_secondCode;  // +0x48, second code argument
    uint8_t m_unknown50 = 0;   // +0x50, not identified
};

class COpenGLES2Driver {
public:
    E_DRIVER_TYPE getDriverType() const;     // 0x5aefa4
    bool swapBuffersImpl(int32_t);           // 0x5aefac
};

}  // namespace glitch::video
