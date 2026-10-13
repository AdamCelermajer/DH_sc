// Virtual slots of the image loaders and writers whose bodies are not reconstructed yet. They exist so that the
// loader classes (which now derive from IImageLoader / IImageWriter, with their full vtables) can be constructed; each
// stops with detail::unreconstructed(). Their ledger rows keep their earlier status.
#include "glitch-video/image_loaders.hpp"
#include "glitch-video/texture_loaders.hpp"
#include "glitch-video/unreconstructed.hpp"

namespace glitch::video {

// 0x602c94 _ZNK6glitch5video12IImageLoader15loadTextureDataEPNS_2io9IReadFileERKN5boost13intrusive_ptrINS0_8ITextureEEERKNS0_12STextureDescE
// The base has no texture load interface: it returns 0.
bool IImageLoader::loadTextureData(io::IReadFile*, const boost::intrusive_ptr<ITexture>&, const STextureDesc&) const {
    return false;
}

// loadImage slots (+0x14).
boost::intrusive_ptr<CImage> CImageLoaderBMP::loadImage(io::IReadFile*) const {
    detail::unreconstructed("CImageLoaderBMP::loadImage", 0x6034ec);
}
boost::intrusive_ptr<CImage> CImageLoaderTGA::loadImage(io::IReadFile*) const {
    detail::unreconstructed("CImageLoaderTGA::loadImage", 0x606368);
}
boost::intrusive_ptr<CImage> CImageLoaderPng::loadImage(io::IReadFile*) const {
    detail::unreconstructed("CImageLoaderPng::loadImage", 0x6050dc);
}
boost::intrusive_ptr<CImage> CImageLoaderJPG::loadImage(io::IReadFile*) const {
    detail::unreconstructed("CImageLoaderJPG::loadImage", 0x604be0);
}
boost::intrusive_ptr<CImage> CImageLoaderPVR::loadImage(io::IReadFile*) const {
    detail::unreconstructed("CImageLoaderPVR::loadImage", 0x605f80);
}
boost::intrusive_ptr<CImage> CImageLoaderDDS::loadImage(io::IReadFile*) const {
    detail::unreconstructed("CImageLoaderDDS::loadImage", 0x6046f4);
}
boost::intrusive_ptr<CImage> CImageLoaderATC::loadImage(io::IReadFile*) const {
    detail::unreconstructed("CImageLoaderATC::loadImage", 0x602fe4);
}

// loadTextureHeader / loadTextureData slots of the texture-capable loaders.
bool CImageLoaderDDS::loadTextureHeader(io::IReadFile*, STextureDesc&) const {
    detail::unreconstructed("CImageLoaderDDS::loadTextureHeader", 0x604398);
}
bool CImageLoaderDDS::loadTextureData(io::IReadFile*, const boost::intrusive_ptr<ITexture>&, const STextureDesc&) const {
    detail::unreconstructed("CImageLoaderDDS::loadTextureData", 0x60496c);
}
bool CImageLoaderATC::loadTextureData(io::IReadFile*, const boost::intrusive_ptr<ITexture>&, const STextureDesc&) const {
    detail::unreconstructed("CImageLoaderATC::loadTextureData", 0x6032dc);
}
bool CImageLoaderPVR::loadTextureData(io::IReadFile*, const boost::intrusive_ptr<ITexture>&, const STextureDesc&) const {
    detail::unreconstructed("CImageLoaderPVR::loadTextureData", 0x60623c);
}

// writeImage slots (+0x10).
bool CImageWriterJPG::writeImage(io::IWriteFile*, const boost::intrusive_ptr<CImage>&, uint32_t) const {
    detail::unreconstructed("CImageWriterJPG::writeImage", 0x606a34);
}
bool CImageWriterPNG::writeImage(io::IWriteFile*, const boost::intrusive_ptr<CImage>&, uint32_t) const {
    detail::unreconstructed("CImageWriterPNG::writeImage", 0x606dc0);
}
bool CImageWriterTGA::writeImage(io::IWriteFile*, const boost::intrusive_ptr<CImage>&, uint32_t) const {
    detail::unreconstructed("CImageWriterTGA::writeImage", 0x607370);
}

}  // namespace glitch::video
