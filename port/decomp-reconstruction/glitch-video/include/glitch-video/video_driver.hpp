// glitch::video::IVideoDriver: the driver interface base. Only the members and
// methods reconstructed so far are declared. Base class and most of the layout
// are not modelled yet.
#pragma once

#include <cstdint>

#include "glitch-video/deps/glitch_core.hpp"
#include "glitch-video/deps/glitch_video_types.hpp"
#include "glitch-video/fps_counter.hpp"

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

private:
    core::dimension2d<int32_t> m_maxTextureSize{};  // +0x44
    CFPSCounter m_fpsCounter;                       // +0x50 (CFPSCounter is 0x28 bytes)
    IShader* m_shaderF4 = nullptr;                  // +0xf4, cleared when that shader is destroyed
};

}  // namespace glitch::video
