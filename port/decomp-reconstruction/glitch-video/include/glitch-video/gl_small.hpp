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

// GL program wrapper. Derives from IShader (0x44 bytes) and adds the two shader code references (+0x44, +0x48), the
// GL program name (+0x4c) and a byte (+0x50). Vtable 0x98b148: the IShader slots with +0x0c / +0x10 / +0x14 overridden.
class CProgrammableGLDriver;

class CGLSLShader : public IShader {
public:
    // Not part of the original: lets a test build a shader without a driver.
    CGLSLShader() : IShader(0, "", nullptr) {}
    // 0x6df0d4 (C1) / 0x6df130 (C2): IShader(0, "", driver), then +0x44 .. +0x50 cleared. Partial: CProgrammableGLDriver
    // is not derived from IVideoDriver in the port (see NOTES), so the base receives no driver.
    explicit CGLSLShader(CProgrammableGLDriver* driver);
    // 0x6df1f0 (C1) / 0x6df2c4 (C2). Partial: the base receives no driver (see above). The codes are kept, the program is
    // created, and the two glAttachShader calls and the link step are not ported. The link result is ignored.
    CGLSLShader(uint16_t id, const char* name, const boost::intrusive_ptr<CGLSLShaderCode>& first,
                const boost::intrusive_ptr<CGLSLShaderCode>& second, CProgrammableGLDriver* driver,
                bool link);
    void serializeAttributes(io::IAttributes* attributes) const override;  // +0x0c  0x6de878, empty
    void deserializeAttributes(io::IAttributes* attributes) override;      // +0x10  0x6de87c, empty
    bool createProgram();                                        // 0x6de8e0: glCreateProgram into +0x4c; returns true
    // 0x6de880: drops both shader code references (+0x44 and +0x48).
    void releaseShaderCode();
    // 0x6de8f8: deletes the GL program (+0x4c) when set, then releaseShaderCode().
    void releaseDriverSpecificResources() override;  // +0x14

    uint32_t m_glProgram = 0;  // +0x4c, the GL program name (set by createProgram)

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
