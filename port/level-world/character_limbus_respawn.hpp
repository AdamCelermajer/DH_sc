#pragma once

#include "character_respawn_outer.hpp"

#include <cstdint>

namespace dh2::character_limbus_respawn {

struct Services {
    void* context;
    // CSLimbus::OnFocus first stores 0 into Character+0x520, then dispatches
    // Character vtable +0x40 as GameObject::SetVisible(false).
    void (*clear_character_focus_word)(void*);
    void (*set_character_visible)(void*, std::uint32_t visible);
    // Read Character+0x530 only after the two prefix operations above.
    std::int32_t (*read_limbus_respawn_gate)(void*, std::uint8_t* raw_byte);

    // Character::GetRespawnDelay reads Character+0x1481 on each invocation.
    // OnFocus calls that method once for the positive-delay gate and, on the
    // eligible path, again for the StartTimer duration. This callback supplies
    // the current byte separately on each call.
    std::int32_t (*read_init_spawned_suppression)(void*,
                                                 std::uint8_t* raw_byte);
    const dh2::character_respawn::Services* respawn_services;

    // Source GetOnline byte+5, consulted only after the first delay is > 0.
    // This is not the separate Character+0x530 or Character+0x1481 byte.
    std::int32_t (*read_online_byte5)(void*, std::uint8_t* raw_byte);
    // Opaque call-through to PlayerManager::IsLocalPlayerHosting(). Its
    // internal Online/GameState/matching/local-player queries remain owned by
    // that source service; the caller must not cache the outer byte+5 result.
    std::int32_t (*is_local_player_hosting)(void*, std::int32_t* source_result);

    // Adapter calls Coordinator::start_timer(duration, repeat, event, ref).
    // The source ignores this return value.
    std::int32_t (*start_timer)(void*, std::uint32_t duration_ms,
                                std::int32_t repeat, std::int32_t event,
                                std::uintptr_t user_ref);
    // CSLimbus::OnFocus calls CharAI::AI_ClearAllAggro on every completed
    // branch, after the optional timer call.
    void (*clear_all_aggro)(void*);
};

struct Result {
    std::uint32_t focus_word_cleared;
    std::uint32_t visibility_set_false;
    std::uint32_t limbus_gate_read;
    std::uint8_t limbus_gate_byte;
    std::uint8_t reserved0[3];
    std::uint32_t delay_getter_calls;
    std::uint32_t init_spawned_byte_reads;
    std::uint32_t property_reads;
    std::int32_t first_delay_ms;
    std::uint32_t online_byte_read;
    std::uint8_t online_byte5;
    std::uint8_t reserved1[3];
    std::uint32_t hosting_query;
    std::int32_t hosting_source_result;
    std::uint32_t timer_requested;
    std::uint32_t timer_duration_bits;
    std::int32_t second_delay_ms;
    std::uint32_t all_aggro_cleared;
};

enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    limbus_gate_read_failed = 2,
    init_spawned_read_failed = 3,
    respawn_delay_failed = 4,
    online_byte_read_failed = 5,
    hosting_query_failed = 6,
    timer_service_missing = 7,
};

// Reconstructs the source-owned Limbus OnFocus prefix and respawn-timer
// decision. It does not own character storage, PlayerManager, or timer slots.
Status on_focus(const Services* services, Result* result);

}  // namespace dh2::character_limbus_respawn
