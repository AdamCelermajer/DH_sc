#pragma once

#include "character_quest_progress_v1.hpp"
#include "../../../engine-ui/hud_text_v1.hpp"

namespace dh::foundation {

struct SourceQuestPageTextV1 {
    std::optional<std::string> title;
    std::optional<std::string> pre_description;
    std::optional<std::string> objective_description;
    std::optional<std::string> post_description;
    bool primary{};   // Quest::IsPrimary (MAIN QUEST tag when true, SIDE QUEST otherwise)
};

// Borrows the already-loaded source StringManager cache/environment. It does
// not load translations or create an English fallback table.
bool bind_source_quest_text_resolver_v1(
    dh2::ui::HudTextV1&, const dh2::ui::HudTextEnvironmentV1&,
    CharacterQuestTextV1&, std::string& error);

// Exact Quest::GetTitle/GetPreDescription/GetObjectiveDescription/
// GetPostDescription source behavior over the original definition row.
// Missing resolver leaves only the fields needing a StringID unset.
bool resolve_source_quest_page_text_v1(
    const dh2::data::QuestDefinitionV51&, const CharacterState&,
    const CharacterQuestTextV1&, SourceQuestPageTextV1&, std::string& error);

} // namespace dh::foundation
