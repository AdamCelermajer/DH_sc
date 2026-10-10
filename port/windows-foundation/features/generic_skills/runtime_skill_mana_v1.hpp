#pragma once

#include "../../../game-data/class_tables.hpp"
#include "../../../game-data/properties.hpp"

#include <cstdint>
#include <string>

namespace dh::foundation::generic_skills {

struct SkillManaCostV1 {
    std::int32_t class_table_id = -1;
    std::uint32_t rank = 0;
    std::int32_t fixed_skill_level = 0;
    std::int32_t fixed_mana_cost = 0;
};

struct SkillTempPropertiesV1 {
    std::int32_t class_table_id = -1;
    std::uint32_t rank = 0;
    std::int32_t fixed_skill_level = 0;
    dh2::data::PropertySheet properties{};
};

// Source SetTempProps for audited skill OnSkillInfo callbacks: ClearProps(true),
// SetProp(SnS_Level, ToFixed(rank), true), ApplyPropClass(CLASS_ID, true).
// The resulting scratch sheet is isolated from the same-character resolved
// properties, which are borrowed only as the source buff input.
bool evaluate_skill_temp_properties_v1(
    const dh2::data::ClassTables&, const dh2::data::PropertyRules&,
    const dh2::data::PropertySheet& current_resolved,
    const std::string& authored_class_table_token, std::uint32_t saved_rank,
    SkillTempPropertiesV1&, std::string& error);

// Reproduces the authored skills-commons CalcManaCost path over source tables:
// ClearProps(true), SetProp(SnS_Level, ToFixed(rank), true),
// ApplyPropClass(class_id, true), GetProp(SnS_ManaCost, true). The class token
// must come from that skill's authored CLASS_ID declaration; current_resolved
// is borrowed from the same live character property owner. No CharacterState
// resource field or synthetic cost formula is consulted.
bool evaluate_skill_mana_cost_v1(
    const dh2::data::ClassTables&, const dh2::data::PropertyRules&,
    const dh2::data::PropertySheet& current_resolved,
    const std::string& authored_class_table_token, std::uint32_t saved_rank,
    SkillManaCostV1&, std::string& error);

} // namespace dh::foundation::generic_skills
