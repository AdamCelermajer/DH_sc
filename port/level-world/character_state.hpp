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
 combat_machine_flags
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
int dh2_character_combat_skill_state_transition(dh2::character::State*,const dh2::character::Facts*,
 const dh2::character::CombatFacts*,dh2::character_skill_state_dispatch_v1::Projection*,
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
// Limbus and CSSpawn OnUpdate are source no-ops; this never invents a timeout
// or idle transition. Return 0 for a valid no-op and -1 for malformed input.
int dh2_character_spawn_update(const dh2::character::State*,
 const dh2::character::SpawnFacts*);
}
