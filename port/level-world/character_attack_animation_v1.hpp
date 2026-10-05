#pragma once
#include "character_ai_attack.hpp"

namespace dh2::character {
enum AttackAnimationServiceV1 : std::uint32_t {
 attack_anim_step_index_v1, attack_anim_step_count_v1,
 attack_anim_raise_v1, attack_anim_controller_look_v1,
 attack_anim_pre_attack_v1, attack_anim_has_combo_v1,
 attack_anim_clear_nonsticky_v1, attack_anim_set_step_v1,
 attack_anim_can_range_v1, attack_anim_target_dead_v1,
 attack_anim_skip_next_v1
};
struct AttackAnimationRequestV1 {
 std::uint32_t service, value;
 std::uintptr_t subject;
};
struct AttackAnimationServicesV1 {
 void* context{};
 // Reached queries and operations are synchronous; callbacks may mutate the
 // same AI, phase, target or owner. A failed callback preserves prior effects.
 int (*invoke)(void*, AttackState64&, const AttackAnimationRequestV1&, std::uint32_t&){};
};
struct AttackAnimationBorrowV1 {
 AttackState64* ai{};
 const std::uint32_t* phase{}; // actual Character Animator +4c8 depth
 const std::uintptr_t* look_at{}; // actual Character +408
};
}
// Full CharAI::_OnAnimStepBegin_Attack (3d4044) and End (3d3e44).
// 1 completed, -1 malformed/reached backend failure. No constructor defaults
// are introduced for source-unwritten AI fields +74/+78/+79/+7a.
extern "C" int dh2_character_attack_animation_begin_v1(
 const dh2::character::AttackAnimationBorrowV1*,
 const dh2::character::AttackAnimationServicesV1*);
extern "C" int dh2_character_attack_animation_end_v1(
 const dh2::character::AttackAnimationBorrowV1*,
 const dh2::character::AttackAnimationServicesV1*);
