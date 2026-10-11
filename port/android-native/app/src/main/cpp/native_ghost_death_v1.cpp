#include "native_ghost_death_v1.hpp"

#include "../../../../../../port/level-world/character_stance.hpp"

#include <cstring>

namespace dh2::native::ghost_death {
namespace {

bool get_constant(const dh2_pycst_view* constants, const char* group,
                  const char* key, std::int32_t& value) {
    if (!constants || !group || !key) return false;
    dh2_pycst_result result{};
    if (dh2_pycst_get(constants, group,
                      static_cast<std::uint32_t>(std::strlen(group)), key,
                      static_cast<std::uint32_t>(std::strlen(key)), &result) ||
        !result.found)
        return false;
    value = result.value;
    return true;
}

int fail(std::string& error, const char* message) {
    if (error.empty()) error = message;
    return 1;
}

} // namespace

int invoke(void* raw, const player_ai_death_v1::Request* request,
           player_ai_death_v1::Reply* reply, std::string& error) {
    if (!raw || !request || !reply)
        return fail(error, "Ghost death provider received invalid arguments");
    const auto& bindings = *static_cast<const Bindings*>(raw);
    if (!bindings.ai || !bindings.character || request->ai != bindings.ai ||
        request->character != bindings.character)
        return fail(error, "Ghost death provider identity differs from retained owners");

    using player_ai_death_v1::Operation;
    switch (request->operation) {
    case Operation::anim_stance: {
        if (!bindings.classification || !bindings.classification_services ||
            bindings.classification->character != bindings.character)
            return fail(error, "Ghost death Character classification is unavailable");
        character_ai_classification::Result classification{};
        if (character_ai_classification::query(
                character_ai_classification::Query::monster,
                bindings.classification, bindings.classification_services,
                &classification) != character_ai_classification::Status::complete ||
            classification.word != 1)
            return fail(error, "Ghost death stance requires the source Monster branch");
        std::int32_t count = 0;
        if (!get_constant(bindings.constants, "AnimStances", "COUNT_IPHONE", count))
            return fail(error, "Ghost death stance count is unavailable");
        character::StanceFacts16 facts{};
        facts.count = count;
        std::int32_t stance = 0;
        if (dh2_character_anim_stance(&stance, &facts) != 1)
            return fail(error, "Ghost death Monster stance lookup failed");
        reply->word = static_cast<std::uint32_t>(stance);
        return 0;
    }
    case Operation::skill_cleanup:
    case Operation::spell_cleanup: {
        if (!bindings.skills)
            return fail(error, "Ghost death skill owner is unavailable");
        const auto list = request->operation == Operation::skill_cleanup
            ? character_ai_set_skills_and_spells::List::skill
            : character_ai_set_skills_and_spells::List::faery;
        ghost_skills::DeathCleanupResult result{};
        const auto status = bindings.skills->cleanup_death_list(list, result);
        if (status != ghost_skills::Status::complete || !result.completed)
            return fail(error, "Ghost death reached an unsupported skill cleanup entry");
        reply->count = result.slots_examined;
        return 0;
    }
    default:
        return fail(error, "Ghost death provider does not own this operation");
    }
}

} // namespace dh2::native::ghost_death
