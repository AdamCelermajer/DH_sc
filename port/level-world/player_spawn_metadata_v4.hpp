#pragma once
#include "canonical_object_factory_v1.hpp"
#include <string>
namespace dh2::player {
// Exact inputs produced by original PlayerManager::_AddCharacter372220.
// This is metadata derivation, not execution of Spawn/AddCharacter or a new
// player/base allocation. The id is the actual AddCharacter input argument.
struct PlayerSpawnMetadataV4 {
 std::string name;
 const world::CanonicalFactoryEntryV1* factory{};
 const char* archetype{};
 std::int32_t room{-1};
 bool deferred{true},network{true};
};
bool player_spawn_metadata_v4(std::int32_t actual_internal_input,
 PlayerSpawnMetadataV4&,std::string&);
}
