#pragma once
#include "../game-data/combat_result.hpp"
#include <cstdint>
namespace dh2::character {
struct DotPlayerReactionServicesV7 {
 void* context{};
 // Same defender FSM's SM_IsIdle(false), not an animation/skill guess.
 int(*is_idle)(void*,std::uintptr_t,bool*){};
};
// Exact F_ApplyResult player-only prefix 3b17ec..3b1830. Caller has already
// reached the alive target's genuine IsPlayer query. This changes only source
// AttackResult outcome bits; subsequent dodge/injure execution remains required.
// 1 delivered, -1 malformed, -2 missing/failed FSM provider.
int dot_player_reaction_v7(data::CombatResult*,std::uintptr_t defender,
 const DotPlayerReactionServicesV7*);
}
