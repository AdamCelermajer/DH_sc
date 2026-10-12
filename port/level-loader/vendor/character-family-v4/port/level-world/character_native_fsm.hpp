#pragma once
#include <cstddef>
#include "character_state.hpp"
#include "../script-runtime/script_runtime.h"
namespace dh2::character {
// Borrowed actual Character/state projection, not an ARM object overlay.
// Presence is separate from the signed ID, matching source nullable StateInfo.
struct NativeFsm24 {State* state;std::uintptr_t character;std::uint32_t current_present,reserved;};
enum NativeFsmService:std::uint32_t {fsm_profile_begin,fsm_engine_dt,fsm_set_stun,fsm_set_scare,fsm_current_update,fsm_profile_end};
struct NativeFsmRequest32 {std::uint32_t service,argument0,argument1,force;std::uintptr_t subject,payload;};
struct NativeFsmServices16 {void* context;int(*invoke)(void*,NativeFsm24*,const NativeFsmRequest32*,std::uint32_t* response);};
static_assert(sizeof(NativeFsm24)==24&&sizeof(NativeFsmRequest32)==32&&sizeof(NativeFsmServices16)==16);
// Synchronous provider returns0 delivery; nonzero native failure stops at its
// delivered prefix (-2), never accepted as a missing state implementation.
// state binding remains stable for the complete outer call. Current presence,
// ID/masks/policy and Character owner can change at recovered reload points.
// effect calls: duration UINT_MAX, original policy bool, payload0, force0.
// current update: argument0 signed ID bits, subject current Character owner.
// engine_dt response is raw application dt bits. profile requests use zeros.
// The module advances wrapping elapsed once before enforcing pending effects.
}
extern "C" {
//1 completed,-1 malformed. Selector0state,1elapsed signed32 integer bits.
int dh2_character_native_fsm_get_integer(std::int32_t*,const dh2::character::NativeFsm24*,std::uint32_t selector);
// Proved CString producer only: exact Limbus0,PreSpawn17,all other/empty3.
// Source property/loading producer remains external. Null is malformed.
int dh2_character_native_fsm_preset_state(std::int32_t*,const char*);
int dh2_character_native_fsm_update(dh2::character::NativeFsm24*,const dh2::character::NativeFsmServices16*);
// Required current-tail provider for the genuinely reconstructed four states.
// dt0 avoids a second elapsed increment. Other current IDs return-2, rather
// than pretending their original virtual methods are empty. Null-current0.
int dh2_character_native_fsm_bounded_tail(dh2::character::NativeFsm24*,const dh2::character::Facts*,const dh2::character::Services*);
// Actual script getter callback bodies: source argument count ignored, integer
// converted to float32 signed (high-bit elapsed becomes negative). Caller binds
// through genuine source-values VM bridge; outputs capacity1 required.
int dh2_character_native_fsm_script_get_state(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
int dh2_character_native_fsm_script_get_time(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
}
