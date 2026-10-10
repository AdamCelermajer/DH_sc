#include "../app/src/main/cpp/source_campaign_cancel_unload_v135.hpp"
#include <level_unload_source_v1.hpp>
#include <stage_loader_v46_early.hpp>
#include <iostream>
#include <stdexcept>
using namespace dh2;
namespace {
unsigned checks{};
void check(bool value,const std::string& why){++checks;if(!value)throw std::runtime_error(why);}
struct World {
 std::shared_ptr<model_renderer::SourceCampaignAdmissionV104> admission_v104=
  std::make_shared<model_renderer::SourceCampaignAdmissionV104>();
 std::shared_ptr<loader::LevelUnloadSourceV1> release;
};
struct Fixture {
 std::shared_ptr<World> world=std::make_shared<World>();
 std::shared_ptr<loader::CanonicalLevelContextV1> level;
 std::shared_ptr<void> services_owner=std::make_shared<int>(1),script=std::make_shared<int>(2),save=std::make_shared<int>(3);
 std::shared_ptr<player::PlayerManagerOwnerV1> players;
 std::shared_ptr<world::CanonicalObjectManagerV1> objects=std::make_shared<world::CanonicalObjectManagerV1>(world::CanonicalObjectManagerServicesV1{});
 std::shared_ptr<void> menus=std::make_shared<int>(4),loading_menu=std::make_shared<int>(5),sound=std::make_shared<int>(6);
 std::uint32_t big_i=7,big_v=9,seed=77,synced_seed=88,debug_count{},module_id{};
 std::uint8_t byte_b4=3;
 unsigned cleans{},stage_stops{},unload_stops{},menu_pushes{},menu_pops{},pm_updates{},network_uninit{},d1_calls{};
 bool fail_unload_sound{},throw_unload_sound{};
 data::LevelTables tables;
 std::string error;
 static bool player_service(void*,const player::PlayerManagerRequestV1& q,player::PlayerManagerResponseV1& out,std::string& e){
  if(q.operation==player::PlayerManagerOperationV1::construct_player_info){*q.player={};return true;}
  if(q.operation==player::PlayerManagerOperationV1::online_enabled){out.value=0;return true;}
  e="Unexpected test PlayerManager operation";return false;
 }
 Fixture(){
  loader::LevelSourceRequestV1 request;request.identity="CancellationFixture";request.definition="cancel.mlx";
  check(loader::CanonicalLevelContextV1::create(request,services_owner,level,error),error);
  loader::LevelConstructorServicesV3 constructor;constructor.application=services_owner;
  constructor.debug_level_load_count=&debug_count;constructor.module_id_global=&module_id;constructor.levels=&tables;
  constructor.construct_script=[this](auto,bool,auto& out,auto&){out=script;return true;};
  constructor.script_assign_path=[](const auto&,const char*,std::size_t,auto&){return true;};
  constructor.script_load=[](const auto&,const char*,auto&){return true;};
  constructor.online_byte5=[](auto& value,auto&){value=0;return true;};
  constructor.allocate_save=[this](auto& out,auto&){out=save;return true;};
  constructor.construct_save=[](const auto&,const auto&,auto&){return true;};
  check(level->construct_source_v3({"cancel.mlx",0,0,0,0,0,0,-1,0},std::move(constructor),error),error);
  players=std::make_shared<player::PlayerManagerOwnerV1>(player::PlayerManagerServicesV1{nullptr,player_service});
  check(players->initialize(error),error);
  player::PlayerQuickSaveServicesV29 qs;qs.owner=services_owner;qs.players=players.get();
  auto quick=std::make_shared<player::PlayerLevelQuickSaveV29>(std::move(qs));
  loader::LevelUnloadServicesV1 unload;unload.owner=services_owner;
  unload.quiesce_delivery=[weak=std::weak_ptr<World>(world)](const auto&,auto& e){auto w=weak.lock();return w&&w->admission_v104->require_closed_quiescent(e);};
  unload.quicksave=[quick](auto& out,auto&){out=quick;return true;};
  unload.menu_manager=[this](auto& out,auto&){out={menus,reinterpret_cast<std::uintptr_t>(menus.get())};return true;};
  unload.menu_by_name=[this](const auto&,const char*,auto& out,auto&){out={loading_menu,reinterpret_cast<std::uintptr_t>(loading_menu.get())};return true;};
  unload.push_menu=[this](const auto&,const auto&,auto&){++menu_pushes;return true;};
  unload.menu_visible=[](const auto&,bool& value,auto&){value=true;return true;};
  unload.pop_menu=[this](const auto&,const auto&,auto&){++menu_pops;return true;};
  unload.save_all_players=[](const auto&,bool,auto&){return true;};
  unload.sound_manager=[this](auto& out,auto&){out={sound,reinterpret_cast<std::uintptr_t>(sound.get())};return true;};
  unload.stop_all_sounds=[this](const auto& actual,int fade,auto& e){
   check(actual.owner==sound&&fade==500,"Unload changed SAME sound/500");++unload_stops;
   if(throw_unload_sound)throw std::runtime_error("Delivered unload sound exception");
   if(fail_unload_sound){e="Delivered unload sound failure";return false;}return true;
  };
  unload.application=[this](auto& out,auto&){out={services_owner,reinterpret_cast<std::uintptr_t>(services_owner.get())};return true;};
  unload.objects=[this](const auto&,auto& out,auto&){out=objects;return true;};
  unload.network_uninit_level=[this](auto&,auto&){++network_uninit;return true;};
  unload.players=[this](const auto&,auto& out,auto&){out=players;return true;};
  unload.player_update=[this](auto&,auto&){++pm_updates;return true;};
  unload.real_time=[](std::uint32_t& out,auto&){out=123;return true;};
  unload.random_globals=[this](auto& out,auto&){out={services_owner,&seed,&synced_seed};return true;};
  world->release=std::make_shared<loader::LevelUnloadSourceV1>(level,std::move(unload));
 }
 loader::LifecycleServicesV36 loading(){
  loader::Stage0LevelBorrowV46 actual;
  check(loader::borrow_stage0_level_v46(level,actual,error),error);
  loader::Stage0ApplicationBorrowV46 app{services_owner,services_owner,reinterpret_cast<std::uintptr_t>(services_owner.get()),&big_i,&big_v,&byte_b4};
  loader::Stage0ServicesV46 stage;
  stage.clean_glitch=[this](const auto&,auto&){++cleans;return true;};
  stage.sound_manager_owner=sound;stage.sound_manager_identity=reinterpret_cast<std::uintptr_t>(sound.get());
  stage.stop_all_sounds=[this](auto id,int fade,auto&){check(id==reinterpret_cast<std::uintptr_t>(sound.get())&&fade==500,"Stage0 changed sound receiver");++stage_stops;return true;};
  loader::LifecycleServicesV36 out;out.stage_body[0]=loader::stage0_body_v46(std::move(actual),std::move(app),std::move(stage));
  out.publish_progress=[](auto,auto,auto&){return true;};
  out.cancel_and_unload=model_renderer::source_campaign_cancel_unload_v135(std::weak_ptr<World>(world),std::weak_ptr<loader::CanonicalLevelContextV1>(level),
   [](const auto& w,const auto& l,auto& e){return w->release->execute(l,e);});
  return out;
 }
 std::unique_ptr<loader::LifecycleV36> driver(){
  loader::LifecycleBorrowV36 fields;check(loader::borrow_lifecycle_fields_v36(level,fields,error),error);
  return std::make_unique<loader::LifecycleV36>(fields.fields,fields.actual_level_owner,std::vector<std::shared_ptr<const void>>{world->release},loading());
 }
};
void cancel_at(unsigned ticks){
 Fixture f;auto source=f.driver();
 for(unsigned i=0;i<ticks;++i)check(source->tick()==loader::LifecycleStatusV36::loading,"Early loading tick failed");
 check(f.level->constructor_fields_v3().field130==ticks,"Fixture did not reach requested early phase");
 const auto clean=f.cleans,stops=f.stage_stops;
 loader::GSLevelFieldsV2<loader::CanonicalLevelContextV1> gs_fields;std::shared_ptr<loader::CanonicalLevelContextV1> current;
 loader::GSLevelServicesV2<loader::CanonicalLevelContextV1> gs_services;
 gs_services.flush_animation_sets=[](auto&){return true;};
 gs_services.allocate_level=[&](auto& out,auto&){out=f.level;return true;};
 gs_services.construct_level=[&](const auto&,const auto&,auto& out,auto&){out=f.level;return true;};
 gs_services.get_menu=[](const char*,auto& out,auto&){out=0;return true;};
 gs_services.online_byte5=[](auto& value,auto&){value=0;return true;};
 gs_services.unload_level=[&](const auto& l,auto& e){return f.world->release->execute(l,e);};
 gs_services.destroy_level=[&](const auto&,auto&){++f.d1_calls;return true;};
 loader::GSLevelLifecycleV2<loader::CanonicalLevelContextV1> gs(gs_fields,current,std::move(gs_services));
 check(gs.construct(),gs.error());
 source->request_cancel();check(source->tick()==loader::LifecycleStatusV36::cancelled,source->diagnostics().error);
 check(f.cleans==clean&&f.stage_stops==stops,"Cancellation replayed Stage0");
 check(f.unload_stops==1&&f.menu_pushes==1&&f.menu_pops==1&&f.pm_updates==1&&f.network_uninit==1,"Authentic unload did not execute exactly once");
 check(f.world->release->complete()&&f.seed==123&&f.synced_seed==0,"Unload tail did not complete");
 check(source->diagnostics().owned_pins==0,"Successful cancellation kept loader pins");
 check(f.d1_calls==0&&current==f.level&&gs_fields.level34==f.level,"Cancellation performed GS destruction/unpublication");
 source->request_cancel();check(source->tick()==loader::LifecycleStatusV36::cancelled&&f.unload_stops==1,"Completed cancellation replayed unload");
 check(gs.destroy(),gs.error());check(f.unload_stops==1&&f.menu_pushes==1&&f.d1_calls==1&&!current&&!gs_fields.level34,"GS did not reuse cached unload then perform separate D1");
 check(gs.destroy()&&f.d1_calls==1,"GS replayed completed D1");
}
void pending_scope(){
 Fixture f;auto source=f.driver();
 {
  model_renderer::SourceCampaignDeliveryV104 delivery;bool blocked{};
  check(delivery.acquire(f.world->admission_v104,model_renderer::SourceCampaignDeliveryKindV104::input,blocked,f.error)&&!blocked,f.error);
  source->request_cancel();check(source->tick()==loader::LifecycleStatusV36::cancelling,"Outstanding delivery did not yield");
  check(f.menu_pushes==0&&f.unload_stops==0&&source->diagnostics().owned_pins==2,"Pending teardown touched source/pins");
  model_renderer::SourceCampaignDeliveryV104 rejected;
  check(rejected.acquire(f.world->admission_v104,model_renderer::SourceCampaignDeliveryKindV104::draw,blocked,f.error)&&blocked,"Cancellation did not close new draw admission");
 }
 check(source->tick()==loader::LifecycleStatusV36::cancelled&&f.unload_stops==1,"Pending cancellation did not resume after delivery");
}
void failed_journal(bool throwing){
 Fixture f;f.fail_unload_sound=!throwing;f.throw_unload_sound=throwing;auto source=f.driver();source->request_cancel();
 check(source->tick()==loader::LifecycleStatusV36::failed,"Delivered unload failure was accepted");
 const auto failure=source->diagnostics().error;
 check(f.world->release->failed_at()==loader::LevelUnloadPhaseV1::sound&&!f.world->release->complete()&&source->diagnostics().owned_pins==2,"Failure lost exact journal/pins");
 check(f.menu_pushes==1&&f.unload_stops==1&&f.menu_pops==0&&f.pm_updates==0,"Failure ran later unload suffix");
 f.fail_unload_sound=f.throw_unload_sound=false;
 source->request_cancel();check(source->tick()==loader::LifecycleStatusV36::failed&&source->diagnostics().error==failure,"Retry erased first delivered failure");
 check(f.menu_pushes==1&&f.unload_stops==1&&source->diagnostics().owned_pins==2,"Retry replayed reached unload prefix/released pins");
 check(!f.world->release->execute(f.level,f.error)&&f.error==failure&&f.unload_stops==1,"GS-style unload retry replayed failed journal");
}
void late_journal(){
 Fixture f;auto source=f.driver();auto journal=f.world->release;f.world->release.reset();
 // Production wrapper rejects missing late enrollment before entering a journal.
 auto callback=model_renderer::source_campaign_cancel_unload_v135(std::weak_ptr<World>(f.world),std::weak_ptr<loader::CanonicalLevelContextV1>(f.level),
  [](const auto& w,const auto& l,auto& e){if(!w->release){e="Actual release enrollment pending";return false;}return w->release->execute(l,e);});
 check(callback(f.error)==loader::LifecycleStepV36::failed&&!journal->complete()&&f.menu_pushes==0,"Missing journal began teardown");
 f.world->release=journal;check(callback(f.error)==loader::LifecycleStepV36::complete&&f.unload_stops==1,"Pre-delivery retry could not use reached journal");
 std::weak_ptr<World> weak=f.world;f.world.reset();check(weak.expired(),"Cancellation callback created a containing World cycle");
 check(callback(f.error)==loader::LifecycleStepV36::failed,"Expired callback authority accepted");
}
}
int main(){try{
 cancel_at(0);cancel_at(1);cancel_at(2);pending_scope();failed_journal(false);failed_journal(true);late_journal();
 std::cout<<"PASS campaign cancellation: "<<checks<<" checks; actual Level C1/Stage0/dispatcher/Unload/GS journals and admission gate; native resource leaves are explicit fixtures; phases0/1/2, pending resume, failed-prefix retention, cached unload, separate D1\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
