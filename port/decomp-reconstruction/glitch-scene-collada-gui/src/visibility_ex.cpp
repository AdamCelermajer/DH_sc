#include "glitch-scene-collada-gui/visibility_ex.hpp"

namespace glitch::collada::animation_track::visibility_ex {

// 0x6e493c _ZNK6glitch7collada15animation_track13CVisibilityEx13retrieveValueEPvS3_
void retrieveValue(const CVisibilityEx* self, void* /*unused*/, std::uint8_t* out) {
    *out = static_cast<std::uint8_t>(self->visible_flags & 1u);
}

// 0x6e494c _ZNK6glitch7collada15animation_track13CVisibilityEx12getValueSizeEv
int getValueSize() {
    return 4;
}

// 0x6e4954 _ZNK6glitch7collada15animation_track13CVisibilityEx16getIdentityValueEPv
void getIdentityValue(const CVisibilityEx* /*self*/, std::uint32_t* out) {
    *out = 1;
}

}  // namespace glitch::collada::animation_track::visibility_ex
