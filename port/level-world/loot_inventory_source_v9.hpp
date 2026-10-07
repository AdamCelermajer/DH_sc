#pragma once
#include "../game-data/loot_temporary_inventory_v8.hpp"
#include "../game-data/fresh_inventory_owned_v4.hpp"
#include <functional>
namespace dh2::character {
// Synchronous view of one actual original ItemInventory. No copied slot/item
// vector or second temporary inventory is created by this transport.
struct LootInventorySourceV9 {
 std::shared_ptr<void> owner;
 std::function<std::size_t()> count;
 std::function<data::ItemInstanceV1*(std::uint32_t)> item;
 std::function<const data::Item*(const data::ItemInstanceV1&)> metadata;
 std::function<bool(std::uint32_t,bool,bool,void*,
  bool(*)(void*,std::unique_ptr<data::ItemInstanceV1>&,bool,bool,std::int32_t&,std::string&),
  std::int32_t&,std::string&)> transfer;
};
bool fresh_inventory_loot_source_v9(data::FreshInventoryOwnedV4&,
 std::shared_ptr<void>,LootInventorySourceV9&,std::string&);
}
