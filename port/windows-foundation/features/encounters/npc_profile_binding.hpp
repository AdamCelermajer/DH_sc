#pragma once
#include "../../../game-data/character_templates_v78.hpp"
#include "../../../level-world/character_props_id_owner_v1.hpp"
namespace dh::foundation::encounters {
struct NpcProfileSelection {
 std::int32_t row=-1;
 std::string name;
};
// Borrow SAME actor caches and SAME application RNG. Selection does not admit,
// spawn, enable, change authored properties, or manufacture property state.
bool select_npc_profile(const dh2::data::CharacterTable&,
 const dh2::data::CharacterTemplateTableV78&,const std::string& authored_array,
 const std::string& authored_name,std::int16_t& same13c8,
 std::int16_t& same13ca,dh2::data::LootRandom8V2& same_random,
 NpcProfileSelection&,std::string&);
}
