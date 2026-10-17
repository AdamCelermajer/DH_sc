#include "game_object_stop.hpp"

#include <cstddef>

namespace dh2::game_object_stop { namespace {
struct Range { std::uintptr_t begin, end; };
bool range(const void* pointer, std::size_t size, std::size_t alignment, Range& out) {
    const auto begin = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || begin % alignment || begin > UINTPTR_MAX - size) return false;
    out = {begin, begin + size};
    return true;
}
bool overlap(Range left, Range right) {
    return left.begin < right.end && right.begin < left.end;
}
Status invoke(State* state, const Services& services, Result& result,
              Operation operation, std::uintptr_t subject, const float* values,
              std::uint32_t* word = nullptr) {
    if (!services.invoke) return Status::service_unavailable;
    Request request{operation, 0, state->object_identity, subject, {0, 0, 0}};
    if (values) for (unsigned i = 0; i < 3; ++i) request.values[i] = values[i];
    Response response{};
    ++result.service_calls;
    result.last_operation = static_cast<std::uint32_t>(operation);
    try {
        if (services.invoke(services.context, state, &request, &response))
            return Status::service_failed;
    } catch (...) {
        return Status::service_failed;
    }
    if (word) *word = response.word;
    return Status::complete;
}
}  // namespace

Status execute(State* state, const Services* services, Result* result) {
    Range ranges[3];
    if (!range(state, sizeof(*state), alignof(State), ranges[0]) ||
        !range(services, sizeof(*services), alignof(Services), ranges[1]) ||
        !range(result, sizeof(*result), alignof(Result), ranges[2]))
        return Status::invalid_argument;
    for (unsigned i = 0; i < 3; ++i)
        for (unsigned j = 0; j < i; ++j)
            if (overlap(ranges[i], ranges[j])) return Status::invalid_argument;
    if (!state->object_identity || !state->path_identity ||
        state->reserved[0] || state->reserved[1] || state->reserved[2] ||
        state->reserved[3] || state->reserved[4] || state->reserved[5])
        return Status::invalid_argument;
    if (!services->invoke) return Status::service_unavailable;

    const auto object = state->object_identity;
    const auto path = state->path_identity;
    const Services bound = *services;
    *result = {};
    auto status = invoke(state, bound, *result, Operation::drop_path, path, nullptr);
    if (status != Status::complete) return status;
    result->phase = 1;  // DropPath returned.

    // These writes follow PFWorld::DropPath and precede the optional physical
    // branch. The source copies the live position after DropPath returns.
    for (unsigned i = 0; i < 3; ++i) state->destination[i] = state->position[i];
    state->moving = 0;
    state->heading_active = 0;
    state->heading[0] = state->heading[1] = state->heading[2] = 0.f;
    result->phase = 2;  // Logical destination/movement/heading writes complete.

    if (!state->physical_identity) return Status::complete;
    std::uint32_t updates_position_from_physics = 0;
    status = invoke(state, bound, *result,
                    Operation::is_updating_position_from_physics,
                    object, nullptr, &updates_position_from_physics);
    if (status != Status::complete || !updates_position_from_physics) return status;
    result->phase = 3;  // Source vtable query admitted physical reset.

    const float zero[3]{};
    status = invoke(state, bound, *result, Operation::set_linear_velocity,
                    state->physical_identity, zero);
    if (status != Status::complete) return status;
    ++result->physical_setters;
    result->phase = 4;
    status = invoke(state, bound, *result, Operation::set_angular_velocity,
                    state->physical_identity, zero);
    if (status != Status::complete) return status;
    ++result->physical_setters;
    result->phase = 5;

    // GameObject::Stop reloads +0x2dc and current XY after each synchronous
    // physical call. Preserve that live-owner/fact behavior here.
    const float current_xy[3]{state->position[0], state->position[1], 0.f};
    status = invoke(state, bound, *result, Operation::set_position,
                    state->physical_identity, current_xy);
    if (status != Status::complete) return status;
    ++result->physical_setters;
    result->phase = 6;
    if (!bound.resolve_body) return Status::service_unavailable;
    dh2::physical::BodyState* body = nullptr;
    try {
        body = bound.resolve_body(bound.context, state->physical_identity);
    } catch (...) {
        return Status::service_failed;
    }
    Range body_range{};
    if (!range(body, sizeof(*body), alignof(dh2::physical::BodyState), body_range))
        return Status::invalid_body;
    for (const auto forbidden : ranges)
        if (overlap(body_range, forbidden)) return Status::invalid_body;
    if (dh2_physical_stop_finish(body) != 0)
        return Status::invalid_body;
    result->phase = 7;
    return Status::complete;
}

}  // namespace dh2::game_object_stop

extern "C" int dh2_game_object_stop(
    dh2::game_object_stop::State* state,
    const dh2::game_object_stop::Services* services,
    dh2::game_object_stop::Result* result) {
    return static_cast<int>(dh2::game_object_stop::execute(state, services, result));
}
