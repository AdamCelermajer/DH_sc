#pragma once
#include <cstddef>
#include <cstdint>

namespace dh2::character_native_bindings {
enum class Kind : std::uint8_t {function,method};
enum class Context : std::uint8_t {none,character};
// Stable native provider keys, never original ELF callable addresses.
enum class Function : std::uint16_t {
    game_object_lock,
    game_object_unlock,
    game_object_get_id,
    game_object_get_name,
    game_object_get_self,
    game_object_get_object_by_name,
    game_object_is_dead,
    game_object_is_character,
    game_object_is_player,
    game_object_is_door,
    game_object_is_decor,
    game_object_get_position,
    game_object_set_position,
    game_object_get_distance_from,
    game_object_get_distance_between,
    game_object_is_in_range,
    game_object_is_over_a_hole,
    game_object_is_in_water,
    game_object_is_flying,
    game_object_mark_as_flying,
    game_object_is_swimming,
    game_object_mark_as_swimming,
    game_object_has_path,
    game_object_set_target_list_character_filter,
    game_object_set_target_list_object_filter,
    game_object_set_target_list_sorting,
    game_object_target_list_search,
    game_object_target_list_search_rect,
    game_object_target_list_resort,
    game_object_target_list_backup,
    game_object_is_target_list_empty,
    game_object_get_target_list_size,
    game_object_get_target_list_top,
    game_object_pop_target_list,
    game_object_play_fx,
    game_object_grab_fx,
    game_object_drop_fx,
    game_object_set_fx_end_point,
    game_object_play_sound3_d,
    game_object_summon,
    game_object_register_summon,
    game_object_summon_trigger_trap,
    game_object_summon_timer_trap,
    game_object_rand,
    game_object_enable_collisions,
    game_object_deal_damages,
    game_object_set_max_path,
    character_rotate_by,
    character_look_at,
    character_stop,
    character_head_to,
    character_move_to,
    character_warp_to,
    character_warp_behind,
    character_flee,
    character_attack,
    character_do_skill,
    character_begin_skill,
    character_end_skill,
    character_kill,
    character_get_char_ai_flags,
    character_has_aggro,
    character_add_aggro,
    character_clear_aggro,
    character_has_target,
    character_get_target,
    character_set_target,
    character_clear_target,
    character_set_is_targetable,
    character_look_at_target,
    character_target_in_melee_range,
    character_has_master,
    character_is_master_host_player,
    character_get_master,
    character_set_master,
    character_clear_master,
    character_get_skill_id_from_oid,
    character_get_current_skill_info,
    character_get_current_spell_info,
    character_get_current_equipped_faery_id,
    character_get_current_equipped_faery_level,
    character_get_equipped_faery_element,
    character_spawn_skill_projectile,
    character_set_projectile_target,
    character_enable_spot_targeting,
    character_get_spot_target,
    character_stop_skill,
    character_skill_combat_roll,
    character_spell_combat_roll,
    character_register_anim,
    character_play_anim,
    character_get_state,
    character_get_state_time,
    character_set_scare_state,
    character_set_stun_state,
    character_set_knock_back_state,
    character_allow_rotation,
    character_allow_skill_break,
    character_start_timer,
    character_pause_timer,
    character_resume_timer,
    character_stop_timer,
    character_get_char_id,
    character_dbg_dump_props,
    character_set_level,
    character_set_prop,
    character_get_prop,
    character_apply_prop_class,
    character_clear_props,
    character_create_buff,
    character_remove_buff,
    character_get_prop_hp,
    character_get_prop_bonus_attack_rating,
    character_get_prop_bonus_crit_rating,
    character_get_prop_bonus_damage,
    character_has_mana,
    character_use_mana,
    character_regen_hp,
    character_regen_mp,
    character_remove_dots,
    character_has_shield,
    character_has_bow,
    character_can_attack_in_melee,
    character_can_attack_from_range,
    character_get_hit_count,
    character_is_connected,
    character_set_skill_cooldown_timer_id,
    character_set_spell_cooldown_timer_id,
    count
};
struct Binding {Kind kind;const char* name;Function function;Context context;};
struct State {std::uintptr_t character,binder;};
struct Services {
    void* context;
    // Zero success. Function installation retains the exact supplied userdata.
    // Method installation has no explicit userdata (zero); its callback receiver
    // is resolved from the actual Lua object wrapper at invocation. Methods are
    // not global closures bound to this Character. The provider owns real Binder
    // installation and native callback dispatch. Unsupported providers must report
    // unresolved errors on use, never succeed with invented empty callback bodies.
    std::int32_t (*bind)(void*,std::uintptr_t binder,const Binding*,std::uintptr_t userdata);
};
struct Result {std::uint32_t calls,functions_bound,methods_bound;};
enum class Status : std::int32_t {complete,invalid_argument,service_unavailable,service_failed};
const Binding* game_object_bindings(std::size_t* count);
const Binding* character_own_bindings(std::size_t* count);
const char* provider_name(Function) noexcept;

// Complete original2648B GameObject/5332B Character registration callers.
// Character performs all86 inherited entries before its179 own entries, with
// duplicate names/aliases retained in source order. SetCharacter writes owner98
// before invoking this stage. No underlying callback/Binder bodies are claimed.
Status bind_game_object(const State*,const Services*,Result*);
Status bind_character(const State*,const Services*,Result*);

// One owning thread retains Character/Binder/provider/context/retired VM backing
// through synchronous return. Entry identities and services are captured once;
// external registration maps may mutate, but control storage must stay live and
// disjoint, output must not be overwritten and same-output reentry is forbidden.
// Independent objects/outputs may nest. Missing provider, nonzero service result
// or exception stops at the failing entry, retaining earlier effects, with no
// rollback, cleanup or extra registration. Null/misaligned/aliasing controls reject
// before effects. Native VM installation remains adapter-owned.
} // namespace dh2::character_native_bindings
