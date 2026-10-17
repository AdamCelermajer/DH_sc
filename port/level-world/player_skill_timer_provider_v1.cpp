#include "player_skill_timer_provider_v1.hpp"

#include <cstdio>
#include <exception>

namespace {
using namespace dh2;
using namespace dh2::player_skill_timer_provider_v1;

std::int32_t start_timer(void* raw, std::uintptr_t character,
                         std::uint32_t duration, std::int32_t repeat,
                         std::int32_t event, std::uintptr_t reference) {
    auto& bindings = *static_cast<Bindings*>(raw);
    if (!bindings.coordinator || character != bindings.character ||
        bindings.coordinator->owner() != bindings.character ||
        event != 0x35 || reference != 0) {
        bindings.diagnostic = -1;
        return -1;
    }
    const auto id = bindings.coordinator->start_timer(
        duration, repeat, event, reference);
    if (id < 0) bindings.diagnostic = id;
    return id;
}

void stop_timer(void* raw, std::uintptr_t character, std::uint32_t id) {
    auto& bindings = *static_cast<Bindings*>(raw);
    if (!bindings.coordinator || character != bindings.character ||
        bindings.coordinator->owner() != bindings.character) {
        bindings.diagnostic = -1;
        return;
    }
    const auto status = bindings.coordinator->stop_timer(id);
    if (status < 0) bindings.diagnostic = status;
}

int invoke(Bindings* bindings, const dh2_script_value* arguments,
           std::uint32_t count, dh2_script_value* values,
           std::uint32_t capacity, std::uint32_t* returned, char* error,
           std::size_t error_capacity, bool starting) noexcept {
    if (!bindings) return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
    bindings->diagnostic = 0;
    int status = 0;
    try {
        status = (starting ? dh2_script_game_start_timer
                           : dh2_script_game_stop_timer)(
            &bindings->game, arguments, count, values, capacity, returned,
            error, error_capacity);
    } catch (...) {
        if (returned) *returned = 0;
        if (error && error_capacity)
            std::snprintf(error, error_capacity,
                          "native Player skill timer %s threw",
                          starting ? "start" : "stop");
        return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
    }
    if (status || !bindings->diagnostic) return status;
    if (returned) *returned = 0;
    if (error && error_capacity)
        std::snprintf(error, error_capacity,
                      "native Player skill timer %s failed (%d)",
                      starting ? "start" : "stop", bindings->diagnostic);
    return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
}
}

namespace dh2::player_skill_timer_provider_v1 {

bool bind(Bindings* bindings, character::Coordinator* coordinator,
          std::uintptr_t identity) noexcept {
    if (!bindings || !coordinator || !identity || coordinator->owner() != identity)
        return false;
    *bindings = {};
    bindings->coordinator = coordinator;
    bindings->character = identity;
    bindings->game = {bindings, identity, start_timer, stop_timer, 0};
    return true;
}

int start(Bindings* bindings, const dh2_script_value* arguments,
          std::uint32_t count, dh2_script_value* values,
          std::uint32_t capacity, std::uint32_t* returned, char* error,
          std::size_t error_capacity) noexcept {
    return invoke(bindings, arguments, count, values, capacity, returned,
                  error, error_capacity, true);
}

int stop(Bindings* bindings, const dh2_script_value* arguments,
         std::uint32_t count, dh2_script_value* values,
         std::uint32_t capacity, std::uint32_t* returned, char* error,
         std::size_t error_capacity) noexcept {
    return invoke(bindings, arguments, count, values, capacity, returned,
                  error, error_capacity, false);
}

} // namespace dh2::player_skill_timer_provider_v1
