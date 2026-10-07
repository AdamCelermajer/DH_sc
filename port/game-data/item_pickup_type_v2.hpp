#pragma once
#include "item_inventory_v1.hpp"
namespace dh2::data {
// The frozen V1 projection named source ItemInstance signed16+58
// "requirement". Original GetPickUpType3f9e34 proves its real meaning:
// override -1 delegates to actual ItemTable row word3. Keep ABI/storage and
// expose its source meaning; never allocate a second override authority.
inline std::int16_t& item_pickup_override58_v2(ItemInstanceV1& item)noexcept{
 return item.requirement;
}
inline const std::int16_t& item_pickup_override58_v2(const ItemInstanceV1& item)noexcept{
 return item.requirement;
}
inline std::int32_t item_pickup_type_v2(const ItemInstanceV1& item,const ItemRecord164& actual_row)noexcept{
 const auto override=item_pickup_override58_v2(item);
 return override==-1?actual_row.words[3]:std::int32_t(override);
}
}
