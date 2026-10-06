#pragma once
#include "item_power_tables_v5.hpp"
#include <memory>

namespace dh2::data {
struct LootPowerChoiceV7 {std::int32_t power;std::int8_t probability;};
struct LootQuantityChoiceV7 {std::int16_t quantity,probability;};
struct LootPowerInputsV7 {
 Bytes powers,power_names,power_schema;
 Bytes monopoly,monopoly_names,monopoly_schema;
 // Exact NumProbArray section of the original loot cache. It is a serialized
 // count followed by its original rows, not a converted or inferred table.
 Bytes quantities,loot_names,loot_schema;
};
// One immutable resource snapshot, pinning the existing V5 power authority.
// Decoding validates all supplied definitions against that same Borrow.
class LootPowerResourcesV7 {
 struct Snapshot;std::shared_ptr<const Snapshot> snapshot_;
public:
 class Borrow {
  friend class LootPowerResourcesV7;std::shared_ptr<const Snapshot> snapshot_;
  explicit Borrow(std::shared_ptr<const Snapshot> p):snapshot_(std::move(p)){}
 public:
  Borrow()=default;explicit operator bool()const noexcept{return bool(snapshot_);}
  const ItemPowerTablesV5::Borrow& powers()const;
  const std::vector<std::vector<LootPowerChoiceV7>>& lists()const;
  const std::vector<std::vector<std::int32_t>>& monopolies()const;
  const std::vector<std::vector<LootQuantityChoiceV7>>& quantities()const;
  const std::vector<std::string>& list_names()const;
  const std::vector<std::string>& monopoly_names()const;
  const std::vector<std::string>& quantity_names()const;
 };
 bool load(const LootPowerInputsV7&,ItemPowerTablesV5::Borrow,std::string&);
 Borrow borrow()const{return Borrow(snapshot_);}
};
}
