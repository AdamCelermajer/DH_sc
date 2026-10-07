#pragma once
#include "../game-data/fresh_inventory_owned_v4.hpp"
namespace dh2::ui {
// Borrowed source sort/list entries: identities stay in the authoritative V4.
bool character_menu_slot_candidate_v1(const data::ItemInstanceV1&,
 const data::ItemRecord164&,const data::PropertySheet&,std::uint32_t slot)noexcept;
bool character_menu_item_name_less_v1(const data::ItemInstanceV1&,
 const data::ItemInstanceV1&)noexcept;
bool character_menu_item_value_less_v1(const data::ItemInstanceV1&,
 const data::ItemRecord164&,const data::ItemInstanceV1&,
 const data::ItemRecord164&,std::int32_t class_index)noexcept;
bool character_menu_item_equipment_less_v1(const data::ItemInstanceV1&,
 const data::ItemRecord164&,const data::ItemInstanceV1&,
 const data::ItemRecord164&,std::int32_t class_index,bool available_a,
 bool available_b,bool equipped_pair_a,bool equipped_pair_b,
 bool equipped_requested_a)noexcept;
}
