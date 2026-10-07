#include "source_campaign_quests_v76.hpp"
#include "source_campaign_conditions_v70.hpp"
#include "source_campaign_events_v75.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "model_renderer.hpp"
#include "source_assertion_process_v76.hpp"
#include "design_settings.hpp"
#include "source_campaign_quicksave_v88.hpp"
#include "source_campaign_death_rewards_v84.hpp"
#include "source_campaign_items_v88.hpp"
#include "native_source_script_ui_v98.hpp"
#include "source_campaign_script_execution_v96.hpp"
#include "source_process_trophies_v100.hpp"
#include <application_player_manager_bootstrap_v59.hpp>
#include <application_services_owner_v5.hpp>
#include <script_manager_owner_v52.hpp>
#include <player_save_write_owner_v1.hpp>
#include <level_constructor_bindings_v4.hpp>
#include <native_level_application_v25.hpp>
#include <canonical_level_context_v1.hpp>
#include <quest_condition_compile_v70.hpp>
#include <player_save_difficulty_global_v29.hpp>
#include <cstring>
namespace model_renderer {namespace {
bool required(std::string& e,const char* name){if(e.empty())e=std::string("Required actual Quest gameplay owner: ")+name;return false;}
#include "source_campaign_quest_frame_v108.inc"
struct QuestTransportV76 {
 std::weak_ptr<SourceWorldBorrowV61> world;
 bool resolve(std::shared_ptr<SourceWorldBorrowV61>& out,std::string& e)const{
  out=world.lock();return out&&out->owner&&out->canonical_world&&out->settings?true:required(e,"SAME retained World/ObjectManager/settings");
 }
 dh2::world::QuestTalkMarkerServicesV76 marker_services(const std::shared_ptr<QuestTransportV76>& self){
  dh2::world::QuestTalkMarkerServicesV76 s;s.provider=self;
  s.first_character=[self](auto target,auto& out,auto& e){
   std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;out={};
   for(auto identity:actual->canonical_world->manager.characters()){
    std::int32_t props=-1;SourceCampaignCharacterBorrowV62 borrow;
    if(identity){
     if(!borrow_source_campaign_character_v62(actual->owner,identity,borrow,e)||!borrow.character||!borrow.character->actor)return false;
     auto& r=*borrow.character;
     if(!r.init_fields.properties_id13c8&&!r.actor->init_post_fields(r.init_fields,e))return false;
     dh2::character::NpcInitPostRequestV1 request;request.source_entry=0x3b3d38;request.subject=identity;
     dh2::character::NpcInitPostResponseV1 response;
     if(!dh2::world::CanonicalCharacterCandidateRecordV60::init_service(&r,request,response,e))return false;
     props=response.value;
    }
    if(props!=target)continue;
    if(identity){out.receiver=borrow.character;out.identity=identity;
     const auto weak=std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>(borrow.character);
     out.store_flags=[weak](auto required,auto primary){if(auto r=weak.lock())r->actor->publish_talk_marker_flags_v76(required,primary);};}
    break; //Original first match, including NULL -> -1.
   }e.clear();return true;
  };
  s.settings_effect=[self](auto offset,auto& id,auto& e){
   std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;
   auto table=actual->settings->borrow();
   if(offset!=0x64&&offset!=0x68&&offset!=0x6c&&offset!=0x70)return required(e,"captured quest marker setting offset");
   const auto* bits=table?table.word(0,static_cast<std::uint32_t>(offset/4)):nullptr;
   if(!bits)return required(e,"actual DesignSettings first row marker effect cell");
   std::memcpy(&id,bits,4);e.clear();return true;
  };
  s.actual_fx=[self](auto& lease,auto*& manager,auto& e){
   std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;
   const auto& p=actual->quest_marker_dependencies_v76;
   if(!p||!p->provider||!p->actual_fx)return required(e,"fresh campaign animated-FX instance publication");
   return p->actual_fx(lease,manager,e);
  };
  s.visual_owner204=[self](auto& manager,auto id,auto owner,auto& e){
   std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;
   const auto& p=actual->quest_marker_dependencies_v76;
   if(!p||!p->provider||!p->visual_owner204)return required(e,"actual AnimatedFX visual reference204 producer");
   return p->visual_owner204(manager,id,owner,e);
  };
  s.animator_loop=[self](auto& manager,auto id,auto looping,auto& e){
   std::shared_ptr<SourceWorldBorrowV61> actual;if(!self->resolve(actual,e))return false;
   const auto& p=actual->quest_marker_dependencies_v76;
   if(!p||!p->provider||!p->animator_loop)return required(e,"actual AnimatedFX GetAnimator/SetLooping");
   return p->animator_loop(manager,id,looping,e);
  };return s;
 }
 bool compile(const std::shared_ptr<dh2::character::CharacterMenuQuestsV51>& quests,std::uintptr_t id,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> actual;if(!resolve(actual,e))return false;
  SourceCampaignCandidateBorrowV55 candidate;if(!borrow_source_campaign_candidate_runtime_v61(candidate,e))return false;
  if(candidate.actual_world!=actual->owner)return required(e,"SAME retained campaign candidate");
  std::shared_ptr<dh2::world::NativeQuestRuntimeV76> runtime;
  if(!borrow_source_campaign_quest_runtime_v76(candidate,quests,runtime,e)){if(quests)quests->retain_runtime_failure_v76(e);return false;}
  return quests->compile_quest_v76(id,e);
 }
};
}
void source_campaign_condition_dependencies_v76(const std::shared_ptr<SourceWorldBorrowV61>& world,SourceConditionDependenciesV70& out){
 auto assertions=dh2::world::SourceAssertionProcessV76::borrow();out.assertion_owner=assertions;out.assertion_mode=assertions->source_level();
 out.assertion=[assertions](auto file,auto line,auto expression,auto& e){return assertions->report(file,line,expression,e);};
 auto compiler=std::make_shared<QuestTransportV76>();compiler->world=world;out.quest_compile_owner=compiler;
 out.quest_compile480178=[compiler](const auto& quests,auto id,auto& e){return compiler->compile(quests,id,e);};
}
bool borrow_source_campaign_quest_runtime_v76(const SourceCampaignCandidateBorrowV55& candidate,
 const std::shared_ptr<dh2::character::CharacterMenuQuestsV51>& quests,
 std::shared_ptr<dh2::world::NativeQuestRuntimeV76>& out,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 if(!quests||!quests->save()||!quests->save()->character())return required(e,"actual Character14e8 Quest facade");
 SourceCampaignCharacterBorrowV62 character;
 if(!borrow_source_campaign_character_v62(world->owner,quests->save()->character(),character,e)||!character.character||
    character.character->save!=quests->save()||!character.character->profile_bootstrap||
    character.character->profile_bootstrap->quest_owner_v70()!=quests)return required(e,"SAME canonical Character/Save/profile Quest owner");
 if(quests->runtime_v76()){out=quests->runtime_v76();e.clear();return true;}
 dh2::world::ConditionDataInitServicesV3 source;dh2::world::NativeQuestRuntimeServicesV76 services;
 if(!borrow_source_campaign_conditions_v70(candidate,source,services.conditions,e)||
    !borrow_source_campaign_objectives_v75(candidate,services.objectives,e))return false;
 auto transport=std::make_shared<QuestTransportV76>();transport->world=world;services.provider=transport;
 services.remove_talk_marker=[transport](auto& objective,auto& error){return dh2::world::quest_remove_talk_marker_v76(objective,transport->marker_services(transport),error);};
 services.install_talk_marker=[transport](auto& objective,auto priority,auto state,auto& error){
  return dh2::world::quest_install_talk_marker_v76(objective,priority,state,transport->marker_services(transport),error);
 };
 auto runtime=std::make_shared<dh2::world::NativeQuestRuntimeV76>(quests,std::move(services));
 auto frame=std::make_shared<QuestFrameTransportV108>();frame->world=world;frame->quests=quests;
 if(!runtime->bind_frame_v108(frame->services(),e))return false;
 if(!quests->bind_runtime_v76(runtime,e))return false;out=std::move(runtime);e.clear();return true;
}
bool source_campaign_character_sg_update_v108(const std::shared_ptr<void>& actual_world,std::uintptr_t id,bool force,std::string& e){
 SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;SourceCampaignCharacterBorrowV62 b;
 if(!borrow_source_campaign_candidate_runtime_v61(c,e)||c.actual_world!=actual_world||!borrow_source_campaign_condition_world_v70(c,w,e)||!borrow_source_campaign_character_v62(actual_world,id,b,e)||!b.character||!b.character->actor)return required(e,"Character.SG_Update actual receiver");
 auto r=b.character;if(!r->save_fields||!r->save_fields->save_slot14e8())return required(e,"Character source Save14e8 cell");
 if(!*r->save_fields->save_slot14e8()){e.clear();return true;} //3bc480 source NULL-save guard
 if(!r->save||*r->save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(r->save.get())||r->save->character()!=id||!r->quest_sync_owner)return required(e,"SAME PlayerSavegame.SG_Update owner");
 auto quests=r->profile_bootstrap?r->profile_bootstrap->quest_owner_v70():nullptr;
 if(!quests||quests->save()!=r->save)return required(e,"actual initialized SAME Quest collections");
 auto frame=std::make_shared<QuestFrameTransportV108>();frame->world=w;frame->quests=quests;
 auto sync=r->services.quest_sync;
 //The source synchronizer remains the sole ready14 writer. Native transport
 //reads the actual App COnline owner; online message leaves remain genuine.
 if(!sync.owner)sync.owner=frame;
 if(!sync.online)sync.online=[frame](auto& online,auto& e){return frame->online(online,e);};
 if(!r->quest_sync_owner->try_sync(sync,e))return false;
 bool online;if(!frame->online(online,e))return false;
 auto& collection=online?r->save->volatile_quests_v45():r->save->regular_quests_v45();
 if(*collection.source_character5c_v70()!=id)return required(e,"SAME SG_SetPlayer collection5c");
 std::shared_ptr<dh2::world::NativeQuestRuntimeV76> runtime;if(!borrow_source_campaign_quest_runtime_v76(c,quests,runtime,e)||!runtime)return false;
 if(runtime->failed()||!quests->runtime_failure_v76().empty()){e=runtime->failed()?runtime->failure():quests->runtime_failure_v76();return false;}
 //PushProfilingContext3136b4/Pop3136b8 are both literal BXLR in this ELF.
 dh2::world::QuestConditionCompileBorrowV70 fields;
 fields.receiver=std::shared_ptr<void>(r->save,&collection);fields.collection=&collection;fields.character5c=collection.source_character5c_v70();fields.compiled28=collection.source_compiled28_v70();
 dh2::world::QuestConditionCompileServicesV70 services;services.transport=frame;
 services.character_difficulty3bb8e4=[frame](auto id,auto& difficulty,auto& e){return frame->difficulty(id,difficulty,e);};
 services.quest_compile480178=[runtime](auto id,auto& e){return runtime->compile(id,e);};
 if(!dh2::world::quest_condition_compile_v70(fields,force,services,e)){quests->retain_runtime_failure_v76(e);return false;}
 auto difficulty=[&](std::int32_t& value){if(!frame->difficulty(id,value,e))return false;if(value<0||value>=3)return required(e,"Quest.Update actual difficulty0..2");return true;};
 std::int32_t selected;if(!difficulty(selected))return false;
 const auto count=collection.source_quests_v45()[std::size_t(selected)].size();
 for(std::size_t i=0;i<count;++i){
  if(!difficulty(selected))return false;const auto& current=collection.source_quests_v45()[std::size_t(selected)];
  if(i>=current.size()||!current[i])return required(e,"actual Quest.Update captured count/live slot");
  if(!runtime->update_quest_v108(current[i],e)){quests->retain_runtime_failure_v76(e);return false;}
 }
 e.clear();return true;
}
bool bind_source_campaign_quest_markers_v76(const SourceCampaignCandidateBorrowV55& candidate,
 SourceQuestMarkerDependenciesV76 dependencies,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 if(!dependencies.provider||(!dependencies.provider.owner_before(world->owner)&&!world->owner.owner_before(dependencies.provider))||
    !dependencies.actual_fx||!dependencies.visual_owner204||!dependencies.animator_loop||world->quest_marker_dependencies_v76){
  e="Quest marker publication requires one actual instance transport without containing World cycles";return false;
 }
 world->quest_marker_dependencies_v76=std::make_shared<SourceQuestMarkerDependenciesV76>(std::move(dependencies));e.clear();return true;
}
}
