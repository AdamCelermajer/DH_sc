#pragma once
// Player-VM GameObject/Character host functions and object methods.
//
// The Player skill VM binds every `Kind::function` / `Kind::method` row of
// character_native_bindings. Rows whose bodies already have an owner (target
// list, combat rolls, timers, properties, buffs, mana, projectiles) are routed
// to those owners by native_player_skills.cpp. This module owns the remaining
// GameObject/Character natives that spell and skill scripts call:
//
//   PlayFX        GameObject::_PlayFX        0x3923cc
//   PlaySound3D   GameObject::_PlaySound3D   0x39212c
//   GetRand       GameObject::_Rand          0x393310
//   GetSelf       GameObject::_GetSelf       0x38eaf4
//   GetID         GameObject::_GetID         0x38ebe4
//   GetName       GameObject::_GetName       0x38ebf0
//   GetPosition   GameObject::_GetPosition   0x38e700
//   GetHP/GetPropHP Character::_GetPropHP    0x3b6cd8
//   HasBow        Character::_HasBow         0x3b6f84
//   CanAttackInMelee Character::_CanAttackInMelee 0x3b6f64
//   RegenHP       Character::_RegenHP        0x3b77ec
//   SetIsTargetable Character::_SetIsTargetable 0x3b7a64
//   SetTarget     Character::_SetTarget      0x3b8f38
//   CreateBuff/RemoveBuff/ApplyBuff, GetProp on a non-Player receiver
//   GetCharAIFlags Character::_GetCharAIFlags 0x3b6e80
//   TargetInMeleeRange Character::_TargetInMeleeRange 0x3b7020
//   IsDead/IsCharacter/IsPlayer/IsDoor/IsDecor, GetState,
//   LOCK/UNLOCK, Kill, SetStunState/SetScareState/SetKnockBackState
//   HasPath       GameObject::_HasPath       0x38e98c
//   MoveTo        Character::_MoveTo         0x3bada8
//   HeadTo        Character::_HeadTo         0x3ba71c
//   WarpTo        Character::_WarpTo         0x3bb268
//   Stop          Character::_Stop           0x3b56b4
//   SetMaster     Character::_SetMaster      0x3b9084
//   SetLevel      Character::_SetLevel       0x3b73a4
//   Summon        GameObject::_Summon        0x39193c
//   SummonTimerTrap GameObject::_SummonTimerTrap 0x390c50
//   RegisterAnim  Character::_RegisterAnim   0x3b70d8
//   PlayAnim      Character::_PlayAnim       0x3ba388
//   SetProp/ApplyPropClass on a receiver other than the Player
//                 Character::_SetProp 0x3b7eac / _ApplyPropClass 0x3babdc
//
// Every original native is `(Arguments, ReturnValues, GameObject* this)`. The
// receiver is the Player for a global call and the table's `_this` identity for
// a `target:Method()` call; this module takes the receiver explicitly and keeps
// the original argument checks, defaults and return arity. Facts and effects
// come from the live owners through Services; a missing provider is an
// explicit required-service failure, never a successful no-op.
#include "../../../../../adam-script-runtime/script_runtime.h"
#include "../../../../../level-world/character_native_bindings.hpp"
extern "C" {
#include "../../../../../random/random.h"
}
#include <cstddef>
#include <cstdint>
#include <string>

namespace dh2::native::player_skill_host_v1 {

// Facts read from any GameObject identity (Player, Monster, Faery, other
// Character). Filled by the renderer from the canonical owners.
struct ObjectFacts {
    bool is_character=false,is_player=false,is_dead=false,is_door=false,is_decor=false;
    // CharStateMachine::SM_GetState, or -1 for a non-Character.
    std::int32_t state=-1;
    // CharacterProperties 36 (HP) and 38 (Max_HP) raw fixed-point words.
    std::int32_t hp_raw=0,max_hp_raw=0;
    // AITable[GetCharAIId()].Flags (+20), valid for a Character.
    std::uint32_t ai_flags=0;
    bool has_ai_flags=false;
    // GameObject+0x160 position (the frame PlayAnimFXSet / Play3D consume).
    float position[3]{};
    // GameObject+0x16c EulerXYZ words (+0x174 is the heading GetLookAtVec uses).
    float rotation[3]{};
    // ObjectBase+0x44 name.
    char name[128]{};
};

// Character::_CreateBuff / _RemoveBuff / _ApplyBuff on a receiver other than the
// Player. The Player's own buffs stay with its BuffOwner (native_player_skills).
enum class BuffCommand : std::uint8_t { create, remove, apply };

enum class ObjectCommand : std::uint8_t {
    lock,unlock,
    // Cmd_Kill(controller, killer, false). `other` is the killer identity or 0.
    kill,
    // CharStateMachine::SM_SetStunState / SM_SetScareState(duration, flag,
    // nullptr, false): `duration` and `flag`.
    stun,scare,
    // CharStateMachine::SM_SetKnockBackState(great = `flag`, nullptr, false).
    knock_back,
};

// v2Controller::Cmd_* commands the Character natives issue on their own
// controller. Each one is gated by the controller (forced || !blocked &&
// !locked) and then runs the Character controllable body, which refuses a
// remotely updated object itself.
enum class ControlCommand : std::uint8_t {
    // Cmd_MoveTo(const Point3D&) -> Character::Ctrl_MoveTo(Point3D) 0x3ada30:
    // GameObject::PathTo(point).
    move_to_point,
    // Cmd_HeadTo(const Point3D&) -> Character::Ctrl_HeadTo(Point3D) 0x3adac8:
    // SetHeadingDirection(point - position, true), SetDestination(point),
    // RaiseEvent(0).
    head_to_point,
    // Cmd_WarpTo(const Point3D&) -> Character::Ctrl_WarpTo 0x3adcf4:
    // PFWorld::ValidatePosition, SetPosition(point, true), ForceUpdatePosition,
    // RaiseEvent(1); a rejected point does nothing.
    warp_to_point,
    // Cmd_MoveTo(GameObject*) -> Character::Ctrl_MoveTo(GameObject*) 0x3ad9e4:
    // GameObject::PathTo(object->GetTargetPosition()).
    move_to_object,
    // Cmd_Stop() -> Character::Ctrl_Stop 0x3ad890: GameObject::Stop,
    // RaiseEvent(63).
    stop,
};

// GameObject::_Summon after argument decoding: the final world position (floor
// handling and Character::SetInitialPosition are the provider's), the
// EulerXYZ words handed to GameObject::SetRotation, the SM_SetSpawnState flag
// and the RootSceneNode::ForwardAnim milliseconds.
struct SummonRequest {
    std::uint32_t character_table_oid=0;
    bool request_spawn_state=false;
    float position[3]{};
    float rotation[3]{};
    std::uint32_t forward_anim_ms=0;
};

// GameObject::_SummonTimerTrap (0x390c50) after its argument checks: the
// TimerTraps row, Create's third argument (the trap's hurt-chance word) and the
// optional placement. `has_position` is false when the script passed no point,
// leaving the trap at its owner's position (TimerTrap::Create copies it).
struct SummonTimerTrapRequest {
    std::uint32_t trap_id=0;
    std::uint32_t hurt_chance=0;
    bool has_position=false;
    float position[3]{};
};

// Character::_PlayAnim after argument decoding (see do_play_anim).
struct PlayAnimRequest {
    std::uint32_t anim_dict=0;
    bool has_second=false;
    std::int32_t second=-1;
    bool flag=false;
};

struct Services {
    // Renderer-owned callbacks share `context`.
    void* context=nullptr;
    // Player Character identity (the receiver of a global call).
    std::uintptr_t self=0;
    // Process RNG stream (Random::s_seed), the online flag and Character+252
    // object seed used by GameObject::_Rand.
    dh2_random_state* random=nullptr;
    const std::uint8_t* online=nullptr;
    std::uint32_t* object_seed=nullptr;

    int (*object_facts)(void*,std::uintptr_t identity,ObjectFacts*)=nullptr;
    // VisualFXManager::PlayAnimFXSet. `attach` is the GameObject the effect is
    // bound to (position offset origin), or 0 with an explicit `position`.
    int (*play_fx)(void*,std::int32_t effect,std::uintptr_t attach,
                   const float* position,std::string& error)=nullptr;
    // VoxSoundManager::Play3D(sound, position, false, 1, -1, -1).
    int (*play_sound_3d)(void*,const char* name,std::size_t bytes,
                         const float position[3],std::string& error)=nullptr;
    // EquipmentWeaponFlagV1 bits of the Player's canonical inventory.
    int (*weapon_flags)(void*,std::uint32_t* flags,std::string& error)=nullptr;
    // Character::RegenHP(raw amount) on the Player.
    int (*regen_hp)(void*,std::int32_t raw_amount,std::string& error)=nullptr;
    // Character+0x415 (CharAI+0x4d) for the receiver.
    int (*set_targetable)(void*,std::uintptr_t receiver,bool targetable,
                          std::string& error)=nullptr;
    // CharAI::AI_SetTarget(target, false) for the receiver.
    int (*set_target)(void*,std::uintptr_t receiver,std::uintptr_t target,
                      std::string& error)=nullptr;
    int (*object_command)(void*,ObjectCommand,std::uintptr_t receiver,
                          std::uintptr_t other,std::uint32_t duration,bool flag,
                          std::string& error)=nullptr;
    // CharAI::AI_IsInMeleeRange(receiver's AI, receiver's current target).
    int (*target_in_melee_range)(void*,std::uintptr_t receiver,bool* value,
                                 std::string& error)=nullptr;
    // The receiver's own BuffOwner. Arguments are the source Arguments; the
    // callback returns the BuffOwner status and fills results/text.
    int (*buff_command)(void*,BuffCommand,std::uintptr_t receiver,
                        const dh2_script_value* arguments,std::uint32_t count,
                        dh2_script_value* results,std::uint32_t capacity,
                        std::uint32_t* returned,char* text,std::size_t bytes)=nullptr;
    // CharacterProperties[id] of the receiver's resolved sheet (GetProp(id)).
    int (*get_property)(void*,std::uintptr_t receiver,std::uint32_t id,
                        std::int32_t* value,std::string& error)=nullptr;
    // CharProperties::PROPS_Set(id, value) on the receiver's own CharProperties
    // (Character+0x560): a type-32 property writes the saved sheet and
    // recalculates it, a type-8 property writes the resolved sheet, any other
    // type is ignored.
    int (*set_property)(void*,std::uintptr_t receiver,std::uint32_t id,
                        std::int32_t value,std::string& error)=nullptr;
    // CharProperties::PROPS_ApplyClass(class, to_temp) on the receiver's own
    // CharProperties: to_temp selects the shared s_temp sheet, otherwise the
    // receiver's resolved sheet.
    int (*apply_property_class)(void*,std::uintptr_t receiver,std::int32_t class_id,
                                bool to_temp,std::string& error)=nullptr;
    // Number of rows of Arrays::ClassTable / Arrays::CharacterTable.
    std::uint32_t (*class_table_size)(void*)=nullptr;
    std::uint32_t (*character_table_size)(void*)=nullptr;
    // v2Controller::Cmd_* on the receiver (see ControlCommand). `object` is the
    // GameObject for move_to_object, `point` the Point3D for the point forms.
    int (*control)(void*,ControlCommand,std::uintptr_t receiver,std::uintptr_t object,
                   const float point[3],std::string& error)=nullptr;
    // GameObject::_HasPath: whether the receiver's path list is not empty.
    int (*has_path)(void*,std::uintptr_t receiver,bool* value,std::string& error)=nullptr;
    // CharAI::AI_SetMaster(Character*) on the receiver's CharAI.
    int (*set_master)(void*,std::uintptr_t receiver,std::uintptr_t master,
                      std::string& error)=nullptr;
    // Character::_SetLevel with the raw fixed-point Level word the script passed.
    int (*set_level)(void*,std::uintptr_t receiver,float raw_level,
                     std::string& error)=nullptr;
    // GameObject::_Summon: Character::CreateNPC + SetInitialPosition +
    // SetPosition + SetRotation + the room/no-room/ZoneEntered tail. `created`
    // is the new Character, or 0 when the source pushes nothing.
    int (*summon)(void*,std::uintptr_t summoner,const SummonRequest&,
                  std::uintptr_t* created,std::string& error)=nullptr;
    // GameObject::_SummonTimerTrap: TimerTrap::Create(owner = receiver) +
    // GameObject::SetPosition. The provider applies the source table bounds
    // (trap < Arrays::TimerTraps::size, damager < Arrays::GameObjectDamager::size)
    // and reports `created` = 0 when the source creates nothing.
    int (*summon_timer_trap)(void*,std::uintptr_t owner,const SummonTimerTrapRequest&,
                             std::uintptr_t* created,std::string& error)=nullptr;
    // CharAnimator::ANIM_AddAnimDictToSet(receiver's animator, dict); a negative
    // dict is the source's silent no-op after the animator's set id assignment.
    int (*register_anim)(void*,std::uintptr_t receiver,std::int32_t anim_dict,
                         std::string& error)=nullptr;
    // Number of rows of Arrays::AnimDict.
    std::uint32_t (*anim_dict_size)(void*)=nullptr;
    // The scripted AnimTable rows plus CharStateMachine::SM_SetAnimState.
    int (*play_anim)(void*,std::uintptr_t receiver,const PlayAnimRequest&,
                     std::string& error)=nullptr;
};

// True when `function` is owned by this module. native_player_skills.cpp calls
// invoke() for those and keeps its existing owners for the others.
bool handles(character_native_bindings::Function function) noexcept;

// Runs `function` with `receiver` as the GameObject `this`. `arguments` are the
// source Arguments (without the receiver). Returns zero on success;
// DH2_SCRIPT_REQUIRED_SERVICE_FAILURE with `text` set otherwise.
int invoke(const Services&,character_native_bindings::Function function,
           const char* name,std::uintptr_t receiver,
           const dh2_script_value* arguments,std::uint32_t count,
           dh2_script_value* results,std::uint32_t capacity,
           std::uint32_t* returned,char* text,std::size_t bytes) noexcept;

} // namespace dh2::native::player_skill_host_v1
