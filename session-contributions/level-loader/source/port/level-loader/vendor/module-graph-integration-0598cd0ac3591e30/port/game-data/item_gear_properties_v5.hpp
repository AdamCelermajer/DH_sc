#pragma once
#include "items.hpp"
#include "properties.hpp"

namespace dh2::data {
// Logical projection of Structs::ItemPowerProperty's ARM32 +4/+8/+c
// fields. The source virtual pointer is not copied into native storage.
struct GearPowerProperty12V5 {std::int32_t type,value,extra;};
struct GearPowerView16V5 {const GearPowerProperty12V5* entries;std::uint32_t count,reserved;};
static_assert(sizeof(GearPowerProperty12V5)==12&&sizeof(GearPowerView16V5)==16);
}
extern "C" {
// Source raw sheet operations: unlike SetToSheet these do not resolve or gate
// on property type. Default equality chooses overwrite vs wrapping addition.
int dh2_gear_reset_v5(std::int32_t* gear,const std::int32_t* defaults) noexcept;
int dh2_gear_stats_v5(std::int32_t* gear,const std::int32_t* defaults,
 const dh2::data::ItemRecord164* item,std::uint32_t left_hand) noexcept;
int dh2_gear_power_v5(std::int32_t* gear,const std::int32_t* defaults,
 const dh2::data::GearPowerView16V5*,std::uint32_t left_hand) noexcept;
int dh2_gear_validate_vitals_v5(dh2::data::PropertyView*) noexcept;
}
