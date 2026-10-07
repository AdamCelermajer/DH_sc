#pragma once
#include "properties.hpp"
namespace dh2::data {
// Caller-owned service facts used by Character::HitFor. These are snapshots,
// not substitutes for the game's actor/session/debug service producers.
enum HealthFacts : std::uint32_t {
 health_dead=1,health_player=2,health_monster=4,health_online=8,
 health_game_present=16,health_main_player_present=32,health_main_player_dead=64,
 health_god_monster=128,health_one_shot=256,health_child_present=512,
 health_child_player=1024,health_remote=2048
};
struct HealthRequest {
 PropertyView* properties;
 std::uint32_t damage,facts;
 std::int32_t session_state;
 std::uint32_t low_health_armed;
};
struct HealthChange {
 std::int32_t raw_add,before,after;
 std::uint32_t kill_requested,low_health_armed,low_health_cue;
 // Original ObjectBase +0x11c write. -1 means no write; 3 is a lifecycle
 // request, NOT the separate Character::IsDead byte.
 std::int32_t lifecycle_write;
 std::uint32_t skipped_dead;
};
}
// Reconstructs HitFor's health writes, gates, kill request and low-health
// hysteresis. The caller dispatches kill/audio/lifecycle requests. Actual kill
// backend, tutorial, SFX, party credit and achievements are outside this API.
// Returns 0 on success; invalid input leaves sheets and output untouched.
extern "C" unsigned dh2_health_hit(dh2::data::HealthChange*,const dh2::data::HealthRequest*);
