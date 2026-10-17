#include "player_character_kill_provider_v1.hpp"

#include <stdexcept>

namespace dh2::player_character_kill_provider_v1 {

Runtime::Runtime(Bindings bindings)
    : bindings_(bindings),
      continuation_({bindings.character, bindings.dead, bindings.properties,
                     bindings.continuation}),
      caller_(bindings.character, {this, invoke}) {
    if (!bindings_.character || !bindings_.dead || !bindings_.properties ||
        !bindings_.events.on_died || !bindings_.events.state_event ||
        *bindings_.dead > 1 || dh2_property_validate(bindings_.properties))
        throw std::invalid_argument("Invalid Player Character::Kill provider bindings");
}

int Runtime::invoke(void* raw, const ctrl::Request* request,
                    ctrl::Reply* reply, std::string& error) {
    if (!raw || !request || !reply) {
        error = "Player Character::Kill provider received an invalid request";
        return 1;
    }
    return static_cast<Runtime*>(raw)->dispatch(*request, *reply, error);
}

int Runtime::dispatch(const ctrl::Request& request, ctrl::Reply& reply,
                      std::string& error) {
    if (busy_) {
        error = "Player Character::Kill provider reentered";
        return 1;
    }
    if (request.character != bindings_.character || request.force > 1) {
        error = "Player Character::Kill request identity or force is invalid";
        return 1;
    }
    busy_ = true;
    struct Release { bool& busy; ~Release() { busy = false; } } release{busy_};

    switch (request.operation) {
    case ctrl::Operation::is_dead:
        if (request.subject != bindings_.character) {
            error = "Character::IsDead receiver differs from the bound Character";
            return 1;
        }
        ++snapshot_.outer_is_dead_reads;
        reply.word = *bindings_.dead;
        return 0;
    case ctrl::Operation::kill:
        if (request.subject != bindings_.character) {
            error = "Character::Kill receiver differs from the bound Character";
            return 1;
        }
        return inner_kill(request.killer, request.force, error);
    case ctrl::Operation::event2:
        if (request.subject != bindings_.character || request.argument != 2) {
            error = "Character::RaiseEvent arguments differ from event 2";
            return 1;
        }
        return raise_event2(request.killer, error);
    default:
        error = "Unexpected operation reached Player Character::Kill provider";
        return 1;
    }
}

int Runtime::inner_kill(std::uintptr_t killer, std::uint32_t force,
                        std::string& error) {
    // Character::Kill independently calls IsDead, even though Ctrl_Kill
    // already passed its own outer IsDead gate.
    ++snapshot_.inner_is_dead_reads;
    if (*bindings_.dead != 0) return 0;
    if (kill_consumed_) {
        error = "Player Character::Kill episode was already consumed";
        return 1;
    }
    kill_consumed_ = true;
    snapshot_.kill_prefix_started = 1;

    // Exact source order at Character::Kill 0x3a5b5c/0x3a5b68:
    // Character+0x1449 = 1, then CharProperties::PROPS_Set(36, 0).
    *bindings_.dead = 1;
    if (dh2_property_set(bindings_.properties, 36, 0)) {
        error = "Character::Kill PROPS_Set(36,0) failed";
        return 1;
    }

    if (continuation_.continue_after_hp0(killer, force,
            &snapshot_.continuation_result, error) != ctrl::Status::complete) {
        if (error.empty()) error = "Player Character::Kill continuation failed";
        return 1;
    }
    snapshot_.kill_prefix_completed = 1;
    return 0;
}

int Runtime::raise_event2(std::uintptr_t killer, std::string& error) {
    if (event_consumed_) {
        error = "Character::RaiseEvent(2) episode was already consumed";
        return 1;
    }
    event_consumed_ = true;
    snapshot_.on_died_started = 1;
    try {
        if (bindings_.events.on_died(bindings_.events.context,
                bindings_.character, killer, error)) {
            if (error.empty()) error = "Character::RaiseEvent OnDied failed";
            return 1;
        }
    } catch (...) {
        error = "Character::RaiseEvent OnDied provider threw";
        return 1;
    }
    snapshot_.on_died_completed = 1;

    snapshot_.state_event_started = 1;
    try {
        if (bindings_.events.state_event(bindings_.events.context,
                bindings_.character, 2, killer, error)) {
            if (error.empty()) error = "Character state event 2 failed";
            return 1;
        }
    } catch (...) {
        error = "Character state event 2 provider threw";
        return 1;
    }
    snapshot_.state_event_completed = 1;
    error.clear();
    return 0;
}

} // namespace dh2::player_character_kill_provider_v1
