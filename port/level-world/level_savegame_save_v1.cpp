#include "level_savegame_save_v1.hpp"

namespace dh2::level_savegame_save_v1 {

Status run(const State* state, const Services* services, Result* output,
           std::string& error) {
    if (!state || !services || !output) {
        error = "LevelSavegame::Save requires state, services, and result";
        return Status::invalid_argument;
    }
    Result result{};
    error.clear();
    const auto finish = [&](Status status) {
        result.status = status;
        *output = result;
        return status;
    };
    const bool host_can_save = state->local_player_is_host &&
                               !state->player_manager_gate_719;
    if (!state->savegame || state->gate_39 ||
        (state->online && !host_can_save))
        return finish(Status::skipped);
    if (!services->save_all) {
        result.status = Status::service_unavailable;
        *output = result;
        error = "canonical Savegame::saveAll provider unavailable";
        return result.status;
    }
    ++result.service_calls;
    try {
        if (services->save_all(services->context, state->savegame, error)) {
            result.status = Status::service_failed;
            *output = result;
            if (error.empty()) error = "canonical Savegame::saveAll provider failed";
            return result.status;
        }
    } catch (...) {
        result.status = Status::service_failed;
        *output = result;
        error = "canonical Savegame::saveAll provider threw";
        return result.status;
    }
    return finish(Status::saved);
}

} // namespace dh2::level_savegame_save_v1
