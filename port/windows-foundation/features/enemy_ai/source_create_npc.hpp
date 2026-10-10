#pragma once

#include "../../../level-world/canonical_class_receiver_bindings_v1.hpp"
#include "../../../level-world/canonical_spawn_owner_v1.hpp"
#include "../../../game-data/data.hpp"

#include <cstdint>
#include <functional>
#include <string>

namespace dh2::enemy_ai {

struct SourceCreateNpcServicesV1 {
 const data::CharacterTable* characters{};
 // Must execute the SAME source ObjectManager::Spawn and canonical Character
 // factory/Add path. `deferred` and `network` are kept distinct from the later
 // InitPost/online Summon tail.
 std::function<bool(const char*,const char*,bool,bool,
  world::CanonicalClassReceiverV1&,bool&,std::string&)> source_manager_spawn;
 std::function<bool(world::CanonicalClassReceiverV1&,bool,std::string&)> set_idle_state;
};

struct SourceCreateNpcResultV1 {
 world::CanonicalClassReceiverV1 receiver;
 std::string generated_name;
 std::int32_t character_id{-1};
 bool created{};
};

// Reusable adapter for the source ObjectManager::Spawn stage. This delegates
// to the same canonical Character factory, PropertyMap and ObjectManager; the
// outer CreateNPC kernel then performs the source Character-name/InitPost/
// InitFinal/idle continuation.
struct SourceCanonicalSpawnOwnerV1 {
 world::CanonicalObjectManagerV1* manager{};
 world::CanonicalPropertyMapV1* properties{};
 world::CanonicalSpawnServicesV1 spawn;
};
bool source_canonical_manager_spawn_v1(SourceCanonicalSpawnOwnerV1&,
 const char*,const char*,bool,bool,world::CanonicalClassReceiverV1&,bool&,std::string&);

// Character::CreateNPC(int,const char*,bool), 0x3ad1f8, including the
// CharacterTable lookup and source overloaded body 0x3ad064. The caller owns
// the canonical manager/factory and lends its exact ObjectManager::Spawn path.
bool source_create_npc_v1(std::int32_t character_id,const char* supplied_name,
 bool description_field,bool network,const SourceCreateNpcServicesV1&,
 SourceCreateNpcResultV1&,std::string&);

} // namespace dh2::enemy_ai
