// glitch::video::CCommonGLDriver: render-state setters of the GL driver. The
// original is the template instance
//   CCommonGLDriver<CProgrammableGLDriver<CGLSLShaderHandler>,
//                   detail::CProgrammableGLFunctionPointerSet>
// Only this instance is modelled, so the template is not reproduced. State
// members are inherited from CCommonGLDriverBase, which keeps the original
// offsets. Each setter follows the original pattern: compare with the cached
// value, call flush() (vtable +0x1fc in the original), call GL, then store.
#pragma once

#include <cstddef>
#include <cstdint>
#include <map>

#include "glitch-video/ccommon_gl_driver_base.hpp"
#include "glitch-video/deps/glitch_core.hpp"
#include "glitch-video/deps/glitch_video_types.hpp"
#include "glitch-video/deps/intrusive_ptr.hpp"
#include "glitch-video/tb/renderpass_srenderstate.hpp"

namespace glitch::video {

class CCommonGLDriver : public CCommonGLDriverBase {
public:
    // The GL buffer: CCommonGLDriver<...>::CBuffer in the original (vtable +0x0c .. +0x20 implemented with glBindBuffer,
    // glMapBufferRange and friends). Partial: the GL calls are not reconstructed, so the slots keep the IBuffer state and
    // do no GL work; mapImpl returns null.
    class CBuffer : public CBufferBase {
    public:
        using CBufferBase::CBufferBase;
        void bindImpl(bool) override;                              // +0x0c  0x5b051c family
        void unbindImpl() override;                                // +0x10
        uint8_t* mapImpl(uint32_t) const override;                 // +0x14  0x5b3a1c family
        void unmapImpl() const override;                           // +0x18
        boost::intrusive_ptr<IBuffer> cloneImpl() const override;  // +0x20  0x5b1480
    };

    virtual ~CCommonGLDriver() = default;

    // vtable +0x1fc (IVideoDriver::flush in the original). The port has no
    // command queue, so the default does nothing.
    virtual void flush();

    // vtable +0x218 in the original; the slot is not identified. Default does nothing.
    virtual void presentHook();

    // vtable +0x6c in the original; not identified. Default does nothing.
    virtual void stateHook();

    // Capability setters (glEnable / glDisable on the cap in the comment).
    void setBlendEnable(bool enable);                    // 0x5b7080, GL_BLEND 0x0be2
    void setCullFaceEnable(bool enable);                 // 0x5b7034, GL_CULL_FACE 0x0b44
    void setDepthTestEnable(bool enable);                // 0x5b6fe8, GL_DEPTH_TEST 0x0b71
    void setDitherEnable(bool enable);                   // 0x5b6f9c, GL_DITHER 0x0bd0
    void setPolygonOffsetFillEnable(bool enable);        // 0x5b6f50, GL_POLYGON_OFFSET_FILL 0x8037
    void setSampleAlphaToCoverageEnable(bool enable);    // 0x5b6f04, GL_SAMPLE_ALPHA_TO_COVERAGE 0x809e
    void setSampleCoverageEnable(bool enable);           // 0x5b6eb8, GL_SAMPLE_COVERAGE 0x80a0
    void setScissorEnable(bool enable);                  // 0x5b6e6c, GL_SCISSOR_TEST 0x0c11
    void setStencilTestEnable(bool enable);              // 0x5b6e20, GL_STENCIL_TEST 0x0b90

    // The two polygon-offset line/point enables are empty in the original.
    void setPolygonOffsetLineEnable(bool enable);        // 0x5af2b0
    void setPolygonOffsetPointEnable(bool enable);       // 0x5af2b4

    void setDepthMask(bool flag);                        // 0x5afccc
    void setLineWidth(float width);                      // 0x5afd00
    void setPointSize(float size);                       // 0x5af220, no GL call
    void setClearDepth(float depth);                     // 0x5b0e94, no store (see notes)
    void setDepthRange(float nearValue, float farValue); // 0x5b0ed4
    void setPolygonOffset(float factor, float units);    // 0x5af828
    void setSampleCoverageValue(float value);            // 0x5af8f8
    void setSampleCoverageInvert(bool invert);           // 0x5af93c
    void setStencilMask(uint8_t mask);                   // 0x5b0e2c
    void setClearStencil(uint8_t value);                 // 0x5b0e60
    void setColorMask(bool r, bool g, bool b, bool a);   // 0x5b0cec
    void setPolygonModeFront(E_POLYGON_MODE mode);       // 0x5af258, no GL call
    void setPolygonModeBack(E_POLYGON_MODE mode);        // 0x5af284, no GL call

    // 0x5af1fc: the original returns 0xffff.
    uint32_t getMaximalPrimitiveCount() const;
    // 0x5af204: no capture on this back end; returns false.
    bool captureFramebufferImpl(const boost::intrusive_ptr<ITexture>& texture, const core::position2d<int32_t>& pos,
                                const core::rect<int32_t>& area, uint8_t mip, E_TEXTURE_CUBE_MAP_FACE face,
                                bool flag);
    // 0x5af20c: calls the presentation hook (vtable +0x218).
    void swapBuffers(int32_t);

    // Partial: the GL enum maps (BlendFactorMap, BlendEquationMap, FaceCullingMap,
    // FaceWindingMap, CompareFuncMap, StencilOpMap) are .rodata tables that are not
    // in the repo. These setters update the cache exactly and skip the GL call.
    void setBlendFunc(E_BLEND_FACTOR src, E_BLEND_FACTOR dst);  // 0x5af35c
    void setBlendEquation(E_BLEND_EQUATION mode);               // 0x5af3b4
    void setCullFace(E_FACE_SIDE side);                         // 0x5af6b8
    void setFrontFace(E_FACE_WINDING winding);                  // 0x5afc78
    void setDepthFunc(E_COMPARE_FUNC func);                     // 0x5af760
    void setStencilFunc(E_COMPARE_FUNC func);                   // 0x5afa24
    void setStencilFuncRef(uint8_t ref);                        // 0x5af9cc
    void setStencilFuncMask(uint8_t mask);                      // 0x5af974
    void setStencilOpFail(E_STENCIL_OP op);                     // 0x5afc14
    void setStencilOpZFail(E_STENCIL_OP op);                    // 0x5afbb0
    void setStencilOpZPass(E_STENCIL_OP op);                    // 0x5afb4c

    // Partial: the colour conversion is read from the ARM listing (x / 255 per
    // byte); the cache store after the GL call is not verified.
    void setBlendColor(SColor color);                           // 0x5af574
    void setClearColor(SColor color);                           // 0x5b0d4c

    // 0x5b1860: forwards to IVideoDriver::beginScene and returns true.
    bool beginScene();
    // 0x5b1838: flush, glFlush, then IVideoDriver::endScene. The listing passes no
    // time argument, so the port passes 0 (partial, see ledger).
    bool endScene();
    // 0x5b13f4: returns a new CBufferBase (intrusive reference taken by the wrapper).
    // Partial: the original stores the result through a hidden return slot.
    boost::intrusive_ptr<IBuffer> createBuffer(E_BUFFER_TYPE type, E_BUFFER_USAGE usage, uint32_t size,
                                               void* data, bool flag);

    // Buffer map keyed by the 32-bit buffer name (red-black tree at +0x800 in the original).
    // 0x5b5be4: copies byteCount bytes of data and inserts the entry (insert_unique: an
    // existing key is kept and its copy is left allocated, as in the original).
    void registerBufferMap(uint32_t key, uint32_t arg1, uint32_t arg2, uint32_t arg3, size_t byteCount,
                           const void* data);
    // 0x5b1f5c: frees the copy of the entry with this key and erases it.
    void unregisterBufferMap(uint32_t key);
    // 0x5b35d8: frees every copy and empties the map.
    void clearBufferMap();

    // Render-state apply paths. In the original these are members of the template
    // instance, applied to detail::renderpass::SRenderState only, so they are declared
    // here for that one argument type.
    // 0x5af3f8 applyRenderStateBlend<SRenderState>: blend enable, equation, factors, colour.
    void applyRenderStateBlend(const detail::renderpass::SRenderState& state);
    // 0x5af6fc applyRenderStateDepthTest<SRenderState>: depth test enable and depth function.
    void applyRenderStateDepthTest(const detail::renderpass::SRenderState& state);

    // Test helper, not in the original: exposes the state for checks.
    uint32_t getColorMaskBits() const { return m_colorMask; }
    int32_t getScissorFlag() const { return m_scissorFlag; }
    size_t getBufferMapSize() const { return m_bufferMap.size(); }

protected:
    // Shared body of the capability setters: flush, enable or disable, store.
    void setCapEnable(bool& state, bool enable, uint32_t cap);

    bool m_frontFaceFlip = false;  // +0x4a0, byte at word index 0x128: inverts the winding map
    int32_t m_scissorFlag = 0;     // +0x23c, cached with the scissor rectangle
    // BufferInfo entries; field names are not identified (arg1..arg3 are the three
    // uint32 arguments of registerBufferMap; the key is not stored again).
    struct BufferInfo {
        uint32_t arg1 = 0;     // +0x14 in the node
        uint32_t arg2 = 0;     // +0x18
        uint32_t arg3 = 0;     // +0x1c
        size_t byteCount = 0;  // +0x20
        void* data = nullptr;  // +0x24, malloc copy, freed on unregister and clear
    };
    std::map<uint32_t, BufferInfo> m_bufferMap;  // +0x800 in the original
};

}  // namespace glitch::video
