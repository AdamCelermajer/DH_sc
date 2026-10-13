#include "game_object_trap_tables_v1.hpp"

#include <algorithm>
#include <cstring>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace dh2::game_object_trap_tables_v1 {
namespace {

constexpr std::size_t kMaxBytes = 8u * 1024u * 1024u;
constexpr std::uint32_t kMaxRows = 4096;
constexpr std::uint32_t kMaxString = 4096;

class Reader {
public:
    explicit Reader(Bytes bytes) : bytes_(bytes) {
        if (!bytes.data || bytes.size < 4 || bytes.size > kMaxBytes)
            throw std::runtime_error("GameObject trap table byte span is invalid");
    }
    std::uint32_t word() {
        require(4);
        const auto* p = bytes_.data + cursor_;
        cursor_ += 4;
        return std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8) |
            (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
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
            throw std::runtime_error("GameObject trap table string exceeds limit");
        require(length);
        std::string value(reinterpret_cast<const char*>(bytes_.data + cursor_), length);
        cursor_ += length;
        if (value.find('\0') != std::string::npos)
            throw std::runtime_error("GameObject trap table string contains an embedded NUL");
        return value;
    }
    std::vector<std::string> strings() {
        const auto count = word();
        if (!count || count > kMaxRows)
            throw std::runtime_error("GameObject trap table name count exceeds limit");
        std::vector<std::string> values;
        values.reserve(count);
        for (std::uint32_t i = 0; i < count; ++i) values.push_back(string());
        return values;
    }
    std::size_t cursor() const noexcept { return cursor_; }
    std::size_t size() const noexcept { return bytes_.size; }

private:
    void require(std::size_t n) const {
        if (cursor_ > bytes_.size || n > bytes_.size - cursor_)
            throw std::runtime_error("GameObject trap table is truncated");
    }
    Bytes bytes_{};
    std::size_t cursor_{};
};

std::vector<std::vector<std::string>> read_groups(Reader& reader) {
    std::vector<std::vector<std::string>> groups;
    while (reader.cursor() < reader.size()) {
        groups.push_back(reader.strings());
        if (groups.size() > 16)
            throw std::runtime_error("GameObject trap table group count exceeds limit");
    }
    return groups;
}

// One decoded row: the integer fields by schema name and the Script string.
struct DecodedRow {
    std::vector<std::pair<std::string, std::int32_t>> numbers;
    std::string script;
    std::int32_t number(const char* field, std::int32_t fallback) const {
        for (const auto& item : numbers)
            if (item.first == field) return item.second;
        return fallback;
    }
};

DecodedRow decode_row(Reader& data, const std::vector<std::string>& fields) {
    DecodedRow row;
    for (const auto& field : fields) {
        if (field == "Script") {
            row.script = data.string();
        } else if (field == "KeepSolid" || field == "IsActive") {
            row.numbers.emplace_back(field, data.byte());
        } else {
            row.numbers.emplace_back(field, data.signed_word());
        }
    }
    return row;
}

void require_fields(const std::vector<std::string>& actual,
                    std::initializer_list<const char*> expected,
                    const char* label) {
    if (actual.size() != expected.size() ||
        !std::equal(actual.begin(), actual.end(), expected.begin()))
        throw std::runtime_error(std::string(label) + " source field schema differs");
}

} // namespace

bool Tables::load(Bytes game_objects, Bytes game_object_names,
                  Bytes game_object_fields, Bytes visual_dictionary,
                  Bytes visual_dictionary_names, std::string& error) {
    try {
        Reader data(game_objects), names(game_object_names), fields(game_object_fields);
        Reader dictionary_values(visual_dictionary), dictionary_names(visual_dictionary_names);
        auto array_names = read_groups(names);
        auto struct_fields = read_groups(fields);
        // Arrays 0..10 use these Structs (Structs 4 is the unused AnimatedDecor
        // row); array 3 is GameObjectDamager and array 7 is TimerTraps.
        static constexpr std::uint32_t kStructForArray[] = {
            0, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11};
        constexpr std::size_t kDamagerArray = 3, kTimerTrapArray = 7;
        if (array_names.size() != sizeof(kStructForArray) / sizeof(kStructForArray[0]) ||
            struct_fields.size() != 12 || names.cursor() != game_object_names.size ||
            fields.cursor() != game_object_fields.size)
            throw std::runtime_error("GameObject trap source array/schema section count differs");
        require_fields(struct_fields[kStructForArray[kDamagerArray]],
                       {"Damage", "ImmunityDelay", "Max", "Min", "Type"},
                       "GameObjectDamager");
        require_fields(struct_fields[kStructForArray[kTimerTrapArray]],
                       {"Damage", "Delay", "HurtChance", "Script", "Sound", "Visual"},
                       "TimerTraps");
        std::vector<DamagerRow> next_damagers;
        std::vector<TimerTrapRow> next_traps;
        for (std::size_t section = 0; section < array_names.size(); ++section) {
            const auto& schema = struct_fields[kStructForArray[section]];
            const auto count = data.word();
            if (count != array_names[section].size() || count > kMaxRows)
                throw std::runtime_error("GameObject trap source array names/count mismatch");
            for (std::uint32_t index = 0; index < count; ++index) {
                const auto row = decode_row(data, schema);
                if (section == kDamagerArray) {
                    DamagerRow out;
                    out.name = array_names[section][index];
                    out.damage = row.number("Damage", -1);
                    out.immunity_delay = row.number("ImmunityDelay", 0);
                    out.max = row.number("Max", 0);
                    out.min = row.number("Min", 0);
                    out.type = row.number("Type", -1);
                    next_damagers.push_back(std::move(out));
                } else if (section == kTimerTrapArray) {
                    TimerTrapRow out;
                    out.name = array_names[section][index];
                    out.damage = row.number("Damage", -1);
                    out.delay = row.number("Delay", 0);
                    out.hurt_chance = row.number("HurtChance", 0);
                    out.script = row.script;
                    out.sound = row.number("Sound", -1);
                    out.visual = row.number("Visual", -1);
                    next_traps.push_back(std::move(out));
                }
            }
        }
        if (data.cursor() != game_objects.size)
            throw std::runtime_error("GameObject trap source table boundary differs");
        auto next_visual_names = dictionary_names.strings();
        auto next_visual_paths = dictionary_values.strings();
        if (next_visual_names.size() != next_visual_paths.size() ||
            dictionary_names.cursor() != visual_dictionary_names.size ||
            dictionary_values.cursor() != visual_dictionary.size)
            throw std::runtime_error("GameObjectDict names/values differ or have trailing bytes");
        damagers_ = std::move(next_damagers);
        timer_traps_ = std::move(next_traps);
        visual_paths_ = std::move(next_visual_paths);
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    } catch (...) {
        error = "GameObject trap source table allocation failed";
        return false;
    }
}

const TimerTrapRow* Tables::timer_trap(std::int32_t id) const noexcept {
    return id < 0 || static_cast<std::size_t>(id) >= timer_traps_.size()
        ? nullptr : &timer_traps_[static_cast<std::size_t>(id)];
}

const TimerTrapRow* Tables::find_timer_trap(std::string_view name,
                                            std::int32_t* id) const noexcept {
    const auto found = std::find_if(timer_traps_.begin(), timer_traps_.end(),
        [&](const TimerTrapRow& row) { return row.name == name; });
    if (found == timer_traps_.end()) return nullptr;
    if (id) *id = static_cast<std::int32_t>(found - timer_traps_.begin());
    return &*found;
}

const DamagerRow* Tables::damager(std::int32_t id) const noexcept {
    return id < 0 || static_cast<std::size_t>(id) >= damagers_.size()
        ? nullptr : &damagers_[static_cast<std::size_t>(id)];
}

const DamagerRow* Tables::find_damager(std::string_view name,
                                       std::int32_t* id) const noexcept {
    const auto found = std::find_if(damagers_.begin(), damagers_.end(),
        [&](const DamagerRow& row) { return row.name == name; });
    if (found == damagers_.end()) return nullptr;
    if (id) *id = static_cast<std::int32_t>(found - damagers_.begin());
    return &*found;
}

const std::string* Tables::visual_path(std::int32_t id) const noexcept {
    return id < 0 || static_cast<std::size_t>(id) >= visual_paths_.size()
        ? nullptr : &visual_paths_[static_cast<std::size_t>(id)];
}

} // namespace dh2::game_object_trap_tables_v1
