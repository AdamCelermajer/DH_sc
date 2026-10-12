#pragma once
#include "character_world_attack_geometry_v1.hpp"
namespace dh2::character {
// ItemInventory C1 3ff200 constructor graph for one actual NPC. This owns the
// inventory, not a Gear inherited from another actor or a constant query view.
// World supplies its SAME immutable loot table lease and sole loot RNG. No
// random draw, item creation, equip, loot insertion or callbacks occur here.
class NpcInventoryOwnerV1 final {
 data::FreshInventoryOwnedV4 inventory_;
public:
 NpcInventoryOwnerV1(std::uintptr_t character,data::LootTablesV2::Borrow tables,
  data::LootRandom8V2& random,std::shared_ptr<data::PropertyState> properties):
  inventory_(character,std::move(tables),random,std::int8_t(-1),std::move(properties)){}
 NpcInventoryOwnerV1(const NpcInventoryOwnerV1&)=delete;
 NpcInventoryOwnerV1& operator=(const NpcInventoryOwnerV1&)=delete;
 data::FreshInventoryOwnedV4& inventory()noexcept{return inventory_;}
 const data::FreshInventoryOwnedV4& inventory()const noexcept{return inventory_;}
 skills::WorldAttackInventoryBorrowV1 attack_borrow()const noexcept{
  return {&inventory_,nullptr,nullptr,0,inventory_.properties()->resolved.data()};
 }
};
}
