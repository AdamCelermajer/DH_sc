#pragma once

#include "character_state.hpp"
#include "character_skill_fsm_callbacks_v1.hpp"

namespace dh2::character_skill_state_dispatch_v1 {

// Borrows the sole Coordinator State and the captured CSSkill callback graph.
// Character flags520/528 must point at that State's flags/attack_gate. The raw
// Character heading412, machine moving58 and physical2dc remain live producer
// fields; this projection allocates no FSM, AI, animation, timer or VM owner.
struct Projection {
    character::State* machine;
    character_skill_fsm_callbacks_v1::State* callbacks;
    const character_skill_fsm_callbacks_v1::Globals* globals;
    const character_skill_fsm_callbacks_v1::Services* services;
};

enum class Status : std::int32_t {
    complete, invalid_argument, invalid_source_fact, service_unavailable,
    service_failed, unsupported_assertion, unsupported_source_state
};
struct Result {
    std::int32_t next = -1;
    std::uint32_t on_event_writes = 0, predicate = 0, registered = 0;
    character_skill_fsm_callbacks_v1::Result callback{};
};

// Reuses the frozen CSSkill Focus/Blur callers. The caller already installed
// state6 for Focus and still has state6 installed for Blur. A failed provider
// preserves source effects and the callback prefix; no cleanup/transition is
// added. State6 selection/elapsed reset/Character event1d belong to Coordinator.
Status callback(Projection*, character_skill_fsm_callbacks_v1::Callback, Result*);

// State6 OnEvent28 performs strcmp(payload,"is_stoppable") and sets flags8000
// only on equality, then registered predicates see the updated flags. Entries
// from selected states3/4/5 onC355 select6 unconditionally. Source6 maps22->3,
// C358->12; C351->4,C354->5,C355->6 require flags8000. Interrupt C35A/B/C/D
// requires flags10000 and targets11/10/9/8; accepted destinations outside the
// selected state set return unsupported_source_state with next preserved.
// No transition is executed here. The single Coordinator must run outgoing
// Blur, install next, reset elapsed iff IDs differ, incoming Focus and event1d.
Status event(character::State*, std::uint32_t event, const char* payload, Result*);

// CSSkill OnUpdate is the original bx-lr body: no calls, animation timeout or
// transition. Machine elapsed accounting remains at its existing owner.
Status update(const character::State*, Result*);

// Borrowed storage lives through synchronous return on one owning thread.
// Labels are valid NUL-terminated source strings. Invalid port controls leave
// Result and producer fields unchanged. Raw event payloads for events other
// than28 are ignored. Callbacks may reenter with distinct output/controls as
// permitted by the frozen caller; captured producer backing cannot retire.
} // namespace dh2::character_skill_state_dispatch_v1
