#pragma once

#include "physical_controls.hpp"
#include <cstdint>

namespace dh2::game_object_stop {

// A logical projection of fields touched by GameObject::Stop. It is not an
// ARM32 object overlay. Identities and the currently resolved body owner stay
// full width; synchronous providers own the embedded PF object and physical
// object implementations.
struct State {
    std::uintptr_t object_identity;
    std::uintptr_t path_identity;       // source `this + 0x1c8`
    std::uintptr_t physical_identity;  // source `this + 0x2dc`, refreshed live
    float position[3];                 // source +0x160
    float destination[3];              // source +0x1a8
    float heading[3];                  // source +0x1b8
    std::uint8_t moving, heading_active;
    std::uint8_t reserved[6];
};

enum class Operation : std::uint32_t {
    drop_path,
    is_updating_position_from_physics,
    set_linear_velocity,
    set_angular_velocity,
    set_position,
};
struct Request {
    Operation operation;
    std::uint32_t reserved;
    std::uintptr_t object_identity;
    std::uintptr_t subject_identity;
    float values[3];
};
struct Response { std::uint32_t word; };
struct Services {
    void* context;
    // Zero is synchronous success. drop_path must perform the owned path
    // release. is_updating_position_from_physics is the original virtual
    // query at GameObject vtable +0x64 (Character overrides it from +0x520
    // bit 1). Setter services must perform their named physical operation, including the actual
    // transform backend for set_position. No missing service is treated as a
    // successful Stop.
    std::int32_t (*invoke)(void*, State*, const Request*, Response*);
    // Native projection lookup for the final inline writes at source
    // `physical+0x14`. This is not an original virtual call; it must resolve
    // the currently reloaded physical identity after setPosition returns.
    dh2::physical::BodyState* (*resolve_body)(void*, std::uintptr_t);
};
enum class Status : std::int32_t {
    complete = 0, invalid_argument = 1, service_unavailable = 2,
    service_failed = 3, invalid_body = 4,
};
struct Result {
    std::uint32_t phase, service_calls, last_operation, physical_setters;
};

// Complete bounded caller ordering from GameObject::Stop at 0x3938f8:
// PFWorld::DropPath; destination/moving/heading reset; physical-presence gate;
// virtual IsUpdatingPositionFromPhysics query; zero linear, zero angular,
// current XY position; and
// direct sleeping/velocity/force/torque/sleep-time body reset. Path and setter
// owners remain required synchronous services.
Status execute(State*, const Services*, Result*);

static_assert(sizeof(void*) == 8);
static_assert(sizeof(State) == 72);
static_assert(sizeof(Request) == 40);
static_assert(sizeof(Services) == 24);
static_assert(sizeof(Result) == 16);

}  // namespace dh2::game_object_stop

extern "C" int dh2_game_object_stop(
    dh2::game_object_stop::State*, const dh2::game_object_stop::Services*,
    dh2::game_object_stop::Result*);
