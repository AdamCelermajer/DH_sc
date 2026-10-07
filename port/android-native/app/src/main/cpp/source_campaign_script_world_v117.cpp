#include "source_campaign_script_world_v117.hpp"
#include "model_renderer.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_fx_v77.hpp"
#include "renderer_character_campaign_v62.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <canonical_character_spawn_select_v87.hpp>
#include <world_loot_gameplay_v23.hpp>
#include <canonical_object_manager_v1.hpp>
#include <production_noncharacter_composition_v67.hpp>
#include <script_command_receivers_v59.hpp>
#include <application_player_manager_bootstrap_v59.hpp>
#include <player_save_difficulty_global_v29.hpp>
#include <cstring>
namespace model_renderer {namespace {
bool required(const char* leaf,std::string& error){
 error=std::string("Required campaign world script ")+leaf;return false;
}
bool trace(const std::shared_ptr<SourceWorldBorrowV61>& world,std::string& error){
 if(!world||!world->debug||!world->debug_files)return required("actual Debug/file owner",error);
 if(dh2_character_debug_load(world->debug.get(),world->debug_files)!=1)
  return required("Debug.load",error);
 std::uint32_t ignored{};
 if(dh2_character_debug_get(&ignored,world->debug.get(),"isTracingScriptCmd",world->debug_files)!=1)
  return required("Debug.GetSwitch",error);
 return true;
}
bool word(const dh2::loader::CheckedCommandBorrowV59& command,std::uint32_t offset,
 std::int32_t& value,std::string& error){
 const auto* cell=command.actual_data->scalar(offset);
 if(!cell||cell->width!=4)return required("retained command Data signed word",error);
 std::memcpy(&value,&cell->bits,sizeof(value));return true;
}
bool object(const SourceCampaignCandidateBorrowV55& candidate,const char* name,std::int32_t module,
 const dh2::world::CanonicalObjectBorrowV1*& value,std::string& error){
 if(!candidate.objects||!name)return required("ObjectManager/name",error);
 dh2::target_providers::Handle16 handle{};
 return candidate.objects->by_name(name,module,false,nullptr,handle,error)&&
  candidate.objects->resolve_handle_v4(handle,false,value,{},error);
}
}
bool execute_source_campaign_script_world_v117(const SourceCampaignCandidateBorrowV55& candidate,
 const dh2::loader::CheckedCommandBorrowV59& command,bool skip,std::int32_t module,
 bool& handled,std::string& error){
 handled=false;
 if(!command.kind8||!command.actual_data)return required("canonical command receiver/Data",error);
 const auto kind=*command.kind8;
 if(kind!=16&&kind!=17&&kind!=20&&kind!=21&&kind!=30&&kind!=48&&kind!=60&&kind!=61&&kind!=62)return true;
 handled=true;
 // Original PlayEffect alone skips before Debug or object lookup. StopEffect
 // does not trace; SpawnCharacter/Container ignore skip.
 if(kind==20&&skip){error.clear();return true;}
 std::shared_ptr<SourceWorldBorrowV61> world;
 if(!borrow_source_campaign_condition_world_v70(candidate,world,error))return false;
 if(kind!=21&&!trace(world,error))return false;
 if(kind==16||kind==17)return set_campaign_safe_zone_music_v101(candidate.actual_world,kind==16,error);
 if(kind==61||kind==62){
  auto pm=candidate.application?candidate.application->source_player_manager_v59():nullptr;
  dh2::player::PlayerInfoFieldsV1* player{};
  if(!pm||!pm->get_local_player(0,true,player,error)||!player)
   return required("local PlayerInfo for saved area state",error);
  if(!player->character660){error.clear();return true;}
  SourceCampaignCharacterBorrowV62 actor;
  if(!borrow_source_campaign_character_v62(candidate.actual_world,player->character660,actor,error)||!actor.character)
   return required("same local Character saved state",error);
  auto& record=*actor.character;
  if(!record.save_fields||!record.save_fields->save_slot14e8())return required("Character Save14e8 cell",error);
  if(!*record.save_fields->save_slot14e8()){error.clear();return true;}
  if(!record.save||*record.save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(record.save.get())||
     record.save->character()!=player->character660||!record.services.difficulty_global)
   return required("retained selected Save and process difficulty",error);
  std::int32_t index{},state{};
  if(!word(command,8,index,error)||!word(command,12,state,error))return false;
  const auto tier=record.services.difficulty_global->value();
  return kind==61?record.save->set_level_state_v117(index,state,tier,error):
   record.save->set_map_state_v117(index,state,tier,error);
 }
 if(kind==20||kind==21){
  std::int32_t set{};
  const dh2::world::CanonicalObjectBorrowV1* anchor{};
  if(kind==20&&!object(candidate,command.actual_data->cstring(0x20),module,anchor,error))return false;
  if(!word(command,8,set,error))return false;
  std::shared_ptr<void> manager_owner;dh2::fx::CharacterMeshFxOwnerV4* manager{};
  if(!borrow_source_campaign_fx_v77(candidate.actual_world,manager_owner,manager,error)||!manager)
   return required("actual VisualFXManager",error);
  if(kind==21)return manager->drop_set_by_id_v117(set,error);
  float position[3]{};
  for(std::uint32_t axis=0;axis<3;++axis){
   std::int32_t offset{};if(!word(command,0x10+axis*4,offset,error))return false;
   position[axis]=static_cast<float>(offset);
  }
  return manager->play_set(set,position,nullptr,anchor?anchor->identity:0,nullptr,error);
 }
 const dh2::world::CanonicalObjectBorrowV1* target{};
 if(!object(candidate,command.actual_data->cstring(0x0c),module,target,error))return false;
 if(kind==48){
  std::uintptr_t character{};
  if(target&&(!target->as_character||!target->as_character(target->context,character,error)))return false;
  const dh2::world::CanonicalObjectBorrowV1* scatter_target{};
  if(!object(candidate,command.actual_data->cstring(0x14),module,scatter_target,error))return false;
  if(!character){error.clear();return true;}
  if(!world->prepared_items_v88||!world->prepared_items_v88->ready()||!world->prepared_items_v88->loot())
   return required("same prepared Item/loot pool",error);
  return world->prepared_items_v88->loot()->drop_character_v117(character,scatter_target?scatter_target->identity:0,error);
 }
 if(!target){error.clear();return true;}
 if(kind==30){
  std::uintptr_t character{};
  if(!target->as_character||!target->as_character(target->context,character,error))return false;
  if(!character){error.clear();return true;}
  SourceCampaignCharacterBorrowV62 actor;
  if(!borrow_source_campaign_character_v62(candidate.actual_world,character,actor,error)||!actor.character)
   return required("same Spawn Character",error);
  return dh2::character::source_character_select_spawn_v87(*actor.character,0,0,error);
 }
 // Container conversion uses the actual retained derived factory owners.
 // Wrong-type and absent targets have the original NULL conversion behavior.
 if(!world->noncharacter_owners_v105||!world->noncharacter_owners_v105->containers)
  return required("same container catalog",error);
 auto& containers=*world->noncharacter_owners_v105->containers;
 if(auto openable=containers.openable(target->identity))
  return openable->receiver().receiver().spawn(error);
 if(auto destructible=containers.destructible(target->identity))return destructible->spawn(error);
 error.clear();return true;
}
}
