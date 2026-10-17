#include "destructible_container_table_v1.hpp"

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <utility>

namespace dh2::destructible_container_table_v1 {
namespace {

constexpr std::size_t kMaxBytes = 8u * 1024u * 1024u;
constexpr std::uint32_t kMaxRows = 4096;
constexpr std::uint32_t kMaxString = 4096;

class Reader {
public:
    explicit Reader(Bytes bytes) : bytes_(bytes) {
        if (!bytes.data || bytes.size < 4 || bytes.size > kMaxBytes)
            throw std::runtime_error("Container source table byte span is invalid");
    }

    std::uint32_t word() {
        require(4);
        const auto* value = bytes_.data + cursor_;
        cursor_ += 4;
        return std::uint32_t(value[0]) |
            (std::uint32_t(value[1]) << 8) |
            (std::uint32_t(value[2]) << 16) |
            (std::uint32_t(value[3]) << 24);
    }

    std::int32_t signed_word() {
        const auto raw = word();
        std::int32_t value{};
        std::memcpy(&value, &raw, sizeof(value));
        return value;
    }

    std::uint8_t byte() {
        require(1);
        return bytes_.data[cursor_++];
    }

    std::string string(std::uint32_t length) {
        if (length > kMaxString) throw std::runtime_error("Container source string exceeds limit");
        require(length);
        std::string value(reinterpret_cast<const char*>(bytes_.data + cursor_), length);
        cursor_ += length;
        if (value.find('\0') != std::string::npos)
            throw std::runtime_error("Container source string contains an embedded NUL");
        return value;
    }

    std::vector<std::string> strings(bool unique) {
        const auto count = word();
        if (count == 0 || count > kMaxRows)
            throw std::runtime_error("Container source name count exceeds limit");
        std::vector<std::string> values;
        values.reserve(count);
        for (std::uint32_t i = 0; i < count; ++i) {
            const auto length = word();
            auto value = string(length);
            if (unique && std::find(values.begin(), values.end(), value) != values.end())
                throw std::runtime_error("Container source names contain a duplicate");
            values.push_back(std::move(value));
        }
        return values;
    }

    std::size_t cursor() const noexcept { return cursor_; }

private:
    void require(std::size_t size) const {
        if (cursor_ > bytes_.size || size > bytes_.size - cursor_)
            throw std::runtime_error("Container source table is truncated");
    }

    Bytes bytes_{};
    std::size_t cursor_{};
};

void require_schema(const std::vector<std::string>& fields) {
    static constexpr const char* expected[] = {
        "AirResist", "DamageReduction", "EarthResist", "Effect",
        "FireResist", "HitPoints", "InteractSound", "KeepSolid",
        "LightningResist", "Loot", "Script", "TrapChance", "TrapType",
        "Visual", "WaterResist"};
    if (fields.size() != sizeof(expected) / sizeof(expected[0]))
        throw std::runtime_error("DestructibleContainer source schema width differs");
    for (std::size_t i = 0; i < fields.size(); ++i)
        if (fields[i] != expected[i])
            throw std::runtime_error("DestructibleContainer source schema order differs");
}

Row read_row(Reader& reader) {
    Row row{};
    row.air_resist = reader.signed_word();
    row.damage_reduction = reader.signed_word();
    row.earth_resist = reader.signed_word();
    row.effect = reader.signed_word();
    row.fire_resist = reader.signed_word();
    row.hit_points = reader.signed_word();
    row.interact_sound = reader.signed_word();
    row.keep_solid = reader.byte();
    row.lightning_resist = reader.signed_word();
    row.loot = reader.signed_word();
    row.script = reader.string(reader.word());
    row.trap_chance = reader.signed_word();
    row.trap_type = reader.signed_word();
    row.visual = reader.signed_word();
    row.water_resist = reader.signed_word();
    if (row.keep_solid > 1)
        throw std::runtime_error("DestructibleContainer KeepSolid is not a source bool");
    return row;
}

} // namespace

bool Tables::load(Bytes game_objects, Bytes game_object_names,
                  Bytes game_object_fields, Bytes visual_dictionary,
                  Bytes visual_dictionary_names, std::string& error) {
    try {
        Reader data(game_objects), names(game_object_names), fields(game_object_fields);
        Reader dictionary_values(visual_dictionary), dictionary_names(visual_dictionary_names);
        auto next_names = names.strings(true);
        auto next_fields = fields.strings(true);
        auto next_visual_names = dictionary_names.strings(true);
        auto next_visual_paths = dictionary_values.strings(false);
        require_schema(next_fields);
        const auto row_count = data.word();
        if (row_count == 0 || row_count > kMaxRows || row_count != next_names.size())
            throw std::runtime_error("DestructibleContainer table names/count mismatch");
        if (next_visual_names.size() != next_visual_paths.size() ||
            dictionary_names.cursor() != visual_dictionary_names.size ||
            dictionary_values.cursor() != visual_dictionary.size)
            throw std::runtime_error("GameObjectDict names/values differ or have trailing bytes");
        std::vector<Row> next_rows;
        next_rows.reserve(row_count);
        for (std::uint32_t row = 0; row < row_count; ++row)
            next_rows.push_back(read_row(data));
        // Other GameObject tables follow this section in game_objects_pyarray.
        // Keep them opaque; the selected first section is still fully bounded.
        if (names.cursor() >= game_object_names.size ||
            fields.cursor() >= game_object_fields.size ||
            data.cursor() > game_objects.size)
            throw std::runtime_error("Container source table section boundary is invalid");
        data_names_ = std::move(next_names);
        rows_ = std::move(next_rows);
        visual_names_ = std::move(next_visual_names);
        visual_paths_ = std::move(next_visual_paths);
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}

const Row* Tables::find(std::string_view name, std::int32_t* data_id) const noexcept {
    const auto found = std::find(data_names_.begin(), data_names_.end(), name);
    if (found == data_names_.end()) return nullptr;
    const auto index = static_cast<std::size_t>(found - data_names_.begin());
    if (index >= rows_.size() || index > static_cast<std::size_t>(INT32_MAX)) return nullptr;
    if (data_id) *data_id = static_cast<std::int32_t>(index);
    return &rows_[index];
}

const std::string* Tables::visual_path(std::int32_t visual_id) const noexcept {
    if (visual_id < 0 || static_cast<std::size_t>(visual_id) >= visual_paths_.size())
        return nullptr;
    return &visual_paths_[static_cast<std::size_t>(visual_id)];
}

const std::string* Tables::data_desc(std::int32_t data_id) const noexcept {
    if (data_id < 0 || static_cast<std::size_t>(data_id) >= data_names_.size())
        return nullptr;
    return &data_names_[static_cast<std::size_t>(data_id)];
}

} // namespace dh2::destructible_container_table_v1
