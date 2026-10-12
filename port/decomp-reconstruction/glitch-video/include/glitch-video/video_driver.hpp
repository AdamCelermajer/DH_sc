// glitch::video::IVideoDriver: the driver interface base. Only the members and
// methods reconstructed so far are declared. Base class and most of the layout
// are not modelled yet.
#pragma once

#include <cstdint>
#include <vector>

#include "glitch-video/deps/glitch_core.hpp"
#include "glitch-video/deps/glitch_video_types.hpp"
#include "glitch-video/fps_counter.hpp"

// CMaterial is declared in deps/glitch_video_types.hpp.

namespace glitch::video {

class IShader;

class IVideoDriver {
public:
    // Compile-time data record used by beginCompile. Layout not reconstructed.
    class ICompileData {
    public:
        virtual ~ICompileData() = default;  // 0x5a8ae0 / 0x5a8ae4 (empty bodies)
    };

    virtual ~IVideoDriver() = default;

    uint32_t getFPS() const;                    // 0x5aab18
    uint32_t getDrawCallCount() const;          // 0x5aaae8
    uint32_t getDrawCall2DCount() const;        // 0x5aaae0
    uint32_t getTextureBindingCount() const;    // 0x5aaad8
    virtual bool checkDriverReset();            // 0x5a8cc8
    uint32_t getMaximalPrimitiveCount() const;  // 0x5a90f4
    void setMaxTextureSize(const core::dimension2d<int32_t>& size);  // 0x5a909c
    void onShaderDestroyed(IShader* shader);    // 0x5a8b90
    bool beginScene();                          // 0x5a8b74
    // 0x5aab20: registers the frame with the FPS counter. The timer read is
    // passed in as now (os::Timer::getRealTime in the original). Partial: the
    // original's tail beyond registerFrame is not modelled.
    bool endScene(uint32_t now);
    // 0x5aaaf0: 0 = this frame, 1 = average, anything else = total.
    uint32_t getPrimitiveCountDrawn(uint32_t which) const;
    void onMaterialDestroyed(CMaterial* material);  // 0x5a8ba4
    void unregisterRenderBuffer(IRenderBuffer* buffer);  // 0x5b1338
    // 0x5a90b0: sets or clears option bits. Bit 0x100 calls a hook in the original
    // (vtable +0x1fc), which is not modelled here. Partial.
    void setOption(uint32_t option, bool enable);

    // Test helper, not in the original: exposes the option word (+0x88).
    uint32_t getOptionFlags() const { return m_options; }

private:
    core::dimension2d<int32_t> m_maxTextureSize{};  // +0x44
    CFPSCounter m_fpsCounter;                       // +0x50 (CFPSCounter is 0x28 bytes)
    uint32_t m_framePrimitives = 0;                 // +0x78, cleared by beginScene
    uint32_t m_frameDrawCalls = 0;                  // +0x7c
    uint32_t m_frameDrawCalls2D = 0;                // +0x80
    uint32_t m_frameTextureBindings = 0;            // +0x84
    uint32_t m_options = 0;                         // +0x88
    CMaterial* m_materialEc = nullptr;              // +0xec, last material compared on destroy
    void* m_stateE8 = nullptr;                      // +0xe8, cleared with m_materialEc; meaning not identified
    CMaterial* m_materialF0 = nullptr;              // +0xf0
    uint8_t m_flagF8 = 0;                           // +0xf8, set to 0xff when m_materialEc is cleared
    uint8_t m_flagF9 = 0;                           // +0xf9, set to 0xff when m_materialF0 is cleared
    std::vector<IRenderBuffer*> m_renderBuffers;    // +0x154..+0x158
    IShader* m_shaderF4 = nullptr;                  // +0xf4, cleared when that shader is destroyed
};

}  // namespace glitch::video
