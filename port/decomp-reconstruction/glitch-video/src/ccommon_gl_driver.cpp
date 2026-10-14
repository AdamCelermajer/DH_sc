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

// The GL enum tables of the original (.rodata, the anonymous-namespace arrays BlendFactorMap, BlendEquationMap, FaceCullingMap,
// CompareFuncMap, StencilOpMap and FaceWindingMap, read from the ELF at 0x8e0e3c and the following addresses). They are indexed by the
// engine's enumerators.
constexpr uint32_t kBlendFactorMap[15] = {0x0,    0x1,    0x300,  0x301,  0x302, 0x303, 0x306, 0x307,
                                          0x304,  0x305,  0x8001, 0x8002, 0x8003, 0x8004, 0x308};
constexpr uint32_t kBlendEquationMap[5] = {0x8006, 0x800a, 0x800b, 0x8007, 0x8008};
constexpr uint32_t kFaceCullingMap[3] = {0x405, 0x404, 0x408};
constexpr uint32_t kCompareFuncMap[8] = {0x200, 0x201, 0x202, 0x203, 0x204, 0x205, 0x206, 0x207};
constexpr uint32_t kStencilOpMap[8] = {0x1e00, 0x0, 0x1e01, 0x1e02, 0x8507, 0x1e03, 0x8508, 0x150a};
constexpr uint32_t kFaceWindingMap[2] = {0x901, 0x900};

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
// Compares the packed (src | dst << 8) pair with the cache, flushes, glBlendFunc(BlendFactorMap[src], BlendFactorMap[dst]), stores.
void CCommonGLDriver::setBlendFunc(E_BLEND_FACTOR src, E_BLEND_FACTOR dst) {
    uint8_t s = static_cast<uint8_t>(src & 0xff);
    uint8_t d = static_cast<uint8_t>(dst & 0xff);
    if (s == m_blendSrc && d == m_blendDst) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.blendFunc) t.blendFunc(kBlendFactorMap[s], kBlendFactorMap[d]);
    m_blendSrc = s;
    m_blendDst = d;
}

// 0x5af3b4 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE16setBlendEquationENS0_16E_BLEND_EQUATIONE
void CCommonGLDriver::setBlendEquation(E_BLEND_EQUATION mode) {
    if (mode == m_blendEquation) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.blendEquation) t.blendEquation(kBlendEquationMap[mode]);
    m_blendEquation = mode;
}

// 0x5af6b8 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE11setCullFaceENS0_11E_FACE_SIDEE
void CCommonGLDriver::setCullFace(E_FACE_SIDE side) {
    if (side == m_cullFace) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.cullFace) t.cullFace(kFaceCullingMap[side]);
    m_cullFace = side;
}

// 0x5afc78 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE12setFrontFaceENS0_14E_FACE_WINDINGE
// The winding is flipped (1 - winding) when the byte at +0x4a0 is set; the cache keeps the unflipped value.
void CCommonGLDriver::setFrontFace(E_FACE_WINDING winding) {
    if (winding == m_frontFace) return;
    flush();
    const int32_t index = m_frontFaceFlip ? 1 - winding : winding;
    auto& t = deps::activeGLFunctionTable();
    if (t.frontFace) t.frontFace(kFaceWindingMap[index]);
    m_frontFace = winding;
}

// 0x5af760 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE12setDepthFuncENS0_14E_COMPARE_FUNCE
void CCommonGLDriver::setDepthFunc(E_COMPARE_FUNC func) {
    if (func == m_depthFunc) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.depthFunc) t.depthFunc(kCompareFuncMap[func]);
    m_depthFunc = func;
}

// 0x5afa24 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE14setStencilFuncENS0_14E_COMPARE_FUNCE
// The function, reference and mask are the bytes +0x1f0, +0x1f1 and +0x1f2 of one cached word; the call passes all three.
void CCommonGLDriver::setStencilFunc(E_COMPARE_FUNC func) {
    uint8_t f = static_cast<uint8_t>(func & 0xff);
    if (f == m_stencilFunc) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.stencilFunc) t.stencilFunc(kCompareFuncMap[f], m_stencilFuncRef, m_stencilFuncMask);
    m_stencilFunc = f;
}

// 0x5af9cc _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE17setStencilFuncRefEh
void CCommonGLDriver::setStencilFuncRef(uint8_t ref) {
    if (ref == m_stencilFuncRef) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.stencilFunc) t.stencilFunc(kCompareFuncMap[m_stencilFunc], ref, m_stencilFuncMask);
    m_stencilFuncRef = ref;
}

// 0x5af974 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE18setStencilFuncMaskEh
void CCommonGLDriver::setStencilFuncMask(uint8_t mask) {
    if (mask == m_stencilFuncMask) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.stencilFunc) t.stencilFunc(kCompareFuncMap[m_stencilFunc], m_stencilFuncRef, mask);
    m_stencilFuncMask = mask;
}

// 0x5afc14 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE16setStencilOpFailENS0_12E_STENCIL_OPE
// The fail, z-fail and z-pass operations are the bytes +0x1f4, +0x1f5 and +0x1f6 of one cached word; the call passes all three.
void CCommonGLDriver::setStencilOpFail(E_STENCIL_OP op) {
    uint8_t v = static_cast<uint8_t>(op & 0xff);
    if (v == m_stencilOpFail) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.stencilOp) t.stencilOp(kStencilOpMap[v], kStencilOpMap[m_stencilOpZFail], kStencilOpMap[m_stencilOpZPass]);
    m_stencilOpFail = v;
}

// 0x5afbb0 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE17setStencilOpZFailENS0_12E_STENCIL_OPE
void CCommonGLDriver::setStencilOpZFail(E_STENCIL_OP op) {
    uint8_t v = static_cast<uint8_t>(op & 0xff);
    if (v == m_stencilOpZFail) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.stencilOp) t.stencilOp(kStencilOpMap[m_stencilOpFail], kStencilOpMap[v], kStencilOpMap[m_stencilOpZPass]);
    m_stencilOpZFail = v;
}

// 0x5afb4c _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE17setStencilOpZPassENS0_12E_STENCIL_OPE
void CCommonGLDriver::setStencilOpZPass(E_STENCIL_OP op) {
    uint8_t v = static_cast<uint8_t>(op & 0xff);
    if (v == m_stencilOpZPass) return;
    flush();
    auto& t = deps::activeGLFunctionTable();
    if (t.stencilOp) t.stencilOp(kStencilOpMap[m_stencilOpFail], kStencilOpMap[m_stencilOpZFail], kStencilOpMap[v]);
    m_stencilOpZPass = v;
}

// 0x5af574 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE13setBlendColorENS0_6SColorE
// glBlendColor with each byte times 1/255, then the four bytes are cached (the word at +0x204).
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
// The same for glClearColor and the word at +0x208.
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

// 0x5b0594 _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE15ReloadbufferMapEv
// The entry fields are the registerBufferMap arguments: arg1 is the GL buffer name, arg2 the target and arg3 the usage.
void CCommonGLDriver::ReloadbufferMap() {
    auto& t = deps::activeGLFunctionTable();
    for (auto& entry : m_bufferMap) {
        const BufferInfo& info = entry.second;
        if (t.bindBuffer) t.bindBuffer(info.arg2, info.arg1);
        if (t.bufferData) t.bufferData(info.arg2, static_cast<int32_t>(info.byteCount), info.data, info.arg3);
        if (t.bindBuffer) t.bindBuffer(info.arg2, 0);
    }
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
