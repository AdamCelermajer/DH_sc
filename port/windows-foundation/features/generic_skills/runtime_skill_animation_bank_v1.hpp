#pragma once

#include "runtime_skill_activation_v1.hpp"
#include "runtime_skill_cast_prepare_v1.hpp"
#include "../skills_animation/skill_animation_program.hpp"

#include <array>
#include <optional>

namespace dh::foundation::generic_skills {

struct RuntimeSkillAnimationSlotV1 {
    std::uint32_t equipment_set = 0;
    std::uint32_t source_slot = 0;
    SkillVisualRequestV1 skill;
    std::string selection_state;
};

// Optional pre-init entry for a position in the same active source SkillList.
// This is animation-bank metadata only: it does not grant rank, assignment,
// cast usability, or training eligibility.
struct RuntimeSkillAnimationClassRootV1 {
    int active_skill_list_id = -1;
    int class_skill_position = -1;
    int skill_table_id = -1;
    std::string source_skill_name;
    std::string source_script;
    std::int32_t animation_sequence_id = -1;
    std::string selection_state;
    bool loaded = false;
    std::string diagnostic;
};

struct RuntimeSkillFaeryAnimationSlotV1 {
    std::int32_t source_animation_table_id = -1;
    std::int32_t faery_slot = -1;
    std::int32_t animation_sequence_id = -1;
    std::string selection_state;
    bool loaded = false;
    std::string diagnostic;
};

struct RuntimeSkillAnimationBankRequestV1 {
    const CharacterState* character = nullptr;
    const dh2::data::CharacterTable* characters = nullptr;
    dh2::data::SkillTables::Borrow skills;
    const AssetCatalog* assets = nullptr;
    const dh2::data::AnimationTables* animations = nullptr;
    const dh2::data::Dictionary* animation_dictionary = nullptr;
    const CharacterVisualConfig* same_actor_visual = nullptr;
    std::string actor_role;
    std::uint32_t equipment_set = 0;
    // If true, preload every exact nonnegative SkillTable.Anim root in the
    // active source SkillList, regardless of saved rank or hotbar assignment.
    bool preload_current_class_roots = false;

    // The property2-resolved CharAnimTable id from this same actor's original
    // property owner. When present, the helper reads each authored
    // CharAnimTable.Spells[0..4] root and compiles loadable Cast sequences.
    // The current saved Faery slot is resolved separately on every cast.
    std::optional<std::int32_t> source_animation_table_id;
};

struct RuntimeSkillAnimationBankV1 {
    skills_animation::SkillAnimationPrograms programs;
    std::array<std::optional<RuntimeSkillAnimationSlotV1>, 3> hotbar;
    std::string character_state_id;
    std::string class_id;
    int active_skill_list_id = -1;
    bool class_roots_preloaded = false;
    std::vector<RuntimeSkillAnimationClassRootV1> class_skill_roots;
    std::array<RuntimeSkillFaeryAnimationSlotV1, 5> faery_cast_slots;
    std::optional<std::int32_t> source_animation_table_id;
    // Compatibility/diagnostic view of the exact authored slot4 root only.
    std::optional<std::int32_t> active_faery_cast_sequence;
    std::string active_faery_cast_selection_state;
};

// Builds the exact pre-initialization clip bank for assigned NativeHUDSkill
// slots 0..2 and, when requested, the actor's authored CharAnimTable
// all five CharAnimTable.Spells Cast clips. All roots and clip URIs come from the supplied original
// tables; output is atomic on missing/stale source data.
bool build_runtime_skill_animation_bank_v1(
    const RuntimeSkillAnimationBankRequestV1&,
    const OriginalCombatVisualPlan& base_plan,
    RuntimeSkillAnimationBankV1&, std::string& error);

// Merge the bank's exact clip/config and source sequence policy metadata into
// the existing visual plan before CharacterVisual initialization.
bool merge_runtime_skill_animation_bank_v1(
    const RuntimeSkillAnimationBankV1&,
    OriginalCombatVisualPlan& visual_plan,
    OriginalSequencePolicies& sequence_policies,
    std::string& error);

// Resolve the current saved assignment on every use, then select its already
// loaded class-list root. This intentionally does not read the possibly stale
// hotbar snapshot built before a menu assignment/remap.
bool resolve_runtime_skill_animation_slot_v1(
    const CharacterState&, const dh2::data::CharacterTable&,
    dh2::data::SkillTables::Borrow, std::uint32_t equipment_set,
    std::uint32_t source_slot, const RuntimeSkillAnimationBankV1&,
    RuntimeSkillAnimationSlotV1&, std::string& error);

// Resolve the current difficulty's saved Faery slot fresh against the
// preloaded source CharAnimTable spell bank. This intentionally does not
// interpret saved unlock state as a cast permission.
bool resolve_runtime_faery_animation_slot_v1(
    const CharacterState&, std::int32_t difficulty,
    const RuntimeSkillAnimationBankV1&,
    RuntimeSkillFaeryAnimationSlotV1&, std::string& error);

} // namespace dh::foundation::generic_skills
