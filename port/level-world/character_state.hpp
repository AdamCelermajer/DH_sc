#pragma once
#include <cstdint>
namespace dh2::character_skill_state_dispatch_v1 { struct Projection; }
namespace dh2::character_cast_lifecycle_v1 { struct Projection; }
namespace dh2::character {
// Logical projection of Character and CharStateMachine, never an ARM32 overlay.
struct State {
 std::int32_t current=-1;
 std::uint32_t flags=0,attack_gate=0,move_type=0,elapsed_ms=0;
 float cached_speed=0;
 std::uint32_t idle_suppressed=0,stop_attack_allowed=0,heading_active=0,
               controller_locked=0,dead_alternate=0,body_present=0;
 std::int32_t animation_override=-1,current_animation=-1;
};
// Authored table/stance/property/controller producers remain explicit facts.
// Borrowed storage can be mutable through the callback context: services that
// change a producer (e.g. Stop resets heading) must refresh corresponding facts
// before returning, since following focus/update reads those current values.
struct Facts {
 std::uint32_t is_player=0,stance_mask=0;
 std::uint64_t target=0;
 float heading[3]{},walk_threshold=0,run_threshold=0,walk_speed=1,attack_speed=1;
 std::int32_t idle=-1,walk=-1,run=-1,attack_static=-1,attack_moving=-1,
              death=-1,stance=0; // death is upstream selection; focus reads State.animation_override.
 std::uint32_t attack_delay=0,despawn_delay=0,reserved=0;
 std::uint32_t is_at_destination=0,following_path=0,has_ranged_weapon=0;
};
// Combat-only data is projected separately so the established Facts/State
// layouts and their differential reference format remain stable.
struct CombatFacts {
 // These are source-selected animation sequence IDs after the source
 // AnimStancedAnim/GetAnimStance gate, not table-row offsets.
 std::int32_t scared_animation=-1,stunned_animation=-1,injured_animation=-1;
 // Character+0x528 and CharStateMachine+0x44 are the same source word.
 // It is retained once in the same Coordinator owner.
 std::uint32_t machine_flags_44=0,controller_present=0;
 // Character::CSM_Interrupted loads byte [Character+0x441] for state 5.
 std::uint32_t interrupted_byte_441=0;
 // CSInjured::OnBlur loads [Character+0x408] as its focus GameObject.
 std::uint64_t last_target=0;
 // The caller must distinguish an observed null/zero source field from an
 // unimplemented projection; handlers fail closed only when they consume it.
 std::uint32_t interrupted_byte_available=0,last_target_available=0;
};
enum Service : std::uint32_t {
 stop=1,pin,unpin,set_animation,set_speed,swap_animation,stop_loop,
 start_timer,look_at,remove_highlight,set_death_filter,reset_filter,
 disable_state_fx,disable_self_fx,remove_buffs,cancel_sneaking,
 raise_event,remove_body,idle_common_update,set_heading,
 // Actor-owned source services used by the Limbus/Spawn slice below. The
 // Character vptr +0x40 resolves to GameObject::SetVisible(bool).
 set_visible,reset_controller_lock,restore_limbus_position,
 restore_limbus_rotation,revive_character,set_limbus_group_status,
 clear_all_aggro,clear_ai_target,sync_last_ai_target,start_fade_in,
 init_physical_object,enable_collisions,
 // Source CSDead OnFocus begins with two real Debug load/query calls before
 // flags/LookAt. Appended service preserves all previous enum values and ABI.
 dead_focus_prelude,store_previous_flags, // CSDespawn saves Character flags before replacing them.
 controller_lock,scare_focus,scare_blur,knockback_impact_event,
 combat_machine_flags,restore_interaction_rotation
};
struct Request {
 std::uint32_t service;
 std::int32_t argument[3];
 float scalar;
 std::uint32_t reserved;
 std::uint64_t identity;
};
// Arguments: set_animation[0]=sequence; set_speed.scalar=speed;
// swap_animation[0]=desired root,[1]=old root (preserving stack is a service);
// start_timer[0]=delay bits,[1]=0,[2]=event; set_death_filter=[group,category,mask]
// with scalar0 (do not change the secondary shape). look_at.identity is a
// borrowed object identity; argument0:0 GameObject.LookAt(object),1 controller
// Cmd_LookAt(object),2 GameObject.LookAt(target.GetTargetPosition()).
// raise_event[0]=event, identity=payload. For event1d
// argument[1] also contains the previous state ID. set_heading arguments carry
// three IEEE float words; scalar1 means enabled. set_visible[0] is bool;
// true restores ObjectBase enabled-byte visibility. Other services use zeros.
// knockback_impact_event is CSKnockedBack::OnEvent(event=35)'s conditional
// same-Character sequence: call Character::IsDead; if true, CharAnimator::ANIM_Stop
// then CharStateMachine::SM_SetDeadState(true,nullptr,true). The service owns
// those real app Character/animator/machine objects and must update this same
// Coordinator before returning; Scheduler.StopLoop is not ANIM_Stop.
// combat_machine_flags[0]=mask,[1]=value updates the one Character+0x528
// machine word (State.attack_gate) as (word&~mask)|value; CSKnockedBack uses the
// full mask 0xffffffff with value 0x20 for the great-knock replacement.
// Callback is synchronous and may change State or reenter the coordinator.
// Animation/body/timer/AI/FX services must preserve the emitted call order.
using Callback=void(*)(void*,State*,const Request*);
struct Services {void* context;Callback invoke;};
// Additional source facts for the actor-owned Limbus/Spawn path. These are
// separate from Facts so existing Character-state fixture and adapter ABI
// remain unchanged. respawn_timer_eligible is the caller's resolved source
// timer gate projected by the caller; live double-delay reads and hosting are
// implemented separately in character_limbus_respawn. Group inputs reflect
// the source group predicate and are not reconstructed by this bounded core.
struct SpawnFacts {
 std::int32_t spawn_animation=-1;
 // Reserved to preserve the initial SpawnFacts ABI. CSSpawn selects a stance
 // only when entering from PreSpawn with Character flags bit 0x2000 set; the
 // source transition reads that prior State directly, never this caller fact.
 std::uint32_t reserved_zero=0,visual_present=0;
 float raw_fade_in_argument=0;
 std::uint32_t respawn_timer_eligible=0,respawn_delay_ms=0,
              can_respawn=0,group_role3=0,group_has_other_in_limbus=0;
};
// CSAnim (CharStateMachine state 14: OnInit 0x3c8b28, OnFocus 0x3c324c, OnUpdate
// 0x3c1a3c, OnEvent 0x3c1a14, OnBlur 0x3c2dd0, an empty body). SM_SetAnimState
// (0x3c1aa8) stores the pending AnimTable row in the machine's +0x28 word
// (State.animation_override) and two bytes at CharStateMachine+0x44/+0x45, which
// are Character+0x540/+0x541. Character+0x1480 is the scripted byte that
// Script_MarkCharacterAsScripted writes. The Coordinator owns all three; this
// projection borrows copies so the established State/Facts layouts stay intact.
struct AnimFacts {
 // +0x44 (Character+0x540): CSAnim::OnEvent returns to Idle on event 34.
 // PlayAnim's Lua boolean; Script_PlayActorAnim passes false.
 std::uint32_t wait_for_end_540=0;
 // +0x45 (Character+0x541): selects the OnFocus flag word and lets OnUpdate
 // leave the state unless the Character is scripted. Script_PlayActorAnim
 // passes true; Lua PlayAnim passes false.
 std::uint32_t idle_on_update_541=0;
 // Character+0x1480.
 std::uint32_t scripted_1480=0;
};
static_assert(sizeof(AnimFacts)==12);
static_assert(sizeof(SpawnFacts)==36);
static_assert(sizeof(State)==56&&sizeof(Facts)==96&&sizeof(CombatFacts)==40);
static_assert(sizeof(Request)==32&&sizeof(Services)==16);
static_assert(sizeof(void*)==8,"native coordinator requires 64-bit pointers");
}
extern "C" {
// Original getter reads resolved native property48, int32 fixed point /256,
// then percentage .01, adds1, and clamps nonpositive results to +0.
int dh2_character_attack_speed(float*,const std::int32_t* resolved224);
// Source bool mode: IDs3/13 true; ID18 !mode; Move4 and others false.
int dh2_character_state_is_idle(std::int32_t current,std::uint32_t mode);
// 1 executed/accepted, 0 event ignored/rejected, -1 malformed before mutation.
// Events are STATE events (after AI routing), not arbitrary Character events.
// 0x2a/2b/2c first clear machine mask1/2/4 before current OnEvent.
int dh2_character_state_transition(dh2::character::State*,const dh2::character::Facts*,
 std::int32_t next,std::int32_t event,std::uint64_t payload,const dh2::character::Services*);
int dh2_character_state_event(dh2::character::State*,const dh2::character::Facts*,
 std::uint32_t event,std::uint64_t payload,const dh2::character::Services*);
// Bounded state OnUpdate. IdleCommonUpdate remains an explicit service;
// machine stun/scare enforcement and general timers/AI are outside this API.
int dh2_character_state_update(dh2::character::State*,const dh2::character::Facts*,
 std::uint32_t dt_ms,const dh2::character::Services*);
// Combat IDs 8..11 are handlers on this same State and Coordinator. The
// additional facts are transient projections of the existing Character,
// controller, body, and animation-table owners; persistent flags remain in
// State and the existing Character/CharStateMachine backing.
int dh2_character_combat_state_transition(dh2::character::State*,const dh2::character::Facts*,
 const dh2::character::CombatFacts*,std::int32_t next,std::int32_t event,
 std::uint64_t payload,const dh2::character::Services*);
int dh2_character_combat_state_event(dh2::character::State*,const dh2::character::Facts*,
 const dh2::character::CombatFacts*,std::uint32_t event,std::uint64_t payload,
 const dh2::character::Services*);
int dh2_character_combat_state_update(dh2::character::State*,const dh2::character::Facts*,
 const dh2::character::CombatFacts*,std::uint32_t dt_ms,const dh2::character::Services*);
// Registered edges: CSInjured(11) event 50005 -> CSSkill(6), and a direct
// _SetState from CSSkill(6) into a combat reaction state 8..11 (F_ApplyResult
// special hits), which runs CSSkill::OnBlur before the combat Focus.
int dh2_character_combat_skill_state_transition(dh2::character::State*,const dh2::character::Facts*,
 const dh2::character::CombatFacts*,dh2::character_skill_state_dispatch_v1::Projection*,
 std::int32_t next,std::int32_t event,std::uint64_t payload,
 const dh2::character::Services*);
// Direct _SetState from CSCast(7) into a combat reaction state 8..11.
int dh2_character_combat_cast_state_transition(dh2::character::State*,const dh2::character::Facts*,
 const dh2::character::CombatFacts*,dh2::character_cast_lifecycle_v1::Projection*,
 std::int32_t next,std::int32_t event,std::uint64_t payload,
 const dh2::character::Services*);
// Coordinator-only CSSkill transition path. The projection borrows this same
// State and the captured callback graph; the original State/Facts/Services
// layouts and ordinary state entry points remain unchanged.
int dh2_character_skill_state_transition(dh2::character::State*,const dh2::character::Facts*,
 dh2::character_skill_state_dispatch_v1::Projection*,std::int32_t next,
 std::int32_t event,std::uint64_t payload,const dh2::character::Services*);
// CSCast state7 transitions borrow the same Coordinator State and a separate
// callback projection for Character RaiseEvent32/33 and owner services.
int dh2_character_cast_state_transition(dh2::character::State*,const dh2::character::Facts*,
 dh2::character_cast_lifecycle_v1::Projection*,std::int32_t next,
 std::int32_t event,std::uint64_t payload,const dh2::character::Services*);
// Source-backed Limbus(0), Spawn(1), and existing Idle(3) transitions. The
// original SpawnCharacter script and Limbus respawn timer both arrive here.
int dh2_character_spawn_transition(dh2::character::State*,const dh2::character::Facts*,
 const dh2::character::SpawnFacts*,std::int32_t next,const dh2::character::Services*);
// Event 0x28 carries the exact string "is_interactive" to construct the body;
// event 0x22 completes Spawn into Idle. Limbus event 0x2f is gated by the
// source CanRespawn result supplied in SpawnFacts. Other events are ignored.
int dh2_character_spawn_event(dh2::character::State*,const dh2::character::Facts*,
 const dh2::character::SpawnFacts*,std::uint32_t event,const char* event_name,
 const dh2::character::Services*);
// CSSpawn::OnInit registers event 50008 -> CSDead (12). This transition
// preserves the exact _SetState/RaiseStateEvent payload as CSDead's killer.
int dh2_character_spawn_dead_transition(dh2::character::State*,const dh2::character::Facts*,
 const dh2::character::SpawnFacts*,std::uint64_t payload,const dh2::character::Services*);
// Limbus and CSSpawn OnUpdate are source no-ops; this never invents a timeout
// or idle transition. Return 0 for a valid no-op and -1 for malformed input.
int dh2_character_spawn_update(const dh2::character::State*,
 const dh2::character::SpawnFacts*);
// CSAnim (state 14) is entered by CharStateMachine::SM_SetAnimState and left by
// its two registered events (50001 -> CSMove, 50005 -> CSSkill), its OnEvent /
// OnUpdate Idle returns, or any direct _SetState. Exactly one side of a
// transition may be state 14 (14 -> 14 is allowed: a second SM_SetAnimState
// re-runs OnFocus without resetting the state timer). The non-CSAnim side is
// one of the base states -1/2/3/4/5/12/13 or, with CombatFacts, 8..11; CSSkill
// (6) and CSCast (7) use their own projections below. The pending AnimTable row
// is State.animation_override; OnFocus plays and clears it through the existing
// set_animation service (SM_SetAnim(-1)). 1 executed, -1 malformed or an
// unattached source state.
int dh2_character_anim_state_transition(dh2::character::State*,const dh2::character::Facts*,
 const dh2::character::AnimFacts*,const dh2::character::CombatFacts*,
 std::int32_t next,std::int32_t event,std::uint64_t payload,const dh2::character::Services*);
// Limbus (0), Spawn (1) and PreSpawn (17) -> CSAnim (14), e.g. Script_PlayActorAnim
// on a cutscene actor that has not finished spawning: the outgoing state's
// source OnBlur runs first (CSLimbus revives and shows the body, CSSpawn creates
// the physical object, CSPreSpawn revives and enables collisions).
int dh2_character_anim_spawn_state_transition(dh2::character::State*,const dh2::character::Facts*,
 const dh2::character::SpawnFacts*,const dh2::character::AnimFacts*,
 std::int32_t event,const dh2::character::Services*);
// CSAnim (14) -> Limbus (0), e.g. Script_PutCharacterInLimbus (SM_SetLimbusState
// 0x3c1a74 -> _SetState(0,-1,0)) on a cutscene actor that is still playing its
// Script_PlayActorAnim row: CSAnim::OnBlur (0x3c2dd0) is empty and
// CSLimbus::OnFocus hides the body, starts the respawn timer and clears aggro.
int dh2_character_anim_limbus_transition(dh2::character::State*,const dh2::character::Facts*,
 const dh2::character::SpawnFacts*,const dh2::character::AnimFacts*,
 std::int32_t event,const dh2::character::Services*);
// CSSkill (6) <-> CSAnim (14): CSSkill::OnBlur / OnFocus run through the same
// callback projection the ordinary CSSkill transitions borrow.
int dh2_character_anim_skill_state_transition(dh2::character::State*,const dh2::character::Facts*,
 const dh2::character::AnimFacts*,dh2::character_skill_state_dispatch_v1::Projection*,
 std::int32_t next,std::int32_t event,std::uint64_t payload,const dh2::character::Services*);
// CSCast (7) -> CSAnim (14).
int dh2_character_anim_cast_state_transition(dh2::character::State*,const dh2::character::Facts*,
 const dh2::character::AnimFacts*,dh2::character_cast_lifecycle_v1::Projection*,
 std::int32_t next,std::int32_t event,std::uint64_t payload,const dh2::character::Services*);
// State events while CSAnim is current: event 34 returns to Idle when the
// +0x44 byte is set (OnEvent runs before the registered transitions); 50001
// enters CSMove; 50005 enters CSSkill and needs the skill projection. Other
// events are ignored (CSAnim registers nothing else). 1 transitioned, 0
// ignored, -1 malformed.
int dh2_character_anim_state_event(dh2::character::State*,const dh2::character::Facts*,
 const dh2::character::AnimFacts*,dh2::character_skill_state_dispatch_v1::Projection*,
 std::uint32_t event,std::uint64_t payload,const dh2::character::Services*);
// CSAnim::OnUpdate: with the scripted byte clear and the +0x45 byte set the
// state calls SM_SetIdleState(false); otherwise nothing happens. 1 on success.
int dh2_character_anim_state_update(dh2::character::State*,const dh2::character::Facts*,
 const dh2::character::AnimFacts*,std::uint32_t dt_ms,const dh2::character::Services*);
}
