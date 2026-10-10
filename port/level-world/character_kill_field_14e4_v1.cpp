#include "character_kill_field_14e4_v1.hpp"

namespace dh2::character_kill_field_14e4_v1 {

Status character_constructor_completed(State* state, std::uintptr_t character) {
    if (!state || !character) return Status::invalid_argument;
    if (state->character && state->character != character)
        return Status::wrong_character;
    if (state->retired) return Status::retired;
    state->character = character;
    state->value = 0;
    state->constructor_write_seen = 1;
    state->network_spawn_write_seen = 0;
    return Status::complete;
}

Status network_spawn_marker_written(State* state, std::uintptr_t character) {
    if (!state || !character) return Status::invalid_argument;
    if (state->retired) return Status::retired;
    if (!state->constructor_write_seen) return Status::not_initialized;
    if (state->character != character) return Status::wrong_character;
    state->value = 1;
    state->network_spawn_write_seen = 1;
    return Status::complete;
}

Status read(const State* state, std::uintptr_t character, std::uint8_t* value) {
    if (!state || !character || !value) return Status::invalid_argument;
    if (state->retired) return Status::retired;
    if (!state->constructor_write_seen) return Status::not_initialized;
    if (state->character != character) return Status::wrong_character;
    *value = state->value;
    return Status::complete;
}

Status retire(State* state, std::uintptr_t character) {
    if (!state || !character) return Status::invalid_argument;
    if (state->character != character) return Status::wrong_character;
    if (state->retired) return Status::retired;
    state->retired = 1;
    return Status::complete;
}

} // namespace dh2::character_kill_field_14e4_v1
