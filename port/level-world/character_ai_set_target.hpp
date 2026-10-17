#pragma once

#include <cstdint>

namespace dh2::character::set_target {

// Borrowed source projections. These are not overlays of the original 32-bit
// Character/CharAI objects.
struct OwnerFacts {
    std::uintptr_t identity;
    std::int32_t character_ai_id;  // source Character+0xffc
    std::uint16_t target_change_marker_14d0;
    std::uint16_t reserved;
};

struct State {
    std::uintptr_t identity;
    OwnerFacts* owner;             // source CharAI+4, reread at use sites
    std::uintptr_t requested_target; // source CharAI+0x3c
    std::uintptr_t target;          // source CharAI+0x40
    std::uintptr_t last_target;     // source CharAI+0x44
    std::uint8_t alive_snapshot;    // source CharAI+0x48
    std::uint8_t sight_snapshot;    // source CharAI+0x49
    std::uint8_t sticky;            // source CharAI+0x4c
    std::uint8_t reserved;
};

struct Request {
    std::uint32_t operation;
    std::uint32_t key;
    std::uintptr_t ai_identity;
    std::uintptr_t owner_identity;
    std::uintptr_t target_identity;
};

struct Response {
    std::uint32_t word;
    std::uint32_t reserved;
};

struct Services {
    void* context;
    std::int32_t ai_property_count; // global Character AI row count
    int (*invoke)(void*, const Request*, Response*);
};

enum Operation : std::uint32_t {
    debug_switches_load = 1,
    debug_switch_lookup,
    target_is_dead,
    ai_is_in_sight
};

enum DebugKey : std::uint32_t {
    trace_target_changes = 1, // "IsTracingCharAITarget"
    trace_target_details = 2  // "isTracingCharAITarget"
};

enum Status : int {
    complete = 0,
    invalid_argument = 1,
    source_service_failed = 2
};

// Implements CharAI::AI_SetTarget(target, force). The original returns void;
// this adapter returns a port status. Service errors stop at that exact point
// and retain any source writes already performed.
extern "C" int dh2_character_ai_set_target(State*, std::uintptr_t target,
                                             std::uint8_t force,
                                             const Services*);

static_assert(sizeof(OwnerFacts) == 16);
static_assert(sizeof(State) == 48);
static_assert(sizeof(Request) == 32);
static_assert(sizeof(Response) == 8);

}  // namespace dh2::character::set_target
