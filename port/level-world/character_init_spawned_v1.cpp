#include "character_init_spawned_v1.hpp"

namespace dh2::character_init_spawned_v1 {
namespace {

bool emit(const Services& services, Operation operation, std::int32_t integer,
          std::int32_t secondary, std::uint32_t virtual_slot,
          const float* position) noexcept {
    Request request{};
    request.operation = operation;
    request.integer = integer;
    request.secondary = secondary;
    request.virtual_slot = virtual_slot;
    if (position) {
        request.position[0] = position[0];
        request.position[1] = position[1];
        request.position[2] = position[2];
    }
    return services.invoke(services.context, &request) == 0;
}

} // namespace

Status activate(std::int32_t character_id, const float position[3],
                const Services* services) noexcept {
    if (!position || !services || !services->invoke)
        return Status::invalid_argument;
    const auto character_id_bits = static_cast<std::uint32_t>(character_id) & 0xffffu;
    const auto stored_character_id = character_id_bits < 0x8000u
        ? static_cast<std::int32_t>(character_id_bits)
        : static_cast<std::int32_t>(character_id_bits) - 0x10000;

    // Character::InitSpawned stores the character id, sets Character+0x1481 and
    // GameObject+0x2f0, then initializes and positions the owner. It calls
    // vtable byte offsets +0x1c, +0x58, +0x40(true) before asking the state
    // machine for Spawn with arguments false,false.
    if (!emit(*services, Operation::write_character_id, stored_character_id, 0, 0,
              nullptr) ||
        !emit(*services, Operation::write_init_spawned_suppression, 1, 0, 0,
              nullptr) ||
        !emit(*services, Operation::write_in_zone, 1, 0, 0, nullptr) ||
        !emit(*services, Operation::set_initial_position, 0, 0, 0, position) ||
        !emit(*services, Operation::set_game_object_position, 0, 0, 0,
              nullptr) ||
        !emit(*services, Operation::invoke_virtual, 0, 0, 0x1c, nullptr) ||
        !emit(*services, Operation::invoke_virtual, 0, 0, 0x58, nullptr) ||
        !emit(*services, Operation::invoke_virtual, 1, 0, 0x40, nullptr) ||
        !emit(*services, Operation::request_spawn_state, 0, 0, 0, nullptr))
        return Status::service_rejected;

    return Status::complete;
}

} // namespace dh2::character_init_spawned_v1
