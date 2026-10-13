// Batch g1-game-app-global-f216b6-2 (<global>, part 3 of 22).
// The object factories GetNewInstance<T>() for TriggerTrap to AnimatedDecor (declared in dh2/global/object_factory.hpp),
// ToLowerCase(char*, int, int), GetApplicator(glitch::scene::ISceneNodeAnimator*) and
// CF_SetCombatants(Character*, Character*, int, bool, bool) with the combat state it writes.
// IsMultiplayerGame() is in g1-game-app-global-f216b6-2_players.cpp: dh2/Character.hpp (pydata IStreamBase) and
// dh2/PlayerManager.hpp (net-transport IStreamBase) define the same class differently, so one translation unit cannot
// include both. The split follows g1-game-app-global-f216b6-1_objects.cpp.
// Declarations: dh2/global/object_factory.hpp, dh2/global/ToLowerCase.hpp, dh2/global/GetApplicator.hpp and
// dh2/global/CombatFormulas.hpp.
// Constructor constants come from the asm (MOV R1,#n before each constructor BL), since the Hex-Rays listing drops
// some of them (Module passes 5 there).
#include <cstddef>
#include <cstring>

#include "dh2/global/object_factory.hpp"
#include "dh2/global/ToLowerCase.hpp"
#include "dh2/global/GetApplicator.hpp"
#include "dh2/global/CombatFormulas.hpp"

#include "dh2/Character.hpp"

// ---------------------------------------------------------------------------------------------------------------
// Object factories. PARTIAL stubs: none of the class headers exists in game/include/dh2 yet (AnimatedDecor.hpp is
// present but includes the absent dh2/Decor.hpp). Each original is operator new(size, hint 0), then the class
// constructor with the GO_IDS shown; replace the stub with that call when the header lands. The GameObject classes
// (ColBox, Floor, Dummy, Decor, AnimatedDecor) have no separate constructor in the pseudocode: the asm calls
// GameObject(GO_IDS 20) and then writes the class's vtables and members inline, so their stubs wait on the class header
// that holds that code.
// ---------------------------------------------------------------------------------------------------------------

// 0x340e7c ObjectBase* GetNewInstance<TriggerTrap>()
// Original: operator new(1028), then TriggerTrap(GO_IDS 15). PARTIAL: waits on dh2/TriggerTrap.hpp.
template <>
ObjectBase* GetNewInstance<TriggerTrap>() {
    return nullptr;
}

// 0x340ea0 ObjectBase* GetNewInstance<TriggerZoneExitLevel>()
// Original: operator new(2080), then TriggerZoneExitLevel(GO_IDS 14). PARTIAL: waits on dh2/TriggerZoneExitLevel.hpp.
template <>
ObjectBase* GetNewInstance<TriggerZoneExitLevel>() {
    return nullptr;
}

// 0x340ec4 ObjectBase* GetNewInstance<TriggerZone>()
// Original: operator new(2008), then TriggerZone(GO_IDS 20). PARTIAL: waits on dh2/TriggerZone.hpp.
template <>
ObjectBase* GetNewInstance<TriggerZone>() {
    return nullptr;
}

// 0x340ee8 ObjectBase* GetNewInstance<TriggerPlate>()
// Original: operator new(1920), then TriggerPlate(GO_IDS 18). PARTIAL: waits on dh2/TriggerPlate.hpp.
template <>
ObjectBase* GetNewInstance<TriggerPlate>() {
    return nullptr;
}

// 0x340f0c ObjectBase* GetNewInstance<TriggerObject>()
// Original: operator new(1936), then TriggerObject() with no GO_IDS. PARTIAL: waits on dh2/TriggerObject.hpp.
template <>
ObjectBase* GetNewInstance<TriggerObject>() {
    return nullptr;
}

// 0x340f2c ObjectBase* GetNewInstance<CheckpointZone>()
// Original: operator new(928), then CheckpointZone(GO_IDS 12). PARTIAL: waits on dh2/CheckpointZone.hpp.
template <>
ObjectBase* GetNewInstance<CheckpointZone>() {
    return nullptr;
}

// 0x340f50 ObjectBase* GetNewInstance<QuestMoveInZone>()
// Original: operator new(904), then QuestMoveInZone(GO_IDS 20). PARTIAL: waits on dh2/QuestMoveInZone.hpp.
template <>
ObjectBase* GetNewInstance<QuestMoveInZone>() {
    return nullptr;
}

// 0x340f74 ObjectBase* GetNewInstance<RoomZone>()
// Original: operator new(924), then RoomZone(GO_IDS 11). PARTIAL: waits on dh2/RoomZone.hpp.
template <>
ObjectBase* GetNewInstance<RoomZone>() {
    return nullptr;
}

// 0x340f98 ObjectBase* GetNewInstance<Module>()
// Original: operator new(1052), then Module(GO_IDS 5). The Hex-Rays listing shows Module(v0) with no id. PARTIAL: waits on dh2/Module.hpp.
template <>
ObjectBase* GetNewInstance<Module>() {
    return nullptr;
}

// 0x340fbc ObjectBase* GetNewInstance<Billboard>()
// Original: operator new(380), then Billboard(GO_IDS 20). PARTIAL: waits on dh2/Billboard.hpp.
template <>
ObjectBase* GetNewInstance<Billboard>() {
    return nullptr;
}

// 0x340fe0 ObjectBase* GetNewInstance<ColBox>()
// Original: operator new(896), GameObject(GO_IDS 20), then the ColBox vtables, m_static (+0x84) = 1 and the
// members at +0x374, +0x378 and +0x37c set to 0. PARTIAL: waits on dh2/ColBox.hpp for its constructor.
template <>
ObjectBase* GetNewInstance<ColBox>() {
    return nullptr;
}

// 0x34104c ObjectBase* GetNewInstance<Floor>()
// Original: operator new(884), GameObject(GO_IDS 20), then m_static (+0x84) = 1 and the Floor vtables. PARTIAL: waits on dh2/Floor.hpp.
template <>
ObjectBase* GetNewInstance<Floor>() {
    return nullptr;
}

// 0x3410a4 ObjectBase* GetNewInstance<Dummy>()
// Original: operator new(884), GameObject(GO_IDS 20), then m_static (+0x84) = 1 and the Dummy vtables. PARTIAL: waits on dh2/Dummy.hpp.
template <>
ObjectBase* GetNewInstance<Dummy>() {
    return nullptr;
}

// 0x3410fc ObjectBase* GetNewInstance<Decor>()
// Original: operator new(888), GameObject(GO_IDS 20), then +0x84 = 1, +0x375 = 1, the Decor vtables and +0x376 = 1.
// PARTIAL: waits on dh2/Decor.hpp.
template <>
ObjectBase* GetNewInstance<Decor>() {
    return nullptr;
}

// 0x34115c ObjectBase* GetNewInstance<LightPoint>()
// Original: operator new(436), then LightPoint() with no GO_IDS. PARTIAL: waits on dh2/LightPoint.hpp.
template <>
ObjectBase* GetNewInstance<LightPoint>() {
    return nullptr;
}

// 0x342600 ObjectBase* GetNewInstance<AnimatedDecor>()
// Original: operator new(916), GameObject(GO_IDS 20), then the AnimatedDecor vtables, an empty string at +0x37c
// (a 16-byte block, first byte 0), m_static (+0x84) = 1, +0x375 set and cleared again, and +0x376 = 1. PARTIAL: waits on
// dh2/AnimatedDecor.hpp compiling, which needs dh2/Decor.hpp (absent).
template <>
ObjectBase* GetNewInstance<AnimatedDecor>() {
    return nullptr;
}

// ---------------------------------------------------------------------------------------------------------------
// Free functions.
// ---------------------------------------------------------------------------------------------------------------

// 0x34e414 ToLowerCase(char*, int, int)
// Lowercases the ASCII capitals in str[from..to] inclusive. A `to` of -1, or one at or past the end, means the last
// character. Returns strlen(str) on every path that gets that far (the asm keeps the LC_API_STRLEN result in R0), and 0
// for a null str. The original's end-of-range arithmetic is kept: when `to` is at or past the end, it becomes len - 1.
std::size_t ToLowerCase(char* str, int from, int to) {
    if (str == nullptr) {
        return 0;
    }
    const int length = static_cast<int>(std::strlen(str));
    if (to == -1 || length <= to) {
        to = length - 1;
    }
    for (int i = from; i <= to; ++i) {
        const char c = str[i];
        if (static_cast<unsigned char>(c - 'A') <= 25u) {  // 'A' .. 'Z'
            str[i] = static_cast<char>(c + 32);
        }
    }
    return static_cast<std::size_t>(length);
}

// 0x369160 AnimApplicator* GetApplicator(glitch::scene::ISceneNodeAnimator*)
// PARTIAL. The original reads the animator's type id through its vtable slot at +0x24 (slot 9; asm LDR PC,[R3,#0x24]),
// then returns the applicator at a type-dependent offset: types 11 and 13 at +0x58, type 12 at +0x88, type 14 at +0xd8,
// type 15 at +0x84, and null for any other type. A null animator returns null (asm checked). Waits on (1) a declaration
// of glitch::scene::ISceneNodeAnimator with the type getter at slot 9 (the collada animator in glitch-scene-collada-gui
// has no getType), and (2) the animator classes that hold these applicators. The offsets are recorded here and are not
// added to the pointer. Stub: null after the null check.
AnimApplicator* GetApplicator(glitch::scene::ISceneNodeAnimator* animator) {
    if (animator == nullptr) {
        return nullptr;
    }
    return nullptr;
}

// Combat formula state. File-static in the original (see dh2/global/CombatFormulas.hpp). This batch defines the five
// variables that only CF_SetCombatants writes: s_attacker, s_defender, s_element, s_offHanded and s_isSkill. The other
// four, s_ad_dlevel, s_da_dlevel, s_isBlocked and s_isCritical, are defined once in
// game-app/src/g1-game-app-global-f216b6-3.cpp (declared in dh2/global/cf_calc.hpp). CMakeLists.txt links every
// game-app/src/*.cpp into one target, so each symbol needs exactly one strong definition; this batch must not define
// those four.
Character* s_attacker = nullptr;  // 0x9a29bc
Character* s_defender = nullptr;  // 0x9a29c0
int s_element = 0;                // 0x9a29cc
bool s_offHanded = false;         // 0x9a29d0
bool s_isSkill = false;           // 0x9a29d1

// 0x3b059c CF_SetCombatants(Character*, Character*, int, bool, bool)
// Stores both characters and the flags, and returns the attacker's property 19 minus the defender's. That difference is
// also stored in s_ad_dlevel, and its negation in s_da_dlevel. The original reads property 19 with
// CharProperties::_GetProperty(this, this + 0xa94, 19), where this + 0xa94 is the final sheet m_props. The public
// CharProperties::PROPS_Get(propId, useTemp) makes that same call: its clones at 0x3af76c and 0x3bd130 are
// _GetProperty(this, this + 2708, propId), and useTemp is not used. The subtraction and the negation wrap at 32 bits as
// they do on ARM, so they are done on unsigned values.
int CF_SetCombatants(Character* attacker, Character* defender, int element, bool offHanded, bool isSkill) {
    s_attacker = attacker;
    s_defender = defender;
    const int attackerProperty = attacker->PROPS_Get(19, false);
    const int defenderProperty = defender->PROPS_Get(19, false);
    const int result = static_cast<int>(static_cast<unsigned int>(attackerProperty) -
                                        static_cast<unsigned int>(defenderProperty));
    s_isCritical = false;
    s_da_dlevel = static_cast<int>(0u - static_cast<unsigned int>(result));
    s_element = element;
    s_offHanded = offHanded;
    s_isSkill = isSkill;
    s_ad_dlevel = result;
    s_isBlocked = false;
    return result;
}
