#pragma once
#include "character_timers.hpp"
#include "../script-runtime/script_game_bindings.h"

namespace dh2::character {
// Port-owned adapter, not an original object layout. Every pointer is borrowed
// and must remain stable until the VM is destroyed or its globals replaced.
// Expiry dispatch remains the caller's genuine selected-AIS/event responsibility.
struct ScriptTimerBridge {
 dh2_script_game_bindings bindings{};
 TimerStore32* timers=nullptr;
 const TimerServices32* services=nullptr;
 std::int32_t diagnostic=0;
};
static_assert(sizeof(ScriptTimerBridge)==64,"native borrowed timer bridge ABI");
}
// Installs the source-coercion StartTimer/StopTimer/Trace callbacks against the
// actual native CharacterTimers. Invalid native storage/allocation is surfaced
// as a protected port error, never returned to Lua as a negative timer ID.
// This does not create/publish an active AIS or initialize a LuaManager.
extern "C" int dh2_character_script_bind_timers(dh2_script_vm*,
 dh2::character::ScriptTimerBridge*);
