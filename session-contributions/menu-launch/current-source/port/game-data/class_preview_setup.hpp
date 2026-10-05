#pragma once
#include "class_tables.hpp"
#include "properties.hpp"
#include "loot_tables_v2.hpp"
#include "animation_tables.hpp"
#include <array>
namespace dh2::data {
// Authored inputs to MenuCharacterSelect's three temporary actors. This
// describes starting items; it does not deliver loot or equip an inventory.
struct ClassPreviewItem {
 std::int32_t item=-1;
 std::uint8_t quantity=0;
 std::string identifier,module;
};
struct ClassPreviewDefinition {
 std::string character;
 std::int32_t row=-1,loot=-1,animation_table=-1;
 PropertyState properties;
 std::vector<ClassPreviewItem> starting_items;
 std::string idle_clip,select_clip,template_clip;
};
bool class_preview_definitions(const CharacterTable&,const ClassTables&,
 const PropertyRules&,const LootTablesV2::Borrow&,const AnimationTables&,
 const Dictionary&,std::array<ClassPreviewDefinition,3>&,std::string&);
}
