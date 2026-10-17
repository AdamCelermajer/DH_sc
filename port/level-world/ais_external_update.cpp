#include "ais_external_update.hpp"

#include <cstddef>
#include <cstdint>

namespace dh2::ais_external_update {
namespace {
struct Range { std::uintptr_t begin, end; };

bool make_range(const void* pointer, std::size_t size, std::size_t alignment,
                Range& out) noexcept {
    const auto begin = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || begin % alignment || begin > UINTPTR_MAX - size) return false;
    out = {begin, begin + size};
    return true;
}

bool overlaps(Range a, Range b) noexcept {
    return a.begin < b.end && b.begin < a.end;
}

Status invoke(State& state, const Services& services, Result& result,
              Phase phase, Operation operation, ScriptCallback callback,
              std::uintptr_t owner, std::uint32_t argument) noexcept {
    if (!services.invoke) return Status::service_unavailable;
    result.phase = phase;
    const Request request{operation, callback, state.ais, owner, argument, 0};
    ++result.service_calls;
    try {
        return services.invoke(services.context, &state, &request) == 0 ?
               Status::complete : Status::service_failed;
    } catch (...) {
        return Status::service_failed;
    }
}
}  // namespace

Status update(State* state, const Services* services, Result* result) {
    Range ranges[3];
    if (!make_range(state, sizeof(*state), alignof(State), ranges[0]) ||
        !make_range(services, sizeof(*services), alignof(Services), ranges[1]) ||
        !make_range(result, sizeof(*result), alignof(Result), ranges[2]))
        return Status::invalid_argument;
    for (unsigned i = 0; i < 3; ++i)
        for (unsigned j = 0; j < i; ++j)
            if (overlaps(ranges[i], ranges[j])) return Status::invalid_argument;
    if (!state->ais || !services->invoke) return Status::invalid_argument;

    const Services bound = *services;
    *result = {};
    if (state->counter_bc > 199u) {
        result->default_pause_due = 1;
        // Source writes AIS+0xbc before loading the Character's CharAI and
        // making the synchronous pause call.
        state->counter_bc = 0;
        if (!state->owner) return Status::invalid_source_fact;
        auto status = invoke(*state, bound, *result, Phase::pause_update,
            Operation::pause_character_ai, ScriptCallback::none,
            state->owner, 1000u);
        if (status != Status::complete) return status;

        // AISDefault reloads AIS+0x98 after AI_PauseUpdate, so a callback's
        // owner replacement is observed by Cmd_Stop.
        if (!state->owner) return Status::invalid_source_fact;
        status = invoke(*state, bound, *result, Phase::controller_stop,
            Operation::stop_character_controller, ScriptCallback::none,
            state->owner, 0);
        if (status != Status::complete) return status;
    }

    // AISExternal tests the live flags only after AISDefault has returned.
    if (state->flags_b8 & 1u) {
        const auto status = invoke(*state, bound, *result,
            Phase::script_on_update, Operation::call_ais_on_update,
            ScriptCallback::on_update, state->owner, 0);
        if (status != Status::complete) return status;
        result->script_on_update_called = 1;
    }

    // The source helper functions independently reload +0xb4. The adapters
    // behind these two operations implement each null guard and read +0x14 or
    // +0x2c from the live table; this orchestration deliberately does not cache
    // that pointer across either callback.
    auto status = invoke(*state, bound, *result, Phase::state_update,
        Operation::call_state_update, ScriptCallback::state_update,
        state->owner, 0);
    if (status != Status::complete) return status;
    status = invoke(*state, bound, *result, Phase::state_conditions,
        Operation::call_state_conditions, ScriptCallback::state_conditions,
        state->owner, 0);
    if (status != Status::complete) return status;
    result->phase = Phase::complete;
    return Status::complete;
}

}  // namespace dh2::ais_external_update
