#include "glitch-video/ccommon_gl_driver.hpp"

#include <cstdlib>
#include <cstring>

#include "glitch-video/deps/gl_function_table.hpp"

namespace glitch::video {

namespace {

// Entries of the GL table are optional; a null entry is skipped.
void glEnableCap(uint32_t cap) {
    auto& t = deps::activeGLFunctionTable();
    if (t.enable) t.enable(cap);
}

void glDisableCap(uint32_t cap) {
    auto& t = deps::activeGLFunctionTable();
    if (t.disable) t.disable(cap);
}

// Packs the low byte of a value into one byte lane of a word.
uint32_t lane(uint32_t value, unsigned shift) {
    return (value & 0xffu) << shift;
}

// Float constant 1/255 (bit pattern 0x3b808081 in the original).
constexpr float kInv255 = 1.0f / 255.0f;

}  // namespace

// 0x5b7080 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE14setBlendEnableEb
void CCommonGLDriver::setBlendEnable(bool enable) { setCapEnable(m_blendEnable, enable, 0xbe2); }

// 0x5b7034 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE17setCullFaceEnableEb
void CCommonGLDriver::setCullFaceEnable(bool enable) { setCapEnable(m_cullFaceEnable, enable, 0xb44); }

// 0x5b6fe8 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE18setDepthTestEnableEb
void CCommonGLDriver::setDepthTestEnable(bool enable) { setCapEnable(m_depthTestEnable, enable, 0xb71); }

// 0x5b6f9c _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE15setDitherEnableEb
void CCommonGLDriver::setDitherEnable(bool enable) { setCapEnable(m_ditherEnable, enable, 0xbd0); }

// 0x5b6f50 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE26setPolygonOffsetFillEnableEb
void CCommonGLDriver::setPolygonOffsetFillEnable(bool enable) {
    setCapEnable(m_polygonOffsetFillEnable, enable, 0x8037);
}

// 0x5b6f04 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE30setSampleAlphaToCoverageEnableEb
void CCommonGLDriver::setSampleAlphaToCoverageEnable(bool enable) {
    setCapEnable(m_sampleAlphaToCoverageEnable, enable, 0x809e);
}

// 0x5b6eb8 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE23setSampleCoverageEnableEb
void CCommonGLDriver::setSampleCoverageEnable(bool enable) {
    setCapEnable(m_sampleCoverageEnable, enable, 0x80a0);
}

// 0x5b6e6c _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE16setScissorEnableEb
void CCommonGLDriver::setScissorEnable(bool enable) { setCapEnable(m_scissorEnable, enable, 0xc11); }

// 0x5b6e20 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE20setStencilTestEnableEb
void CCommonGLDriver::setStencilTestEnable(bool enable) {
    setCapEnable(m_stencilTestEnable, enable, 0xb90);
}

// 0x5af2b0 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE26setPolygonOffsetLineEnableEb
// Empty in the original.
void CCommonGLDriver::setPolygonOffsetLineEnable(bool) {}

// 0x5af2b4 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE27setPolygonOffsetPointEnableEb
// Empty in the original.
void CCommonGLDriver::setPolygonOffsetPointEnable(bool) {}

// 0x5afccc _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE12setDepthMaskEb
void CCommonGLDriver::setDepthMask(bool flag) {
    if (m_depthMask == flag) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.depthMask) t.depthMask(flag ? 1 : 0);
    m_depthMask = flag;
}

// 0x5afd00 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE12setLineWidthEf
void CCommonGLDriver::setLineWidth(float width) {
    if (width == m_lineWidth) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.lineWidth) t.lineWidth(width);
    m_lineWidth = width;
}

// 0x5af220 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE12setPointSizeEf
// The original stores the value without a GL call (point size is not set on GLES).
void CCommonGLDriver::setPointSize(float size) {
    if (size == m_pointSize) return;
    flush();
    m_pointSize = size;
}

// 0x5b0e94 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE13setClearDepthEf
// The original compares with the cached clear depth but never stores the new
// value (confirmed in the ARM listing: the GL call is a tail branch, no store).
void CCommonGLDriver::setClearDepth(float depth) {
    if (depth == m_clearDepth) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.clearDepthf) t.clearDepthf(depth);
}

// 0x5b0ed4 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE13setDepthRangeEff
void CCommonGLDriver::setDepthRange(float nearValue, float farValue) {
    if (nearValue == m_depthRangeNear && farValue == m_depthRangeFar) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.depthRangef) t.depthRangef(nearValue, farValue);
    m_depthRangeFar = farValue;
    m_depthRangeNear = nearValue;
}

// 0x5af828 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE16setPolygonOffsetEff
void CCommonGLDriver::setPolygonOffset(float factor, float units) {
    if (factor == m_polygonOffsetFactor && units == m_polygonOffsetUnits) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.polygonOffset) t.polygonOffset(factor, units);
    m_polygonOffsetUnits = units;
    m_polygonOffsetFactor = factor;
}

// 0x5af8f8 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE22setSampleCoverageValueEf
void CCommonGLDriver::setSampleCoverageValue(float value) {
    if (value == m_sampleCoverageValue) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.sampleCoverage) t.sampleCoverage(value, m_sampleCoverageInvert ? 1 : 0);
    m_sampleCoverageValue = value;
}

// 0x5af93c _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE23setSampleCoverageInvertEb
void CCommonGLDriver::setSampleCoverageInvert(bool invert) {
    if (m_sampleCoverageInvert == invert) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.sampleCoverage) t.sampleCoverage(m_sampleCoverageValue, invert ? 1 : 0);
    m_sampleCoverageInvert = invert;
}

// 0x5b0e2c _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE14setStencilMaskEh
void CCommonGLDriver::setStencilMask(uint8_t mask) {
    if (m_stencilMask == mask) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.stencilMask) t.stencilMask(mask);
    m_stencilMask = mask;
}

// 0x5b0e60 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE15setClearStencilEh
void CCommonGLDriver::setClearStencil(uint8_t value) {
    if (m_clearStencil == value) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.clearStencil) t.clearStencil(value);
    m_clearStencil = value;
}

// 0x5b0cec _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE12setColorMaskEbbbb
void CCommonGLDriver::setColorMask(bool r, bool g, bool b, bool a) {
    uint32_t packed = lane(r, 0) | lane(g, 8) | lane(b, 16) | (static_cast<uint32_t>(a) << 24);
    if (packed == m_colorMask) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.colorMask) t.colorMask(r ? 1 : 0, g ? 1 : 0, b ? 1 : 0, a ? 1 : 0);
    m_colorMask = packed;
}

// 0x5af258 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE19setPolygonModeFrontENS0_14E_POLYGON_MODEE
void CCommonGLDriver::setPolygonModeFront(E_POLYGON_MODE mode) {
    if (mode == m_polygonModeFront) return;
    flush();
    m_polygonModeFront = mode;
}

// 0x5af284 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE18setPolygonModeBackENS0_14E_POLYGON_MODEE
void CCommonGLDriver::setPolygonModeBack(E_POLYGON_MODE mode) {
    if (mode == m_polygonModeBack) return;
    flush();
    m_polygonModeBack = mode;
}

// 0x5af1fc _ZNK6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE24getMaximalPrimitiveCountEv
uint32_t CCommonGLDriver::getMaximalPrimitiveCount() const { return 0xffff; }

// 0x5af204 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE22captureFramebufferImplERKN5boost13intrusive_ptrINS0_8ITextureEEERKNS_4core10position2dIiEERKNSE_4rectIiEEhNS0_23E_TEXTURE_CUBE_MAP_FACEEb
bool CCommonGLDriver::captureFramebufferImpl(const boost::intrusive_ptr<ITexture>&, const core::position2d<int32_t>&,
                                             const core::rect<int32_t>&, uint8_t, E_TEXTURE_CUBE_MAP_FACE, bool) {
    return false;
}

// 0x5af20c _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE11swapBuffersEi
void CCommonGLDriver::swapBuffers(int32_t) { presentHook(); }

// 0x5af35c _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE12setBlendFuncENS0_14E_BLEND_FACTORES8_
// Partial: glBlendFunc(BlendFactorMap[src], BlendFactorMap[dst]) is omitted
// because the map table is not in the repo. The cache is updated as in the original.
void CCommonGLDriver::setBlendFunc(E_BLEND_FACTOR src, E_BLEND_FACTOR dst) {
    uint8_t s = static_cast<uint8_t>(src & 0xff);
    uint8_t d = static_cast<uint8_t>(dst & 0xff);
    if (s == m_blendSrc && d == m_blendDst) return;
    flush();
    m_blendSrc = s;
    m_blendDst = d;
}

// 0x5af3b4 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE16setBlendEquationENS0_16E_BLEND_EQUATIONE
// Partial: glBlendEquation(BlendEquationMap[mode]) omitted (map not in repo).
void CCommonGLDriver::setBlendEquation(E_BLEND_EQUATION mode) {
    if (mode == m_blendEquation) return;
    flush();
    m_blendEquation = mode;
}

// 0x5af6b8 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE11setCullFaceENS0_11E_FACE_SIDEE
// Partial: glCullFace(FaceCullingMap[side]) omitted (map not in repo).
void CCommonGLDriver::setCullFace(E_FACE_SIDE side) {
    if (side == m_cullFace) return;
    flush();
    m_cullFace = side;
}

// 0x5afc78 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE12setFrontFaceENS0_14E_FACE_WINDINGE
// Partial: glFrontFace(FaceWindingMap[index]) omitted (map not in repo). The
// index is flipped when the byte at +0x4a0 is set, as in the original.
void CCommonGLDriver::setFrontFace(E_FACE_WINDING winding) {
    if (winding == m_frontFace) return;
    flush();
    int32_t index = m_frontFaceFlip ? 1 - winding : winding;
    (void)index;
    m_frontFace = winding;
}

// 0x5af760 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE12setDepthFuncENS0_14E_COMPARE_FUNCE
// Partial: glDepthFunc(CompareFuncMap[func]) omitted (map not in repo).
void CCommonGLDriver::setDepthFunc(E_COMPARE_FUNC func) {
    if (func == m_depthFunc) return;
    flush();
    m_depthFunc = func;
}

// 0x5afa24 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE14setStencilFuncENS0_14E_COMPARE_FUNCE
// Partial: glStencilFunc is omitted (CompareFuncMap not in repo). Only the
// func byte of the packed stencil word is compared and stored.
void CCommonGLDriver::setStencilFunc(E_COMPARE_FUNC func) {
    uint8_t f = static_cast<uint8_t>(func & 0xff);
    if (f == m_stencilFunc) return;
    flush();
    m_stencilFunc = f;
}

// 0x5af9cc _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE17setStencilFuncRefEh
// Partial: glStencilFunc is omitted (see setStencilFunc).
void CCommonGLDriver::setStencilFuncRef(uint8_t ref) {
    if (ref == m_stencilFuncRef) return;
    flush();
    m_stencilFuncRef = ref;
}

// 0x5af974 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE18setStencilFuncMaskEh
// Partial: glStencilFunc is omitted (see setStencilFunc).
void CCommonGLDriver::setStencilFuncMask(uint8_t mask) {
    if (mask == m_stencilFuncMask) return;
    flush();
    m_stencilFuncMask = mask;
}

// 0x5afc14 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE16setStencilOpFailENS0_12E_STENCIL_OPE
// Partial: glStencilOp is omitted (StencilOpMap not in repo).
void CCommonGLDriver::setStencilOpFail(E_STENCIL_OP op) {
    uint8_t v = static_cast<uint8_t>(op & 0xff);
    if (v == m_stencilOpFail) return;
    flush();
    m_stencilOpFail = v;
}

// 0x5afbb0 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE17setStencilOpZFailENS0_12E_STENCIL_OPE
// Partial: glStencilOp is omitted (see setStencilOpFail).
void CCommonGLDriver::setStencilOpZFail(E_STENCIL_OP op) {
    uint8_t v = static_cast<uint8_t>(op & 0xff);
    if (v == m_stencilOpZFail) return;
    flush();
    m_stencilOpZFail = v;
}

// 0x5afb4c _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE17setStencilOpZPassENS0_12E_STENCIL_OPE
// Partial: glStencilOp is omitted (see setStencilOpFail).
void CCommonGLDriver::setStencilOpZPass(E_STENCIL_OP op) {
    uint8_t v = static_cast<uint8_t>(op & 0xff);
    if (v == m_stencilOpZPass) return;
    flush();
    m_stencilOpZPass = v;
}

// 0x5af574 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE13setBlendColorENS0_6SColorE
// Partial: the per-byte x / 255 conversion follows the ARM listing; the cache
// store after the GL call is not verified against the listing.
void CCommonGLDriver::setBlendColor(SColor color) {
    if (color.color == m_blendColor) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.blendColor) {
        t.blendColor(static_cast<float>(color.color & 0xff) * kInv255,
                     static_cast<float>((color.color >> 8) & 0xff) * kInv255,
                     static_cast<float>((color.color >> 16) & 0xff) * kInv255,
                     static_cast<float>((color.color >> 24) & 0xff) * kInv255);
    }
    m_blendColor = color.color;
}

// 0x5b0d4c _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE13setClearColorENS0_6SColorE
// Partial: same conversion as setBlendColor, glClearColor; the cache store
// is not verified against the listing.
void CCommonGLDriver::setClearColor(SColor color) {
    if (color.color == m_clearColor) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.clearColor) {
        t.clearColor(static_cast<float>(color.color & 0xff) * kInv255,
                     static_cast<float>((color.color >> 8) & 0xff) * kInv255,
                     static_cast<float>((color.color >> 16) & 0xff) * kInv255,
                     static_cast<float>((color.color >> 24) & 0xff) * kInv255);
    }
    m_clearColor = color.color;
}

// Virtual hooks with empty defaults in the port.
// 0x5b1860 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE10beginSceneEv
// Partial: the original also runs IVideoDriver::beginScene (0x5a8b74). CCommonGLDriverBase
// is not derived from IVideoDriver in this port, so that base call is not made.
bool CCommonGLDriver::beginScene() { return true; }

// 0x5b1838 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE8endSceneEv
// Partial: the original also runs IVideoDriver::endScene (0x5aab20), which is not
// reachable from this base in the port.
bool CCommonGLDriver::endScene() {
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.flush) t.flush();
    return true;
}

// 0x5b13f4 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE12createBufferENS0_13E_BUFFER_TYPEENS0_14E_BUFFER_USAGEEjPvb
boost::intrusive_ptr<IBuffer> CCommonGLDriver::createBuffer(E_BUFFER_TYPE type, E_BUFFER_USAGE usage, uint32_t size,
                                                            void* data, bool flag) {
    return boost::intrusive_ptr<IBuffer>(new CBuffer(this, type, usage, size, data, flag), true);  // a new buffer has count 0
}

// Partial slots of the GL buffer (see the header): no GL work.
void CCommonGLDriver::CBuffer::bindImpl(bool) {}
void CCommonGLDriver::CBuffer::unbindImpl() {}
uint8_t* CCommonGLDriver::CBuffer::mapImpl(uint32_t) const { return nullptr; }
void CCommonGLDriver::CBuffer::unmapImpl() const {}
boost::intrusive_ptr<IBuffer> CCommonGLDriver::CBuffer::cloneImpl() const {
    return boost::intrusive_ptr<IBuffer>(
        new CBuffer(m_driver, m_type, m_usage, m_size, m_data, (m_flags12 & kFlagOwnsData) != 0), true);
}

// 0x5b5be4 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE17registerBufferMapEjjjjPKv
void CCommonGLDriver::registerBufferMap(uint32_t key, uint32_t arg1, uint32_t arg2, uint32_t arg3, size_t byteCount,
                                        const void* data) {
    BufferInfo info;
    info.arg1 = arg1;
    info.arg2 = arg2;
    info.arg3 = arg3;
    info.byteCount = byteCount;
    info.data = std::malloc(byteCount);
    std::memcpy(info.data, data, byteCount);
    // insert_unique: an existing key keeps its old entry; the new copy is not freed.
    m_bufferMap.insert(std::make_pair(key, info));
}

// 0x5b1f5c _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE19unregisterBufferMapEj
void CCommonGLDriver::unregisterBufferMap(uint32_t key) {
    auto it = m_bufferMap.find(key);
    if (it == m_bufferMap.end()) return;
    std::free(it->second.data);
    m_bufferMap.erase(it);
}

// 0x5b35d8 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE14clearBufferMapEv
void CCommonGLDriver::clearBufferMap() {
    for (auto& entry : m_bufferMap) std::free(entry.second.data);
    m_bufferMap.clear();
}

void CCommonGLDriver::flush() {}
void CCommonGLDriver::presentHook() {}
void CCommonGLDriver::stateHook() {}

// Shared capability setter: compare, flush, glEnable or glDisable, store.
void CCommonGLDriver::setCapEnable(bool& state, bool enable, uint32_t cap) {
    if (state == enable) return;
    flush();
    if (enable) {
        glEnableCap(cap);
    } else {
        glDisableCap(cap);
    }
    state = enable;
}

}  // namespace glitch::video
