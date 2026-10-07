#pragma once
#include "stage_loader_save_restore_v1.hpp"
#include <canonical_spawn_point_v15.hpp>
#include <canonical_object_manager_v1.hpp>
#include <cstring>
namespace dh2::loader {
struct CheckpointPlayerManagerBorrowV1 {std::shared_ptr<void> owner;std::uintptr_t identity{};const std::uint8_t* byte719{};};
// Actual CharacterSaveMetadataV3 cells already produced/owned by SAME actor.
// Source suffixes describe raw stores; existing semantic field names remain.
struct CheckpointCharacterFieldsV1 {
 std::shared_ptr<void> owner;std::uintptr_t character{};
 float* point1468{};float* point1474{};
};
struct CheckpointServicesV1 {
 std::shared_ptr<void> owner;std::weak_ptr<void> actual_application;
 std::weak_ptr<level::LevelSavegameRuntimeV1> level_save;
 std::function<bool(std::uint8_t&,std::string&)> online_byte5;
 std::function<bool(const std::shared_ptr<void>&,CheckpointPlayerManagerBorrowV1&,std::string&)> player_manager;
 std::function<bool(const CheckpointPlayerManagerBorrowV1&,bool&,std::string&)> local_player_hosting;
 std::function<bool(const CheckpointPlayerManagerBorrowV1&,std::int32_t,bool,RestoreLocalPlayerBorrowV1&,std::string&)> local_player;
 std::function<bool(std::uintptr_t,RestoreCharacterBorrowV1&,std::string&)> character;
 // Exact Character virtual IsDead+34, never a guessed health threshold.
 std::function<bool(const RestoreCharacterBorrowV1&,bool&,std::string&)> is_dead;
 // Read-only checked alias lending; creates no metadata/Character storage.
 std::function<bool(const RestoreCharacterBorrowV1&,CheckpointCharacterFieldsV1&,std::string&)> checkpoint_fields;
 // Genuine Character::SG_SaveCheckpoint3bc494 on existing selected save.
 std::function<bool(const RestoreCharacterBorrowV1&,std::string&)> character_save_checkpoint;
 // SAME Application38 ObjectManager and original ObjectHandle C1; resolver
 // uses existing manager.resolve_handle_v4(false) body directly.
 std::function<bool(const std::shared_ptr<void>&,std::shared_ptr<world::CanonicalObjectManagerV1>&,std::string&)> objects;
 std::function<bool(const world::CanonicalObjectBorrowV1*,target_providers::Handle16&,std::string&)> make_handle;
 std::function<bool(const world::CanonicalObjectBorrowV1&,std::shared_ptr<world::CanonicalSpawnPointV15>&,std::string&)> spawn_point;
};
// Source state34 checkpoint tail ONLY, after genuine UpdateMaterial prefix.
// Do not install run_tail alone as completed whole Lifecycle.stage_body[34].
// save_point implements whole Level::CheckpointSave3f04b4 and is reusable at
// genuine later call sites on the SAME actual Level, without new save state.
class LevelCheckpointOrchestrationV1 final {
 std::weak_ptr<CanonicalLevelContextV1> level_;CheckpointServicesV1 services_;
 bool tail_busy_{},save_busy_{},failed_{},tail_done_{};std::string failure_;
 template<class A,class B>static bool same(const std::shared_ptr<A>& a,const std::shared_ptr<B>& b){return a&&b&&!a.owner_before(b)&&!b.owner_before(a);}
 bool reject(std::string& e,const char* why){if(e.empty())e=why;if(!failed_)failure_=e;failed_=true;e=failure_;return false;}
 static std::int32_t signed_state(std::uint32_t raw)noexcept{std::int32_t out;std::memcpy(&out,&raw,4);return out;}
 bool scope(std::shared_ptr<CanonicalLevelContextV1>& level,LifecycleBorrowV36& loading,LevelConstructorBorrowV3& fields,std::shared_ptr<void>& app,std::string& e){
  level=level_.lock();if(!borrow_lifecycle_fields_v36(level,loading,e))return false;fields=level->constructor_borrow_v3();app=services_.actual_application.lock();
  if(!fields.fields||!services_.owner||!app||same(services_.owner,level)||same(services_.owner,app)){e="Required live SAME Level/Application and independent checkpoint providers";return false;}return true;
 }
 bool current(const LifecycleBorrowV36& loading,std::uint32_t expected,std::string& e){if(failed_){e=failure_;return false;}if(*loading.fields.state130!=expected){e="Checkpoint primitive changed actual loading state";return false;}return true;}
 bool manager(const std::shared_ptr<void>& app,CheckpointPlayerManagerBorrowV1& out,std::string& e){out={};if(!services_.player_manager||!services_.player_manager(app,out,e))return false;if(!out.owner||!out.identity){e="Required SAME Application40 PlayerManager";return false;}return true;}
 bool local(const std::shared_ptr<void>& app,RestoreLocalPlayerBorrowV1& out,std::string& e){CheckpointPlayerManagerBorrowV1 m;if(!manager(app,m,e))return false;out={};if(!services_.local_player||!services_.local_player(m,0,true,out,e))return false;if(!out.owner||!out.identity||!out.character660){e="Required actual local0 PlayerInfo/character660";return false;}return true;}
 bool character(std::uintptr_t id,RestoreCharacterBorrowV1& out,std::string& e){out={};if(!id||!services_.character||!services_.character(id,out,e))return false;if(!out.same_source(id)){e="Required SAME actual checkpoint Character/base";return false;}return true;}
 bool save(LevelConstructorBorrowV3& fields,const LifecycleBorrowV36& loading,std::shared_ptr<level::LevelSavegameRuntimeV1>& out,std::string& e){
  out=services_.level_save.lock();const auto& actual=fields.fields->save_ec;
  if(!out||!actual||actual.get()!=out.get()||!same(actual,out)||out->owner().fields().level8!=reinterpret_cast<const void*>(loading.identity)){e="Required SAME live LevelEC checkpoint save runtime";return false;}return true;
 }
 // Original GetSpawnPoint3ef614: iterate signed registry order, construct and
 // resolve handle(false), type13, actual visible8a, entrypoint374==Level110.
 bool spawn(LevelConstructorBorrowV3& fields,const std::shared_ptr<void>& app,const LifecycleBorrowV36& loading,std::shared_ptr<world::CanonicalSpawnPointV15>& out,std::string& e){
  std::shared_ptr<world::CanonicalObjectManagerV1> objects;if(!services_.objects||!services_.objects(app,objects,e)||!objects||!current(loading,34,e))return false;
  out.reset();std::int32_t key{};const world::CanonicalObjectBorrowV1* actor{};bool found=objects->source_ordered_begin_v38(key,actor);
  while(found){
   if(actor){
    target_providers::Handle16 handle{};
    if(!services_.make_handle||!services_.make_handle(actor,handle,e)||!current(loading,34,e))return false;
    const world::CanonicalObjectBorrowV1* resolved{};
    if(!objects->resolve_handle_v4(handle,false,resolved,{},e))return false;
    if(resolved){
     if(!resolved->type_f4){e="Required actual spawn lookup typeF4";return false;}
     if(*resolved->type_f4==13){
      std::shared_ptr<world::CanonicalSpawnPointV15> actual;
      if(!services_.spawn_point||!services_.spawn_point(*resolved,actual,e)||!current(loading,34,e))return false;
      if(!actual||actual->base().identity()!=resolved->identity||!same(actual,resolved->lease)){e="Required SAME mapped SpawnPoint receiver/lease";return false;}
      const auto* visible=actual->base().byte(0x8a);if(!visible){e="Required actual SpawnPoint byte8a";return false;}
      if(*visible&&actual->entrypoint()==fields.fields->level110){out=std::move(actual);return true;}
     }
    }
   }
   found=objects->source_ordered_next_v38(key,key,actor);
  }
  return true; // Genuine absent spawn point; caller follows original fallback.
 }
public:
 LevelCheckpointOrchestrationV1(std::weak_ptr<CanonicalLevelContextV1> level,CheckpointServicesV1 services):level_(std::move(level)),services_(std::move(services)){}
 bool save_point(const float* point,bool force,std::string& e){
  if(failed_){e=failure_;return false;}if(save_busy_)return reject(e,"Level.CheckpointSave reentered");e.clear();save_busy_=true;struct Guard{bool& b;~Guard(){b=false;}} guard{save_busy_};
  std::shared_ptr<CanonicalLevelContextV1> level;LifecycleBorrowV36 loading;LevelConstructorBorrowV3 fields;std::shared_ptr<void> app;
  if(!scope(level,loading,fields,app,e))return reject(e,"Required actual checkpoint Level");const auto expected=*loading.fields.state130;
  try{
   // Authentic query BEFORE save_ec/phase/Character guards, 3f04d8..04e8.
   RestoreLocalPlayerBorrowV1 p;if(!local(app,p,e)||!current(loading,expected,e))return reject(e,"Required CheckpointSave local0 query");const auto id=*p.character660;
   if(!fields.fields->save_ec||signed_state(*loading.fields.state130)<=33||!id)return true;
   RestoreCharacterBorrowV1 c;if(!character(id,c,e)||!current(loading,expected,e))return reject(e,"Required CheckpointSave actual Character");
   bool dead{};if(!services_.is_dead||!services_.is_dead(c,dead,e)||!current(loading,expected,e))return reject(e,"Required actual Character.IsDead virtual34");if(dead)return true;
   CheckpointCharacterFieldsV1 metadata;if(!services_.checkpoint_fields||!services_.checkpoint_fields(c,metadata,e)||!current(loading,expected,e))return reject(e,"Required SAME produced Character checkpoint fields");
   if(!metadata.owner||metadata.character!=id||!metadata.point1468||!metadata.point1474||!point)return reject(e,"Required actual checkpoint point/metadata cells");
   // Source rereads each input word before each store; preserve pointer aliases.
   metadata.point1468[0]=point[0];metadata.point1468[1]=point[1];metadata.point1468[2]=point[2];
   const float* position=force?point:c.position160_v80();if(!position)return reject(e,"Required actual Character160 checkpoint position");
   metadata.point1474[0]=position[0];metadata.point1474[1]=position[1];metadata.point1474[2]=position[2];
   if(!services_.character_save_checkpoint||!services_.character_save_checkpoint(c,e)||!current(loading,expected,e))return reject(e,"Required genuine Character.SG_SaveCheckpoint3bc494");
   // Fresh fields AFTER Character SG_SaveCheckpoint, exact 3f0578..0584.
   const auto row=fields.fields->row3c;std::shared_ptr<level::LevelSavegameRuntimeV1> actual_save;
   if(!save(fields,loading,actual_save,e))return reject(e,"Required reread SAME checkpoint save_ec");
   const auto seed=fields.fields->seed114;const auto difficulty=fields.fields->difficulty40;
   if(!actual_save->source_save_checkpoint_v83(seed,difficulty,row,e)||!current(loading,expected,e))return reject(e,"Required genuine LevelSavegame.SaveCheckPoint463130");return true;
  }catch(const std::exception& ex){e=ex.what();return reject(e,"CheckpointSave primitive threw");}catch(...){return reject(e,"CheckpointSave primitive threw an unknown exception");}
 }
 bool run_tail(std::string& e){
  if(failed_){e=failure_;return false;}if(tail_busy_||save_busy_)return reject(e,"State34 checkpoint tail reentered");if(tail_done_)return reject(e,"State34 checkpoint tail cannot replay");
  e.clear();tail_busy_=true;struct Guard{bool& b;~Guard(){b=false;}} guard{tail_busy_};
  std::shared_ptr<CanonicalLevelContextV1> level;LifecycleBorrowV36 loading;LevelConstructorBorrowV3 fields;std::shared_ptr<void> app;
  if(!scope(level,loading,fields,app,e)||*loading.fields.state130!=34)return reject(e,"Checkpoint tail requires SAME actual state34");
  try{
   std::uint8_t online{};if(!services_.online_byte5||!services_.online_byte5(online,e)||!current(loading,34,e))return reject(e,"Required actual Network byte5");
   bool validate_branch=true;
   if(online){
    CheckpointPlayerManagerBorrowV1 m;if(!manager(app,m,e)||!current(loading,34,e))return reject(e,"Required actual checkpoint PlayerManager");
    bool host{};if(!services_.local_player_hosting||!services_.local_player_hosting(m,host,e)||!current(loading,34,e))return reject(e,"Required actual checkpoint IsLocalPlayerHosting");
    if(!host)validate_branch=false;
    else{CheckpointPlayerManagerBorrowV1 now;if(!manager(app,now,e)||!current(loading,34,e))return reject(e,"Required fresh checkpoint manager40");if(!now.byte719)return reject(e,"Required actual manager byte719");if(*now.byte719)validate_branch=false;}
   }
   bool create=!validate_branch;
   if(validate_branch){
    if(fields.fields->save_ec){
     std::shared_ptr<level::LevelSavegameRuntimeV1> actual_save;if(!save(fields,loading,actual_save,e))return reject(e,"Required actual checkpoint validation save_ec");
     const auto seed=fields.fields->seed114;const auto difficulty=fields.fields->difficulty40;const auto row=fields.fields->row3c;bool valid{};
     if(!actual_save->owner().validate_checkpoint(seed,difficulty,row,valid,e)||!current(loading,34,e))return reject(e,"Required genuine ValidateCheckpoint463288");
     create=!valid;if(valid)create=fields.fields->byte_f5!=0;
    }else create=fields.fields->byte_f5!=0;
   }
   if(create){
    std::shared_ptr<world::CanonicalSpawnPointV15> point_owner;if(!spawn(fields,app,loading,point_owner,e))return reject(e,"Required actual original GetSpawnPoint traversal");
    // Source queries local0 even when an actual SpawnPoint was selected.
    RestoreLocalPlayerBorrowV1 p;if(!local(app,p,e)||!current(loading,34,e))return reject(e,"Required actual checkpoint point fallback local0");
    const float* selected{};RestoreCharacterBorrowV1 c;
    const std::array<float,3> origin{{0.0f,0.0f,0.0f}}; // Actual ELF Vec3f_Origin99f854, raw three zero words.
    if(point_owner)selected=point_owner->base().vector3(0x160);
    else if(*p.character660){if(!character(*p.character660,c,e)||!current(loading,34,e))return reject(e,"Required actual checkpoint fallback Character");selected=c.position160_v80();}
    else selected=origin.data();
    if(!selected)return reject(e,"Required actual checkpoint selection point160");
    std::array<float,3> point;point[0]=selected[0];point[1]=selected[1];const bool force=fields.fields->byte_f5!=0;point[2]=selected[2];
    if(!save_point(point.data(),force,e)||!current(loading,34,e))return reject(e,"Actual Level.CheckpointSave source prefix failed");
    fields.fields->byte_f5=0;
   }
   tail_done_=true;e.clear();return true; // ONLY checkpoint tail, not whole state34.
  }catch(const std::exception& ex){e=ex.what();return reject(e,"State34 checkpoint primitive threw");}catch(...){return reject(e,"State34 checkpoint primitive threw an unknown exception");}
 }
};
}
