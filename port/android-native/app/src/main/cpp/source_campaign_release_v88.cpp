#include "source_campaign_startup_abort_v114.hpp"
#include "source_campaign_release_v88.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_admission_v104.hpp"
#include "source_campaign_fx_v77.hpp"
#include "source_campaign_scene_pf_release_v106.hpp"
#include <level_batch_release_binding_v98.hpp>
#include "source_campaign_items_v88.hpp"
#include "source_campaign_character_unload_v105.hpp"
#include "visual_fx_manager_libraries_v63.hpp"
#include "source_campaign_quicksave_v88.hpp"
#include "source_campaign_camera_release_v88.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "model_renderer.hpp"
#include "renderer_native_menu_prefix_v62.hpp"
#include "captured_menu_lease_v101.hpp"
#include "application_services_owner_v5.hpp"
#include "application_save_files_owner_v61.hpp"
#include "campaign_profile_files_v1.hpp"
#include "application_player_manager_bootstrap_v59.hpp"
#include "application_spawn_random_owner_v4.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "character_ai_groups_v87.hpp"
#include "character_oid_cache_v81.hpp"
#include "character_loading_queue_v105.hpp"
#include "projectile_manager_owner_v108.hpp"
#include "source_release_journal_v69.hpp"
#include "native_gslevel_runtime_v27.hpp"
#include "packet_slot_registry_v88.hpp"
#include "game_event_runtime_v75.hpp"
#include <chrono>
#include <ctime>
#include <climits>
#include <cstdio>
namespace model_renderer {namespace {
bool missing(std::string& e){if(e.empty())e="Required SAME retained campaign release receiver";return false;}
template<class A,class B>bool same(const std::shared_ptr<A>& a,const std::shared_ptr<B>& b){
 return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);
}
struct ReleaseTransportV88 {
 std::weak_ptr<SourceWorldBorrowV61> world;
 std::weak_ptr<dh2::loader::CanonicalLevelContextV1> level;
 std::weak_ptr<dh2::loader::NativeGSLevelRuntimeV27> startup_gs_v115;
 bool startup_v115{};
 std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59> players;
 std::shared_ptr<void> unload_providers,destroy_providers;
 bool current(SourceCampaignCandidateBorrowV55& c,std::shared_ptr<SourceWorldBorrowV61>& w,std::string& e)const{
  w=world.lock();auto l=level.lock();
  std::shared_ptr<SourceWorldBorrowV61> observed;
  if(!w||!l||!borrow_source_campaign_release_candidate_v115(c,observed,e)||observed!=w||
     !same(c.actual_world,w->owner)||!same(c.level,l)||!same(c.application,w->application))return missing(e);
  if(startup_v115){SourceCampaignStartupPrefixV114 prefix;
   if(!borrow_source_campaign_startup_prefix_v114(prefix,e)||!same(prefix.gs,startup_gs_v115.lock())||prefix.world!=w||!prefix.level_c1_complete)return missing(e);
  }
  return true;
 }
 bool application(const dh2::loader::LevelReleaseReceiverV1& app,
  SourceCampaignCandidateBorrowV55& c,std::shared_ptr<SourceWorldBorrowV61>& w,std::string& e)const{
  return current(c,w,e)&&same(app.owner,w->application)&&app.identity==w->application->identity()?true:missing(e);
 }
 bool connection(std::shared_ptr<SourceWorldBorrowV61>& w,
  std::shared_ptr<dh2::loader::NativeLevelConnectionV25>& out,std::string& e)const{
  SourceCampaignCandidateBorrowV55 c;if(!current(c,w,e))return missing(e);
  if(startup_v115){auto gs=startup_gs_v115.lock();if(!gs||!gs->connection())return missing(e);out=gs->connection()->level_connection();}
  else{if(!w->native_level_c1_v25||!*w->native_level_c1_v25)return missing(e);out=*w->native_level_c1_v25;}
  if(!out)return missing(e);return same(out->candidate(),c.level)&&out->bindings()?true:missing(e);
 }
};
}
bool read_source_campaign_profile_v88(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 const std::string& directory,std::uint32_t slot,dh2::data::CampaignProfileFileV1& out,std::string& e){
 auto files=app?app->source_save_files_v61():nullptr;
 if(slot>=4||!files||!files->belongs_to_application(app)||!files->matches_directory(directory)){
  e="Required SAME Application private-files/jobs owner and actual Continue slot";return false;
 }
 char name[32];std::snprintf(name,sizeof(name),"dh2_%03u.savegame",slot);
 dh2::data::CampaignProfileFileV1 candidate;bool found{};
 if(!files->read_save(name,found,candidate.bytes,e))return false;
 const auto usable=[](const auto& b){return b.size()>3&&!(b[0]==255&&b[1]==255&&b[2]==255&&b[3]==255);};
 if(!found||!usable(candidate.bytes)){
  candidate.origin=dh2::data::CampaignProfileOriginV1::backup;
  if(!files->read_save(std::string(name)+".bak",found,candidate.bytes,e))return false;
  if(!found||!usable(candidate.bytes)){e="No usable campaign base/backup after actual matching-job flush";return false;}
 }
 out=std::move(candidate);e.clear();return true;
}
//Whole Level.SG_SaveAllPlayer3efc40, including live PM6c4 rereads after each
//nested Save. The count is never repaired from directory/vector membership.
bool source_campaign_save_all_players_v88(const SourceCampaignCandidateBorrowV55& c,bool disable,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> w;if(!borrow_source_campaign_condition_world_v70(c,w,e)||!w->player_manager)return missing(e);
 auto pm=w->player_manager;auto* manager=pm->manager();const auto* count=pm->count_field();
 if(!manager||!count)return missing(e);
 std::uint32_t index=0;
 while(index<static_cast<std::uint32_t>(*count)){
  //Source has unsigned outer count comparison but signed GetPlayer index;
  //negative counts cannot be admitted as a billions-long native traversal.
  if(*count<0||index>static_cast<std::uint32_t>(INT32_MAX)){e="Invalid actual PM6c4/index during source SaveAllPlayers";return false;}
  dh2::player::PlayerInfoFieldsV1* info{};
  if(!manager->get_player(static_cast<std::int32_t>(index),true,info,e)||!info)return missing(e);
  const auto id=info->character660;
  if(id){
   SourceCampaignCharacterBorrowV62 b;if(!borrow_source_campaign_character_v62(w->owner,id,b,e)||!b.character)return false;
   auto r=b.character;bool player{};if(!r->is_player(player,e))return false;
   if(player){
    //Each SG method has its genuine NULL14e8 return. Positive receivers must
    //borrow the sole published Save and its +0c authority, never another Save.
    if(!r->save_fields||!r->save_fields->save_slot14e8())return missing(e);
    if(*r->save_fields->save_slot14e8()){
     if(!r->save||!r->load||*r->save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(r->save.get())||&r->load->save()!=r->save.get()||!r->properties)return missing(e);
     const bool blocked=r->load->save_disabled();
     if(disable)r->load->source_block_store_v88(false); //source override branch
     r->save->set_player_level(r->properties->resolved[19]>>8);
     r->save->source_save_date_store_v88(static_cast<std::uint32_t>(std::time(nullptr)));
     if(!source_campaign_character_save_entrypoint_v68(w->owner,id,c.level->constructor_fields_v3().level110,-1,e))return false;
     //A NULL profile takes SG_Save's actual guarded return. A positive
     //profile requires the previously constructed full15 named writer.
     dh2::data::PlayerSaveWriteServicesV1 services;
     if(r->load->profile().identity){
      auto writer=r->profile_bootstrap?r->profile_bootstrap->campaign_writer():nullptr;
      if(!writer||!writer->ready())return missing(e);services=writer->write_services();
     }
     dh2::data::PlayerSaveWriteOwnerV1 writer(r->load,std::move(services));
     if(!writer.save(e))return false; //preserve reached blocked/store prefix
     r->load->source_block_store_v88(blocked);
    }
   }
  }
  ++index;
 }
 e.clear();return true;
}
class SourceCampaignReleaseV88 final {
 friend bool source_campaign_release_complete_v88(const std::shared_ptr<SourceCampaignReleaseV88>&,bool&,bool&,std::string&);
 friend bool bind_source_campaign_release_v88(const SourceCampaignCandidateBorrowV55&,SourceCampaignReleaseServicesV88,std::shared_ptr<SourceCampaignReleaseV88>&,std::string&,const SourceCampaignStartupPrefixV114*);
 std::shared_ptr<ReleaseTransportV88> transport_;
 std::shared_ptr<dh2::loader::LevelUnloadSourceV1> unload_;
 std::shared_ptr<dh2::loader::LevelDestroySourceV1> destroy_;
};
bool source_campaign_release_complete_v88(const std::shared_ptr<SourceCampaignReleaseV88>& owner,bool& unload,bool& destroy,std::string& e){
 unload=destroy=false;if(!owner||!owner->unload_||!owner->destroy_)return missing(e);
 unload=owner->unload_->complete();destroy=owner->destroy_->complete();e.clear();return true;
}
bool borrow_source_campaign_release_candidate_v115(SourceCampaignCandidateBorrowV55& c,std::shared_ptr<SourceWorldBorrowV61>& world,std::string& e){
 if(borrow_source_campaign_candidate_runtime_v61(c,e))return borrow_source_campaign_condition_world_v70(c,world,e);
 //Only actual retained failed startup has this typed C1 receipt. Never turn a
 //missing gameplay graph into a new/adopted source candidate or ready stage.
 SourceCampaignStartupPrefixV114 prefix;
 if(!borrow_source_campaign_startup_prefix_v114(prefix,e)||!prefix.world||!prefix.level_c1_complete||!prefix.published_level34||
    !borrow_source_campaign_startup_candidate_v114(prefix,c,e))return false;
 world=prefix.world;e.clear();return true;
}
bool prepare_source_campaign_startup_release_v115(const SourceCampaignStartupPrefixV114& prefix,std::string& e){
 if(!prefix.world||!prefix.level_c1_complete||!prefix.gs||!prefix.published_level34){e="Required actual published complete C1 startup release receipt";return false;}
 if(prefix.world->release_v88){e.clear();return true;}
 SourceCampaignCandidateBorrowV55 c;
 if(!borrow_source_campaign_startup_candidate_v114(prefix,c,e))return false;
 struct Provider{};auto owner=std::make_shared<Provider>();SourceCampaignReleaseServicesV88 services;
 services.unload.owner=owner;services.destroy.owner=owner;std::shared_ptr<SourceCampaignReleaseV88> actual;
 return bind_source_campaign_release_v88(c,std::move(services),actual,e,&prefix);
}
bool prepare_source_campaign_release_v108(const SourceCampaignCandidateBorrowV55& c,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;
 if(!borrow_source_campaign_condition_world_v70(c,world,e))return false;
 if(world->release_v88){e="Campaign source release journals already enrolled";return false;}
 //Independent native retention only. All real singleton/receiver providers
 //are composed below from the actual existing graph at their source reach.
 struct Provider{};auto owner=std::make_shared<Provider>();
 SourceCampaignReleaseServicesV88 services;services.unload.owner=owner;services.destroy.owner=owner;
 std::shared_ptr<SourceCampaignReleaseV88> actual;
 return bind_source_campaign_release_v88(c,std::move(services),actual,e);
}
bool bind_source_campaign_release_v88(const SourceCampaignCandidateBorrowV55& c,SourceCampaignReleaseServicesV88 input,
 std::shared_ptr<SourceCampaignReleaseV88>& out,std::string& e,const SourceCampaignStartupPrefixV114* startup){
 using namespace dh2;using namespace loader;
 std::shared_ptr<SourceWorldBorrowV61> w;
 SourceCampaignCandidateBorrowV55 observed;
 if(out||!borrow_source_campaign_release_candidate_v115(observed,w,e)||w->release_v88||
    !same(c.actual_world,observed.actual_world)||!same(c.level,observed.level)||c.objects!=observed.objects||c.properties!=observed.properties||
    !w->player_manager||!input.unload.owner||!input.destroy.owner)return missing(e);
 auto gs=startup?startup->gs:(w->native_gslevel_v27?*w->native_gslevel_v27:std::shared_ptr<NativeGSLevelRuntimeV27>{});
 if(!gs)return missing(e);
 if(startup&&(!startup->level_c1_complete||startup->world!=w||!same(startup->published_level34,c.level)||
    !gs->connection()||gs->connection()->level_connection()!=startup->level_connection))return missing(e);
 //Provider tables must not create GS->journal->containing World/Level/App
 //cycles. Real leaf closures borrow these owners only synchronously.
 for(const auto& pin:{input.unload.owner,input.destroy.owner}){
  if((!pin.owner_before(c.actual_world)&&!c.actual_world.owner_before(pin))||(!pin.owner_before(c.level)&&!c.level.owner_before(pin))||(!pin.owner_before(c.application)&&!c.application.owner_before(pin))||(!pin.owner_before(w)&&!w.owner_before(pin))){e="Release providers must be independent of containing World/Level/Application";return false;}
 }
 auto t=std::make_shared<ReleaseTransportV88>();t->world=w;t->level=c.level;t->players=w->player_manager;
 if(startup){t->startup_v115=true;t->startup_gs_v115=gs;}
 t->unload_providers=std::move(input.unload.owner);t->destroy_providers=std::move(input.destroy.owner);
 auto& s=input.unload;auto& d=input.destroy;s.owner=t;d.owner=t;
 if(!bind_native_level_ui_release_v107(d,e))return false;
 //These are supplied by this connection, never by an independent façade.
 if(s.quicksave||s.camera||s.camera_d0||s.application||s.objects||s.players||s.player_update||s.random_globals||s.save_all_players||s.network_uninit_level||s.menu_manager||s.menu_by_name||s.push_menu||s.pop_menu||s.menu_visible||s.sound_manager||s.stop_all_sounds||
    d.application||d.objects||d.clear_group_info||d.cached_character_oids||d.save_runtime||d.lua_cache||d.lua_script_d1||d.scripts||d.script_flush||d.unlock_objects||d.event_destructors||d.cleanup_skills||d.visual_fx||d.fx_flush_libraries||d.items||d.item_flush){
  e="Campaign release shared-owner slots already bound; refuse mixed authorities";return false;
 }
 const auto source_barrier=[t](const auto& level,auto& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  return t->current(c,w,e)&&same(level,c.level)?require_source_campaign_quiescence_v104(w->owner,e):missing(e);};
 auto prior_unload_barrier=std::move(s.quiesce_delivery);s.quiesce_delivery=[source_barrier,prior=std::move(prior_unload_barrier)](const auto& level,auto& e){return (!prior||prior(level,e))&&source_barrier(level,e);};
 auto prior_destroy_barrier=std::move(d.require_quiescent_delivery);d.require_quiescent_delivery=[source_barrier,prior=std::move(prior_destroy_barrier)](const auto& level,auto& e){return (!prior||prior(level,e))&&source_barrier(level,e);};
 s.menu_manager=[t](LevelReleaseReceiverV1& out,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->current(c,w,e))return false;std::shared_ptr<ui::MenuStackOwnerV1> manager;
  if(!borrow_native_process_menu_directory_v104(w->application,manager,e)||!manager)return missing(e);
  out={manager,reinterpret_cast<std::uintptr_t>(manager.get())};e.clear();return true;
 };
 s.menu_by_name=[t](const LevelReleaseReceiverV1& manager,const char* name,LevelReleaseReceiverV1& out,std::string& e){
  out={};SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!t->current(c,w,e))return false;
  auto receipt=std::make_shared<ui::CapturedMenuLeaseV101>();
  if(!capture_native_process_menu_v104(w->application,name,*receipt,e))return false;
  if(!receipt->identity()){e.clear();return true;} //actual observed NULL lookup
  if(!same(manager.owner,receipt->manager)||manager.identity!=reinterpret_cast<std::uintptr_t>(receipt->manager.get()))return missing(e);
  out={receipt,receipt->identity()};e.clear();return true;
 };
 const auto menu_operation=[](const LevelReleaseReceiverV1& manager,const LevelReleaseReceiverV1& menu,bool pop,std::string& e){
  if(!manager||!menu)return missing(e);auto receipt=std::static_pointer_cast<ui::CapturedMenuLeaseV101>(menu.owner);
  if(receipt->identity()!=menu.identity||!same(manager.owner,receipt->manager)||manager.identity!=reinterpret_cast<std::uintptr_t>(receipt->manager.get()))return missing(e);
  return pop?receipt->pop(e):receipt->push(e);
 };
 s.push_menu=[menu_operation](const auto& manager,const auto& menu,auto& e){return menu_operation(manager,menu,false,e);};
 s.pop_menu=[menu_operation](const auto& manager,const auto& menu,auto& e){return menu_operation(manager,menu,true,e);};
 s.menu_visible=[](const LevelReleaseReceiverV1& menu,bool& visible,std::string& e){if(!menu)return missing(e);
  auto receipt=std::static_pointer_cast<ui::CapturedMenuLeaseV101>(menu.owner);return receipt->identity()==menu.identity?receipt->visible(visible,e):missing(e);};
 s.sound_manager=[t](LevelReleaseReceiverV1& out,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->current(c,w,e))return false;auto receipt=std::make_shared<audio::AudioApplicationBorrowV42>();
  if(!borrow_actual_application_audio_v42(*receipt,e)||!receipt->identity())return missing(e);
  out={receipt,receipt->identity()};e.clear();return true;
 };
 s.stop_all_sounds=[](const LevelReleaseReceiverV1& sound,std::int32_t fade,std::string& e){if(!sound)return missing(e);
  auto receipt=std::static_pointer_cast<audio::AudioApplicationBorrowV42>(sound.owner);
  return receipt->identity()==sound.identity?stop_application_sounds_v101(*receipt,fade,e):missing(e);
 };
 s.quicksave=[t,hosting=std::move(input.positive_local_hosting)](auto& out,auto& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  return t->current(c,w,e)&&borrow_source_campaign_quicksave_v88(c,out,e,hosting);};
 s.camera=[t](std::uintptr_t id,LevelReleaseReceiverV1& out,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->current(c,w,e))return false;auto receipt=std::make_shared<SourceCampaignCameraReleaseBorrowV88>();
  if(!borrow_source_campaign_camera_release_v88(c,id,*receipt,e))return false;out={receipt,id};return true;};
 s.camera_d0=[](const LevelReleaseReceiverV1& actual,std::string& e){
  if(!actual)return missing(e);const auto receipt=std::static_pointer_cast<SourceCampaignCameraReleaseBorrowV88>(actual.owner);
  return receipt->identity==actual.identity&&receipt->deleting_destructor?receipt->deleting_destructor(e):missing(e);};
 s.application=[t](LevelReleaseReceiverV1& out,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->current(c,w,e))return false;out={w->application,w->application->identity()};e.clear();return true;};
 s.objects=[t](const LevelReleaseReceiverV1& app,auto& out,auto& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->application(app,c,w,e)||!w->canonical_world)return missing(e);out=w->canonical_world->manager_lease;return bool(out)?true:missing(e);};
 s.network_uninit_level=[t](world::CanonicalObjectManagerV1& actual,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->current(c,w,e)||c.objects.get()!=&actual)return missing(e);
  auto packets=network::packet_slot_registry_process_v88();
  if(!packets||!packets->unregister_slot(3,e))return false;
  actual.source_network_initialized1ac_v88()=0;e.clear();return true;
 };
 s.players=[t](const LevelReleaseReceiverV1& app,auto& out,auto& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->application(app,c,w,e)||!same(w->player_manager,t->players)||!t->players->manager())return missing(e);
  out={t->players,t->players->manager()};e.clear();return true;};
 s.player_update=[t](player::PlayerManagerOwnerV1& actual,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->current(c,w,e)||&actual!=t->players->manager()||!w->source_pm_update_provider_v70||!w->source_pm_update_v70)return missing(e);
  return w->source_pm_update_v70(e);};
 s.save_all_players=[t](const auto& level,bool flag,auto& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  return t->current(c,w,e)&&same(level,c.level)?source_campaign_save_all_players_v88(c,flag,e):missing(e);};
 if(!s.real_time)s.real_time=[](std::uint32_t& out,std::string& e){
  //Native backend of Timer.getRealTime, at this reached source call. It does
  //not advance/publish another Application frame or change simulation dt.
  out=std::uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());e.clear();return true;};
 s.random_globals=[t](LevelReleaseRandomV1& out,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->current(c,w,e))return false;auto random=w->application->source_random_v62();if(!random)return missing(e);
  out={random,&random->channel(0).seed,&random->channel(1).seed};e.clear();return true;};
 d.application=s.application;d.objects=s.objects;
 if(!d.object_flush)d.object_flush=[t](world::CanonicalObjectManagerV1& actual,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->current(c,w,e)||c.objects.get()!=&actual||!w->source_object_lifecycle_v108){e="Required SAME completed native ObjectManager lifecycle producer before whole Flush";return false;}
  if(!actual.flush_source_v1(*w->source_object_lifecycle_v108,e))return false;
  //Unassigned partial C1 storage is not reached by the source map walk. Its
  //genuine qualified resource unwind precedes Scene/PF destruction, retaining
  //the failed record whenever actual class/alias proof refuses retirement.
  return !w->noncharacter_release_journal_v89||w->noncharacter_release_journal_v89->retire_failed_constructor_prefixes(e);
 };
 d.cleanup_skills=[t](const auto& level,auto& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->current(c,w,e)||!same(level,c.level)||!c.objects)return missing(e);
  //Whole3ef3fc over actual list60. Each receiver uses its original pending
  //script-map erase/search and whole selected AIUnLoadScriptProcess(true).
  for(std::size_t i=0;i<c.objects->characters().size();++i){const auto id=c.objects->characters()[i];
   if(!source_campaign_character_unload_script_v105(w->owner,id,true,e))return false;
  }
  e.clear();return true;
 };
 d.items=[t](std::shared_ptr<character::CharacterLootItemManagerV8>& out,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->current(c,w,e)||!w->prepared_items_v88||!w->prepared_items_v88->items())return missing(e);
  out=std::shared_ptr<character::CharacterLootItemManagerV8>(w->prepared_items_v88,&w->prepared_items_v88->items()->pool().manager());e.clear();return true;
 };
 d.item_flush=[t](character::CharacterLootItemManagerV8& manager,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->current(c,w,e)||!w->prepared_items_v88||!w->prepared_items_v88->items()||&w->prepared_items_v88->items()->pool().manager()!=&manager)return missing(e);
  return release_source_campaign_items_v88(c,e);
 };
 d.visual_fx=[t](LevelReleaseReceiverV1& out,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->current(c,w,e))return false;auto libraries=w->application->source_fx_libraries_v63();
  if(!libraries||!libraries->belongs_to(w->application))return missing(e);out={libraries,libraries->identity()};e.clear();return true;
 };
 d.fx_flush_libraries=[t](const LevelReleaseReceiverV1& receiver,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->current(c,w,e))return false;auto libraries=w->application->source_fx_libraries_v63();
  return libraries&&same(receiver.owner,libraries)&&receiver.identity==libraries->identity()?flush_source_campaign_fx_libraries_v88(c,e):missing(e);
 };
 d.event_destructors=[t](const std::shared_ptr<GameEventManagerV50>& manager,GameEventNativeDestructionV1& out,std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!t->current(c,w,e)||!manager)return missing(e);
  GameEventLevelFieldsV50 fields;if(!c.level->game_event_fields_v50(fields,e)||!fields.owner194||!same(*fields.owner194,manager))return missing(e);
  const auto storage=std::weak_ptr<GameEventManagerV50>(manager);
  out.owner=t;out.objective_d1=[t,storage](GameEventObjectiveV50& objective,std::string& e){
   SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;auto manager=storage.lock();
   if(!t->current(c,w,e)||!manager)return missing(e);
   if(w->game_events_v75)return w->game_events_v75->destroy_objective_native_v88(manager,objective,e);
   //A bounded-load C1 prefix can exist before any runtime is enrolled. Its
   //source derived D1 is still base-vptr stores plus BXLR, not Compile or
   //Unregister. The actual private Objective has no published V75 adapter.
   bool owned=false;for(const auto& event:manager->events())if(event)for(const auto& item:event->objectives())if(item.get()==&objective)owned=true;
   if(const auto* pending=manager->pending_event())for(const auto& item:pending->objectives())if(item.get()==&objective)owned=true;
   if(!owned||objective.fields().type4<0||objective.fields().type4>12)return missing(e);
   e.clear();return true;
  };e.clear();return true;
 };
 d.unlock_objects=[t](const auto& level,auto& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->current(c,w,e)||!same(level,c.level)||!c.objects)return missing(e);
  //Source Add34b438/34b444 produces this SAME list60 order. Distinct list70
  //tracks changed characters and must never substitute for this source walk.
  for(auto id:c.objects->characters()){
   SourceCampaignCharacterBorrowV62 b;if(!borrow_source_campaign_character_v62(w->owner,id,b,e)||!b.character||!b.character->actor)return missing(e);
   auto* lock=b.character->actor->source_lock29_v88();if(!lock)return missing(e);if(*lock)*lock=0;
  }
  e.clear();return true;
 };
 d.scripts=[t](auto& out,auto& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->current(c,w,e))return false;out=w->application->source_script_manager_v52();return bool(out)?true:missing(e);};
 d.script_flush=[t](ScriptManagerOwnerV52& actual,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->current(c,w,e)||w->application->source_script_manager_v52().get()!=&actual)return missing(e);
  return actual.flush_source_v88([](std::string& e){return source_store_menu_cut_screen_v62(0,e);},e);};
 d.clear_group_info=[](std::string& e){character::source_character_clear_group_info_v87();e.clear();return true;};
 d.clear_all_aggro=[t](std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->current(c,w,e)||!c.objects)return missing(e);
  const auto clear=[&](std::uintptr_t id){SourceCampaignCharacterBorrowV62 b;
   if(!borrow_source_campaign_character_v62(w->owner,id,b,e)||!b.character||!b.character->actor)return missing(e);
   auto* trees=b.character->actor->source_aggro_v84();return trees?trees->source_clear_storage_v108(e):missing(e);
  };
  //3cd3a4 source list60 pass, followed by map4 ObjectHandle conversion pass.
  for(const auto id:c.objects->characters())if(!clear(id))return false;
  std::int32_t key{};const world::CanonicalObjectBorrowV1* object{};
  bool found=c.objects->source_ordered_begin_v38(key,object);
  while(found){const auto captured=key;
   if(object){target_providers::Handle16 handle;const world::CanonicalObjectBorrowV1* resolved{};
    if(!c.objects->get_handle(key,handle,e)||!c.objects->resolve_handle_v4(handle,false,resolved,{},e))return false;
    if(resolved){std::uintptr_t character{};
     if(!resolved->as_character||!resolved->as_character(resolved->context,character,e))return missing(e);
     if(character&&!clear(character))return false;
    }
   }
   found=c.objects->source_ordered_next_v38(captured,key,object);
  }
  e.clear();return true;
 };
 d.projectiles=[](LevelReleaseReceiverV1& out,std::string& e){auto actual=world::projectile_manager_process_v108();
  out={actual,actual->identity()};e.clear();return true;
 };
 d.projectile_flush=[t](const LevelReleaseReceiverV1& receiver,std::string& e){auto actual=world::projectile_manager_process_v108();
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!same(receiver.owner,actual)||receiver.identity!=actual->identity()||!t->current(c,w,e))return missing(e);
  return actual->flush_for_retirement_v111(c.objects,[world=w->owner](std::string& e){return require_source_campaign_quiescence_v104(world,e);},e);
 };
 d.clear_concurrent_ai=[](std::string& e){auto actual=character::character_loading_queue_v105();
  if(!actual||dh2_character_deferred_queue_clear_v108(actual.get())!=0){e="ClearConcurrentAI requires SAME idle process map";return false;}
  e.clear();return true;
 };
 d.animation_sets=[t](LevelReleaseReceiverV1& out,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->current(c,w,e)||!w->camera_application||!w->camera_application->animation_manager())return missing(e);
  auto actual=w->camera_application->animation_manager();out={actual,reinterpret_cast<std::uintptr_t>(actual.get())};e.clear();return true;
 };
 d.animation_sets_flush=[t](const LevelReleaseReceiverV1& receiver,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->current(c,w,e)||!w->camera_application)return missing(e);auto actual=w->camera_application->animation_manager();
  if(!actual||!same(receiver.owner,actual)||receiver.identity!=reinterpret_cast<std::uintptr_t>(actual.get()))return missing(e);
  actual->source_flush_v17();e.clear();return true;
 };
 if(!d.zoom_set_camera_null)d.zoom_set_camera_null=[t](const LevelReleaseReceiverV1& app,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->application(app,c,w,e)||!w->camera_application||!w->camera_application->zoom_handler50())return missing(e);
  return w->camera_application->zoom_handler50()->set_camera({},e);
 };
 if(!d.clean_glitch)d.clean_glitch=[t](const LevelReleaseReceiverV1& app,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  return t->application(app,c,w,e)&&clean_native_graphics_v50(e);
 };
 if(!d.retire_after_unpublication)d.retire_after_unpublication=[t](const auto& level,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->current(c,w,e)||!same(c.level,level)||!c.objects||!c.objects->characters().empty()||!c.objects->modules().empty()||
     c.objects->source_count50()!=0||c.objects->source_map_size1c_v38()!=1||
     !c.objects->source_active2c_v102().empty()||!c.objects->source_pending34_v102().empty()||!c.objects->source_deletion3c_v102().empty()){
   e="Level D1 native retirement precedes genuine manager unpublication";return false;
  }
  auto transport=c.receiver_transport_v69.lock();
  if(!transport||transport->retained_count()!=0||!w->noncharacter_release_journal_v89||w->noncharacter_release_journal_v89->retained_prefix_count()!=0){
   e="Level D1 native retirement still has real transport/class C1 journals";return false;
  }
  //Root's additional catalog/factory loan expires only after real class D0,
  //manager erase, transport erase, and independent journal retirement above.
  if(w->projectile_retire_receipts_v111&&!w->projectile_retire_receipts_v111(e))return false;
  w->projectile_runtime_v112.reset();w->projectile_precache_v96.reset();w->projectile_retire_receipts_v111={};
  w->noncharacter_owners_v105.reset();e.clear();return true;
 };
 d.cached_character_oids=[](LevelCachedCharacterOIDsV1& out,std::string& e){auto owner=character::character_oid_cache_process_v81();if(!owner)return missing(e);
  out={owner,&owner->source_tree_for_level_d1_v88()};e.clear();return true;};
 d.save_runtime=[t](const auto& receiver,auto& out,auto& e){std::shared_ptr<SourceWorldBorrowV61> w;std::shared_ptr<NativeLevelConnectionV25> connection;
  if(!t->connection(w,connection,e))return false;out=connection->bindings()->save();return same(receiver,out)?true:missing(e);};
 d.lua_cache=[t](const LevelReleaseReceiverV1& app,auto& out,auto& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->application(app,c,w,e)||!w->level_application)return missing(e);out=w->level_application->script_cache();return bool(out)?true:missing(e);};
 d.lua_script_d1=[t](const auto& receiver,auto& e){std::shared_ptr<SourceWorldBorrowV61> w;std::shared_ptr<NativeLevelConnectionV25> connection;
  if(!t->connection(w,connection,e))return false;auto script=connection->bindings()->script();return same(receiver,script)?script->close_source_v88(e):missing(e);};
 if(!bind_source_campaign_scene_pf_release_v106(c,d,e))return false;
 if(!bind_level_batch_release_source_v98(d,c.level,e))return false;
 auto owner=std::make_shared<SourceCampaignReleaseV88>();owner->transport_=t;
 owner->unload_=std::make_shared<LevelUnloadSourceV1>(c.level,std::move(s));
 owner->destroy_=std::make_shared<LevelDestroySourceV1>(c.level,std::move(d));
 //The existing GS lifecycle remains the single destructor/current-slot
 //authority. Loader journals perform source prefix stores once, on its thread.
 const auto unload=[journal=owner->unload_](const auto& level,auto& e){return journal->execute(level,e);};
 const auto destroy=[journal=owner->destroy_](const auto& level,auto& e){return journal->execute(level,e);};
 if(startup&&gs->connection()->lifecycle().stage()==GSLevelLifecycleStageV2::failed){
  if(!gs->bind_failed_constructor_release_v115(unload,destroy,e))return false;
 }else if(!gs->bind_release_services_v88(unload,destroy,e))return false;
 w->release_v88=owner;out=std::move(owner);e.clear();return true;
}
}
