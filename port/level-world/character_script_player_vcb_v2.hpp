#pragma once
#include "../script-runtime/script_function_alias.h"
#include <cstdint>
// Actual AISPlayer::InitVCB3dd884, inherited by AISPlayerIPhone: base reset/
// OnTargetHit/OnTargetMissed, then alias-membership OnKill bit0x400. This is
// alias-map membership, never a Lua-global existence check. Caller owns flags.
extern "C" int dh2_character_script_player_vcb_v2(std::uint32_t*,const dh2_script_aliases*);
