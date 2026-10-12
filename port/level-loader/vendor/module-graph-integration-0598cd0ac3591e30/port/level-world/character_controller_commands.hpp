#pragma once
#include <cstdint>
namespace dh2::character {
enum ControllerCommand : std::uint32_t {controller_look_object=0,controller_move_object,controller_stop};
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
enum CharacterControlService : std::uint32_t {
 control_is_remotely_updated=0,control_target_position,control_stop_object,
 control_character_event,control_path_to,control_look_at_point
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
static_assert(sizeof(void*)==8&&sizeof(ControllerCommandState32)==32);
static_assert(sizeof(ControllerCommandRequest24)==24&&sizeof(CharacterControlRequest32)==32);
static_assert(sizeof(CharacterControlResponse16)==16);
}
extern "C" {
// Complete v2Controller Cmd_LookAt(object)/Cmd_MoveTo(object)/Cmd_Stop gates.
// 1 means completed source void wrapper (including blocked); -1 malformed or
// missing synchronous service. Raw byte flags support0..255 as in the source.
int dh2_character_controller_command(const dh2::character::ControllerCommandState32*,
 std::uint32_t command,std::uintptr_t target,const dh2::character::ControllerCommandServices16*);
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
}
