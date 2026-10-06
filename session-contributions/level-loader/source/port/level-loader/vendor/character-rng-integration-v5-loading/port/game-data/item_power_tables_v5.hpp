#pragma once
#include "item_gear_properties_v5.hpp"
#include <memory>
namespace dh2::data {
struct ItemPowerScalars28V5 {std::int32_t palette,special_effect,description,gold_multiplier,sorting_order,gold_bonus,monopoly;};
struct ItemPowerDecoded48V5 {ItemPowerScalars28V5 scalars;std::uint32_t count;const std::uint8_t* entries;std::uint32_t consumed,reserved;};
static_assert(sizeof(ItemPowerScalars28V5)==28&&sizeof(ItemPowerDecoded48V5)==48);
struct ItemPowerRecordV5 {ItemPowerScalars28V5 scalars{};std::vector<GearPowerProperty12V5> properties;};
// Retained immutable original cache table, including exact ordered entries.
// Serialized Palette is one byte; entry Flags is a full signed32 word.
class ItemPowerTablesV5 {
 struct Snapshot;std::shared_ptr<const Snapshot> snapshot_;
public:
 class Borrow {std::shared_ptr<const Snapshot> snapshot_;friend class ItemPowerTablesV5;explicit Borrow(std::shared_ptr<const Snapshot> p):snapshot_(std::move(p)){}
 public:Borrow()=default;const std::vector<ItemPowerRecordV5>& rows()const;const std::vector<std::string>& names()const;explicit operator bool()const noexcept{return bool(snapshot_);}};
 bool load(Bytes,Bytes names,Bytes schema,std::string& error);Borrow borrow()const{return Borrow(snapshot_);}
};
}
extern "C" int dh2_item_power_decode_v5(dh2::data::ItemPowerDecoded48V5*,const std::uint8_t*,std::uint32_t) noexcept;

