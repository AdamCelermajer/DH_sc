#pragma once
#include "character_skill_combat_v6.hpp"
#include "character_skill_readonly_v6.hpp"
#include "character_target_providers.hpp"
namespace dh2::character::skills {
struct SkillCombatWorldV6 {
 void* context;
 // Pure borrows from the actual world object's GetHandle backing and genuine
 // shared registry. No new handle registry is owned by this binding factory.
 int(*handle)(void*,std::uintptr_t,target_providers::Handle16**,target_providers::Registry24**);
 target_providers::Services16 classify;
 // Refresh actual same-identity live equipment/state/property projections.
 int(*actor)(void*,std::uintptr_t,SkillAttackActorV6**,SkillApplyActorV6**);
 // Required source GetKind/target kind8 activation, when non-Character reached.
 const SkillCombatServicesV6* other;
};
class CharacterSkillNativeBindingsV6 final {
 CharacterSkillNativeReadOnlyBindingsV6& previous_;const data::FreshInventoryOwnedV4& inventory_;
 NativeFsm24& fsm_;data::SkillTables::Borrow tables_;SkillCombatWorldV6 world_;
 DotCombatContext32& context_;data::CombatRandom& random_;
 const SkillAttackNativeServicesV6& debug_;const SkillApplyServicesV6& application_;
 CharacterScriptSessionV3* session_=nullptr;std::vector<SkillCombatRowV6> rows_;
 bool busy_=false;std::string error_;
 bool coherent()const noexcept;
 static int invoke(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
public:
 // Borrow all authorities through the retained Session's VM finalizers. V5
 // retains target list/search and mana; V6 intercepts only _SkillCombatRoll.
 CharacterSkillNativeBindingsV6(CharacterSkillNativeReadOnlyBindingsV6&,const data::FreshInventoryOwnedV4&,
  NativeFsm24&,data::SkillTables::Borrow,const SkillCombatWorldV6&,
  DotCombatContext32&,data::CombatRandom&,const SkillAttackNativeServicesV6&,const SkillApplyServicesV6&);
 int attach(CharacterScriptSessionV3&);
 static int binding(void*,std::uint32_t,dh2_script_function*,void**);
 const std::string& error()const noexcept{return error_;}
};
}
