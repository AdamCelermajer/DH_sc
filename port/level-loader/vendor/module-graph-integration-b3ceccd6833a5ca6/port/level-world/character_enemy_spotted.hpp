#pragma once
#include "character_target_events.hpp"
namespace dh2::character {
struct EnemySpottedState16 {TargetEventState32* ai;std::uintptr_t group;};
enum EnemySpottedService : std::uint32_t {enemy_debug_load=0,enemy_debug_construct,enemy_debug_query,enemy_debug_destroy,enemy_group_spotted,enemy_awaiting_spawn,enemy_in_limbus,enemy_in_combat,enemy_is_player,enemy_get_aggro,enemy_add_aggro,enemy_active_dispatch};
struct EnemySpottedRequest48 {std::uint32_t service,reserved;std::uintptr_t subject,other,enemy;const char* text;std::uint32_t word,reserved2;};
struct EnemySpottedServices24 {
 void* context;
 int(*invoke)(void*,EnemySpottedState16*,const EnemySpottedRequest48*,std::uint32_t* result_word);
 // Live Arrays::DesignSettingsTable first-row EnemySpottedAggro float bits
 // (original runtime+0x30). Read only after GetAggro==0 at source reload point.
 const std::uint32_t* initial_threat;
};
static_assert(sizeof(EnemySpottedState16)==16&&sizeof(EnemySpottedRequest48)==48&&sizeof(EnemySpottedServices24)==24);
}
// Complete CharAI.OnEnemySpotted3d14b4 prefix. The captured enemy is a genuine
// nonnull Character identity. Group, SM, combat, player, aggro and selected AIS
// services remain synchronous owned-provider boundaries; no accepted fallback.
// add_aggro subject is owner of captured embeddedAI, enemy is original input,
// word is exact authored threat bits. Get/AddAggro replies are float bits.
// active_dispatch is AIS virtual+34 after prefixes. Upstream existing complete
// AI event dispatcher supplies original event9 gates and following FSM delivery.
// 0 complete/source skip;1 malformed entry atomic;2 provider/lifetime failure.
extern "C" int dh2_character_enemy_spotted(dh2::character::EnemySpottedState16*,std::uintptr_t enemy,const dh2::character::EnemySpottedServices24*);
