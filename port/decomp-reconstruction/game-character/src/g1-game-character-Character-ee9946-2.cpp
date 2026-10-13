// Batch g1-game-character-Character-ee9946-2: part 3 of the Character functions (queries on the character
// tables, the combat and quest helpers, the multiplayer and FX helpers, and the AI pointer refresh).
// Declarations come from dh2/Character.hpp. Partial bodies name the missing field, table or callee in a
// PARTIAL comment; the ledger rows in game-character/ledger.d/g1-game-character-Character-ee9946-2.csv repeat it.

#include <cstdint>
#include <cstring>

#include "dh2/Character.hpp"
#include "pydata-accessors/w7_arrays_02.hpp"

namespace {

// CharProperties::PROPS_GetOpacity and PROPS_GetRotationSpeed return the bit pattern of a float (the original
// returns LODWORD of its float result). The bits are reinterpreted here, not converted.
float FloatFromBits(int bits) {
    float value = 0.0f;
    std::memcpy(&value, &bits, sizeof value);
    return value;
}

}  // namespace

// 0x3a3294 _ZNK9Character15GetCharSoundsIdEv
int Character::GetCharSoundsId() const {
    // PARTIAL: the raw id is the CharProperties final-sheet word at Character +0x1018 (see the ledger notes). It is
    // not reachable from Character yet, so the id is taken as unresolved (-1) and the out-of-range fallback 2 is
    // returned. The range check against Arrays::CharSoundsTable::size is kept as in the original.
    const std::int32_t id = -1;
    if (id < 0 || static_cast<std::uint32_t>(id) >= Arrays::CharSoundsTable::size) {
        return 2;
    }
    return id;
}

// 0x3a32d0 _ZNK9Character13GetCharSoundsEv
const void* Character::GetCharSounds() const {
    // PARTIAL: the original returns Arrays::CharSoundsTable::members + 40 * GetCharSoundsId() (file stride 40).
    // The rows are not decoded into host elements yet (pydata Arrays element ABI is not ported), so no row pointer
    // is formed here.
    return nullptr;
}

// 0x3a3300 _ZNK9Character14GetFXFootprintEv
int Character::GetFXFootprint() const {
    // PARTIAL: the row index is the CharProperties final-sheet word at Character +0x1014 (not reachable yet). The
    // value is word +0x0c of CharEffectTable row (24-byte file stride), row 0 when the index is out of range. Neither
    // the index nor the row elements are available, so 0 is returned.
    return 0;
}

// 0x3a3368 _ZNK9Character15GetFXBloodDeathEv
int Character::GetFXBloodDeath() const {
    // PARTIAL: same index as GetFXFootprint (Character +0x1014). The value is word +0x04 of the CharEffectTable row
    // (24-byte file stride), row 0 when out of range. Not available, so 0 is returned.
    return 0;
}

// 0x3a33d0 _ZNK9Character10GetFXBloodEv
int Character::GetFXBlood() const {
    // PARTIAL: same index as GetFXFootprint (Character +0x1014). The value is word +0x08 of the CharEffectTable row
    // (24-byte file stride), row 0 when out of range. Not available, so 0 is returned.
    return 0;
}

// 0x3a3438 _ZNK9Character14GetAttackDelayEv
int Character::GetAttackDelay() const {
    // PARTIAL: the original returns word +0x04 of the Arrays::AITable row (68-byte file stride) for GetCharAIId().
    // AITable rows are not decoded into host elements yet, so 0 is returned.
    return 0;
}

// 0x3a346c _ZNK9Character14HasComboAttackEv
bool Character::HasComboAttack() const {
    // PARTIAL: the original reads word +0x04 of the Arrays::CharAnimTable row (160-byte file stride) for
    // GetCharAnimTableId(), which indexes Arrays::AnimTable (20-byte file stride), and returns true when that row's
    // word +0x10 is 1. Neither table's rows are decoded into host elements yet, so false is returned.
    return false;
}

// 0x3a34f0 _ZNK9Character17GetNumComboAttackEv
int Character::GetNumComboAttack() const {
    // PARTIAL: same table chain as HasComboAttack. The original returns the Arrays::AnimTable row word +0x08 when
    // the word +0x10 flag is 1, otherwise 0. The rows are not available, so 0 is returned.
    return 0;
}

// 0x3a36e4 _ZNK9Character9IsZonableEv
bool Character::IsZonable() const {
    if (IsPlayer() || IsFaerie()) {
        return false;
    }
    return GameObject::MeetCondition();
}

// 0x3a3724 _ZNK9Character10GetOpacityEv
float Character::GetOpacity() const {
    return FloatFromBits(CharProperties::PROPS_GetOpacity());
}

// 0x3a372c _ZNK9Character16GetRotationSpeedEv
float Character::GetRotationSpeed() const {
    // PARTIAL: the original returns -1.0f (bits 0xbf800000, the literal -1082130432) when CharStateMachine
    // m_stateFlags (+0x24, private to CharStateMachine) has bit 0x20 set. That word is not reachable from Character
    // yet, so the bit is taken as clear and the CharProperties value is returned.
    constexpr bool kNoRotationFlag = false;
    if (kNoRotationFlag) {
        return -1.0f;
    }
    return FloatFromBits(CharProperties::PROPS_GetRotationSpeed());
}

// 0x3a374c _ZNK9Character20GetInteractionRadiusEv
float Character::GetInteractionRadius() const {
    return CharAI::AI_GetMeleeRadius();
}

// 0x3a3754 _ZNK9Character19IsAvoidingObstaclesEv
bool Character::IsAvoidingObstacles() const {
    return CharStateMachine::SM_IsMoving(false);
}

// 0x3a3fe4 _ZN9Character23CheckForAllQuestsTrophyEv
void Character::CheckForAllQuestsTrophy() {
    // PARTIAL: the original counts the quests with SG_GetNumQuests(-1). When there are none, or every quest
    // (SG_GetQuestByID(i, -1)) has its first word above 12, it unlocks trophy "quest_allcomplete" through
    // TrophyManager::s_instance and Arrays::GetMemberIDByString<Arrays::TrophyTable>. dh2/Quest.hpp and
    // dh2/TrophyManager.hpp do not exist yet, so the quest test and the unlock are not made here.
    const int numQuests = SG_GetNumQuests(-1);
    (void)numQuests;
}

// 0x3a4068 _ZN9Character26RemoveMultiplayerHighlightEv
void Character::RemoveMultiplayerHighlight() {
    if (m_multiplayerHighlightFX != nullptr) {
        // PARTIAL: the original calls VisualFXManager::DropAnimatedFX(&Singleton<VisualFXManager>::s_inst,
        // &m_multiplayerHighlightFX) first. dh2/VisualFXManager.hpp does not exist yet, so the drop is not made.
        m_multiplayerHighlightFX = nullptr;
    }
}

// 0x3a40b0 _ZN9Character13DisableSelfFXEv
void Character::DisableSelfFX() {
    if (m_selfFX != nullptr) {
        // PARTIAL: the original returns VisualFXManager::DropAnimatedFX(&Singleton<VisualFXManager>::s_inst,
        // &m_selfFX). dh2/VisualFXManager.hpp does not exist yet, so the drop is not made.
    }
}

// 0x3a40e4 _ZN9Character14DisableStateFXEv
void Character::DisableStateFX() {
    if (m_stateFX != nullptr) {
        // PARTIAL: the original returns VisualFXManager::DropAnimatedFX(&Singleton<VisualFXManager>::s_inst,
        // &m_stateFX). dh2/VisualFXManager.hpp does not exist yet, so the drop is not made.
    }
}

// 0x3a41a0 _ZN9Character23AddMultiplayerHighlightEv
void Character::AddMultiplayerHighlight() {
    if (m_multiplayerHighlightFX != nullptr) {
        return;
    }
    // PARTIAL: with no highlight yet, the original (1) gets this character's player record through
    // PlayerManager::GetPlayerByCharacter (declared in dh2/PlayerManager.hpp) and reads its slot word (+0x678, 0..3),
    // (2) scans the names of Arrays::AnimatedEffectTable from index 0 for "multiplayer_player01_glow", (3) grabs the
    // effect at the found index plus the slot through VisualFXManager::GrabAnimFX and stores it in
    // m_multiplayerHighlightFX, (4) copies Vec3f_Origin into the effect words +0x34..+0x3c, runs AnimatedFX::SyncIrrData
    // and SetVisible, stores this character at effect +0x28 and syncs again, and (5) calls two virtual slots of the
    // effect's animator (+0x44 and +0x40). dh2/PlayerInfo.hpp (the record and its slot word) and dh2/VisualFXManager.hpp
    // do not exist yet, Vec3f_Origin has no declaration, and AnimatedFX +0x28 and +0x34..+0x3c are unnamed, so none of
    // these steps are made here.
}

// 0x3a4344 _ZN9Character16UpdateAIPointersEv
void Character::UpdateAIPointers() {
    CharAI::UpdatePointers();
    if (m_objectOfInterest != nullptr && m_objectOfInterest->m_markedForDeletion != 0) {
        m_objectOfInterest = nullptr;
    }
    if (m_killer != nullptr && m_killer->m_markedForDeletion != 0) {
        m_killer = nullptr;
    }
}

// 0x3a4398 _ZN9Character15SetRelativeAABBERK4aabbIfEb
void Character::SetRelativeAABB(const aabb<float>& box, bool update) {
    // PARTIAL: the original stores the six floats of box in the GameObject local AABB (m_relAabb, +0x144). When update
    // is false it first scales all six by (the Character +0x1038 percent word, CharProperties final-sheet word, not
    // reachable yet) * 0.01f. That scale is not applied here, so only the GameObject call is made.
    GameObject::SetRelativeAABB(box, update);
}
