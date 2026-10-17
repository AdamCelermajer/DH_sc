#include "character_ai_skill_script_constructor.hpp"

#include <cstddef>
#include <limits>

namespace dh2::character_ai_skill_script_constructor {
namespace {
struct Range { std::uintptr_t first, end; };
bool valid_range(const void* pointer, std::size_t size, std::size_t alignment,
                 Range& out) {
    const auto first = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || first % alignment ||
        first > std::numeric_limits<std::uintptr_t>::max() - size) return false;
    out = {first, first + size};
    return true;
}
bool overlaps(Range a, Range b) { return a.first < b.end && b.first < a.end; }

Status assertion(State* state, Globals* globals, const Services& services,
                 Result& result, Assertion which, std::uintptr_t args) {
    const auto level = globals->assert_level; // source reloads global per check.
    if (level == 2) return Status::fatal_source_assertion;
    if (level != 1) return Status::complete;
    if (!services.report_assertion) return Status::service_unavailable;
    const Request request{Operation::report_assertion, args, nullptr, 0,
                          which, 0x2e};
    ++result.assertions;
    result.last_operation = static_cast<std::uint32_t>(Operation::report_assertion);
    try {
        if (services.report_assertion(services.context, state, &request) != 0)
            return Status::service_failed;
    } catch (...) {
        return Status::service_failed;
    }
    return Status::complete;
}

Status invoke(State* state, const Services& services, Result& result,
              Operation operation, std::uintptr_t args, const char* text,
              std::uint32_t integer) {
    if (!services.invoke) return Status::service_unavailable;
    const Request request{operation, args, text, integer,
                          Assertion::character_nonnull, 0};
    ++result.service_calls;
    result.last_operation = static_cast<std::uint32_t>(operation);
    try {
        if (services.invoke(services.context, state, &request) != 0)
            return Status::service_failed;
    } catch (...) {
        return Status::service_failed;
    }
    return Status::complete;
}
} // namespace

Status construct(State* state, std::uintptr_t character, const char* script_name,
                 std::uint32_t skill_index, Globals* globals,
                 const Services* services, Result* result) {
    Range ranges[4];
    if (!valid_range(state, sizeof(*state), alignof(State), ranges[0]) ||
        !valid_range(globals, sizeof(*globals), alignof(Globals), ranges[1]) ||
        !valid_range(services, sizeof(*services), alignof(Services), ranges[2]) ||
        !valid_range(result, sizeof(*result), alignof(Result), ranges[3]) ||
        overlaps(ranges[0], ranges[1]) || overlaps(ranges[0], ranges[2]) ||
        overlaps(ranges[0], ranges[3]) || overlaps(ranges[1], ranges[2]) ||
        overlaps(ranges[1], ranges[3]) || overlaps(ranges[2], ranges[3]) ||
        state->identity == 0 || state->identity >
            std::numeric_limits<std::uintptr_t>::max() - 0x0cu)
        return Status::invalid_argument;

    const Services bound = *services;
    const auto object_identity = state->identity;
    const auto argument_identity = object_identity + 0x0cu;
    const auto input_character = character;
    const auto input_name = script_name;
    const auto input_index = skill_index;
    *result = {};

    // Source stores vtable, owner, and name before constructing its embedded
    // Arguments member. The native projection uses an enum tag, never the
    // original image's address point as a callable host pointer.
    state->dispatch_table = DispatchTable::char_ai_skill_script;
    state->character = input_character;
    state->script_name = input_name;
    result->dispatch_table = DispatchTable::char_ai_skill_script;

    auto status = invoke(state, bound, *result, Operation::arguments_construct,
                         argument_identity, nullptr, 0);
    if (status != Status::complete) return status;
    // These two assignments occur after Arguments::Arguments returns.
    state->last_skill_id_18 = -1;
    state->skill_index_14 = input_index;

    if (input_character == 0) {
        status = assertion(state, globals, bound, *result,
                           Assertion::character_nonnull, argument_identity);
        if (status != Status::complete) return status;
    }
    if (input_name == nullptr) {
        status = assertion(state, globals, bound, *result,
                           Assertion::name_nonnull, argument_identity);
        if (status != Status::complete) return status;
    }

    status = invoke(state, bound, *result, Operation::arguments_push_string,
                    argument_identity, input_name, 0);
    if (status != Status::complete) return status;
    status = invoke(state, bound, *result, Operation::arguments_push_integer,
                    argument_identity, nullptr, input_index);
    if (status != Status::complete) return status;

    result->returned_identity = object_identity;
    return Status::complete;
}

} // namespace dh2::character_ai_skill_script_constructor
