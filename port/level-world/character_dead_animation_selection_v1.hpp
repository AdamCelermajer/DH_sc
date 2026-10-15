#pragma once

#include "character_anim_table_resolver_v1.hpp"
#include "character_ai_classification.hpp"
#include "character_stance.hpp"

#include <cstdint>
#include <cstring>
#include <string>

extern "C" {
#include "../pydata-constants/constants.h"
}

namespace dh2::character_dead_animation_selection_v1 {

struct Request {
    character_anim_table_resolver_v1::CharacterView character{};
    const data::AnimationTables* tables = nullptr;
    const dh2_pycst_view* constants = nullptr;
    character_ai_classification::State* classification = nullptr;
    const character_ai_classification::Services* classification_services = nullptr;
    // These are independent source reads: AI_SetDead/SM_SetDeadState may
    // observe a changed flag before choosing the despawn variant.
    std::uint8_t great_knockback_for_death = 0;
    std::uint8_t great_knockback_for_despawn = 0;
    // SM_SetDeadState rereads machine byte +0x3f for the despawn branch after
    // the first animation's optional GetAnimStance call. This live projection
    // preserves that ordering when a provider reenters the same Coordinator.
    const std::uint32_t* live_great_knockback_for_despawn = nullptr;
    // Exact Character::GetAnimStance projection. Reached only if the selected
    // sequence is marked in AnimStancedAnim/SL__LIST_IPHONE.
    void* anim_stance_context = nullptr;
    int (*get_anim_stance)(void*, std::uintptr_t character,
                           std::int32_t* stance) = nullptr;
};

struct Result {
    std::int32_t table_id = 17;
    std::uint32_t table_count = 0;
    std::uint32_t row_valid = 0;
    std::int32_t death_animation = -1;
    std::int32_t despawn_animation = -1;
    std::uint32_t mask_queries = 0;
    std::uint32_t stance_queries = 0;
};

struct OneAnimationRequest {
    std::uintptr_t character = 0;
    const data::CharacterAnimations* row = nullptr;
    const dh2_pycst_view* constants = nullptr;
    std::uint32_t field = 0;
    std::uint32_t stance_mask = 0;
    character_ai_classification::State* classification = nullptr;
    const character_ai_classification::Services* classification_services = nullptr;
    void* anim_stance_context = nullptr;
    int (*get_anim_stance)(void*, std::uintptr_t character,
                           std::int32_t* stance) = nullptr;
};

struct OneAnimationResult {
    std::int32_t animation = -1;
    std::uint32_t mask_queries = 0;
    std::uint32_t stance_queries = 0;
    std::uint32_t failure_point = 0; // diagnostic: which source check rejected
};

enum class Status : std::uint32_t {
    complete,
    invalid_argument,
    invalid_source_fact,
};

namespace detail {
inline bool constant(const dh2_pycst_view* view, const char* group,
                     const char* key, std::int32_t& value) noexcept {
    if (!view || !view->bytes || !group || !key) return false;
    dh2_pycst_result found{};
    if (dh2_pycst_get(view, group, static_cast<std::uint32_t>(std::strlen(group)),
                      key, static_cast<std::uint32_t>(std::strlen(key)), &found) ||
        !found.found) return false;
    value = found.value;
    return true;
}

inline bool row_scalar(const data::CharacterAnimations& row,
                       std::size_t field, std::int32_t& value) noexcept {
    if (field >= row.fields.size() || row.fields[field].size() != 1) return false;
    value = row.fields[field][0];
    return true;
}

inline std::int32_t wrap_add(std::int32_t value, std::int32_t stance) noexcept {
    const auto bits = static_cast<std::uint32_t>(value) +
                      static_cast<std::uint32_t>(stance);
    std::int32_t result = 0;
    std::memcpy(&result, &bits, sizeof(result));
    return result;
}
} // namespace detail

inline Status select_one(const OneAnimationRequest* request,
                         OneAnimationResult* output) noexcept {
    if (!request || !output || !request->character || !request->row ||
        !request->constants || request->field >= request->row->fields.size())
        return Status::invalid_argument;
    OneAnimationResult result{};
    if (!detail::row_scalar(*request->row, request->field, result.animation))
        { output->failure_point = 1; return Status::invalid_source_fact; }
    std::int32_t source_mask = 0;
    if (!detail::constant(request->constants, "AnimStancedAnim",
                          "SL__LIST_IPHONE", source_mask))
        { output->failure_point = 2; return Status::invalid_source_fact; }
    ++result.mask_queries;
    if ((static_cast<std::uint32_t>(source_mask) & request->stance_mask) != 0) {
        std::int32_t stance = 0;
        if (request->get_anim_stance) {
            int code = 1;
            try {
                code = request->get_anim_stance(request->anim_stance_context,
                                                request->character, &stance);
            } catch (...) {
                { output->failure_point = 3; return Status::invalid_source_fact; }
            }
            if (code != 0)
                { output->failure_point = 4; return Status::invalid_source_fact; }
            std::int32_t stance_count = 0;
            if (!detail::constant(request->constants, "AnimStances",
                                  "COUNT_IPHONE", stance_count) ||
                stance < 0 || stance >= stance_count)
                { output->failure_point = 5; return Status::invalid_source_fact; }
        } else {
            if (!request->classification || !request->classification_services ||
                request->classification->character != request->character)
                { output->failure_point = 6; return Status::invalid_source_fact; }
            character_ai_classification::Result classification{};
            if (character_ai_classification::query(
                    character_ai_classification::Query::monster,
                    request->classification, request->classification_services,
                    &classification) != character_ai_classification::Status::complete ||
                classification.word != 1)
                { output->failure_point = 7; return Status::invalid_source_fact; }
            std::int32_t stance_count = 0;
            if (!detail::constant(request->constants, "AnimStances",
                                  "COUNT_IPHONE", stance_count))
                { output->failure_point = 8; return Status::invalid_source_fact; }
            character::StanceFacts16 facts{};
            facts.count = stance_count;
            if (dh2_character_anim_stance(&stance, &facts) != 1)
                { output->failure_point = 9; return Status::invalid_source_fact; }
        }
        ++result.stance_queries;
        result.animation = detail::wrap_add(result.animation, stance);
    }
    *output = result;
    return Status::complete;
}

// Reads the same cached CharAnimTable row and source fields used by
// SM_SetDeadState: DeadlyGreatKB +0x10 (field 4), Despawn +0x14 (field 5),
// DespawnGreatKB +0x18 (field 6), and Died +0x1c (field 7). The caller must
// supply two fresh great-knockback observations. Stance is reached only when
// the selected bit is present in AnimStancedAnim/SL__LIST_IPHONE. This slice
// accepts either the legacy verified Monster path or an explicit source
// Character::GetAnimStance provider, which also covers Faeries and Players.
inline Status select(const Request* request, Result* output) noexcept {
    if (!request || !output || !request->character.identity ||
        !request->character.properties || !request->tables ||
        !request->constants)
        return Status::invalid_argument;

    character_anim_table_resolver_v1::Result table{};
    const auto table_status = character_anim_table_resolver_v1::resolve(
        &request->character, request->tables, &table);
    if (table_status == character_anim_table_resolver_v1::Status::invalid_argument)
        return Status::invalid_argument;
    if (table_status != character_anim_table_resolver_v1::Status::complete)
        return Status::invalid_source_fact;

    Result result{};
    result.table_id = table.table_id;
    result.table_count = table.table_count;
    if (!table.row) {
        *output = result;
        return Status::complete;
    }
    result.row_valid = 1;

    const bool death_great = request->great_knockback_for_death != 0;
    OneAnimationRequest one{};
    one.character = request->character.identity;
    one.row = table.row;
    one.constants = request->constants;
    one.field = death_great ? 3u : 6u;
    one.stance_mask = death_great ? 0x20000u : 0x8000u;
    one.classification = request->classification;
    one.classification_services = request->classification_services;
    one.anim_stance_context = request->anim_stance_context;
    one.get_anim_stance = request->get_anim_stance;
    OneAnimationResult selected{};
    auto status = select_one(&one, &selected);
    if (status != Status::complete) {
        *output = result;
        return status;
    }
    result.death_animation = selected.animation;
    result.mask_queries += selected.mask_queries;
    result.stance_queries += selected.stance_queries;

    const bool despawn_great = request->live_great_knockback_for_despawn
        ? *request->live_great_knockback_for_despawn != 0
        : request->great_knockback_for_despawn != 0;
    one.field = despawn_great ? 5u : 4u;
    one.stance_mask = despawn_great ? 0x40000u : 0x10000u;
    status = select_one(&one, &selected);
    result.despawn_animation = selected.animation;
    result.mask_queries += selected.mask_queries;
    result.stance_queries += selected.stance_queries;
    *output = result;
    return status;
}

} // namespace dh2::character_dead_animation_selection_v1
