#include "character_ai_skill_machine_projection_v1.hpp"

#include <exception>
#include <stdexcept>

namespace dh2::character_ai_skill_machine_projection_v1 {

Projection::Projection(character::Coordinator& coordinator,
                       std::uintptr_t character_identity,
                       std::uintptr_t animation_owner_identity)
    : coordinator_(&coordinator) {
    if (!character_identity || character_identity != coordinator.owner() ||
        !animation_owner_identity)
        throw std::invalid_argument(
            "Skill machine projection requires the same Character owner and an animation owner");

    // IDA: CharStateMachine::CharStateMachine (0x3c1ac4) initializes the
    // selected-animation field at +0x28 to -1 and the +0x54/+0x58 fields to 0.
    // SM_SetSkillState (0x3c6670) writes those exact fields before event 0xc355.
    // SM_GetState (0x3c01ac) reads the installed state's first word through
    // machine+0x20; this port maps that read to the Coordinator's sole state ID.
    state_query_.current_state_id = &coordinator.state.current;
    machine_.identity = reinterpret_cast<std::uintptr_t>(&machine_);
    machine_.owner_04 = character_identity;
    machine_.state_query = &state_query_;
    machine_.animation_28 = &animation_28_;
    machine_.skill_index_54 = &skill_index_54_;
    machine_.moving_58 = &moving_58_;
    character_.identity = character_identity;
    character_.skill_machine_4fc = &machine_;
    // Character+0x49c is the CharAnimator receiver used by AI_EndSkill's
    // StopLoop branch (0x3d8474). Require the host to provide its identity.
    character_.stop_skill_loop_receiver_49c = animation_owner_identity;
}

Projection::~Projection() {
    // A bound projection is borrowed by Coordinator. Silently destroying it
    // would leave a dangling callback graph. Normal retirement detaches after
    // leaving state 6; fail hard if the owner violates that lifecycle.
    if (fsm_bound_ &&
        (!coordinator_ ||
         !coordinator_->unbind_skill_projection(&fsm_projection_)))
        std::terminate();
}

bool Projection::bind_skill_state_callbacks(std::uintptr_t ai_identity,
    std::uintptr_t debug_switches_identity, std::uint8_t* ooi_intent_412,
    const std::uintptr_t* physical_2dc,
    const character_skill_fsm_callbacks_v1::Services& services,
    std::string& error) {
    error.clear();
    if (!coordinator_ || !coordinator_->bound() ||
        coordinator_->owner() != character_.identity ||
        !ai_identity || !debug_switches_identity || !ooi_intent_412 ||
        !physical_2dc || !services.invoke || !machine_.identity ||
        machine_.owner_04 != character_.identity ||
        character_.stop_skill_loop_receiver_49c == 0 ||
        (!fsm_bound_ && coordinator_->state.current == 6)) {
        error = "CSSkill callback binding requires the same live Character, AI, Debug, source fields and operation provider before state 6";
        return false;
    }
    if (fsm_bound_) {
        const auto& current = fsm_character_;
        const bool same = current.identity == character_.identity &&
            current.ai == ai_identity && current.machine == machine_.identity &&
            current.animator == character_.stop_skill_loop_receiver_49c &&
            current.flags_520 == &coordinator_->state.flags &&
            current.flags_528 == &coordinator_->state.attack_gate &&
            current.ooi_intent_412 == ooi_intent_412 &&
            current.machine_moving_58 == machine_.moving_58 &&
            current.physical_2dc == physical_2dc &&
            fsm_globals_.debug_switches == debug_switches_identity &&
            fsm_services_.context == services.context &&
            fsm_services_.invoke == services.invoke;
        if (same) {
            // Coordinator::bind replaces its base service bundle and may
            // clear optional projections while retaining this callback graph.
            // Reattach our borrowed projection instead of reporting a stale
            // successful bind after Home/resume or another base rebind.
            if (!coordinator_->bind_skill_projection(&fsm_projection_)) {
                error = "CSSkill callback projection no longer belongs to the active Coordinator";
                return false;
            }
            return true;
        }
        error = "CSSkill callback graph is already bound to different borrowed owners";
        return false;
    }

    fsm_character_ = {character_.identity, ai_identity, machine_.identity,
        character_.stop_skill_loop_receiver_49c,
        reinterpret_cast<std::uintptr_t>(&coordinator_->timers()),
        &coordinator_->state.flags, &coordinator_->state.attack_gate,
        ooi_intent_412, machine_.moving_58, physical_2dc};
    fsm_state_ = {&fsm_character_};
    fsm_globals_ = {debug_switches_identity};
    fsm_services_ = services;
    fsm_projection_ = {&coordinator_->state, &fsm_state_, &fsm_globals_,
                       &fsm_services_};
    if (!coordinator_->bind_skill_projection(&fsm_projection_)) {
        fsm_character_ = {};
        fsm_state_ = {};
        fsm_globals_ = {};
        fsm_services_ = {};
        fsm_projection_ = {};
        error = "CSSkill callback projection could not bind to the existing Coordinator";
        return false;
    }
    fsm_bound_ = true;
    return true;
}

bool Projection::unbind_skill_state_callbacks(std::string& error) {
    error.clear();
    if (!fsm_bound_ || !coordinator_) {
        error = "CSSkill callback projection is not bound";
        return false;
    }
    if (!coordinator_->unbind_skill_projection(&fsm_projection_)) {
        error = "CSSkill callback projection cannot detach while dispatch is active or state 6 is live";
        return false;
    }
    fsm_bound_ = false;
    fsm_projection_ = {};
    fsm_services_ = {};
    fsm_globals_ = {};
    fsm_state_ = {};
    fsm_character_ = {};
    return true;
}

bool Projection::retire_skill_state_callbacks(std::string& error) {
    error.clear();
    if (!fsm_bound_ || !coordinator_) {
        error = "CSSkill callback projection is not bound";
        return false;
    }
    if (!coordinator_->retire_skill_projection(&fsm_projection_)) {
        const auto status=coordinator_->skill_projection_retirement_status(&fsm_projection_);
        error = "CSSkill callback projection retirement blocked: coordinator_bound="+
            std::to_string(status.coordinator_bound)+
            " active_facts="+std::to_string(status.dispatch_active)+
            " timer_update_depth="+std::to_string(status.timer_update_depth)+
            " projection_matches="+std::to_string(status.projection_matches)+
            " expected_projection="+std::to_string(status.expected_projection)+
            " bound_projection="+std::to_string(status.bound_projection)+
            " state="+std::to_string(status.current_state);
        return false;
    }
    fsm_bound_ = false;
    fsm_projection_ = {};
    fsm_services_ = {};
    fsm_globals_ = {};
    fsm_state_ = {};
    fsm_character_ = {};
    return true;
}

} // namespace dh2::character_ai_skill_machine_projection_v1
