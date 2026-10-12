#pragma once
#include "character_design_services.hpp"
#include "character_game_design.hpp"
#include "character_target_bindings.hpp"
#include "../game-data/combat_application.hpp"
#include "../script-runtime/script_object_bridge.h"
#include <array>
#include <map>
#include <memory>
#include <string>
namespace dh2::character {
// A native scene lifetime, not an overlay of the original SceneManager or its
// handle map. The renderer and scripts share actual gameplay property/life
// backing. Position is the raw GameObject point used while no cached node exists.
struct ScriptCharacterObject {
 const std::uintptr_t identity;
 const std::string name;
 std::shared_ptr<data::PropertyState> properties;
 std::shared_ptr<data::CombatActorState> life;
 std::array<float,3> position{};
 TargetOwner16 owner{};
 TargetState48 target{};
 TargetBindings48 binding{};
 ScriptCharacterObject(std::uintptr_t,std::string,std::shared_ptr<data::PropertyState>,
  std::shared_ptr<data::CombatActorState>,const std::array<float,3>&);
 ScriptCharacterObject(const ScriptCharacterObject&)=delete;
 ScriptCharacterObject& operator=(const ScriptCharacterObject&)=delete;
};
// Owns retained object records until the world and all its VMs close. This native
// identity lookup supplies object service lifetimes; it does not stand in for
// source FindObject/GetHandle, hostility, discovery, or deletion policy.
class CharacterScriptObjects {
 CharacterGameDesign::Borrow design_;
 DebugSwitches* debug_;
 const DebugFileServices24* files_;
 std::vector<std::int32_t> ai_types_;
 std::map<std::uintptr_t,std::shared_ptr<ScriptCharacterObject>> records_;
 dh2_script_object_services services_{this,type,methods,invoke};
 static int type(void*,std::uintptr_t,const char**);
 static int methods(void*,std::uintptr_t,const dh2_script_object_method**,std::uint32_t*);
 static int invoke(void*,const dh2_script_callback_scope*,std::uintptr_t,std::uint32_t,
  const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
 static int target(void*,TargetState48*,const TargetRequest24*,std::uint32_t*);
public:
 CharacterScriptObjects(CharacterGameDesign::Borrow&&,DebugSwitches*,const DebugFileServices24*);
 CharacterScriptObjects(const CharacterScriptObjects&)=delete;
 CharacterScriptObjects& operator=(const CharacterScriptObjects&)=delete;
 std::shared_ptr<ScriptCharacterObject> add(std::uintptr_t,const std::string&,
  std::shared_ptr<data::PropertyState>,std::shared_ptr<data::CombatActorState>,const std::array<float,3>&);
 std::shared_ptr<ScriptCharacterObject> find(std::uintptr_t) const noexcept;
 const dh2_script_object_services& services() const noexcept{return services_;}
};
}
