#pragma once
#include "character_script_call_timer.hpp"
#include "character_script_selection.hpp"
// Complete source initial virtual bodies: slots8/c/10/14 are Init/Post/Final/
// Terminate. All six captured classes inherit empty initial bodies except
// AISExternal, which calls the literal name via its own alias map/private VM.
// 0 delivered; -1 malformed caller; VM diagnostics are returned unchanged.
// These are native diagnostics, not fabricated source script acceptance.
extern "C" int dh2_character_script_initial_virtual(
 const dh2::character::ScriptTimerCall16*,std::uint32_t kind,std::uint32_t slot);
// Actual Default InitVCB resets flags, tests alias membership for TargetHit/
// TargetMissed; external extends with10 ordered alias tests. This NEVER asks
// whether a Lua global exists. No VM or provider callbacks are needed here.
extern "C" int dh2_character_script_init_vcb(
 std::uint32_t* flags,const dh2_script_aliases*,std::uint32_t external);
