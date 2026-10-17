#pragma once

#include "native_ghost_skills.hpp"

#include "../../../../../../port/level-world/character_ai_classification.hpp"
#include "../../../../../../port/level-world/player_ai_death_v1.hpp"

namespace dh2::native::ghost_death {

// Borrow the same source owners used by the retained Monster Character and
// CharAI. This adapter handles only GetAnimStance and skill/spell cleanup;
// group callbacks, AIS OnDied, target relations and aggro remain outer services.
struct Bindings {
    std::uintptr_t ai = 0;
    std::uintptr_t character = 0;
    const dh2_pycst_view* constants = nullptr;
    character_ai_classification::State* classification = nullptr;
    const character_ai_classification::Services* classification_services = nullptr;
    ghost_skills::Runtime* skills = nullptr;
};

// Compatible with player_ai_death_v1::Backend::invoke.
int invoke(void* context, const player_ai_death_v1::Request* request,
           player_ai_death_v1::Reply* reply, std::string& error);

} // namespace dh2::native::ghost_death
