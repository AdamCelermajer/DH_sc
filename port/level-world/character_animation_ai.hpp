#pragma once
#include <cstddef>
#include <cstdint>

namespace dh2::character {
// Borrowed live projection. owner+4c8 is animator stack depth, not combo type.
struct AnimationAIState96 {
 std::uintptr_t owner,controller,target,look_target;
 std::int32_t animation_depth;
 std::uint32_t owner_flags,seeking,target_sticky;
 std::int32_t attack_index;
 std::uint32_t attack_continued,attack_last,attack_finisher,skill_started,skill_stop_requested;
 // Raw signed owner byte+14a8. Its higher-level producer is not reconstructed.
 std::int32_t owner_byte14a8;
 std::uint32_t reserved0;
 float owner_position[3];
 // Validity metadata for the existing CharAI+0x79 byte, which aliases
 // Character+0x441. CharAI::C2 does not initialize it; the Attack-step writer
 // below makes it known. This occupies the previous trailing reserved word,
 // so the borrowed projection stays 96 bytes.
 std::uint32_t attack_last_79_available;
};
enum AnimationAIService : std::uint32_t {
 ai_animation_state=0,ai_animation_step_index,ai_animation_step_count,
 ai_has_combo_attack,ai_target_position,ai_melee_radius_squared,
 ai_controller_move_to,ai_controller_look_at,ai_pre_attack_virtual,
 ai_target_is_dead,ai_owner_can_range_attack,ai_clear_nonsticky_target,
 ai_animation_set_step,ai_animation_skip_next_step,ai_animation_stop_loop,
 ai_character_event
};
// Exact predicates used by the reached CharAI attack-step target virtuals:
// base GameObject::IsDead returns 0; Container::IsDead reads +0x394 and tests
// the unsigned range [3,4]. The renderer must first resolve the canonical
// runtime class before choosing either predicate.
constexpr bool source_game_object_is_dead() noexcept { return false; }
constexpr bool source_container_is_dead(std::int32_t state_394) noexcept {
 return static_cast<std::uint32_t>(state_394)-3u<=1u;
}
struct AnimationAIRequest32 {
 std::uint32_t service,argument,reserved0,reserved1;
 std::uintptr_t subject,payload;
};
struct AnimationAIResponse16 {
 std::uint32_t word;
 float position[3];
};
struct AnimationAIServices16 {
 void* context;
 // Invoke synchronously. Queries return word (including float radius bits)
 // or XYZ. Callbacks may refresh live state; captured source locals stay local.
 // Return zero when the requested provider operation completed. A failure
 // stops the source consumer before it can issue dependent follow-up calls.
 std::int32_t(*invoke)(void*,AnimationAIState96*,const AnimationAIRequest32*,AnimationAIResponse16*);
};
enum AnimationAIOperation : std::uint32_t {
 ai_step_begin=0,ai_step_end,ai_move_begin,ai_attack_begin,ai_attack_end,
 ai_skill_begin,ai_skill_end
};
static_assert(sizeof(void*)==8&&sizeof(AnimationAIState96)==96);
static_assert(sizeof(AnimationAIRequest32)==32&&sizeof(AnimationAIResponse16)==16);
static_assert(sizeof(AnimationAIServices16)==16);
}
// 1 source consumer accepted after its actual kernel/services; -1 malformed.
extern "C" int dh2_character_animation_ai(
 dh2::character::AnimationAIState96*,std::uint32_t,
 const dh2::character::AnimationAIServices16*);
// Character::HasComboAttack's resolved table projection. Lookup/table producer
// remains explicit: Attack index row+4, sequence type row+10, not equipment.
extern "C" int dh2_character_animation_has_combo(
 std::int32_t attack_sequence,std::int32_t sequence_count,std::int32_t sequence_type);
extern "C" std::int32_t dh2_character_animation_table_id(
 std::int32_t cached_property_index2,std::int32_t animation_table_count);
