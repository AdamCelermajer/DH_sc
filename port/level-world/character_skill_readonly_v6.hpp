#pragma once
#include "character_skill_native_v5.hpp"
namespace dh2::character::skills {
class CharacterSkillNativeReadOnlyBindingsV6 final {
 CharacterScriptSessionV3* session_=nullptr;
 const data::FreshInventoryOwnedV4& inventory_;
 NativeFsm24& fsm_;
 SkillManaOwnerV5 mana_;
 SkillManaServicesV5 mana_services_;
 SkillNativeWorldV5 world_;
 std::vector<target_search::Target24> heap_;
 target_search::List40 list_{};
 std::map<std::string,std::vector<target_search::Target24>> backups_;
 std::uint32_t character_filter_=1,object_filter_=0;
 bool initialized_=false;
 std::string error_;
 struct Binding {CharacterSkillNativeReadOnlyBindingsV6* owner;std::uint32_t address;};
 std::map<std::uint32_t,Binding> bindings_;
 static int invoke(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
 int call(std::uint32_t,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*);
 bool coherent()const noexcept;
public:
 CharacterSkillNativeReadOnlyBindingsV6(const data::FreshInventoryOwnedV4&,NativeFsm24&,
  const SkillManaServicesV5&,const SkillNativeWorldV5&,std::uint32_t capacity=1024);
 CharacterSkillNativeReadOnlyBindingsV6(const CharacterSkillNativeReadOnlyBindingsV6&)=delete;
 CharacterSkillNativeReadOnlyBindingsV6& operator=(const CharacterSkillNativeReadOnlyBindingsV6&)=delete;
 // Attach the actual retained session before it initializes native bindings.
 // Properties must be exactly the inventory's same shared PropertyState.
 int attach(CharacterScriptSessionV3&);
 static int binding(void*,std::uint32_t,dh2_script_function*,void**);
 // Ordinary gameplay_binding cannot push source objects. After VCB delivery,
 // install GetTargetListTop through THIS VM's existing object bridge provider.
 int install_object_binding();
 int stance(std::int32_t&);
 const std::string& error()const noexcept{return error_;}
 const target_search::List40& targets()const noexcept{return list_;}
 SkillManaOwnerV5& mana()noexcept{return mana_;}
};
}
