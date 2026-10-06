#pragma once
#include "character_ai_attack.hpp"
#include <string>
namespace dh2::character {
enum class RangedAttackOperationV41 { owner_dead,range_parameters,is_attacking,melee_fallback,debug,list_create,can_attack_current,target_dead,owner_player,list_frontal_sort,frontal_angle,list_search,set_target,look_at,list_destroy,set_attack_state };
struct RangedAttackRequestV41 {RangedAttackOperationV41 operation{};std::uintptr_t subject{},other{};std::uint32_t speculative{};float radius{},cone{};const char* name{};};
struct RangedAttackResponseV41 {std::uint32_t word{};std::int32_t parameters[3]{};AttackTargetList24* list{};};
struct RangedAttackServicesV41 {void* context{};int(*invoke)(void*,AttackState64&,const RangedAttackRequestV41&,RangedAttackResponseV41&,std::string&){};};
// Whole AI_DoRangeAttack3d076c. Borrow SAME fields; callbacks may refresh the
// live owner/target/heading. Last79 is read only on actual IsAttacking=true.
// No projectile is spawned here: original authored OnAnimEvent owns that stage.
int character_ranged_attack_v41(AttackState64&,std::uintptr_t requested,std::uint32_t speculative,const RangedAttackServicesV41&,std::string&);
}
