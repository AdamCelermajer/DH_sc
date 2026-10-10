#include "pc_cooldown_frame_v1.hpp"

#include <algorithm>
#include <cmath>

namespace dh::foundation::generic_skills {

double pc_cooldown_remaining_fraction_v1(double ready_at_ms, double elapsed_ms,
                                         double total_ms) noexcept {
    if (!std::isfinite(ready_at_ms) || !std::isfinite(elapsed_ms) ||
        !std::isfinite(total_ms) || total_ms <= 0.0)
        return 0.0;
    const double left = ready_at_ms - elapsed_ms;
    if (left <= 0.0) return 0.0;
    return std::clamp(left / total_ms, 0.0, 1.0);
}

std::int32_t pc_cooldown_frame_from_remaining_v1(double remaining_fraction) noexcept {
    if (!std::isfinite(remaining_fraction)) return 0;
    const double scaled = std::clamp(remaining_fraction, 0.0, 1.0) * 100.0;
    const auto frame = static_cast<std::int32_t>(scaled) - 1;
    return std::clamp(frame, 0, 99);
}

} // namespace dh::foundation::generic_skills
