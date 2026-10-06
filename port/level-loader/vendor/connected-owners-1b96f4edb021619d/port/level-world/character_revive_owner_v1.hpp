#pragma once
#include "../game-data/combat_application.hpp"
#include <cstdint>
namespace dh2::character {
struct CharacterReviveBorrowV1 {
 std::uintptr_t identity{};
 data::CombatActorState* life{}; // SAME dead1449 / cue-armed1448 authority.
 std::uint8_t* remote118{};std::int32_t* network110{};
 std::uint32_t* network114{};const float* respawn_position1474{};
};
struct CharacterReviveRequestV1 {
 std::uint32_t entry{},argument0{},argument1{},reserved{};
 std::uintptr_t subject{},payload{};
};
struct CharacterReviveServicesV1 {
 void* context{};
 // 0 delivered, nonzero reached failure. Scalar responses are actual source
 // queries; position payload525508 is a mutable localXYZ, not actor storage.
 int(*invoke)(void*,const CharacterReviveRequestV1*,std::uint32_t*){};
};
struct CharacterReviveResultV1 {std::uint32_t calls{},last_entry{},completed{},reserved{};};
}
// Whole Character::Revive3a59ac. Target parameter is genuinely unused.
// 1 completed,-1 malformed before effects,-2 required service failure.
extern "C" int dh2_character_revive_v1(dh2::character::CharacterReviveResultV1*,
 const dh2::character::CharacterReviveBorrowV1*,std::uintptr_t target,
 std::uint32_t initialize_physical,const dh2::character::CharacterReviveServicesV1*);
