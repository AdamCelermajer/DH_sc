#include "character_combat_queries.hpp"
#include <cstddef>
namespace {
bool aligned(const void* p,std::size_t size){return p&&reinterpret_cast<std::uintptr_t>(p)%size==0;}
constexpr std::uint32_t limit=65536;
std::int32_t bank_id(std::int32_t property,std::uint32_t count){return property>=0&&std::uint32_t(property)<count?property:17;}
}
extern "C" int dh2_inventory_has_ranged_weapon(const dh2::character::CombatInventory16* inventory,
                                               const dh2::character::CombatItemRecord164* items,std::uint32_t count){
 if(!aligned(inventory,8)||!inventory->count||inventory->count>128||!aligned(inventory->sets,8)||
    inventory->current_set<0||inventory->current_set>127||std::uint32_t(inventory->current_set)>=inventory->count)return -1;
 const auto* reference=inventory->sets[inventory->current_set].main_hand;
 if(!reference)return 0;
 if(!aligned(reference,8)||!aligned(*reference,4)||!count||count>limit||!aligned(items,4))return -1;
 const auto id=(*reference)->item_id;if(id<0||std::uint32_t(id)>=count)return -1;
 return items[id].words[22]==4||items[id].words[22]==5;
}
extern "C" int dh2_character_can_range_attack(const dh2::character::CombatProperties896* properties,
                                             const dh2::character::CombatInventory16* inventory,
                                             const dh2::character::CombatItemRecord164* items,std::uint32_t count){
 if(!aligned(properties,4))return -1;
 if(properties->words[32]!=-1)return 1;
 return dh2_inventory_has_ranged_weapon(inventory,items,count);
}
extern "C" int dh2_character_has_combo_attack(const dh2::character::CombatProperties896* properties,
                                             const dh2::character::CombatAnimationBank8* banks,std::uint32_t count,
                                             const std::int32_t* types,std::uint32_t sequences){
 if(!aligned(properties,4)||!count||count>limit||!aligned(banks,4)||sequences>limit)return -1;
 const auto id=bank_id(properties->words[2],count);if(std::uint32_t(id)>=count)return -1;
 const auto attack=banks[id].attack_sequence;if(attack<0||std::uint32_t(attack)>=sequences)return 0;
 if(!aligned(types,4))return -1;
 return types[attack]==1;
}
namespace dh2::character {
int has_combo_attack(const CombatProperties896& properties,const data::AnimationTables& table)noexcept{
 if(table.characters.empty()||table.characters.size()>limit||table.sequences.size()>limit)return -1;
 const auto id=bank_id(properties.words[2],std::uint32_t(table.characters.size()));
 if(std::size_t(id)>=table.characters.size())return -1;
 const auto& attack=table.characters[id].fields[0];if(attack.size()!=1)return -1;
 const auto sequence=attack[0];if(sequence<0||std::size_t(sequence)>=table.sequences.size())return 0;
 return table.sequences[sequence].type==1;
}
}
