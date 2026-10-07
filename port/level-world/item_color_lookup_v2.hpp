#pragma once
#include "character_game_design.hpp"
#include "../game-data/item_inventory_v1.hpp"
namespace dh2::character {
struct ItemColorLookupServicesV2 {
 CharacterGameDesign::Borrow design;
 void* context{};
 bool(*font_text_color)(void*,std::int32_t row,std::uint32_t& color,std::string&){};
};
bool item_color_lookup_v2(const data::ItemInstanceV1&,const ItemColorLookupServicesV2&,std::uint32_t&,std::string&);
}
