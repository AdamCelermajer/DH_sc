#pragma once

#include "actor_runtime.hpp"
#include "character_coordinator.hpp"
#include "game_object_stop.hpp"

#include <cstdint>

namespace dh2::native_character_stop {

// Borrowed live owners for one synchronous Character::Stop dispatch. The
// caller retains Coordinator, RuntimeState, NativeBody, Box2D world and body
// contact owners until stop() returns. This is not an engine-object overlay.
// The source movement bytes are distinct storage outside those typed owners;
// neither the view nor output may alias their owners or route backing.
struct LiveView {
    std::uint64_t actor_identity;
    character::Coordinator* character;
    actor::RuntimeState* runtime;
    std::uint8_t* moving;
    std::uint8_t* heading_active;
    // The stable identity supplied as GameObject +0x2dc. This bounded adapter
    // uses the address of the owner's NativeBody wrapper; zero means absent.
    std::uintptr_t physical_identity;
    physical::NativeBody* native_body;
};

enum class Status : std::int32_t {
    complete = 0,
    invalid_view = 1,
    owner_mismatch = 2,
    body_mismatch = 3,
    source_failure = 4,
};

struct Result {
    std::int32_t source_status;
    std::uint32_t phase;
    std::uint32_t service_calls;
    std::uint32_t last_operation;
    std::uint32_t physical_setters;
};

// Executes the frozen GameObject::Stop caller on the borrowed source/native
// projections. Source-owned destination/heading/movement bytes are published
// before observing services. Synchronous callback edits survive later failure.
Status stop(const LiveView*, Result*);

static_assert(sizeof(void*) == 8);
static_assert(sizeof(LiveView) == 56);
static_assert(sizeof(Result) == 20);

}  // namespace dh2::native_character_stop

extern "C" int dh2_native_character_stop(
    const dh2::native_character_stop::LiveView*,
    dh2::native_character_stop::Result*);
