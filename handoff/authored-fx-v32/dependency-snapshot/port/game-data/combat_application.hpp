#pragma once
#include "combat_result.hpp"
#include "health.hpp"
#include "vitals.hpp"
namespace dh2::data {
struct CombatActorState {
 std::uint32_t dead=0,low_health_armed=1,combo_hits=0,push_death=0;
 std::int32_t lifecycle=0;
};
struct MonsterApplicationRequest {
 CombatResult* result;
 PropertyView *attacker,*defender;
 CombatActorState *attacker_state,*defender_state;
};
enum CombatStatusRequests : std::uint32_t {
 request_dot=1,request_dodge=2,request_block=4,request_hurt=8,
 request_push=16,request_stun=32,request_fear=64,request_slow=128
};
struct MonsterApplication {
 HealthChange health{};
 VitalsChange hp_leech{},mp_leech{};
 float threat=0;
 std::uint32_t hit_called=0,status_requests=0;
 std::int32_t dot_duration=0,dot_amount=0,dot_element=0,stun_duration=0,fear_duration=0,slow_duration=0;
 std::uint32_t special=0,push=0;
};
}
// Offline, single-player, nonplayer F_ApplyResult path: direct health owner,
// default debug switches, living main player, no inventory-gold damage mask.
// Mutates combo, HP/MP, core kill/dead/lifecycle state and result outcomes.
// Threat and ordered status requests are returned for their separate services;
// full aggro, buffs/FSM, FX/audio, kill rewards and AI callbacks remain pending.
// Invalid input leaves all inputs/output unchanged.
extern "C" unsigned dh2_combat_apply_monster(dh2::data::MonsterApplication*,const dh2::data::MonsterApplicationRequest*);
// Same offline direct-owner core with a player attacker and nonplayer target.
// No active critical-triggered skill. Player achievements, combat text/audio,
// aggro/status services and the remainder of the kill backend stay external.
extern "C" unsigned dh2_combat_apply_player_to_monster(dh2::data::MonsterApplication*,const dh2::data::MonsterApplicationRequest*);
// Offline nonplayer attacker -> player defender, direct owner, default debug
// settings and no inventory gold mask. player_idle is the SM_IsIdle(false)
// service snapshot (0 or 1). Includes original idle reaction result bits and
// HitFor low-health hysteresis. Player stats/tutorial/audio remain external.
extern "C" unsigned dh2_combat_apply_monster_to_player(dh2::data::MonsterApplication*,const dh2::data::MonsterApplicationRequest*,unsigned player_idle);
