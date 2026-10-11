#pragma once

#include <cstdint>

namespace dh2::ais_faery_master_callbacks_v1 {

struct Point3 {
    float value[3];
};

struct State {
    // Borrowed identities from the live AISFaery/Character composition.
    std::uintptr_t ais{};
    std::uintptr_t character{};
    std::uintptr_t master{};
    std::uintptr_t controller{};
};

enum class Command : std::uint32_t {
    warp_to_point,
    head_towards_point,
    head_to_point,
};

enum class Operation : std::uint32_t {
    target_position,
    look_at_vector,
    state_machine_idle,
    controller_point_command,
};

struct Request {
    Operation operation{};
    Command command{};
    std::uint32_t argument{};
    std::uintptr_t subject{};
    Point3 point{};
};

struct Response {
    std::uint32_t word{};
    Point3 point{};
};

struct Services {
    void* context{};
    // Routes synchronously to live GameObject/CharStateMachine/v2Controller
    // owners. Zero succeeds; no positions, FSM state, or controller are owned
    // by this callback kernel.
    std::int32_t (*invoke)(void*, const Request*, Response*){};
};

struct Result {
    std::uint32_t callbacks{};
    std::uint32_t event{};
    bool command_issued{};
    Command command{};
    Point3 point{};
};

enum class Status {
    complete,
    unsupported_event,
    invalid_argument,
    service_unavailable,
    failed,
};

// AISFaery's source master callbacks: out-of-sight (0x14), out-of-range
// (0x16), and in-melee-range (0x19). Other AIS/default events remain owned by
// their existing event dispatchers.
Status dispatch(State*, std::uint32_t event, const Services*, Result*);

} // namespace dh2::ais_faery_master_callbacks_v1
