#pragma once

#include "character_state_page_v1.hpp"
#include "../generic_skills/runtime_skill_animation_bank_v1.hpp"
#include "../../combat_session.hpp"

namespace dh::foundation::faery_menu {

// The modern Session host supplies the same-actor check and the three source
// ChangeFaery continuations. The page never writes current_faery itself.
struct SessionFaeryActivationV1 {
    std::shared_ptr<void> owner;
    CharacterState* character{};
    CombatSession* session{};
    dh2::data::FaeryTables::Borrow tables;
    const generic_skills::RuntimeSkillAnimationBankV1* animation_bank{};
    std::int32_t difficulty{-1};
    // Resolved from this actor's actual CharacterTable FaeryList field.
    std::int32_t profile_faery_list_id{-1};
    std::function<bool(CombatSession&, const CharacterState&, std::string&)> validate_same_session;
    std::function<bool(CombatSession&, CharacterState&, std::uint32_t, std::string&)> change_current;
    std::function<bool(CombatSession&, CharacterState&, std::string&)> update_all_skills;
    std::function<bool(CombatSession&, CharacterState&, std::string&)> place_selected_visual;
};

// Builds a Faery tab provider over the same CharacterState, Session, source
// FaeryList and preloaded Cast bank. Missing UpdateAllSkills or visual
// placement continuation rejects at bind time; clicks are never handled by
// the page's fixture-only direct-cell path.
bool bind_session_faery_page_provider_v1(
    CharacterStateFaeryBindingsV1,
    SessionFaeryActivationV1,
    dh::foundation::character_menu::SourcePageProviderV1&,
    std::string& error);

} // namespace dh::foundation::faery_menu
