#pragma once
#include <cstdint>
namespace dh2::character {
// Fresh-only constructor fields absent from the existing root player graph.
// SAME FSM+3f is already CharacterConstructorCombatFieldsV1.push_death and is
// intentionally absent here. Never reset these during restore or world reload.
struct PlayerMissingDeathFieldsV56 {
 std::int32_t secondary38{}; // CSM C1 3c1b24.
 std::uintptr_t group34{}; // CharAI C1 3cedac.
 std::uintptr_t self_fx1484{},state_fx148c{},highlight14a0{};
 // Character C1 3aa4b0/3aa4c0/3aa4e8. Distinct from target circles1494/1498.
};
}
