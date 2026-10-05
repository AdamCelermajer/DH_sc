#pragma once
#include "character_ai_attack.hpp"
#include "character_target_bindings.hpp"
#include "character_world_runtime_v1.hpp"
#include "character_world_ai_can_attack_v1.hpp"
#include <vector>
namespace dh2::character::skills {
struct PlayerAttackBackendsV1 {
 void* context{};
 // Required reached source queries, diagnostics, ranged/network and FSM bodies.
 // Zero means the requested source operation completed, not predicate false.
 int(*invoke)(void*,const AttackRequest32*,AttackResponse16*){};
 WorldAIAttackServicesV1 queries{};
 bool(*melee_radius)(void*,std::uintptr_t,float&,std::string&){};
};
// Borrows sole source AI attack fields, same target authority, controller and
// State. target/last_target/flags in AttackState64 are refreshed projections;
// continued/last/index/finisher/seeking/OOI are the actual supplemental fields.
class CharacterWorldPlayerAttackOwnerV1 {
 CharacterWorldRuntimeV1& world_;
 AttackState64& fields_;
 TargetState48& target_;
 ControllerAttackState32& controller_;
 State& machine_;
 TargetServices16 target_services_;
 PlayerAttackBackendsV1 backends_;
 std::vector<target_search::Target24> heap_;
 std::vector<std::uintptr_t> ordered_;
 target_search::List40 list_{};
 AttackTargetList24 view_{};
 std::string error_;
 bool active_{};
 static void invoke(void*,AttackState64*,ControllerAttackState32*,const AttackRequest32*,AttackResponse16*);
 static bool query(void*,std::uintptr_t,std::uintptr_t,WorldAIAttackQueryV1,std::int32_t&,std::string&);
 static int search_service(void*,const target_search::Request24*,target_search::Response16*);
 void call(AttackState64&,const AttackRequest32&,AttackResponse16&);
 void refresh(AttackState64&);
 void publish(const AttackState64&);
 void fail(const char*,std::uint32_t);
 bool valid_binding();
public:
 CharacterWorldPlayerAttackOwnerV1(CharacterWorldRuntimeV1&,AttackState64&,
  TargetState48&,ControllerAttackState32&,State&,TargetServices16,
  PlayerAttackBackendsV1,std::uint32_t capacity=4096);
 // Genuine controller command including speculative network prefix if reached.
 int command(std::uintptr_t requested=0);
 int melee(std::uintptr_t requested=0,std::uint32_t speculative=0);
 const std::string& error()const noexcept{return error_;}
};
}
