#include "trigger_interaction_table_v1.hpp"

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <utility>

namespace dh2::trigger_interaction_table_v1 {
namespace {

constexpr std::size_t kMaxBytes = 8u * 1024u * 1024u;
constexpr std::uint32_t kMaxRows = 4096;
constexpr std::uint32_t kMaxString = 4096;

class Reader {
public:
    explicit Reader(Bytes bytes) : bytes_(bytes) {
        if (!bytes.data || bytes.size < 4 || bytes.size > kMaxBytes)
            throw std::runtime_error("TriggerObject source table byte span is invalid");
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

    std::string string() {
        const auto length = word();
        if (length > kMaxString)
            throw std::runtime_error("TriggerObject source string exceeds limit");
        require(length);
        std::string value(reinterpret_cast<const char*>(bytes_.data + cursor_), length);
        cursor_ += length;
        if (value.find('\0') != std::string::npos)
            throw std::runtime_error("TriggerObject source string contains an embedded NUL");
        return value;
    }

    std::vector<std::string> strings() {
        const auto count = word();
        if (!count || count > kMaxRows)
            throw std::runtime_error("TriggerObject source name count exceeds limit");
        std::vector<std::string> values;
        values.reserve(count);
        for (std::uint32_t i = 0; i < count; ++i)
            values.push_back(string());
        return values;
    }

    std::size_t cursor() const noexcept { return cursor_; }
    std::size_t size() const noexcept { return bytes_.size; }

private:
    void require(std::size_t size) const {
        if (cursor_ > bytes_.size || size > bytes_.size - cursor_)
            throw std::runtime_error("TriggerObject source table is truncated");
    }

    Bytes bytes_{};
    std::size_t cursor_{};
};

std::vector<std::vector<std::string>> read_groups(Reader& reader) {
    std::vector<std::vector<std::string>> groups;
    while (reader.cursor() < reader.size()) {
        groups.push_back(reader.strings());
        if (groups.size() > 16)
            throw std::runtime_error("TriggerObject source schema group count exceeds limit");
    }
    return groups;
}

void skip_row(Reader& data, const std::vector<std::string>& fields) {
    for (const auto& field : fields) {
        if (field == "Script") {
            (void)data.string();
        } else if (field == "KeepSolid" || field == "IsActive") {
            (void)data.byte();
        } else {
            (void)data.word();
        }
    }
}

void require_fields(const std::vector<std::string>& actual,
                    const std::vector<std::string>& expected) {
    if (actual != expected)
        throw std::runtime_error("TriggerObject source field schema differs");
}

} // namespace

bool Tables::load(Bytes game_objects, Bytes game_object_names,
                  Bytes game_object_fields, std::string& error) {
    try {
        Reader data(game_objects), names(game_object_names), fields(game_object_fields);
        auto array_names = read_groups(names);
        auto struct_fields = read_groups(fields);
        static constexpr std::uint32_t kStructForArray[] = {
            0, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11};
        if (array_names.size() != sizeof(kStructForArray) / sizeof(kStructForArray[0]) ||
            struct_fields.size() != 12 || names.cursor() != game_object_names.size ||
            fields.cursor() != game_object_fields.size)
            throw std::runtime_error("TriggerObject source array/schema section count differs");

        static const std::vector<std::string> trigger_fields = {
            "InteractionType", "Script", "Sound", "Visual"};
        std::vector<Row> next_rows;
        for (std::size_t section = 0; section < array_names.size(); ++section) {
            const auto struct_id = kStructForArray[section];
            const auto& schema = struct_fields[struct_id];
            if (section == 8)
                require_fields(schema, trigger_fields);
            const auto count = data.word();
            if (count != array_names[section].size() || count > kMaxRows)
                throw std::runtime_error("TriggerObject source array names/count mismatch");
            for (std::uint32_t row = 0; row < count; ++row) {
                if (section == 8) {
                    const auto interaction_type = data.signed_word();
                    (void)data.string();
                    (void)data.signed_word();
                    (void)data.signed_word();
                    next_rows.push_back({array_names[section][row], interaction_type});
                } else {
                    skip_row(data, schema);
                }
            }
        }
        if (data.cursor() != game_objects.size || next_rows.size() != 6)
            throw std::runtime_error("TriggerObject source table boundary/row count differs");
        for (std::size_t i = 0; i < next_rows.size(); ++i)
            if (next_rows[i].name.empty() ||
                std::find_if(next_rows.begin(), next_rows.begin() + i,
                    [&](const Row& other) { return other.name == next_rows[i].name; }) !=
                    next_rows.begin() + i)
                throw std::runtime_error("TriggerObject source names are empty or duplicated");

        rows_ = std::move(next_rows);
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}

const Row* Tables::find(std::string_view name, std::int32_t* data_id) const noexcept {
    const auto found = std::find_if(rows_.begin(), rows_.end(),
        [&](const Row& row) { return row.name == name; });
    if (found == rows_.end()) return nullptr;
    const auto index = static_cast<std::size_t>(found - rows_.begin());
    if (index > static_cast<std::size_t>(INT32_MAX)) return nullptr;
    if (data_id) *data_id = static_cast<std::int32_t>(index);
    return &*found;
}

} // namespace dh2::trigger_interaction_table_v1
