#pragma once

#include <cstdint>

namespace dh::foundation::generic_skills {

// Remaining fraction of a source timer that started at elapsed=0 with a total
// duration: 1 - elapsed/total (CharAISkillScript::GetCooldown). Here the timer
// is described by its ready time (elapsed value at which it ends). Returns 1 at
// the start of the cooldown and 0 when ready, clamped to [0,1]. A non-positive
// or non-finite total is treated as no cooldown (0).
double pc_cooldown_remaining_fraction_v1(double ready_at_ms, double elapsed_ms,
                                         double total_ms) noexcept;

// Source FastUpdate frame for a CoolDown sprite: GotoFrame(
// (int)(remaining*100.0) - 1) clamped to [0, 99]. Frame 0 is the empty/ready
// wedge; frame 99 is the full overlay right after a cast.
std::int32_t pc_cooldown_frame_from_remaining_v1(double remaining_fraction) noexcept;

} // namespace dh::foundation::generic_skills
