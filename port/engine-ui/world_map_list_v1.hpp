#pragma once

#include "../game-data/level_tables.hpp"
#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace dh2::ui {

struct WorldMapListRowV1 {
    std::string level_name;
    std::int32_t description_id{-1};
    std::int32_t importance{};
    std::int32_t entry_point_id{};
    std::int32_t description_string_id{-1};
    bool visible{};
};

// Ephemeral read-only projection for NativeGetWorldMapLocations. The source
// FastTravelList order and the canonical Save's two-word bitset are borrowed;
// this owns no second catalogue or unlock state.
bool world_map_list_v1(
    const std::vector<dh2::data::FastTravelDestination>&,
    const std::array<std::uint32_t, 2>& unlocked_words,
    std::vector<WorldMapListRowV1>& output, std::string& error);

}  // namespace dh2::ui
