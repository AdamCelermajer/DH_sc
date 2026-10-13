// glitch::video::CCommonGLDriverBase: GL driver state shared by the GLES1/GLES2
// back ends. Only the state getters are reconstructed here. Member names come
// from the accessor names; the offsets are the original ones.
#pragma once

#include <cstdint>

#include "glitch-video/buffer.hpp"
#include "glitch-video/deps/glitch_core.hpp"
#include "glitch-video/deps/glitch_video_types.hpp"

namespace glitch::video {

class CCommonGLDriverBase {
public:
    // Shadow copy of the driver state. The word names are not identified; only the
    // constructor's initial values are reconstructed.
    struct SCommonShadowState {
        SCommonShadowState();  // 0x6dcd54 (C2) / 0x6dcd90 (C1)
        uint32_t m_words[12];  // +0x00..+0x2c
    };

    class CBufferBase : public IBuffer {
    public:
        // 0x6ddf18 (C2) / 0x6ddf84 (C1): IBuffer base, then the driver and a zeroed word.
        CBufferBase(CCommonGLDriverBase* driver, E_BUFFER_TYPE type, E_BUFFER_USAGE usage,
                    uint32_t size, void* data, bool flag);
        // The class stays abstract (bindImpl, unbindImpl, mapImpl, unmapImpl and cloneImpl are pure in the base; the
        // GL buffer CCommonGLDriver<...>::CBuffer implements them). Vtable: the IBuffer slots with +0x1c overridden.
        uint8_t* getMappedPtr() const override;  // +0x1c  0x6dcbe4
        ~CBufferBase() override = default;       // 0x6dd160 (D1) / 0x6de1e8 (D0)
    protected:
        CCommonGLDriverBase* m_driver = nullptr;  // +0x14
        uint32_t m_unknown18 = 0;                 // +0x18, zeroed by the constructor
        uint8_t* m_mappedPtr = nullptr;           // +0x1c
    };

    class CTextureBase {
    public:
        // 0x6dde98 (C2) / 0x6dded8 (C1). Partial: the ITexture base constructor (0x5fe404) is not
        // ported, so only the vtable and the two fields after the base call are set here.
        CTextureBase(const char* name, CCommonGLDriverBase* driver, const STextureDesc& desc);
        void* getMappedPtr() const;  // 0x6dcbec
        void unmapImpl() const;      // 0x6dcbf4
    private:
        uint32_t m_unknown54 = 0;  // +0x54, zeroed by the constructor
        uint8_t m_unknown58 = 0;   // +0x58, zeroed by the constructor
    };

    class CFramebufferBase {
    public:
        void unbind();               // 0x6dcbf8
    };

    class CRenderTargetBase {
    public:
        // One attachment slot, 8 bytes in the original: a 16-bit kind (255 = empty), two flag
        // bytes, then a reference-counted object pointer at +4.
        struct SAttachment {
            uint16_t kind = 255;      // +0x00
            uint8_t flagA = 0;        // +0x02
            uint8_t flagB = 0;        // +0x03
            const void* object = nullptr;  // +0x04, reference-counted in the original
        };

        // 0x6ddbec (C2) / 0x6ddb2c (C1). PARTIAL: the IRenderTarget base constructor (which
        // takes the driver and {-1, -1}) and the vtable store (off_98B088) are not ported, so
        // the driver pointer is not kept. The member initialisers below give the state that
        // the constructor writes after the base call.
        explicit CRenderTargetBase(CCommonGLDriverBase* driver);
        E_PIXEL_FORMAT getColorFormat() const;  // 0x6dcd4c
        // 0x6dcbfc: number of targets for an IRenderTarget::E_ATTACHMENT_TYPE (int32 alias).
        // Type 0 returns the byte at +0x59, types 1 and 2 test the two handles, and type 3
        // tests that the first handle is set and equal to the second.
        int32_t getTargetCount(int32_t attachmentType) const;
        // Test helper, not in the original: sets the three fields that getTargetCount reads.
        void setTargetStateForTest(const void* attachment0, const void* attachment1, uint8_t count) {
            m_attachment0.object = attachment0;
            m_attachment1.object = attachment1;
            m_targetCount = count;
        }
    private:
        uint32_t m_unknown24 = 0;              // +0x24, zeroed by the constructor (not named)
        SAttachment m_slots[4];                // +0x28..+0x47, four attachment slots
        SAttachment m_attachment0;             // +0x48, object pointer at +0x4c
        SAttachment m_attachment1;             // +0x50, object pointer at +0x54
        E_PIXEL_FORMAT m_colorFormat = static_cast<E_PIXEL_FORMAT>(39);  // +0x58, set by the constructor
        uint8_t m_targetCount = 0;             // +0x59, byte
        uint8_t m_flag5a = 1;                  // +0x5a, set by the constructor (meaning not named)
    };

    virtual ~CCommonGLDriverBase() = default;

    bool getBlendEnable() const;               // 0x6dcde8
    E_BLEND_EQUATION getBlendEquation() const; // 0x6dce24
    void getBlendFunc(E_BLEND_FACTOR& src, E_BLEND_FACTOR& dst) const;  // 0x6dce2c
    bool getCullFaceEnable() const;            // 0x6dce9c
    E_FACE_SIDE getCullFace() const;           // 0x6dcea4
    E_FACE_WINDING getFrontFace() const;       // 0x6dceac
    bool getDepthTestEnable() const;           // 0x6dceb4
    E_COMPARE_FUNC getDepthFunc() const;       // 0x6dcebc
    bool getDepthMask() const;                 // 0x6dcec4
    float getClearDepth() const;               // 0x6dcecc
    void getDepthRange(float& nearValue, float& farValue) const;  // 0x6dced4
    bool getDitherEnable() const;              // 0x6dcee8
    float getLineWidth() const;                // 0x6dcef0
    float getPointSize() const;                // 0x6dcef8
    E_POLYGON_MODE getPolygonModeFront() const;  // 0x6dcf00
    E_POLYGON_MODE getPolygonModeBack() const;   // 0x6dcf08
    bool getPolygonOffsetFillEnable() const;   // 0x6dcf10
    bool getPolygonOffsetLineEnable() const;   // 0x6dcf18
    bool getPolygonOffsetPointEnable() const;  // 0x6dcf20
    void getPolygonOffset(float& factor, float& units) const;  // 0x6dcf28
    bool getSampleAlphaToCoverageEnable() const;  // 0x6dcf3c
    bool getSampleCoverageEnable() const;      // 0x6dcf44
    bool getSampleCoverageInvert() const;      // 0x6dcf4c
    float getSampleCoverageValue() const;      // 0x6dcf54
    bool getScissorEnable() const;             // 0x6dcf5c
    const core::rect<int32_t>& getScissor() const;  // 0x6dcf64
    bool getStencilTestEnable() const;         // 0x6dcf6c
    uint8_t getStencilFunc() const;            // 0x6dcf74
    uint8_t getStencilFuncRef(bool*) const;    // 0x6dcf7c
    uint8_t getStencilFuncMask(bool*) const;   // 0x6dcf84
    uint8_t getStencilOpFail() const;          // 0x6dcf8c
    uint8_t getStencilOpZFail() const;         // 0x6dcf94
    uint8_t getStencilOpZPass() const;         // 0x6dcf9c
    uint8_t getStencilMask(bool*) const;       // 0x6dcfa4
    uint8_t getClearStencil() const;           // 0x6dcfac
    void getColorMask(bool& r, bool& g, bool& b, bool& a) const;  // 0x6dce40
    uint32_t getBlendColor() const;            // 0x6dcdf0, packed SColor
    uint32_t getClearColor() const;            // 0x6dce68, packed SColor
    E_PIXEL_FORMAT getColorFormat() const;     // 0x6dcdcc

protected:
    // Offsets are the original ones; the accessor names give the member names.
    bool m_blendEnable = false;                // +0x1c4
    bool m_cullFaceEnable = false;             // +0x1c5
    bool m_depthTestEnable = false;            // +0x1c6
    bool m_depthMask = false;                  // +0x1c7
    bool m_ditherEnable = false;               // +0x1c8
    uint8_t m_blendSrc = 0;                    // +0x200 (byte)
    uint8_t m_blendDst = 0;                    // +0x201 (byte)
    E_BLEND_EQUATION m_blendEquation = 0;      // +0x1fc
    E_FACE_SIDE m_cullFace = 0;                // +0x1d8
    E_FACE_WINDING m_frontFace = 0;            // +0x1dc
    E_COMPARE_FUNC m_depthFunc = 0;            // +0x1e0
    float m_clearDepth = 0.0f;                 // +0x20c
    float m_depthRangeNear = 0.0f;             // +0x210
    float m_depthRangeFar = 0.0f;              // +0x214
    float m_lineWidth = 0.0f;                  // +0x218
    float m_pointSize = 0.0f;                  // +0x21c
    E_POLYGON_MODE m_polygonModeFront = 0;     // +0x1e4
    E_POLYGON_MODE m_polygonModeBack = 0;      // +0x1e8
    bool m_polygonOffsetFillEnable = false;    // +0x1cc
    bool m_polygonOffsetLineEnable = false;    // +0x1cd
    bool m_polygonOffsetPointEnable = false;   // +0x1ce
    float m_polygonOffsetFactor = 0.0f;        // +0x220
    float m_polygonOffsetUnits = 0.0f;         // +0x224
    bool m_sampleAlphaToCoverageEnable = false;  // +0x1d0
    bool m_sampleCoverageEnable = false;      // +0x1d1
    bool m_sampleCoverageInvert = false;       // +0x1d2
    float m_sampleCoverageValue = 0.0f;        // +0x228
    bool m_scissorEnable = false;              // +0x1d3
    core::rect<int32_t> m_scissor{};           // +0x22c
    bool m_stencilTestEnable = false;          // +0x1d4
    uint8_t m_stencilFunc = 0;                 // +0x1f0
    uint8_t m_stencilFuncRef = 0;              // +0x1f1
    uint8_t m_stencilFuncMask = 0;             // +0x1f2
    uint8_t m_stencilOpFail = 0;               // +0x1f4
    uint8_t m_stencilOpZFail = 0;              // +0x1f5
    uint8_t m_stencilOpZPass = 0;              // +0x1f6
    uint8_t m_stencilMask = 0;                 // +0x1f8
    uint8_t m_clearStencil = 0;                // +0x1f9
    E_PIXEL_FORMAT m_colorFormat = 0;          // +0x284
    uint32_t m_colorMask = 0;                  // +0x1ec, packed R | G<<8 | B<<16 | A<<24
    uint32_t m_blendColor = 0;                 // +0x204, packed SColor
    uint32_t m_clearColor = 0;                 // +0x208, packed SColor
};

}  // namespace glitch::video
