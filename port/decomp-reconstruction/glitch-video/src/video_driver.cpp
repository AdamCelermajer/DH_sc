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

// 0x5a8b74 _ZN6glitch5video12IVideoDriver10beginSceneEv
bool IVideoDriver::beginScene() {
    m_framePrimitives = 0;
    m_frameDrawCalls = 0;
    m_frameDrawCalls2D = 0;
    m_frameTextureBindings = 0;
    return true;
}

// 0x5aab20 _ZN6glitch5video12IVideoDriver8endSceneEv
bool IVideoDriver::endScene(uint32_t now) {
    m_fpsCounter.registerFrame(now, m_framePrimitives, m_frameDrawCalls, m_frameDrawCalls2D,
                               m_frameTextureBindings);
    return true;
}

// 0x5aaaf0 _ZNK6glitch5video12IVideoDriver22getPrimitiveCountDrawnEj
uint32_t IVideoDriver::getPrimitiveCountDrawn(uint32_t which) const {
    if (which == 0) return m_fpsCounter.getPrimitive();
    if (which != 1) return m_fpsCounter.getPrimitiveTotal();
    return m_fpsCounter.getPrimitiveAverage();
}

// 0x5a8ba4 _ZN6glitch5video12IVideoDriver19onMaterialDestroyedEPNS0_9CMaterialE
void IVideoDriver::onMaterialDestroyed(CMaterial* material) {
    bool matchedEc = material == m_materialEc;
    if (matchedEc) {
        m_stateE8 = nullptr;
        m_materialEc = nullptr;
    }
    if (matchedEc) {
        m_flagF8 = 0xff;
    }
    if (material == m_materialF0) {
        m_flagF9 = 0xff;
        m_materialF0 = nullptr;
    }
}

// 0x5b1338 _ZN6glitch5video12IVideoDriver22unregisterRenderBufferEPNS0_13IRenderBufferE
void IVideoDriver::unregisterRenderBuffer(IRenderBuffer* buffer) {
    for (auto it = m_renderBuffers.begin(); it != m_renderBuffers.end(); ++it) {
        if (*it == buffer) {
            m_renderBuffers.erase(it);
            return;
        }
    }
}

// 0x5a90b0 _ZN6glitch5video12IVideoDriver9setOptionEjb
void IVideoDriver::setOption(uint32_t option, bool enable) {
    if (enable) {
        m_options |= option;
        return;
    }
    m_options &= ~option;
    if ((option & 0x100) != 0) {
        // Hook at vtable +0x1fc is not modelled.
    }
}

}  // namespace glitch::video
