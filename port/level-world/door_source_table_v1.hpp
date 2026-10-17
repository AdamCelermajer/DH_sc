#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace dh2::door_source_table_v1 {

struct Bytes { const std::uint8_t* data{}; std::size_t size{}; };

// Structs::Door row from game_objects_pyarray: the source member-name key,
// SoundClose, SoundOpen and Visual (GameObjectDict index).
struct Row {
    std::string script;
    std::int32_t sound_close{-1};
    std::int32_t sound_open{-1};
    std::int32_t visual{-1};
};

class Tables {
public:
    bool load(Bytes game_objects, Bytes game_object_names,
              Bytes game_object_fields, Bytes visual_dictionary,
              Bytes visual_dictionary_names, std::string& error);
    const Row* find(std::string_view data_name,
                    std::int32_t* data_id = nullptr) const noexcept;
    const std::string* visual_path(std::int32_t visual_id) const noexcept;
    const std::string* data_name(std::int32_t data_id) const noexcept;
    std::size_t row_count() const noexcept { return rows_.size(); }

private:
    std::vector<std::string> names_;
    std::vector<Row> rows_;
    std::vector<std::string> visual_names_;
    std::vector<std::string> visual_paths_;
};

} // namespace dh2::door_source_table_v1
