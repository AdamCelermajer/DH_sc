#pragma once

#include "character_target_search.hpp"

#include <cstdint>
#include <cstring>

namespace dh2::player_skill_target_top_v1 {

// GameObject::_GetTargetListTop (0x38eb68) ReturnValues projection.
//
// Non-empty list: pushUserData(top.object), pushNumber(distance),
// pushNumber(angle * 57.296f), pushBoolean(flags & 1) and pushNumber of the fifth
// TargetInfo word (always zero from TargetList::Search). Empty list:
// pushPointer(nullptr) and nothing else. `Value` is the Player VM's source
// value record (`type` 2 Pointer, 3 number, 1 boolean). The object is carried as
// the Pointer identity; the Lua layer wraps it into a method-bearing UserData
// table, exactly like GetSelf (player_skill_session_v1.cpp).
//
// The angle scale is the single-precision constant loaded at 0x38eb98
// (movw #0x2ee0, movt #0x4265) and multiplied with __aeabi_fmul.
inline float degrees_per_radian() noexcept {
    const std::uint32_t bits = 0x42652ee0u;
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

// Returns 0 on success, 1 when the result buffer is too small. `returned` is
// set to the pushed value count.
template <class Value>
int write(const dh2::target_search::List48& list, Value* out,
          std::uint32_t capacity, std::uint32_t* returned) noexcept {
    *returned = 0;
    if (!list.heap || !list.count) {
        if (!out || capacity < 1) return 1;
        out[0] = {};
        out[0].type = 2;
        out[0].identity = 0;
        *returned = 1;
        return 0;
    }
    if (!out || capacity < 5) return 1;
    const auto& top = list.heap[0];
    out[0] = {};
    out[0].type = 2;
    out[0].identity = top.identity;
    out[1] = {};
    out[1].type = 3;
    out[1].number = top.distance;
    out[2] = {};
    out[2].type = 3;
    out[2].number = top.angle * degrees_per_radian();
    out[3] = {};
    out[3].type = 1;
    out[3].boolean = top.flags & 1u;
    out[4] = {};
    out[4].type = 3;
    float extra;
    static_assert(sizeof(extra) == sizeof(top.reserved));
    std::memcpy(&extra, &top.reserved, sizeof(extra));
    out[4].number = extra;
    *returned = 5;
    return 0;
}

} // namespace dh2::player_skill_target_top_v1
