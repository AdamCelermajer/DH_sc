#pragma once
#include "character_native_fsm.hpp"
#include "../game-data/combat.hpp"
namespace dh2::character {
// Actual Character+1434/+1438 signed spawn_delay bounds, in raw milliseconds.
// Random is the caller's shared original RNG projection, not a per-body seed.
struct NativeSpawn24 {NativeFsm24* fsm;data::CombatRandom* random;std::int32_t minimum_ms,maximum_ms;};
enum SpawnSelectService:std::uint32_t {spawn_select_state,spawn_select_timer};
struct SpawnSelectRequest32 {std::uint32_t service,argument0,argument1,argument2;std::uintptr_t character,payload;};
struct SpawnSelectServices16 {void* context;int(*invoke)(void*,NativeSpawn24*,const SpawnSelectRequest32*,std::uint32_t*);};
static_assert(sizeof(NativeSpawn24)==24&&sizeof(SpawnSelectRequest32)==32&&sizeof(SpawnSelectServices16)==16);
// state arguments state1,eventUINT_MAX,0,payload0: actual _SetState, not a direct
// assignment. timer arguments duration,repeat0,event2d,payload0: actual TMR_Start
// on the Character. Delivered timer return (evenUINT_MAX) is ignored. Nonzero
// provider status fails at its delivered prefix; no rollback. Providers may
// synchronously reenter; keep FSM, field projection and global RNG alive.
}
extern "C" {
// Actual SM_SetSpawnState3c2734. delay_enabled is source truthy uint32; the
// second source bool is unused and preserved as an ignored input. Delay false
// keeps bounds/RNG unchanged and requests Spawn1 immediately. Delay true clamps
// minimum>=0 and maximum>=minimum; zero interval selects Spawn1, positive bounds
// start a timer, unequal bounds consume genuine shared Random once.1complete,
//-1malformed atomic,-2 required source provider failure after delivered effects.
int dh2_character_spawn_select(dh2::character::NativeSpawn24*,std::uint32_t delay_enabled,std::uint32_t ignored_mode,const dh2::character::SpawnSelectServices16*);
}
