#pragma once

#include "../quests/character_quest_progress_v1.hpp"
#include "../quests/quest_text_resolver_v1.hpp"
#include "../../../engine-ui/hud_text_v1.hpp"
#include "../../../game-data/quest_persistence_v51.hpp"

namespace dh::foundation::dialogue {

// A caller-owned join of one real source TalkToNPC objective row and one
// localized source dialogue string row. `dialogue_symbol` is resolved through
// the existing StringManager constants/cache; no fallback text is synthesized.
struct SourceNpcDialogueRouteV1 {
    std::uint32_t collection{};
    std::int32_t difficulty{}, quest_row{};
    const dh2::data::QuestDefinitionV51* quest{};
    const dh2::data::QuestObjectiveDefinitionV51* talk_objective{};
    std::int32_t npc_identity{};
    std::int32_t dialogue_string_id{};
    const char* dialogue_symbol{};
};

struct SourceNpcDialogueResponseV1 {
    CharacterQuestIdV1 quest_id;
    std::string character_id;
    std::string quest_name;
    std::int32_t quest_state{};
    std::int32_t npc_identity{};
    std::int32_t dialogue_string_id{};
    std::optional<std::string> quest_title;
    std::optional<std::string> quest_objective;
    std::string dialogue_text;
};

// Read-only projection for an accepted TalkToNPC interaction. The route must
// point into the same decoded original Quest table; progress and localized
// output must belong to the exact CharacterState passed here. It does not
// mutate quest progress, raise events, start a cutscene, or create a dialogue.
bool project_source_npc_dialogue_v1(
    const CharacterState&,
    const CharacterQuestProgressV1&,
    const dh2::data::QuestTablesPersistenceV51&,
    const SourceNpcDialogueRouteV1&,
    const CharacterQuestTextV1&,
    dh2::ui::HudTextV1&,
    const dh2::ui::HudTextEnvironmentV1&,
    SourceNpcDialogueResponseV1&,
    std::string& error);

} // namespace dh::foundation::dialogue
