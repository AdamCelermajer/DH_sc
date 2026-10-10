#pragma once

#include <cstdint>

namespace dh2::character_target_out_of_sight_v1 {

struct State {
    std::uintptr_t ai;
    std::uintptr_t active_ais;
    std::uint32_t out_of_sight_4c;
    std::uint32_t reserved;
};

enum class Operation : std::uint32_t {
    capture_debug_switch_owner,
    query_debug_switch,
    is_target_a_character,
    get_target_as_character,
    clear_aggro,
    ais_on_target_out_of_sight,
    recapture_debug_switch_owner,
};

struct Request {
    Operation operation;
    std::uint32_t reserved;
    std::uintptr_t subject;
    std::uintptr_t argument;
};

struct Response {
    std::uintptr_t identity;
    std::uint32_t word;
};

struct Services {
    void* context;
    std::int32_t (*invoke)(void*, State*, const Request*, Response*);
};

struct Result {
    std::uint32_t provider_calls;
    std::uint32_t debug_switch_queries;
    std::uint32_t target_character_word;
    std::uint32_t out_of_sight_cleared;
    std::uint32_t debug_owner_changed;
    std::uint32_t reserved;
    std::uintptr_t target_character;
    std::uintptr_t ais_callback_target;
};

enum class Status : std::int32_t {
    complete,
    invalid_argument,
    service_unavailable,
    service_failed,
    source_invariant_failed,
};

// CharAI::OnTargetOutOfSight (original 0x3d2410): after its debug prelude,
// Character targets are resolved and passed to AI_ClearAggro; then CharAI+0x4c
// is cleared and the borrowed current AIS receives its no-argument slot+0x48.
// The provider preserves exact target/aggro and AIS lifetimes. This body does
// not choose an idle state; that remains in the existing AIS script callback.
Status run(State*, const Services*, Result*);

} // namespace dh2::character_target_out_of_sight_v1
