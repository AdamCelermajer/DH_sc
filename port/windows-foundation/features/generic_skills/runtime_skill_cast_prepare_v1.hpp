#pragma once

#include "runtime_skill_mana_v1.hpp"
#include "runtime_skill_activation_v1.hpp"
#include "runtime_skill_progression_v1.hpp"
#include "../../combat_session.hpp"
#include "../../original_actor_properties.hpp"

#include <optional>
#include <string>

namespace dh::foundation::generic_skills {

// Snapshot values returned by the original synchronous Application/Player/
// DebugSwitches providers. Missing values stay unknown; callers must not infer
// player, god-mode, or debug policy from a CharacterState or actor faction.
struct SkillManaSourceFactsV1 {
    std::optional<bool> application_byte5;
    std::optional<bool> is_player;
    std::optional<bool> god_mana_registered;
    std::optional<bool> god_mana_enabled;
    std::optional<bool> character_byte14f0;
    // This is reached after the source property41 debit. An unavailable query
    // therefore returns a failed status with the committed prefix preserved.
    std::optional<bool> tracing_character_stats;
};

enum class SkillManaPrepareStatusV1 : unsigned char {
    unavailable,
    insufficient_mana,
    mana_bypass,
    mana_committed,
    provider_failure_after_mana_commit
};

struct SkillManaPrepareResultV1 {
    SkillManaPrepareStatusV1 status = SkillManaPrepareStatusV1::unavailable;
    SkillVisualRequestV1 skill;
    SkillManaCostV1 cost;
    std::int32_t mana_before = 0;
    std::int32_t mana_after = 0;
    bool mana_spent = false;
    // Source-equivalent BashDown OnSkillCheck_ HasMana result computed before
    // its later Pre UseMana prefix; disengaged means it was not reached.
    std::optional<bool> source_has_mana_check;
};

// The authored Character Skills mapping circles are laid out left-to-right as
// source slots [2, 0, 1] (`btn_activeskill03`, `01`, `02`). The Windows
// keyboard adaptation uses visible one-based numbers, so key 2 addresses the
// middle circle/source slot 0. This never rewrites saved slot identity.
bool pc_skill_number_to_source_slot_v1(unsigned key_number,
                                       std::uint32_t& source_slot,
                                       std::string& error);

// Source NativeHUDSkill uses zero-based saved hotbar slots 0..2. The current
// equipment set is explicit because CharacterState stores assignment per set.
bool resolve_assigned_skill_position_v1(
    const CharacterState&, dh2::data::SkillTables::Borrow,
    std::uint32_t equipment_set, std::uint32_t source_slot,
    int& class_skill_position, std::uint32_t& saved_skill_row,
    std::string& error);

// Resolve one exact source SkillList position and execute the authored
// HasMana/UseMana property41 prefix over the existing session actor's property
// state. It does not search targets or execute SkillCombatRoll, so a committed
// mana result is preparation, never a completed cast receipt.
bool prepare_skill_cast_mana_v1(
    CharacterState&, CombatSession&, ActorId,
    const dh2::data::CharacterTable&, dh2::data::SkillTables::Borrow,
    const dh2::data::ClassTables&, const dh2::data::PropertyRules&,
    int class_skill_position, const std::string& authored_class_table_token,
    const SkillManaSourceFactsV1&, SkillManaPrepareResultV1&,
    std::string& error);

} // namespace dh::foundation::generic_skills
