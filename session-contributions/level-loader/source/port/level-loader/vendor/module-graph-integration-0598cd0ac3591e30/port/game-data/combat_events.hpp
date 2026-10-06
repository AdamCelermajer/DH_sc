#pragma once
#include <cstdint>
namespace dh2::data {
enum class CombatEventKind:std::int32_t {none,melee,projectile};
struct CombatEventContext {
 std::int32_t state,sequence_step,clip_step;
 std::uint32_t can_range;
 std::int32_t projectile;
};
struct CombatEventAction {
 CombatEventKind kind=CombatEventKind::none;
 std::int32_t sequence_step=0,attack_step=0,offhand=0;
};
}
// Reconstructs the state-5 attack decision in CharAI::_OnAnimEvent. Other
// event categories (FX/audio, scripts, skills/spells/interact) remain separate.
// For projectiles sequence_step carries the projectile template identifier.
extern "C" unsigned dh2_combat_event_route(dh2::data::CombatEventAction*,const dh2::data::CombatEventContext*,const char* name);
