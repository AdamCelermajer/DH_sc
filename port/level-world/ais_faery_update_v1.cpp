#include "ais_faery_update_v1.hpp"

#include <cstddef>

namespace dh2::ais_faery_update_v1 {
namespace {
template<class T> bool valid_range(const T* value) {
    const auto address = reinterpret_cast<std::uintptr_t>(value);
    return value && address % alignof(T) == 0 &&
        address <= UINTPTR_MAX - sizeof(T);
}
bool overlaps(const void* left, std::size_t left_size,
              const void* right, std::size_t right_size) {
    const auto a = reinterpret_cast<std::uintptr_t>(left);
    const auto b = reinterpret_cast<std::uintptr_t>(right);
    return a < b + right_size && b < a + left_size;
}
}

Status update(State* state, const Services* services, Result* result) {
    if (!valid_range(state) || !valid_range(services) || !valid_range(result) ||
        !state->identity || !valid_range(state->character) ||
        !state->character->identity ||
        overlaps(state, sizeof(*state), services, sizeof(*services)) ||
        overlaps(state, sizeof(*state), result, sizeof(*result)) ||
        overlaps(services, sizeof(*services), result, sizeof(*result)) ||
        overlaps(state->character, sizeof(*state->character), state, sizeof(*state)) ||
        overlaps(state->character, sizeof(*state->character), services, sizeof(*services)) ||
        overlaps(state->character, sizeof(*state->character), result, sizeof(*result)))
        return Status::invalid_argument;
    const auto bound = *services;
    *result = {};
    result->current_faery_id = state->last_faery_id;
    if (!bound.invoke) return Status::service_unavailable;

    const auto call = [&](Operation operation, std::uintptr_t subject,
                          std::int32_t first, std::int32_t second,
                          std::int32_t* output) {
        ++result->callbacks;
        try {
            return bound.invoke(bound.context,
                {operation, subject, first, second}, output) == 0;
        } catch (...) {
            return false;
        }
    };

    std::int32_t ignored = 0;
    if (!call(Operation::default_on_update, state->identity, 0, 0, &ignored))
        return Status::failed;
    const auto master = state->character->master;
    if (!master) return Status::complete;
    const auto visual = state->character->visual;
    if (!visual) return Status::complete;

    std::int32_t observed = -1;
    if (!call(Operation::current_faery_id, master, -1, 0, &observed))
        return Status::failed;
    if (observed == state->last_faery_id) return Status::complete;

    // Source calls SG_GetCurrentFaerieId a second time at the store site.
    std::int32_t selected = -1;
    if (!call(Operation::current_faery_id, master, -1, 0, &selected))
        return Status::failed;
    state->last_faery_id = selected;
    result->current_faery_id = selected;
    if (!call(Operation::set_modular_skin, visual, 0, selected, &ignored))
        return Status::failed;
    result->visual_changed = true;
    return Status::complete;
}

} // namespace dh2::ais_faery_update_v1
