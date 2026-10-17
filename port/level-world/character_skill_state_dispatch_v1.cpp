#include "character_skill_state_dispatch_v1.hpp"

#include <cstddef>
#include <cstring>

namespace dh2::character_skill_state_dispatch_v1 {
namespace {
struct Range { std::uintptr_t begin, end; };
bool range(const void* p, std::size_t size, std::size_t alignment, Range& out) {
    const auto at = reinterpret_cast<std::uintptr_t>(p);
    if (!p || at % alignment || at > UINTPTR_MAX - size) return false;
    out = {at, at + size}; return true;
}
bool overlaps(Range a, Range b) { return a.begin < b.end && b.begin < a.end; }
template<class T> bool object(const T* p, Range& out) {
    return range(p, sizeof(T), alignof(T), out);
}
bool controls(const character::State* state, const Result* out) {
    Range a{}, b{};
    return object(state, a) && object(out, b) && !overlaps(a, b);
}
bool projection(const Projection* p, const Result* out) {
    Range controls_range[6]{};
    if (!object(p, controls_range[0]) || !object(out, controls_range[1]) ||
        !object(p->machine, controls_range[2]) ||
        !object(p->callbacks, controls_range[3]) ||
        !object(p->globals, controls_range[4]) ||
        !object(p->services, controls_range[5])) return false;
    for (unsigned i=0; i<6; ++i)
        for (unsigned j=0; j<i; ++j)
            if (overlaps(controls_range[i], controls_range[j])) return false;
    Range owner{};
    if (!object(p->callbacks->character, owner)) return false;
    for (const auto r : controls_range) if (overlaps(owner, r)) return false;
    const auto& c = *p->callbacks->character;
    if (c.flags_520 != &p->machine->flags ||
        c.flags_528 != &p->machine->attack_gate) return false;
    // The frozen caller performs its complete graph validation. Check its
    // remaining producer fields against this wrapper's extra controls before
    // invoking it; its temporary Result cannot cover the caller's Result.
    Range fields[3]{};
    if (!object(c.ooi_intent_412, fields[0]) ||
        !object(c.machine_moving_58, fields[1]) ||
        !object(c.physical_2dc, fields[2])) return false;
    for (const auto field : fields) {
        if (overlaps(field, owner)) return false;
        for (const auto r : controls_range) if (overlaps(field, r)) return false;
    }
    return true;
}
} // namespace

Status callback(Projection* p, character_skill_fsm_callbacks_v1::Callback which,
                Result* out) {
    namespace frozen = character_skill_fsm_callbacks_v1;
    if (which > frozen::Callback::blur || !projection(p, out))
        return Status::invalid_argument;
    if (p->machine->current != 6) return Status::invalid_source_fact;
    Result result{};
    const auto status = frozen::execute(p->callbacks, which, p->globals,
                                        p->services, &result.callback);
    if (status == frozen::Status::invalid_argument) return Status::invalid_argument;
    *out = result;
    return static_cast<Status>(status);
}

Status event(character::State* state, std::uint32_t event_id,
             const char* payload, Result* out) {
    if (!controls(state, out)) return Status::invalid_argument;
    const auto current = state->current;
    if (current != 3 && current != 4 && current != 5 && current != 6)
        return Status::invalid_source_fact;
    Range text{}, state_range{}, result_range{};
    if (current == 6 && event_id == 0x28) {
        // strcmp only visits the equal prefix through its first mismatch or
        // the literal NUL; labels shorter than13 bytes remain valid strings.
        if (!range(payload, sizeof("is_stoppable"), 1, text) || !object(state, state_range) ||
            !object(out, result_range) || overlaps(text, state_range) ||
            overlaps(text, result_range)) return Status::invalid_argument;
    }
    *out = {};
    if (current != 6) {
        if (event_id == 0xc355) { out->next = 6; out->registered = 1; }
        return Status::complete;
    }
    if (event_id == 0x28 && std::strcmp(payload, "is_stoppable") == 0) {
        state->flags |= 0x8000u; out->on_event_writes = 1;
    }
    if (event_id == 0x22 || event_id == 0xc358) {
        out->next = event_id == 0x22 ? 3 : 12;
        out->registered = 1; return Status::complete;
    }
    if (event_id == 0xc351 || event_id == 0xc354 || event_id == 0xc355) {
        out->registered = 1; out->predicate = (state->flags >> 15) & 1u;
        if (out->predicate)
            out->next = event_id == 0xc351 ? 4 : event_id == 0xc354 ? 5 : 6;
        return Status::complete;
    }
    if (event_id >= 0xc35a && event_id <= 0xc35d) {
        out->registered = 1; out->predicate = (state->flags >> 16) & 1u;
        if (!out->predicate) return Status::complete;
        out->next = event_id == 0xc35a ? 11 : event_id == 0xc35b ? 10 :
                    event_id == 0xc35c ? 9 : 8;
        return Status::unsupported_source_state;
    }
    return Status::complete;
}

Status update(const character::State* state, Result* out) {
    if (!controls(state, out)) return Status::invalid_argument;
    if (state->current != 6) return Status::invalid_source_fact;
    *out = {}; return Status::complete;
}
} // namespace dh2::character_skill_state_dispatch_v1
