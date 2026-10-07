#pragma once
#include "level_source_loading_v43.hpp"
#include "canonical_receiver_transport_v1.hpp"
#include "module_room_zone_connection_v91.hpp"
#include <object_enable_condition_v2.hpp>
namespace dh2::loader {
struct SourceObjectLoadingLeavesV95 {
 // Independent source authority. Methods weakly borrow the same campaign.
 std::shared_ptr<void> owner;
 std::function<bool(std::string&)> current;
 std::function<bool(const world::CanonicalObjectBorrowV1&,bool,std::string&)> test_enable;
 std::function<bool(std::string&)> null_handle_assertion;
};
// Source bodies use actual manager map/list nodes. No parallel actor registry,
// source phase, copied receiver vector or class-policy fallback is introduced.
class SourceObjectLoadingV95 final:public std::enable_shared_from_this<SourceObjectLoadingV95> {
 std::weak_ptr<world::CanonicalObjectManagerV1> manager_;
 std::weak_ptr<CanonicalReceiverTransportV1> transport_;
 std::weak_ptr<ModuleRoomZoneConnectionV91> rooms_;
 SourceObjectLoadingLeavesV95 leaves_;
 bool final_busy_{},final_failed_{},final_complete_{};std::string final_error_;
 bool current(std::shared_ptr<world::CanonicalObjectManagerV1>& manager,std::shared_ptr<CanonicalReceiverTransportV1>& transport,std::string& e)const{
  manager=manager_.lock();transport=transport_.lock();
  if(!manager||!transport||!leaves_.owner||!leaves_.current||!leaves_.current(e)){if(e.empty())e="Required live SAME source object-loading authority";return false;}return true;
 }
 bool receiver(const world::CanonicalObjectBorrowV1& object,world::CanonicalClassReceiverV1& out,std::string& e)const{
  std::shared_ptr<world::CanonicalObjectManagerV1> manager;std::shared_ptr<CanonicalReceiverTransportV1> transport;
  if(!current(manager,transport,e)||!object.identity||!object.lease||!object.shared_handle)return false;
  const auto* actual=manager->object(object.shared_handle->key);const world::CanonicalClassReceiverV1* dispatch{};
  if(!actual||actual->identity!=object.identity||actual->lease.get()!=object.lease.get()||actual->lease.owner_before(object.lease)||object.lease.owner_before(actual->lease)||!transport->receiver(*actual,dispatch,e)||!dispatch||dispatch->object.lease.get()!=actual->lease.get()||dispatch->object.lease.owner_before(actual->lease)||actual->lease.owner_before(dispatch->object.lease)){if(e.empty())e="Changed actual loading receiver/dispatch ownership";return false;}
  out=*dispatch;return true;
 }
 bool fields(const world::CanonicalObjectBorrowV1& object,world::CanonicalObjectLoadingFieldsV95& out,std::string& e)const{
  world::CanonicalClassReceiverV1 actual;if(!receiver(object,actual,e)||!actual.source_loading_fields_v95){if(e.empty())e="Required actual ObjectBase loading-field borrower";return false;}
  if(!actual.source_loading_fields_v95(out,e)||!out.receiver||out.receiver.owner_before(object.lease)||object.lease.owner_before(out.receiver)){if(e.empty())e="Loading fields belong to foreign receiver";return false;}return true;
 }
 bool list(std::uint32_t offset,const world::CanonicalObjectBorrowV1* object,std::string& e){
  std::shared_ptr<world::CanonicalObjectManagerV1> manager;std::shared_ptr<CanonicalReceiverTransportV1> transport;if(!current(manager,transport,e))return false;
  std::list<std::uintptr_t>* values{};
  switch(offset){case 0x24:values=&manager->source_rooms24_v104();break;case 0x2c:values=&manager->source_active2c_v102();break;case 0x34:values=&manager->source_pending34_v102();break;case 0x44:values=&manager->source_conditions44_v102();break;default:e="Unsupported actual InitPost list offset";return false;}
  if(object){world::CanonicalClassReceiverV1 actual;if(!receiver(*object,actual,e))return false;values->push_back(object->identity);}else values->clear();e.clear();return true;
 }
public:
 static bool bind(const std::shared_ptr<world::CanonicalObjectManagerV1>& manager,const std::shared_ptr<CanonicalReceiverTransportV1>& transport,
  std::weak_ptr<ModuleRoomZoneConnectionV91> rooms,SourceObjectLoadingLeavesV95 leaves,SourceLoadingInputsV43& inputs,std::string& e){
  if(!manager||!transport||!leaves.owner||!leaves.current||!leaves.test_enable||inputs.manager.get()!=manager.get()||inputs.manager.owner_before(manager)||manager.owner_before(inputs.manager)||inputs.external.stage_body[17]){e="Required sole actual Stage10/17 loading provider";return false;}
  auto& supplied=inputs.object_services;
  if(supplied.load_module||supplied.init_post||supplied.make_handle||supplied.resolve||supplied.test_enable_condition||supplied.type_name||supplied.is_updatable||supplied.room_init_object_list||supplied.membership_fields||supplied.append_list||supplied.clear_list){e="Do not replace or duplicate an existing source dispatcher";return false;}
  auto self=std::make_shared<SourceObjectLoadingV95>();self->manager_=manager;self->transport_=transport;self->rooms_=std::move(rooms);self->leaves_=std::move(leaves);
  using A=world::CanonicalObjectBorrowV1;using H=target_providers::Handle16;
  CanonicalInitPostServicesV38<world::CanonicalObjectManagerV1> services;
  services.make_handle=[self](const A* object,H& out,std::string& e){
   std::shared_ptr<world::CanonicalObjectManagerV1> manager;std::shared_ptr<CanonicalReceiverTransportV1> transport;if(!self->current(manager,transport,e))return false;
   // Original GetHandle33f524 genuine NULL receiver constructor33dd2c.
   if(!object){out={0,UINT32_MAX,0};e.clear();return true;}
   world::CanonicalClassReceiverV1 same;if(!self->receiver(*object,same,e))return false;
   return manager->get_handle(object->shared_handle->key,out,e);
  };
  services.resolve=[self](H& handle,bool asserted,const A*& out,std::string& e){std::shared_ptr<world::CanonicalObjectManagerV1> manager;std::shared_ptr<CanonicalReceiverTransportV1> transport;return self->current(manager,transport,e)&&manager->resolve_handle_v4(handle,asserted,out,self->leaves_.null_handle_assertion,e);};
  services.test_enable_condition=[self](const A& object,bool mark,std::string& e){world::CanonicalClassReceiverV1 same;if(!self->receiver(object,same,e))return false;return self->leaves_.test_enable(object,mark,e);};
  services.type_name=[self](const A& object,std::string& name,std::string& e){world::CanonicalObjectLoadingFieldsV95 fields;if(!self->fields(object,fields,e)||!fields.archetype48){if(e.empty())e="Required SAME archetype CString48/buffer5c";return false;}name=*fields.archetype48;return true;};
  services.is_updatable=[self](const A& object,bool& value,std::string& e){world::CanonicalClassReceiverV1 same;if(!self->receiver(object,same,e)||!same.source_is_updatable_v95){if(e.empty())e="Required actual class virtual38";return false;}return same.source_is_updatable_v95(value,e);};
  services.room_init_object_list=[self](const A& object,std::string& e){auto rooms=self->rooms_.lock();if(!rooms){e="Required SAME generated RoomZone V91 connection";return false;}return rooms->init_object_list(object.identity,e);};
  services.membership_fields=[self](const A& object,std::uintptr_t& a8,std::uint8_t& ac,std::uintptr_t& cc,std::uint8_t& d0,std::string& e){world::CanonicalObjectLoadingFieldsV95 f;if(!self->fields(object,f,e)||!f.condition_a8||!f.tested_ac||!f.condition_cc||!f.tested_d0){if(e.empty())e="Required SAME a8/ac/cc/d0 membership cells";return false;}a8=*f.condition_a8;ac=*f.tested_ac;cc=*f.condition_cc;d0=*f.tested_d0;return true;};
  services.append_list=[self](std::uint32_t offset,const A& object,std::string& e){return self->list(offset,&object,e);};
  services.clear_list=[self](std::uint32_t offset,std::string& e){return self->list(offset,nullptr,e);};
  // SourceV43 installs load_module/init_post itself. Keep these EMPTY.
  inputs.object_services=std::move(services);inputs.external.stage_body[17]=[self](std::string& e){return self->final(e);};inputs.resource_pins.push_back(self);e.clear();return true;
 }
 LifecycleStepV36 final(std::string& e){
  if(final_failed_){e=final_error_;return LifecycleStepV36::failed;}if(final_complete_)return LifecycleStepV36::complete;
  auto fail=[&](){final_failed_=true;if(final_error_.empty())final_error_=e.empty()?"Actual source InitFinal failed":e;e=final_error_;return LifecycleStepV36::failed;};
  if(final_busy_){e="Source InitFinal reentered";return fail();}struct Busy{bool& v;Busy(bool& b):v(b){v=true;}~Busy(){v=false;}}busy(final_busy_);
  std::shared_ptr<world::CanonicalObjectManagerV1> manager;std::shared_ptr<CanonicalReceiverTransportV1> transport;if(!current(manager,transport,e))return fail();
  std::int32_t key{};const world::CanonicalObjectBorrowV1* object{};
  for(bool more=manager->source_ordered_begin_v38(key,object);more;more=manager->source_ordered_next_v38(key,key,object)){
   if(object){auto observed=*object;target_providers::Handle16 handle{};
    if(!manager->get_handle(observed.shared_handle->key,handle,e))return fail();
    const world::CanonicalObjectBorrowV1* resolved{};if(!manager->resolve_handle_v4(handle,false,resolved,leaves_.null_handle_assertion,e))return fail();
    if(resolved){world::CanonicalClassReceiverV1 same;if(!receiver(*resolved,same,e)||!same.is_game_object){if(e.empty())e="Required actual virtual20 GetGameObject";return fail();}
     bool game_object{};if(!same.is_game_object(game_object,e)||final_failed_)return fail();
     // _LoadFinalInit3efce8 GetGameObject: real NULL skips virtual58.
     if(game_object){if(!same.source_init_final_v95){e="Required actual derived virtual58 InitFinal";return fail();}if(!same.source_init_final_v95(e)||final_failed_)return fail();}
    }
   }
   // Original advances the current live map node AFTER each virtual call.
   if(!current(manager,transport,e)||!manager->source_ordered_entry_v38(key,object)){if(e.empty())e="Actual InitFinal current map node erased";return fail();}
  }
  final_complete_=true;e.clear();return LifecycleStepV36::complete;
 }
};
}
