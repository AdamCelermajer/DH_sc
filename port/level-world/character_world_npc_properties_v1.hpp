#pragma once
#include "actor_initialization.hpp"
#include "character_world_npc_object_v1.hpp"
namespace dh2::character {
// Owned source-XML inventory, separate from immutable historical CAI1. Current
// Crypt records have absent visible/static overrides; arbitrary bool parsing
// and templates remain unavailable rather than silently taking these defaults.
class CharacterWorldNpcPropertiesV1 {
 std::vector<ActorInitializationKey> keys_;
 std::string error_;
public:
 bool load(const std::uint8_t*,std::size_t,const ActorInitializationDigest& actual_dact,
  const std::vector<ActorInitializationKey>& actual_keys);
 // Source PropertyMap LoadDefaultProperties -> bool SetToDefaultValue. This
 // writes the same +80/+84 fields directly; it does not call SetVisible or FX.
 bool initialize(std::uint32_t room,const std::string& name,const std::string& character,
  WorldNpcObjectFieldsV1&);
 const std::string& error()const noexcept{return error_;}
};
}
