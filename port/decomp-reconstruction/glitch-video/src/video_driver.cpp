#include "glitch-video/video_driver.hpp"

#include "glitch-video/deps/os_logging.hpp"
#include "glitch-video/shader_manager.hpp"
#include "glitch-video/unreconstructed.hpp"

namespace glitch::video {

// 0x5aaf1c _ZN6glitch5video12IVideoDriverC2EPNS_7IDeviceEPNS0_14IShaderManagerEPNS0_22CMaterialRendererManagerEPNS0_15CTextureManagerEPNS0_31CGlobalMaterialParameterManagerERKN5boost13intrusive_ptrINS0_6CLightEEE
// 0x5ab2f8 _ZN6glitch5video12IVideoDriverC1EPNS_7IDeviceEPNS0_14IShaderManagerEPNS0_22CMaterialRendererManagerEPNS0_15CTextureManagerEPNS0_31CGlobalMaterialParameterManagerERKN5boost13intrusive_ptrINS0_6CLightEEE
// Partial: the original creates the material renderer manager (0x98 bytes), the texture manager (0x78) and the global
// parameter manager (0x3c) when the argument is null and records that in +0x138 (0x10 / 0x20), and it looks the
// DynamicLight0, FogColor<n> and SceneAmbientLight ids up in the global parameter manager; those managers are not
// constructible here, so the pointers are kept as given and the ids stay 0xffff.
IVideoDriver::IVideoDriver(glitch::IDevice* device, IShaderManager* shaderManager,
                           CMaterialRendererManager* rendererManager, CTextureManager* textureManager,
                           CGlobalMaterialParameterManager* globalParameters,
                           const boost::intrusive_ptr<CLight>& defaultLight)
    : m_defaultDynamicLight(defaultLight),
      m_device(device),
      m_shaderManager(shaderManager),
      m_materialRendererManager(rendererManager),
      m_textureManager(textureManager),
      m_globalParameters(globalParameters) {
    referenceCounter = 1;  // +0x04
    if (m_shaderManager != nullptr) {
        m_shaderManager->init(this, true);  // vtable +0x08 of the shader manager
    }
    if (!m_defaultDynamicLight) {
        m_defaultDynamicLight = CLight::allocate(nullptr);
        resetDefaultDynamicLight();
    }
    m_rendererManagerCopy = m_materialRendererManager;  // +0x104
    m_techniqueRemap = nullptr;                          // +0x108
}

// 0x5ad5b4 _ZN6glitch5video12IVideoDriverD2Ev / 0x5ae2dc D1 / 0x5ae5d4 D0
// Partial: the original deletes the material renderer manager, the texture manager and the global parameter manager when
// +0x138 says it created them. The members (intrusive pointers and vectors) release themselves.
IVideoDriver::~IVideoDriver() {
    delete m_shaderManager;  // vtable +0x04 of the shader manager (deleting destructor)
    m_shaderManager = nullptr;
}

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

// 0x5aa8a0 _ZNK6glitch5video12IVideoDriver19checkPrimitiveCountEj
// Partial: the os::Printer log of the failure is not written (the original formats "Could not draw triangles, too many
// primitives(%u), maxium is %u." into a 1 KiB buffer); the result is kept.
bool IVideoDriver::checkPrimitiveCount(uint32_t count) const {
    return getMaximalPrimitiveCount() >= count;
}

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
// The original reads os::Timer::getRealTime() and registers the frame with the FPS counter.
bool IVideoDriver::endScene() { return endScene(os::Timer::getRealTime()); }

// Port helper: the same registration with an explicit time stamp (os::Timer::getRealTime in the original).
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
        flush();  // vtable +0x1fc
    }
}

// 0x5aab60 _ZN6glitch5video12IVideoDriver4initEtth
// Partial: see the header.
void IVideoDriver::init(uint16_t unk3e, uint16_t lightLimit, uint8_t flag) {
    unk_135 = flag;
    unk_3e = unk3e;
    m_dynamicLightLimit = lightLimit;
}

// 0x5a8ae8 _ZN6glitch5video12IVideoDriver23resetDefaultDynamicLightEv
void IVideoDriver::resetDefaultDynamicLight() {
    CLight* light = m_defaultDynamicLight.get();
    light->m_ambientColor = SColorf{0.0f, 0.0f, 0.0f, 0.0f};
    light->m_diffuseColor = SColorf{0.0f, 0.0f, 0.0f, 0.0f};
    light->m_specularColor = SColorf{0.0f, 0.0f, 0.0f, 0.0f};
    light->m_attenuation.X = 1.0f;
    light->m_attenuation.Y = 0.0f;
    light->m_attenuation.Z = 0.0f;
    light->m_radius = 0.0f;
    light->m_innerConeAngle = 0.0f;
    light->m_outerConeAngle = 0.0f;
    light->m_falloff = 0.0f;
    light->m_type = static_cast<uint16_t>(kLightTypeDefaultDynamic);
    light->m_castShadows = 0;
}

// addDynamicLight (0x5aaa40) is defined in tb_video-glitch_video_IVideoDriver-0.cpp. deleteAllDynamicLights (0x5aaa84) and
// getDynamicLight (0x5aa940) are declared only: they read and write the light slots of the global parameter table through
// the element setter of IMaterialParameters<CGlobalMaterialParameterManager> for intrusive_ptr<CLight> (0x5be68c).

// 0x5aa2e0 _ZN6glitch5video12IVideoDriver13createBindingEv
CDriverBinding* IVideoDriver::createBinding() { return new CDriverBinding(this); }

// 0x5a95f8 _ZN6glitch5video12IVideoDriver13ensureBindingEPPNS0_14CDriverBindingE
CDriverBinding* IVideoDriver::ensureBinding(CDriverBinding** binding) {
    if (*binding == nullptr) {
        *binding = createBinding();  // vtable +0x1f8
    }
    return *binding;
}

// 0x5a97a8 _ZN6glitch5video12IVideoDriver18clearRenderBuffersEv
void IVideoDriver::clearRenderBuffers() {
    for (IRenderBuffer* buffer : m_renderBuffers) {
        buffer->unbind();  // vtable +0x0c
    }
}

// 0x5ab6d4 _ZN6glitch5video12IVideoDriver16pushRenderTargetERKN5boost13intrusive_ptrINS0_13IRenderTargetEEE
void IVideoDriver::pushRenderTarget(const boost::intrusive_ptr<IRenderTarget>& target) {
    m_stateFlags |= 4u;
    m_renderTargets.push_back(target);
    target->bind();  // vtable +0x10
    m_stateFlags &= ~4u;
}

// 0x5a953c _ZN6glitch5video12IVideoDriver15popRenderTargetEv
// Returns the top target. With two or more targets it unbinds the top one (vtable +0x14), pops it and binds the one under
// it (vtable +0x10); with a single target that target is returned and the stack stays as it is; with none a null pointer
// is returned.
boost::intrusive_ptr<IRenderTarget> IVideoDriver::popRenderTarget() {
    if (m_renderTargets.empty()) {
        return boost::intrusive_ptr<IRenderTarget>();
    }
    boost::intrusive_ptr<IRenderTarget> top = m_renderTargets.back();
    if (m_renderTargets.size() > 1) {
        top->unbind();                // vtable +0x14
        m_renderTargets.pop_back();
        m_renderTargets.back()->bind();  // vtable +0x10 of the target underneath
    }
    return top;
}

// 0x5a8bdc _ZN6glitch5video12IVideoDriver14draw3DTriangleERKNS_4core9triangle3dIfEENS0_6SColorE
// The three edges as lines: draw3DLines (vtable +0x20) with the index pairs (0, 1), (1, 2), (2, 0).
void IVideoDriver::draw3DTriangle(const core::triangle3d<float>& triangle, SColor color) {
    core::vector3d<float> positions[3] = {triangle.A, triangle.B, triangle.C};
    uint16_t indices[6] = {0, 1, 1, 2, 2, 0};
    SColor colors[3] = {color, color, color};
    draw3DLines(positions, indices, colors, 3, 3);
}

// 0x5a9990 _ZN6glitch5video12IVideoDriver9draw3DBoxERKNS_4core9aabbox3dIfEENS0_6SColorE
// The twelve edges of the box as lines: draw3DLines (vtable +0x20) with the eight corners from aabbox3d::getEdges.
void IVideoDriver::draw3DBox(const core::aabbox3d<float>& box, SColor color) {
    core::vector3d<float> corners[8];
    box.getEdges(corners);
    static const uint16_t kEdges[24] = {5, 1, 1, 3, 3, 7, 7, 5, 0, 2, 2, 6, 6, 4, 4, 0, 1, 0, 3, 2, 7, 6, 5, 4};
    uint16_t indices[24];
    for (int i = 0; i < 24; ++i) indices[i] = kEdges[i];
    SColor colors[8] = {color, color, color, color, color, color, color, color};
    draw3DLines(corners, indices, colors, 8, 12);
}

// The slots below are declared in the vtable but their bodies are not reconstructed: they move vertices through the
// immediate-mode streams and buffers (+0xa4..+0xb8), the process-buffer heap, the batch buffers and the managers,
// whose classes (CAppendMeshBuffer, the material renderer manager, the global parameter manager, CBatchDriver's
// SIDedCollection users) are not in the port. See the ledger rows of the addresses.
void IVideoDriver::draw3DLine(const core::vector3d<float>&, const core::vector3d<float>&, SColor) {
    detail::unreconstructed("IVideoDriver::draw3DLine", 0x5ac430);
}
void IVideoDriver::draw3DLines(core::vector3d<float>*, uint16_t*, SColor*, uint32_t, uint32_t) {
    detail::unreconstructed("IVideoDriver::draw3DLines", 0x5ac2a8);
}
void IVideoDriver::draw2DLine(const core::position2d<int32_t>&, const core::position2d<int32_t>&, SColor) {
    detail::unreconstructed("IVideoDriver::draw2DLine", 0x5ac7bc);
}
void IVideoDriver::draw2DLines(core::position2d<int32_t>*, uint16_t*, SColor*, uint32_t, uint32_t) {
    detail::unreconstructed("IVideoDriver::draw2DLines", 0x5ac5b8);
}
void IVideoDriver::draw(const boost::intrusive_ptr<const CVertexStreams>&, const CPrimitiveStream&, CDriverBinding**,
                        const boost::intrusive_ptr<const scene::CMeshBuffer>&) {
    detail::unreconstructed("IVideoDriver::draw", 0x5adfa8);
}
void IVideoDriver::onResize(const core::dimension2d<int32_t>&) {
    detail::unreconstructed("IVideoDriver::onResize", 0x5a8cd0);
}
void IVideoDriver::removeUnused() { detail::unreconstructed("IVideoDriver::removeUnused", 0x5aa290); }
E_DRIVER_ALLOCATION_RESULT IVideoDriver::getProcessBuffer(E_PROCESS_BUFFER_TYPE, uint32_t, uint32_t, uint32_t,
                                                          const boost::intrusive_ptr<CVertexStreams>&,
                                                          CDriverBinding**, bool) {
    detail::unreconstructed("IVideoDriver::getProcessBuffer", 0x5acc34);
}
void IVideoDriver::releaseProcessBuffer(E_PROCESS_BUFFER_TYPE, const boost::intrusive_ptr<CVertexStreams>&, uint32_t,
                                        uint32_t, CDriverBinding*) {
    detail::unreconstructed("IVideoDriver::releaseProcessBuffer", 0x5ac158);
}
// 0x5adb9c _ZN6glitch5video12IVideoDriver5flushEv: a 4-byte branch to drawPendingBatch (0x5ad8ac).
void IVideoDriver::flush() { drawPendingBatch(); }

// 0x5ad8ac _ZN6glitch5video12IVideoDriver16drawPendingBatchEv
// Partial: with no batch buffer the original returns 1 straight away; with one it unmaps the batch buffer's streams, sets
// it up (CAppendMeshBuffer::setup) and draws it through the +0x200 slot, which needs the scene's CAppendMeshBuffer.
bool IVideoDriver::drawPendingBatch() {
    if (!m_batchBuffer) {
        return true;
    }
    detail::unreconstructed("IVideoDriver::drawPendingBatch", 0x5ad8ac);
}

}  // namespace glitch::video
