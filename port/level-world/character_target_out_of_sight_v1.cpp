#include "character_target_out_of_sight_v1.hpp"

#include <cstring>

namespace dh2::character_target_out_of_sight_v1 {
namespace {

Status invoke(State* state, const Services& services, Result& result,
             Operation operation, std::uintptr_t subject,
             std::uintptr_t argument, Response& response) {
    if (!services.invoke) return Status::service_unavailable;
    const Request request{operation, 0, subject, argument};
    response = {};
    ++result.provider_calls;
    try {
        return services.invoke(services.context, state, &request, &response) == 0
            ? Status::complete : Status::service_failed;
    } catch (...) {
        return Status::service_failed;
    }
}

bool overlap(const void* a, std::size_t an, const void* b, std::size_t bn) {
    const auto x = reinterpret_cast<std::uintptr_t>(a);
    const auto y = reinterpret_cast<std::uintptr_t>(b);
    return x <= y ? y - x < an : x - y < bn;
}

} // namespace

Status run(State* state, const Services* services, Result* output) {
    if (!state || !services || !output || !state->ai || state->reserved ||
        !services->invoke || overlap(state, sizeof(*state), services, sizeof(*services)) ||
        overlap(state, sizeof(*state), output, sizeof(*output)) ||
        overlap(services, sizeof(*services), output, sizeof(*output)))
        return Status::invalid_argument;

    const Services bound = *services;
    *output = {};
    Response response{};
    auto status = invoke(state, bound, *output,
        Operation::capture_debug_switch_owner, 0, 0, response);
    if (status != Status::complete) return status;
    const auto debug_owner = response.identity;

    status = invoke(state, bound, *output, Operation::query_debug_switch,
        debug_owner, 0, response);
    if (status != Status::complete) return status;
    ++output->debug_switch_queries;

    status = invoke(state, bound, *output, Operation::is_target_a_character,
        state->ai, 0, response);
    if (status != Status::complete) return status;
    output->target_character_word = response.word;
    if (response.word) {
        status = invoke(state, bound, *output, Operation::get_target_as_character,
            state->ai, 0, response);
        if (status != Status::complete) return status;
        output->target_character = response.identity;
        status = invoke(state, bound, *output, Operation::clear_aggro,
            state->ai, response.identity, response);
        if (status != Status::complete) return status;
    }

    // This is CharacterAI state, not a new target/visibility store. Source
    // clears CharAI+0x4c between AI_ClearAggro and the current AIS callback.
    state->out_of_sight_4c = 0;
    output->out_of_sight_cleared = 1;

    const auto active_ais = state->active_ais; // source load immediately before vcall
    if (active_ais) {
        status = invoke(state, bound, *output,
            Operation::ais_on_target_out_of_sight, active_ais, 0, response);
        if (status != Status::complete) return status;
        output->ais_callback_target = active_ais;
    }

    status = invoke(state, bound, *output,
        Operation::recapture_debug_switch_owner, 0, 0, response);
    if (status != Status::complete) return status;
    output->debug_owner_changed = response.identity != debug_owner;
    if (output->debug_owner_changed) return Status::source_invariant_failed;
    return Status::complete;
}

} // namespace dh2::character_target_out_of_sight_v1
