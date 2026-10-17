#include "character_ai_update_all_skills.hpp"

#include <cstddef>
#include <limits>

namespace dh2::character_ai_update_all_skills {
namespace {
struct Range { std::uintptr_t first, end; };

bool range(const void* pointer, std::size_t size, std::size_t alignment, Range& out) {
    const auto first = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || first % alignment ||
        first > std::numeric_limits<std::uintptr_t>::max() - size) return false;
    out = {first, first + size};
    return true;
}

bool overlap(Range a, Range b) { return a.first < b.end && b.first < a.end; }

Status vector_count(const ScriptVector& vector, std::uint32_t& out) {
    const auto first = reinterpret_cast<std::uintptr_t>(vector.begin);
    const auto last = reinterpret_cast<std::uintptr_t>(vector.end);
    if (!first && !last) { out = 0; return Status::complete; }
    if (!first || !last || first % alignof(std::uintptr_t) ||
        last % alignof(std::uintptr_t) || last < first) return Status::invalid_source_fact;
    const auto bytes = last - first;
    if (bytes % sizeof(std::uintptr_t)) return Status::invalid_source_fact;
    const auto count = bytes / sizeof(std::uintptr_t);
    // A source vector this large cannot be a valid game actor list. This is an
    // adapter memory bound; ordinary source list counts are unaffected.
    if (count > 1'000'000u) return Status::invalid_source_fact;
    out = static_cast<std::uint32_t>(count);
    return Status::complete;
}

Status invoke(State* state, const Services& services, Result& result,
              Operation operation, List list, std::uintptr_t subject,
              std::uint32_t index, std::uint32_t& word) {
    if (!services.invoke) return Status::service_unavailable;
    const Request request{operation, list, subject, index};
    Response response{};
    ++result.service_calls;
    result.last_operation = static_cast<std::uint32_t>(operation);
    try {
        if (services.invoke(services.context, state, &request, &response))
            return Status::service_failed;
    } catch (...) {
        return Status::service_failed;
    }
    word = response.word;
    return Status::complete;
}

Status update_list(State* state, const Services& services, Result& result,
                   List which, std::uint32_t count) {
    for (std::uint32_t index = 0; index < count; ++index) {
        // The ARM loop reloads CharAI+0xb4/+0xc0 at every iteration. The
        // phase count remains the value captured before this loop.
        const ScriptVector current = which == List::skill ? state->skills : state->faeries;
        std::uint32_t current_count = 0;
        const auto measured = vector_count(current, current_count);
        if (measured != Status::complete || index >= current_count)
            return Status::invalid_source_fact;
        const auto script = current.begin[index];
        if (!script) continue;
        std::uint32_t ignored = 0;
        const auto status = invoke(state, services, result, Operation::on_skill_update,
                                   which, script, index, ignored);
        if (status != Status::complete) return status;
        ++result.script_updates;
    }
    return Status::complete;
}
} // namespace

Status update(State* state, const Services* services, Result* result) {
    Range controls[3];
    if (!range(state, sizeof(*state), alignof(State), controls[0]) ||
        !range(services, sizeof(*services), alignof(Services), controls[1]) ||
        !range(result, sizeof(*result), alignof(Result), controls[2]) ||
        overlap(controls[0], controls[1]) || overlap(controls[0], controls[2]) ||
        overlap(controls[1], controls[2]) || !state->ai || !state->owner)
        return Status::invalid_argument;

    const auto ai = state->ai; // source r5 remains the original CharAI throughout.
    const Services bound = *services;
    *result = {};
    (void)ai;

    std::uint32_t word = 0;
    auto status = invoke(state, bound, *result, Operation::is_using_skill,
                         List::none, state->owner, 0, word);
    if (status != Status::complete) return status;
    result->using_skill_word = word;
    if (word) {
        result->decision = Decision::skipped_while_using_skill;
        return Status::complete;
    }

    // The source reloads CharAI+4 between its two predicates. A synchronous
    // provider may therefore replace State::owner before this second request.
    if (!state->owner) return Status::invalid_source_fact;
    word = 0;
    status = invoke(state, bound, *result, Operation::is_casting,
                    List::none, state->owner, 0, word);
    if (status != Status::complete) return status;
    result->casting_word = word;
    if (word) {
        result->decision = Decision::skipped_while_casting;
        return Status::complete;
    }

    status = vector_count(state->skills, result->skill_slots);
    if (status != Status::complete) return status;
    status = update_list(state, bound, *result, List::skill, result->skill_slots);
    if (status != Status::complete) return status;

    // Faery vector boundaries are fetched only after all skill callbacks.
    status = vector_count(state->faeries, result->faery_slots);
    if (status != Status::complete) return status;
    status = update_list(state, bound, *result, List::faery, result->faery_slots);
    if (status != Status::complete) return status;
    result->decision = Decision::completed;
    return Status::complete;
}

} // namespace dh2::character_ai_update_all_skills
