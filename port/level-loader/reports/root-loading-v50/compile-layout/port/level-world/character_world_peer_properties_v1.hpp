#pragma once
#include "character_world_npc_object_v1.hpp"
#include "actor_initialization.hpp"
#include "objects.hpp"
namespace dh2::character {
class CharacterWorldPeerPropertiesV1 {
 struct Key {std::uint32_t index,room;std::string name;};
 std::vector<Key> keys_;std::string error_;
public:
 bool load(const std::uint8_t*,std::size_t,const ActorInitializationDigest& actual_dact,
  const std::vector<objects::Record>&);
 // Actual AnimatedDecor factory342600 static1/type14 then inherited source
 // DeclareProperties->LoadDefaultProperties visible1. XML inventory has no
 // visible/static/condition override. No body or animation factory is claimed.
 bool initialize_decor(std::uint32_t descriptor_index,WorldNpcObjectFieldsV1&,
  std::uint32_t& same_object_type_f4);
 const std::string& error()const noexcept{return error_;}
};
// Exact inherited Character default-property producer for fresh constructor
// fields. Call only at the real LoadDefaultProperties stage; this is not the
// whole player/NPC initialization or a visibility policy.
bool character_object_default_properties_v1(WorldNpcObjectFieldsV1&,std::string&);
}
