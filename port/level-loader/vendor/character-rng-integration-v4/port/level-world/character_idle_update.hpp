#pragma once
#include <cstdint>
namespace dh2::character {
// Borrowed mutable source fields: current GameObject position+160, controller
// +378, Character state-time+55c and heading-active byte+1b5. No ARM overlay.
struct IdleCharacter40 {
 std::uintptr_t identity,controller;
 float position[3];
 std::uint32_t state_time_ms,heading_active,reserved;
};
enum IdleUpdateService:std::uint32_t {
 idle_raise_event=0,idle_is_player,idle_online_disabled,idle_delay,
 idle_player_count,idle_player_character,idle_get_state,idle_distance,
 idle_move_to
};
struct IdleUpdateRequest32 {
 std::uint32_t service,argument;
 std::uintptr_t subject;
 float point[3];
 std::uint32_t reserved;
};
struct IdleUpdateResponse16 {std::uintptr_t identity;std::uint32_t word,reserved;};
struct IdleUpdateServices16 {
 void* context;
 int(*invoke)(void*,IdleCharacter40*,const IdleUpdateRequest32*,IdleUpdateResponse16*);
};
static_assert(sizeof(IdleCharacter40)==40&&sizeof(IdleUpdateRequest32)==32&&sizeof(IdleUpdateResponse16)==16&&sizeof(IdleUpdateServices16)==16);
// Synchronous, required services return0 delivery. Nonzero stops at the
// delivered prefix (-2); never treat missing provider as an accepted no-op.
// delay/distance are exact CharacterDesign/PlayerInterPenetration_Delay/Dist
// signed32 constant bits. Delay is compared unsigned to source state-time.
// player_count captures current manager identity and signed count together;
// player_character gets manager argument(index,false) and returns its nullable
// Character projection, including original Player+660 pointer load. get_state
// returns signed StateInfo ID (null =>-1). online_disabled returns source byte5.
// event argument0/payloadNULL; move_to subject is the current controller owner.
// Root Character identity/binding stays stable across calls; position, controller
// and state-time can change synchronously and are reread at source points.
// Candidate storage and captured manager remain alive through the complete call.
}
extern "C" {
// Full source IdleCommonUpdate / OnUpdate tail. No elapsed/dt advancement.
//1 complete,-1 malformed before callbacks,-2 required service/provider failure.
int dh2_character_idle_update(dh2::character::IdleCharacter40*,const dh2::character::IdleUpdateServices16*);
}
