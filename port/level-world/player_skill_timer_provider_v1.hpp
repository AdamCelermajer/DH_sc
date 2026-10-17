#pragma once

#include "character_coordinator.hpp"
#include "../adam-script-runtime/script_game_bindings.h"

#include <cstddef>
#include <cstdint>

namespace dh2::player_skill_timer_provider_v1 {

// The player skill callback borrows the Character's sole Coordinator timer
// store. It owns no timer slots or expiry state of its own.
struct Bindings {
    character::Coordinator* coordinator = nullptr;
    std::uintptr_t character = 0;
    dh2_script_game_bindings game{};
    std::int32_t diagnostic = 0;
};

bool bind(Bindings*, character::Coordinator*, std::uintptr_t character) noexcept;
int start(Bindings*, const dh2_script_value*, std::uint32_t,
          dh2_script_value*, std::uint32_t, std::uint32_t*, char*,
          std::size_t) noexcept;
int stop(Bindings*, const dh2_script_value*, std::uint32_t,
         dh2_script_value*, std::uint32_t, std::uint32_t*, char*,
         std::size_t) noexcept;

} // namespace dh2::player_skill_timer_provider_v1
