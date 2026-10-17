#include "ais_faery_master_callbacks_v1.hpp"

#include <cstddef>
#include <limits>

namespace dh2::ais_faery_master_callbacks_v1 {
namespace {
template<class T> bool valid(const T* value) {
    const auto address = reinterpret_cast<std::uintptr_t>(value);
    return value && address % alignof(T) == 0 &&
        sizeof(T) - 1 <= std::numeric_limits<std::uintptr_t>::max() - address;
}

bool overlaps(const void* left, std::size_t left_size,
              const void* right, std::size_t right_size) {
    const auto a = reinterpret_cast<std::uintptr_t>(left);
    const auto b = reinterpret_cast<std::uintptr_t>(right);
    return a <= b ? b - a < left_size : a - b < right_size;
}
}

Status dispatch(State* state, std::uint32_t event,
                const Services* services, Result* result) {
    if (!valid(state) || !valid(services) || !valid(result) ||
        overlaps(state, sizeof(*state), services, sizeof(*services)) ||
        overlaps(state, sizeof(*state), result, sizeof(*result)) ||
        overlaps(services, sizeof(*services), result, sizeof(*result)) ||
        !state->ais || !state->character || !state->master || !state->controller)
        return Status::invalid_argument;

    *result = {};
    result->event = event;
    if (event != 0x14 && event != 0x16 && event != 0x19)
        return Status::unsupported_event;
    const auto bound = *services;
    if (!bound.invoke) return Status::service_unavailable;

    const auto invoke = [&](Operation operation, std::uintptr_t subject,
                            Command command, std::uint32_t argument,
                            const Point3& point, Response& response) {
        const Request request{operation, command, argument, subject, point};
        ++result->callbacks;
        try {
            return bound.invoke(bound.context, &request, &response) == 0;
        } catch (...) {
            return false;
        }
    };
    const auto vector_for = [&](Operation operation, std::uintptr_t subject,
                                Point3& point) {
        Response response{};
        if (!invoke(operation, subject, Command::warp_to_point, 0, {}, response))
            return false;
        point = response.point;
        return true;
    };
    const auto idle = [&](std::uintptr_t subject, bool& is_idle) {
        Response response{};
        if (!invoke(Operation::state_machine_idle, subject,
                    Command::warp_to_point, 0, {}, response)) return false;
        is_idle = response.word != 0;
        return true;
    };
    const auto issue = [&](Command command, const Point3& point) {
        Response ignored{};
        if (!invoke(Operation::controller_point_command, state->controller,
                    command, 0, point, ignored)) return false;
        result->command = command;
        result->point = point;
        result->command_issued = true;
        return true;
    };

    Point3 destination{};
    if (event == 0x14) {
        if (!vector_for(Operation::target_position, state->master, destination))
            return Status::failed;
        return issue(Command::warp_to_point, destination)
            ? Status::complete : Status::failed;
    }
    if (event == 0x16) {
        Point3 current{};
        if (!vector_for(Operation::target_position, state->master, destination) ||
            !vector_for(Operation::target_position, state->character, current))
            return Status::failed;
        for (std::size_t i = 0; i != 3; ++i)
            destination.value[i] -= current.value[i];
        return issue(Command::head_towards_point, destination)
            ? Status::complete : Status::failed;
    }

    bool is_idle = false;
    if (!idle(state->character, is_idle)) return Status::failed;
    if (!is_idle) return Status::complete;
    if (!idle(state->master, is_idle)) return Status::failed;
    if (!is_idle) return Status::complete;

    Point3 look{};
    if (!vector_for(Operation::look_at_vector, state->master, look) ||
        !vector_for(Operation::target_position, state->master, destination))
        return Status::failed;
    for (std::size_t i = 0; i != 3; ++i)
        destination.value[i] += -look.value[i] * 200.0f;
    return issue(Command::head_to_point, destination)
        ? Status::complete : Status::failed;
}

} // namespace dh2::ais_faery_master_callbacks_v1
