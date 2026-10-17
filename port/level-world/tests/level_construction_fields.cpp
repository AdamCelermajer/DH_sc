#include "../level_construction_fields.hpp"
#include "../../game-data/data.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace {

using dh2::data::LevelDeclaration;
using dh2::data::LevelTables;
using dh2::level_construction_fields::Result;
using dh2::level_construction_fields::State;
using dh2::level_construction_fields::Status;

int failures = 0;
int cases = 0;

void check(bool condition, const char* name) {
    ++cases;
    if (!condition) {
        ++failures;
        std::cerr << "FAIL " << name << '\n';
    }
}

LevelDeclaration row(std::string level_file, std::int32_t hub, bool is_random) {
    LevelDeclaration value{};
    value.level_file = std::move(level_file);
    value.hub = hub;
    value.is_random = is_random;
    return value;
}

State poisoned_state() {
    State value{};
    value.level_list_index_3c = 0x12345678;
    value.hub_40 = 0x23456789;
    value.is_random_e8 = 0xa5;
    value.reserved[0] = 0xb1;
    value.reserved[1] = 0xb2;
    value.reserved[2] = 0xb3;
    value.difficulty_118 = 0x3456789a;
    return value;
}

bool equal(const State& a, const State& b) {
    return a.level_list_index_3c == b.level_list_index_3c &&
           a.hub_40 == b.hub_40 &&
           a.is_random_e8 == b.is_random_e8 &&
           a.reserved[0] == b.reserved[0] &&
           a.reserved[1] == b.reserved[1] &&
           a.reserved[2] == b.reserved[2] &&
           a.difficulty_118 == b.difficulty_118;
}

void run_source_fixtures() {
    using dh2::level_construction_fields::initialize;
    {
        LevelTables table;
        table.levels = {row("CRYPTA.MLX", 9, true), row("crypta.mlx", 12, false)};
        State state{};
        Result result{};
        const auto status = initialize(&table, "maps/crypta.mlx.backup", -7, &state, &result);
        check(status == Status::selected && result.rows_examined == 1 && result.selected_index == 0,
              "substring-and-first-match");
        check(state.level_list_index_3c == 0 && state.hub_40 == 9 && state.is_random_e8 == 1 &&
              state.difficulty_118 == -7, "first-row-fields-and-raw-difficulty");
    }
    {
        LevelTables table;
        table.levels = {row("CryptA.mlx", 5, true)};
        State state{};
        Result result{};
        const auto status = initialize(&table, "maps/CRYPTA.mlx", 11, &state, &result);
        check(status == Status::no_match && result.rows_examined == 1 && result.selected_index == -1,
              "haystack-case-is-preserved");
        check(state.level_list_index_3c == -1 && state.hub_40 == -1 && state.is_random_e8 == 0 &&
              state.difficulty_118 == 11, "no-match-constructor-sentinels");
    }
    {
        LevelTables table;
        table.levels = {row("003_darkwood.mlx", 4, false), row("003_darkwood.mlx", 5, true),
                        row("else.mlx", 7, false)};
        State state{};
        Result result{};
        const auto status = initialize(&table, "data/003_darkwood.mlx", 3, &state, &result);
        check(status == Status::selected && result.selected_index == 0 && result.rows_examined == 1 &&
              state.hub_40 == 4 && state.is_random_e8 == 0, "duplicate-file-selects-first-ordinal");
    }
    {
        LevelTables table;
        table.levels = {row("other.mlx", 4, true), row("also.mlx", 5, false)};
        State state{};
        Result result{};
        const auto status = initialize(&table, "crypt01.mlx", 0x76543210, &state, &result);
        check(status == Status::no_match && result.rows_examined == 2 && result.selected_index == -1,
              "no-match-scans-full-list");
        check(state.level_list_index_3c == -1 && state.hub_40 == -1 && state.is_random_e8 == 0 &&
              state.difficulty_118 == 0x76543210, "no-match-preserves-raw-difficulty");
    }
    {
        LevelTables table;
        State state{};
        Result result{};
        const auto status = initialize(&table, "anything", 6, &state, &result);
        check(status == Status::no_match && result.rows_examined == 0 &&
              state.level_list_index_3c == -1 && state.hub_40 == -1 && state.is_random_e8 == 0,
              "empty-list-sentinels");
    }
    {
        LevelTables table;
        table.levels = {row("", 8, true)};
        State state{};
        Result result{};
        const auto status = initialize(&table, "", -3, &state, &result);
        check(status == Status::selected && result.selected_index == 0 && state.hub_40 == 8,
              "empty-needle-matches-as-strstr");
    }
    {
        LevelTables table;
        table.levels = {row(std::string(1024, 'a'), 8, true)};
        State state = poisoned_state(), before = state;
        Result result{};
        const auto status = initialize(&table, "aaa", 0, &state, &result);
        check(status == Status::unsafe_level_file && equal(state, before),
              "port-rejects-source-stack-overflow-row-before-write");
    }
    {
        LevelTables table;
        table.levels = {row("valid.mlx", 8, true)};
        std::string embedded("valid\0suffix", 12);
        State state = poisoned_state(), before = state;
        Result result{};
        const auto status = initialize(&table, embedded, 0, &state, &result);
        check(status == Status::invalid_argument && equal(state, before),
              "port-rejects-embedded-null-before-write");
    }
}

std::vector<std::uint8_t> read_file(const char* path) {
    std::ifstream stream(path, std::ios::binary);
    return std::vector<std::uint8_t>(std::istreambuf_iterator<char>(stream), {});
}

bool test_real_cache(int argc, char** argv, std::int32_t& crypt_index) {
    if (argc != 4) return false;
    auto records = read_file(argv[1]);
    auto names = read_file(argv[2]);
    auto schema = read_file(argv[3]);
    if (records.empty() || names.empty() || schema.empty()) return false;
    dh2::data::Bytes record_bytes{records.data(), records.size()};
    dh2::data::Bytes name_bytes{names.data(), names.size()};
    dh2::data::Bytes schema_bytes{schema.data(), schema.size()};
    LevelTables table;
    std::string error;
    if (!dh2::data::load_levels(record_bytes, name_bytes, schema_bytes, table, error)) return false;

    const auto row_index = dh2::data::find_level(table, "GOTHICUS_CRYPT_01");
    if (row_index != 23 || table.levels.size() <= static_cast<std::size_t>(row_index)) return false;
    const auto& crypt = table.levels[static_cast<std::size_t>(row_index)];
    if (crypt.level_file != "007_crypt_01.rule.xml") return false;

    State state{};
    Result result{};
    const auto status = dh2::level_construction_fields::initialize(
        &table, crypt.level_file, 2, &state, &result);
    if (status != Status::selected || result.selected_index != row_index ||
        result.rows_examined != 24 || state.level_list_index_3c != row_index ||
        state.hub_40 != crypt.hub || state.is_random_e8 != (crypt.is_random ? 1 : 0) ||
        state.difficulty_118 != 2) return false;
    crypt_index = result.selected_index;
    return true;
}

}  // namespace

int main(int argc, char** argv) {
    run_source_fixtures();
    std::int32_t crypt_index = -1;
    const bool cache_ok = test_real_cache(argc, argv, crypt_index);
    check(cache_ok, "actual-cache-crypt-level-23");
    std::cout << "{\"validation\":\"" << (failures == 0 ? "PASS" : "FAIL")
              << "\",\"level_construction_cases\":" << cases
              << ",\"mismatches\":" << failures
              << ",\"cache_crypt_index\":" << crypt_index << "}\n";
    return failures == 0 ? 0 : 1;
}
