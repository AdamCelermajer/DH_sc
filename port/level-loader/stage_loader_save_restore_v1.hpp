#pragma once
#include <canonical_level_context_v1.hpp>
#include "lifecycle_v36_counter_borrow.hpp"
#include "stage_loader_v46_early.hpp"
#include <level_savegame_runtime_v1.hpp>
#include <game_object_set_position_v2.hpp>
#include <game_object_initialization_borrow_v62.hpp>
#include <array>
#include <exception>
namespace dh2::loader {
// Actual Application+40, PlayerInfo+660 and Character/base receiver views.
// Borrow callbacks only resolve/lend existing owners and fields; they allocate
// no player/Character/save/manager and execute no functional stage body.
struct RestorePlayerManagerBorrowV1 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};
 const std::uint8_t* byte6d0{};const float* position6d4{};
};
struct RestoreLocalPlayerBorrowV1 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};
 const std::uintptr_t* character660{};
};
struct RestoreCharacterBorrowV1 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};
 world::CanonicalGameObjectBaseOwnerV1* base{}; // existing canonical domains
 // Existing RetainedCharacterActor storage, NOT a second canonical base.
 // Field functions and receiver pin are borrowed from that SAME actor.
 world::GameObjectInitializationFieldsV62 fields_v80;
 bool same_source(std::uintptr_t expected)const noexcept{
  if(!owner||!expected||identity!=expected)return false;
  if(base)return !fields_v80.receiver&&base->identity()==expected;
  return fields_v80.receiver&&fields_v80.source_identity==expected;
 }
 const float* position160_v80()const{
  return base?base->vector3(0x160):fields_v80.vector3?fields_v80.vector3(0x160):nullptr;
 }
};
struct Stage33RestoreServicesV1 {
 // Independent provider storage. Capture containing Application/World/Level
 // weakly in every leaf. Body pins actual receivers only during delivery.
 std::shared_ptr<void> owner;std::weak_ptr<void> actual_application;
 // SAME runtime published by existing LevelConstructorBindingsV4 at save_ec.
 std::weak_ptr<level::LevelSavegameRuntimeV1> level_save;
 EarlyLoadingDebugV46 debug;
 std::function<bool(std::uint8_t&,std::string&)> online_byte5;
 std::function<bool(const std::shared_ptr<void>&,RestorePlayerManagerBorrowV1&,std::string&)> player_manager;
 std::function<bool(const RestorePlayerManagerBorrowV1&,bool&,std::string&)> local_player_hosting;
 std::function<bool(const RestorePlayerManagerBorrowV1&,std::int32_t,bool,RestoreLocalPlayerBorrowV1&,std::string&)> local_player;
 std::function<bool(const RestorePlayerManagerBorrowV1&,bool,std::int32_t&,std::string&)> local_player_count;
 std::function<bool(std::uintptr_t,RestoreCharacterBorrowV1&,std::string&)> character;
 // Exact PFWorld.GetFloorHeightAt(query,&height,NULL,NULL,NULL,false).
 // Bool return is delivery; found is the original native query result.
 std::function<bool(const std::array<float,3>&,float&,bool&,std::string&)> floor_height;
 // Read-only lending of existing actual attached/PF/visual backend leaves.
 // Whole SetPosition393db4/SetDestination393600 remain existing source code.
 std::function<bool(const RestoreCharacterBorrowV1&,world::GameObjectSetPositionServicesV2&,std::string&)> position_services;
 // Genuine Character::SG_SetUseSpawnPoint3bb798 on this SAME Character's
 // existing save/difficulty authority; never a copied LUSP array or disk write.
 std::function<bool(const RestoreCharacterBorrowV1&,bool,std::string&)> set_use_spawn_point;
 // Actual existing retained Character SetPosition393db4/SetDestination body.
 // Pointer aliases are preserved; no copied pose or proxy canonical base.
 std::function<bool(const RestoreCharacterBorrowV1&,const float*,bool,std::string&)> retained_set_position_v80;
};
class Stage33RestoreBodyV1 final {
 std::weak_ptr<CanonicalLevelContextV1> level_;Stage33RestoreServicesV1 services_;
 bool busy_{},failed_{},done_{};std::string failure_;
 LifecycleStepV36 reject(std::string& e,const char* why){if(e.empty())e=why;if(!failed_)failure_=e;failed_=true;e=failure_;return LifecycleStepV36::failed;}
 template<class A,class B>static bool same_owner(const std::shared_ptr<A>& a,const std::shared_ptr<B>& b){return a&&b&&!a.owner_before(b)&&!b.owner_before(a);}
 static bool actual_character(const RestoreCharacterBorrowV1& c,std::uintptr_t id,std::string& e){
  if(!c.same_source(id)){e="Required SAME actual Character/base receiver";return false;}return true;
 }
public:
 Stage33RestoreBodyV1(std::weak_ptr<CanonicalLevelContextV1> level,Stage33RestoreServicesV1 services):level_(std::move(level)),services_(std::move(services)){}
 LifecycleStepV36 step(std::string& e){
  if(busy_)return reject(e,"Stage33 reentered; native prefix retained");
  if(failed_){e=failure_;return LifecycleStepV36::failed;}
  if(done_)return reject(e,"Stage33 body cannot replay after completion");
  e.clear();busy_=true;struct Guard{bool& value;~Guard(){value=false;}} guard{busy_};
  auto level=level_.lock();LifecycleBorrowV36 loading;
  if(!borrow_lifecycle_fields_v36(level,loading,e))return reject(e,"Required SAME completed actual Level C1");
  auto fields=level->constructor_borrow_v3();
  if(!fields.fields||*loading.fields.state130!=33)return reject(e,"Stage33 requires actual state130=33");
  auto app=services_.actual_application.lock();
  if(!services_.owner||!app||same_owner(services_.owner,level)||same_owner(services_.owner,app))return reject(e,"Required independent restore providers and live actual Application");
  auto current=[&](){if(failed_){e=failure_;return false;}if(*loading.fields.state130!=33){e="Stage33 leaf changed actual loading state";return false;}return true;};
  auto manager=[&](RestorePlayerManagerBorrowV1& m){
   m={};if(!services_.player_manager||!services_.player_manager(app,m,e)||!current())return false;
   if(!m.owner||!m.identity){e="Required actual Application40 PlayerManager borrow";return false;}return true;
  };
  auto local=[&](std::int32_t index,RestoreLocalPlayerBorrowV1& p){
   RestorePlayerManagerBorrowV1 m;if(!manager(m))return false;
   p={};if(!services_.local_player||!services_.local_player(m,index,true,p,e)||!current())return false;
   if(!p.owner||!p.identity||!p.character660){e="Required actual GetLocalPlayer(index,true) record/character660";return false;}return true;
  };
  auto character=[&](std::uintptr_t id,RestoreCharacterBorrowV1& c){
   c={};if(!services_.character||!services_.character(id,c,e)||!current())return false;return actual_character(c,id,e);
  };
  auto set_position=[&](const RestoreCharacterBorrowV1& c,const float* xyz){
   if(!c.base){
    if(!services_.retained_set_position_v80){e="Required existing retained Character SetPosition source provider";return false;}
    if(!services_.retained_set_position_v80(c,xyz,true,e))return false;return current();
   }
   world::GameObjectSetPositionServicesV2 leaves;
   if(!services_.position_services||!services_.position_services(c,leaves,e)||!current())return false;
   if(!same_owner(leaves.owner,services_.owner)){e="Position leaves replaced independent restore authority";return false;}
   if(!world::game_object_set_position_v2(*c.base,xyz,true,leaves,e))return false;return current();
  };
  // Source's signed loop reloads actual manager40/count before EVERY iteration.
  // Source position is reborrowed after GetLocalPlayer only on client branch;
  // hosting keeps the SAME host160 pointer, never a copied position vector.
  auto restore_players=[&](const RestoreCharacterBorrowV1* host){
   for(std::int32_t index=0;;++index){
    RestorePlayerManagerBorrowV1 m;if(!manager(m))return false;
    std::int32_t count{};if(!services_.local_player_count||!services_.local_player_count(m,true,count,e)||!current())return false;
    if(index>=count)return true;
    RestoreLocalPlayerBorrowV1 p;if(!local(index,p))return false;
    const auto id=*p.character660;RestoreCharacterBorrowV1 c;if(!character(id,c))return false;
    const float* xyz{};
    if(host){xyz=host->position160_v80();if(!xyz){e="Required SAME hosting Character160 position";return false;}}
    else{RestorePlayerManagerBorrowV1 now;if(!manager(now))return false;xyz=now.position6d4;if(!xyz){e="Required actual PlayerManager6d4 restoration point";return false;}m=std::move(now);}
    if(!set_position(c,xyz))return false;
   }
  };
  try{
   // 3f7630..7660: ignored debug query result, then fresh actual byteF1 read.
   if(!early_loading_trace_v46(services_.debug,e)||!current())return reject(e,"Required original Stage33 Debug trace");
   if(fields.fields->byte_f1){
    auto save=services_.level_save.lock();const auto& actual=fields.fields->save_ec;
    if(!save||!actual||actual.get()!=save.get()||!same_owner(actual,save)||save->owner().fields().level8!=reinterpret_cast<const void*>(loading.identity))return reject(e,"Stage33 requires SAME LevelEC save runtime and Level identity");
    // Existing real owner, INFO then OBJS; no recache/PlayerSavegame parsing.
    if(!save->owner().load(e)||!current())return reject(e,"Actual LevelSavegame.Load delivery failed");
    std::uint8_t online{};if(!services_.online_byte5||!services_.online_byte5(online,e)||!current())return reject(e,"Required original Network byte5");
    if(online){
     RestorePlayerManagerBorrowV1 m;if(!manager(m))return reject(e,"Required actual online PlayerManager");
     bool hosting{};if(!services_.local_player_hosting||!services_.local_player_hosting(m,hosting,e)||!current())return reject(e,"Required actual IsLocalPlayerHosting");
     if(hosting){
      RestoreLocalPlayerBorrowV1 p;if(!local(0,p))return reject(e,"Required actual hosting local player");
      const auto id=*p.character660;
      if(id){
       RestoreCharacterBorrowV1 host;if(!character(id,host))return reject(e,"Required actual hosting Character");
       const auto* position=host.position160_v80();if(!position)return reject(e,"Required hosting Character160 fields");
       std::array<float,3> query{{position[0],position[1],position[2]}};float height{};bool found{};
       if(!services_.floor_height||!services_.floor_height(query,height,found,e)||!current())return reject(e,"Required actual PFWorld.GetFloorHeightAt");
       if(found){query[2]=height;if(!set_position(host,query.data()))return reject(e,"Actual hosting floor SetPosition failed");}
       if(!restore_players(&host))return reject(e,"Actual hosting player restoration failed");
      } // Original NULL hosting Character skips floor and propagation.
     }else{
      RestorePlayerManagerBorrowV1 now;if(!manager(now))return reject(e,"Required live nonhosting manager40");
      if(!now.byte6d0)return reject(e,"Required actual PlayerManager byte6d0");
      if(*now.byte6d0&&!restore_players(nullptr))return reject(e,"Actual nonhosting player restoration failed");
     }
    }
   }
   // 3f7674..769c: fresh manager/local/character reads, even after host loop.
   RestoreLocalPlayerBorrowV1 p;if(!local(0,p))return reject(e,"Required actual local player spawn metadata owner");
   RestoreCharacterBorrowV1 c;if(!character(*p.character660,c))return reject(e,"Required actual Character for SG_SetUseSpawnPoint(false)");
   if(!services_.set_use_spawn_point||!services_.set_use_spawn_point(c,false,e)||!current())return reject(e,"Required genuine Character.SG_SetUseSpawnPoint(false)");
   fields.fields->byte_f3=0;done_=true;e.clear();return LifecycleStepV36::complete;
   // Existing dispatcher owns the original state130 increment and progress tail.
  }catch(const std::exception& ex){e=ex.what();return reject(e,"Stage33 native primitive threw");}
  catch(...){return reject(e,"Stage33 native primitive threw an unknown exception");}
 }
};
inline bool bind_stage33_restore_v1(LifecycleServicesV36& loading,std::weak_ptr<CanonicalLevelContextV1> level,Stage33RestoreServicesV1 services,std::string& e){
 if(loading.stage_body[33]){e="Stage33 actual body already bound; refusing replacement";return false;}
 auto actual_level=level.lock();auto actual_application=services.actual_application.lock();
 const auto same_owner=[](const auto& a,const auto& b){return a&&b&&!a.owner_before(b)&&!b.owner_before(a);};
 if(!actual_level||!services.owner||!actual_application||same_owner(services.owner,actual_level)||same_owner(services.owner,actual_application)){
  e="Required actual Level/Application and independent restore authority";return false;
 }
 auto body=std::make_shared<Stage33RestoreBodyV1>(std::move(level),std::move(services));
 loading.stage_body[33]=[body=std::move(body)](std::string& error){return body->step(error);};e.clear();return true;
}
}
