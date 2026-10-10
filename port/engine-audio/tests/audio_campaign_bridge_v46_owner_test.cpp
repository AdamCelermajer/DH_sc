// Reuse the genuine source GS/Level C1 fixture, but keep this probe scoped to
// typed Application publication ownership and the first missing audio leaf.
#define main static prior_native_gslevel_runtime_v27_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-function"
#include "../../level-loader/tests/native_gslevel_runtime_v27.cpp"
#pragma GCC diagnostic pop
#undef main

#include "../../windows-foundation/features/actor_frame/source_current_level_backend_v1.hpp"
#include "../integration-v42/audio_application_manager_v42.hpp"
#include "../audio_campaign_bridge_v46.hpp"
#include "../integration-v42/audio_session_control_factory_v42.hpp"
#include "../../windows-foundation/features/audio/windows_source_session_control_v1.hpp"
#include "../../level-world/application_services_owner_v5.hpp"
#include "../../level-world/gameplay_camera_application_v23.hpp"
#include "../../engine-ui/owned_hud_settings_v1.hpp"
#include "../../level-world/character_design_services.hpp"
#include "../../windows-foundation/features/audio/winmm_output.hpp"
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <stdexcept>

using namespace dh2;
using namespace dh2::audio;
using namespace dh2::loader;
using namespace dh::foundation::actor_frame;

namespace {
void require(bool value,const std::string& why){if(!value)throw std::runtime_error(why);}
struct ActorFixture {
 target_providers::Handle16 handle{0,UINT32_MAX,0};std::uint32_t type{3};
 std::uint8_t across{};std::int32_t room{-1};target_search::Object48 search{};
 std::shared_ptr<void> lease;std::string name,archetype;
 explicit ActorFixture(std::uintptr_t id){search.identity=id;}
 static int refresh(void* raw,character::skills::WorldTargetActorBorrowV1* out){
  auto& self=*static_cast<ActorFixture*>(raw);*out={};out->identity=self.search.identity;
  out->search=&self.search;out->receiver_lease=self.lease;return 0;
 }
 static bool set_name(void* raw,const char* value,std::string&){static_cast<ActorFixture*>(raw)->name=value;return true;}
 static bool set_archetype(void* raw,const char* value,std::string&){static_cast<ActorFixture*>(raw)->archetype=value;return true;}
 static bool as_character(void* raw,std::uintptr_t& out,std::string&){out=static_cast<ActorFixture*>(raw)->search.identity;return true;}
};
struct WorldRootFixture {data::AiTables ai;character::skills::CharacterWorldRuntimeV1 actor_world;WorldRootFixture():actor_world(ai){}};
std::vector<std::uint8_t> read_file(const std::filesystem::path& path){
 std::ifstream file(path,std::ios::binary);
 if(!file)throw std::runtime_error("Required exact test source file "+path.string());
 return {std::istreambuf_iterator<char>(file),{}};
}
struct ExactAssets {
 std::filesystem::path root;
 unsigned reads{};
 static bool read(void* raw,const char* uri,
                  std::shared_ptr<const std::vector<std::uint8_t>>& out,
                  std::string& error){
  auto& self=*static_cast<ExactAssets*>(raw);++self.reads;
  if(!uri){error="Required exact audio source asset URI";return false;}
  const std::string name(uri);std::filesystem::path path;
  if(name.rfind("data/pydata/",0)==0)
   path=self.root/"port/engine-audio/reference/source-bindings-v38"/name.substr(12);
  else if(name.rfind("data/sounds/",0)==0)
   path=self.root/".local-inputs/audio-v34/cache"/name.substr(12);
  else{error="Unexpected non-source audio asset URI: "+name;return false;}
  try{out=std::make_shared<const std::vector<std::uint8_t>>(read_file(path));}
  catch(const std::exception& ex){error=ex.what();return false;}
  error.clear();return true;
 }
 static bool read_optional(void* raw,const char* uri,bool& found,
                           std::shared_ptr<const std::vector<std::uint8_t>>& out,
                           std::string& error){
  found=false;out.reset();
  if(!uri){error="Required exact optional audio source asset URI";return false;}
  auto& self=*static_cast<ExactAssets*>(raw);const std::string name(uri);
  if(name.rfind("data/sounds/",0)!=0){error="Unexpected optional audio asset URI: "+name;return false;}
  const auto path=self.root/".local-inputs/audio-v34/cache"/name.substr(12);
  std::error_code ec;found=std::filesystem::exists(path,ec);
  if(ec){error="Exact audio asset existence query failed: "+ec.message();return false;}
  if(found){try{out=std::make_shared<const std::vector<std::uint8_t>>(read_file(path));}
   catch(const std::exception& ex){error=ex.what();return false;}}
  error.clear();return true;
 }
};
struct SourceDebugFiles {
 static int open(void*,const char*,std::uintptr_t* handle){if(!handle)return 1;*handle=0;return 0;}
 static int close(void*,std::uintptr_t handle){return handle?1:0;}
};
bool read_source_debug(const std::shared_ptr<character::DebugSwitches>& owner,
                       const character::DebugFileServices24& files,const char* key,
                       bool& value,std::string& error){
 if(!owner||!key||dh2_character_debug_load(owner.get(),&files)!=1){
  error="Required actual source campaign Debug Load";return false;
 }
 std::uint32_t raw{};
 if(dh2_character_debug_get(&raw,owner.get(),key,&files)!=1){
  error="Required actual source campaign Debug getter";return false;
 }
 value=raw!=0;error.clear();return true;
}
bool missing_network(bool&,std::string& error){
 error="Required actual OnlineGameState network mute global9a4863";return false;
}
bool missing_random(void*,int&){return false;}
struct TestPlaybackPublicationV46 {
 std::weak_ptr<AudioApplicationManagerV42> manager;
 std::shared_ptr<ExactAssets> assets;
 AudioGameplaySourcesV40 sources;
 std::shared_ptr<void> current_provider;
 unsigned publishes{},detaches{};
 bool fail_detach{};
 static int route_gates(void* raw,const dh2::sound::VoxPlay3DRequestV2& request,
                        dh2::sound::VoxPlay3DResponseV2& response){
  auto& self=*static_cast<TestPlaybackPublicationV46*>(raw);
  if(!self.current_provider||!self.sources.gates.invoke)return -1;
  return self.sources.gates.invoke(self.sources.gates.context,request,response);
 }
 static bool route_random(void* raw,int& value){
  auto& self=*static_cast<TestPlaybackPublicationV46*>(raw);
  return self.current_provider&&self.sources.random.next&&
         self.sources.random.next(self.sources.random.context,value);
 }
 static bool route_command(void* raw,const character::CombatSoundPlayV1& play,
                           const AudioSoundV34& sound,const AudioGroupV34& group,
                           AudioCommandV34& command,std::string& error){
  auto& self=*static_cast<TestPlaybackPublicationV46*>(raw);
  if(!self.current_provider||!self.sources.source_command){
   error="Required active typed campaign audio publication";return false;
  }
  return self.sources.source_command(self.sources.context,play,sound,group,command,error);
 }
 static bool publish(void* raw,const AudioGameplaySourcesV40& sources,
                     const std::shared_ptr<void>& provider,std::string& error){
  auto& self=*static_cast<TestPlaybackPublicationV46*>(raw);
  const auto manager=self.manager.lock();
  if(!manager||!provider||!sources.context||!sources.gates.invoke||
     !sources.random.next||!sources.source_command){
   error="Required same-manager typed campaign playback publication";return false;
  }
  auto bridge=std::static_pointer_cast<AudioCampaignBridgeV46>(provider);
  if(bridge.get()!=sources.context||bridge->manager().manager!=manager||
     bridge->manager().manager.owner_before(manager)||manager.owner_before(bridge->manager().manager)){
   error="Campaign playback publication lease is not the same retained bridge/manager";return false;
  }
  auto* runtime=manager->runtime_on_producer();AudioDeviceClockV40 clock;
  if(!runtime||!runtime->source_data_initialized()||!runtime->clock().snapshot(clock)||
     !clock.ready||!clock.rate){error="Required same V42 source runtime/mixer/output clock";return false;}
  self.sources=sources;self.current_provider=provider;++self.publishes;error.clear();return true;
 }
 static bool detach(void* raw,const AudioApplicationBorrowV42& captured,
                    const std::shared_ptr<void>& expected,bool& detached,
                    std::string& error){
  auto& self=*static_cast<TestPlaybackPublicationV46*>(raw);++self.detaches;detached=false;
  auto manager=self.manager.lock();
  if(!manager||captured.manager!=manager||captured.manager.owner_before(manager)||
     manager.owner_before(captured.manager)||!expected){
   error="Required same captured V42 manager and expected bridge lease";return false;
  }
  if(self.fail_detach){error="Test publisher detach failed before expected-owner clear";return false;}
  if(self.current_provider&&self.current_provider.get()==expected.get()&&
     !self.current_provider.owner_before(expected)&&!expected.owner_before(self.current_provider)){
   self.current_provider.reset();detached=true;
  }
  error.clear();return true;
 }
};
AudioGameplayPublisherV46 publisher_for(
 const AudioApplicationBorrowV42& captured,
 const std::shared_ptr<TestPlaybackPublicationV46>& context){
 AudioGameplayPublisherV46 publisher;publisher.target=captured;
 publisher.context_owner=context;publisher.context=context.get();
 publisher.publish=TestPlaybackPublicationV46::publish;
 publisher.detach_expected=TestPlaybackPublicationV46::detach;return publisher;
}
AudioCampaignServicesV46 campaign_services(
 const std::shared_ptr<void>& world,const std::shared_ptr<NativeGSLevelRuntimeV27>& gs,
 const std::shared_ptr<CanonicalLevelContextV1>& level,
 SourceCurrentLevelBackendV1& current,const std::shared_ptr<application::ApplicationServicesOwnerV5>& app,
 const std::shared_ptr<ui::OwnedHudSettingsV1>& settings,
 const std::shared_ptr<camera::GameplayCameraApplicationV23>& camera,
 const std::shared_ptr<AudioApplicationManagerV42>& manager,
 const std::shared_ptr<character::DebugSwitches>& debug,
 const character::DebugFileServices24& debug_io,
 const std::shared_ptr<const std::vector<std::uint8_t>>& listener_stream){
 AudioCampaignServicesV46 result;result.actual_world=world;result.actual_gs=gs;
 result.selected_level=level;result.settings_owner=settings;
 result.rng_owner=app->source_random_v62();result.camera_owner=camera;
 result.current_level=[&current](auto& out,std::string& error){
  SourceCurrentLevelBorrowV1 borrow;
  if(!current.current(borrow,error))return false;
  out=borrow.level();error.clear();return true;
 };
 result.actual_debug=[debug,debug_io](const char* key,bool& value,std::string& error){
  return read_source_debug(debug,debug_io,key,value,error);
 };
 const auto online=app->get_online_loading_v55();
 result.actual_online=[online](bool& out,std::string& error){
  if(!online){error="Required SAME actual Application Online byte5 owner";return false;}
  out=online->byte5()!=0;error.clear();return true;
 };
 result.actual_network_muted=missing_network;
 result.actual_trace=[debug=result.actual_debug](const auto&,int,std::string& error){
  bool value{};return debug("isTracingSFX",value,error);
 };
 result.actual_general=[manager](auto& out,bool& complete,std::string& error){
  return manager->source_general_v68(out,complete,error);
 };
 result.actual_emitter_modifiers=[](const auto&,const auto&,const auto&,float&,float&,int&,std::string& error){
  error="Required actual source Play emitter modifier owner";return false;
 };
 result.actual_group_volumes=[manager](auto& out,std::string& error){
  return manager->source_bus_volumes_v68(out,error);
 };
 result.actual_update_listener=[manager](const auto&,float* position,float* front,float* up,std::string& error){
  const auto& authority=manager->listener_authority_on_producer();
  if(!authority||!position||!front||!up){error="Required actual reached SetListenerPos on SAME camera node";return false;}
  std::copy_n(authority->listener.position,3,position);
  std::copy_n(authority->listener.front,3,front);
  std::copy_n(authority->listener.up,3,up);error.clear();return true;
 };
 result.actual_vox_random={nullptr,missing_random};
 result.exact_legacy_listener_stream=listener_stream;return result;
}
}

int main(int argc,char** argv){try{
 require(argc==4,"Expected source data, script cache, and save cache arguments");
 const std::filesystem::path root=std::filesystem::current_path();
 Inputs inputs(argv[1]);character::CharacterGameDesign design;std::string error;
 require(design.initialize(inputs.input,error),"Actual CharacterGameDesign: "+error);
 auto design_borrow=std::make_shared<character::CharacterGameDesign::Borrow>(design.borrow());
 require(design_borrow->levels()!=nullptr,"Actual source LevelTable");
 auto files=std::make_shared<ApplicationFilesFixture>();files->script_dir=argv[2];files->save_dir=argv[3];
 auto lua_cache=std::make_shared<scripts::LuaScriptCacheOwnerV13>(
  scripts::LuaScriptCacheServicesV13{files,files.get(),ApplicationFilesFixture::script});
 auto app=std::make_shared<application::ApplicationServicesOwnerV5>();
 auto online=app->get_online_loading_v55();
 std::uint32_t debug_level_load_count{},module_id_global{};
 LevelConstructorApplicationV4 application;application.owner=design_borrow;
 application.debug_level_load_count=&debug_level_load_count;
 application.module_id_global=&module_id_global;
 application.levels=design_borrow->levels();application.lua_cache=lua_cache;
 application.lua.owner=design_borrow;application.lua.design=*design_borrow->design();
 application.private_vm_limit=16u*1024u*1024u;application.saves.files.context=files.get();
 application.saves.files.read_file=ApplicationFilesFixture::save;application.saves.files.storage_lease=files;
 application.online_byte5=[online](std::uint8_t& value,std::string& e){value=online->byte5();e.clear();return true;};
 auto world_root=std::make_shared<WorldRootFixture>();std::shared_ptr<void> world=world_root;
 std::shared_ptr<void> navigation=std::make_shared<int>(2),floors=std::make_shared<int>(3);
 world::CanonicalObjectManagerServicesV1 object_services;
 object_services.assign_network_id=[](void*,world::CanonicalObjectBorrowV1&,std::string&){return true;};
 auto objects=std::make_shared<world::CanonicalObjectManagerV1>(object_services);
 std::shared_ptr<character::skills::CharacterWorldRuntimeV1> actor_world(world_root,&world_root->actor_world);
 constexpr std::uintptr_t actor_id=0x101010;auto actor=std::make_shared<ActorFixture>(actor_id);actor->lease=std::make_shared<int>(4);
 world::CanonicalObjectBorrowV1 object;object.identity=actor_id;object.lease=actor->lease;
 object.shared_handle=&actor->handle;object.type_f4=&actor->type;object.across_rooms87=&actor->across;
 object.room64=&actor->room;object.context=actor.get();object.set_name=ActorFixture::set_name;
 object.set_archetype=ActorFixture::set_archetype;object.as_character=ActorFixture::as_character;
 target_providers::Handle16 object_handle{};
 require(objects->add(object,"SourceTestActor","Character",0,true,object_handle,error),"Actual ObjectManager add: "+error);
 character::skills::WorldActorRegistrationV1 registration;registration.identity=actor_id;
 registration.handle_key=object_handle.key;registration.context=actor.get();registration.refresh=ActorFixture::refresh;
 registration.shared_handle=&actor->handle;require(actor_world->add(registration)==0,"Actual CharacterWorld actor registration");
 auto globals=std::make_shared<NativeGSLevelGlobalsV27>();
 auto gs=std::make_shared<NativeGSLevelRuntimeV27>(globals);
 SourceCurrentLevelGraphV1 graph;graph.root_scope=app;graph.application=design_borrow;graph.world=world;
 graph.objects=objects;graph.navigation=navigation;graph.floors=floors;graph.gs_globals=globals;
 graph.gs_runtime=gs;graph.actor_world=actor_world;graph.level_tables=design_borrow->levels();
 SourceCurrentLevelBackendV1 current(std::move(graph));
 require(!design_borrow->levels()->levels.empty(),"Actual source Level rows");
 const auto& source_row=design_borrow->levels()->levels.front();std::string source_name=source_row.file;
 for(char& ch:source_name)ch=char(std::tolower(static_cast<unsigned char>(ch)));
 LevelSourceRequestV1 request;request.identity=source_name;request.definition=source_row.file;request.seed=7;
 GSLevelArgumentsV2 arguments{source_name,0,7,1,0,1,0,-1,0};
 GSLevelServicesV2<CanonicalLevelContextV1> gs_services;
 gs_services.flush_animation_sets=[](std::string&){return true;};
 gs_services.get_menu=[](const char*,std::uintptr_t& menu,std::string&){menu=0;return true;};
 gs_services.online_byte5=application.online_byte5;
 gs_services.online_state34=[](std::int32_t&,std::string& e){e="unreached offline state";return false;};
 gs_services.unload_level=[](const auto&,std::string&){return true;};
 gs_services.destroy_level=[](const auto&,std::string&){return true;};
 require(current.construct(std::move(request),std::move(arguments),application,std::move(gs_services),
  [](std::uint32_t& value,std::string&){value=0;return true;},error),"Actual same World/GS Level C1: "+error);
 SourceCurrentLevelBorrowV1 current_level;require(current.current(current_level,error),"Actual current s_level: "+error);
 require(current_level&&current_level.level()==globals->s_level&&current_level.level()==gs->fields().level34,
         "Same source current-Level receiver and GS C1");

 auto audio_assets=std::make_shared<ExactAssets>();audio_assets->root=root;
 auto publication=std::make_shared<TestPlaybackPublicationV46>();
 publication->assets=audio_assets;
 // Native-host activity fixture for the same process lifecycle gate used by
 // V42. The test does not mark the campaign, Level, settings, or sound source
 // initialized; those remain source-owned checks below.
 require(application_audio_gate_v40().publish_activity(1,1,true,true,true,false),
         "Actual V42 lifecycle gate accepts the native host activity fixture");
 AudioGameplaySourcesV40 sources;sources.context=publication.get();
 sources.exact_assets={audio_assets.get(),ExactAssets::read,ExactAssets::read_optional};
 sources.gates={publication.get(),TestPlaybackPublicationV46::route_gates};
 sources.random={publication.get(),TestPlaybackPublicationV46::route_random};
 sources.source_command=TestPlaybackPublicationV46::route_command;
 std::string no_factory_error;
 auto default_manager=AudioApplicationManagerV42::create(sources,publication,false,no_factory_error);
 require(!default_manager&&no_factory_error.find("Required actual platform audio control factory")!=std::string::npos,
         "Windows default manager must fail closed without platform control: "+no_factory_error);
 auto manager=AudioApplicationManagerV42::create(sources,publication,false,error,
   windows_source_session_control_v1());
 require(bool(manager),"Actual Windows WinMM-backed Application audio manager: "+error);
 publication->manager=manager;
 auto* runtime=manager->runtime_on_producer();require(runtime&&runtime->source_data_initialized(),"Actual V42 source pack runtime");
 AudioDeviceClockV40 clock;bool clock_ready{};
 for(unsigned attempt=0;attempt<300;++attempt){
  if(runtime->clock().snapshot(clock)&&clock.ready&&clock.rate){clock_ready=true;break;}
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
 }
 if(!clock_ready){manager->request_output_close();std::string close_error;manager->shutdown(close_error);
  throw std::runtime_error("Actual same V42 WinMM clock did not publish: "+manager->control_error()+"; "+close_error);}

 auto settings=std::make_shared<ui::OwnedHudSettingsV1>();
 require(app->publish_source_settings4c_v67(settings,error),"Actual Application settings owner: "+error);
 auto camera=std::make_shared<camera::GameplayCameraApplicationV23>();
 require(app->publish_native_camera_services_v20(camera,error),"Actual Application camera owner: "+error);
 auto debug=std::shared_ptr<character::DebugSwitches>(dh2_character_debug_create(),dh2_character_debug_destroy);
 require(bool(debug)&&app->publish_source_debug_services_v55(debug,error),"Actual source Debug owner: "+error);
 auto debug_files=std::make_shared<SourceDebugFiles>();
 const character::DebugFileServices24 debug_io{debug_files.get(),SourceDebugFiles::open,SourceDebugFiles::close};
 auto listener_stream=std::make_shared<const std::vector<std::uint8_t>>(
   read_file(root/"port/android-native/app/src/main/assets/data/sounds_pyarray.bin"));
 auto services=campaign_services(world,gs,current_level.level(),current,app,settings,camera,manager,
                                 debug,debug_io,listener_stream);
 publication->manager=manager;
 std::weak_ptr<TestPlaybackPublicationV46> publication_lifetime=publication;
 AudioApplicationBorrowV42 captured{manager};

 // Same raw manager pointer with a different shared control block is rejected
 // before the publication callback can mutate its owner.
 auto wrong_context_block=publisher_for(captured,publication);
 wrong_context_block.target.manager=std::shared_ptr<AudioApplicationManagerV42>(manager.get(),[](auto*){});
 unsigned before=publication->publishes;
 auto rejected=AudioCampaignBridgeV46::publish_on_owner(captured,services,std::move(wrong_context_block),error);
 require(!rejected&&error=="Required publisher target is the exact captured Application audio manager"&&
         publication->publishes==before,"Reject wrong publisher control block before effects");

 auto first=AudioCampaignBridgeV46::publish_on_owner(captured,services,publisher_for(captured,publication),error);
 require(bool(first),"Typed publication through same actual V42 manager: "+error);
 require(publication->publishes==1&&publication->current_provider.get()==first.get()&&
         !publication->current_provider.owner_before(std::static_pointer_cast<void>(first))&&
         !std::static_pointer_cast<void>(first).owner_before(publication->current_provider),
         "Publisher retains exact first bridge lease");
 auto newer=AudioCampaignBridgeV46::publish_on_owner(captured,services,publisher_for(captured,publication),error);
 require(bool(newer)&&publication->publishes==2&&publication->current_provider.get()==newer.get(),
         "New source owner replaces publication on same process manager");
 require(first->unpublish_expected(error)&&!publication->current_provider.owner_before(std::static_pointer_cast<void>(newer))&&
         !std::static_pointer_cast<void>(newer).owner_before(publication->current_provider),
         "Stale expected-owner detach leaves newer publication intact");
 publication->fail_detach=true;
 require(!newer->unpublish_expected(error)&&error=="Test publisher detach failed before expected-owner clear"&&
         publication->current_provider.get()==newer.get()&&!publication_lifetime.expired(),
         "Detach failure retains publication and callback-context pin for retry");
 publication->fail_detach=false;
 require(!publication_lifetime.expired(),"Live bridge retains its callback-context owner until destruction");

 // C1's actual loading status is not synthesized. The original Play3D owner
 // may therefore take its native status early return; this does not promote
 // the Level to loaded or claim playback readiness.
 require(publication->sources.source_command!=nullptr,"Published actual source command callback");
 const auto& sound=runtime->catalog().sounds().front();const auto& group=runtime->catalog().groups().front();
 character::CombatSoundPlayV1 play;play.manager=manager->identity();play.target=actor_id;
 std::int64_t event_ns{};std::string time_error;
 require(dh::foundation::audio::winmm_monotonic_ns(event_ns,time_error),"Actual source QPC timestamp: "+time_error);
 const bool source_submit=manager->submit_actual_play(play,event_ns,error);
 require(runtime->owned_voices_v101()==0,"No voice accepted without an actual source listener/emitter path");
 CanonicalLevelContextV1::LoadingFieldsV26 loading_fields;
 require(current_level.level()->loading_fields_v26(loading_fields,error)&&loading_fields.state130,
         "Actual C1 loading-status field remains owned by the native current Level: "+error);
 const auto source_status130=*loading_fields.state130;
 require(source_status130!=38&&source_submit,
         "Unloaded source C1 retains original status early return; no service success is inferred");
 // The original command callback is also exercised independently to identify
 // the first missing same-Level camera listener owner without forcing status38.
 AudioCommandV34 command;require(!publication->sources.source_command(publication->sources.context,play,sound,group,command,error)&&
   error=="Required actual reached SetListenerPos on SAME camera node",
   "Source command fails closed at real unavailable listener leaf: "+error);

 require(newer->unpublish_expected(error)&&!publication->current_provider,
         "Expected-owner detach clears only the currently published bridge");

 require(first->unpublish_expected(error)&&first->unpublish_expected(error),"Stale bridge detach is idempotent");
 manager->request_output_close();require(manager->shutdown(error),"Actual WinMM manager checked shutdown: "+error);
 manager.reset();captured.manager.reset();first.reset();newer.reset();publication.reset();
 require(publication_lifetime.expired(),"Publisher callback-context owner released after bridges");

 // A different actual manager target (and hence raw pointer as well as owner
 // block) is rejected even after the captured manager has been closed.
 auto second_manager=AudioApplicationManagerV42::create(sources,audio_assets,false,error,
   windows_source_session_control_v1());
 require(bool(second_manager),"Second real Windows manager for target mismatch check: "+error);
 auto foreign_publication=std::make_shared<TestPlaybackPublicationV46>();foreign_publication->manager=second_manager;
 AudioGameplayPublisherV46 foreign_publisher=publisher_for(AudioApplicationBorrowV42{second_manager},foreign_publication);
 auto foreign=AudioCampaignBridgeV46::publish_on_owner(AudioApplicationBorrowV42{manager},services,
  std::move(foreign_publisher),error);
 require(!foreign&&error=="Required publisher target is the exact captured Application audio manager"&&
         foreign_publication->publishes==0,"Reject foreign manager target before effects");
 second_manager->request_output_close();require(second_manager->shutdown(error),"Second WinMM manager checked shutdown: "+error);
 second_manager.reset();
 require(current.destroy(error),"Actual source current Level teardown: "+error);
 std::cout<<"PASS typed audio publication target/control-block, same V42 manager/runtime/clock, expected detach/retry/replacement; native C1 state130="
  <<source_status130<<" retains the original submit early return; direct command stops at unprovided SetListenerPos. No playable campaign readiness claimed.\n";
 return 0;
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
