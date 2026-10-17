#include "character_cast_lifecycle_v1.hpp"

#include <cstddef>

namespace dh2::character_cast_lifecycle_v1 {
namespace {
struct Range { std::uintptr_t begin, end; };

bool make_range(const void* pointer, std::size_t size, std::size_t alignment,
                Range& out) {
    const auto begin = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || begin % alignment || begin > UINTPTR_MAX - size)
        return false;
    out = {begin, begin + size};
    return true;
}

bool overlaps(Range left, Range right) {
    return left.begin < right.end && right.begin < left.end;
}

struct Run {
    const Character& character;
    std::uintptr_t debug;
    const Services& services;
    Result& result;

    Status call(Operation operation, std::uintptr_t subject,
                std::uint32_t argument = 0, std::uintptr_t string = 0,
                const char* text = nullptr, Response* response = nullptr) {
        if (!services.invoke) return Status::service_unavailable;
        const Request request{operation, character.identity, subject, string,
                              argument, text};
        Response reply{};
        result.last_operation = operation;
        ++result.calls;
        try {
            if (services.invoke(services.context, &request, &reply))
                return Status::service_failed;
        } catch (...) {
            return Status::service_failed;
        }
        if (response) *response = reply;
        return Status::complete;
    }

    Status trace_state() {
        auto status = call(Operation::debug_load, debug);
        if (status != Status::complete) return status;

        Response string{};
        status = call(Operation::string_construct, debug, 0, 0,
                      "isTracingCharState", &string);
        if (status != Status::complete) return status;
        if (!string.identity) return Status::invalid_source_fact;

        const auto query_status =
            call(Operation::debug_query, debug, 0, string.identity);
        // CSCast::OnFocus destroys its stack std::string before proceeding.
        // The native projection retains constructed strings by identity, so
        // preserve that lifetime even when the debug query provider fails.
        const auto destroy_status =
            call(Operation::string_destroy, string.identity);
        if (query_status != Status::complete) return query_status;
        return destroy_status;
    }

    Status focus() {
        auto status = trace_state();
        if (status != Status::complete) return status;

        *character.flags_520 = 25345u;
        result.flags_written = 1;

        status = call(Operation::raise_event, character.identity, 32u);
        if (status != Status::complete) return status;
        status = call(Operation::set_animation, character.machine,
                      UINT32_MAX);
        if (status != Status::complete) return status;
        status = call(Operation::set_speed, character.animator, 0x3f800000u);
        if (status != Status::complete) return status;

        *character.ooi_intent_412 = 0;
        result.ooi_intent_cleared = 1;
        return call(Operation::cancel_sneaking, character.identity);
    }

    Status blur() {
        auto status = trace_state();
        if (status != Status::complete) return status;
        return call(Operation::raise_event, character.identity, 33u);
    }
};
} // namespace

Status execute(Callback callback, const Character* character,
               const Globals* globals, const Services* services,
               Result* output) {
    if (callback != Callback::focus && callback != Callback::blur)
        return Status::invalid_argument;

    Range character_range{}, globals_range{}, services_range{}, output_range{},
          state_range{}, intent_range{};
    if (!make_range(character, sizeof(*character), alignof(Character),
                    character_range) ||
        !make_range(globals, sizeof(*globals), alignof(Globals), globals_range) ||
        !make_range(services, sizeof(*services), alignof(Services), services_range) ||
        !make_range(output, sizeof(*output), alignof(Result), output_range) ||
        overlaps(character_range, globals_range) ||
        overlaps(character_range, services_range) ||
        overlaps(character_range, output_range) ||
        overlaps(globals_range, services_range) ||
        overlaps(globals_range, output_range) ||
        overlaps(services_range, output_range))
        return Status::invalid_argument;

    const auto& owner = *character;
    if (!owner.identity || !owner.coordinator_state || !owner.machine ||
        !owner.animator || !owner.ooi_intent_412 || !globals->debug_switches ||
        owner.flags_520 != &owner.coordinator_state->flags ||
        owner.coordinator_state->current != 7 ||
        !make_range(owner.coordinator_state, sizeof(*owner.coordinator_state),
                    alignof(character::State), state_range) ||
        !make_range(owner.ooi_intent_412, sizeof(*owner.ooi_intent_412),
                    alignof(std::uint8_t), intent_range) ||
        overlaps(state_range, character_range) ||
        overlaps(state_range, globals_range) ||
        overlaps(state_range, services_range) ||
        overlaps(state_range, output_range) ||
        overlaps(state_range, intent_range) ||
        overlaps(intent_range, character_range) ||
        overlaps(intent_range, globals_range) ||
        overlaps(intent_range, services_range) ||
        overlaps(intent_range, output_range))
        return Status::invalid_argument;

    *output = {};
    if (!services->invoke) return Status::service_unavailable;
    Run run{owner, globals->debug_switches, *services, *output};
    const auto status = callback == Callback::focus ? run.focus() : run.blur();
    if (status == Status::complete) output->complete = 1;
    return status;
}

Status execute(Projection* projection, Callback callback, Result* output) {
    if (!projection) return Status::invalid_argument;
    return execute(callback, &projection->character, &projection->globals,
                   &projection->services, output);
}

} // namespace dh2::character_cast_lifecycle_v1
