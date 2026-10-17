#pragma once

#include "../game-data/level_tables.hpp"

#include <cstdint>
#include <string_view>

namespace dh2::level_construction_fields {

enum class Status : std::uint32_t {
    selected = 0,
    no_match = 1,
    invalid_argument = 2,
    unsafe_level_file = 3,
};

// Named projection of the fields produced by the Level constructor scan.
// These are not offsets into a native ARM object.
struct State {
    std::int32_t level_list_index_3c;
    std::int32_t hub_40;
    std::uint8_t is_random_e8;
    std::uint8_t reserved[3];
    std::int32_t difficulty_118;
};

struct Result {
    Status status;
    std::uint32_t rows_examined;
    std::int32_t selected_index;
};

// Reconstruct only the Level constructor's LevelList identity/metadata scan.
// Each row's LevelFile is copied to the source's 1024-byte local buffer and
// ASCII-lowercased. The incoming filename remains case-sensitive. The first
// substring match supplies row index, IsRandom, and Hub. The raw constructor
// difficulty is preserved unchanged.
//
// Port safety guards (invalid pointers, embedded NULs, malformed/overlong row
// strings) run before any output write. The caller must keep the filename and
// owned LevelTables immutable for the duration of the call.
Status initialize(const data::LevelTables* tables,
                  std::string_view incoming_level_file,
                  std::int32_t constructor_difficulty_118,
                  State* output,
                  Result* result) noexcept;

static_assert(sizeof(State) == 16, "named Level field projection layout");
static_assert(sizeof(Result) == 12, "constructor scan result layout");

}  // namespace dh2::level_construction_fields
