#pragma once
#include "items.hpp"
#include <memory>
namespace dh2::data {
struct LootEntry32V2 {std::int32_t words[8];};
struct LootSpan16V2 {const std::uint8_t* bytes;std::uint32_t count,reserved;};
struct LootRow64V2 {std::int32_t roll_type,num_random_item_probs;LootSpan16V2 random_entries,fixed_entries,sub_loots;std::uint32_t consumed,reserved;};
struct LootItemEntryV2 {std::int32_t item;std::int16_t probability;std::uint8_t quantity;};
struct LootRecordV2 {std::int32_t roll_type,num_random_item_probs;std::vector<LootEntry32V2> random_entries,fixed_entries;std::vector<std::int32_t> sub_loots;};
static_assert(sizeof(LootEntry32V2)==32&&sizeof(LootSpan16V2)==16&&sizeof(LootRow64V2)==64);
class LootTablesV2 {
 struct Snapshot;std::shared_ptr<const Snapshot> snapshot_;
public:
 class Borrow {
  friend class LootTablesV2;std::shared_ptr<const Snapshot> snapshot_;
  explicit Borrow(std::shared_ptr<const Snapshot> p):snapshot_(std::move(p)){}
 public:
  Borrow()=default;explicit operator bool()const noexcept{return bool(snapshot_);}
  const ItemTable& items()const;
  const std::vector<std::vector<LootItemEntryV2>>& item_lists()const;
  const std::vector<LootRecordV2>& loots()const;
  const std::vector<std::string>& loot_names()const;
  const std::vector<std::string>& item_list_names()const;
  std::size_t consumed()const;
 };
 // Owns the first six original loot cache sections, names and exact Loot
 // schema. Later loot-power/merchant sections remain outside this owner.
 // Atomic failure; snapshots and borrowed row identities survive owner death.
 bool load(Bytes records,Bytes names,Bytes schema,std::string&);
 Borrow borrow()const{return Borrow(snapshot_);}
};
struct LootRandom8V2 {std::uint32_t seed,calls;};
}
extern "C" {
// Source 4fe794/4fec08 stream projection. Bounded native malformed-span guards
// precede writes; returned spans point into the original serialized input.
int dh2_loot_v2_decode(dh2::data::LootRow64V2*,const std::uint8_t*,std::uint32_t) noexcept;
// Source Random.GetRandom clone401afc. Seed is supplied by the real caller;
// this helper does not invent source application seed/clock initialization.
int dh2_loot_v2_random(dh2::data::LootRandom8V2*,std::int32_t,std::int32_t*) noexcept;
}
