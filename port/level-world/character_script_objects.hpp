#pragma once
#include "character_design_services.hpp"
#include "character_game_design.hpp"
#include "character_target_bindings.hpp"
#include "../game-data/combat_application.hpp"
#include "../game-data/loot_tables_v2.hpp"
#include "../script-runtime/script_object_bridge.h"
#include <array>
#include <functional>
#include <map>
#include <memory>
#include <string>
namespace dh2::character {
// A native scene lifetime, not an overlay of the original SceneManager or its
// handle map. The renderer and scripts share actual gameplay property/life
// backing. Position is the raw GameObject point used while no cached node exists.
struct ScriptCharacterObject {
 const std::uintptr_t identity;
 // ObjectBase.SetName mutates the SAME published receiver during removal.
 // Its identity stays fixed; scripts continue borrowing this name storage.
 std::string name;
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
 // The source Application's one process Random channel 0. Shared by every
 // Character receiver and pinned through an aliasing lease to its real owner.
 std::shared_ptr<data::LootRandom8V2> source_random_channel0_v125_;
 std::function<bool(bool&,std::string&)> online_byte5_v125_;
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
 // Modern Lua facade publication of the SAME canonical C1 object. No new
 // Character/target/properties/life or source ObjectManager Add is performed.
 bool publish_existing_v62(const std::shared_ptr<ScriptCharacterObject>&,std::string&);
 // Host identity loan retirement after the SAME native Character/VM D0 and
 // ObjectManager unpublication. Caller supplies its exact captured receiver.
 bool retire_native_receiver_v123(std::uintptr_t,const std::shared_ptr<ScriptCharacterObject>&,std::string&);
 // Bind the exact App-owned channel-0 RNG and its GetOnline byte5 query once.
 // Online _Rand needs the original ReturnValues+0xfc seed, so invoke rejects
 // online calls until that separate receiver is reconstructed.
 bool bind_source_random_channel0_v125(std::shared_ptr<data::LootRandom8V2>,
  std::function<bool(bool&,std::string&)>,std::string&);
 const dh2_script_object_services& services() const noexcept{return services_;}
};
}
