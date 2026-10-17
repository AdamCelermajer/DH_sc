#include "world_map_list_v1.hpp"

namespace dh2::ui {

bool world_map_list_v1(
    const std::vector<dh2::data::FastTravelDestination>& source,
    const std::array<std::uint32_t, 2>& unlocked_words,
    std::vector<WorldMapListRowV1>& output, std::string& error) {
    if (source.size() > 64) {
        error = "FastTravelList exceeds the source 64-bit unlock set";
        return false;
    }
    try {
        std::vector<WorldMapListRowV1> rows;
        rows.reserve(source.size());
        for (std::size_t i = 0; i < source.size(); ++i) {
            const auto& row = source[i];
            rows.push_back({row.level_name, row.description_id,
                            row.location_type, row.entrypoint_id,
                            row.string_id,
                            (unlocked_words[i >> 5] &
                             (std::uint32_t(1) << (i & 31))) != 0});
        }
        output = std::move(rows);
        error.clear();
        return true;
    } catch (...) {
        error = "FastTravelList UI projection allocation failed";
        return false;
    }
}

}  // namespace dh2::ui
