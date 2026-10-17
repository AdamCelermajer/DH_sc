#pragma once

#include <cstdint>

namespace dh2::character_init_spawned_v1 {

// Ordered owner-side effects observed in Character::InitSpawned. The adapter
// deliberately carries the three virtual calls by byte offset: their target
// functions are resolved by the Character vtable at runtime.
enum class Operation : std::uint32_t {
    write_character_id,
    write_init_spawned_suppression,
    write_in_zone,
    set_initial_position,
    set_game_object_position,
    invoke_virtual,
    request_spawn_state,
};

struct Request {
    Operation operation{};
    std::int32_t integer{};
    std::int32_t secondary{};
    std::uint32_t virtual_slot{}; // byte offset from the vtable address point
    float position[3]{};
};

using Callback = std::int32_t (*)(void*, const Request*);
struct Services {
    void* context{};
    Callback invoke{};
};

enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    service_rejected,
};

// Source order for Character::InitSpawned(int, Point3D<float> const&),
// pinned at ELF 0x3b379c. Writes are intentionally not rolled back if a later
// service fails, matching the native method's in-place activation behavior.
Status activate(std::int32_t character_id, const float position[3],
                const Services* services) noexcept;

} // namespace dh2::character_init_spawned_v1
