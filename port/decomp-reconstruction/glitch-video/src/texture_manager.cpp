#include "glitch-video/texture_manager.hpp"

#include <algorithm>

namespace glitch::video {

// 0x5ea050 _ZN6glitch5video15CTextureManagerD2Ev / 0x5ea138 D1
// clearPlaceHolders(), removeAll() on the collection, then the explicit drop of every loader (each loader carries the
// extra use that addExternalImageLoader and the constructor give it, and the vector's destructor drops the other) and
// of every writer, and the file system. Partial: the SIDedCollection::removeAll step is not modelled.
CTextureManager::~CTextureManager() {
    clearPlaceHolders();
    for (const boost::intrusive_ptr<IImageLoader>& loader : m_externalImageLoaders) {
        if (loader) loader->drop();  // the explicit loop of the original; the vector's destructor drops the second use
    }
}

// 0x5e8260 _ZN6glitch5video15CTextureManager16clearPlaceHolderENS0_14E_TEXTURE_TYPEEPNS0_8ITextureE
void CTextureManager::clearPlaceHolder(int32_t type, ITexture* texture) {
    if (m_placeHolders[type] == texture) {
        m_placeHolders[type] = nullptr;
    }
    if (m_placeHolders[type + 4] == texture) {
        m_placeHolders[type + 4] = nullptr;
    }
}

// 0x5e828c _ZN6glitch5video15CTextureManager17clearPlaceHoldersEv
void CTextureManager::clearPlaceHolders() {
    for (ITexture*& slot : m_placeHolders) {
        if (slot != nullptr && slot->referenceCounter == 1) {
            slot = nullptr;
        }
    }
}

// 0x5e90e0 _ZN6glitch5video15CTextureManager22addExternalImageLoaderERKN5boost13intrusive_ptrINS0_12IImageLoaderEEE
// The original grabs the loader by hand and then copies it into the vector: two uses, which the destructor's explicit
// loop and the vector's destructor give back.
void CTextureManager::addExternalImageLoader(const boost::intrusive_ptr<IImageLoader>& loader) {
    if (loader.get() == nullptr) return;
    ++loader->referenceCounter;
    m_externalImageLoaders.push_back(loader);
}

// 0x5eaa20 _ZN6glitch5video15CTextureManager22addExternalImageWriterEPNS0_12IImageWriterE
void CTextureManager::addExternalImageWriter(IImageWriter* writer) {
    if (writer == nullptr) return;
    m_externalImageWriters.push_back(boost::intrusive_ptr<IImageWriter>(writer, true));  // adds the use the original adds by hand
}

// 0x5e8dd4 _ZN6glitch5video15CTextureManager23markTextureAsUnloadableERKN5boost13intrusive_ptrINS0_8ITextureEEE
void CTextureManager::markTextureAsUnloadable(const boost::intrusive_ptr<ITexture>& texture) {
    ITexture* raw = texture.get();
    if (std::find(m_textures.begin(), m_textures.end(), raw) == m_textures.end()) {
        m_textures.push_back(raw);
    }
}

// 0x3849a0 _ZN6glitch5video15CTextureManager13removeTextureEPNS0_8ITextureE
int32_t CTextureManager::removeTexture(ITexture* texture) {
    if (texture == nullptr) {
        return 0;
    }
    int32_t removed = 0;
    auto it = std::find(m_textures.begin(), m_textures.end(), texture);
    if (it != m_textures.end()) {
        m_textures.erase(it);
        removed = 1;
    }
    if (removed != 0) {
        clearPlaceHolder(static_cast<int32_t>(texture->m_flags38 & 3), texture);
    }
    return removed;
}

// 0x384eb4 _ZN6glitch5video15CTextureManager13removeTextureERN5boost13intrusive_ptrINS0_8ITextureEEE
// Clears the smart pointer (dropping the caller's use) and removes the texture. Partial: returns the count, not the ITexture*.
uint32_t CTextureManager::removeTexture(boost::intrusive_ptr<ITexture>& texture) {
    ITexture* raw = texture.get();
    if (raw == nullptr) {
        return 0;
    }
    texture = boost::intrusive_ptr<ITexture>();  // drops the caller's use
    return static_cast<uint32_t>(removeTexture(raw));
}

}  // namespace glitch::video
