#pragma once

#include "generic_skills_page_v1.hpp"
#include "../character_menu/menu_text.hpp"
#include "runtime_skill_mana_v1.hpp"

#include <memory>
#include <string_view>

namespace dh::foundation::generic_skills {

// Fresh source facts needed to reproduce the original details guards on the
// same live CharacterState. The provider callbacks must borrow the existing
// player/property/action owners; this type owns no player or save state.
struct RuntimeSkillsDetailsBindingsV1 {
    const dh2::data::ClassTables* classes{};
    const dh2::data::PropertyRules* property_rules{};
    std::function<bool(const CharacterState&, dh2::data::PropertySheet&,
                       std::string& error)> current_resolved;
    std::function<bool(const CharacterState&, std::int32_t& max_skill_level,
                       std::string& error)> max_skill_level;
    std::function<bool(const CharacterState&, int class_skill_position,
                       bool& can_increment, std::string& error)> can_increment;
    // Source CharacterMenu query for the current faery's OID offset. Only
    // needed when a selected SkillTable row has its authored faery flag set.
    std::function<bool(const CharacterState&, std::int32_t& oid_offset,
                       std::string& error)> faery_oid_offset;
};

// SetTempProps ClassTable token of a skill script whose recovered OnSkillInfo_
// body was audited (every player prince_* script except prince_warrior_charge
// and prince_paladin_energy_balance, which depend on live HasShield/faery
// state), or nullptr. `rank` is the larger of the two ranks the native details
// query evaluates (saved rank + 1); it only matters for Magi's Shielding.
const char* audited_skill_info_class_v1(std::string_view script, std::uint32_t rank);

// Provides source-backed static fields and, when the native source facts are
// bound, the Current/Next level strings of NativeGetSkillDetails. The static
// guard strings (locked by level, rank 0, max rank, level-limited training) do
// not need a script; the script-dependent values are produced for every
// audited OnSkillInfo SetTempProps body and left blank for the rest.
class RuntimeSkillsTextProviderV1 {
    struct State;
    std::shared_ptr<State> state_;

public:
    // Retains the actual immutable SkillTables borrow and references the same
    // CharacterState/MenuLocalization owners used by the menu. Existing
    // caller-supplied localized_text callbacks are preserved.
    bool bind(const dh2::data::CharacterTable& source_characters,
              dh2::data::SkillTables::Borrow source_skills,
              character_menu::MenuLocalization&,
              const CharacterState& same_character_state,
              ServicesV1&, std::string& error);

    // Adds the pure source Details producer to the same ServicesV1. It
    // evaluates the audited skill CLASS_ID on an isolated default scratch
    // sheet, walks that record's display_props in source order, and sends the
    // exact SkillTable text OIDs/values through this same HudText cache.
    bool bind_details(const RuntimeSkillsDetailsBindingsV1&,
                      ServicesV1&, std::string& error);
};

} // namespace dh::foundation::generic_skills
