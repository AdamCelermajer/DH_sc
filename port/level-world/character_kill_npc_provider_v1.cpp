#include "character_kill_npc_provider_v1.hpp"

#include <exception>
#include <stdexcept>

namespace dh2::character_kill_npc_provider_v1 {

Runtime::Runtime(Bindings bindings)
    : bindings_(bindings),
      caller_(bindings.character, {this, invoke}) {
    if (!bindings_.character || !bindings_.dead ||
        !bindings_.properties || dh2_property_validate(bindings_.properties) ||
        !bindings_.classification_state ||
        bindings_.classification_state->character != bindings_.character ||
        !bindings_.classification_services ||
        *bindings_.dead > 1 || !bindings_.services.preflight ||
        !bindings_.services.player_branch ||
        !bindings_.services.raise_event2) {
        throw std::invalid_argument("Invalid Character::Kill NPC provider bindings");
    }
}

int Runtime::invoke(void* raw, const ctrl::Request* request,
                    ctrl::Reply* reply, std::string& error) {
    if (!raw || !request || !reply) {
        error = "Character::Kill NPC provider received an invalid request";
        return 1;
    }
    return static_cast<Runtime*>(raw)->dispatch(*request, *reply, error);
}

int Runtime::dispatch(const ctrl::Request& request, ctrl::Reply& reply,
                      std::string& error) {
    if (busy_) {
        error = "Character::Kill NPC provider reentered";
        return 1;
    }
    if (request.character != bindings_.character || request.force > 1) {
        error = "Character::Kill NPC request identity or force is invalid";
        return 1;
    }
    busy_ = true;
    struct Release { bool& busy; ~Release() { busy = false; } } release{busy_};

    try {
        switch (request.operation) {
        case ctrl::Operation::is_dead:
            if (request.subject != bindings_.character) {
                error = "Character::IsDead receiver differs from the bound Character";
                return 1;
            }
            ++snapshot_.inner_is_dead_reads;
            reply.word = static_cast<std::int32_t>(*bindings_.dead);
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
            error = "Unexpected operation reached Character::Kill NPC provider";
            return 1;
        }
    } catch (const std::exception& ex) {
        error = ex.what();
    } catch (...) {
        error = "Character::Kill NPC provider threw";
    }
    if (error.empty()) error = "Character::Kill NPC provider failed";
    return 1;
}

int Runtime::inner_kill(std::uintptr_t killer, std::uint32_t force,
                        std::string& error) {
    // Character::Kill performs its own IsDead after Ctrl_Kill's outer check.
    // Already-dead is a successful no-op and does not consume a new episode.
    ++snapshot_.inner_is_dead_reads;
    if (*bindings_.dead != 0) return 0;
    if (kill_consumed_) {
        error = "Character::Kill episode was already consumed";
        return 1;
    }

    // This readiness callback must be observational only. It checks every
    // owner the selected branch can reach before the first dead/HP mutation.
    if (!bindings_.services.preflight(bindings_.services.context,
            bindings_.character, killer, force, error)) {
        if (error.empty()) error = "Character::Kill NPC preflight failed";
        return 1;
    }

    kill_consumed_ = true;
    snapshot_.prefix_started = 1;
    // Source Character::Kill 0x3a5b5c/0x3a5b68: dead byte +0x1449 first,
    // then PROPS_Set(36, 0). This is the same canonical state used by combat.
    *bindings_.dead = 1;
    snapshot_.dead_written = 1;
    snapshot_.hp_zero_attempted = 1;
    if (dh2_property_set(bindings_.properties, 36, 0)) {
        error = "Character::Kill PROPS_Set(36,0) failed";
        return 1;
    }
    snapshot_.hp_zero_completed = 1;

    snapshot_.is_player_attempted = 1;
    const auto classification_status = character_ai_classification::query(
        character_ai_classification::Query::player,
        bindings_.classification_state, bindings_.classification_services,
        &snapshot_.classification);
    if (classification_status != character_ai_classification::Status::complete) {
        error = "Character::Kill IsPlayer source classification failed";
        return 1;
    }
    snapshot_.is_player_result =
        static_cast<std::int32_t>(snapshot_.classification.word);
    snapshot_.is_player_completed = 1;

    if (snapshot_.is_player_result != 0 && force == 0) {
        snapshot_.player_branch_attempted = 1;
        if (!bindings_.services.player_branch(bindings_.services.context,
                bindings_.character, killer, force, error)) {
            if (error.empty()) error = "Character::Kill player continuation failed";
            return 1;
        }
        snapshot_.player_branch_completed = 1;
        error.clear();
        return 0;
    }

    snapshot_.general_branch_attempted = 1;
    character_kill_death_tail_v1::Runtime tail(
        {bindings_.character, killer, force}, bindings_.services.general_tail);
    const auto status = tail.run(&snapshot_.general_tail, error);
    if (status != character_kill_death_tail_v1::Status::complete) {
        if (error.empty()) error = "Character::Kill general branch failed";
        return 1;
    }
    snapshot_.general_branch_completed = 1;
    error.clear();
    return 0;
}

int Runtime::raise_event2(std::uintptr_t killer, std::string& error) {
    if (event2_consumed_) {
        error = "Character::RaiseEvent(2) episode was already consumed";
        return 1;
    }
    event2_consumed_ = true;
    snapshot_.event2_attempted = 1;
    if (!bindings_.services.raise_event2(bindings_.services.context,
            bindings_.character, killer, error)) {
        if (error.empty()) error = "Character::RaiseEvent(2,killer) failed";
        return 1;
    }
    snapshot_.event2_completed = 1;
    error.clear();
    return 0;
}

} // namespace dh2::character_kill_npc_provider_v1
