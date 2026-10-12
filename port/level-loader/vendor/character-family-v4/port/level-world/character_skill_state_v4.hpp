#pragma once
#include "character_state.hpp"
#include "../game-data/skill_tables.hpp"
namespace dh2::character::skills {
// Borrow the authoritative FSM state; these are only fields absent from State56.
struct SkillStateV4 {
 State* state;std::uintptr_t character,physical,target,last_target;
 std::uint32_t index;std::uint8_t moving,heading_enabled,reserved[2];
};
enum SkillStateOperationV4:std::uint32_t {skill_state_focus_v4,skill_state_blur_v4,skill_state_event_v4,skill_state_update_v4,skill_state_select_v4,skill_state_ai_event_v4};
enum SkillStateServiceV4:std::uint32_t {
 skill_state_debug_load_v4=1,skill_state_debug_construct_v4,skill_state_debug_get_v4,skill_state_debug_destroy_v4,
 skill_state_stop_v4,skill_state_raise_v4,skill_state_animation_v4,skill_state_speed_v4,skill_state_cancel_sneaking_v4,
 skill_state_pin_v4,skill_state_unpin_v4,skill_state_timer_v4,skill_state_monster_v4,skill_state_miniboss_v4,skill_state_boss_v4,
 skill_state_row_v4,skill_state_constant_v4,skill_state_stance_v4,skill_state_state_event_v4,skill_state_transition_v4,
 skill_state_step_index_v4,skill_state_step_count_v4,skill_state_current_v4,skill_state_use_v4,skill_state_named_prefix_v4,skill_state_other_event_v4
};
struct SkillStateRequest32V4 {std::uint32_t operation,value,index,reserved;std::uintptr_t subject,payload;};
struct SkillStateResponse16V4 {std::uint32_t word,reserved;std::uintptr_t identity;};
struct SkillStateServices16V4 {void* context;int(*invoke)(void*,SkillStateV4*,const SkillStateRequest32V4*,SkillStateResponse16V4*);};
static_assert(sizeof(SkillStateV4)==48&&sizeof(SkillStateRequest32V4)==32&&sizeof(SkillStateResponse16V4)==16&&sizeof(SkillStateServices16V4)==16);
}
// 0 complete, -1 malformed pre-entry, -2 required backend/unsafe source domain.
// Select inputs are unsigned skill index, RAW moving byte, payload, force bool.
// Event input is the state event ID and a borrowed NUL-terminated C string for28.
// AI event entry reproduces index/count/prefix/state order; exact do_skill state6
// uses the same skill callback. Nonempty generic prefixes and states5/7/13
// require their complete source service, and never default to success.
// Requests preserve synchronous source order and retain effects on failure.
extern "C" int dh2_character_skill_state_v4(dh2::character::skills::SkillStateV4*,std::uint32_t operation,std::uint32_t index,std::uint32_t moving,std::uintptr_t payload,std::uint32_t force,const dh2::character::skills::SkillStateServices16V4*);
