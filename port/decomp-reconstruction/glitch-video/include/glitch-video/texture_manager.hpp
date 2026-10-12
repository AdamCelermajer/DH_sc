// glitch::video::CTextureManager: owns placeholder textures and the external
// image loader and writer lists. The texture collection (SIDedCollection) is
// not reconstructed, so lookups are not modelled here.
#pragma once

#include <cstdint>
#include <vector>

#include "glitch-video/deps/intrusive_ptr.hpp"
#include "glitch-video/texture.hpp"
#include "glitch-video/texture_loaders.hpp"

namespace glitch::video {

// Stand-in for glitch::video::IImageWriter. Only the reference count at +0x04 is
// used here.
class IImageWriter {
public:
    uint32_t m_refCount = 0;  // +0x04
};

class CTextureManager {
public:
    // 0x5e8260: clears the placeholder for a texture type when it matches the texture.
    void clearPlaceHolder(int32_t type, ITexture* texture);
    // 0x5e828c: clears every placeholder whose texture is referenced only by the placeholder.
    void clearPlaceHolders();
    // 0x5e90e0: appends a loader. The original also bumps the loader's reference count,
    // which the stand-in boost pointer does not model. Partial.
    void addExternalImageLoader(const boost::intrusive_ptr<IImageLoader>& loader);
    // 0x5eaa20: appends a writer and bumps its reference count (+0x04).
    void addExternalImageWriter(IImageWriter* writer);
    // 0x3849a0: removes the texture from the list and clears its placeholder. Returns 1 when
    // it was present. Partial: the original removes by id from the SIDedCollection; the
    // port's list is a plain vector, so presence in the list stands for the id removal.
    int32_t removeTexture(ITexture* texture);
    // 0x5e8dd4: appends the texture to the m_textures list when it is not already in it.
    void markTextureAsUnloadable(const boost::intrusive_ptr<ITexture>& texture);
    // 0x384eb4: clears the smart pointer and removes the texture. The reference drop is not
    // modelled (the stand-in pointer does not count references). Partial.
    uint32_t removeTexture(boost::intrusive_ptr<ITexture>& texture);

    // +0x48: placeholder slots; index type for the first group, type + 4 for the second.
    ITexture* m_placeHolders[8] = {};
    std::vector<boost::intrusive_ptr<IImageLoader>> m_externalImageLoaders;  // +0x30
    std::vector<IImageWriter*> m_externalImageWriters;                       // +0x40
    std::vector<ITexture*> m_textures;                                       // +0x68..+0x6c, raw texture pointers
};

}  // namespace glitch::video
