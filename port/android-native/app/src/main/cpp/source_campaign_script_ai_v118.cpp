#include "source_campaign_script_ai_v118.hpp"
#include "model_renderer.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "source_campaign_character_frame_v111.hpp"
#include "source_campaign_character_fsm_v101.hpp"
#include "source_campaign_script_actor_v96.hpp"
#include "source_campaign_death_rewards_v84.hpp"
#include "source_process_trophies_v100.hpp"
#include "source_settings_update_job_v102.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <canonical_object_manager_v1.hpp>
#include <script_command_receivers_v59.hpp>
#include <application_services_owner_v5.hpp>
#include <application_player_manager_bootstrap_v59.hpp>
#include <player_save_difficulty_global_v29.hpp>
#include <owned_hud_settings_v1.hpp>
#include <script_manager_owner_v52.hpp>
#include <source_assertion_process_v76.hpp>
#include <character_aggro_clear_all_v2.hpp>
#include <character_ai_groups_v87.hpp>
#include <cstring>
namespace model_renderer {namespace {
using Record=dh2::world::CanonicalCharacterCandidateRecordV60;
bool need(const char* leaf,std::string& e){if(e.empty())e=std::string("Required source campaign Script AI ")+leaf;return false;}
bool word(const dh2::loader::CheckedCommandBorrowV59& q,unsigned offset,std::int32_t& value,std::string& e){auto* cell=q.actual_data->scalar(offset);if(!cell||cell->width!=4)return need("actual command word",e);std::memcpy(&value,&cell->bits,4);return true;}
bool character(const SourceCampaignCandidateBorrowV55& c,const char* name,std::int32_t room,std::shared_ptr<Record>& out,std::string& e){
 if(!name||!c.objects)return need("actual object name/manager",e);dh2::target_providers::Handle16 h{};const dh2::world::CanonicalObjectBorrowV1* object{};
 if(!c.objects->by_name(name,room,false,nullptr,h,e)||!c.objects->resolve_handle_v4(h,false,object,{},e))return false;
 out.reset();if(!object)return true;if(!object->as_character)return need("selected Character virtual24",e);std::uintptr_t id{};if(!object->as_character(object->context,id,e))return false;if(!id)return true;
 SourceCampaignCharacterBorrowV62 loan;if(!borrow_source_campaign_character_v62(c.actual_world,id,loan,e))return false;out=std::move(loan.character);return bool(out);
}
bool trace(const std::shared_ptr<SourceWorldBorrowV61>& w,std::string& e){unsigned ignored{};return w->debug&&w->debug_files&&dh2_character_debug_load(w->debug.get(),w->debug_files)==1&&dh2_character_debug_get(&ignored,w->debug.get(),"isTracingScriptCmd",w->debug_files)==1?true:need("actual Debug prefix",e);}
bool faery_difficulty(const std::shared_ptr<Record>& r,std::int32_t& difficulty,std::string& e){return r->save_fields&&dh2::player::character_game_difficulty_v29({r->actor,r->actor->object->identity,r->save_fields->save_slot14e8(),r->services.difficulty_global},difficulty,e);}
bool faery_storage(const std::shared_ptr<Record>& r,std::int32_t difficulty,std::uint32_t index,std::string& e){
 if(!r->save||!r->save_fields||*r->save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(r->save.get())||difficulty<0||difficulty>=3||!r->save->faeries_initialized()[difficulty])return need("SAME produced Save14e8 faery tier",e);
 if(index>=r->save->faeries()[difficulty].size()){auto assertion=dh2::world::SourceAssertionProcessV76::borrow();if(!assertion||!assertion->report("Character_Faery.cpp",169,"faeryIdx < SG_GetFaerieCount(diff)",e))return false;return need("original out-of-range faery unsafe access",e);}return true;
}
bool update_skills(Record& r,std::string& e){if(r.player_script_owner_v62){if(r.player_script_owner_v62->native_source_update_all_skills_v70()<0){e=r.player_script_owner_v62->error();return false;}return true;}if(r.npc_skills_v84){if(r.npc_skills_v84->update()<0){e=r.npc_skills_v84->error();return false;}return true;}return need("actual CharAI.UpdateAllSkills owner",e);}
}
bool execute_source_campaign_script_ai_v118(const SourceCampaignCandidateBorrowV55& c,const dh2::loader::CheckedCommandBorrowV59& q,bool skip,std::int32_t room,bool& handled,std::string& e){
 (void)skip;handled=false;if(!q.kind8||!q.actual_data)return need("actual command receiver/Data",e);const auto kind=*q.kind8;
 if(kind!=27&&kind!=28&&kind!=29&&kind!=47&&(kind<33||kind>38))return true;handled=true;
 //These shipping Execute bodies are literal BX LR, not missing AI effects:
 //45579c,4557a8,4557b4,4557c0,4557cc respectively. No Debug or Data reads.
 if(kind>=33&&kind<=37){e.clear();return true;}
 std::shared_ptr<SourceWorldBorrowV61> w;if(!borrow_source_campaign_condition_world_v70(c,w,e)||!trace(w,e))return false;
 if(kind==47){std::shared_ptr<Record> actor,master;if(!character(c,q.actual_data->cstring(28),room,actor,e)||!character(c,q.actual_data->cstring(16),room,master,e))return false;if(!actor||!master){e.clear();return true;}return source_campaign_character_set_master_v111(c.actual_world,actor->actor->object->identity,master->actor->object->identity,e);}
 if(kind==29){std::shared_ptr<Record> actor;if(!character(c,q.actual_data->cstring(16),room,actor,e))return false;if(!actor){e.clear();return true;}auto* flag=q.actual_data->scalar(8);if(!flag||flag->width!=1)return need("actual Limbus mode byte",e);return source_campaign_character_set_limbus_v118(c.actual_world,actor->actor->object->identity,flag->bits!=0,e);}
 if(kind==38){std::shared_ptr<Record> actor;if(!character(c,q.actual_data->cstring(20),room,actor,e))return false;if(!actor){e.clear();return true;}const auto script=actor->actor->ai_events.active;if(!script){e.clear();return true;}const dh2_script_int_bindings* bindings{};
  if(actor->player_script_owner_v62){if(!actor->player_script_owner_v62->session().owner().integer_bindings(script,bindings))return need("SAME active player LuaScript integer receiver",e);}
  else if(!actor->actor->session||!actor->actor->session->owner().integer_bindings(script,bindings))return need("SAME active NPC LuaScript integer receiver",e);
  const auto* name=q.actual_data->cstring(12);std::int32_t value{};if(!name||!word(q,24,value,e)||!bindings||!bindings->map)return need("actual LuaScript.SetInt map/name/value",e);if(dh2_script_int_set(bindings->map,name,value)){return need("actual LuaScript.SetInt delivery",e);}e.clear();return true;
 }
 auto pm=w->application->source_player_manager_v59();if(!pm)return need("actual PlayerManager",e);
 if(kind==28){auto online=w->application->get_online_loading_v55();if(!online)return need("actual GetOnline",e);if(online->byte5())return need("original online IsLocalPlayerHosting/PM719 gates",e);}
 dh2::player::PlayerInfoFieldsV1* player{};if(!pm->get_local_player(0,true,player,e)||!player)return need("actual local0,true PlayerInfo",e);if(!player->character660){e.clear();return true;}
 SourceCampaignCharacterBorrowV62 loan;if(!borrow_source_campaign_character_v62(c.actual_world,player->character660,loan,e)||!loan.character)return false;auto r=loan.character;std::int32_t raw_index{},difficulty{};
 if(!word(q,8,raw_index,e)||!faery_difficulty(r,difficulty,e)||!faery_storage(r,difficulty,static_cast<std::uint32_t>(raw_index),e))return false;
 const auto index=static_cast<std::uint32_t>(raw_index);
 if(kind==27){std::int32_t state{};if(!word(q,12,state,e))return false;
  if(index&&state==1){auto online=w->application->get_online_loading_v55();if(!online)return need("actual GetOnline",e);if(!online->byte5()){std::int32_t current{};if(!faery_difficulty(r,current,e))return false;if(!current){auto settings=w->application->source_settings4c_v67();auto* tutorial=settings?settings->source_tutorial_cell_v116(0x2b):nullptr;if(!tutorial)return need("actual settings tutorial2b",e);if(*tutorial){auto scripts=w->application->source_script_manager_v52();if(!scripts)return need("actual ScriptManager",e);const auto id=scripts->id_from_name("cinematic_Tuto_faery",true);if(id!=-1&&!scripts->start_script_v96(id,-1,false,e))return false;*tutorial=0;if(!save_process_settings_v102(w->application,e))return false;}}}}
  return r->save->set_faery_state(index,state,static_cast<std::uint32_t>(difficulty),e);
 }
 const auto previous=r->save->faery_level(index,static_cast<std::uint32_t>(difficulty));const auto next=static_cast<std::int32_t>(std::uint32_t(previous)+1u);
 if(!r->save->set_faery_level(index,next,static_cast<std::uint32_t>(difficulty),e)||!update_skills(*r,e))return false;
 for(std::uint32_t i=0;i<r->save->faeries()[difficulty].size();++i){if(!faery_storage(r,difficulty,i,e))return false;if(!r->services.difficulty_global)return need("actual source difficulty global",e);const auto global=r->services.difficulty_global->value();if(global<0||global>=3||!r->save->faeries_initialized()[global])return need("actual default difficulty faery tier",e);if(!r->save->faery_level(i,static_cast<std::uint32_t>(global))){e.clear();return true;}}
 dh2::android_ui::ProcessTrophyBorrowV100 trophy;if(!dh2::android_ui::borrow_source_process_trophies_v100(w->application,trophy,e)||!trophy.manager)return false;bool local{};if(!pm->source_is_local_player_v61(player->character660,local,e))return false;if(!local){e.clear();return true;}if(trophy.manager->unlock(trophy.manager->catalog().index("faery_charged"))){e=trophy.manager->error();return false;}e.clear();return true;
}
int source_campaign_limbus_body_v118(dh2::world::CanonicalCharacterCandidateRecordV60& r,const dh2::character::StateOwnerRequest48& q,std::string& e){
 using namespace dh2::character;
 if(q.operation==state_owner_event&&q.source_function==0x3bffe8)return 0; //literal CSLimbus.OnEvent
 if(q.operation!=state_owner_focus&&q.operation!=state_owner_blur)return need("original Limbus operation",e),-1;
 unsigned ignored{};if(!r.services.debug||!r.services.debug_files||dh2_character_debug_load(r.services.debug,r.services.debug_files)!=1||dh2_character_debug_get(&ignored,r.services.debug,"isTracingCharState",r.services.debug_files)!=1)return need("actual CSLimbus Debug",e),-1;
 const auto id=r.actor->object->identity;const auto world=r.services.world;
 if(q.operation==state_owner_focus&&q.source_function==0x3c2e58){
  r.actor->machine->state().flags=0;
  if(!source_campaign_character_set_visible_v96(world,id,false,e))return -1;
  const auto* mode=r.actor->source_bool_field(0x530);if(!mode)return need("source FSM Limbus530 mode",e),-1;
  auto delay=[&](std::uint32_t& value){const auto* no_respawn=r.actor->source_bool_field(0x1481);if(!no_respawn)return need("source Character.GetRespawnDelay1481",e);std::int32_t raw{};if(dh2_property_resolve(&r.view,11,&raw))return false;value=*no_respawn?0u:1000u*std::uint32_t(raw>>8);return true;};
  if(*mode){std::uint32_t first{};if(!delay(first))return -1;if(first){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!borrow_source_campaign_candidate_runtime_v61(c,e)||c.actual_world!=world||!borrow_source_campaign_condition_world_v70(c,w,e))return -1;auto online=w->application->get_online_loading_v55();if(!online)return need("actual GetOnline",e),-1;if(online->byte5())return need("original online Limbus IsLocalPlayerHosting",e),-1;std::uint32_t duration{};if(!delay(duration))return -1;int timer=-2;if(r.player_script_owner_v62)timer=r.player_script_owner_v62->session().start_timer(duration,0,47);else if(r.actor->session)timer=dh2_character_timer_start(&r.actor->session->timers(),duration,0,47,0,&r.actor->session->native_timer_services());if(timer< -1)return need("actual Limbus CharTimers47",e),-1;}}
  struct Context {std::shared_ptr<void> world;std::vector<std::shared_ptr<Record>> pins;};Context context{world,{}};
  AggroClearAllServicesV2 services;services.context=&context;
  services.actor=[](void* raw,std::uintptr_t actor,AggroClearActorBorrowV2& out,std::string& e){auto& c=*static_cast<Context*>(raw);SourceCampaignCharacterBorrowV62 b;if(!borrow_source_campaign_character_v62(c.world,actor,b,e)||!b.character||!b.character->actor)return false;auto* a=b.character->actor.get();auto* maps=a->source_aggro_v84();if(!maps||!maps->outgoing()||!maps->incoming()||!a->object)return need("SAME actual Limbus reciprocal maps",e);out={actor,maps->outgoing(),maps->incoming(),&a->object->binding};c.pins.push_back(std::move(b.character));return true;};
  services.on_deaggro=[](void* raw,std::uintptr_t receiver,std::uintptr_t other,const dh2_script_callback_scope* scope,std::string& e){auto& c=*static_cast<Context*>(raw);return source_campaign_character_aggro_event_v84(c.world,dh2::data::aggro_notify_target_cleared,other,receiver,scope,e);};
  AggroClearActorBorrowV2 actor;if(!services.actor(&context,id,actor,e))return -1;AggroClearAllResultV2 result;return character_aggro_clear_all_v2(result,actor,services,e)?0:-1;
 }
 if(q.operation==state_owner_blur&&q.source_function==0x3c2be4){
  ControllerCommandState32 snapshot;if(!r.services.controller||!r.services.controller(snapshot,r,e)||!r.actor->controller)return need("actual CSLimbus controller",e),-1;auto* controller=r.actor->controller->command_state(snapshot.global_blocked);if(!controller)return -1;controller->locked=0;
  if(!source_campaign_character_set_visible_v96(world,id,true,e))return -1;
  dh2::world::GameObjectInitializationFieldsV62 fields;if(!r.actor->inherited_initialization_fields_v62(r.actor,fields,e)||!fields.vector3)return -1;auto* initial=fields.vector3(0x1450);if(!initial||!r.position||!r.position->set_position(initial,true,e))return -1;
  auto* rotation=fields.vector3(0x16c);auto* original=fields.vector3(0x145c);if(!rotation||!original)return need("actual CSLimbus initial rotation145c",e),-1;std::memcpy(rotation,original,12);r.actor->runtime.rotation.heading_angle=original[2];
  if(r.actor->source_visual()){auto v=r.visual?r.visual->visual():nullptr;if(!v||!v->sync_rotation_v86(e))return -1;}
  if(!r.revive_v70(0,false,e))return -1;
  if(r.actor->source_ai_group_role38==3){if(!r.ai_group_v87||!r.ai_group_v87->source_limbus_blur_v118(r,e))return -1;}
  return 0;
 }
 return need("exact original CSLimbus selected body",e),-1;
}
}
