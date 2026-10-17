#pragma once

#include <cstddef>
#include <cstdint>

namespace dh2::ui {

// Evidence required before Application::SetLanguage may traverse a native
// Level. Values must come from the live owners, never from renderer counts or
// caller assertions. Current Crypt projections intentionally leave the
// lifecycle/type/cache proof fields false.
struct LevelLanguageSceneReadinessV1 {
    std::uintptr_t level_identity{};
    std::uint64_t level_generation{};
    std::uintptr_t object_manager_identity{};
    std::uintptr_t object_manager_level_identity{};
    std::uint64_t object_manager_generation{};
    std::uint64_t object_order_generation{};
    std::size_t expected_object_count{};
    std::size_t object_count{};
    std::uintptr_t character_list_identity{};
    std::uintptr_t character_list_level_identity{};
    std::uint64_t character_list_generation{};
    std::size_t expected_character_count{};
    std::size_t character_count{};

    bool source_add_remove_flush_complete{};
    bool character_roster_complete{};
    bool role_predicates_complete{};
    bool item_type3_refresh_complete{};
    bool type14_cache_byte_complete{};
    bool one_localization_owner{};
};

enum class LevelLanguageSceneReadinessStatusV1 : std::uint8_t {
    ready,
    missing_owner,
    wrong_level_owner,
    stale_generation,
    membership_count_mismatch,
    incomplete_object_manager_lifecycle,
    incomplete_character_roster,
    missing_role_predicates,
    missing_item_refresh,
    missing_type14_cache_byte,
    duplicate_or_missing_localization_owner,
};

inline LevelLanguageSceneReadinessStatusV1 check_level_language_scene_readiness_v1(
        const LevelLanguageSceneReadinessV1& proof) noexcept {
    if (!proof.level_identity || !proof.level_generation ||
        !proof.object_manager_identity || !proof.character_list_identity)
        return LevelLanguageSceneReadinessStatusV1::missing_owner;
    if (proof.object_manager_level_identity != proof.level_identity ||
        proof.character_list_level_identity != proof.level_identity)
        return LevelLanguageSceneReadinessStatusV1::wrong_level_owner;
    if (proof.object_manager_generation != proof.level_generation ||
        proof.character_list_generation != proof.level_generation ||
        proof.object_order_generation != proof.level_generation)
        return LevelLanguageSceneReadinessStatusV1::stale_generation;
    if (proof.object_count != proof.expected_object_count ||
        proof.character_count != proof.expected_character_count)
        return LevelLanguageSceneReadinessStatusV1::membership_count_mismatch;
    if (!proof.source_add_remove_flush_complete)
        return LevelLanguageSceneReadinessStatusV1::incomplete_object_manager_lifecycle;
    if (!proof.character_roster_complete)
        return LevelLanguageSceneReadinessStatusV1::incomplete_character_roster;
    if (!proof.role_predicates_complete)
        return LevelLanguageSceneReadinessStatusV1::missing_role_predicates;
    if (!proof.item_type3_refresh_complete)
        return LevelLanguageSceneReadinessStatusV1::missing_item_refresh;
    if (!proof.type14_cache_byte_complete)
        return LevelLanguageSceneReadinessStatusV1::missing_type14_cache_byte;
    if (!proof.one_localization_owner)
        return LevelLanguageSceneReadinessStatusV1::duplicate_or_missing_localization_owner;
    return LevelLanguageSceneReadinessStatusV1::ready;
}

} // namespace dh2::ui
