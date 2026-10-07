#include "catalog_auxiliary_v67.hpp"
#include <cstring>
#include <tuple>
#include <utility>
#include <exception>
namespace dh2::loader {
struct AuxiliaryStateV67 {
 ScopeV67 scope;std::shared_ptr<const CatalogAuxiliaryServicesV67> leaves;
 std::shared_ptr<AuxiliaryReleaseRecordV67> pending;
 std::function<void()> pending_erase;
 std::map<std::uintptr_t,std::function<void()>> erase;
};
namespace {
template<class A,class B>bool same_owner(const std::shared_ptr<A>& a,const std::shared_ptr<B>& b){return a&&b&&!a.owner_before(b)&&!b.owner_before(a);}
bool independent(const ScopeV67& s,const std::shared_ptr<const void>& pin){return !pin||(!same_owner(pin,s.actual_world.lock())&&!same_owner(pin,s.level.lock())&&!same_owner(pin,s.objects.lock()));}
bool borrow(const AuxiliaryStateV67& s,AuxiliaryDeliveryV67& b,std::string& e){
 b={s.scope.actual_world.lock(),s.scope.level.lock(),s.scope.objects.lock(),s.scope.properties};
 if(!b.actual_world||!b.level||!b.objects||!b.properties){e="Auxiliary actual World/Level/manager lifetime expired";return false;}
 return s.leaves->validate_current(b,e);
}
template<class Record>bool receiver(const std::weak_ptr<Record>& weak,std::shared_ptr<Record>& r,std::string& e){r=weak.lock();if(!r||!r->owner){e="Required SAME produced auxiliary C1/runtime";return false;}return true;}
template<class Record,class Services,class... Args>
std::function<bool(Args...)> member(const std::shared_ptr<AuxiliaryStateV67>& state,std::weak_ptr<Record> weak,
 AuxiliaryServicesLenderV67<Record,Services> lend,std::function<bool(Args...)> Services::* field,const char* operation){
 return [state,weak,lend=std::move(lend),field,operation](Args... args){
  auto& error=std::get<sizeof...(Args)-1>(std::forward_as_tuple(args...));
  AuxiliaryDeliveryV67 delivery;std::shared_ptr<Record> actual;if(!borrow(*state,delivery,error)||!receiver(weak,actual,error))return false;
  if(!lend){error=std::string("Required genuine auxiliary engine services: ")+operation;return false;}
  Services current;if(!lend(delivery,*actual->owner,current,error))return false;
  if(!same_owner(current.owner,state->scope.services_owner)){error=std::string("Auxiliary service replaced independent authority: ")+operation;return false;}
  if(!state->leaves->validate_current(delivery,error))return false;
  auto& call=current.*field;if(!call){error=std::string("Required genuine reached auxiliary engine leaf: ")+operation;return false;}
  if(!call(std::forward<Args>(args)...))return false;
  return state->leaves->validate_current(delivery,error);
 };
}
// Nested Zone resource leaves reuse the SAME current class lender and weak
// constructor record. No whole Zone body is delegated across this boundary.
template<class Record,class Services,class... Args>
std::function<bool(Args...)> zone_startup_member_v76(const std::shared_ptr<AuxiliaryStateV67>& state,std::weak_ptr<Record> weak,
 AuxiliaryServicesLenderV67<Record,Services> lend,std::function<bool(Args...)> world::ZoneStartupServicesV76::* field,const char* operation){
 return [state,weak,lend=std::move(lend),field,operation](Args... args){
  auto& error=std::get<sizeof...(Args)-1>(std::forward_as_tuple(args...));
  AuxiliaryDeliveryV67 delivery;std::shared_ptr<Record> actual;
  if(!borrow(*state,delivery,error)||!receiver(weak,actual,error))return false;
  if(!lend){error=std::string("Required genuine Zone engine lender: ")+operation;return false;}
  Services current;if(!lend(delivery,*actual->owner,current,error))return false;
  if(!same_owner(current.owner,state->scope.services_owner)||!same_owner(current.startup.owner,state->scope.services_owner)){
   error="Zone startup replaced independent auxiliary authority";return false;
  }
  if(!state->leaves->validate_current(delivery,error))return false;
  auto& call=current.startup.*field;if(!call){error=std::string("Required genuine reached Zone resource: ")+operation;return false;}
  if(!call(std::forward<Args>(args)...))return false;
  return state->leaves->validate_current(delivery,error);
 };
}
template<class Record,class Services>world::ZoneStartupServicesV76 zone_startup_v76(const std::shared_ptr<AuxiliaryStateV67>& state,
 std::weak_ptr<Record> weak,AuxiliaryServicesLenderV67<Record,Services> lend){
 world::ZoneStartupServicesV76 out;out.owner=state->scope.services_owner;using Z=world::ZoneStartupServicesV76;
 out.set_bounding_box=zone_startup_member_v76(state,weak,lend,&Z::set_bounding_box,"virtual9c SetBoundingBox");
 out.create_zone_physical=zone_startup_member_v76(state,weak,lend,&Z::create_zone_physical,"actual Zone physical constructor/assignment");
 out.bind_visual_collision_zone=zone_startup_member_v76(state,weak,lend,&Z::bind_visual_collision_zone,"actual _colzone native resources");return out;
}
template<class Record,class... Args>
std::function<bool(Args...)> door_startup_member_v77(const std::shared_ptr<AuxiliaryStateV67>& state,std::weak_ptr<Record> weak,
 AuxiliaryServicesLenderV67<Record,world::DoorServicesV27> lend,std::function<bool(Args...)> world::DoorStartupServicesV77::* field,const char* operation){
 return [state,weak,lend=std::move(lend),field,operation](Args... args){
  auto& error=std::get<sizeof...(Args)-1>(std::forward_as_tuple(args...));AuxiliaryDeliveryV67 delivery;std::shared_ptr<Record> actual;
  if(!borrow(*state,delivery,error)||!receiver(weak,actual,error))return false;
  if(!lend){error="Required genuine Door engine lender";return false;}
  world::DoorServicesV27 current;if(!lend(delivery,*actual->owner,current,error))return false;
  if(!same_owner(current.owner,state->scope.services_owner)||!same_owner(current.startup.owner,state->scope.services_owner)){error="Door startup replaced independent auxiliary authority";return false;}
  if(!state->leaves->validate_current(delivery,error))return false;
  auto& call=current.startup.*field;if(!call){error=std::string("Required genuine reached Door engine leaf: ")+operation;return false;}
  if(!call(std::forward<Args>(args)...))return false;return state->leaves->validate_current(delivery,error);
 };
}
template<class Record,class... Args>
std::function<bool(Args...)> trigger_object_startup_member_v78(const std::shared_ptr<AuxiliaryStateV67>& state,std::weak_ptr<Record> weak,
 AuxiliaryServicesLenderV67<Record,world::TriggerObjectServicesV28> lend,std::function<bool(Args...)> world::TriggerObjectStartupServicesV78::* field,const char* operation){
 return [state,weak,lend=std::move(lend),field,operation](Args... args){
  auto& error=std::get<sizeof...(Args)-1>(std::forward_as_tuple(args...));AuxiliaryDeliveryV67 delivery;std::shared_ptr<Record> actual;
  if(!borrow(*state,delivery,error)||!receiver(weak,actual,error))return false;
  if(!lend){error="Required genuine TriggerObject engine lender";return false;}
  world::TriggerObjectServicesV28 current;if(!lend(delivery,*actual->owner,current,error))return false;
  if(!same_owner(current.owner,state->scope.services_owner)||!same_owner(current.startup.owner,state->scope.services_owner)){error="TriggerObject startup replaced independent auxiliary authority";return false;}
  if(!state->leaves->validate_current(delivery,error))return false;
  auto& call=current.startup.*field;if(!call){error=std::string("Required genuine reached TriggerObject startup leaf: ")+operation;return false;}
  if(!call(std::forward<Args>(args)...))return false;return state->leaves->validate_current(delivery,error);
 };
}
template<class Record,class... Args>
std::function<bool(Args...)> initialization_member(const std::shared_ptr<AuxiliaryStateV67>& state,std::weak_ptr<Record> weak,
 std::function<bool(Args...)> world::GameObjectInitializationServicesV1::* field,const char* operation){
 return [state,weak,field,operation](Args... args){
  auto& error=std::get<sizeof...(Args)-1>(std::forward_as_tuple(args...));AuxiliaryDeliveryV67 delivery;std::shared_ptr<Record> actual;
  if(!borrow(*state,delivery,error)||!receiver(weak,actual,error))return false;
  world::GameObjectInitializationServicesV1 current;
  if(!state->leaves->lend_initialization(delivery,actual->owner->base(),current,error))return false;
  if(!same_owner(current.owner,state->scope.services_owner)){error="Initialization replaced independent auxiliary authority";return false;}
  if(!state->leaves->validate_current(delivery,error))return false;
  auto& call=current.*field;if(!call){error=std::string("Required genuine GameObject engine leaf: ")+operation;return false;}
  if(!call(std::forward<Args>(args)...))return false;return state->leaves->validate_current(delivery,error);
 };
}
template<class Record>void initialization(const std::shared_ptr<AuxiliaryStateV67>& state,const std::shared_ptr<Record>& actual,world::GameObjectInitializationServicesV1& out){
 out.owner=state->scope.services_owner;out.difficulty_names=state->leaves->difficulty_names.get();out.sound_names=state->leaves->sound_names.get();std::weak_ptr<Record> weak=actual;
 using S=world::GameObjectInitializationServicesV1;
#define BASE_LEAF(name) out.name=initialization_member(state,weak,&S::name,#name)
 BASE_LEAF(condition_init);BASE_LEAF(check_spawn_probability);BASE_LEAF(device_high_performance);BASE_LEAF(load_visual);BASE_LEAF(visual_sync);
 BASE_LEAF(set_visible);BASE_LEAF(init_pf_object);BASE_LEAF(light_set_id);BASE_LEAF(visual_set_light_set);BASE_LEAF(visual_root);BASE_LEAF(node_from_name);BASE_LEAF(update_pf_object);
#undef BASE_LEAF
 // Loader owns whole SetPosition/SetDestination over SAME canonical runtime.
 // Main lends only reached attachment/physical/visual backend leaves.
 out.set_position=[state,weak](const float* position,bool destination,std::string& error){
  AuxiliaryDeliveryV67 delivery;std::shared_ptr<Record> actual;if(!borrow(*state,delivery,error)||!receiver(weak,actual,error))return false;
  world::GameObjectSetPositionServicesV2 leaves;
  if(!state->leaves->lend_position(delivery,actual->owner->base(),leaves,error))return false;
  if(!same_owner(leaves.owner,state->scope.services_owner)){error="SetPosition replaced independent auxiliary authority";return false;}
  if(!state->leaves->validate_current(delivery,error))return false;
  if(!world::game_object_set_position_v2(actual->owner->base(),position,destination,leaves,error))return false;
  return state->leaves->validate_current(delivery,error);
 };
}
template<class Record>bool admit(const std::shared_ptr<AuxiliaryStateV67>& state,const world::CanonicalSourceObjectRequestV1& source,const std::shared_ptr<Record>& actual,const char* name,std::string& error){
 AuxiliaryDeliveryV67 delivery;if(!borrow(*state,delivery,error))return false;
 auto record=std::make_shared<AuxiliaryReleaseRecordV67>();record->record=actual;record->class_name=name;std::weak_ptr<Record> weak=actual;
 record->has_constructed_owner=[weak](){auto value=weak.lock();return value&&bool(value->owner);};
 record->constructor_state=[weak](){auto value=weak.lock();return value?value->constructor_state:CanonicalConstructorStateV89::failed;};
 record->identity=[weak](){auto value=weak.lock();return value&&value->owner?value->owner->base().identity():0;};
 record->destroy_source=[state,weak](std::string& error){AuxiliaryDeliveryV67 delivery;std::shared_ptr<Record> actual;if(!borrow(*state,delivery,error)||!receiver(weak,actual,error))return false;
  // Existing class owns original Dtor prefix/failed-source stickiness; no replay
  // or successful passive drop is manufactured by the catalog.
  if(!actual->owner->destroy(error))return false;return state->leaves->validate_current(delivery,error);};
 record->borrow_save_header=[weak](level::LevelSaveObjectBorrowV2& out,std::string& e){auto actual=weak.lock();if(!actual||actual->constructor_state!=CanonicalConstructorStateV89::completed||!actual->owner){e="Required completed SAME auxiliary OBJS header";return false;}
  auto& b=actual->owner->base();out={};out.identity=reinterpret_cast<const void*>(b.identity());out.checkpoint28=b.byte(0x28);out.gametype48=b.string(0x48);out.map_name=b.string(0x30);out.room64=&b.room64();out.disabled81=b.byte(0x81);out.receiver_lease_v86=actual;e.clear();return true;};
 record->make_save_connection=[weak](NonCharacterRestoreServicesV89 leaves,std::shared_ptr<NonCharacterSaveConnectionV89>& out,std::string& e){
  auto actual=weak.lock();if(!actual){e="Required SAME actual auxiliary C1 save record";return false;}
  auto project=[](auto& receiver,NonCharacterSaveFieldsV89& fields,std::string& e){using R=std::decay_t<decltype(receiver)>;
   if constexpr(std::is_same_v<R,world::CanonicalTriggerObjectV28>)return trigger_object_save_fields_v89(receiver,fields,e);
   else if constexpr(std::is_same_v<R,world::CanonicalDoorV27>)return door_save_fields_v89(receiver,fields,e);
   else if constexpr(std::is_base_of_v<world::CanonicalTriggerZoneV22,R>)return trigger_save_fields_v89(static_cast<world::CanonicalTriggerZoneV22&>(receiver),fields,e);
   else return gameobject_save_fields_v89(receiver.base(),fields,e);
  };return make_noncharacter_record_save_v89(actual,std::move(project),std::move(leaves),out,e);
 };
 if(!state->leaves->retain_release_record(delivery,record,source,error))return false;
 if(!state->leaves->validate_current(delivery,error))return false;
 state->pending=std::move(record);state->pending_erase=[weak](){if(auto value=weak.lock())value->owner.reset();};return true;
}
#define FAMILY_LEAF(name) out.name=member(state,weak,lend,&S::name,#name)
template<class Record>world::DummyContinuationServicesV14 dummy(const std::shared_ptr<AuxiliaryStateV67>& state,std::weak_ptr<Record> weak,AuxiliaryServicesLenderV67<Record,world::DummyContinuationServicesV14> lend){world::DummyContinuationServicesV14 out;out.owner=state->scope.services_owner;using S=world::DummyContinuationServicesV14;FAMILY_LEAF(destroy_base);return out;}
template<class Record>world::SpawnPointServicesV15 spawn(const std::shared_ptr<AuxiliaryStateV67>& state,std::weak_ptr<Record> weak,AuxiliaryServicesLenderV67<Record,world::SpawnPointServicesV15> lend){world::SpawnPointServicesV15 out;out.owner=state->scope.services_owner;using S=world::SpawnPointServicesV15;FAMILY_LEAF(script_id);FAMILY_LEAF(set_object_position);FAMILY_LEAF(set_object_rotation);FAMILY_LEAF(start_script);FAMILY_LEAF(destroy_base);return out;}
template<class Record>world::SpawnSpotServicesV108 spawn_spot(const std::shared_ptr<AuxiliaryStateV67>& state,std::weak_ptr<Record> weak,AuxiliaryServicesLenderV67<Record,world::SpawnSpotServicesV108> lend){world::SpawnSpotServicesV108 out;out.owner=state->scope.services_owner;using S=world::SpawnSpotServicesV108;FAMILY_LEAF(insert);FAMILY_LEAF(erase);FAMILY_LEAF(set_object_position);FAMILY_LEAF(set_object_rotation);FAMILY_LEAF(destroy_base);return out;}

template<class Record>world::DecorServicesV15 decor(const std::shared_ptr<AuxiliaryStateV67>& state,std::weak_ptr<Record> weak,AuxiliaryServicesLenderV67<Record,world::DecorServicesV15> lend){world::DecorServicesV15 out;out.owner=state->scope.services_owner;using S=world::DecorServicesV15;FAMILY_LEAF(visual_sync);FAMILY_LEAF(visual_physical);FAMILY_LEAF(construct_podecor);FAMILY_LEAF(set_physical);FAMILY_LEAF(visual_root);FAMILY_LEAF(load_room);FAMILY_LEAF(extend_bounds);FAMILY_LEAF(destroy_base);return out;}
template<class Record>world::TriggerZoneServicesV22 trigger(const std::shared_ptr<AuxiliaryStateV67>& state,std::weak_ptr<Record> weak,AuxiliaryServicesLenderV67<Record,world::TriggerZoneServicesV22> lend){
 world::TriggerZoneServicesV22 out;out.owner=state->scope.services_owner;using S=world::TriggerZoneServicesV22;
 FAMILY_LEAF(spawn_roll_probability);FAMILY_LEAF(set_bounding_box);FAMILY_LEAF(bind_visual_collision_zone);FAMILY_LEAF(create_zone_physical);
 FAMILY_LEAF(script_id);FAMILY_LEAF(effect_id);FAMILY_LEAF(find_named_object);FAMILY_LEAF(door_state);FAMILY_LEAF(script_flags);
 FAMILY_LEAF(local_player);FAMILY_LEAF(player_count);FAMILY_LEAF(player_character);FAMILY_LEAF(touching);FAMILY_LEAF(zone_inside);FAMILY_LEAF(is_character);FAMILY_LEAF(is_player);
 FAMILY_LEAF(online5);FAMILY_LEAF(virtual54);FAMILY_LEAF(app_delta_ms);FAMILY_LEAF(require_online_update);FAMILY_LEAF(play_idle_sound);FAMILY_LEAF(script_running);FAMILY_LEAF(start_script);
 FAMILY_LEAF(create_marker);FAMILY_LEAF(marker_owner);FAMILY_LEAF(marker_restart);FAMILY_LEAF(marker_visible);FAMILY_LEAF(marker_loop);FAMILY_LEAF(marker_release);FAMILY_LEAF(destroy_network);FAMILY_LEAF(destroy_base);return out;
}
#undef FAMILY_LEAF
const std::vector<std::pair<const char*,std::uint32_t>>& entries(){static const std::vector<std::pair<const char*,std::uint32_t>> values{
 {"Dummy",0x3410a4},{"SpawnPoint",0x340e10},{"SpawnSpot",0x340dec},{"Decor",0x3410fc},{"TriggerZone",0x340ec4},{"AnimatedDecor",0x342600},
 {"CheckpointZone",0x340f2c},{"Door",0x340824},{"TriggerObject",0x340f0c},{"TriggerZoneExitLevel",0x340ea0},{"QuestMoveInZone",0x340f50},{"SoundEmitter",0x340ccc}};return values;}
}bool CatalogAuxiliaryV67::create(ScopeV67 scope,std::shared_ptr<const CatalogAuxiliaryServicesV67> leaves,
 std::shared_ptr<CatalogAuxiliaryV67>& out,PartV67& part,std::string& error){
 if(!leaves||!scope.services_owner||!same_owner(leaves->owner,scope.services_owner)||!leaves->validate_current||
  !leaves->lend_initialization||!leaves->lend_position||!leaves->dummy||!leaves->spawn_point||!leaves->decor||
  !leaves->trigger_zone||!leaves->animated_decor||!leaves->checkpoint||!leaves->door||!leaves->trigger_object||
  !leaves->exit_zone||!leaves->quest_move||!leaves->sound||!leaves->retain_release_record||!leaves->release_completed){
  error="Production auxiliary catalog requires genuine initialization/class/release journal services";return false;
 }
 if(!independent(scope,leaves->owner)||!independent(scope,leaves->difficulty_names)||!independent(scope,leaves->sound_names)){
  error="Auxiliary authority/Arrays cannot own containing World/Level/manager";return false;
 }
 try{
  auto state=std::make_shared<AuxiliaryStateV67>();state->scope=scope;state->leaves=leaves;AuxiliaryDeliveryV67 delivery;if(!borrow(*state,delivery,error))return false;
  PartV67 selected;
  for(const auto& required:entries()){
   const world::CanonicalFactoryEntryV1* original=nullptr;
   for(const auto& candidate:world::canonical_factories_v1())if(candidate.name&&!std::strcmp(candidate.name,required.first)&&candidate.original_address==required.second)original=&candidate;
   if(!original){error=std::string("Required exact original auxiliary factory entry: ")+required.first;return false;}
   selected.entries.push_back(*original);
  }
  CanonicalAuxiliaryInputsV16 inputs;inputs.world=scope.services_owner;
  inputs.dummy=[state](const auto& source,const auto& actual,auto& init,auto& services,auto& e){initialization(state,actual,init);services=dummy(state,std::weak_ptr<CanonicalDummyRecordV16>(actual),state->leaves->dummy);return admit(state,source,actual,"Dummy",e);};
  inputs.spawn_point=[state](const auto& source,const auto& actual,auto& init,auto& services,auto& e){initialization(state,actual,init);services=spawn(state,std::weak_ptr<CanonicalSpawnPointRecordV16>(actual),state->leaves->spawn_point);return admit(state,source,actual,"SpawnPoint",e);};
  inputs.spawn_spot=[state](const auto& source,const auto& actual,auto& init,auto& services,auto& e){initialization(state,actual,init);services=spawn_spot(state,std::weak_ptr<CanonicalSpawnSpotRecordV108>(actual),state->leaves->spawn_spot);return admit(state,source,actual,"SpawnSpot",e);};
  inputs.decor=[state](const auto& source,const auto& actual,auto& init,auto& services,auto& e){initialization(state,actual,init);services=decor(state,std::weak_ptr<CanonicalDecorRecordV16>(actual),state->leaves->decor);return admit(state,source,actual,"Decor",e);};
  inputs.trigger_zone=[state](const auto& source,const auto& actual,auto& init,auto& services,auto& e){initialization(state,actual,init);auto base=init;services=trigger(state,std::weak_ptr<CanonicalTriggerZoneRecordV22>(actual),state->leaves->trigger_zone);services.initialization=std::move(base);return admit(state,source,actual,"TriggerZone",e);};
  inputs.animated_decor=[state](const auto& source,const auto& actual,auto& init,auto& services,auto& e){
   initialization(state,actual,init);std::weak_ptr<CanonicalAnimatedDecorRecordV23> weak=actual;auto lend=state->leaves->animated_decor;
   AuxiliaryServicesLenderV67<CanonicalAnimatedDecorRecordV23,world::DecorServicesV15> inherited=[lend](const auto& delivery,auto& receiver,auto& out,auto& error){world::AnimatedDecorServicesV23 whole;if(!lend(delivery,receiver,whole,error))return false;out=std::move(whole.decor);return true;};
   services.owner=state->scope.services_owner;services.decor=decor(state,weak,std::move(inherited));// Keep original class constructor and same weak record; lend only real
   // engine leaves to its local source InitPost/CallbackRandomAll body.
   AuxiliaryServicesLenderV67<CanonicalAnimatedDecorRecordV23,world::AnimatedDecorServicesV1> animation=[lend](const auto& delivery,auto& receiver,auto& out,auto& error){world::AnimatedDecorServicesV23 whole;if(!lend(delivery,receiver,whole,error))return false;out=std::move(whole.source_init);return true;};
   services.source_init.owner=state->scope.services_owner;
   using A=world::AnimatedDecorServicesV1;
   services.source_init.visual_sync=member(state,weak,animation,&A::visual_sync,"AnimatedDecor ApplyMeshBox");
   services.source_init.visual_physical28=member(state,weak,animation,&A::visual_physical28,"AnimatedDecor actual visual28");
   services.source_init.construct_podecor=member(state,weak,animation,&A::construct_podecor,"AnimatedDecor actual PODecor");
   services.source_init.set_physical=member(state,weak,animation,&A::set_physical,"AnimatedDecor SetPhysicalObject");
   services.source_init.animation_count=member(state,weak,animation,&A::animation_count,"AnimatedDecor timeline count");
   services.source_init.animation_exists=member(state,weak,animation,&A::animation_exists,"AnimatedDecor timeline name");
   services.source_init.play_index=member(state,weak,animation,&A::play_index,"AnimatedDecor timeline Play index");
   services.source_init.play_name=member(state,weak,animation,&A::play_name,"AnimatedDecor timeline Play name");
   services.source_init.random=member(state,weak,animation,&A::random,"AnimatedDecor SAME Random");
   services.source_init.install_random_completion=member(state,weak,animation,&A::install_random_completion,"AnimatedDecor actual completion");
   services.source_init.random_play_assertion=member(state,weak,animation,&A::random_play_assertion,"AnimatedDecor assertion policy");
   services.source_init.update=member(state,weak,animation,&A::update,"AnimatedDecor actual GameObject Update");return admit(state,source,actual,"AnimatedDecor",e);
  };
  inputs.checkpoint=[state](const auto& source,const auto& actual,auto& init,auto& services,auto& e){
   initialization(state,actual,init);std::weak_ptr<CanonicalCheckpointRecordV26> weak=actual;auto lend=state->leaves->checkpoint;services.owner=state->scope.services_owner;using S=world::CheckpointZoneServicesV26;
   services.startup=zone_startup_v76(state,weak,lend);services.whole_collision_begin=member(state,weak,lend,&S::whole_collision_begin,"Checkpoint actual collision/save");services.whole_destroy=member(state,weak,lend,&S::whole_destroy,"Checkpoint source destruction");return admit(state,source,actual,"CheckpointZone",e);
  };
  inputs.door=[state](const auto& source,const auto& actual,auto& init,auto& services,auto& e){
   initialization(state,actual,init);std::weak_ptr<CanonicalDoorRecordV27> weak=actual;auto lend=state->leaves->door;services.owner=state->scope.services_owner;using S=world::DoorServicesV27;
   services.startup.owner=state->scope.services_owner;
   AuxiliaryServicesLenderV67<CanonicalDoorRecordV27,world::DoorServicesV27> zone_lend=lend;
   // Reuse same nested Zone resource forwarding; DoorStartup adds its local leaves.
   services.startup.zone.owner=state->scope.services_owner;
   using D=world::DoorStartupServicesV77;
#define DOOR_STARTUP(name) services.startup.name=door_startup_member_v77(state,weak,lend,&D::name,#name)
   DOOR_STARTUP(borrow_tables);DOOR_STARTUP(register_animation_callbacks);DOOR_STARTUP(apply_mesh_box);DOOR_STARTUP(create_door_physical);
   DOOR_STARTUP(borrow_sound_manager);DOOR_STARTUP(play_idle_animation);DOOR_STARTUP(physical_filter);DOOR_STARTUP(flag_floor_dead_end);DOOR_STARTUP(source_delete);
#undef DOOR_STARTUP
   services.startup.zone.set_bounding_box=[state,weak,lend](auto& base,const float* box,bool flag,auto& error){AuxiliaryDeliveryV67 d;std::shared_ptr<CanonicalDoorRecordV27> r;if(!borrow(*state,d,error)||!receiver(weak,r,error))return false;world::DoorServicesV27 current;if(!lend(d,*r->owner,current,error))return false;
    if(!same_owner(current.owner,state->scope.services_owner)||!same_owner(current.startup.zone.owner,state->scope.services_owner)||!current.startup.zone.set_bounding_box){error="Required genuine SAME Door Zone bounding service";return false;}
    if(!state->leaves->validate_current(d,error))return false;if(&base!=&r->owner->base()){error="Required SAME Door Zone bounding receiver";return false;}
    if(!current.startup.zone.set_bounding_box(base,box,flag,error))return false;return state->leaves->validate_current(d,error);};
   services.startup.zone.create_zone_physical=[state,weak,lend](auto& base,bool trigger,auto& error){AuxiliaryDeliveryV67 d;std::shared_ptr<CanonicalDoorRecordV27> r;if(!borrow(*state,d,error)||!receiver(weak,r,error))return false;world::DoorServicesV27 current;if(!lend(d,*r->owner,current,error))return false;
    if(!same_owner(current.owner,state->scope.services_owner)||!same_owner(current.startup.zone.owner,state->scope.services_owner)||!current.startup.zone.create_zone_physical){error="Required genuine SAME Door Zone physical service";return false;}
    if(!state->leaves->validate_current(d,error))return false;if(&base!=&r->owner->base()){error="Required SAME Door Zone physical receiver";return false;}
    if(!current.startup.zone.create_zone_physical(base,trigger,error))return false;return state->leaves->validate_current(d,error);};
   services.startup.zone.bind_visual_collision_zone=[state,weak,lend](auto& base,const char* name,auto& node,auto& pin,auto& error){AuxiliaryDeliveryV67 d;std::shared_ptr<CanonicalDoorRecordV27> r;if(!borrow(*state,d,error)||!receiver(weak,r,error))return false;world::DoorServicesV27 current;if(!lend(d,*r->owner,current,error))return false;
    if(!same_owner(current.owner,state->scope.services_owner)||!same_owner(current.startup.zone.owner,state->scope.services_owner)||!current.startup.zone.bind_visual_collision_zone){error="Required genuine SAME Door Zone colzone service";return false;}
    if(!state->leaves->validate_current(d,error))return false;if(&base!=&r->owner->base()){error="Required SAME Door Zone colzone receiver";return false;}
    if(!current.startup.zone.bind_visual_collision_zone(base,name,node,pin,error))return false;return state->leaves->validate_current(d,error);};
   services.whole_destroy=member(state,weak,lend,&S::whole_destroy,"Door source destruction");return admit(state,source,actual,"Door",e);
  };
  inputs.trigger_object=[state](const auto& source,const auto& actual,auto& init,auto& services,auto& e){
   initialization(state,actual,init);std::weak_ptr<CanonicalTriggerObjectRecordV28> weak=actual;auto lend=state->leaves->trigger_object;services.owner=state->scope.services_owner;using S=world::TriggerObjectServicesV28;
   services.startup.owner=state->scope.services_owner;services.startup.zone.owner=state->scope.services_owner;using T=world::TriggerObjectStartupServicesV78;
#define TRIGGER_OBJECT_STARTUP(name) services.startup.name=trigger_object_startup_member_v78(state,weak,lend,&T::name,#name)
   TRIGGER_OBJECT_STARTUP(borrow_tables);TRIGGER_OBJECT_STARTUP(borrow_script_manager);TRIGGER_OBJECT_STARTUP(borrow_conditions);
   TRIGGER_OBJECT_STARTUP(play_idle_animation);TRIGGER_OBJECT_STARTUP(create_physical);TRIGGER_OBJECT_STARTUP(borrow_sound_manager);TRIGGER_OBJECT_STARTUP(load_external_script);TRIGGER_OBJECT_STARTUP(source_delete);
#undef TRIGGER_OBJECT_STARTUP
   services.startup.zone.set_bounding_box=[state,weak,lend](auto& base,const float* box,bool flag,auto& error){AuxiliaryDeliveryV67 d;std::shared_ptr<CanonicalTriggerObjectRecordV28> r;if(!borrow(*state,d,error)||!receiver(weak,r,error))return false;world::TriggerObjectServicesV28 current;if(!lend(d,*r->owner,current,error))return false;
    if(!same_owner(current.owner,state->scope.services_owner)||!same_owner(current.startup.zone.owner,state->scope.services_owner)||!current.startup.zone.set_bounding_box){error="Required genuine SAME TriggerObject Zone bounding service";return false;}
    if(!state->leaves->validate_current(d,error)||&base!=&r->owner->base())return false;
    if(!current.startup.zone.set_bounding_box(base,box,flag,error))return false;return state->leaves->validate_current(d,error);};
   services.startup.zone.create_zone_physical=[state,weak,lend](auto& base,bool trigger,auto& error){AuxiliaryDeliveryV67 d;std::shared_ptr<CanonicalTriggerObjectRecordV28> r;if(!borrow(*state,d,error)||!receiver(weak,r,error))return false;world::TriggerObjectServicesV28 current;if(!lend(d,*r->owner,current,error))return false;
    if(!same_owner(current.owner,state->scope.services_owner)||!same_owner(current.startup.zone.owner,state->scope.services_owner)||!current.startup.zone.create_zone_physical){error="Required genuine SAME TriggerObject Zone physical service";return false;}
    if(!state->leaves->validate_current(d,error)||&base!=&r->owner->base())return false;
    if(!current.startup.zone.create_zone_physical(base,trigger,error))return false;return state->leaves->validate_current(d,error);};
   services.startup.zone.bind_visual_collision_zone=[state,weak,lend](auto& base,const char* name,auto& node,auto& pin,auto& error){AuxiliaryDeliveryV67 d;std::shared_ptr<CanonicalTriggerObjectRecordV28> r;if(!borrow(*state,d,error)||!receiver(weak,r,error))return false;world::TriggerObjectServicesV28 current;if(!lend(d,*r->owner,current,error))return false;
    if(!same_owner(current.owner,state->scope.services_owner)||!same_owner(current.startup.zone.owner,state->scope.services_owner)||!current.startup.zone.bind_visual_collision_zone){error="Required genuine SAME TriggerObject Zone colzone service";return false;}
    if(!state->leaves->validate_current(d,error)||&base!=&r->owner->base())return false;
    if(!current.startup.zone.bind_visual_collision_zone(base,name,node,pin,error))return false;return state->leaves->validate_current(d,error);};
   services.whole_update=member(state,weak,lend,&S::whole_update,"TriggerObject Update");services.destroy_trigger_base=member(state,weak,lend,&S::destroy_trigger_base,"TriggerObject inherited TriggerD1/network/Zone/GameObjectD2");services.whole_interact=member(state,weak,lend,&S::whole_interact,"TriggerObject actual interaction");return admit(state,source,actual,"TriggerObject",e);
  };
  inputs.exit_zone=[state](const auto& source,const auto& actual,auto& init,auto& services,auto& e){
   initialization(state,actual,init);auto base=init;std::weak_ptr<CanonicalExitZoneRecordV29> weak=actual;auto lend=state->leaves->exit_zone;
   AuxiliaryServicesLenderV67<CanonicalExitZoneRecordV29,world::TriggerZoneServicesV22> inherited=[lend](const auto& delivery,auto& receiver,auto& out,auto& error){world::ExitZoneServicesV29 whole;if(!lend(delivery,receiver,whole,error))return false;out=std::move(whole.trigger);return true;};
   services.owner=state->scope.services_owner;services.trigger=trigger(state,weak,std::move(inherited));services.trigger.initialization=std::move(base);using S=world::ExitZoneServicesV29;
   services.level_name_id=member(state,weak,lend,&S::level_name_id,"Exit actual LevelList name ID");services.whole_update=member(state,weak,lend,&S::whole_update,"Exit actual transition");services.whole_destroy=member(state,weak,lend,&S::whole_destroy,"Exit source destruction");return admit(state,source,actual,"TriggerZoneExitLevel",e);
  };
  inputs.quest_move=[state](const auto& source,const auto& actual,auto& init,auto& services,auto& e){
   initialization(state,actual,init);std::weak_ptr<CanonicalQuestMoveRecordV31> weak=actual;auto lend=state->leaves->quest_move;services.owner=state->scope.services_owner;using S=world::QuestMoveZoneServicesV31;
   services.startup=zone_startup_v76(state,weak,lend);services.whole_collision_begin=member(state,weak,lend,&S::whole_collision_begin,"QuestMove actual quest collision");services.whole_destroy=member(state,weak,lend,&S::whole_destroy,"QuestMove source destruction");return admit(state,source,actual,"QuestMoveInZone",e);
  };
  inputs.sound=[state](const auto& source,const auto& actual,auto& init,auto& services,auto& e){
   initialization(state,actual,init);std::weak_ptr<CanonicalSoundEmitterRecordV32> weak=actual;auto lend=state->leaves->sound;services.owner=state->scope.services_owner;services.sound_names=state->leaves->sound_names;using S=world::SoundEmitterServicesV32;
   services.whole_update=member(state,weak,lend,&S::whole_update,"SoundEmitter actual audio update");services.whole_destroy=member(state,weak,lend,&S::whole_destroy,"SoundEmitter actual audio shutdown");return admit(state,source,actual,"SoundEmitter",e);
  };
  // Containers have their separate actual catalog part. No dormant provider
  // or TriggerTrap alias is advertised as an authored SWAMP auxiliary entry.
  auto actual=std::shared_ptr<CatalogAuxiliaryV67>(new CatalogAuxiliaryV67);actual->state_=std::move(state);actual->families_=std::make_unique<CanonicalAuxiliaryFamiliesV16>(std::move(inputs));
  selected.owner=actual;selected.construct=[actual](const auto& entry,const auto& source,auto& receiver,auto& error){return actual->construct(entry,source,receiver,error);};
  out=std::move(actual);part=std::move(selected);error.clear();return true;
 }catch(const std::exception& ex){error=ex.what();return false;}
}
bool CatalogAuxiliaryV67::construct(const world::CanonicalFactoryEntryV1& entry,const world::CanonicalSourceObjectRequestV1& source,world::CanonicalClassReceiverV1& out,std::string& error){
 if(busy_){failed_=true;error="Production auxiliary constructor reentered";return false;}if(failed_){error="Production auxiliary failed source prefix retained";return false;}
 busy_=true;struct Guard{CatalogAuxiliaryV67& catalog;~Guard(){catalog.busy_=false;catalog.state_->pending.reset();catalog.state_->pending_erase={};}} guard{*this};
 AuxiliaryDeliveryV67 delivery;if(!borrow(*state_,delivery,error))return false;
 world::CanonicalClassReceiverV1 made;if(!families_->construct(entry,source,made,error))return false;
 if(failed_||!state_->leaves->validate_current(delivery,error))return false;
 if(!state_->pending||!state_->pending->identity||state_->pending->identity()!=made.object.identity){error="Auxiliary C1 did not match its admitted SAME release record";failed_=true;return false;}
 state_->erase.emplace(made.object.identity,state_->pending_erase);out=std::move(made);error.clear();return true;
}
bool CatalogAuxiliaryV67::borrow_spawn_point(std::uintptr_t identity,std::shared_ptr<world::CanonicalSpawnPointV15>& out,std::string& error){
 if(busy_||failed_){error="SpawnPoint borrow requires a completed live auxiliary constructor scope";return false;}
 if(!identity||!state_||!families_){error="Required retained actual SpawnPoint identity/catalog";return false;}
 busy_=true;struct Guard{bool& value;~Guard(){value=false;}} guard{busy_};
 AuxiliaryDeliveryV67 delivery;if(!borrow(*state_,delivery,error))return false;
 if(state_->erase.find(identity)==state_->erase.end()){error="SpawnPoint identity is not admitted or was retired";return false;}
 for(const auto& record:families_->spawn_points()){
  if(record&&record->owner&&record->owner->base().identity()==identity){
   out=std::shared_ptr<world::CanonicalSpawnPointV15>(record,record->owner.get());
   error.clear();return true;
  }
 }
 error="Required SAME canonical SpawnPoint record for source placement";return false;
}
bool CatalogAuxiliaryV67::erase_after_source_release(std::uintptr_t identity,std::string& error){
 if(busy_){error="Cannot erase auxiliary receiver during constructor delivery";return false;}auto at=state_->erase.find(identity);
 if(at==state_->erase.end()){error="Required SAME retained auxiliary release identity";return false;}
 AuxiliaryDeliveryV67 delivery;if(!borrow(*state_,delivery,error)||!state_->leaves->release_completed(delivery,identity,error)||!state_->leaves->validate_current(delivery,error))return false;
 // Confirmation includes exhausted transport/native/draw aliases. C1 storage
 // drops only now; the family may retain a diagnostic runtime/declaration shell.
 at->second();state_->erase.erase(at);error.clear();return true;
}
bool make_auxiliary_catalog_part_v67(ScopeV67 scope,CatalogAuxiliaryServicesV67 leaves,PartV67& out,std::string& error){
 std::shared_ptr<CatalogAuxiliaryV67> actual;return CatalogAuxiliaryV67::create(std::move(scope),std::make_shared<const CatalogAuxiliaryServicesV67>(std::move(leaves)),actual,out,error);
}
}
