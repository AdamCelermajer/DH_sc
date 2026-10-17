#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace dh2::trigger_interaction_table_v1 {

struct Bytes {
    const std::uint8_t* data{};
    std::size_t size{};
};

struct Row {
    std::string name;
    std::int32_t interaction_type{};
};

// Validates the embedded PyData arrays and retains the exact
// Arrays::TriggerObjects InteractionType field used by TriggerObject's
// GetInteractionType virtual. The names and field schemas are the original
// game_objects_pyarraynames/pystructnames streams.
class Tables {
public:
    bool load(Bytes game_objects, Bytes game_object_names,
              Bytes game_object_fields, std::string& error);
    const Row* find(std::string_view data_name,
                    std::int32_t* data_id = nullptr) const noexcept;
    std::size_t row_count() const noexcept { return rows_.size(); }

private:
    std::vector<Row> rows_;
};

} // namespace dh2::trigger_interaction_table_v1
