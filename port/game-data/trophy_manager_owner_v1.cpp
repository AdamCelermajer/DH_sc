#include "trophy_manager_owner_v1.hpp"

#include <algorithm>
#include <cstring>
#include <set>
#include <stdexcept>
#include <utility>

namespace dh2::data {
namespace {
constexpr std::size_t kMaximumInputBytes = 1u * 1024u * 1024u;
constexpr std::uint32_t kMaximumRows = TrophyManagerOwnerV1::source_save_bit_count;
constexpr std::uint32_t kMaximumStringBytes = 4096;
constexpr const char* kSourceFields[] = {
    "Desc", "GLIndex", "GLLive", "Grade", "Label", "Name", "Type"
};

class Reader {
    Bytes bytes_{};
    std::size_t offset_{};
public:
    explicit Reader(Bytes bytes) : bytes_(bytes) {
        if ((!bytes.data && bytes.size) || bytes.size > kMaximumInputBytes)
            throw std::runtime_error("TrophyTable byte span outside limits");
    }
    std::uint32_t word() {
        if (!bytes_.data || offset_ > bytes_.size || bytes_.size - offset_ < 4)
            throw std::runtime_error("Truncated TrophyTable word");
        const auto* p = bytes_.data + offset_;
        offset_ += 4;
        return std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8) |
               (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
    }
    std::int32_t integer() {
        const auto bits = word();
        std::int32_t value{};
        static_assert(sizeof(value) == sizeof(bits), "source integer width differs");
        std::memcpy(&value, &bits, sizeof(value));
        return value;
    }
    std::vector<std::string> strings(bool allow_empty) {
        const auto count = word();
        if (count > kMaximumRows || (!allow_empty && !count))
            throw std::runtime_error("TrophyTable string count outside limits");
        std::vector<std::string> result;
        result.reserve(count);
        std::set<std::string> unique;
        for (std::uint32_t n = 0; n < count; ++n) {
            const auto size = word();
            if (!size || size > kMaximumStringBytes || offset_ > bytes_.size ||
                size > bytes_.size - offset_)
                throw std::runtime_error("TrophyTable string length outside limits");
            std::string value(reinterpret_cast<const char*>(bytes_.data + offset_), size);
            offset_ += size;
            if (std::any_of(value.begin(), value.end(), [](unsigned char ch) {
                    return ch < 0x21 || ch > 0x7e;
                }) || !unique.insert(value).second) {
                throw std::runtime_error("TrophyTable names must be unique ASCII identifiers");
            }
            result.push_back(std::move(value));
        }
        return result;
    }
    bool exhausted() const noexcept { return offset_ == bytes_.size; }
};

bool same_fields(const std::vector<std::string>& fields) {
    if (fields.size() != sizeof(kSourceFields) / sizeof(kSourceFields[0])) return false;
    for (std::size_t i = 0; i < fields.size(); ++i)
        if (fields[i] != kSourceFields[i]) return false;
    return true;
}

TrophyDataV1 project(std::int32_t id, const TrophyTableRowV1& row) {
    // InitTrophies copies Trophy fields into TrophyData in this order:
    // Name, Desc, unlocked=0, Type, Grade, Label, GLLive, GLIndex.
    return {id, row.name, row.desc, false, row.type, row.grade, row.label,
            row.gl_live, row.gl_index};
}
} // namespace

bool TrophyTableV1::load(Bytes records, Bytes names, Bytes fields,
                         std::string& error) {
    error.clear();
    try {
        Reader record_reader(records), name_reader(names), field_reader(fields);
        const auto count = record_reader.word();
        if (count > kMaximumRows)
            throw std::runtime_error("TrophyTable exceeds source 128-bit save capacity");
        auto row_names = name_reader.strings(true);
        auto row_fields = field_reader.strings(false);
        if (row_names.size() != count || !same_fields(row_fields))
            throw std::runtime_error("TrophyTable record/name/schema dimensions differ");

        std::vector<TrophyTableRowV1> candidate;
        candidate.reserve(count);
        for (std::uint32_t i = 0; i < count; ++i) {
            TrophyTableRowV1 row{};
            row.table_name = std::move(row_names[i]);
            row.desc = record_reader.integer();
            row.gl_index = record_reader.integer();
            row.gl_live = record_reader.integer();
            row.grade = record_reader.integer();
            row.label = record_reader.integer();
            row.name = record_reader.integer();
            row.type = record_reader.integer();
            candidate.push_back(std::move(row));
        }
        if (!record_reader.exhausted() || !name_reader.exhausted() ||
            !field_reader.exhausted())
            throw std::runtime_error("Unexpected TrophyTable cache suffix");
        rows_.swap(candidate);
        return true;
    } catch (const std::exception& e) {
        error = e.what();
        return false;
    } catch (...) {
        error = "TrophyTable decode failed";
        return false;
    }
}

bool TrophyManagerOwnerV1::initialize(const TrophyTableV1& table,
                                     std::string& error) {
    error.clear();
    if (!unlocking_.empty()) {
        error = "cannot replace TrophyManager rows while an unlock callback is active";
        return false;
    }
    const auto& source = table.rows();
    if (source.size() > source_save_bit_count) {
        error = "TrophyTable exceeds source 128-bit save capacity";
        return false;
    }
    try {
        std::vector<TrophyDataV1> candidate;
        std::vector<std::string> candidate_names;
        candidate.reserve(source.size());
        candidate_names.reserve(source.size());
        for (std::size_t i = 0; i < source.size(); ++i) {
            candidate.push_back(project(static_cast<std::int32_t>(i), source[i]));
            candidate_names.push_back(source[i].table_name);
        }
        rows_.swap(candidate);
        table_names_.swap(candidate_names);
        initialized_ = true;
        return true;
    } catch (...) {
        error = "TrophyManager row projection failed";
        return false;
    }
}

const TrophyDataV1* TrophyManagerOwnerV1::get(std::int32_t id) const noexcept {
    if (!initialized_ || id < 0 || static_cast<std::size_t>(id) >= rows_.size())
        return nullptr;
    const auto& row = rows_[static_cast<std::size_t>(id)];
    return row.id == id ? &row : nullptr;
}

std::int32_t TrophyManagerOwnerV1::find_id_by_name(
    const std::string& table_name) const noexcept {
    if (!initialized_) return -1;
    for (std::size_t i = 0; i < table_names_.size(); ++i)
        if (table_names_[i] == table_name) return static_cast<std::int32_t>(i);
    return -1;
}

bool TrophyManagerOwnerV1::is_unlocked(std::int32_t id) const noexcept {
    const auto* row = get(id);
    return row && row->unlocked;
}

bool TrophyManagerOwnerV1::is_unlocking(std::int32_t id) const noexcept {
    return id >= 0 && std::find(unlocking_.begin(), unlocking_.end(), id) != unlocking_.end();
}

TrophyUnlockStatusV1 TrophyManagerOwnerV1::begin_unlock(std::int32_t id) noexcept {
    if (!get(id)) return TrophyUnlockStatusV1::invalid_id;
    if (is_unlocked(id)) return TrophyUnlockStatusV1::already_unlocked;
    if (is_unlocking(id)) return TrophyUnlockStatusV1::already_unlocking;
    try {
        unlocking_.push_back(id);
        return TrophyUnlockStatusV1::started;
    } catch (...) {
        return TrophyUnlockStatusV1::failed;
    }
}

TrophyUnlockStatusV1 TrophyManagerOwnerV1::complete_unlock(
    std::int32_t id, TrophyUnlockEventV1* event) noexcept {
    if (!get(id)) return TrophyUnlockStatusV1::invalid_id;
    const auto pending = std::find(unlocking_.begin(), unlocking_.end(), id);
    if (pending == unlocking_.end())
        return is_unlocked(id) ? TrophyUnlockStatusV1::already_unlocked
                               : TrophyUnlockStatusV1::not_unlocking;

    // TrophyUnlockedCB first removes this ID from the in-progress vector,
    // then marks the corresponding table-derived TrophyData as unlocked.
    unlocking_.erase(pending);
    auto& row = rows_[static_cast<std::size_t>(id)];
    row.unlocked = true;
    if (event) {
        *event = {row.id, row.name_text_id, row.desc_text_id, row.type,
                  row.grade, row.label, row.gl_live, row.gl_index};
    }
    return TrophyUnlockStatusV1::completed;
}

TrophyUnlockStatusV1 TrophyManagerOwnerV1::unlock(
    std::int32_t id, TrophyUnlockEventV1* event) noexcept {
    const auto started = begin_unlock(id);
    if (started != TrophyUnlockStatusV1::started) return started;
    return complete_unlock(id, event);
}

bool TrophyManagerOwnerV1::encode_savegame_bits(
    std::array<std::uint8_t, 16>& payload, std::string& error) const {
    error.clear();
    if (!initialized_) {
        error = "TrophyManager is not initialized";
        return false;
    }
    if (rows_.size() > source_save_bit_count) {
        error = "TrophyTable exceeds source 128-bit save capacity";
        return false;
    }

    payload.fill(0);
    for (std::size_t id = 0; id < rows_.size(); ++id) {
        if (!rows_[id].unlocked) continue;
        const auto byte_index = id / 8;
        const auto bit_index = static_cast<unsigned>(id % 8);
        payload[byte_index] = static_cast<std::uint8_t>(
            payload[byte_index] | static_cast<std::uint8_t>(1u << bit_index));
    }
    return true;
}

bool TrophyManagerOwnerV1::load_savegame_bits(Bytes payload,
                                               std::string& error) {
    error.clear();
    if (!initialized_) {
        error = "TrophyManager is not initialized";
        return false;
    }
    if (rows_.size() > source_save_bit_count) {
        error = "TrophyTable exceeds source 128-bit save capacity";
        return false;
    }
    if (!payload.data || payload.size != 16) {
        error = "achievements.savegame payload must be exactly 16 bytes";
        return false;
    }

    // Validate before changing the owner. LoadTrophies only visits rows in
    // the active table; unused high bits are retained by the file but ignored.
    for (std::size_t id = 0; id < rows_.size(); ++id) {
        const auto byte_index = id / 8;
        const auto bit_index = static_cast<unsigned>(id % 8);
        if ((payload.data[byte_index] & static_cast<std::uint8_t>(1u << bit_index)) == 0)
            continue;
        const auto result = unlock(static_cast<std::int32_t>(id));
        if (result != TrophyUnlockStatusV1::completed &&
            result != TrophyUnlockStatusV1::already_unlocked) {
            error = "TrophyManager failed to restore a saved unlocked row";
            return false;
        }
    }
    return true;
}

} // namespace dh2::data
