#pragma once

#include <cstdint>

#include "glitch-video/deps/glitch_core.hpp"
#include "glitch-video/deps/glitch_video_types.hpp"
#include "glitch-video/video_driver.hpp"

namespace glitch::video {

// glitch::video::CNullDriver and its nested buffer/texture/framebuffer stubs.
// The original is a virtual driver interface; methods here are the ones that
// are empty or return a constant in the original binary.
class CNullDriver {
public:
    class CBuffer {
    public:
        virtual ~CBuffer() = default;
    virtual void bindImpl(bool);
    virtual void unbindImpl();
    virtual uint8_t* mapImpl(uint32_t) const;
    virtual void unmapImpl() const;
    virtual uint8_t* getMappedPtr() const;
    };

    class CTexture {
    public:
        virtual ~CTexture() = default;
    virtual bool bindImpl(bool);
    virtual void unbindImpl();
    virtual uint8_t* mapImpl(uint8_t, E_TEXTURE_CUBE_MAP_FACE, uint8_t);
    virtual void unmapImpl() const;
    virtual uint8_t* getMappedPtr() const;
    virtual void generateMipmapsImpl();
    private:
        uint16_t m_flags = 0;  // +0x40
    };

    class CFramebuffer {
    public:
        virtual ~CFramebuffer() = default;
    virtual void bind();
    virtual void unbind();
    };

    virtual ~CNullDriver() = default;
    virtual bool checkDriverReset();
    virtual bool beginScene();
    virtual bool captureFramebufferImpl(const intrusive_ptr<ITexture>& texture, const core::position2d<int32_t>& pos, const core::rect<int32_t>& area, uint8_t mip, E_TEXTURE_CUBE_MAP_FACE face, bool flag);
    virtual bool swapBuffers(int32_t);
    virtual void clearBuffers(int32_t);
    virtual bool beginScene2D();
    virtual bool endScene2D();
    virtual bool ReinitDriver();
    virtual void registerBufferMap(uint32_t, uint32_t, uint32_t, uint32_t, const void*);
    virtual void ReloadbufferMap();
    virtual void unregisterBufferMap(uint32_t);
    virtual void clearBufferMap();
    virtual void resetTexturesLoader();
    virtual bool reloadTexturesData();
    virtual void reloadShaders();
    virtual bool getBlendEnable() const;
    virtual SColor getBlendColor() const;
    virtual E_BLEND_EQUATION getBlendEquation() const;
    virtual void getBlendFunc(E_BLEND_FACTOR& src, E_BLEND_FACTOR& dst) const;
    virtual void getColorMask(bool& r, bool& g, bool& b, bool& a) const;
    virtual SColor getClearColor() const;
    virtual bool getCullFaceEnable() const;
    virtual E_FACE_SIDE getCullFace() const;
    virtual E_FACE_WINDING getFrontFace() const;
    virtual bool getDepthTestEnable() const;
    virtual E_COMPARE_FUNC getDepthFunc() const;
    virtual bool getDepthMask() const;
    virtual float getClearDepth() const;
    virtual void getDepthRange(float& nearValue, float& farValue) const;
    virtual bool getDitherEnable() const;
    virtual float getLineWidth() const;
    virtual float getPointSize() const;
    virtual E_POLYGON_MODE getPolygonModeFront() const;
    virtual E_POLYGON_MODE getPolygonModeBack() const;
    virtual bool getPolygonOffsetFillEnable() const;
    virtual bool getPolygonOffsetLineEnable() const;
    virtual bool getPolygonOffsetPointEnable() const;
    virtual void getPolygonOffset(float& factor, float& units) const;
    virtual bool getSampleAlphaToCoverageEnable() const;
    virtual bool getSampleCoverageEnable() const;
    virtual bool getSampleCoverageInvert() const;
    virtual float getSampleCoverageValue() const;
    virtual bool getScissorEnable() const;
    virtual const core::rect<int32_t>& getScissor() const;
    virtual bool getStencilTestEnable() const;
    virtual E_COMPARE_FUNC getStencilFunc() const;
    virtual uint8_t getStencilFuncRef(bool*) const;
    virtual uint8_t getStencilFuncMask(bool*) const;
    virtual E_STENCIL_OP getStencilOpFail() const;
    virtual E_STENCIL_OP getStencilOpZFail() const;
    virtual E_STENCIL_OP getStencilOpZPass() const;
    virtual uint8_t getStencilMask(bool*) const;
    virtual uint8_t getClearStencil() const;
    virtual void setBlendEnable(bool);
    virtual void setBlendEquation(E_BLEND_EQUATION);
    virtual void setBlendFunc(E_BLEND_FACTOR, E_BLEND_FACTOR);
    virtual void setColorMask(bool, bool, bool, bool);
    virtual void setCullFaceEnable(bool);
    virtual void setCullFace(E_FACE_SIDE);
    virtual void setFrontFace(E_FACE_WINDING);
    virtual void setDepthTestEnable(bool);
    virtual void setDepthFunc(E_COMPARE_FUNC);
    virtual void setDepthMask(bool);
    virtual void setClearDepth(float);
    virtual void setDepthRange(float, float);
    virtual void setDitherEnable(bool);
    virtual void setLineWidth(float);
    virtual void setPointSize(float);
    virtual void setPolygonModeFront(E_POLYGON_MODE);
    virtual void setPolygonModeBack(E_POLYGON_MODE);
    virtual void setPolygonOffsetFillEnable(bool);
    virtual void setPolygonOffsetLineEnable(bool);
    virtual void setPolygonOffsetPointEnable(bool);
    virtual void setPolygonOffset(float, float);
    virtual void setSampleAlphaToCoverageEnable(bool);
    virtual void setSampleCoverageEnable(bool);
    virtual void setSampleCoverageInvert(bool);
    virtual void setSampleCoverageValue(float);
    virtual void setScissorEnable(bool);
    virtual void setScissor(const core::rect<int32_t>&);
    virtual void setStencilTestEnable(bool);
    virtual void setStencilFunc(E_COMPARE_FUNC);
    virtual void setStencilFuncRef(uint8_t);
    virtual void setStencilFuncMask(uint8_t);
    virtual void setStencilOpFail(E_STENCIL_OP);
    virtual void setStencilOpZFail(E_STENCIL_OP);
    virtual void setStencilOpZPass(E_STENCIL_OP);
    virtual void setStencilMask(uint8_t);
    virtual void setClearStencil(uint8_t);
    virtual void getRenderState(detail::driver::SRenderState&) const;
    virtual void setRenderState(const detail::driver::SRenderState&);
    virtual void* getTransform(E_TRANSFORMATION_STATE) const;
    virtual void commitCurrentMaterial();
    virtual void commitMaterialRenderer(CMaterialRenderer*);
    virtual bool setRenderTarget(const intrusive_ptr<ITexture>&, int32_t);
    virtual void setViewportImpl(const core::rect<int32_t>&);
    virtual uint32_t getMaxUserClipPlanes() const;
    virtual E_PIXEL_FORMAT getColorFormat() const;
    virtual E_DRIVER_TYPE getDriverType() const;
    virtual intrusive_ptr<IImage> createScreenShot();
    virtual void onMaterialDestroyed(CMaterial*);
    virtual void beginCompile(IVideoDriver::ICompileData*);
    virtual void endCompile();
    virtual intrusive_ptr<IRenderBuffer> createRenderBuffer(const core::dimension2d<int32_t>&, E_PIXEL_FORMAT);
    virtual intrusive_ptr<IRenderTarget> createRenderTarget(const intrusive_ptr<ITexture>&, uint32_t);
    virtual intrusive_ptr<IRenderTarget> createMultipleRenderTarget();
    virtual void restoreShadowState();
    virtual bool drawImpl(const intrusive_ptr<CVertexStreams>&, const CPrimitiveStream&, CDriverBinding**);
    virtual void commitCurrentMaterial(bool);
    virtual void setBlendColor(SColor);
    virtual void setClearColor(SColor);
    virtual void draw2DLine(const core::position2d<int32_t>&, const core::position2d<int32_t>&, SColor);
    virtual void draw2DRectangle(const core::rect<int32_t>&, const core::rect<int32_t>&, const SColor*, const core::rect<int32_t>*);

private:
    core::rect<int32_t> m_scissorStub{};  // placeholder, see getScissor
};

}  // namespace glitch::video
