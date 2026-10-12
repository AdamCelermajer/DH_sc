#include "glitch-video/material.hpp"

namespace glitch::video {

// 0x5c5d7c _ZN6glitch5video9CMaterial23getRenderStateBlockSizeERKNS0_17CMaterialRendererE
uint32_t CMaterial::getRenderStateBlockSize(const CMaterialRenderer&) {
    return 0;
}

// 0x5c5d84 _ZN6glitch5video9CMaterial22setRenderStateInternalEhhRKNS0_6detail8material12SRenderStateE
void CMaterial::setRenderStateInternal(uint8_t, uint8_t, const detail::material::SRenderState&) {}

}  // namespace glitch::video
