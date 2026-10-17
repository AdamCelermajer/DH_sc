#pragma once

#include "character_state.hpp"

namespace dh2::character_cast_lifecycle_v1 {

enum class Callback : std::uint32_t { focus, blur };
enum class Operation : std::uint32_t {
    debug_load,
    string_construct,
    debug_query,
    string_destroy,
    raise_event,
    set_animation,
    set_speed,
    cancel_sneaking
};
enum class Status : std::int32_t {
    complete,
    invalid_argument,
    service_unavailable,
    service_failed,
    invalid_source_fact
};

// Borrowed views of the active Character/Coordinator and its existing machine
// and animator. flags_520 must be the Coordinator's one projected flags word;
// no second FSM, timer, animation, or Player owner is created here.
// The real CancelSneaking service may reach the retained Player/skill VM owners.
struct Character {
    std::uintptr_t identity = 0;
    character::State* coordinator_state = nullptr;
    std::uint32_t* flags_520 = nullptr;
    std::uintptr_t machine = 0;
    std::uintptr_t animator = 0;
    std::uint8_t* ooi_intent_412 = nullptr;
};
struct Globals { std::uintptr_t debug_switches = 0; };
struct Request {
    Operation operation;
    std::uintptr_t character;
    std::uintptr_t subject;
    std::uintptr_t string;
    std::uint32_t argument;
    const char* text;
};
struct Response { std::uintptr_t identity = 0; };
struct Services {
    void* context = nullptr;
    // Zero means the corresponding real source operation completed. The host
    // routes raise_event 32/33 through the existing Character/Coordinator path
    // (state7 OnPreSkill/OnPostSkill); other operations must also be real.
    int (*invoke)(void*, const Request*, Response*) = nullptr;
};
// Stable aggregate of borrowed owner identities, source field pointers and
// required real services. Coordinator retains only this projection's address.
struct Projection {
    Character character{};
    Globals globals{};
    Services services{};
};
struct Result {
    std::uint32_t calls = 0;
    std::uint32_t flags_written = 0;
    std::uint32_t ooi_intent_cleared = 0;
    std::uint32_t complete = 0;
    Operation last_operation = Operation::debug_load;
};

// IDA ARMv7: CSCast::OnFocus 0x3c39d0 and OnBlur 0x3c3934. Focus order is
// Debug load/query, flags=25345, RaiseEvent(32), SM_SetAnim(-1), speed=1,
// clear OOI intent, CancelSneaking. Blur loads/queries debug then raises 33.
// Every service is required; a provider failure stops at that exact prefix.
// Effects already delivered are retained with no rollback/compensating calls.
Status execute(Callback, const Character*, const Globals*, const Services*,
               Result*);
Status execute(Projection*, Callback, Result*);

} // namespace dh2::character_cast_lifecycle_v1
