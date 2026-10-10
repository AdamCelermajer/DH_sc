#pragma once

#include "generic_skills_page_v1.hpp"
#include "../character_menu/menu_text.hpp"
#include "runtime_skill_mana_v1.hpp"

#include <memory>

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

// Provides source-backed static fields and, when the native source facts are
// bound, current/next details for the two audited OnSkillInfo SetTempProps
// branches. Other skill scripts remain explicitly unavailable.
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
