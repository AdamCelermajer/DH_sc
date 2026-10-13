// glitch::video::CTextureManager: the texture collection of the driver (a SIDedCollection of ITexture), the placeholder
// textures, the external image loaders and writers, and the texture-creation front end. Canonical definition (engine
// unification wave u1-video); the forward declarations in glitch-scene-collada-gui and game/ are replaced by this header.
// The SIDedCollection base and most creation functions are not reconstructed; the members that are modelled keep their
// original offsets in the comments.
//
// Layout of the original object (32-bit; constructors 0x5eaa7c C2 / 0x5eaed8 C1 with an IVideoDriver*, size 0x78):
//   +0x00  SIDedCollection<intrusive_ptr<ITexture>, ushort, false, texturemanager::STextureProperties, SValueTraits> (0x28 bytes)
//   +0x28  IVideoDriver*
//   +0x2c  .. +0x3c  the file system / image-loader state used by writeImageToFile and getImageLoader
//   +0x30  vector<intrusive_ptr<IImageLoader>>    +0x40  vector<IImageWriter*> (external writers, counted)
//   +0x48  placeholder slots (8 pointers, +0x48..+0x64): index type for the first group, type + 4 for the second
//   +0x68  vector of the unloadable textures (begin / end / capacity at +0x68 / +0x6c / +0x70)
//   +0x74  unnamed
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "glitch-video/deps/intrusive_ptr.hpp"
#include "glitch-video/image.hpp"
#include "glitch-video/image_loaders.hpp"
#include "glitch-video/texture.hpp"
#include "glitch-video/texture_loaders.hpp"
#include "glitch-video/video_types.hpp"

namespace glitch::video {

class CTextureManager {
public:
    using E_PLACE_HOLDER = int32_t;
    struct SIterator;  // the SIDedCollection iterator of rmReloadDataTexture

    // Not part of the original (which constructs through the IVideoDriver constructor): lets a test build a manager.
    CTextureManager() = default;
    // 0x5eaa7c (C1) / 0x5eaed8 (C2): the SIDedCollection base, the file system of the driver's device, the bundled image
    // loaders (BMP, JPG, TGA, ATC, PNG, DDS, PVR) and writers (JPG, TGA, PNG) and the placeholder table. Declared only.
    explicit CTextureManager(IVideoDriver* driver);
    // 0x5ea050 (D2) / 0x5ea138 (D1): clearPlaceHolders(), then removeAll() on the collection and the releases of the loader
    // and writer lists and of the file system. Partial: the collection is not modelled; the members release themselves.
    ~CTextureManager();

    // 0x5e8260: clears the placeholder for a texture type when it matches the texture.
    void clearPlaceHolder(int32_t type, ITexture* texture);
    // 0x5e828c: clears every placeholder whose texture is referenced only by the placeholder.
    void clearPlaceHolders();
    // 0x5e90e0: grabs the loader by hand and appends it (two uses; the destructor gives both back).
    void addExternalImageLoader(const boost::intrusive_ptr<IImageLoader>& loader);
    // 0x5eaa20: appends a writer and bumps its reference count (the original keeps raw pointers in a std::vector and
    // drops them in an explicit loop of the destructor; one use per entry either way).
    void addExternalImageWriter(IImageWriter* writer);
    // 0x3849a0: removes the texture from the list and clears its placeholder. Returns 1 when
    // it was present. Partial: the original removes by id from the SIDedCollection; the
    // port's list is a plain vector, so presence in the list stands for the id removal.
    int32_t removeTexture(ITexture* texture);
    // 0x5e8dd4: appends the texture to the m_textures list when it is not already in it.
    void markTextureAsUnloadable(const boost::intrusive_ptr<ITexture>& texture);
    // 0x384eb4: clears the smart pointer (dropping the caller's use) and removes the texture. Partial: see the pointer form.
    uint32_t removeTexture(boost::intrusive_ptr<ITexture>& texture);

    // ---- the rest of the queue, declared only ----
    // Placeholders: 0x5ea228 / 0x5ec1b8 (free-function form of setPlaceHolder in misc_wave4_b.hpp).
    void setPlaceHolder(E_PLACE_HOLDER placeHolder, const boost::intrusive_ptr<ITexture>& texture, E_TEXTURE_TYPE type);
    boost::intrusive_ptr<ITexture> getPlaceHolder(E_PLACE_HOLDER placeHolder, E_TEXTURE_TYPE type);
    // Images: 0x5e86f4 / 0x5e8740 / 0x5e8788 / 0x5e87d0 / 0x5e913c / 0x5e9198 / 0x5e8144 / 0x5e82c0.
    boost::intrusive_ptr<CImage> createImage(const boost::intrusive_ptr<CImage>& source, const core::position2d<int32_t>& position,
                                             const core::dimension2d<int32_t>& size);
    boost::intrusive_ptr<CImage> createImage(E_PIXEL_FORMAT format, const boost::intrusive_ptr<CImage>& source);
    boost::intrusive_ptr<CImage> createImage(E_PIXEL_FORMAT format, const core::dimension2d<int32_t>& size);
    boost::intrusive_ptr<CImage> createImageFromData(E_PIXEL_FORMAT format, const core::dimension2d<int32_t>& size, void* data,
                                                     bool copy, bool own);
    boost::intrusive_ptr<CImage> createImageFromFile(io::IReadFile* file);
    boost::intrusive_ptr<CImage> createImageFromFile(const char* filename);
    boost::intrusive_ptr<IImageLoader> getImageLoader(io::IReadFile* file) const;
    bool writeImageToFile(const boost::intrusive_ptr<CImage>& image, const char* filename, uint32_t flags);
    // Textures: 0x5e8f2c / 0x5e94a8 / 0x5e9908 / 0x5ea2a8 / 0x5ea764 / 0x5ea7f8 / 0x5ea8f8 / 0x5ec460 / 0x5ecaa4 / 0x5ecba4 /
    // 0x5ecf34 / 0x5ed0c4 / 0x5ed210 / 0x5ed380 / 0x5eb334 / 0x5ebd7c / 0x5ebf8c / 0x5e97e0.
    boost::intrusive_ptr<ITexture> findTexture(const char* name) const;
    void renameTexture(const boost::intrusive_ptr<ITexture>& texture, const char* name);
    uint16_t getHashName(const char* name) const;
    boost::intrusive_ptr<ITexture> getTexture(const char* name, bool flag, core::SScopedProcessArray<char>& buffer) const;
    boost::intrusive_ptr<ITexture> getTexture(io::IReadFile* file, const char* name, bool flag);
    boost::intrusive_ptr<ITexture> getTexture(const char* name, const char* alias);
    boost::intrusive_ptr<ITexture> getTextureInternal(io::IReadFile* file, const std::string& name, bool flag);
    boost::intrusive_ptr<ITexture> addTexture(const boost::intrusive_ptr<ITexture>& texture, E_PIXEL_FORMAT format, const char* name);
    boost::intrusive_ptr<ITexture> addTexture(const char* name, const STextureDesc& desc, bool flag);
    boost::intrusive_ptr<ITexture> addTexture(const core::dimension2d<int32_t>& size, const char* name, E_PIXEL_FORMAT format,
                                              bool flag);
    boost::intrusive_ptr<ITexture> addTexture(const char* name, const boost::intrusive_ptr<CImage>& image, bool flag,
                                              E_TEXTURE_LAYOUT layout);
    boost::intrusive_ptr<ITexture> createTextureFromImage(const char* name, const boost::intrusive_ptr<CImage>& image,
                                                          E_TEXTURE_LAYOUT layout);
    boost::intrusive_ptr<ITexture> loadTextureFromFile(io::IReadFile* file, const char* name, E_PIXEL_FORMAT& format, bool flag);
    void rmReloadDataTexture(SIterator iterator, const char* name);
    void makeNormalMapTexture(const boost::intrusive_ptr<ITexture>& texture, float amplitude) const;
    void makeColorKeyTexture(const boost::intrusive_ptr<ITexture>& texture, core::position2d<int32_t> keyPixel) const;
    void makeColorKeyTexture(const boost::intrusive_ptr<ITexture>& texture, SColor key) const;
    void clearDriverSpecificResources();

    // Layout (32-bit, from the constructor 0x5eaa7c and the destructor 0x5ea050): +0x00..+0x27 is the
    // SIDedCollection<intrusive_ptr<ITexture>, ushort, false, STextureProperties, SValueTraits> base (not modelled).
    IVideoDriver* m_driver = nullptr;                                         // +0x28
    boost::intrusive_ptr<glitch::IReferenceCounted> m_fileSystem;            // +0x2c, io::IFileSystem (driver device +0x34)
    std::vector<boost::intrusive_ptr<IImageLoader>> m_externalImageLoaders;  // +0x30..+0x38
    std::vector<boost::intrusive_ptr<IImageWriter>> m_externalImageWriters;  // +0x3c..+0x44
    ITexture* m_placeHolders[8] = {};                                         // +0x48..+0x64; index type, second group type + 4
    std::vector<ITexture*> m_textures;                                       // +0x68..+0x70, raw texture pointers
    uint32_t unk_74 = 67;                                                     // +0x74, 67 after construction
};

}  // namespace glitch::video
