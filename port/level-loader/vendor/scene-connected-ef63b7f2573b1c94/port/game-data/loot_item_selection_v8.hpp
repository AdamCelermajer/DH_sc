#pragma once
#include "loot_entry_selection_v8.hpp"
namespace dh2::data {
struct LootItemInfoV8 {
 std::int16_t id{};
 const LootEntry32V2* entry{};
 std::uint8_t quantity{};
 const Item* item{};
};
// Source 401dbc and 402dfc. Immutable table/entry identities remain borrowed;
// selected quantity is the authored byte, not a new quantity roll.
class LootItemSelectionV8 {
 LootTablesV2::Borrow tables_; LootRandom8V2& random_;
 LootEntrySelectionV8 entries_; bool running_{};
 bool append(const LootEntry32V2&,const LootItemEntryV2&,std::uint32_t,
             std::vector<LootItemInfoV8>&,std::string&);
public:
 LootItemSelectionV8(LootTablesV2::Borrow t,LootRandom8V2& r,LootEntryServicesV8 s):
  tables_(std::move(t)),random_(r),entries_(r,s){}
 bool weighted_index(const std::vector<LootItemEntryV2>&,std::uint32_t&,std::string&);
 bool expand(const std::vector<const LootEntry32V2*>&,bool give_all,
             std::vector<LootItemInfoV8>&,std::string&);
};
}
