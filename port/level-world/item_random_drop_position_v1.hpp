#pragma once

#include <string>

#include "../game-data/loot_tables_v2.hpp"

namespace dh2::item_random_drop_position_v1 {

struct Point {
    float x{};
    float y{};
    float z{};
};

// ItemObject::_GetRandomDropPos at 0x3ec668 uses the process Random stream
// through this borrowed service. The service is the same process_state owner
// used by inventory creation; no RNG state is created by this helper.
bool generate(const Point& center, const Point* killer,
              data::InventoryRandomServiceV4 random, Point* output,
              std::string& error);

}  // namespace dh2::item_random_drop_position_v1
