#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace dh2::destructible_container_table_v1 {

struct Bytes {
    const std::uint8_t* data{};
    std::size_t size{};
};

// Named interpretation of Structs::DestructibleContainer's serialized row.
// DataID and Visual are array-name indices, as used by the native callers.
struct Row {
    std::int32_t air_resist{};
    std::int32_t damage_reduction{};
    std::int32_t earth_resist{};
    std::int32_t effect{};
    std::int32_t fire_resist{};
    std::int32_t hit_points{};
    std::int32_t interact_sound{};
    std::uint8_t keep_solid{};
    std::int32_t lightning_resist{};
    std::int32_t loot{};
    std::string script;
    std::int32_t trap_chance{};
    std::int32_t trap_type{};
    std::int32_t visual{};
    std::int32_t water_resist{};
};

class Tables {
public:
    // Reads the first PyArray section (DestructibleContainers) with its first
    // pyarraynames/pystructnames sections, plus the complete GameObjectDict.
    // All table views are owned by this object after a successful load.
    bool load(Bytes game_objects, Bytes game_object_names, Bytes game_object_fields,
              Bytes visual_dictionary, Bytes visual_dictionary_names,
              std::string& error);
    const Row* find(std::string_view data_desc,
                    std::int32_t* data_id = nullptr) const noexcept;
    const std::string* visual_path(std::int32_t visual_id) const noexcept;
    const std::string* data_desc(std::int32_t data_id) const noexcept;
    std::size_t row_count() const noexcept { return rows_.size(); }

private:
    std::vector<std::string> data_names_;
    std::vector<Row> rows_;
    std::vector<std::string> visual_names_;
    std::vector<std::string> visual_paths_;
};

} // namespace dh2::destructible_container_table_v1
