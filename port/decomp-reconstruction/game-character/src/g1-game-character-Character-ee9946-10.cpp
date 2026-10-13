// Batch g1-game-character-Character-ee9946-10: twenty Character functions (part 11 of 20 of the class).
// createBindings and the Lua callbacks that the class binds in it: _ClearMaster, _LookAtTarget, _ClearTarget, _Stop,
// _GetMaster, _GetTarget, _GetSpotTarget, _GetHitCount, _GetPropHP, _GetCharID, _GetStateTime, _GetState,
// _GetCurrentEquippedFaeryId, _GetEquippedFaeryElement, _GetCurrentEquippedFaeryLevel, _GetCurrentSpellInfo,
// _GetCharAIFlags, _IsConnected and _CanAttackFromRange. Declarations are in dh2/Character.hpp, which this batch
// does not change (every function here is already declared there).
//
// Callback convention (as in the game-app LuaScript batch): the Hex-Rays first parameter is the argument list, a2 is
// the ReturnValues that the push calls take, and a3 is the Character. The header's static signature (args, ret, self)
// follows that use: self is the Character the binding was registered with.
//
// Status: _ClearMaster, _ClearTarget and _Stop are reconstructed (they only forward to engine calls and return
// nothing). Every other body is PARTIAL, for two reasons:
//  - The Lua return side is not declared in this tree. There is no header for sfc::script::lua::ReturnValues
//    (pushInteger, pushNumber, pushBoolean, pushUserData) or sfc::script::lua::Binder (bindFunction, bindMethod).
//    Where a body reads a declared member or calls a declared method, it computes the value into a [[maybe_unused]]
//    local, and its comment names the missing push.
//  - Some reads are private or unnamed. CharAI::m_target and CharAI::m_master are private in dh2/CharAI.hpp,
//    CharStateMachine::m_stateTime is private in dh2/CharStateMachine.hpp, the HP words sit in the CharProperties
//    sheet without a property id, and PlayerManager.hpp cannot be included with Character.hpp.
// No test: the bodies only forward to engine calls and to Lua returns, with no standalone arithmetic to check.

#include <cstdint>

#include "dh2/Character.hpp"
#include "dh2/v2Controller.hpp"

// 0x3b5678 Character::_ClearMaster(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// Lua ClearMaster: CharAI::AI_SetMaster(nullptr) on the CharAI base (+0x3c8). The listing returns the call's int, but
// AI_SetMaster is void and a binding's result is not read, so the callback returns nothing.
void Character::_ClearMaster([[maybe_unused]] const sfc::script::lua::Arguments& args,
                             [[maybe_unused]] sfc::script::lua::ReturnValues& ret, void* self) {
    Character* character = static_cast<Character*>(self);
    character->CharAI::AI_SetMaster(nullptr);
}

// 0x3b5684 Character::_LookAtTarget(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. The original calls GameObject::LookAt(GameObject*) non-virtually on the Character with the
// CharAI target (CharAI +0x40, Character +0x408). CharAI::m_target is private in dh2/CharAI.hpp (declared after its
// `private:` label), and no public accessor returns that raw pointer: AI_GetTargetAsCharacter returns a Character*.
// Missing: a friend declaration or a public getter for m_target in CharAI.
void Character::_LookAtTarget([[maybe_unused]] const sfc::script::lua::Arguments& args,
                              [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x3b5690 Character::_ClearTarget(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// Lua ClearTarget: CharAI::AI_SetTarget(nullptr, false) (the listing passes force as 0), then
// CharAI::AI_SyncLastTarget(), both on the CharAI base. The listing returns the sync's int; both calls are void, so
// the callback returns nothing.
void Character::_ClearTarget([[maybe_unused]] const sfc::script::lua::Arguments& args,
                             [[maybe_unused]] sfc::script::lua::ReturnValues& ret, void* self) {
    Character* character = static_cast<Character*>(self);
    character->CharAI::AI_SetTarget(nullptr, false);
    character->CharAI::AI_SyncLastTarget();
}

// 0x3b56b4 Character::_Stop(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// Lua Stop: v2Controller::Cmd_Stop on the controller of the v2Controllable base (+0x374; m_controller is at +0x378).
// Cmd_Stop forwards to the controller's Ctrl_Stop slot.
void Character::_Stop([[maybe_unused]] const sfc::script::lua::Arguments& args,
                      [[maybe_unused]] sfc::script::lua::ReturnValues& ret, void* self) {
    Character* character = static_cast<Character*>(self);
    character->v2Controllable::m_controller->Cmd_Stop();
}

// 0x3b56bc Character::createBindings(sfc::script::lua::Binder&)
// PARTIAL. Written: the base registration, GameObject::createBindings(binder), which the listing calls first with
// this and the binder. Missing: sfc::script::lua::Binder has no declaration in this tree (bindFunction(binder, name,
// handler, userData) and bindMethod(binder, name, handler)), so the registrations below are not made.
// The registration list in listing order (Lua name -> handler) is recorded here. Every name is registered with
// bindFunction (userData this) and bindMethod, except SetProjectileTarget, which has bindFunction with nullptr only.
// (All 90 names are listed; the handlers are declared in dh2/Character.hpp, except GameObject::_SetPosition.)
//   "SetActorPosition"           -> GameObject::_SetPosition
//   "RotateBy"                   -> Character::_RotateBy
//   "LookAt"                     -> Character::_LookAt
//   "Stop"                       -> Character::_Stop
//   "HeadTo"                     -> Character::_HeadTo
//   "MoveTo"                     -> Character::_MoveTo
//   "WarpTo"                     -> Character::_WarpTo
//   "WarpBehind"                 -> Character::_WarpBehind
//   "Flee"                       -> Character::_Flee
//   "Attack"                     -> Character::_Attack
//   "DoSkill"                    -> Character::_DoSkill
//   "BeginSkill"                 -> Character::_BeginSkill
//   "EndSkill"                   -> Character::_EndSkill
//   "Kill"                       -> Character::_Kill
//   "GetCharAIFlags"             -> Character::_GetCharAIFlags
//   "HasAggro"                   -> Character::_HasAggro
//   "AddAggro"                   -> Character::_AddAggro
//   "ClearAggro"                 -> Character::_ClearAggro
//   "HasTarget"                  -> Character::_HasTarget
//   "GetTarget"                  -> Character::_GetTarget
//   "SetTarget"                  -> Character::_SetTarget
//   "ClearTarget"                -> Character::_ClearTarget
//   "SetIsActorTargetable"       -> Character::_SetIsTargetable
//   "SetIsTargetable"            -> Character::_SetIsTargetable
//   "LookAtTarget"               -> Character::_LookAtTarget
//   "TargetInMeleeRange"         -> Character::_TargetInMeleeRange
//   "HasMaster"                  -> Character::_HasMaster
//   "IsMasterHostPlayer"         -> Character::_IsMasterHostPlayer
//   "GetMaster"                  -> Character::_GetMaster
//   "SetMaster"                  -> Character::_SetMaster
//   "ClearMaster"                -> Character::_ClearMaster
//   "GetSkillIDFromOID"          -> Character::_GetSkillIDFromOID
//   "GetCurrentSkillInfo__"      -> Character::_GetCurrentSkillInfo
//   "GetCurrentSpellInfo"        -> Character::_GetCurrentSpellInfo
//   "GetCurrentEquippedFaeryId"  -> Character::_GetCurrentEquippedFaeryId
//   "GetCurrentEquippedFaeryLevel" -> Character::_GetCurrentEquippedFaeryLevel
//   "GetEquippedFaeryElement"    -> Character::_GetEquippedFaeryElement
//   "SpawnSkillProjectile__"     -> Character::_SpawnSkillProjectile
//   "SetProjectileTarget"        -> Character::_SetProjectileTarget   [userData nullptr]
//   "EnableSpotTargeting"        -> Character::_EnableSpotTargeting
//   "GetSpotTarget"              -> Character::_GetSpotTarget
//   "StopSkill__"                -> Character::_StopSkill
//   "SkillCombatRoll__"          -> Character::_SkillCombatRoll
//   "SpellCombatRoll"            -> Character::_SpellCombatRoll
//   "RegisterActorAnim"          -> Character::_RegisterAnim
//   "RegisterAnim"               -> Character::_RegisterAnim
//   "PlayActorAnim"              -> Character::_PlayAnim
//   "PlayAnim"                   -> Character::_PlayAnim
//   "GetState"                   -> Character::_GetState
//   "GetStateTime"               -> Character::_GetStateTime
//   "SetScareState"              -> Character::_SetScareState
//   "SetStunState"               -> Character::_SetStunState
//   "SetKnockBackState"          -> Character::_SetKnockBackState
//   "AllowCharacterRotDuringSkill" -> Character::_AllowRotation
//   "AllowRotation"              -> Character::_AllowRotation
//   "AllowSkillBreak"            -> Character::_AllowSkillBreak
//   "StartTimer"                 -> Character::_StartTimer
//   "PauseTimer"                 -> Character::_PauseTimer
//   "ResumeTimer"                -> Character::_ResumeTimer
//   "StopTimer"                  -> Character::_StopTimer
//   "GetCharID"                  -> Character::_GetCharID
//   "DBG_DumpProps"              -> Character::_DBG_DumpProps
//   "SetLevel"                   -> Character::_SetLevel
//   "SetProp"                    -> Character::_SetProp
//   "GetProp"                    -> Character::_GetProp
//   "ApplyPropClass"             -> Character::_ApplyPropClass
//   "ClearProps"                 -> Character::_ClearProps
//   "CreateBuff"                 -> Character::_CreateBuff
//   "RemoveBuff"                 -> Character::_RemoveBuff
//   "SetBuffProp"                -> Character::_SetProp
//   "GetBuffProp"                -> Character::_GetProp
//   "ApplyBuff"                  -> Character::_ApplyPropClass
//   "GetPropHP"                  -> Character::_GetPropHP
//   "GetHP"                      -> Character::_GetPropHP
//   "GetAttackRatingBonus"       -> Character::_GetPropBonusAttackRating
//   "GetCritRatingBonus"         -> Character::_GetPropBonusCritRating
//   "GetDamageBonus"             -> Character::_GetPropBonusDamage
//   "HasMana"                    -> Character::_HasMana
//   "UseMana"                    -> Character::_UseMana
//   "RegenHP"                    -> Character::_RegenHP
//   "RegenMP"                    -> Character::_RegenMP
//   "RemoveDots"                 -> Character::_RemoveDots
//   "HasShield"                  -> Character::_HasShield
//   "HasBow"                     -> Character::_HasBow
//   "CanAttackInMelee"           -> Character::_CanAttackInMelee
//   "CanAttackFromRange"         -> Character::_CanAttackFromRange
//   "GetHitCount"                -> Character::_GetHitCount
//   "IsConnected"                -> Character::_IsConnected
//   "SetSkillCooldownTimerId__"  -> Character::_SetSkillCooldownTimerId
//   "SetSpellCooldownTimerId__"  -> Character::_SetSpellCooldownTimerId
void Character::createBindings(sfc::script::lua::Binder& binder) {
    GameObject::createBindings(binder);
}

// 0x3b6c70 Character::_GetMaster(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. The original pushes the CharAI master (CharAI +0x50, Character +0x418) with
// ReturnValues::pushUserData. CharAI::m_master is private in dh2/CharAI.hpp (the same gap as _LookAtTarget), and
// pushUserData is not declared.
void Character::_GetMaster([[maybe_unused]] const sfc::script::lua::Arguments& args,
                           [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x3b6c7c Character::_GetTarget(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. The original pushes the CharAI target (CharAI +0x40, Character +0x408) with
// ReturnValues::pushUserData. Same gaps as _LookAtTarget (private CharAI::m_target) and _GetMaster (pushUserData).
void Character::_GetTarget([[maybe_unused]] const sfc::script::lua::Arguments& args,
                           [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x3b6c88 Character::_GetSpotTarget(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. The original pushes three numbers: X, Y and Z of m_spotTargetValidPos (Character +0x14bc, the position
// validated against the physics world that drives the spot FX). The coordinates are read into locals. Missing: the
// three ReturnValues::pushNumber calls, in X, Y, Z order.
void Character::_GetSpotTarget([[maybe_unused]] const sfc::script::lua::Arguments& args,
                               [[maybe_unused]] sfc::script::lua::ReturnValues& ret, void* self) {
    Character* character = static_cast<Character*>(self);
    [[maybe_unused]] const float x = character->m_spotTargetValidPos.X;
    [[maybe_unused]] const float y = character->m_spotTargetValidPos.Y;
    [[maybe_unused]] const float z = character->m_spotTargetValidPos.Z;
}

// 0x3b6cc8 Character::_GetHitCount(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. The original pushes an unsigned 16-bit word at Character +0x14d0, which the header declares as unk_14d0
// (int16, recorded there as 0). The Lua name suggests a hit counter; that is not confirmed, so the member keeps its
// unk_ name. The word is read as uint16. Missing: ReturnValues::pushInteger.
void Character::_GetHitCount([[maybe_unused]] const sfc::script::lua::Arguments& args,
                             [[maybe_unused]] sfc::script::lua::ReturnValues& ret, void* self) {
    Character* character = static_cast<Character*>(self);
    [[maybe_unused]] const std::uint16_t hitCount = static_cast<std::uint16_t>(character->unk_14d0);
}

// 0x3b6cd8 Character::_GetPropHP(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. The original reads two words of the final property sheet (CharProperties::m_props, at
// +0x94 and +0x9c of the sheet; Character +0x1088 and +0x1090): the first is the current HP, the second the maximum,
// both in fixed point (>> 8 for display). It pushes three integers: current >> 8, maximum >> 8 and
// (100 * current / maximum) >> 8, or three zeros when the maximum is 0. The property ids that own these two words are
// not named, so the words are not read here. Missing: the property id mapping (layout request) and
// ReturnValues::pushInteger.
void Character::_GetPropHP([[maybe_unused]] const sfc::script::lua::Arguments& args,
                           [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x3b6d5c Character::_GetCharID(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. The original pushes the signed 16-bit word at Character +0x13c8, which is m_charPropsId (the row of
// Arrays::CharacterTable, -1 when unresolved). The raw word is read, not the resolved SafeGetCharPropsId value.
// Missing: ReturnValues::pushInteger.
void Character::_GetCharID([[maybe_unused]] const sfc::script::lua::Arguments& args,
                           [[maybe_unused]] sfc::script::lua::ReturnValues& ret, void* self) {
    Character* character = static_cast<Character*>(self);
    [[maybe_unused]] const int charId = character->m_charPropsId;
}

// 0x3b6d6c Character::_GetStateTime(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. The original pushes the time in the current state (ms), the word at CharStateMachine +0x60
// (m_stateTime, Character +0x55c). CharStateMachine::m_stateTime is private in dh2/CharStateMachine.hpp (declared
// after its `private:` label) and has no public getter. Missing: that access, and ReturnValues::pushInteger.
void Character::_GetStateTime([[maybe_unused]] const sfc::script::lua::Arguments& args,
                              [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x3b6d78 Character::_GetState(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. The original pushes CharStateMachine::SM_GetState() (-1 when there is no current state), read through the
// CharStateMachine base at +0x4fc. The call is made; missing: ReturnValues::pushInteger.
void Character::_GetState([[maybe_unused]] const sfc::script::lua::Arguments& args,
                          [[maybe_unused]] sfc::script::lua::ReturnValues& ret, void* self) {
    Character* character = static_cast<Character*>(self);
    [[maybe_unused]] const int state = character->CharStateMachine::SM_GetState();
}

// 0x3b6da0 Character::_GetCurrentEquippedFaeryId(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. The original pushes SG_GetCurrentFaerieId(-1). The call is made; missing: ReturnValues::pushInteger.
void Character::_GetCurrentEquippedFaeryId([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                           [[maybe_unused]] sfc::script::lua::ReturnValues& ret, void* self) {
    Character* character = static_cast<Character*>(self);
    [[maybe_unused]] const int faeryId = character->SG_GetCurrentFaerieId(-1);
}

// 0x3b6dc4 Character::_GetEquippedFaeryElement(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. The original takes the current faery id (SG_GetCurrentFaerieId(-1)), looks up its row with GetCharFaery and
// pushes the word at row +0x8 (Structs::Faery m_w2). Character::GetCharFaery is still partial and returns nullptr, so
// the row word is not read here (that would be a null dereference). Missing: the GetCharFaery rows, the element read
// and ReturnValues::pushInteger. The lookup itself is made, as in the original.
void Character::_GetEquippedFaeryElement([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                         [[maybe_unused]] sfc::script::lua::ReturnValues& ret, void* self) {
    Character* character = static_cast<Character*>(self);
    const int faeryId = character->SG_GetCurrentFaerieId(-1);
    static_cast<void>(character->GetCharFaery(faeryId));
}

// 0x3b6df8 Character::_GetCurrentEquippedFaeryLevel(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. The original pushes SG_GetFaerieLevel(current faery id, -1). The level is computed; missing:
// ReturnValues::pushInteger.
void Character::_GetCurrentEquippedFaeryLevel([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                              [[maybe_unused]] sfc::script::lua::ReturnValues& ret, void* self) {
    Character* character = static_cast<Character*>(self);
    const unsigned int faeryId = static_cast<unsigned int>(character->SG_GetCurrentFaerieId(-1));
    [[maybe_unused]] const int faeryLevel = character->SG_GetFaerieLevel(faeryId, -1);
}

// 0x3b6e30 Character::_GetCurrentSpellInfo(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. The original looks up the current faery row (the row is discarded), reads the current faery id again and
// pushes SG_GetFaerieLevel of that id. Both lookups are made as in the listing; missing: ReturnValues::pushInteger.
void Character::_GetCurrentSpellInfo([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                     [[maybe_unused]] sfc::script::lua::ReturnValues& ret, void* self) {
    Character* character = static_cast<Character*>(self);
    const int rowId = character->SG_GetCurrentFaerieId(-1);
    static_cast<void>(character->GetCharFaery(rowId));
    const unsigned int faeryId = static_cast<unsigned int>(character->SG_GetCurrentFaerieId(-1));
    [[maybe_unused]] const int faeryLevel = character->SG_GetFaerieLevel(faeryId, -1);
}

// 0x3b6e80 Character::_GetCharAIFlags(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. The original pushes the flag word of the AI row, Structs::AIProps m_w5 (+0x14 of the row at
// Arrays::AITable::members + 68 * GetCharAIId()). GetCharAI() is that row, and the flag-bit helpers in
// game-character/src/g1-game-character-Character-ee9946-1.cpp read the same word. The word is read; missing:
// ReturnValues::pushInteger.
void Character::_GetCharAIFlags([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                [[maybe_unused]] sfc::script::lua::ReturnValues& ret, void* self) {
    Character* character = static_cast<Character*>(self);
    [[maybe_unused]] const std::int32_t flags = character->GetCharAI()->m_w5;
}

// 0x3b6ec4 Character::_IsConnected(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. The original gets the PlayerInfo of this character with
// PlayerManager::GetPlayerByCharacter(this, false) on dword_99F76C, calls its vtable slot +0x5c, which is
// PlayerInfo::IsActive (slot 23 in dh2/PlayerInfo.hpp), and pushes the bool with ReturnValues::pushBoolean.
// PlayerManager.hpp cannot be included together with Character.hpp (its StreamBuffer member needs net-transport's
// IStreamBase, which pydata's stream_reader.hpp also defines), so the call is not written. Missing: that include,
// and ReturnValues::pushBoolean.
void Character::_IsConnected([[maybe_unused]] const sfc::script::lua::Arguments& args,
                             [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* self) {}

// 0x3b6f10 Character::_CanAttackFromRange(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. The original calls the virtual CanRangeAttack() through the primary vtable (+0x124) and pushes the bool with
// ReturnValues::pushBoolean. The call is made; missing: ReturnValues::pushBoolean.
void Character::_CanAttackFromRange([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                    [[maybe_unused]] sfc::script::lua::ReturnValues& ret, void* self) {
    Character* character = static_cast<Character*>(self);
    [[maybe_unused]] const bool canAttack = character->CanRangeAttack();
}
