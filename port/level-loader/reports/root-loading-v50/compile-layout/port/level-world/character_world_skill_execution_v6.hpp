#pragma once
#include "character_world_skill_combat_v6.hpp"
#include "character_world_npc_state_owner_v1.hpp"
#include "trophy_manager_owner_v1.hpp"
#include <map>
namespace dh2::character::skills {
struct WorldSkillExecutionActorV6 {
 std::uintptr_t identity{};data::PropertyView* properties{};data::CombatActorState* life{};
 const data::CombatantView* facts{};State* machine{};
 CharacterConstructorCombatFieldsV1* fields{};
 const data::FreshInventoryOwnedV4* inventory{};BuffOwner* buffs{};
 const BuffDictionary16* dot_ids{};const BuffDictionary16* dot_fx_ids{};
 // Borrow the actual Character controller pointer field. Missing producer
 // leaves Hit unavailable; zero from a genuine field remains source null.
 const std::uintptr_t* controller{};
 data::AggroTable* outgoing{};data::AggroTable* incoming{};
};
struct WorldSkillExecutionRegistrationV6 {std::uintptr_t identity{};void* context{};int(*refresh)(void*,WorldSkillExecutionActorV6*){};};
class CharacterWorldSkillExecutionV6 {
 struct Entry {WorldSkillExecutionRegistrationV6 registration;WorldSkillExecutionActorV6 borrow{};SkillAttackActorV6 attack{};SkillApplyActorV6 application{};HitActor32 hit{};std::int32_t lifecycle_before{};};
 struct Binding {dh2_script_function function{};void* context{};CharacterWorldSkillExecutionV6* owner{};};
 CharacterWorldRuntimeV1& world_;DebugSwitches& switches_;const DebugFileServices24& debug_files_;
 trophies::TrophyNativeBindingsV1* trophies_;WorldSkillCombatBackendsV6 backends_;
 std::map<std::uintptr_t,std::string> strings_;std::uintptr_t string_serial_{};
 SkillAttackNativeServicesV6 debug_;std::unique_ptr<CharacterWorldSkillCombatV6> combat_;
 std::unique_ptr<CharacterSkillNativeBindingsV6> native_;std::list<Entry> actors_;std::map<std::uint32_t,Binding> bindings_;std::string error_;
 static int debug_service(void*,const SkillAttackNativeRequestV6*,std::uintptr_t*);
 static int hit_service(void*,HitActor32*,const HitRequest32*,std::uintptr_t*);
 static int application_service(void*,const SkillApplyRequestV6*,SkillApplyResponseV6*,data::CombatResult*);
 static int refresh_entry(void*,WorldSkillCombatBorrowV6*);
 static int invoke(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
public:
 // One source mutation bridge over the SAME readonly target/mana binding,
 // world, FSM/Gear, shared CF context and shared combat RNG. Owns no actor,
 // property, life, alternate world or private random generator.
 CharacterWorldSkillExecutionV6(CharacterWorldRuntimeV1&,const data::AiTables&,
  CharacterSkillNativeReadOnlyBindingsV6&,const data::FreshInventoryOwnedV4&,NativeFsm24&,data::SkillTables::Borrow,
  DotCombatContext32&,data::CombatRandom&,DebugSwitches&,const DebugFileServices24&,
  trophies::TrophyNativeBindingsV1*,WorldSkillCombatBackendsV6);
 int add(const WorldSkillExecutionRegistrationV6&);int remove(std::uintptr_t);
 int attach(CharacterScriptSessionV3&);
 static int binding(void*,std::uint32_t,dh2_script_function*,void**);
 // Compatibility life scalars are mirrors of exact actor/FSM halfword/byte
 // fields after actual source writes. Old kernels must synchronize their
 // actual scalar writes into the source fields before the next execution.
 // Never import presentation state strings.
 void publish_compatibility_fields()noexcept;
 const std::string& error()const noexcept{return error_;}
 CharacterWorldSkillCombatV6& combat()noexcept{return *combat_;}
};
}
