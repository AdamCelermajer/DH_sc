#pragma once
#include "../script-runtime/script_runtime.h"
#include <cstdint>
namespace dh2::character {
enum ScriptCommand : std::uint32_t {script_stop,script_head_to,script_move_to,script_attack,script_flee,script_has_path};
// Genuine live Character controller/target, raw GameObject position, Vec3f_K
// and route-list count. All borrowed records/point backings survive callbacks.
struct ScriptCommandState48 {
 std::uintptr_t character,controller,target;
 const float* position;const float* axis;
 std::uint32_t path_count,reserved;
};
enum ScriptCommandService : std::uint32_t {
 script_controller_stop,script_controller_move_object,script_controller_head_point,
 script_controller_move_point,script_controller_attack,script_target_position,script_look_vector
};
struct ScriptCommandRequest40 {
 std::uint32_t service,reserved;std::uintptr_t subject,target;float point[3];std::uint32_t reserved1;
};
struct ScriptCommandServices24 {
 void* context;
 // 0 genuine delivery; nonzero missing/error. Point getters return a borrowed
 // position pointer, allowing later synchronous source calls to mutate it.
 int(*invoke)(void*,ScriptCommandState48*,const ScriptCommandRequest40*,const float**);
 // Actual Value.getNumber conversion, not Lua's ordinary tonumber policy.
 // It may invoke genuine string/numeric-identity dependencies synchronously.
 int(*number)(void*,const dh2_script_value*,float*);
};
struct ScriptCommandBindings40 {
 ScriptCommandState48* state;ScriptCommandServices24 services;
 const dh2_script_callback_scope* scope;
};
static_assert(sizeof(ScriptCommandState48)==48&&sizeof(ScriptCommandRequest40)==40);
static_assert(sizeof(ScriptCommandServices24)==24&&sizeof(ScriptCommandBindings40)==40);
}
extern "C" {
// Original Lua wrapper bodies only. They do not replace the supplied controller,
// FindPath, attack, target-position or Character event services. The original
// zero-argument HeadTo/MoveTo asserts then reads invalid argument storage;
// native returns -3 explicitly without inventing a destination.
// 1 complete (including source argument rejection), -1 malformed atomic,
// -2 unavailable/failed required service at its source prefix, -3 source assert.
int dh2_character_script_command(dh2::character::ScriptCommandState48*,std::uint32_t,
 const dh2_script_value*,std::uint32_t,const dh2::character::ScriptCommandServices24*,
 dh2_script_value*,std::uint32_t,std::uint32_t*);
}
