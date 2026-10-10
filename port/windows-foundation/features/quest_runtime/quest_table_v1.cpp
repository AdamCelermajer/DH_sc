#include "quest_table_v1.hpp"

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

} // namespace dh::foundation::quest_runtime
