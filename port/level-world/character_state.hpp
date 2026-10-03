#pragma once
#include <cstdint>
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
enum Service : std::uint32_t {
 stop=1,pin,unpin,set_animation,set_speed,swap_animation,stop_loop,
 start_timer,look_at,remove_highlight,set_death_filter,reset_filter,
 disable_state_fx,disable_self_fx,remove_buffs,cancel_sneaking,
 raise_event,remove_body,idle_common_update,set_heading
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
// three IEEE float words; scalar1 means enabled. All other services use zeros.
// Callback is synchronous and may change State or reenter the coordinator.
// Animation/body/timer/AI/FX services must preserve the emitted call order.
using Callback=void(*)(void*,State*,const Request*);
struct Services {void* context;Callback invoke;};
static_assert(sizeof(State)==56&&sizeof(Facts)==96);
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
}
