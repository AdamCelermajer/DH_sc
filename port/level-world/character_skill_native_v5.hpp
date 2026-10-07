#pragma once
#include "character_script_session_v3.hpp"
#include "character_target_search.hpp"
#include "character_skill_state_v4.hpp"
#include "../game-data/fresh_inventory_owned_v4.hpp"
#include <map>
namespace dh2::character::skills {
enum SkillManaServiceV5:std::uint32_t {mana_application_v5=1,mana_player_v5,
 mana_debug_contains_v5,mana_debug_load_v5,mana_debug_construct_v5,
 mana_debug_get_v5,mana_debug_destroy_v5};
struct SkillManaRequestV5 {std::uint32_t service,reserved;std::uintptr_t subject;const char* name;};
struct SkillManaResponseV5 {std::uint32_t word,reserved;std::uintptr_t identity;};
struct SkillManaServicesV5 {void* context;int(*invoke)(void*,const SkillManaRequestV5*,SkillManaResponseV5*);};
struct SkillManaOwnerV5 {std::uintptr_t character;data::PropertyView* properties;std::uint8_t byte14f0,reserved[7];};
// Raw native nonnegative mana costs; source negative assertion domain fails.
// Source return is out; 0 complete/-1 malformed/-2 required reached provider.
extern "C" int dh2_character_skill_mana_v5(std::uint32_t*,SkillManaOwnerV5*,
 std::uint32_t use,std::int32_t cost,const SkillManaServicesV5*);

// Same live room registry and genuine target query producers. TargetList owns
// its source results/backups, not a shadow World or invented target identities.
struct SkillNativeWorldV5 {
 target_search::Object48* character;
 const target_search::Registry8* registry;
 target_search::Services16 search;
 void* context;
 // Original controller Cmd_LookAt(object), missing delivery fails.
 int(*look_at)(void*,std::uintptr_t character,std::uintptr_t target);
 int(*resolve_object)(void*,std::uintptr_t,target_search::Object48**);
 const dh2_script_object_services* objects;
};
class CharacterSkillNativeBindingsV5 final {
 CharacterScriptSessionV3* session_=nullptr;
 data::FreshInventoryOwnedV4& inventory_;
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
 struct Binding {CharacterSkillNativeBindingsV5* owner;std::uint32_t address;};
 std::map<std::uint32_t,Binding> bindings_;
 static int invoke(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
 int call(std::uint32_t,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*);
 bool coherent()const noexcept;
public:
 CharacterSkillNativeBindingsV5(data::FreshInventoryOwnedV4&,NativeFsm24&,
  const SkillManaServicesV5&,const SkillNativeWorldV5&,std::uint32_t capacity=1024);
 CharacterSkillNativeBindingsV5(const CharacterSkillNativeBindingsV5&)=delete;
 CharacterSkillNativeBindingsV5& operator=(const CharacterSkillNativeBindingsV5&)=delete;
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
