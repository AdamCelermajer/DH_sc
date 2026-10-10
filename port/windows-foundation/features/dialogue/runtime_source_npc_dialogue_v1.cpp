#include "runtime_source_npc_dialogue_v1.hpp"

#include <cstring>

namespace dh::foundation::dialogue {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

std::int32_t signed_word(std::uint32_t word) {
    std::int32_t value{};
    std::memcpy(&value, &word, sizeof value);
    return value;
}

bool contains_objective(const dh2::data::QuestDefinitionV51& quest,
                        const dh2::data::QuestObjectiveDefinitionV51* objective) {
    for (const auto& row : quest.objectives)
        if (&row == objective) return true;
    return false;
}
} // namespace

bool project_source_npc_dialogue_v1(
    const CharacterState& character,
    const CharacterQuestProgressV1& progress,
    const dh2::data::QuestTablesPersistenceV51& tables,
    const SourceNpcDialogueRouteV1& route,
    const CharacterQuestTextV1& quest_text,
    dh2::ui::HudTextV1& text,
    const dh2::ui::HudTextEnvironmentV1& environment,
    SourceNpcDialogueResponseV1& output,
    std::string& error) {
    output = {};
    if (!tables.ready() || route.collection > 1 || route.difficulty < 0 ||
        route.difficulty >= 3 || route.quest_row < 0 ||
        static_cast<std::size_t>(route.quest_row) >= tables.rows().size() ||
        route.quest != &tables.rows()[static_cast<std::size_t>(route.quest_row)] ||
        !route.talk_objective || !contains_objective(*route.quest, route.talk_objective))
        return fail(error, "NPC dialogue requires exact retained source Quest/objective rows");

    if (!environment.localization.constant || !route.dialogue_symbol ||
        !*route.dialogue_symbol)
        return fail(error, "NPC dialogue requires source StrID and v2QuestObjectiveType providers");
    std::uint32_t talk_type_word{};
    if (!environment.localization.constant(environment.localization.context,
            "v2QuestObjectiveType", "TalkToNPC", talk_type_word, error)) {
        if (error.empty()) error = "Actual source TalkToNPC objective constant lookup failed";
        return false;
    }
    const auto talk_type = signed_word(talk_type_word);
    if (route.talk_objective->type != talk_type || route.npc_identity < 0 ||
        route.talk_objective->oid1 != route.npc_identity)
        return fail(error, "NPC dialogue route does not match its actual TalkToNPC target");

    CharacterQuestStateV1 state;
    const CharacterQuestIdV1 quest_id{route.collection, route.difficulty,
                                      route.quest_row};
    if (!progress.query(character, quest_id, state, error)) return false;
    // The source Quest Log uses this same assigned interval. This projection
    // is only for an active objective response; it does not choose new state.
    if (state.state < 6 || state.state > 12)
        return fail(error, "NPC dialogue response requires the same active source Quest");

    std::uint32_t dialogue_id_word{};
    if (!environment.localization.constant(environment.localization.context,
            "StrID", route.dialogue_symbol, dialogue_id_word, error)) {
        if (error.empty()) error = "Actual source dialogue StrID lookup failed";
        return false;
    }
    const auto dialogue_id = signed_word(dialogue_id_word);
    if (dialogue_id < 0 || dialogue_id != route.dialogue_string_id)
        return fail(error, "NPC dialogue StringID differs from its actual source symbol row");

    SourceQuestPageTextV1 page;
    if (!resolve_source_quest_page_text_v1(*route.quest, character, quest_text,
                                           page, error))
        return false;
    dh2::ui::LocalizationResult localized;
    if (!text.native_string(route.dialogue_symbol, environment.localization,
                            localized, error))
        return false;
    if (!localized.found || localized.text.empty() || localized.text == "notfound" ||
        localized.text == "#!SNL!#" || localized.text == "#!WTF!#")
        return fail(error, "Actual source NPC dialogue line is unavailable in this language pack");

    SourceNpcDialogueResponseV1 staged;
    staged.quest_id = quest_id;
    staged.character_id = character.id;
    staged.quest_name = route.quest->name;
    staged.quest_state = state.state;
    staged.npc_identity = route.npc_identity;
    staged.dialogue_string_id = dialogue_id;
    staged.quest_title = std::move(page.title);
    staged.quest_objective = std::move(page.objective_description);
    staged.dialogue_text = std::move(localized.text);
    output = std::move(staged);
    error.clear();
    return true;
}

} // namespace dh::foundation::dialogue
