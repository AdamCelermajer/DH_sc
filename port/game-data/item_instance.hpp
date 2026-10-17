#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace dh2::data {

// Heap-owned source item payload shared by the V4 inventory and V5 effects.
// This is only the ItemInstance value, not an inventory/savegame owner.
struct ItemInstanceV1 {
    std::int32_t id{-1};
    std::uint16_t quantity{};
    std::int32_t value{};
    std::int16_t requirement{-1};
    std::uint8_t identified{1}, buyback{};
    std::string name, description, requirements;
    std::vector<std::int32_t> powers;

    // The source reads quantity as signed16 despite storing its low16 bits.
    std::int32_t signed_quantity() const noexcept;
};

}
