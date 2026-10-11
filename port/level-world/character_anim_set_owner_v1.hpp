#pragma once

#include "animation_tables.hpp"
#include "skill_tables.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace dh2::character_faery_visual_facade_v1 {
struct CharacterVisual;
}

namespace dh2::character_anim_set_owner_v1 {

using Identity = std::uintptr_t;

enum class RegistrationKind : std::uint8_t { animation, template_animation };
enum class Phase : std::uint8_t { empty, registering, ready, failed };
enum class Result : std::uint8_t {
    registered,
    already_registered,
    invalid_identity,
    identity_mismatch,
    invalid_animation_table,
    invalid_skill_list,
    malformed_source,
    effect_failed,
};

// One source AnimSetManager AddAnim/AddTemplateAnim entry. The source sequence
// identifies the CharAnimTable state/list entry being registered; the table
// sequence identifies the actual AnimTable row after Redir traversal.
struct Registration {
    RegistrationKind kind{RegistrationKind::animation};
    std::int32_t source_sequence_id{-1};
    std::int32_t animation_table_sequence_id{-1};
    std::int32_t animation_dictionary_id{-1};
    std::int32_t stance{};
    std::uint32_t flags{};
    std::uint32_t stance_mask{};
};

// This is the per-Character AnimSetManager projection stored on that same
// CharacterVisual/Character Record. It owns registration facts only; source
// tables and renderer/audio/FX resources remain borrowed from their owners.
struct AnimSetState {
    Identity character{};
    std::int32_t anim_set_id{-1};
    std::int32_t animation_table_id{-1};
    std::int32_t skill_list_id{-1};
    Phase phase{Phase::empty};
    std::vector<Registration> registrations;
    std::vector<std::int32_t> animation_dictionary_ids;

    std::vector<std::int32_t> animation_dictionary_ids_for_sequence(
        std::int32_t source_sequence_id) const;
};

// Source sound/FX managers are singletons and are optional. A missing callback
// means that source singleton is unavailable; a present callback failure stops
// registration at that exact source side-effect point without rolling back the
// already-applied registration/effect prefix.
struct Effects {
    void* context{};
    bool (*load_sound)(void*, std::int32_t sound_id, std::string& error){};
    bool (*register_fx)(void*, std::int32_t fx_id, std::string& error){};
};

// Reconstructs Character::SetAnimationSet for the non-player Faery path.
// Source constants are COUNT_IPHONE=5 and SL__LIST_IPHONE=210; non-player
// Characters use one stance. A negative/out-of-range Character SkillList
// selector resolves to source fallback row 3.
Result register_faery_anim_set(
    character_faery_visual_facade_v1::CharacterVisual& visual,
    Identity expected_character,
    const data::AnimationTables& animation_tables,
    const data::SkillTables& skill_tables,
    std::int32_t animation_table_id,
    std::int32_t source_skill_list_id,
    const Effects& effects,
    std::string& error);

// Clear only the AnimSet projection attached to the matching CharacterVisual.
bool retire(character_faery_visual_facade_v1::CharacterVisual& visual,
            Identity expected_character) noexcept;

} // namespace dh2::character_anim_set_owner_v1
