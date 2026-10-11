#pragma once

#include "character_ai_initialization.hpp"
#include "character_coordinator.hpp"

#include <cstdint>
#include <string>

namespace dh2::monster_external_script { class Session; }

namespace dh2::character_external_script_timer_v1 {

enum class AISKind : std::uint8_t { default_ais, external_ais };

// Borrowed owners for CharAI::OnScriptTimer -> active AIS +0x90. The selected
// AIS class must use the source AISDefault implementation (directly or through
// AISExternal inheritance). Timer storage and Lua session stay caller-owned.
struct Bindings {
    AISKind kind{};
    std::uintptr_t character{};
    std::uintptr_t ai_identity{};
    std::uintptr_t retained_ais{};
    std::uintptr_t controller{};
    std::uintptr_t properties{};
    character_ai_initialization::State* ai{};
    character::Coordinator* coordinator{};
    monster_external_script::Session* session{};
};

struct Result {
    std::uint32_t callback_calls{};
    std::uint32_t event_service_calls{};
    bool inactive_ais{};
};

enum class Status : std::uint8_t {
    delivered,
    inactive_ais,
    invalid_owner,
    unavailable,
    failed,
};

// Dispatches one already-expired event-0x35 timer through the existing source
// CharAI event kernel, then calls OnTimer(uint32 id) on the same retained VM.
// It never advances or owns the Character's timer store.
Status dispatch(const Bindings&, std::uint32_t timer_id, Result*,
                std::string& error);

} // namespace dh2::character_external_script_timer_v1
