#pragma once

#include "character_ai_skill_commands_v1.hpp"
#include "character_coordinator.hpp"
#include "character_skill_state_dispatch_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::character_ai_skill_machine_projection_v1 {

// Logical source projection of Character+0x4fc's CharStateMachine. It borrows
// the single Coordinator state ID and owns only the three additional machine
// fields consumed by AI_BeginSkill/SM_SetSkillState. It is not another FSM.
// The animation receiver is supplied by the existing host animation owner;
// this layer does not invent an identity for it.
class Projection final {
    std::int32_t animation_28_ = -1;
    std::uint32_t skill_index_54_ = 0;
    std::uint8_t moving_58_ = 0;
    character_skill_state_queries::Machine state_query_{};
    character_ai_skill_commands_v1::Machine machine_{};
    character_ai_skill_commands_v1::Character character_{};
    character_skill_fsm_callbacks_v1::Character fsm_character_{};
    character_skill_fsm_callbacks_v1::State fsm_state_{};
    character_skill_fsm_callbacks_v1::Globals fsm_globals_{};
    character_skill_fsm_callbacks_v1::Services fsm_services_{};
    character_skill_state_dispatch_v1::Projection fsm_projection_{};
    character::Coordinator* coordinator_ = nullptr;
    bool fsm_bound_ = false;

public:
    Projection(character::Coordinator&, std::uintptr_t character_identity,
               std::uintptr_t animation_owner_identity);
    ~Projection();
    Projection(const Projection&) = delete;
    Projection& operator=(const Projection&) = delete;
    Projection(Projection&&) = delete;
    Projection& operator=(Projection&&) = delete;

    // Pointers are stable until this projection or its Coordinator is retired.
    // The command kernel's current-state query reads Coordinator::state.current
    // afresh; AI writes remain in the one projected source-machine field set.
    character_ai_skill_commands_v1::Character* character() noexcept {
        return &character_;
    }
    character_ai_skill_commands_v1::Machine* machine() noexcept {
        return &machine_;
    }
    const character_skill_state_queries::Machine* state_query() const noexcept {
        return &state_query_;
    }

    // Bind the exact CSSkill Focus/Blur caller graph to this same machine and
    // Coordinator. OOI-intent and physical are borrowed source Character fields;
    // callbacks must provide every reached operation and fail on missing
    // services. `debug_switches_identity` is the live Debug singleton handle.
    // Binding is valid only before entering state 6. No FSM, VM, timer store,
    // property sheet, or animation owner is created here.
    bool bind_skill_state_callbacks(std::uintptr_t ai_identity,
        std::uintptr_t debug_switches_identity,
        std::uint8_t* ooi_intent_412,
        const std::uintptr_t* physical_2dc,
        const character_skill_fsm_callbacks_v1::Services&,
        std::string& error);
    // Normal unbind must follow leaving state 6 because outgoing Blur needs
    // this graph. Terminal Character destruction uses the separate retire
    // path and does not invoke that callback.
    bool unbind_skill_state_callbacks(std::string& error);
    // Character destruction discards the whole source state machine without
    // invoking CSSkill::OnBlur; use only after the owning session is terminal.
    bool retire_skill_state_callbacks(std::string& error);
    bool skill_state_callbacks_bound() const noexcept { return fsm_bound_; }
};

} // namespace dh2::character_ai_skill_machine_projection_v1
