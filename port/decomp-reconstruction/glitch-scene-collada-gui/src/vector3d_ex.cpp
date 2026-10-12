#include "glitch-scene-collada-gui/vector3d_ex.hpp"

namespace glitch::collada::animation_track {

// 0x6e42bc _ZNK6glitch7collada15animation_track11CVector3dEx16getIdentityValueEPv
void CVector3dEx_getIdentityValue(float* out) {
    out[0] = 0.0f;
    out[1] = 0.0f;
    out[2] = 0.0f;
}

// 0x6e4420 _ZNK6glitch7collada15animation_track11CVector3dEx10applyValueEPvS3_PNS1_15CApplicatorInfoE
// Original copies getValueSize() bytes from value to target (applicator unused).
void CVector3dEx_applyValue(const CVector3dEx* self, const void* value, void* target) {
    std::memcpy(target, value, self->getValueSize());
}

}  // namespace glitch::collada::animation_track
