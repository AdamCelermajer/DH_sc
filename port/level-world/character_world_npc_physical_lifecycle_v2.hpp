#pragma once
#include "character_world_physical_peers_v1.hpp"
#include "character_world_npc_scene_bridge_v1.inc"
#include "actor_runtime.hpp"
namespace dh2::character {
struct WorldNpcPhysicalGraphBorrowV2 {
 skills::CharacterWorldRuntimeV1& world;physical::NativeWorld& physical_world;
 CharacterWorldPhysicalPeersV1& peers;physical::CharacterNpcBodyModel& model;
 CharacterWorldNpcStateOwnerV1& machine;CharacterScriptSession& script;
 ScriptCharacterObject& object;AIEventState64& ai;ControllerCommandState32& controller;
 WorldNpcAISCollisionFieldsV1& collision_fields;WorldNpcCollisionGlobalsV1& collisions;
 WorldNpcObjectFieldsV1& object_fields;actor::RuntimeState& runtime;
 const navigation::CollisionWorld& floors;navigation::ObstacleRegistry& obstacles;
 const data::AiTables& ai_tables;WorldNpcInitFinalFieldsV1& final_fields;
 WorldNpcSpawnGlobalsV1& random;WorldNpcNetworkModeV1& network;
 visual::SceneBinding& visual;scene::Scene& scene;
 std::uint32_t& light40;std::uintptr_t& target_node180;
 std::uint8_t& source_flat2f9;
 DebugSwitches& debug;const DebugFileServices24& debug_files;
};
struct WorldNpcPhysicalGraphServicesV2 {
 WorldNpcCollisionServicesV1 collision;
 WorldNpcSceneServicesV1 scene;
 void* context{};
 // Real visibility sync and whole Character::Stop. Filter operations below
 // are provided by the actual owned physical object, never delegated stubs.
 int(*object_method)(void*,WorldNpcObjectMethodV1,std::string&){};
};
class CharacterWorldNpcPhysicalLifecycleV2 {
 WorldNpcPhysicalGraphBorrowV2 borrow_;WorldNpcPhysicalGraphServicesV2 services_;
 CharacterWorldNpcCollisionV1 collision_;CharacterWorldNpcObjectV1 lifecycle_;
 CharacterWorldNpcPhysicalV1 physical_;CharacterWorldNpcSceneBridgeV1 scene_;
 bool registered_{},initialized_{};std::string error_;
 static bool has_visual(void*);static physical::NativeBody* physical(void*);
 static int method(void*,WorldNpcObjectMethodV1,std::string&);
 static bool aabb(void*,float*,std::string&);
 static bool peer(void*,void*,std::uintptr_t&,std::string&);
 static bool enabled(void*,std::uintptr_t,std::uint8_t&,std::string&);
 static int update_pf(void*,std::uintptr_t,const physical::NativeBody*,const physical::NpcBodyProjection*,std::string&);
 static bool debug(void*,const char*,bool&,std::string&);
 static bool clock(void*,std::uint32_t&,std::uint32_t&,std::string&);
 static int ais(void*,std::uintptr_t,std::uintptr_t,std::uintptr_t,std::uint32_t,std::string&);
 static int cancel(void*,std::uintptr_t,std::string&);
 static bool type(void*,std::uintptr_t,std::uint32_t&,std::string&);
public:
 CharacterWorldNpcPhysicalLifecycleV2(WorldNpcPhysicalGraphBorrowV2,WorldNpcPhysicalGraphServicesV2);
 ~CharacterWorldNpcPhysicalLifecycleV2();
 CharacterWorldNpcPhysicalLifecycleV2(const CharacterWorldNpcPhysicalLifecycleV2&)=delete;
 // Caller already created the genuine source FSM/VM/controller, populated
 // same properties/life and visible defaults, and registered canonical World.
 // Executes InitPhysical -> retained InitPost scene/bounds -> whole supported
 // InitFinal. Failures preserve reached body/PF/VM prefixes for explicit close.
 bool initialize(physical::NpcBodyRequest,const char* name,const std::string& light_set);
 // Scene/worldStep remain caller phases. This is the existing complete actor
 // physical/path/rotation/subobject phase, using SAME runtime/body/PF/flags.
 bool actor_phase(actor::RuntimeResult&,actor::RuntimeRequest);
 bool enabled();bool disabled();
 // Destroy physical body with collision receivers and World identities live;
 // update/remove PF obstacle, then unregister physical context. Canonical
 // World/VM/controller destruction must occur afterward.
 bool close();
 CharacterWorldNpcPhysicalV1& physical_owner()noexcept{return physical_;}
 CharacterWorldNpcObjectV1& object_owner()noexcept{return lifecycle_;}
 const std::string& error()const noexcept{return error_;}
};
}
