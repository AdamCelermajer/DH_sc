#pragma once
#include "character_world_runtime_v1.hpp"
#include "character_skill_native_v6.hpp"
#include "../game-data/aggro.hpp"
namespace dh2::character::skills {
struct WorldSkillCombatBorrowV6 {
 SkillAttackActorV6* attack{};SkillApplyActorV6* application{};
 data::CombatActorState* life{};
 const data::FreshInventoryOwnedV4* inventory{};
 data::AggroTable* outgoing{};data::AggroTable* incoming{};
};
struct WorldSkillCombatRegistrationV6 {
 std::uintptr_t identity{};void* context{};
 int(*refresh)(void*,WorldSkillCombatBorrowV6*){};
};
struct WorldSkillCombatBackendsV6 {
 // Actual game/option, trophy, controller Kill and remote-update providers.
 // Missing reached services remain failures after the source HP prefix.
 HitServices16 hit{};
 // Actual FX/FSM/text/audio/AI/player services and game/option/party queries.
 SkillApplyServicesV6 application{};
 // OnAggro/OnDeAggro/target/controller continuations after real table writes.
 void* aggro_context{};
 int(*aggro_event)(void*,std::uint32_t,std::uintptr_t,std::uintptr_t){};
 const SkillCombatServicesV6* other{};
 //Optional whole native successor. Existing bounded V6 exports retain their
 //contracts; canonical offline player receivers use the recovered source tail.
 void* whole_application_context{};
 int(*whole_application)(void*,SkillApplyOutputV6*,data::CombatResult*,
  SkillApplyActorV6*,SkillApplyActorV6*,const SkillApplyServicesV6*){};
};
class CharacterWorldSkillCombatV6 {
 struct Entry {WorldSkillCombatRegistrationV6 registration;HitAttacker24 handle{};};
 CharacterWorldRuntimeV1& world_;const data::AiTables& ai_;
 const SkillAttackNativeServicesV6& debug_;WorldSkillCombatBackendsV6 backends_;
 std::list<Entry> actors_;HitServices16 hit_;SkillApplyServicesV6 application_;
 std::string error_;
 Entry* find(std::uintptr_t);
 int borrow(std::uintptr_t,WorldSkillCombatBorrowV6&);
 int player(std::uintptr_t,std::uintptr_t*);
 static int actor_service(void*,std::uintptr_t,SkillAttackActorV6**,SkillApplyActorV6**);
 static int handle_service(void*,std::uintptr_t,target_providers::Handle16**,target_providers::Registry24**);
 static int hit_service(void*,HitActor32*,const HitRequest32*,std::uintptr_t*);
 static int apply_service(void*,const SkillApplyRequestV6*,SkillApplyResponseV6*,data::CombatResult*);
 int threat(const SkillApplyRequestV6&,SkillApplyResponseV6&);
public:
 CharacterWorldSkillCombatV6(CharacterWorldRuntimeV1&,const data::AiTables&,
  const SkillAttackNativeServicesV6&,WorldSkillCombatBackendsV6);
 int add(const WorldSkillCombatRegistrationV6&);int remove(std::uintptr_t);void clear(){actors_.clear();}
 SkillCombatWorldV6 native_world()noexcept;
 const SkillApplyServicesV6& application_services()const noexcept{return application_;}
 const HitServices16& hit_services()const noexcept{return hit_;}
 // Direct genuine ordered application, preserving the output/status prefix.
 int apply(SkillApplyOutputV6*,data::CombatResult*,std::uintptr_t attacker,std::uintptr_t target);
 const std::string& error()const noexcept{return error_;}
};
}
