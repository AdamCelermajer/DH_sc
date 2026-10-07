#include "source_campaign_events_v75.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_conditions_v70.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "model_renderer.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "application_services_owner_v5.hpp"
#include "character_kill_level_events_v23.hpp"
#include "game_object_spawn_probability_v1.hpp"
#include "script_manager_owner_v52.hpp"
#include "level_gameplay_update_v66.hpp"
#include "character_oid_cache_v81.hpp"
#include <cstring>
namespace model_renderer {namespace {
bool required(const char* name,std::string& e){if(e.empty())e=std::string("Required actual campaign event provider: ")+name;return false;}
struct CampaignEventsTransportV75 {
 std::weak_ptr<SourceWorldBorrowV61> world;
 std::weak_ptr<dh2::loader::CanonicalLevelContextV1> level;
 bool resolve(std::shared_ptr<SourceWorldBorrowV61>& out,std::string& e)const{
  out=world.lock();if(!out||!out->owner||!out->application||!out->canonical_world||!out->design)
   return required("SAME retained World/App/ObjectManager/design",e);
  return true;
 }
 bool props(std::uintptr_t id,std::int32_t& out,std::string& e)const{
  if(!id)return required("SafeGetCharPropsId source NULL dereference",e);
  std::shared_ptr<SourceWorldBorrowV61> actual;if(!resolve(actual,e))return false;
  SourceCampaignCharacterBorrowV62 borrowed;if(!borrow_source_campaign_character_v62(actual->owner,id,borrowed,e)||!borrowed.character)return false;
  auto* cell=borrowed.character->init_fields.properties_id13c8;
  if(!cell)return required("SAME produced Character13c8",e);
  if(*cell!=-1){out=*cell;e.clear();return true;}
  dh2::character::NpcInitPostRequestV1 request;request.source_entry=0x3b3d38;request.subject=id;
  dh2::character::NpcInitPostResponseV1 response;
  if(!dh2::world::CanonicalCharacterCandidateRecordV60::init_service(borrowed.character.get(),request,response,e))return false;
  out=response.value;e.clear();return true;
 }
 dh2::loader::GameEventRuntimeServicesV75 services(const std::shared_ptr<CampaignEventsTransportV75>& self){
  using namespace dh2::loader;GameEventRuntimeServicesV75 s;s.provider=self;
  s.current_level=[self](auto& lease,auto*& dispatcher,auto*& row,auto& e){
   std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;
   CanonicalCurrentLevelBorrowV1 current;if(!borrow_current_native_level_v27(current,e))return false;
   lease.reset();dispatcher=nullptr;row=nullptr;if(!current)return true;
   auto expected=self->level.lock();if(!expected||current.level()!=expected)return required("SAME GS published current Level",e);
   lease=current.level();const auto& fields=current.level()->constructor_fields_v3();dispatcher=fields.events.get();row=&fields.row3c;
   if(!dispatcher||dispatcher->identity()!=current.identity())return required("actual Level inherited EventManager",e);
   e.clear();return true;
  };
  s.constant=[self](auto group,auto key,auto& value,auto& e){
   std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;
   auto borrow=actual->design->borrow();const auto* service=borrow.design();
   if(!service||!service->lookup||service->lookup(service->context,0,group,key,&value))return required("SAME Application PyDataConstants",e);
   e.clear();return true;
  };
  s.common_script_count=[self](auto& count,auto& e){
   std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;
   auto scripts=actual->application->source_script_manager_v52();
   if(!scripts||scripts->diagnostics().failed)return required("actual ScriptManager.common8",e);
   count=scripts->fields().common8;e.clear();return true;
  };
  s.script_id=[self](auto name,auto& id,auto& e){
   std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;
   auto scripts=actual->application->source_script_manager_v52();
   if(!scripts||scripts->diagnostics().failed)return required("TempHackScriptIDAreStrings actual ScriptManager",e);
   //TempHackScriptIDAreStrings47fa34 requires a dot and looks up only the
   //suffix in the current-level domain, with GetIDFromName(flag=false).
   const auto* dot=name?std::strchr(name,'.'):nullptr;
   id=dot?scripts->id_from_name(dot+1,false):-1;e.clear();return true;
  };
  s.start_script=[self](auto id,auto room,auto flag,auto& e){
   std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;
   // Events/objectives may start during the genuine loading prefix, before
   // renderer gameplay admission38. They call the same Application manager;
   // admission of a draw/update transport is not a source script prerequisite.
   const auto scripts=actual->application->source_script_manager_v52();
   if(!scripts)return required("whole shared ScriptManager.StartScript4605c0",e);
   return scripts->start_script_v96(id,room,flag,e);
  };
  s.enemies_loaded=[self](bool templ,auto id,auto& count,auto& e){
   std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;
   std::uint32_t total{};
   //Original list order, duplicates, and dead entries are all source-visible.
   for(auto character:actual->canonical_world->manager.characters()){
    if(!character)continue;
    SourceCampaignCharacterBorrowV62 borrowed;if(!borrow_source_campaign_character_v62(actual->owner,character,borrowed,e)||!borrowed.character)return false;
    std::int32_t property{};
    if(templ){const auto& fields=borrowed.character->actor->kill_fields_v42();
     const auto* name=borrowed.character->actor->source_string(0x1398);
     if(!fields.produced||!name)return required("actual CharacterC1 template13ca/CharPropsArray1398",e);
     if(name->empty())property=fields.template13ca; //3b3784 actual empty-string branch.
     else if(borrowed.character->services.character_templates_v78){
      auto& actual_fields=borrowed.character->actor->kill_fields_v42();
      if(!borrowed.character->services.character_templates_v78->safe_template_id(*name,actual_fields.template13ca,property,e))return false;
     }
     else{
      const auto& dependency=actual->game_event_dependencies_v75;
      if(!dependency||!dependency->provider||!dependency->character_template_id)return required("whole SafeGetCharPropsTemplateId3b36ec Arrays-name producer",e);
      if(!dependency->character_template_id(character,property,e))return false;
     }
    }else if(!self->props(character,property,e))return false;
    if(property==id)++total;
   }
   if(total){std::memcpy(&count,&total,4);e.clear();return true;}
   //HasEnemies<TestCharTemplate>47cc20 compares the selected Compare symbol
   //with TestCharPropId::Compare before the static class-OID cache fallback.
   //Actual ELF GOT339c=47b05c and GOT3784=47b820 are distinct functions; the
   //template specialization therefore returns literal zero on an empty list.
   if(templ){count=0;e.clear();return true;}
   const auto raw=dh2::character::character_oid_cache_process_v81()->has(id);
   std::memcpy(&count,&raw,4);e.clear();return true;
  };
  s.zone_by_name=[self](auto name,auto& zone,auto& e){
   std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;
   auto& manager=actual->canonical_world->manager;dh2::target_providers::Handle16 handle{};
   if(!manager.by_name(name,-1,false,nullptr,handle,e))return false;
   const dh2::world::CanonicalObjectBorrowV1* object{};
   if(!manager.resolve_handle_v4(handle,false,object,{},e))return false;
   if(object){bool meets{};if(!dh2::world::game_object_meet_condition_v1(meets,e))return false;
    if(!meets){zone={};e.clear();return true;}}
   //Source invokes GetObject(false) again after MeetCondition callback.
   if(!manager.resolve_handle_v4(handle,false,object,{},e))return false;
   zone={};if(object){zone.receiver=object->lease;zone.identity=object->identity;}e.clear();return true;
  };
  s.talk_flag=[self](auto id,auto value,auto& e){
   std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;
   for(auto identity:actual->canonical_world->manager.characters()){
    std::int32_t props{};if(!self->props(identity,props,e))return false;if(props!=id)continue;
    if(identity){SourceCampaignCharacterBorrowV62 borrowed;if(!borrow_source_campaign_character_v62(actual->owner,identity,borrowed,e)||!borrowed.character||!borrowed.character->actor)return false;
     borrowed.character->actor->publish_talk_requirement_v75(value);}
    break;
   }
   e.clear();return true;
  };
  s.character_props=[self](auto id,auto& value,auto& e){return self->props(id,value,e);};
  s.inventory_quantity=[self](auto character,auto id,auto& found,auto& count,auto& e){
   std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;
   SourceCampaignCharacterBorrowV62 borrowed;if(!character||!borrow_source_campaign_character_v62(actual->owner,character,borrowed,e)||!borrowed.character)
    return required("GatherLoot actual owner10 Character",e);
   auto* inventory=borrowed.character->inventory37c;
   if(!inventory||inventory->character()!=character)return required("SAME Character inventory37c",e);
   //Whole FindItem3fd15c tests Potion24 before ordinary ordered slots.
   const auto* item=inventory->potion();if(!item||item->id!=id){item=nullptr;
    for(const auto& slot:inventory->items())if(slot&&slot->item&&slot->item->id==id){item=slot->item.get();break;}}
   found=item!=nullptr;if(item)count=static_cast<std::int16_t>(item->signed_quantity());e.clear();return true;
  };
  s.inventory_register=[self](auto character,auto id,bool adding,auto& e){
   std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;
   SourceCampaignCharacterBorrowV62 borrowed;if(!character||!borrow_source_campaign_character_v62(actual->owner,character,borrowed,e)||!borrowed.character)
    return required("GatherLoot actual owner10 Character",e);
   auto* inventory=borrowed.character->inventory37c;
   if(!inventory||inventory->character()!=character)return required("SAME gathering-ID list Character3ac",e);
   if(adding){inventory->gathering_ids_v11().register_id(id);e.clear();return true;}
   dh2::data::InventoryGatheringAssertServicesV11 assertions;
   assertions.context=actual.get();assertions.missing_id=[](void* raw,std::int32_t,std::string& error){
    auto& world=*static_cast<SourceWorldBorrowV61*>(raw);const auto& dependency=world.condition_dependencies_v70;
    if(!dependency||!dependency->assertion_owner||!dependency->assertion_mode)return required("actual inventory missing-ID assertion mode",error);
    if(*dependency->assertion_mode==2){error="Original inventory gathering missing-ID mode2 NULL write refused";return false;}
    if(*dependency->assertion_mode==1){if(!dependency->assertion)return required("actual inventory missing-ID logger",error);
     return dependency->assertion("..\\..\\project_vs2005\\Game/..\\..\\sources/Game/Items/ItemInventory.h",0x14f,"iter != m_activeQuestGatheredItemIds.end()",error);}
    error.clear();return true;
   };
   return inventory->gathering_ids_v11().unregister_id(id,assertions,e);
  };
  s.character_is_player=[self](auto id,auto& value,auto& e){
   std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;
   SourceCampaignCharacterBorrowV62 borrowed;if(!borrow_source_campaign_character_v62(actual->owner,id,borrowed,e)||!borrowed.character)return false;
   return borrowed.character->is_player(value,e);
  };
  s.project_event=[self](const auto& event,auto& out,auto& e){
   std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;
   if(project_scoped_game_quest_event_v75(event,out,e))return true;
   if(!e.empty())return false;
   //Known original Kill event producer proves its family via function identity
   //and exact envelope/stack address before lending mutable source fields.
   if(const auto* source=dh2::character::kill_quest_payload_v23(event)){
    auto* fields=const_cast<dh2::character::KillQuest48*>(source);
    out={event.lifetime,fields->killer,0,fields->subject_id,&fields->flag0,&fields->flag1,&fields->network_id};e.clear();return true;
   }
   const auto& dependency=actual->game_event_dependencies_v75;
   if(!dependency||!dependency->provider||!dependency->project_quest_event)return required("typed actual QuestEvent family producer",e);
   return dependency->project_quest_event(event,out,e);
  };
  s.send_network=[self](const auto& event,auto& e){
   std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;
   const auto online=actual->application->get_online_loading_v55();if(!online)return required("actual GetOnline",e);
   if(!online->byte5()||*event.from_network11||!*event.pending_network10){e.clear();return true;}
   const auto& dependency=actual->game_event_dependencies_v75;
   if(!dependency||!dependency->provider||!dependency->send_network_event)return required("whole CMsgRaisedEvent.Create/CMessaging.SendMsg",e);
   if(!dependency->send_network_event(event,e))return false;*event.pending_network10=0;e.clear();return true;
  };
  s.assertion=[self](auto line,auto expression,auto& e){
   std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;
   const auto& dependency=actual->condition_dependencies_v70;
   if(!dependency||!dependency->assertion_owner||!dependency->assertion_mode)return required("actual shared process assertion mode",e);
   if(*dependency->assertion_mode==2){e="Original Objective assertion mode2 NULL write refused";return false;}
   if(*dependency->assertion_mode==1){if(!dependency->assertion)return required("actual process assertion logger",e);
    return dependency->assertion("..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Progression\\Objective.cpp",line,expression,e);}
   e.clear();return true;
  };
  return s;
 }
};
}
bool bind_source_campaign_event_dependencies_v75(const SourceCampaignCandidateBorrowV55& candidate,
 SourceGameEventDependenciesV75 dependencies,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 if(!dependencies.provider||world->game_event_dependencies_v75){e="Event dependencies require one actual source owner binding";return false;}
 world->game_event_dependencies_v75=std::make_shared<SourceGameEventDependenciesV75>(std::move(dependencies));e.clear();return true;
}
bool borrow_source_campaign_objectives_v75(const SourceCampaignCandidateBorrowV55& candidate,
 std::shared_ptr<dh2::loader::GameEventRuntimeV75>& out,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 if(!world->game_events_v75){
  auto transport=std::make_shared<CampaignEventsTransportV75>();transport->world=world;transport->level=candidate.level;
  world->game_events_v75=std::make_shared<dh2::loader::GameEventRuntimeV75>(transport->services(transport));
 }
 out=world->game_events_v75;e.clear();return true;
}
bool borrow_source_campaign_events_v75(const SourceCampaignCandidateBorrowV55& candidate,
 std::shared_ptr<dh2::loader::GameEventRuntimeV75>& out,std::string& e){
 std::shared_ptr<dh2::loader::GameEventRuntimeV75> runtime;
 if(!borrow_source_campaign_objectives_v75(candidate,runtime,e))return false;
 dh2::loader::GameEventLevelFieldsV50 fields;if(!candidate.level||!candidate.level->game_event_fields_v50(fields,e))return false;
 if(!fields.owner194||!*fields.owner194||!fields.field194||*fields.field194!=reinterpret_cast<std::uintptr_t>(fields.owner194->get())||
    !(*fields.owner194)->diagnostics().storage_load_complete)return required("completed SAME GameEventManager194 source Load",e);
 if(!runtime->attach_manager_v75(*fields.owner194,e))return false;
 out=std::move(runtime);e.clear();return true;
}
bool borrow_source_campaign_level_events_v88(const std::shared_ptr<void>& world,
 SourceCampaignLevelEventsBorrowV88& out,std::string& e){
 out={};dh2::loader::CanonicalCurrentLevelBorrowV1 captured;
 if(!borrow_current_native_level_v27(captured,e))return false;
 if(!captured){e.clear();return true;} //3ffb68/b70 NULL branch skips quest emit
 SourceCampaignCandidateBorrowV55 candidate;
 if(!world||!borrow_source_campaign_candidate_runtime_v61(candidate,e)||candidate.actual_world!=world||candidate.level!=captured.level())return required("captured Application.GetCurrentLevel ownership",e);
 auto level=captured.level();const auto* constructor=level->constructor_owner_v3();
 auto* dispatcher=level->constructor_fields_v3().events.get();
 if(!constructor||constructor->phase()!=dh2::loader::LevelConstructorPhaseV3::complete||!dispatcher||dispatcher->identity()!=captured.identity())return required("actual completed Level inherited EventManager C1",e);
 out={std::move(level),dispatcher,captured.identity()};e.clear();return true;
}
bool raise_source_campaign_level_event_v88(const SourceCampaignLevelEventsBorrowV88& captured,
 const dh2::events::EventBorrowV12& event,std::string& e){
 //Original caller keeps the Level returned at3ffb64, across constant lookup
 //and stack QuestEvent construction. Do not reload current GS/Level here.
 if(!captured.level||captured.identity!=captured.level->identity()||!captured.events||
    captured.events!=captured.level->constructor_fields_v3().events.get()||captured.events->identity()!=captured.identity)return required("SAME captured pickup Level event receiver",e);
 return captured.events->raise_async(event,e); //literal339090→338ebc, no enqueue
}
}
