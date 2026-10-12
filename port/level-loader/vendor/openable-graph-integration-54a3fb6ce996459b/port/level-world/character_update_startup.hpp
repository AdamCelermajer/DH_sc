#pragma once
#include "character_can_update.hpp"
namespace dh2::character {
// Live borrowed source loading-queue header projection. This first coordinator
// does not own or mutate its RB tree: reached eviction/registration is an
// explicit unavailable boundary, never an accepted arbitrary queue callback.
struct UpdateStartupQueue16 {std::uint32_t count,reserved;std::uintptr_t first;};
struct UpdateStartupOwner64 {
 CanUpdateOwner40* eligibility;
 const CanUpdateServices24* eligibility_services;
 std::uintptr_t active_ai,controller;
 std::uint32_t* application_updates;
 UpdateStartupQueue16* queue;
 std::int32_t resolved_hp;
 std::uint8_t delayed_load,reserved[3];
 std::uint32_t reserved_word;
};
struct UpdateStartupPlayer24 {
 std::uintptr_t identity,character;
 std::int32_t player_number;
 std::uint8_t bot_enabled,reserved[3];
};
enum UpdateStartupOperation:std::uint32_t {
 update_debug_load,update_debug_construct,update_debug_query,update_debug_destroy,
 update_get_player,update_set_property,update_is_dead,update_cmd_kill,
 update_is_player,update_set_potions,update_get_state,update_get_current_level,
 update_load_and_init,update_is_monster,update_is_miniboss,update_is_boss,
 update_online,update_real_time,update_player_for_character
};
struct UpdateStartupRequest40 {
 std::uint32_t operation,argument,argument2,reserved;
 std::uintptr_t owner,subject;
 const char* name;
};
struct UpdateStartupResponse16 {std::uintptr_t identity;std::uint32_t word,reserved;};
using UpdateStartupInvoke=int(*)(void*,UpdateStartupOwner64*,const UpdateStartupRequest40*,UpdateStartupResponse16*);
struct UpdateStartupServices24 {void* context;UpdateStartupInvoke invoke;std::uint32_t available,reserved;};
enum UpdateStartupStage:std::uint32_t {update_ineligible,update_controller_ready,update_timers_ready};
static_assert(sizeof(UpdateStartupOwner64)==64&&sizeof(UpdateStartupPlayer24)==24);
static_assert(sizeof(UpdateStartupRequest40)==40&&sizeof(UpdateStartupResponse16)==16&&sizeof(UpdateStartupServices24)==24);
}
// Source Character.Update entry→controller v8 boundary. On source ineligible
// returns stage0; otherwise stage1 after source Application stats+5c increment.
// Does NOT invoke the controller, timers, AI, FSM, Animator, GameObject or tail.
// Services are synchronous; owner fields are reloaded at actual source points.
// 0 complete,1 malformed,2 required service/queue continuation unavailable,
// 3 provider delivery failure. No rollback; stage only changes on complete.
extern "C" int dh2_character_update_startup(dh2::character::UpdateStartupOwner64*,
 const dh2::character::UpdateStartupServices24*,std::uint32_t* stage);
// Call ONLY AFTER actual controller v8 delivery. Exact isABot debug prefix and
// its online/player early returns; reached nonempty target bot logic is a
// required continuation failure. Complete return sets stage2 (timers ready).
extern "C" int dh2_character_update_after_controller(dh2::character::UpdateStartupOwner64*,
 const dh2::character::UpdateStartupServices24*,std::uint32_t* stage);
