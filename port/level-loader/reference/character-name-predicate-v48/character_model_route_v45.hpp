#pragma once
#include "canonical_character_family_v4.hpp"
#include "character_model_name_v38.hpp"
namespace dh2::loader {
struct CharacterModelRouteV45 {
 std::weak_ptr<world::CanonicalCharacterFamilyFactoryV4> actual_factory;
 // Alias points to SAME dictionary; owner pins immutable actual table backing.
 character::CharacterModelNamesV38 actual_models;
 std::function<bool(world::CanonicalCharacterRecordV4&,character::CharacterModelServicesV38&,std::string&)> main_services;
 // This route handles the NPC InitPost request only. Generic later renderer
 // queries use the original stateless model kernel through their own owners.
 bool respond(world::CanonicalCharacterRecordV4&,const character::NpcInitPostRequestV1&,
  character::NpcInitPostResponseV1&,std::string&)const;
};
}
