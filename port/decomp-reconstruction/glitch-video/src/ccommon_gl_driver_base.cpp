#include "glitch-video/ccommon_gl_driver_base.hpp"

namespace glitch::video {

// 0x6dcbe4 _ZNK6glitch5video19CCommonGLDriverBase11CBufferBase12getMappedPtrEv
void* CCommonGLDriverBase::CBufferBase::getMappedPtr() const { return m_mappedPtr; }

// 0x6dcbec _ZNK6glitch5video19CCommonGLDriverBase12CTextureBase12getMappedPtrEv
void* CCommonGLDriverBase::CTextureBase::getMappedPtr() const { return nullptr; }

// 0x6dcbf4 _ZNK6glitch5video19CCommonGLDriverBase12CTextureBase9unmapImplEv
void CCommonGLDriverBase::CTextureBase::unmapImpl() const {}

// 0x6dcbf8 _ZN6glitch5video19CCommonGLDriverBase16CFramebufferBase6unbindEv
void CCommonGLDriverBase::CFramebufferBase::unbind() {}

// 0x6dcd4c _ZNK6glitch5video19CCommonGLDriverBase17CRenderTargetBase14getColorFormatEv
E_PIXEL_FORMAT CCommonGLDriverBase::CRenderTargetBase::getColorFormat() const {
    return m_colorFormat;
}

// 0x6dcdcc _ZNK6glitch5video19CCommonGLDriverBase14getColorFormatEv
E_PIXEL_FORMAT CCommonGLDriverBase::getColorFormat() const { return m_colorFormat; }

// 0x6dcde8 _ZNK6glitch5video19CCommonGLDriverBase14getBlendEnableEv
bool CCommonGLDriverBase::getBlendEnable() const { return m_blendEnable; }

// 0x6dce24 _ZNK6glitch5video19CCommonGLDriverBase16getBlendEquationEv
E_BLEND_EQUATION CCommonGLDriverBase::getBlendEquation() const { return m_blendEquation; }

// 0x6dce2c _ZNK6glitch5video19CCommonGLDriverBase12getBlendFuncERNS0_14E_BLEND_FACTORES3_
void CCommonGLDriverBase::getBlendFunc(E_BLEND_FACTOR& src, E_BLEND_FACTOR& dst) const {
    src = m_blendSrc;
    dst = m_blendDst;
}

// 0x6dce9c _ZNK6glitch5video19CCommonGLDriverBase17getCullFaceEnableEv
bool CCommonGLDriverBase::getCullFaceEnable() const { return m_cullFaceEnable; }

// 0x6dcea4 _ZNK6glitch5video19CCommonGLDriverBase11getCullFaceEv
E_FACE_SIDE CCommonGLDriverBase::getCullFace() const { return m_cullFace; }

// 0x6dceac _ZNK6glitch5video19CCommonGLDriverBase12getFrontFaceEv
E_FACE_WINDING CCommonGLDriverBase::getFrontFace() const { return m_frontFace; }

// 0x6dceb4 _ZNK6glitch5video19CCommonGLDriverBase18getDepthTestEnableEv
bool CCommonGLDriverBase::getDepthTestEnable() const { return m_depthTestEnable; }

// 0x6dcebc _ZNK6glitch5video19CCommonGLDriverBase12getDepthFuncEv
E_COMPARE_FUNC CCommonGLDriverBase::getDepthFunc() const { return m_depthFunc; }

// 0x6dcec4 _ZNK6glitch5video19CCommonGLDriverBase12getDepthMaskEv
bool CCommonGLDriverBase::getDepthMask() const { return m_depthMask; }

// 0x6dcecc _ZNK6glitch5video19CCommonGLDriverBase13getClearDepthEv
float CCommonGLDriverBase::getClearDepth() const { return m_clearDepth; }

// 0x6dced4 _ZNK6glitch5video19CCommonGLDriverBase13getDepthRangeERfS2_
void CCommonGLDriverBase::getDepthRange(float& nearValue, float& farValue) const {
    nearValue = m_depthRangeNear;
    farValue = m_depthRangeFar;
}

// 0x6dcee8 _ZNK6glitch5video19CCommonGLDriverBase15getDitherEnableEv
bool CCommonGLDriverBase::getDitherEnable() const { return m_ditherEnable; }

// 0x6dcef0 _ZNK6glitch5video19CCommonGLDriverBase12getLineWidthEv
float CCommonGLDriverBase::getLineWidth() const { return m_lineWidth; }

// 0x6dcef8 _ZNK6glitch5video19CCommonGLDriverBase12getPointSizeEv
float CCommonGLDriverBase::getPointSize() const { return m_pointSize; }

// 0x6dcf00 _ZNK6glitch5video19CCommonGLDriverBase19getPolygonModeFrontEv
E_POLYGON_MODE CCommonGLDriverBase::getPolygonModeFront() const { return m_polygonModeFront; }

// 0x6dcf08 _ZNK6glitch5video19CCommonGLDriverBase18getPolygonModeBackEv
E_POLYGON_MODE CCommonGLDriverBase::getPolygonModeBack() const { return m_polygonModeBack; }

// 0x6dcf10 _ZNK6glitch5video19CCommonGLDriverBase26getPolygonOffsetFillEnableEv
bool CCommonGLDriverBase::getPolygonOffsetFillEnable() const { return m_polygonOffsetFillEnable; }

// 0x6dcf18 _ZNK6glitch5video19CCommonGLDriverBase26getPolygonOffsetLineEnableEv
bool CCommonGLDriverBase::getPolygonOffsetLineEnable() const { return m_polygonOffsetLineEnable; }

// 0x6dcf20 _ZNK6glitch5video19CCommonGLDriverBase27getPolygonOffsetPointEnableEv
bool CCommonGLDriverBase::getPolygonOffsetPointEnable() const { return m_polygonOffsetPointEnable; }

// 0x6dcf28 _ZNK6glitch5video19CCommonGLDriverBase16getPolygonOffsetERfS2_
void CCommonGLDriverBase::getPolygonOffset(float& factor, float& units) const {
    factor = m_polygonOffsetFactor;
    units = m_polygonOffsetUnits;
}

// 0x6dcf3c _ZNK6glitch5video19CCommonGLDriverBase30getSampleAlphaToCoverageEnableEv
bool CCommonGLDriverBase::getSampleAlphaToCoverageEnable() const {
    return m_sampleAlphaToCoverageEnable;
}

// 0x6dcf44 _ZNK6glitch5video19CCommonGLDriverBase23getSampleCoverageEnableEv
bool CCommonGLDriverBase::getSampleCoverageEnable() const { return m_sampleCoverageEnable; }

// 0x6dcf4c _ZNK6glitch5video19CCommonGLDriverBase23getSampleCoverageInvertEv
bool CCommonGLDriverBase::getSampleCoverageInvert() const { return m_sampleCoverageInvert; }

// 0x6dcf54 _ZNK6glitch5video19CCommonGLDriverBase22getSampleCoverageValueEv
float CCommonGLDriverBase::getSampleCoverageValue() const { return m_sampleCoverageValue; }

// 0x6dcf5c _ZNK6glitch5video19CCommonGLDriverBase16getScissorEnableEv
bool CCommonGLDriverBase::getScissorEnable() const { return m_scissorEnable; }

// 0x6dcf64 _ZNK6glitch5video19CCommonGLDriverBase10getScissorEv
const core::rect<int32_t>& CCommonGLDriverBase::getScissor() const { return m_scissor; }

// 0x6dcf6c _ZNK6glitch5video19CCommonGLDriverBase20getStencilTestEnableEv
bool CCommonGLDriverBase::getStencilTestEnable() const { return m_stencilTestEnable; }

// 0x6dcf74 _ZNK6glitch5video19CCommonGLDriverBase14getStencilFuncEv
uint8_t CCommonGLDriverBase::getStencilFunc() const { return m_stencilFunc; }

// 0x6dcf7c _ZNK6glitch5video19CCommonGLDriverBase17getStencilFuncRefEPb
uint8_t CCommonGLDriverBase::getStencilFuncRef(bool*) const { return m_stencilFuncRef; }

// 0x6dcf84 _ZNK6glitch5video19CCommonGLDriverBase18getStencilFuncMaskEPb
uint8_t CCommonGLDriverBase::getStencilFuncMask(bool*) const { return m_stencilFuncMask; }

// 0x6dcf8c _ZNK6glitch5video19CCommonGLDriverBase16getStencilOpFailEv
uint8_t CCommonGLDriverBase::getStencilOpFail() const { return m_stencilOpFail; }

// 0x6dcf94 _ZNK6glitch5video19CCommonGLDriverBase17getStencilOpZFailEv
uint8_t CCommonGLDriverBase::getStencilOpZFail() const { return m_stencilOpZFail; }

// 0x6dcf9c _ZNK6glitch5video19CCommonGLDriverBase17getStencilOpZPassEv
uint8_t CCommonGLDriverBase::getStencilOpZPass() const { return m_stencilOpZPass; }

// 0x6dcfa4 _ZNK6glitch5video19CCommonGLDriverBase14getStencilMaskEPb
uint8_t CCommonGLDriverBase::getStencilMask(bool*) const { return m_stencilMask; }

// 0x6dcfac _ZNK6glitch5video19CCommonGLDriverBase15getClearStencilEv
uint8_t CCommonGLDriverBase::getClearStencil() const { return m_clearStencil; }

}  // namespace glitch::video
