#include "glitch-video/misc_wave4_b.hpp"

namespace glitch::video::misc_wave4_b {

// 0x5ea228 _ZN6glitch5video15CTextureManager14setPlaceHolderENS1_14E_PLACE_HOLDERERKN5boost13intrusive_ptrINS0_8ITextureEEENS0_14E_TEXTURE_TYPEE
// Free-function form, see the header. A null placeholder slot that the original would dereference
// (the reference count read after the first removal) is skipped here; the original has no check.
void setPlaceHolder(CTextureManager& manager, int32_t placeHolder,
                    const boost::intrusive_ptr<ITexture>& texture, uint32_t textureType) {
    ITexture* raw = texture.get();
    uint32_t type = textureType;
    if (type == kTextureTypeFromTexture) {
        if (raw == nullptr) {
            return;
        }
        type = raw->m_flags38 & 3;  // format bits of the texture
    }

    const uint32_t index = 4u * static_cast<uint32_t>(placeHolder) + type;
    if (index >= sizeof(manager.m_placeHolders) / sizeof(manager.m_placeHolders[0])) {
        return;  // outside the table; the original would write past it
    }

    ITexture* old = manager.m_placeHolders[index];
    if (old != nullptr) {
        manager.removeTexture(old);
        old = manager.m_placeHolders[index];
    }
    if (old != nullptr && old->referenceCounter == 1) {
        manager.removeTexture(old);
    }
    manager.m_placeHolders[index] = raw;
}

}  // namespace glitch::video::misc_wave4_b
