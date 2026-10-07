#pragma once
#include "character_ranged_attack_v41.hpp"
#include "character_world_attack_geometry_v1.hpp"
#include "target_frontal_sort_v41.hpp"
namespace dh2::character::skills {
struct WorldRangedAttackBorrowV41 {
 AttackState64* fields{};TargetState48* target{};State* machine{};
 // SAME GameObject heading flag logical projection; no byte type-punning.
 const std::uint32_t* heading_active1b5{};
 std::shared_ptr<void> lifetime;
 const TargetServices16* target_services{};
};
struct WorldRangedAttackBackendsV41 {
 void* context{};
 int(*inventory)(void*,std::uintptr_t,WorldAttackInventoryBorrowV1*){};
 WorldAIAttackServicesV1 queries{};
 int(*frontal_angle)(void*,std::int32_t*,std::string&){};
 int(*look_at)(void*,std::uintptr_t owner,std::uintptr_t target,std::string&){};
 int(*set_attack_state)(void*,std::uintptr_t,std::uintptr_t,bool,std::string&){};
 // The source fallback is a nested SAME-AI melee invocation, not another
 // user command. Required if capability changes after outer ranged predicate.
 int(*melee_fallback)(void*,std::uintptr_t,std::uintptr_t,std::uint32_t,std::string&){};
};
class CharacterWorldRangedAttackV41 {
 CharacterWorldRuntimeV1& world_;CharacterWorldAttackGeometryV1& geometry_;
 DebugSwitches& debug_;const DebugFileServices24& files_;
 WorldRangedAttackBorrowV41 b_;WorldRangedAttackBackendsV41 s_;
 std::vector<target_search::Target24> heap_;std::vector<std::uintptr_t> ordered_;
 target_search::List40 list_{};AttackTargetList24 view_{};std::string error_;
 bool refresh();bool parameters(std::int32_t[3],bool&);
 static int search(void*,const target_search::Request24*,target_search::Response16*);
 static int invoke(void*,AttackState64&,const RangedAttackRequestV41&,RangedAttackResponseV41&,std::string&);
 int call(const RangedAttackRequestV41&,RangedAttackResponseV41&);
public:
 CharacterWorldRangedAttackV41(CharacterWorldRuntimeV1&,CharacterWorldAttackGeometryV1&,
  DebugSwitches&,const DebugFileServices24&,WorldRangedAttackBorrowV41,
  WorldRangedAttackBackendsV41,std::uint32_t capacity=256);
 int attack(std::uintptr_t requested,std::uint32_t speculative=0);
 const std::string& error()const noexcept{return error_;}
};
}
