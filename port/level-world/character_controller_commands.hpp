#pragma once
#include <cstdint>
namespace dh2::character {
enum ControllerCommand : std::uint32_t {controller_look_object=0,controller_move_object,controller_stop,controller_head_towards_object};
enum ControllerPointCommand : std::uint32_t {
 controller_warp_to_point=0,controller_head_towards_point,controller_head_to_point
};
struct ControllerCommandState32 {
 std::uintptr_t controller,owner;
 std::uint32_t global_blocked,locked,forced,reserved;
};
struct ControllerCommandRequest24 {
 std::uint32_t command,reserved;
 std::uintptr_t owner,target;
};
struct ControllerCommandServices16 {
 void* context;
 // Dispatches the actual owner's controllable virtual synchronously.
 // Return1 completed, -1 missing/error; do not fabricate acceptance.
 int(*invoke)(void*,const ControllerCommandRequest24*);
};
struct ControllerPointCommandRequest32 {
 std::uint32_t command,reserved;
 std::uintptr_t owner;
 float point[3];std::uint32_t reserved2;
};
struct ControllerPointCommandServices16 {
 void* context;
 // Dispatches the actual owner's point-controllable virtual synchronously.
 // Return 1 when completed, -1 when the owner or dispatch is unavailable.
 int(*invoke)(void*,const ControllerPointCommandRequest32*);
};
enum CharacterControlService : std::uint32_t {
 control_is_remotely_updated=0,control_target_position,control_stop_object,
 control_character_event,control_path_to,control_look_at_point,
 // Character::Ctrl_HeadTowards(Object) and its Point3D overload. Each
 // operation is dispatched against the same Character/CharAI/Coordinator.
 control_head_null_enabled,control_is_using_skill,control_is_casting,
 control_set_heading_direction,control_raise_event,
 control_clear_ai_target,control_sync_last_ai_target
};
struct CharacterControlRequest32 {
 std::uint32_t service,argument;
 std::uintptr_t subject;
 float position[3];std::uint32_t reserved;
};
struct CharacterControlResponse16 {std::uint32_t word;float position[3];};
struct CharacterControlServices16 {
 void* context;
 int(*invoke)(void*,const CharacterControlRequest32*,CharacterControlResponse16*);
};
struct CharacterHeadTowardsFacts24 {
 std::uintptr_t character,focus_target;
 std::uint32_t focus_target_available,reserved;
};
// HeadTowards callback contract: `head_null_enabled` returns the raw
// Character+0x1b5 byte; `is_using_skill`/`is_casting` return the same machine's
// source predicates; `CharacterHeadTowardsFacts24` supplies the canonical
// focus target and exact Character identity. The kernel queries target and
// Character positions in source order, then issues clear-ai-target +
// sync-last-target when Point3D::angle exceeds 1.5708. `set_heading_direction`
// receives vector + argument=1, and `raise_event` receives argument=0. All
// callbacks are synchronous and same-Record.
static_assert(sizeof(void*)==8&&sizeof(ControllerCommandState32)==32);
static_assert(sizeof(ControllerCommandRequest24)==24&&sizeof(CharacterControlRequest32)==32);
static_assert(sizeof(ControllerPointCommandRequest32)==32);
static_assert(sizeof(CharacterControlResponse16)==16);
static_assert(sizeof(CharacterHeadTowardsFacts24)==24);
}
extern "C" {
// Complete v2Controller Cmd_LookAt(object)/Cmd_MoveTo(object)/Cmd_Stop gates.
// 1 means completed source void wrapper (including blocked); -1 malformed or
// missing synchronous service. Raw byte flags support0..255 as in the source.
int dh2_character_controller_command(const dh2::character::ControllerCommandState32*,
 std::uint32_t command,std::uintptr_t target,const dh2::character::ControllerCommandServices16*);
// v2Controller point wrappers: WarpTo, HeadTowards and HeadTo. Evaluates the
// same source gate before requiring the live controllable owner.
int dh2_character_controller_point_command(const dh2::character::ControllerCommandState32*,
 std::uint32_t command,const float point[3],
 const dh2::character::ControllerPointCommandServices16*);
// Complete Character controllable methods for the verified Character vtable.
// Its Ctrl_LookAt(Point) override tail-branches to GameObject::LookAt(Point).
// Object LookAt queries the target position before invoking that point service.
// The remote query is live and precedes MoveTo's null-target test. Stop invokes
// complete GameObject.Stop then synchronous Character event0x3f.
int dh2_character_control(std::uintptr_t owner,std::uint32_t command,
 std::uintptr_t target,const dh2::character::CharacterControlServices16*);
// Composition for a controller whose controllable is the verified Character
// implementation. Runs command gates and actual Character body; the deeper
// target/Stop/PathTo/event services remain explicit live owners.
int dh2_character_controller_character(const dh2::character::ControllerCommandState32*,
 std::uint32_t command,std::uintptr_t target,const dh2::character::CharacterControlServices16*);
// Source `v2Controller::Cmd_HeadTowards(Point3D)` gate and Character point
// override. The HeadTowards-only services above provide the live same-Record
// checks and ordered effects; no extra controller or Character state is owned.
int dh2_character_controller_character_point(const dh2::character::ControllerCommandState32*,
 const float point[3],const dh2::character::CharacterHeadTowardsFacts24*,
 const dh2::character::CharacterControlServices16*);
// Object overload includes the canonical CharAI focus target sampled from the
// same Record. A missing projection is rejected only after the controller's
// source forced/blocked/locked gate permits the command to reach Character.
int dh2_character_controller_character_head_towards(
 const dh2::character::ControllerCommandState32*,std::uintptr_t target,
 const dh2::character::CharacterHeadTowardsFacts24*,
 const dh2::character::CharacterControlServices16*);
}
