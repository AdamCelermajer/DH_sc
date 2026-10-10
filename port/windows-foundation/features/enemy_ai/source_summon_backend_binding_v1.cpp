#include "source_summon_backend_binding_v1.hpp"

#include <cstring>

namespace dh2::enemy_ai {
namespace {
using LevelBorrow=dh::foundation::actor_frame::SourceCurrentLevelBorrowV1;
bool same_receiver(const world::CanonicalObjectBorrowV1& a,
 const world::CanonicalObjectBorrowV1& b){
 return a.identity&&a.identity==b.identity&&a.lease&&b.lease&&
  a.lease.get()==b.lease.get()&&!a.lease.owner_before(b.lease)&&
  !b.lease.owner_before(a.lease)&&a.context==b.context&&
  a.shared_handle==b.shared_handle&&a.class_name20==b.class_name20;
}
struct SourceSpawnRequestLeaseV1 {
 std::shared_ptr<void> caller;
 std::string generated_name;
};
}

SourceSummonBackendBindingV1::SourceSummonBackendBindingV1(SourceSummonBackendInputsV1 inputs)
 :inputs_(std::move(inputs)){
 services_.owner=inputs_.caller;
 services_.characters=inputs_.character_table;
 services_.create_npc=[this](std::int32_t id,SourceCreateNpcResultV1& result,std::string& error){
  return create_npc(id,result,error);
 };
 services_.source_object_pose=[this](auto identity,auto& position,auto& rotation,std::string& error){
  if(!inputs_.leaves.source_object_pose){error="Required actual source-object pose provider";return false;}
  return inputs_.leaves.source_object_pose(identity,position,rotation,error);
 };
 services_.source_position_from_offsets=[this](auto identity,const auto& base,const auto& rotation,
   const auto& offset,bool orient,auto& result,std::string& error){
  if(!inputs_.leaves.position_from_offsets){error="Required source GetLookAtVec/offset transform provider";return false;}
  return inputs_.leaves.position_from_offsets(identity,base,rotation,offset,orient,result,error);
 };
 services_.set_initial_position=[this](auto& receiver,const auto& position,std::string& error){
  if(!active_level(error)||!inputs_.leaves.set_initial_position){
   if(error.empty())error="Required same Character SetInitialPosition/PFWorld service";return false;
  }
  std::shared_ptr<world::CanonicalCharacterCandidateRecordV60> record;
  if(!same_character_receiver(receiver,record,error))return false;
  return inputs_.leaves.set_initial_position(active_level_,receiver,position,error);
 };
 services_.set_position=[this](auto& receiver,const auto& position,bool destination,std::string& error){
  if(!active_level(error))return false;
  std::shared_ptr<world::CanonicalCharacterCandidateRecordV60> record;
  if(!same_character_receiver(receiver,record,error))return false;
  if(!receiver.set_position){error="Required same Character SetPosition/physical/visual owner";return false;}
  return receiver.set_position(position,destination,error);
 };
 services_.set_rotation=[this](auto& receiver,const auto& rotation,std::string& error){
  if(!active_level(error)||!inputs_.leaves.set_rotation){
   if(error.empty())error="Required actual same Character SetRotation owner";return false;
  }
  std::shared_ptr<world::CanonicalCharacterCandidateRecordV60> record;
  if(!same_character_receiver(receiver,record,error))return false;
  return inputs_.leaves.set_rotation(active_level_,receiver,rotation,error);
 };
 services_.mark_summoned=[this](auto& receiver,std::string& error){
  std::shared_ptr<world::CanonicalCharacterCandidateRecordV60> record;
  if(!same_character_receiver(receiver,record,error))return false;
  if(!inputs_.leaves.mark_summoned){error="Required same Character summoned byte+0x14e4 owner";return false;}
  return inputs_.leaves.mark_summoned(*record,error);
 };
 services_.room_add_initial_object=[this](auto room,auto character,bool& accepted,std::string& error){
  if(!active_level(error)||!inputs_.leaves.room_add_initial_object){
   if(error.empty())error="Required same caller RoomZone::AddInitialObject";return false;
  }
  return inputs_.leaves.room_add_initial_object(active_level_,room,character,accepted,error);
 };
 services_.manager_add_no_room=[this](auto& receiver,std::string& error){
  if(!active_level(error))return false;
  if(!inputs_.objects){error="Required same source ObjectManager no-room owner";return false;}
  return inputs_.objects->source_add_no_room_object_v89(receiver.object,error);
 };
 services_.zone_entered=[this](auto& receiver,std::string& error){
  if(!active_level(error)){return false;}
  std::shared_ptr<world::CanonicalCharacterCandidateRecordV60> record;
  if(!same_character_receiver(receiver,record,error))return false;
  if(!inputs_.leaves.zone_entered){error="Required same-record Character ZoneEntered source owner";return false;}
  return inputs_.leaves.zone_entered(*record,error);
 };
 services_.set_spawn_state=[this](auto& receiver,bool a,bool b,std::string& error){
  std::shared_ptr<world::CanonicalCharacterCandidateRecordV60> record;
  if(!same_character_receiver(receiver,record,error))return false;
  if(!inputs_.leaves.set_spawn_state){error="Required same-record CharStateMachine::SM_SetSpawnState";return false;}
  return inputs_.leaves.set_spawn_state(*record,a,b,error);
 };
 services_.forward_animation=[this](auto& receiver,std::int32_t animation,std::string& error){
  std::shared_ptr<world::CanonicalCharacterCandidateRecordV60> record;
  if(!same_character_receiver(receiver,record,error))return false;
  if(!inputs_.leaves.forward_animation){error="Required same-record VisualObject::ForwardAnim";return false;}
  return inputs_.leaves.forward_animation(*record,animation,error);
 };
 services_.online_byte5=[this](bool& value,std::string& error){
  if(!inputs_.leaves.online_byte5){error="Required same Application GetOnline byte5 owner";return false;}
  return inputs_.leaves.online_byte5(value,error);
 };
 services_.assign_network_id=[this](auto& receiver,std::string& error){
  std::shared_ptr<world::CanonicalCharacterCandidateRecordV60> record;
  if(!same_character_receiver(receiver,record,error))return false;
  if(!inputs_.leaves.assign_network_id){error="Required same ObjectManager AssignObjectNetworkId owner";return false;}
  return inputs_.leaves.assign_network_id(*record,error);
 };
 services_.online_post_assignment=[this](auto& receiver,auto owner,std::string& error){
  std::shared_ptr<world::CanonicalCharacterCandidateRecordV60> record;
  if(!same_character_receiver(receiver,record,error))return false;
  if(!inputs_.leaves.online_post_assignment){error="Required same source _Summon online tail owner";return false;}
  return inputs_.leaves.online_post_assignment(*record,owner,error);
 };

 spawn_owner_.manager=inputs_.objects.get();spawn_owner_.properties=inputs_.properties;
 spawn_owner_.spawn={this,spawn_construct,spawn_debug,spawn_resolve,spawn_condition,
  spawn_virtual38,spawn_append_pending,spawn_receiver};
 create_services_.characters=inputs_.character_table;
 create_services_.source_manager_spawn=[this](const char* type,const char* name,bool deferred,bool network,
   auto& receiver,bool& created,std::string& error){
  return source_manager_spawn(type,name,deferred,network,receiver,created,error);
 };
 create_services_.set_idle_state=[this](auto& receiver,bool idle,std::string& error){
  if(idle){error="Source CreateNPC requires SM_SetIdleState(false)";return false;}
  std::shared_ptr<world::CanonicalCharacterCandidateRecordV60> record;
  if(!same_character_receiver(receiver,record,error))return false;
  if(!inputs_.leaves.set_idle_state){error="Required same Character CharStateMachine::SM_SetIdleState owner";return false;}
  return inputs_.leaves.set_idle_state(*record,error);
 };
}

bool SourceSummonBackendBindingV1::validate_inputs(std::string& error)const{
 if(!inputs_.characters||!inputs_.receivers||!inputs_.objects||!inputs_.properties||
    !inputs_.current_level||!inputs_.character_table||!inputs_.caller.lease||
    !inputs_.caller.identity||!inputs_.caller.position160||!inputs_.caller.rotation16c||
    !inputs_.resolve_same_receiver){
  error="Required same CandidateFactory/receiver binding/ObjectManager/PropertyMap/CurrentLevel and caller owner";return false;
 }
 const auto& graph=inputs_.current_level->graph();
 if(!graph.objects||graph.objects.get()!=inputs_.objects.get()||
    graph.objects.owner_before(inputs_.objects)||inputs_.objects.owner_before(graph.objects)){
  error="SourceSummon must use the exact CurrentLevel graph ObjectManager";return false;
 }
 std::int32_t key{};const world::CanonicalObjectBorrowV1* actor{};
 bool found=inputs_.objects->source_ordered_begin_v38(key,actor);
 for(std::uint32_t n=0;found&&n<inputs_.objects->source_map_size1c_v38();++n){
  if(actor&&actor->identity==inputs_.caller.identity&&actor->lease&&
     actor->lease.get()==inputs_.caller.lease.get()&&
     !actor->lease.owner_before(inputs_.caller.lease)&&
     !inputs_.caller.lease.owner_before(actor->lease))break;
  if(n+1>=inputs_.objects->source_map_size1c_v38()){actor=nullptr;break;}
  const auto previous=key;found=inputs_.objects->source_ordered_next_v38(previous,key,actor);
 }
 if(!actor||actor->identity!=inputs_.caller.identity||!actor->lease||
    actor->lease.get()!=inputs_.caller.lease.get()||
    actor->lease.owner_before(inputs_.caller.lease)||
    inputs_.caller.lease.owner_before(actor->lease)){
  error="SourceSummon caller is not the same source receiver published in this ObjectManager";return false;
 }
 if(!actor->source_room2f4_v89||*actor->source_room2f4_v89!=inputs_.caller.room){
  error="SourceSummon room argument is not the same caller GameObject+0x2f4 room owner";return false;
 }
 world::CanonicalClassReceiverV1 caller_receiver;
 if(!inputs_.resolve_same_receiver(*actor,caller_receiver,error))return false;
 if(!same_receiver(*actor,caller_receiver.object)){
  error="SourceSummon caller resolver returned another raw receiver/control block";return false;
 }
 error.clear();return true;
}

bool SourceSummonBackendBindingV1::same_character_receiver(
 const world::CanonicalClassReceiverV1& receiver,
 std::shared_ptr<world::CanonicalCharacterCandidateRecordV60>& record,
 std::string& error){
 if(!active_level(error))return false;
 record.reset();
 if(!inputs_.characters||!inputs_.receivers||!inputs_.objects){
  error="Required same CandidateFactory/class-receiver/ObjectManager owners";return false;
 }
 record=inputs_.characters->find(receiver.object.identity);
 if(!record||!record->actor||!record->actor->object){
  error="Required same retained CandidateFactory Character record";return false;
 }
 const auto expected=record->actor->canonical(record);
 if(!same_receiver(receiver.object,expected)){
  error="Summon callback receiver differs from the same CandidateFactory actor/record lease";return false;
 }
 if(!receiver.source_lease||!record->source.source_lease||
    receiver.source_lease.get()!=record->source.source_lease.get()||
    receiver.source_lease.owner_before(record->source.source_lease)||
    record->source.source_lease.owner_before(receiver.source_lease)){
  error="Summon callback receiver lost the source Character-create request lease";return false;
 }
 const auto* published=inputs_.objects->object(expected.shared_handle?expected.shared_handle->key:0);
 if(!published||!same_receiver(*published,expected)){
  error="Summon callback Character is no longer the same ObjectManager publication";return false;
 }
 const auto* retained=inputs_.receivers->receiver(expected,error);
 if(!retained||!same_receiver(retained->object,expected)){
  if(error.empty())error="Required same retained class receiver";return false;
 }
 if(!same_receiver(receiver.object,retained->object)){
  error="Summon callback receiver differs from the source class-receiver binding";return false;
 }
 error.clear();return true;
}

bool SourceSummonBackendBindingV1::current_level(LevelBorrow& out,std::string& error){
 if(!validate_inputs(error))return false;
 if(!inputs_.current_level->current(out,error)||!out){
  if(error.empty())error="Required actual current GS Level for source _Summon";return false;
 }
 return true;
}
bool SourceSummonBackendBindingV1::active_level(std::string& error){
 if(!callback_active_||!active_level_||!inputs_.current_level||
    !inputs_.current_level->still_current(active_level_,error)){
  if(error.empty())error="Required same active SourceCurrentLevelBorrowV1";return false;
 }
 return true;
}

bool SourceSummonBackendBindingV1::construct_character(const world::CanonicalFactoryEntryV1& entry,
 const world::CanonicalSourceObjectRequestV1& request,world::CanonicalClassReceiverV1& out,std::string& error){
 if(!active_level(error))return false;
 auto class_services=inputs_.receivers->services();world::CanonicalObjectBorrowV1 object{};
 if(!class_services.construct||!class_services.construct(class_services.context,entry,request,object,error))return false;
 const auto* retained=inputs_.receivers->receiver(object,error);if(!retained)return false;
 world::CanonicalClassReceiverV1 externally_resolved;
 if(!inputs_.resolve_same_receiver(object,externally_resolved,error))return false;
 if(!same_receiver(retained->object,externally_resolved.object)){
  error="Caller same-receiver resolver returned another raw receiver/control block";return false;
 }
 auto record=inputs_.characters->find(object.identity);
 if(!record||!record->actor||!record->actor->object){error="Required same CandidateFactory record after construction";return false;}
 auto expected=record->actor->canonical(record);
 if(!same_receiver(object,expected)){
  error="Class receiver did not resolve to the same CandidateFactory record/actor/lease";return false;
 }
 out=*retained;error.clear();return true;
}

bool SourceSummonBackendBindingV1::resolve_handle(target_providers::Handle16& handle,bool refresh,
 const world::CanonicalObjectBorrowV1*& out,std::string& error){
 if(!inputs_.objects){error="Required same canonical ObjectManager";return false;}
 return inputs_.objects->resolve_handle_v4(handle,refresh,out,{},error);
}
bool SourceSummonBackendBindingV1::resolve_receiver(const world::CanonicalObjectBorrowV1& object,
 const world::CanonicalClassReceiverV1*& out,std::string& error){
 out=nullptr;if(!active_level(error))return false;
 const auto* retained=inputs_.receivers->receiver(object,error);if(!retained)return false;
 world::CanonicalClassReceiverV1 resolved;
 if(!inputs_.resolve_same_receiver(object,resolved,error))return false;
 if(!same_receiver(retained->object,resolved.object)){
  error="Same-record resolver returned another canonical receiver";return false;
 }
 auto record=inputs_.characters->find(object.identity);
 if(!record||!record->actor||!record->actor->object){error="Required same CandidateFactory record for source Spawn handle";return false;}
 const auto expected=record->actor->canonical(record);
 if(!same_receiver(expected,object)){error="Manager handle escaped the CandidateFactory source actor/record";return false;}
 out=retained;error.clear();return true;
}

bool SourceSummonBackendBindingV1::create_npc(std::int32_t id,SourceCreateNpcResultV1& out,std::string& error){
 if(!current_level(active_level_,error))return false;
 return source_create_npc_v1(id,nullptr,false,false,create_services_,out,error);
}
bool SourceSummonBackendBindingV1::source_manager_spawn(const char* type,const char* name,
 bool deferred,bool network,world::CanonicalClassReceiverV1& out,bool& created,std::string& error){
 if(!active_level(error))return false;
 if(!type||std::strcmp(type,"Character")||!name||!deferred||network){
  error="Source _Summon requires Spawn(Character,generated-name,deferred=true,network=false)";return false;
 }
 auto request=std::make_shared<SourceSpawnRequestLeaseV1>();request->caller=inputs_.caller.lease;request->generated_name=name;
 request_lease_=request;
 return source_canonical_manager_spawn_v1(spawn_owner_,type,request->generated_name.c_str(),deferred,network,out,created,error);
}

bool SourceSummonBackendBindingV1::spawn_construct(void* raw,const world::CanonicalFactoryEntryV1& entry,
 world::CanonicalClassReceiverV1& out,std::string& error){
 auto& self=*static_cast<SourceSummonBackendBindingV1*>(raw);
 if(!self.request_lease_||!self.active_level(error))return false;
 world::CanonicalSourceObjectRequestV1 source;
 source.source_lease=self.request_lease_;source.native_spawn_name_v68=
  static_cast<SourceSpawnRequestLeaseV1*>(self.request_lease_.get())->generated_name.c_str();
 return self.construct_character(entry,source,out,error);
}
bool SourceSummonBackendBindingV1::spawn_debug(void* raw,const char* type,std::string& error){
 auto& self=*static_cast<SourceSummonBackendBindingV1*>(raw);
 if(!self.inputs_.leaves.unknown_type_debug){error="Required source ObjectManager unknown-type Debug owner";return false;}
 return self.inputs_.leaves.unknown_type_debug(type,error);
}
bool SourceSummonBackendBindingV1::spawn_resolve(void* raw,target_providers::Handle16& handle,
 bool refresh,const world::CanonicalObjectBorrowV1*& out,std::string& error){
 return static_cast<SourceSummonBackendBindingV1*>(raw)->resolve_handle(handle,refresh,out,error);
}
bool SourceSummonBackendBindingV1::spawn_condition(void*,const world::CanonicalObjectBorrowV1&,
 bool,std::string& error){error="Source _Summon Spawn(deferred=true) must not reach TestEnableCondition";return false;}
bool SourceSummonBackendBindingV1::spawn_virtual38(void* raw,const world::CanonicalObjectBorrowV1& object,
 bool& accepted,std::string& error){
 auto& self=*static_cast<SourceSummonBackendBindingV1*>(raw);const world::CanonicalClassReceiverV1* receiver{};
 if(!self.resolve_receiver(object,receiver,error))return false;
 if(!receiver)return false;if(!receiver->source_is_updatable_v95){error="Required actual source virtual38 receiver";return false;}
 return receiver->source_is_updatable_v95(accepted,error);
}
bool SourceSummonBackendBindingV1::spawn_append_pending(void* raw,const world::CanonicalObjectBorrowV1& object,
 std::string& error){auto& self=*static_cast<SourceSummonBackendBindingV1*>(raw);
 if(!self.active_level(error))return false;
 return self.inputs_.objects&&self.inputs_.objects->append_pending(object,error);}
bool SourceSummonBackendBindingV1::spawn_receiver(void* raw,const world::CanonicalObjectBorrowV1& object,
 const world::CanonicalClassReceiverV1*& out,std::string& error){
 return static_cast<SourceSummonBackendBindingV1*>(raw)->resolve_receiver(object,out,error);
}

bool SourceSummonBackendBindingV1::gameplay_callback(const dh2_script_value* args,std::uint32_t count,
 dh2_script_value* results,std::uint32_t capacity,std::uint32_t* returned,char* error,std::size_t size){
 if(callback_active_)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
 callback_active_=true;active_level_={};request_lease_.reset();
 const auto reset=[this](){active_level_={};request_lease_.reset();callback_active_=false;};
 try{
  const auto status=source_summon_callback_v1(&services_,args,count,results,capacity,returned,error,size);
  reset();return status;
 }catch(...){
  reset();if(returned)*returned=0;
  if(error&&size)std::snprintf(error,size,"Source _Summon service threw");
  return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
 }
}
int SourceSummonBackendBindingV1::summon_callback(void* raw,const dh2_script_value* args,std::uint32_t count,
 dh2_script_value* results,std::uint32_t capacity,std::uint32_t* returned,char* error,std::size_t size){
 if(!raw)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
 try{return static_cast<SourceSummonBackendBindingV1*>(raw)->gameplay_callback(args,count,results,capacity,returned,error,size);}
 catch(...){if(returned)*returned=0;if(error&&size)std::snprintf(error,size,"Source _Summon native provider threw");return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
}
int SourceSummonBackendBindingV1::gameplay_binding(void* raw,std::uint32_t address,
 dh2_script_function* function,void** context){
 if(!raw||!function||!context)return -1;
 if(address!=0x39193cu)return 0;
 *function=&summon_callback;*context=raw;return 1;
}

bool SourceSummonBackendBindingV1::attach_gameplay_binding(
 character::CharacterScriptSessionInput& input,std::string& error){
 if(binding_installed_){error="Source _Summon gameplay binding cannot be attached/replaced twice";return false;}
 previous_gameplay_context_=input.gameplay_context;previous_gameplay_binding_=input.gameplay_binding;
 input.gameplay_context=this;input.gameplay_binding=&chained_gameplay_binding;
 binding_installed_=true;error.clear();return true;
}
int SourceSummonBackendBindingV1::chained_gameplay_binding(void* raw,std::uint32_t address,
 dh2_script_function* function,void** context){
 if(!raw||!function||!context)return -1;auto& self=*static_cast<SourceSummonBackendBindingV1*>(raw);
 const auto own=gameplay_binding(&self,address,function,context);if(own)return own;
 if(!self.previous_gameplay_binding_)return 0;
 return self.previous_gameplay_binding_(self.previous_gameplay_context_,address,function,context);
}

} // namespace dh2::enemy_ai


