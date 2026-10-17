#pragma once

#include <cstdint>

namespace dh2::character_physical_collision_gate_v1 {

enum class Decision : std::uint32_t {
    delegate_generic,
    reject_limbus,
    reject_knockback_category,
    invalid_argument,
};

// POCharacter::onCollisionTest at 0x46fb4c asks SM_IsInLimbus first, then
// SM_IsKnockedBack. Its sixth scalar argument is the other body's category.
// Every accepted branch continues into PhysicalObject::onCollisionTest.
inline Decision evaluate(std::int32_t current_state,
                         std::uint16_t other_category) noexcept {
    if (current_state == 0) return Decision::reject_limbus;
    if (current_state == 10 && (other_category & 3u) == 0)
        return Decision::reject_knockback_category;
    return Decision::delegate_generic;
}

} // namespace dh2::character_physical_collision_gate_v1
