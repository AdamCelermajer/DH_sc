#pragma once

#include "subobjects_update.hpp"
#include "../engine-ui/scrolling_combat_text_position_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::scene { struct Scene; }

namespace dh2::player_sct_position_provider_v1 {

// Borrowed projections from the one canonical Prince Character/runtime.
// `character_identity` is NativePlayerCharacterOwnerV1::identity();
// `save_character_identity` is PlayerSavegameV1::character(). The caller
// supplies the live GameObject +0x80 byte because the current native Prince
// owner does not yet publish SetVisible state. No position or visibility is
// synthesized here.
struct Owner {
    std::uintptr_t character_identity{};
    std::uintptr_t save_character_identity{};
    const dh2::subobjects::State* runtime{};
    const scene::Scene* visual_scene{};
    const std::uint8_t* source_visible_80{};
};

// Suitable for ScrollingCombatTextPositionLookupV1. Requires the same live
// Character/Save identity and all canonical position/shape/visibility owners.
bool lookup(void* context, std::uintptr_t requested_identity,
            ui::ScrollingCombatTextPositionFactsV1& facts,
            std::string& error) noexcept;

} // namespace dh2::player_sct_position_provider_v1
