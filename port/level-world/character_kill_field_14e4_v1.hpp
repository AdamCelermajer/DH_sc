#pragma once

#include <cstdint>

namespace dh2::character_kill_field_14e4_v1 {

// Character::Kill reads Character+0x14e4 as the objective-tail gate. The
// original Character constructor explicitly stores byte zero there; the
// network spawn receiver later explicitly changes the same byte to one.
// This retained projection records only those two source lifecycle writes.
struct State {
    std::uintptr_t character = 0;
    std::uint8_t value = 0;
    std::uint32_t constructor_write_seen = 0;
    std::uint32_t network_spawn_write_seen = 0;
    std::uint32_t retired = 0;
};

enum class Status : std::uint32_t {
    complete,
    invalid_argument,
    wrong_character,
    not_initialized,
    retired,
};

// Call only after the source-equivalent Character constructor completed.
// This is the source-observed `*(BYTE*)(this + 5348) = 0` at 0x3a9714.
Status character_constructor_completed(State*, std::uintptr_t character);

// Call only from the source-equivalent HandleSpawnObjectMessage branch after
// its explicit `NPC+5348 = 1` store at 0x49abac. Repeated calls are harmless.
Status network_spawn_marker_written(State*, std::uintptr_t character);

// Read the retained byte for the same canonical Character. A never-seen or
// retired Character is unavailable; callers must not substitute zero.
Status read(const State*, std::uintptr_t character, std::uint8_t* value);

// Ends the retained projection when its canonical Character owner retires.
Status retire(State*, std::uintptr_t character);

} // namespace dh2::character_kill_field_14e4_v1
