#include "quest_table_v1.hpp"

#include <cstddef>

#include <fstream>
#include <iterator>

namespace dh::foundation::quest_runtime {
namespace {
bool read_file(const std::string& path, std::vector<std::uint8_t>& out, std::string& error) {
    std::ifstream file(path, std::ios::binary);
    if (!file) { error = "Quest table file is missing: " + path; return false; }
    out.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    if (out.empty()) { error = "Quest table file is empty: " + path; return false; }
    return true;
}
} // namespace

bool decode_quest_table_v1(const std::vector<std::uint8_t>& array,
    const std::vector<std::uint8_t>& names,
    std::shared_ptr<const QuestTableV1>& out, std::string& error) {
    out.reset();
    auto table = std::make_shared<QuestTableV1>();
    if (!table->decode({array.data(), array.size()}, {names.data(), names.size()}, error)) return false;
    if (!table->ready() || table->rows().empty()) {
        error = "Quest table decoded no rows";
        return false;
    }
    out = std::move(table);
    error.clear();
    return true;
}

bool load_quest_table_v1(const std::string& pyarray_path, const std::string& names_path,
    std::shared_ptr<const QuestTableV1>& out, std::string& error) {
    std::vector<std::uint8_t> array, names;
    if (!read_file(pyarray_path, array, error)) return false;
    if (!read_file(names_path, names, error)) return false;
    return decode_quest_table_v1(array, names, out, error);
}

std::size_t quest_rows_in_act_v1(const QuestTableV1& table, std::int32_t act) noexcept {
    std::size_t count = 0;
    for (const auto& row : table.rows())
        if (row.act == act) ++count;
    return count;
}


bool decode_character_row_names_v1(const std::vector<std::uint8_t>& bytes,
    std::map<std::string, std::int32_t>& rows, std::string& error) {
    rows.clear();
    std::size_t at = 0;
    const auto read_u32 = [&](std::uint32_t& out) {
        if (bytes.size() - at < 4) return false;
        out = std::uint32_t(bytes[at]) | (std::uint32_t(bytes[at + 1]) << 8) |
              (std::uint32_t(bytes[at + 2]) << 16) | (std::uint32_t(bytes[at + 3]) << 24);
        at += 4;
        return true;
    };
    std::uint32_t count = 0;
    if (!read_u32(count)) { error = "CharacterTable name table is truncated"; return false; }
    for (std::uint32_t row = 0; row < count; ++row) {
        std::uint32_t length = 0;
        if (!read_u32(length) || bytes.size() - at < length) {
            rows.clear();
            error = "CharacterTable name table is truncated at row " + std::to_string(row);
            return false;
        }
        rows.emplace(std::string(bytes.begin() + std::ptrdiff_t(at), bytes.begin() + std::ptrdiff_t(at + length)),
                     std::int32_t(row));
        at += length;
    }
    error.clear();
    return true;
}

} // namespace dh::foundation::quest_runtime
