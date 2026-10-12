#include "glitch-video/texture_manager.hpp"

namespace glitch::video {

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
        if (slot != nullptr && slot->m_refCount == 1) {
            slot = nullptr;
        }
    }
}

// 0x5e90e0 _ZN6glitch5video15CTextureManager22addExternalImageLoaderERKN5boost13intrusive_ptrINS0_12IImageLoaderEEE
void CTextureManager::addExternalImageLoader(const boost::intrusive_ptr<IImageLoader>& loader) {
    if (loader.get() == nullptr) return;
    m_externalImageLoaders.push_back(loader);
}

// 0x5eaa20 _ZN6glitch5video15CTextureManager22addExternalImageWriterEPNS0_12IImageWriterE
void CTextureManager::addExternalImageWriter(IImageWriter* writer) {
    if (writer == nullptr) return;
    ++writer->m_refCount;
    m_externalImageWriters.push_back(writer);
}

}  // namespace glitch::video
