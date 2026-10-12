#include "glitch-video/video_driver.hpp"

namespace glitch::video {

// 0x5aab18 _ZNK6glitch5video12IVideoDriver6getFPSEv
uint32_t IVideoDriver::getFPS() const { return m_fpsCounter.getFPS(); }

// 0x5aaae8 _ZNK6glitch5video12IVideoDriver16getDrawCallCountEv
uint32_t IVideoDriver::getDrawCallCount() const { return m_fpsCounter.getDrawCalls(); }

// 0x5aaae0 _ZNK6glitch5video12IVideoDriver18getDrawCall2DCountEv
uint32_t IVideoDriver::getDrawCall2DCount() const { return m_fpsCounter.getDrawCalls2D(); }

// 0x5aaad8 _ZNK6glitch5video12IVideoDriver22getTextureBindingCountEv
uint32_t IVideoDriver::getTextureBindingCount() const { return m_fpsCounter.getTextureBindings(); }

// 0x5a8cc8 _ZN6glitch5video12IVideoDriver16checkDriverResetEv
bool IVideoDriver::checkDriverReset() { return false; }

// 0x5a90f4 _ZNK6glitch5video12IVideoDriver24getMaximalPrimitiveCountEv
uint32_t IVideoDriver::getMaximalPrimitiveCount() const { return 0xffffffffu; }

// 0x5a909c _ZN6glitch5video12IVideoDriver17setMaxTextureSizeERKNS_4core11dimension2dIiEE
void IVideoDriver::setMaxTextureSize(const core::dimension2d<int32_t>& size) {
    m_maxTextureSize = size;
}

// 0x5a8b90 _ZN6glitch5video12IVideoDriver17onShaderDestroyedEPNS0_7IShaderE
void IVideoDriver::onShaderDestroyed(IShader* shader) {
    if (m_shaderF4 == shader) {
        m_shaderF4 = nullptr;
    }
}

}  // namespace glitch::video
