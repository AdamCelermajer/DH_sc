#pragma once
#include "character_player_skills_v3.hpp"
#include "character_skill_native_v5.hpp"
#include "../game-data/combat_result.hpp"
#include "character_dot_attack.hpp"
#include "character_hit.hpp"
namespace dh2::character::skills {
// Stable borrowed identities and actual source skill-row projection. Providers
// keep backing live across calls; results must never be fabricated acceptance.
struct SkillCombatRowV6 {std::int32_t element;std::uint32_t mask;};
enum SkillCombatServiceV6:std::uint32_t {
 skill_combat_list_v6=1,skill_combat_handle_v6,skill_combat_character_v6,
 skill_combat_row_v6,skill_combat_main_hand_v6,skill_combat_off_hand_v6,
 skill_combat_calculate_v6,skill_combat_apply_v6,
 skill_combat_target_kind_v6,skill_combat_target_activate_v6
};
struct SkillCombatRequestV6 {
 std::uint32_t service,index,mask,reserved;
 std::uintptr_t attacker,target;
 std::int32_t element;std::uint32_t padding;
};
struct SkillCombatResponseV6 {
 std::uintptr_t identity;const SkillCombatRowV6* row;
 std::uint32_t word,reserved;
};
struct SkillCombatServicesV6 {
 void* context;
 int(*invoke)(void*,const SkillCombatRequestV6*,SkillCombatResponseV6*,data::CombatResult*);
};
struct SkillCombatOutputV6 {std::int32_t amount[2];std::uint32_t count,boolean_count,calls,phase;};
// Original _SkillCombatRoll wrapper. Results remain appended after completed
// application if a later offhand provider fails. Invalid Lua argument guards
// return no values as source. -1 malformed, -2 reached required service.
extern "C" int dh2_character_skill_combat_roll_v6(SkillCombatOutputV6*,
 std::uintptr_t attacker,const dh2_script_value*,std::uint32_t,
 const SkillCombatServicesV6*);
// Versioned original-result kernel. Reached native Debug delivery runs after
// status rolls and before damage; failure retains result/RNG prefix. Properties
// remain borrowed live arrays. No actor/property copies are allocated.
extern "C" unsigned dh2_combat_result_ordered_v6(data::CombatResult*,
 const data::CombatResultRequest*,void*,int(*debug)(void*));
enum SkillAttackNativeServiceV6:std::uint32_t {
 skill_attack_debug_load_v6=1,skill_attack_string_construct_v6,
 skill_attack_debug_get_v6,skill_attack_string_destroy_v6
};
struct SkillAttackNativeRequestV6 {std::uint32_t service,reserved;std::uintptr_t subject;const char* name;};
struct SkillAttackNativeServicesV6 {void* context;int(*invoke)(void*,const SkillAttackNativeRequestV6*,std::uintptr_t*);};
struct SkillAttackActorV6 {std::uintptr_t identity;data::PropertyView* properties;const data::CombatantView* facts;};
// Genuine F_SkillAttack preparation and ordered CalculateResult. Facts are the
// live native world owner's equipment/state projection, with properties exactly
// the borrowed PropertyView.resolved. Random and shared combat context are the
// actual common owners, never a private replacement random/combat singleton.
extern "C" int dh2_character_skill_attack_calculate_v6(data::CombatResult*,
 DotCombatContext32*,data::CombatRandom*,const SkillAttackActorV6*,
 const SkillAttackActorV6*,const data::FreshInventoryOwnedV4*,std::uint32_t mask,
 std::int32_t element,const SkillAttackNativeServicesV6*);
// Recovered genuine bx-lr functions only: Push/PopProfilingContext,
// PlayerStatManager::IncreaseStat/SetStatValue/Reset/Update/ApplyPlayersBonus.
// Unknown original address is rejected; these are not generic no-op providers.
extern "C" int dh2_skill_combat_empty_native_v6(std::uint32_t address);
enum SkillHitServiceV6:std::uint32_t {
 hit_local_player_v6=11,hit_trophy_manager_v6,hit_trophy_index_v6,
 hit_unlock_v6,hit_major_enemy_v6,hit_minor_enemy_v6
};
// Source HitFor offline nonplayer receiver, now including distinct player
// attacker achievement continuation. New services use HitRequest32: manager
// captures actual global identity; index name is exact source string; unlock
// subject is captured manager and force is signed index bits. Each reached
// policy/catalog/achievement call remains mandatory, with native HP prefix.
extern "C" int dh2_character_hit_for_v6(HitResult40*,HitActor32*,std::uint32_t,
 const HitAttacker24*,const HitServices16*);
enum SkillApplyServiceV6:std::uint32_t {
 skill_apply_online_v6=1,skill_apply_saved_option_v6,skill_apply_is_player_v6,
 skill_apply_is_dead_v6,skill_apply_party_count_v6,skill_apply_aggro_v6,
 skill_apply_hit_fx_v6,skill_apply_critical_camera_v6,
 skill_apply_dodge_v6,skill_apply_block_v6,skill_apply_injure_v6,
 skill_apply_push_v6,skill_apply_stun_v6,skill_apply_scare_v6,
 skill_apply_slow_v6,skill_apply_cancel_sneaking_v6,
 skill_apply_scrolling_text_v6,skill_apply_sound_v6,skill_apply_ai_combat_v6,
 skill_apply_player_lookup_v6
};
struct SkillApplyRequestV6 {
 std::uint32_t service,word,flags,reserved;
 std::uintptr_t subject,attacker,target;
 const char* name;float number;std::uint32_t padding;
};
struct SkillApplyResponseV6 {std::uintptr_t identity;std::uint32_t word;float number;};
struct SkillApplyServicesV6 {
 void* context;int(*invoke)(void*,const SkillApplyRequestV6*,SkillApplyResponseV6*,data::CombatResult*);
 const SkillAttackNativeServicesV6* debug;
};
struct SkillApplyActorV6 {
 std::uintptr_t identity;data::PropertyView* properties;BuffOwner* buffs;
 HitActor32* hit;const HitAttacker24* attacker_handle;const HitServices16* hit_services;
 const BuffDictionary16* dot_ids;const BuffDictionary16* dot_fx_ids;
 std::uint16_t* combo;std::uint8_t* invulnerable;std::uint8_t* push_death;
 const std::int32_t* network_id;
};
struct SkillApplyOutputV6 {HitResult40 hit;BuffResult24 dot;std::uint32_t calls,phase;std::int32_t status;std::uint32_t reserved;};
// Ordered F_ApplyResult for offline single-player nonplayer targets. Reached
// online, gold conversion, cooperative scaling and player reaction branches
// return -3 at the source prefix. Missing reached providers return -2. Debug,
// aggro, source FX, state, text, audio and AI deliveries never default success.
// HP/MP and DoT operate on these actual borrowed owners. Debug failures retain
// preceding combo/result/random/HP effects; backing must survive every call.
extern "C" int dh2_character_skill_apply_result_v6(SkillApplyOutputV6*,data::CombatResult*,
 SkillApplyActorV6*,SkillApplyActorV6*,const SkillApplyServicesV6*);
// Genuine RegenHP/RegenMP with captured source delta and Debug before Add.
extern "C" int dh2_character_skill_regen_v6(data::PropertyView*,std::uint32_t mp,
 std::int32_t amount,const SkillAttackNativeServicesV6*);
}
