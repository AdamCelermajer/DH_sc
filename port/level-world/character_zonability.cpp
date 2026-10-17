#include "character_zonability.hpp"

#include <cstddef>

namespace dh2::character_zonability { namespace {
struct Range { std::uintptr_t begin, end; };

bool range(const void* pointer, std::size_t size, std::size_t alignment, Range& out) {
    const auto begin = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || begin % alignment || begin > UINTPTR_MAX - size) return false;
    out = {begin, begin + size};
    return true;
}

bool overlap(Range a, Range b) { return a.begin < b.end && b.begin < a.end; }

Status query(State* state, const Services& services, Result& result,
             Operation operation, std::uintptr_t captured, std::uint32_t& word) {
    if (!services.invoke) return Status::service_unavailable;
    const Request request{operation, 0, captured};
    Response response{};
    ++result.service_calls;
    try {
        if (services.invoke(services.context, state, &request, &response))
            return Status::service_failed;
    } catch (...) {
        return Status::service_failed;
    }
    word = response.word;
    return Status::complete;
}
}  // namespace

Status evaluate(State* state, const Services* services, Result* result) {
    Range ranges[3];
    if (!range(state, sizeof(*state), alignof(State), ranges[0]) ||
        !range(services, sizeof(*services), alignof(Services), ranges[1]) ||
        !range(result, sizeof(*result), alignof(Result), ranges[2]))
        return Status::invalid_argument;
    for (unsigned i = 0; i < 3; ++i)
        for (unsigned j = 0; j < i; ++j)
            if (overlap(ranges[i], ranges[j])) return Status::invalid_argument;
    if (!state->character) return Status::invalid_argument;

    const auto captured = state->character;
    const Services bound = *services;
    *result = {};
    result->captured_character = captured;

    auto status = query(state, bound, *result, Operation::is_player, captured,
                        result->is_player_word);
    if (status != Status::complete) return status;
    if (result->is_player_word) {
        result->decision = Decision::player;
        return Status::complete;
    }

    status = query(state, bound, *result, Operation::is_faerie, captured,
                   result->is_faerie_word);
    if (status != Status::complete) return status;
    if (result->is_faerie_word) {
        result->decision = Decision::faerie;
        return Status::complete;
    }

    result->decision = Decision::base_condition;
    result->zonable = 1;
    return Status::complete;
}

}  // namespace dh2::character_zonability

extern "C" int dh2_character_zonability_evaluate(
    dh2::character_zonability::State* state,
    const dh2::character_zonability::Services* services,
    dh2::character_zonability::Result* result) {
    return static_cast<int>(dh2::character_zonability::evaluate(state, services, result));
}
