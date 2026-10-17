#pragma once
#include <cstdint>

namespace dh2::character_ai_master_update {
// Live port fields, not an overlay of the original 32-bit object. ai is stable
// through the call; callbacks may edit owner/master and the two snapshots.
struct State {
    std::uintptr_t ai, owner, master;
    std::uint8_t alive_54, sight_55;
    std::uint16_t reserved;
};
enum class Operation : std::uint32_t {
    char_ai_id, master_is_dead, sight, my_turn, can_range_attack,
    close_range, range, melee_range, raise_event,
};
struct Request {
    Operation operation;
    std::uint32_t event;
    std::uintptr_t subject, peer;
};
struct Response { std::uint32_t word; };
struct Services {
    void* context;
    // Synchronous borrowed services. May replace owner/master and edit snapshots.
    // Zero succeeds. Exceptions become service_failed. Range and turn queries
    // remain their exact named original boundaries, not guessed distances.
    std::int32_t (*invoke)(void*, State*, const Request*, Response*);
};
struct Result { std::uint32_t calls, events, last_event, last_word; };
enum class Status : std::int32_t {
    complete, invalid_argument, service_failed, invalid_source_fact,
};
// Source _UpdateMaster orchestration. Emits events before writing cached alive
// and sight bytes. Full sight word remains the later gate even if its low byte
// is zero. Providers and event effects are retained on failure, without rollback.
// One owning thread. Borrowed projections/providers must outlive callbacks;
// callback reentry into the same state/output is forbidden. Independent calls
// are allowed. Output must not overlap state/services/context-owned state.
Status update(State*, const Services*, Result*);
}
