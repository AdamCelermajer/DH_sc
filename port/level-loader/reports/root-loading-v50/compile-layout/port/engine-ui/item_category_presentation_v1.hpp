#pragma once
#include "character_menu_inventory_order_v1.hpp"
#include <vector>
#include <string>
namespace dh2::ui {
struct ItemCategoryPresentationEntryV1 {
 std::int32_t index{};
 std::string icon,localization_symbol;
};
struct ItemCategoryPresentationServicesV1 {
 void* context{};
 bool(*constant)(void*,const char* group,const char* key,std::int32_t&,std::string&){};
};
// Category presentation only. Never substitutes for ItemTable.IconName.
// Same item/record/property borrows as source NativeInvGetItemsListForSlot.
bool item_category_presentation_v1(const data::ItemInstanceV1&,
 const data::ItemRecord164&,const data::PropertySheet&,
 const ItemCategoryPresentationServicesV1&,
 std::vector<ItemCategoryPresentationEntryV1>&,std::string&);
}
