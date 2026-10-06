#pragma once
#include <cstdint>
namespace dh2::character {
struct AttackState64 {
 // Live projection: AI owner+4,target+40,last_target+44; owner OOI+14a4.
 std::uintptr_t owner,target,last_target,object_of_interest;
 // owner+528/+1b5 and AI+78/+79/+74; signed owner OOI byte+14a8.
 std::uint32_t owner_flags528,heading_active,continued,last;
 std::int32_t index,object_of_interest_type;
 std::uint32_t finisher,seeking;
};
struct ControllerAttackState32 {
 std::uintptr_t controllable,character;
 std::uint32_t blocked,locked,forced,network_enabled;
};
// Fresh, empty, borrowed ordered records. Search/pop update this same view.
struct AttackTargetList24 {
 std::uintptr_t token;
 const std::uintptr_t* entries;
 std::uint32_t count,cursor;
};
enum AttackService : std::uint32_t {
 attack_owner_dead=0,attack_owner_ranged,attack_is_attacking,
 attack_range_redirect,attack_diagnostic,attack_list_create,
 attack_list_reset_sort,attack_frontal_angle,attack_list_search,
 attack_list_pop,attack_list_destroy,attack_set_target,
 attack_sync_last_target,attack_can_attack_current,attack_target_dead,
 attack_owner_player,attack_current_in_melee,attack_set_attack_state,
 attack_network_mode,attack_network_send,attack_controllable_dispatch
};
struct AttackRequest32 {
 std::uint32_t service,argument0,argument1,reserved;
 std::uintptr_t subject,payload;
};
struct AttackResponse16 {std::uint32_t word,reserved;std::uintptr_t identity;};
struct AttackServices16 {
 void* context;
 // Synchronous source boundaries. State/Controller may be refreshed by calls.
 // Boolean queries supply genuine predicates (IsAttacking returns 0 or 1).
 // All projected owner fields must follow live owner changes made by services.
 // list_create returns an AttackTargetList24*. list_search/pop mutate its view.
 // network_send owns the source queue/packet block and its real object-ID reads.
 // controllable_dispatch owns virtual dispatch; Character calls melee(false).
 void(*invoke)(void*,AttackState64*,ControllerAttackState32*,const AttackRequest32*,AttackResponse16*);
};
static_assert(sizeof(void*)==8&&sizeof(AttackState64)==64&&sizeof(ControllerAttackState32)==32);
static_assert(sizeof(AttackTargetList24)==24&&sizeof(AttackRequest32)==32&&sizeof(AttackResponse16)==16&&sizeof(AttackServices16)==16);
}
// 0 completed source flow, 1 malformed entry before effects, 2 invalid provider
// view/progress after effects. Backend identities/lifetimes are caller-owned.
// No attack-index, last/finisher reset, range acceptance, search or FSM is invented.
extern "C" int dh2_character_ai_melee_attack(dh2::character::AttackState64*,std::uintptr_t,
 std::uint32_t,const dh2::character::AttackServices16*);
extern "C" int dh2_character_cmd_attack(dh2::character::ControllerAttackState32*,
 dh2::character::AttackState64*,std::uintptr_t,const dh2::character::AttackServices16*);
