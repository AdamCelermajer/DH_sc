// Batch g1-game-character-Character-ee9946-0: the Character constant predicates and getters, the
// virtual-dispatch camera check, the cached-OID lookup, and the FX scale and loot getters.
// Declarations come from dh2/Character.hpp.
//
// Partial functions read words that the Character header cannot name yet: the flag word at +0x520
// (CharStateMachine +0x24, private there), the AI target at +0x408 (CharAI +0x40, private there), and the
// raw CharProperties sheet words at +0x5b0 and +0x101c. Each partial body is a placeholder and says so.

#include "dh2/Character.hpp"

// 0x3a2e1c _ZNK9Character11IsCharacterEv
bool Character::IsCharacter() const {
    return true;
}

// 0x3a2e24 _ZNK9Character14GetFocusObjectEv
// partial. The original returns the word at Character +0x408, which is CharAI +0x40 (CharAI::m_target).
// CharAI keeps m_target private and has no public getter, so this returns nullptr until the layout request
// (a public CharAI accessor for m_target) is applied.
GameObject* Character::GetFocusObject() const {
    return nullptr;
}

// 0x3a2e2c _ZNK9Character14IsUpdatingPathEv
// partial. The original returns bit 0x80 of the word at Character +0x520 (see the file comment). Placeholder
// false until that word is readable from Character.
bool Character::IsUpdatingPath() const {
    return false;
}

// 0x3a2e38 _ZNK9Character28IsUpdatingPositionFromVisualEv
// partial. The original returns bit 0x01 of the word at Character +0x520. Placeholder false.
bool Character::IsUpdatingPositionFromVisual() const {
    return false;
}

// 0x3a2e44 _ZNK9Character29IsUpdatingPositionFromPhysicsEv
// partial. The original returns bit 0x02 of the word at Character +0x520. Placeholder false.
bool Character::IsUpdatingPositionFromPhysics() const {
    return false;
}

// 0x3a2e50 _ZNK9Character28IsUpdatingRotationFromVisualEv
// partial. The original returns bit 0x04 of the word at Character +0x520. Placeholder false.
bool Character::IsUpdatingRotationFromVisual() const {
    return false;
}

// 0x3a2e5c _ZNK9Character29IsUpdatingRotationFromPhysicsEv
// partial. The original returns bit 0x08 of the word at Character +0x520. Placeholder false.
bool Character::IsUpdatingRotationFromPhysics() const {
    return false;
}

// 0x3a2e68 _ZNK9Character28IsUpdatingVisualWithRotationEv
// partial. The original returns the inverse of bit 0x10 of the word at Character +0x520 (EOR 0x10, then
// bit 4). Placeholder false.
bool Character::IsUpdatingVisualWithRotation() const {
    return false;
}

// 0x3a2e78 _ZNK9Character25IsValidatingFloorPositionEv
// partial. The original returns the inverse of bit 0x40 of the word at Character +0x520 (EOR 0x40, then
// bit 6), so 1 when the bit is clear. Placeholder 0.
int Character::IsValidatingFloorPosition() const {
    return 0;
}

// 0x3a2e88 _ZNK9Character24IsValidatingCameraLimitsEv
// The original loads the vptr and calls the slot at byte offset 0x28 (IsPlayer) through it. That is the
// same dispatch as a virtual call to IsPlayer().
bool Character::IsValidatingCameraLimits() const {
    return IsPlayer();
}

// 0x3a2e9c _ZNK9Character10GetFXScaleEv
// partial. The original converts the int at Character +0x5b0 to float (__aeabi_i2f) and multiplies it by
// 0.01f (raw word 0x3C23D70A), then returns the result in all three components. The int lies in the
// CharProperties base sheet and has no named accessor, so this returns the GameObject scale (m_scale) as a
// placeholder until the layout request is applied.
Point3D<float> Character::GetFXScale() const {
    return m_scale;
}

// 0x3a2ecc _ZNK9Character15GetFXScaleValueEv
// partial. Returns the int at Character +0x5b0 (see GetFXScale). Placeholder -1, the GameObject default.
int Character::GetFXScaleValue() const {
    return -1;
}

// 0x3a2ed4 _ZNK9Character6IsDeadEv
bool Character::IsDead() const {
    return m_dead != 0;
}

// 0x3a2ee0 _ZNK9Character10IsAnimatedEv
bool Character::IsAnimated() const {
    return true;
}

// 0x3a2ee8 _ZNK9Character10IsObstacleEv
bool Character::IsObstacle() const {
    return true;
}

// 0x3a2ef0 _ZNK9Character17GetObstacleRadiusEv
// The original returns the float whose bit pattern is 0x42480000, which is 50.0f.
float Character::GetObstacleRadius() const {
    return 50.0f;
}

// 0x3a2efc _ZNK9Character19GetObstacleStrengthEv
// The original returns the float whose bit pattern is 0x41A00000, which is 20.0f.
float Character::GetObstacleStrength() const {
    return 20.0f;
}

// 0x3a2f08 _ZNK9Character13getUDTypeNameEv
const char* Character::getUDTypeName() const {
    return "Character";
}

// 0x3a2f48 _ZN9Character17HasCharOIDInCacheEi
// The original walks the red-black tree of s_cachedCharOIDs with a lower_bound search and then checks
// the key for equality. It returns the stored value, or 0 when the key is absent. That is std::map::find.
int Character::HasCharOIDInCache(int oid) {
    const auto it = s_cachedCharOIDs.find(oid);
    if (it == s_cachedCharOIDs.end()) {
        return 0;
    }
    return static_cast<int>(it->second);
}

// 0x3a2fcc _ZNK9Character7GetLootEv
// partial. The original returns the int at Character +0x101c, which lies in the CharProperties base
// (its m_props sheet, sheet offset 0x28). It has no named accessor yet, so this returns -1 as a
// placeholder until the layout request is applied.
int Character::GetLoot() const {
    return -1;
}
