#pragma once
#include "character_world_npc_init_final_v1.hpp"
#include "character_world_npc_scene_v1.hpp"
#include "character_world_npc_physical_v1.hpp"
#include "character_ai_events.hpp"
#include "actor_rotation.hpp"
#include <algorithm>
namespace dh2::character {
// Include outside the renderer's namespace. The bridge borrows every actor and
// global owner; only the scene/PF dispatch adapters are retained here.
class CharacterWorldNpcSceneBridgeV1 {
 skills::CharacterWorldRuntimeV1& world_;CharacterScriptSession& script_;
 ScriptCharacterObject& object_;AIEventState64& ai_;const data::AiTables& tables_;
 CharacterWorldNpcPhysicalV1& physical_;
 CharacterWorldNpcObjectV1& lifecycle_;WorldNpcInitFinalFieldsV1& fields_;
 WorldNpcSpawnGlobalsV1& random_;WorldNpcNetworkModeV1& network_;
 actor::RotationState& rotation_;
 visual::SceneBinding* visual_owner_;scene::Scene* scene_owner_;
 CharacterWorldNpcSceneV1 scene_;CharacterWorldNpcInitFinalV1 final_;
 bool received_{};std::string precondition_error_;
 static auto& self(void* p){return *static_cast<CharacterWorldNpcSceneBridgeV1*>(p);}
 static bool player(void* p,bool& value,std::string& error){auto& s=self(p);return character_npc_actor_player_v1(s.world_,s.object_.identity,value,error);}
 static bool spawn_player(void* p,bool& value,std::string& error){auto& s=self(p);return character_npc_spawn_handle_player_v1(s.world_,s.object_.identity,value,error);}
 static bool network(void* p,bool& value,std::string&){value=self(p).network_.byte5!=0;return true;}
 static bool spawn(void* p,std::int32_t& value,std::string& error){auto& s=self(p);WorldNpcSpawnServicesV1 services{};services.context=&s;services.handle_is_player=spawn_player;services.network_enabled=network;
  return character_check_spawn_probability_v1(s.random_,s.fields_.cached270,s.fields_.threshold274,s.fields_.deleted82,s.lifecycle_,services,value,error);}
 static bool visual(void* p){return self(p).visual_owner_!=nullptr;}
 static bool scene(void* p){return self(p).scene_owner_!=nullptr;}
 static bool sync(void* p,std::string& error){auto& s=self(p);const auto& v=s.physical_.projection().visual;return s.scene_.sync(s.object_.position.data(),s.rotation_.rotation,v.effective_scale,error);}
 static bool light(void* p,std::uint32_t value,std::string&){self(p).scene_.assign_light(value);return true;}
 static bool find(void* p,const char* name,std::uintptr_t& value,std::string& error){return self(p).scene_.find_node(name,value,error);}
 static bool type(void* p,std::int32_t& value,std::string& error){auto& s=self(p);auto id=s.script_.property_view().resolved[1];if(id<0||std::size_t(id)>=s.tables_.rows.size())id=8;if(s.tables_.rows.size()<=8){error="required actual AI table rows unavailable";return false;}value=s.tables_.rows[id].type;return true;}
 static bool ai(void* p,std::string& error){auto& s=self(p);return character_npc_external_init_final_v1(s.script_,s.ai_.active,error);}
 WorldNpcInitFinalServicesV1 services(){WorldNpcInitFinalServicesV1 s{};s.context=this;s.check_spawn=spawn;s.has_visual=visual;s.sync_visual=sync;s.assign_light=light;s.has_scene=scene;s.find_target_node=find;s.char_type=type;s.is_player=player;s.ai_init_final=ai;return s;}
public:
 CharacterWorldNpcSceneBridgeV1(skills::CharacterWorldRuntimeV1& world,CharacterScriptSession& script,
  ScriptCharacterObject& object,AIEventState64& ai,const data::AiTables& tables,
  CharacterWorldNpcPhysicalV1& physical,WorldNpcObjectFieldsV1& object_fields,
  CharacterWorldNpcObjectV1& lifecycle,WorldNpcInitFinalFieldsV1& fields,
  WorldNpcSpawnGlobalsV1& random,WorldNpcNetworkModeV1& network,
  visual::SceneBinding& visual_owner,scene::Scene& scene_owner,std::uint32_t& light40,
  std::uintptr_t& node180,actor::RotationState& rotation,WorldNpcSceneServicesV1 scene_services)
  :world_(world),script_(script),object_(object),ai_(ai),tables_(tables),physical_(physical),
   lifecycle_(lifecycle),fields_(fields),random_(random),network_(network),rotation_(rotation),
   visual_owner_(&visual_owner),scene_owner_(&scene_owner),scene_(visual_owner,scene_owner,light40,scene_services),
   final_(object_fields,lifecycle,fields.once1395,fields.threshold274,fields.pf_debug_name254,node180,services()){}
 CharacterWorldNpcSceneBridgeV1(const CharacterWorldNpcSceneBridgeV1&)=delete;
 bool check_spawn(std::int32_t& value,std::string& error){return spawn(this,value,error);}
 bool receive_init_post(std::string& error){if(physical_.phase()!=4){error="required completed source NPC physical initialization unavailable";return false;}const auto& projection=physical_.projection();std::copy_n(projection.visual.rotation_radians,3,rotation_.rotation);rotation_.heading_angle=rotation_.rotation[2];received_=scene_.receive_init_post(projection,object_.position.data(),error);return received_;}
 bool initialize(const char* name,const std::string& light_set){precondition_error_.clear();if(physical_.phase()!=4||!received_){precondition_error_="required actual InitPost scene/physical producers unavailable";return false;}return final_.initialize(name,light_set,object_.position.data(),physical_.projection().bounds.absolute_box);}
 const std::string& error()const noexcept{return precondition_error_.empty()?final_.error():precondition_error_;}
 bool node_position(std::uintptr_t node,float* xyz,std::string& error)const{return scene_.node_position(node,xyz,error);}
 const actor::RotationState& rotation()const noexcept{return rotation_;}
};
}
