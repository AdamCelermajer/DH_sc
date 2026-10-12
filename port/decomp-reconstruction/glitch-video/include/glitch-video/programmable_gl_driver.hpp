// glitch::video::CProgrammableGLDriver: the programmable (GLSL) GL driver. The
// original is the template instance CProgrammableGLDriver<CGLSLShaderHandler>.
// Only the constant getters and the shader hooks are reconstructed here. The
// members keep their original offsets; the IVideoDriver base part below +0x284
// is not modelled.
#pragma once

#include <cstdint>

#include "glitch-video/ccommon_gl_driver.hpp"
#include "glitch-video/deps/glitch_video_types.hpp"
#include "glitch-video/shader_attributes.hpp"

namespace glitch::video {

class IShaderManager;

class CProgrammableGLDriver : public CCommonGLDriver {
public:
    // 0x5af030: empty in the original.
    void onSet3DMode();

    // 0x5af034: the original returns 8.
    uint32_t queryMaxLightCount() const;
    // 0x5af03c: the original returns 6.
    uint32_t getMaxUserClipPlanes() const;
    // 0x5af044: the shader manager pointer at +0xd8.
    IShaderManager* getShaderManager() const;
    // 0x5af04c: the original returns 0x78.
    E_DRIVER_TYPE getDriverType() const;

    // 0x5af094: empty in the original.
    void setTextureLODBiasTexEnv(const ITexture*);

    // 0x5b1a84: glUseProgram on the shader's GL program, then the current shader is recorded.
    void commitShader(const IShader* shader);
    // 0x5b1aa4: clears the current shader when it is the destroyed one. The original
    // tail-calls IVideoDriver::onShaderDestroyed, which clears the same slot.
    void onShaderDestroyed(IShader* shader);

    // Test helper, not in the original: whether a current shader is recorded.
    bool hasCurrentShader() const { return m_currentShader != nullptr; }

private:
    IShaderManager* m_shaderManager = nullptr;  // +0xd8
    const IShader* m_currentShader = nullptr;   // +0xf4, the IVideoDriver shader slot
};

}  // namespace glitch::video
