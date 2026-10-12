// glitch::video::CMaterial: material instance. Only the two render-state helpers
// are reconstructed here; the instance layout (heap block, parameter blocks,
// hash words) is not modelled yet.
#pragma once

#include <cstdint>

#include "glitch-video/deps/glitch_video_types.hpp"

namespace glitch::video {

namespace detail::material {
// Placeholder for the material render-state block. Layout not reconstructed.
struct SRenderState {};
}  // namespace detail::material

class CMaterial {
public:
    // 0x5c5d7c: the render-state block size for a renderer. The original returns 0.
    uint32_t getRenderStateBlockSize(const CMaterialRenderer&);
    // 0x5c5d84: no body in the original.
    void setRenderStateInternal(uint8_t, uint8_t, const detail::material::SRenderState&);
};

}  // namespace glitch::video
