#pragma once

#include <cstdint>

namespace dh2::native::ghost_script_queries {

// Read-only projections of the one Character Coordinator and its existing
// native path. The caller owns both fields and keeps them live for each query.
struct Binding {
    std::uintptr_t owner;
    const std::int32_t* character_state;
    const std::uint32_t* path_count;
};

enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    stale_owner = 2,
};

Status get_state(const Binding*, std::uintptr_t requested_owner,
                 std::int32_t* output) noexcept;
Status has_path(const Binding*, std::uintptr_t requested_owner,
                std::uint32_t* output) noexcept;

}  // namespace dh2::native::ghost_script_queries
