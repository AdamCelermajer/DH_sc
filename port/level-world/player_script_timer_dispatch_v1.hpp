#pragma once

#include "character_ai_initialization.hpp"
#include "character_coordinator.hpp"
#include "../adam-script-runtime/script_game_bindings.h"

#include <cstdint>
#include <string>

namespace dh2::player_script_timer_dispatch_v1 {

struct Bindings {
    std::uintptr_t character = 0;
    std::uintptr_t ai_identity = 0;
    std::uintptr_t retained_ais = 0;
    std::uintptr_t controller = 0;
    std::uintptr_t properties = 0;
    character_ai_initialization::State* ai = nullptr;
    character::Coordinator* coordinator = nullptr;
    dh2_script_vm* vm = nullptr;
};

enum class Status { delivered, inactive_ais, invalid_owner, unavailable, failed };

// Source CharAI::OnScriptTimer is an active-AIS gate followed by the AIS
// OnScriptTimer virtual. This adapter executes the retained CharAI event kernel
// and implements slot +0x90 for the one retained AISDefault/Player VM. It
// borrows the triggering timer from the same Character Coordinator.
Status dispatch(const Bindings&, std::uint32_t timer_id, std::string& error);

} // namespace dh2::player_script_timer_dispatch_v1
