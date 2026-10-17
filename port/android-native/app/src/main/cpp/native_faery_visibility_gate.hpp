#pragma once

#include <cstdint>

namespace dh2::native::faery_visibility_gate {

// The roster prefix is not a constructed, initialized, placeable Faery.
struct Evidence {
    std::uint8_t source_constructor = 0;
    std::uint8_t init_post = 0;
    std::uint8_t init_final = 0;
    std::uint8_t physics = 0;
    std::uint8_t faery_ai = 0;
    std::uint8_t visual = 0;
    std::uint8_t placement = 0;
    std::uint8_t registered = 0;
};

constexpr bool may_show(const Evidence& e) noexcept {
    return e.source_constructor && e.init_post && e.init_final && e.physics &&
           e.faery_ai && e.visual && e.placement && e.registered;
}

} // namespace dh2::native::faery_visibility_gate
