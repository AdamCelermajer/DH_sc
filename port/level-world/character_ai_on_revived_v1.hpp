#pragma once

#include "character_ai_initialization.hpp"
#include "character_coordinator.hpp"
#include <cstdint>
#include <string>

namespace dh2::character_ai_on_revived_v1 {

enum class Operation : std::uint32_t {
    stop_all_timers,
    get_ai_tick,
    start_ai_tick,
    get_dot_tick,
    start_dot_tick,
    active_ais_on_revived
};

struct Request {
    Operation operation{};
    std::uintptr_t character{};
    std::uintptr_t ai{};
    std::uintptr_t active_ais{};
    std::uint32_t duration_ms{};
    std::int32_t repeat{};
    std::int32_t event{};
};

struct Reply {
    std::uint32_t duration_ms{};
    std::int32_t timer_id{-1};
};

struct Services {
    void* context{};
    // Executes the reached operation against the existing Character,
    // CharTimers, active AIS and Coordinator owners. Nonzero/throw is failure.
    int (*invoke)(void*, const Request*, Reply*, std::string& error){};
};

struct Result {
    std::uintptr_t character{}, ai{}, active_ais{};
    std::uint32_t calls{}, last_operation{}, timers_stopped{},
        ai_tick_started{}, dot_tick_started{}, active_ais_called{};
    std::int32_t ai_tick_timer{-1}, dot_tick_timer{-1};
};

enum class Status : std::int32_t {
    complete,
    invalid_argument,
    service_unavailable,
    service_failed
};

// CharAI::OnRevived source order: TMR_StopAll, start AI_Tick event 51, start
// DoT_Tick event 52, then active AIS::OnRevived. All state and effects remain
// on the caller's existing Character/CharAI/Coordinator owners.
Status execute(character_ai_initialization::State*, character::Coordinator*,
    std::uintptr_t expected_character, std::uintptr_t expected_ai,
    std::uintptr_t expected_active_ais, const Services&, Result*,
    std::string& error);

} // namespace dh2::character_ai_on_revived_v1
