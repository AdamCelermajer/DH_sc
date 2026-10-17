#pragma once

#include <cstdint>

// Host-port views for a small set of read-only GameObject/Character Lua
// callbacks. They contain only source-owned values or pointers to the current
// state-machine owner; they are not the original C++ object ABI.
namespace dh2::gameplay::callbacks {

struct GameObjectView {
    const char* name;                    // borrowed from the owning object
    const float* world_position_xyz;    // borrowed [x,y,z] in world space
};

struct CharacterView {
    GameObjectView object;
    const std::int32_t* current_state;  // supplied by the state-machine owner
    const std::int32_t* state_time;     // supplied by the Character owner
    const std::uint16_t* hit_count;     // supplied by the Character owner
};

// GameObject::_GetID pushes the receiver pointer. Preserve that identity as a
// pointer token; this does not manufacture a numeric object ID.
const void* get_id_identity(const void* callback_self) noexcept;

// Returns false only when the view/output needed to make the projection is
// absent. On failure, caller outputs are left untouched. Name may itself be
// null and is returned as a borrowed pointer, matching the callback's direct
// pushString call.
bool get_name(const GameObjectView* object, const char** out_name) noexcept;

// Copies the three world-position floats verbatim. In particular, this reader
// does not clamp or normalize coordinates and performs no floor/physics work.
bool get_position(const GameObjectView* object, float out_xyz[3]) noexcept;

bool get_state(const CharacterView* character,
               std::int32_t* out_state) noexcept;
bool get_state_time(const CharacterView* character,
                    std::int32_t* out_state_time) noexcept;
// The original 16-bit hit counter is promoted to a non-negative 32-bit Lua
// integer, including 65535.
bool get_hit_count(const CharacterView* character,
                   std::int32_t* out_hit_count) noexcept;

}  // namespace dh2::gameplay::callbacks
