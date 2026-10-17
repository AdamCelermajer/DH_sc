#pragma once

#include "../game-data/ai.hpp"
#include "../game-data/items.hpp"

namespace dh2::character::player_attack_melee_radius_v1 {

// Player side of AI_GetMeleeRadius. The equipped Item row and AI table are
// borrowed from the same live Player inventory/Character snapshot.
enum class Status { complete, invalid_argument };

inline Status query(const data::ItemRecord164* main_hand,
                    const data::AiTables* ai_tables,
                    std::int32_t character_ai_id, float* output) noexcept {
    if (!ai_tables || !output || ai_tables->rows.size() <= 8)
        return Status::invalid_argument;
    const auto* ai = data::ai_props(*ai_tables, character_ai_id);
    if (!ai) return Status::invalid_argument;

    std::int32_t equipment_radius = 0;
    if (main_hand && main_hand->words[22] != 4 && main_hand->words[22] != 5)
        equipment_radius = main_hand->words[39];
    volatile float item_radius = static_cast<float>(equipment_radius);
    volatile float result = item_radius + ai->melee_radius;
    *output = result;
    return Status::complete;
}

} // namespace dh2::character::player_attack_melee_radius_v1
