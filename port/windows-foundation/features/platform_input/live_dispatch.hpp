#pragma once
#include "semantic_input.hpp"
#include "../../../engine-ui/hud_attack_control_v46.hpp"
#include "../../../level-world/character_use_ooi_v47.hpp"
#include <memory>
#include <string>

namespace dh::foundation::platform_input {
// Reacquired once per dispatch. Every pointer aliases an existing live owner.
// This object contains no substitute Character, controller, HUD or skill state.
struct LiveBorrow {
    std::shared_ptr<void> receiver_lease;
    std::uint64_t actor_id{};
    std::uintptr_t character{}, controller{};
    dh2::ui::HudAttackHeldFieldsV46* hud_attack{};
    std::uint8_t* joystick_active{};
    const dh2::ui::HudAttackServicesV46* hud_services{};
    dh2::character::ControllerUseOoiV47* use_ooi{};
    // Must issue the actual same controller command, including admission gates.
    // Begin on pressed, End on released through actual Ctrl_Begin/EndSkill.
    // Held alone never fabricates repeated Begin or AI_UseSkill calls.
    std::function<bool(std::uintptr_t,std::uint32_t,ButtonEdges,std::string&)> skill;
    std::function<bool(std::uintptr_t,ButtonEdges,std::string&)> spell;
    std::function<bool(std::uintptr_t,std::string&)> potion;
    std::function<bool(std::uintptr_t,std::string&)> select_target;
    std::function<bool(std::uintptr_t,Point,std::string&)> target_point;
};
struct LiveDispatchResult {
    InputActions residual{};
    bool attack_dispatched{}, interaction_dispatched{};
    std::array<bool,3> skill_dispatched{};
};
using BorrowLive = std::function<bool(LiveBorrow&,std::string&)>;
// Call after handling menu transitions. Never dispatch the pre-open Frame after
// opening a menu. No provider/borrow is retained across reload, save or restore.
bool dispatch_live(const Frame&,std::uint64_t expected_actor,const BorrowLive&,
                   LiveDispatchResult&,std::string&);
}
