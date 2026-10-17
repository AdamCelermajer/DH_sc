#include "gameplay_object_callbacks.hpp"

#include <cstring>

namespace dh2::gameplay::callbacks {

const void* get_id_identity(const void* callback_self) noexcept {
    return callback_self;
}

bool get_name(const GameObjectView* object, const char** out_name) noexcept {
    if (!object || !out_name) return false;
    *out_name = object->name;
    return true;
}

bool get_position(const GameObjectView* object, float out_xyz[3]) noexcept {
    if (!object || !object->world_position_xyz || !out_xyz) return false;
    float position[3];
    std::memcpy(position, object->world_position_xyz, sizeof(position));
    std::memcpy(out_xyz, position, sizeof(position));
    return true;
}

bool get_state(const CharacterView* character,
               std::int32_t* out_state) noexcept {
    if (!character || !character->current_state || !out_state) return false;
    *out_state = *character->current_state;
    return true;
}

bool get_state_time(const CharacterView* character,
                    std::int32_t* out_state_time) noexcept {
    if (!character || !character->state_time || !out_state_time) return false;
    *out_state_time = *character->state_time;
    return true;
}

bool get_hit_count(const CharacterView* character,
                   std::int32_t* out_hit_count) noexcept {
    if (!character || !character->hit_count || !out_hit_count) return false;
    *out_hit_count = static_cast<std::int32_t>(*character->hit_count);
    return true;
}

}  // namespace dh2::gameplay::callbacks
