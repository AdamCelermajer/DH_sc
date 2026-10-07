#pragma once
#include "stage_loader_checkpoint_v1.hpp"
#include <gameplay_camera_runtime_v11.hpp>
#include <canonical_level_config_module_v1.hpp>
#include <gameobject_scene_root_registry_v1.hpp>
namespace dh2::loader {
// Actual first App58 PlayerLightTweaker fields; no copied tweaker/Scene state.
struct Stage34FogParamsV1 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};
 std::uint8_t* color_f4{}; // four contiguous actual source RGBA bytes
 float* start110{};float* end114{};float* direction11c{}; // actual XYZ
 std::function<bool(std::string&)> source_fog_written_v90;
};
struct Stage34ControllerV1 {std::shared_ptr<void> owner;std::uintptr_t character{};std::uint32_t* locked8{};};
struct Stage34MatchingDestinationV1 {std::shared_ptr<void> owner;std::uintptr_t identity{};};
struct Stage34ServicesV1 {
 std::shared_ptr<void> owner;std::weak_ptr<void> actual_application;
 CheckpointServicesV1 checkpoint;
 EarlyLoadingDebugV46 debug;
 std::function<bool(const std::shared_ptr<void>&,CheckpointPlayerManagerBorrowV1&,std::string&)> player_manager;
 std::function<bool(const CheckpointPlayerManagerBorrowV1&,std::int32_t,bool,RestoreLocalPlayerBorrowV1&,std::string&)> local_player;
 std::function<bool(const CheckpointPlayerManagerBorrowV1&,bool,std::int32_t&,std::string&)> local_player_count;
 std::function<bool(std::uintptr_t,RestoreCharacterBorrowV1&,std::string&)> character;
 std::function<bool(const RestoreCharacterBorrowV1&,std::int32_t,std::int32_t,std::string&)> set_level_id;
 std::function<bool(const RestoreCharacterBorrowV1&,Stage34ControllerV1&,std::string&)> controller;
 std::function<bool(std::uint8_t&,std::string&)> online_byte5;
 // Exact CMatching.Get()->virtualA4; row is read only AFTER this primitive.
 std::function<bool(Stage34MatchingDestinationV1&,std::string&)> matching_destination;
 std::function<bool(const Stage34MatchingDestinationV1&,std::int32_t,std::int32_t,std::string&)> matching_dispatch;
 // Checked read-only views of SAME current native owners, no allocations.
 std::function<bool(std::uintptr_t,std::shared_ptr<camera::GameplayCameraRuntimeV11>&,std::string&)> camera;
 std::function<bool(std::uintptr_t,std::shared_ptr<world::CanonicalLevelConfigV1>&,std::string&)> config;
 std::function<bool(const std::shared_ptr<void>&,Stage34FogParamsV1&,std::string&)> fog_params58;
 std::function<bool(const std::shared_ptr<void>&,std::shared_ptr<world::GameObjectSceneRootRegistryV1>&,std::string&)> scene1c;
 // Exact CSceneManager::update(float,bool)58b9f0 on SAME source scene.
 std::function<bool(const std::shared_ptr<world::GameObjectSceneRootRegistryV1>&,float,bool,std::string&)> scene_virtual60;
 std::function<bool(const std::shared_ptr<void>&,std::shared_ptr<world::CanonicalObjectManagerV1>&,std::string&)> objects38;
 std::function<bool(const std::shared_ptr<world::CanonicalObjectManagerV1>&,std::string&)> handle_no_room_objects;
 // Genuine existing Level gameplay/engine methods, exact source arguments.
 std::function<bool(const std::shared_ptr<CanonicalLevelContextV1>&,std::uintptr_t,std::string&)> update_fog;
 std::function<bool(const std::shared_ptr<CanonicalLevelContextV1>&,bool,std::int32_t,std::string&)> update_light_set;
 std::function<bool(const std::shared_ptr<CanonicalLevelContextV1>&,bool,bool,std::string&)> update_material;
};
class Stage34BodyV1 final {
 std::weak_ptr<CanonicalLevelContextV1> level_;Stage34ServicesV1 services_;
 std::shared_ptr<LevelCheckpointOrchestrationV1> checkpoint_;
 bool busy_{},failed_{},done_{};std::string failure_;
 template<class A,class B>static bool same(const std::shared_ptr<A>& a,const std::shared_ptr<B>& b){return a&&b&&!a.owner_before(b)&&!b.owner_before(a);}
 LifecycleStepV36 reject(std::string& e,const char* why){if(e.empty())e=why;if(!failed_)failure_=e;failed_=true;e=failure_;return LifecycleStepV36::failed;}
public:
 Stage34BodyV1(std::weak_ptr<CanonicalLevelContextV1> level,Stage34ServicesV1 services):level_(level),services_(std::move(services)),checkpoint_(std::make_shared<LevelCheckpointOrchestrationV1>(std::move(level),services_.checkpoint)){}
 const std::shared_ptr<LevelCheckpointOrchestrationV1>& checkpoint()const noexcept{return checkpoint_;}
 LifecycleStepV36 step(std::string& e){
  if(failed_){e=failure_;return LifecycleStepV36::failed;}if(busy_)return reject(e,"State34 reentered; source prefix retained");if(done_)return reject(e,"State34 complete body cannot replay");
  e.clear();busy_=true;struct Guard{bool& b;~Guard(){b=false;}}guard{busy_};
  auto level=level_.lock();LifecycleBorrowV36 loading;if(!borrow_lifecycle_fields_v36(level,loading,e))return reject(e,"Required SAME completed actual Level C1");
  auto fields=level->constructor_borrow_v3();static_assert(std::is_same_v<decltype(fields.fields->field19c),float> && std::is_same_v<decltype(fields.fields->field1a0),float> && std::is_same_v<decltype(fields.fields->field1a4),float>,"Require SAME source-proven float camera delta cells");auto app=services_.actual_application.lock();
  if(!fields.fields||!fields.config38||*loading.fields.state130!=34||!app||!services_.owner||same(services_.owner,app)||same(services_.owner,level))return reject(e,"State34 requires SAME live C1/App and independent primitive providers");
  auto current=[&](){if(failed_){e=failure_;return false;}if(*loading.fields.state130!=34){e="State34 primitive changed actual loading state";return false;}return true;};
  auto manager=[&](CheckpointPlayerManagerBorrowV1& m){m={};if(!services_.player_manager||!services_.player_manager(app,m,e)||!current())return false;if(!m.owner||!m.identity){e="Required actual Application40 PlayerManager";return false;}return true;};
  auto local=[&](const CheckpointPlayerManagerBorrowV1& m,std::int32_t index,RestoreLocalPlayerBorrowV1& p){p={};if(!services_.local_player||!services_.local_player(m,index,true,p,e)||!current())return false;if(!p.owner||!p.identity||!p.character660){e="Required actual local PlayerInfo/character660";return false;}return true;};
  auto character=[&](std::uintptr_t id,RestoreCharacterBorrowV1& c){c={};if(!id||!services_.character||!services_.character(id,c,e)||!current())return false;if(!c.same_source(id)){e="Required SAME actual Character/base";return false;}return true;};
  auto lend_camera=[&](std::uintptr_t id,std::shared_ptr<camera::GameplayCameraRuntimeV11>& c){c.reset();if(!id||!services_.camera||!services_.camera(id,c,e)||!current())return false;if(!c||reinterpret_cast<std::uintptr_t>(c.get())!=id){e="Required SAME actual Level128 CameraRuntime";return false;}return true;};
  auto config=[&](std::shared_ptr<world::CanonicalLevelConfigV1>& c){const auto id=*fields.config38;c.reset();if(!id||!services_.config||!services_.config(id,c,e)||!current())return false;if(!c||c->identity()!=id){e="Required SAME current actual LevelConfig38";return false;}return true;};
  auto scene=[&](std::shared_ptr<world::GameObjectSceneRootRegistryV1>& s){s.reset();if(!services_.scene1c||!services_.scene1c(app,s,e)||!current())return false;if(!s){e="Required SAME actual Application10 scene1c";return false;}return true;};
  try{
   if(!early_loading_trace_v46(services_.debug,e)||!current())return reject(e,"Required original state34 Debug trace");
   // Original reads manager40 and caches camera128 BEFORE GetLocalPlayer.
   CheckpointPlayerManagerBorrowV1 first_manager;if(!manager(first_manager))return reject(e,"Required actual state34 player manager");
   const auto cached_camera_identity=fields.fields->field128;std::shared_ptr<camera::GameplayCameraRuntimeV11> cached_camera;
   RestoreLocalPlayerBorrowV1 first;if(!local(first_manager,0,first))return reject(e,"Required actual state34 local0");
   const auto first_id=*first.character660;
   if(first_id){
    RestoreCharacterBorrowV1 c;if(!character(first_id,c))return reject(e,"Required actual local Character");
    if(!services_.set_level_id||!services_.set_level_id(c,fields.fields->row3c,-1,e)||!current())return reject(e,"Required genuine Character.SG_SetLevelId(row,-1)");
    if(!lend_camera(cached_camera_identity,cached_camera))return reject(e,"Required actual cached state34 Camera128 target");
    if(!cached_camera->set_target(first_id,0,e)||!current())return reject(e,"Actual CameraTarget.SetTarget failed");
   }
   std::uint8_t online{};if(!services_.online_byte5||!services_.online_byte5(online,e)||!current())return reject(e,"Required state34 actual GetOnline byte5");
   if(online){Stage34MatchingDestinationV1 destination;if(!services_.matching_destination||!services_.matching_destination(destination,e)||!current())return reject(e,"Required actual CMatching.Get/virtualA4");if(!destination.owner||!destination.identity)return reject(e,"Required actual matching destination receiver");if(!services_.matching_dispatch||!services_.matching_dispatch(destination,3,fields.fields->row3c,e)||!current())return reject(e,"Required matching method8(3,fresh row3c)");}
   if(!cached_camera&&!lend_camera(cached_camera_identity,cached_camera))return reject(e,"Required actual cached state34 Camera128 activation");
   if(!cached_camera->activate(e)||!current())return reject(e,"Actual CameraBase.SetActive failed");
   std::shared_ptr<world::GameObjectSceneRootRegistryV1> initial_scene;if(!scene(initial_scene))return reject(e,"Required actual scene before virtual60");
   if(!services_.scene_virtual60||!services_.scene_virtual60(initial_scene,0.0f,false,e)||!current())return reject(e,"Required actual CSceneManager.update(0.0f,false)");
   if(!cached_camera->update(e)||!current())return reject(e,"Actual cached Camera128.virtual10 failed");
   for(std::int32_t index=0;;++index){
    CheckpointPlayerManagerBorrowV1 m;if(!manager(m))return reject(e,"Required live controller-loop manager40");
    std::int32_t count{};if(!services_.local_player_count||!services_.local_player_count(m,true,count,e)||!current())return reject(e,"Required live GetNumLocalPlayers(true)");if(index>=count)break;
    CheckpointPlayerManagerBorrowV1 now;if(!manager(now))return reject(e,"Required fresh controller-loop manager40");RestoreLocalPlayerBorrowV1 p;if(!local(now,index,p))return reject(e,"Required actual controller-loop local player");
    if(*p.character660){RestoreCharacterBorrowV1 c;if(!character(*p.character660,c))return reject(e,"Required actual controller-loop Character");Stage34ControllerV1 controller;if(!services_.controller||!services_.controller(c,controller,e)||!current())return reject(e,"Required SAME Character378 controller");if(!controller.owner||controller.character!=c.identity||!controller.locked8)return reject(e,"Required actual logical source controller byte8");*controller.locked8=1;}
   }
   // Fresh128 after player callbacks, SAME existing scene positions owner.
   std::shared_ptr<camera::GameplayCameraRuntimeV11> actual_camera;if(!lend_camera(fields.fields->field128,actual_camera))return reject(e,"Required reread actual camera128");camera::PointV2 position,parent;if(!actual_camera->source_positions_v67(position,parent,e)||!current())return reject(e,"Required actual camera/parent absolute position caches");
   const float y=position[1]-parent[1];const float z=position[2]-parent[2];const float x=position[0]-parent[0];
   fields.fields->field1a0=y;fields.fields->field19c=x;fields.fields->field1a4=z;
   Stage34FogParamsV1 params;if(!services_.fog_params58||!services_.fog_params58(app,params,e)||!current())return reject(e,"Required actual Application58 PlayerLightTweaker fog params");if(!params.owner||!params.identity||!params.color_f4||!params.start110||!params.end114||!params.direction11c)return reject(e,"Required SAME mutable PlayerLightTweaker58 source fog cells");
   std::shared_ptr<world::CanonicalLevelConfigV1> cfg;if(!config(cfg))return reject(e,"Required current config fog color");const auto* color=cfg->vector(0x1e0);if(!color)return reject(e,"Required actual config1e0 fog RGB");
   const float green=(*color)[1],blue=(*color)[2],red=(*color)[0];const auto r=world::source_fog_color_byte_v68(red),g=world::source_fog_color_byte_v68(green),b=world::source_fog_color_byte_v68(blue);
   params.color_f4[3]=255;params.color_f4[2]=b;params.color_f4[0]=r;params.color_f4[1]=g;
   if(!config(cfg))return reject(e,"Required fresh config fog_start");const auto* start=cfg->integer(0x1d8);if(!start)return reject(e,"Required config signed int1d8");const float start_value=static_cast<float>(*start);
   if(!config(cfg))return reject(e,"Required fresh config fog_end");const auto* end=cfg->integer(0x1dc);if(!end)return reject(e,"Required config signed int1dc");const float end_value=static_cast<float>(*end);
   // Native PLT30e964 is __aeabi_i2f, NOT float-to-int conversion.
   *params.start110=start_value;*params.end114=end_value;
   if(!config(cfg))return reject(e,"Required fresh config direction for manager58");const auto* direction=cfg->vector(0x1f8);if(!direction)return reject(e,"Required config1f8 fog direction mask");const auto manager_direction=*direction;params.direction11c[0]=manager_direction[0];params.direction11c[1]=manager_direction[1];params.direction11c[2]=manager_direction[2];
   if(params.source_fog_written_v90&&(!params.source_fog_written_v90(e)||!current()))return reject(e,"Actual tweaker write notification failed");
   std::shared_ptr<world::GameObjectSceneRootRegistryV1> transform_scene;if(!scene(transform_scene))return reject(e,"Required fresh scene1c fog transform owner");
   if(!config(cfg))return reject(e,"Required fresh config direction for scene1c");direction=cfg->vector(0x1f8);if(!direction)return reject(e,"Required fresh config1f8 vector");transform_scene->source_frame_fields_v67().fog_transform458=*direction;++transform_scene->source_frame_fields_v67().parameter_revision;
   // Scene receiver is reread and pinned BEFORE native Debug switch call.
   std::shared_ptr<world::GameObjectSceneRootRegistryV1> lighting_scene;if(!scene(lighting_scene))return reject(e,"Required fresh scene1c lighting owner");bool disable_lighting{};if(!services_.debug.actual_owner||!services_.debug.get_instance_and_switch||!services_.debug.get_instance_and_switch("RENDERING_DisableAllLighting",disable_lighting,e)||!current())return reject(e,"Required actual rendering Debug switch");lighting_scene->source_frame_fields_v67().lighting430=disable_lighting?0:1;++lighting_scene->source_frame_fields_v67().parameter_revision;
   std::shared_ptr<world::CanonicalObjectManagerV1> objects;if(!services_.objects38||!services_.objects38(app,objects,e)||!objects||!current())return reject(e,"Required actual Application38 object manager");if(!services_.handle_no_room_objects||!services_.handle_no_room_objects(objects,e)||!current())return reject(e,"Required genuine ObjectManager.HandleNoRoomObjects345954");
   if(!services_.update_fog||!services_.update_fog(level,0,e)||!current())return reject(e,"Required genuine Level.UpdateFog(NULL)");
   if(!services_.update_light_set||!services_.update_light_set(level,true,4,e)||!current())return reject(e,"Required genuine Level.UpdateLightSet(true,4)");
   if(!services_.update_material||!services_.update_material(level,false,true,e)||!current())return reject(e,"Required genuine Level.UpdateMaterial(false,true)");
   if(!checkpoint_->run_tail(e)||!current())return reject(e,"Actual state34 checkpoint tail failed");done_=true;e.clear();return LifecycleStepV36::complete;
   // Existing dispatcher owns actual130 increment/progress/license/menu tail.
  }catch(const std::exception& ex){e=ex.what();return reject(e,"State34 genuine primitive threw");}catch(...){return reject(e,"State34 genuine primitive threw an unknown exception");}
 }
};
inline bool bind_stage34_v1(LifecycleServicesV36& loading,std::weak_ptr<CanonicalLevelContextV1> level,Stage34ServicesV1 services,std::string& e,std::shared_ptr<Stage34BodyV1>* actual_owner=nullptr){
 if(loading.stage_body[34]){e="State34 actual body already bound; refusing replacement";return false;}
 auto app=services.actual_application.lock();auto checkpoint_app=services.checkpoint.actual_application.lock();
 if(level.expired()||!services.owner||!app||!checkpoint_app||app.get()!=checkpoint_app.get()||app.owner_before(checkpoint_app)||checkpoint_app.owner_before(app)||!services.checkpoint.owner){e="Required SAME actual Level/App checkpoint scope and independent stage34 providers";return false;}
 auto body=std::make_shared<Stage34BodyV1>(std::move(level),std::move(services));loading.stage_body[34]=[body](std::string& error){return body->step(error);};if(actual_owner)*actual_owner=std::move(body);e.clear();return true;
}
}
