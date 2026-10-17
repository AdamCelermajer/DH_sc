#include "character_ai_update_skills.hpp"

#include "character_skill_state_queries.hpp"

#include <limits>
#include <vector>

namespace dh2::character_ai_update_skills {
namespace {
struct Range { std::uintptr_t begin, end; };

template<class T>
bool range(const T* pointer, Range& result) {
    const auto begin = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || begin % alignof(T) ||
        begin > std::numeric_limits<std::uintptr_t>::max() - sizeof(T))
        return false;
    result = {begin, begin + sizeof(T)};
    return true;
}

bool overlap(Range a, Range b) {
    return a.begin < b.end && b.begin < a.end;
}

Status vector_count(const ScriptVector& vector, std::uint32_t& count) {
    const auto begin = reinterpret_cast<std::uintptr_t>(vector.begin);
    const auto end = reinterpret_cast<std::uintptr_t>(vector.end);
    if (!begin && !end) {
        count = 0;
        return Status::complete;
    }
    if (!begin || !end || begin % alignof(std::uintptr_t) ||
        end % alignof(std::uintptr_t) || end < begin ||
        (end - begin) % sizeof(std::uintptr_t))
        return Status::invalid_source_fact;
    const auto entries = (end - begin) / sizeof(std::uintptr_t);
    if (entries > UINT32_MAX) return Status::invalid_source_fact;
    count = static_cast<std::uint32_t>(entries);
    return Status::complete;
}

Status get_vector(State* state, const Services& services, Result& result,
                  List list, ScriptVector& vector, std::uint32_t& count) {
    if (!services.script_vector) return Status::service_unavailable;
    ++result.service_calls;
    try {
        if (services.script_vector(services.context, state, list, &vector))
            return Status::service_failed;
    } catch (...) {
        return Status::service_failed;
    }
    return vector_count(vector, count);
}

Status update_script(State* state, const Services& services, Result& result,
                     List list, std::uint32_t index, std::uintptr_t script) {
    if (!script) return Status::complete;
    if (!services.on_skill_update) return Status::service_unavailable;
    ++result.service_calls;
    try {
        if (services.on_skill_update(services.context, state, list, index,
                                     script))
            return Status::service_failed;
    } catch (...) {
        return Status::service_failed;
    }
    ++result.script_updates;
    if (list == List::faery) result.faery_updated = 1;
    return Status::complete;
}
} // namespace

Status update(State* state, const Services* services, Result* output) {
    Range controls[3]{};
    if (!range(state, controls[0]) || !range(services, controls[1]) ||
        !range(output, controls[2]) || overlap(controls[0], controls[1]) ||
        overlap(controls[0], controls[2]) || overlap(controls[1], controls[2]) ||
        !state->ai || !state->owner || !state->current_state ||
        !state->savegame || !state->current_difficulty ||
        state->savegame->character() != state->owner)
        return Status::invalid_argument;

    *output = {};
    const auto bound = *services;
    character_skill_state_queries::Machine machine{state->current_state};
    character_skill_state_queries::Result predicate{};
    if (character_skill_state_queries::is_using_skill(&machine, &predicate) !=
        character_skill_state_queries::Status::complete)
        return Status::invalid_source_fact;
    output->using_state_word = predicate.state_word;
    if (predicate.value) {
        output->decision = Decision::skipped_while_using_skill;
        return Status::complete;
    }

    // Query the same live state word again; source calls SM_IsCasting only
    // after the UsingSkill result is false.
    if (!state->current_state) return Status::invalid_source_fact;
    if (character_skill_state_queries::is_casting(&machine, &predicate) !=
        character_skill_state_queries::Status::complete)
        return Status::invalid_source_fact;
    output->casting_state_word = predicate.state_word;
    if (predicate.value) {
        output->decision = Decision::skipped_while_casting;
        return Status::complete;
    }

    // ItemInventory::GetCurrentSkillSet(int) is a constant-zero source body
    // (IDA 0x3fc6a0). SG_TellSlots iterates this actual Save map in sorted key
    // order; the callback indexes CharAI's vector by the key and ignores the
    // mapped skill row. Copy only keys so a callback cannot invalidate a native
    // iterator; a map mutation stops the adapter after preserving effects.
    const auto& saved_slots = state->savegame->skill_slots()[0];
    std::vector<std::int32_t> keys;
    keys.reserve(saved_slots.size());
    for (const auto& slot : saved_slots) keys.push_back(slot.first);
    for (const auto slot : keys) {
        ++output->saved_slots;
        ScriptVector vector{};
        std::uint32_t count = 0;
        auto status = get_vector(state, bound, *output, List::skill,
                                 vector, count);
        if (status != Status::complete) return status;
        const auto index = static_cast<std::uint32_t>(slot);
        if (index < count) {
            status = update_script(state, bound, *output, List::skill, index,
                                   vector.begin[index]);
            if (status != Status::complete) return status;
        }
        const auto& current_slots = state->savegame->skill_slots()[0];
        if (current_slots.size() != keys.size()) return Status::invalid_source_fact;
        auto current = current_slots.begin();
        for (const auto key : keys) {
            if (current == current_slots.end() || current->first != key)
                return Status::invalid_source_fact;
            ++current;
        }
    }

    // Character::SG_GetCurrentFaerieId(-1) reads the static difficulty and
    // this Save's current-faery array after all skill callbacks.
    const auto difficulty = *state->current_difficulty;
    if (difficulty < 0 || difficulty >= 3) return Status::invalid_source_fact;
    const auto faery = state->savegame->current_faery(
        static_cast<std::uint32_t>(difficulty));
    output->faery_index = static_cast<std::uint32_t>(faery);
    ScriptVector faeries{};
    std::uint32_t faery_count = 0;
    auto status = get_vector(state, bound, *output, List::faery,
                             faeries, faery_count);
    if (status != Status::complete) return status;
    if (output->faery_index < faery_count) {
        status = update_script(state, bound, *output, List::faery,
                               output->faery_index,
                               faeries.begin[output->faery_index]);
        if (status != Status::complete) return status;
    }
    output->decision = Decision::updated;
    return Status::complete;
}

} // namespace dh2::character_ai_update_skills
