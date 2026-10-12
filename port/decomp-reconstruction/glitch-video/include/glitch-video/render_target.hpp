// glitch::video::IRenderTarget: the size and viewport state of a render target. Only the
// fields that setViewport reads are named. The vtable and the reference count at
// +0x00..+0x07 are not modelled here.
#pragma once

#include <cstdint>

#include "glitch-video/deps/glitch_core.hpp"

namespace glitch::video {

class IVideoDriver;

class IRenderTarget {
public:
    // 0x6dc5a8 setViewport(rect<int> const&). Stores the rectangle, clamps the far corner to the
    // target size and the near corner to the rectangle. PARTIAL: the final step, which hands the
    // rectangle to the driver when this target is the driver's current one, is not written.
    void setViewport(const core::rect<int32_t>& rect);

    const core::rect<int32_t>& viewport() const { return m_viewport; }

    // Test helper: the target size. How the original sets it is not recovered.
    void setSizeForTest(int32_t width, int32_t height) {
        m_width = width;
        m_height = height;
    }

private:
    IVideoDriver* m_driver = nullptr;   // +0x08, owning driver (set by the creation path, not recovered)
    int32_t m_width = 0;                // +0x0c, target width
    int32_t m_height = 0;               // +0x10, target height
    core::rect<int32_t> m_viewport{};   // +0x14..+0x23: upper-left X, Y then lower-right X, Y
};

}  // namespace glitch::video
