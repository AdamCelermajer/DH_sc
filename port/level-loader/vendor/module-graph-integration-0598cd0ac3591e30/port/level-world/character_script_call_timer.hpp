#pragma once
#include <cstdint>
#include "../script-runtime/script_function_alias.h"
namespace dh2::character {
// Borrow both from the SAME source LuaScript/session. The caller resolves the
// active AIS before calling; this pair neither owns nor publishes an AIS.
struct ScriptTimerCall16 {
 dh2_script_vm* vm;
 const dh2_script_aliases* aliases;
};
static_assert(sizeof(void*)==8&&sizeof(ScriptTimerCall16)==16);
}
// AISDefault::OnScriptTimer's signed32 -> float32 argument, source alias lookup
// and fresh Lua global call with ordered discarded-return projection.
// 0 success; -1 malformed borrowed pair; VM protected errors are propagated.
// No same-VM public C ABI reentry, constructor or manager ownership is supplied.
extern "C" int dh2_character_script_call_timer(
 const dh2::character::ScriptTimerCall16*,std::uint32_t timer_id);
