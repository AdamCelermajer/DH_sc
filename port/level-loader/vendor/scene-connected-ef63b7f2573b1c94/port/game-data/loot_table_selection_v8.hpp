#pragma once
#include "loot_entry_selection_v8.hpp"
#include "loot_power_creation_v7.hpp"
namespace dh2::data {
class LootTableSelectionV8 {
 LootTablesV2::Borrow tables_;LootPowerResourcesV7::Borrow powers_;
 LootRandom8V2& random_;LootEntrySelectionV8 entries_;bool running_{};
 std::vector<std::int32_t> recursion_;
 bool quantity(std::int32_t,std::int32_t&,std::string&);
 bool table(std::int32_t,std::vector<const LootEntry32V2*>&,
            std::vector<const LootEntry32V2*>*,std::string&);
 bool rolls(const std::vector<const LootEntry32V2*>&,std::int32_t,
            bool,std::vector<const LootEntry32V2*>&,std::string&);
public:
 LootTableSelectionV8(LootTablesV2::Borrow t,LootPowerResourcesV7::Borrow p,
                       LootRandom8V2& r,LootEntryServicesV8 s):
  tables_(std::move(t)),powers_(std::move(p)),random_(r),entries_(r,s){}
 // Original _AddLootTable: fixed entries, own weighted/percentage rows,
 // recursive subloot collection, then parent quantity/weighted/percentage
 // draws. Outputs borrow this retained immutable cache; reached appends and
 // shared RNG/Debug prefixes survive required service failure.
 bool select(std::int32_t,std::vector<const LootEntry32V2*>&,std::string&);
 const LootTablesV2::Borrow& tables()const noexcept{return tables_;}
};
}
