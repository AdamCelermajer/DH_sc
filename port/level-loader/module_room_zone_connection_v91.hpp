#pragma once
#include "source_release_journal_v69.hpp"
#include <module_room_zone_spawn_v3.hpp>
#include <canonical_module_graph_v3.hpp>
namespace dh2::loader {
struct ModuleRoomZoneLeavesV91 {
 std::shared_ptr<void> owner; // SAME independent source.auxiliary authority
 std::function<bool(const std::shared_ptr<world::CanonicalRoomZoneRecordV3>&,world::RoomZoneServicesV3&,std::string&)> lend;
 std::function<bool(const std::shared_ptr<world::CanonicalRoomZoneRecordV3>&,world::RoomZoneRuntimeServicesV104&,std::string&)> lend_runtime_v104;
 std::function<bool(std::uintptr_t,std::uint8_t*&,std::shared_ptr<void>&,std::string&)> module_visited_v104;
 std::function<bool(const world::CanonicalObjectBorrowV1&,bool,std::string&)> test_enable_condition;
 std::function<bool(std::string&)> null_handle_assertion;
 // Actual native RoomZone/Zone/GameObject D0, including membership/resource
 // teardown. Absence is preserved until real cleanup reaches this leaf.
 std::function<bool(world::CanonicalRoomZoneV3&,std::string&)> destroy_source;
 // Enroll SAME base/position/init-final into Parent's existing V69 graph and
 // receiver routing. Called post C1 / transport admission, pre InitPost.
 std::function<bool(const std::shared_ptr<world::CanonicalRoomZoneRecordV3>&,std::string&)> enroll;
};
// Existing RoomZone factory + existing Spawn owner, not a new actor registry.
// External World holds this as sibling; producer and journal callbacks weak.
class ModuleRoomZoneConnectionV91 final:public std::enable_shared_from_this<ModuleRoomZoneConnectionV91> {
 ScopeV67 scope_;std::weak_ptr<CanonicalReceiverTransportV1> transport_;
 std::weak_ptr<SourceReleaseJournalV69> journal_;ModuleRoomZoneLeavesV91 leaves_;
 std::unique_ptr<world::CanonicalRoomZoneFactoryV3> factory_;
 std::unique_ptr<world::ModuleRoomZoneSpawnV3> spawn_;
 ModuleRoomZoneConnectionV91(ScopeV67 s,std::weak_ptr<CanonicalReceiverTransportV1> t,std::weak_ptr<SourceReleaseJournalV69> j,ModuleRoomZoneLeavesV91 l):scope_(std::move(s)),transport_(std::move(t)),journal_(std::move(j)),leaves_(std::move(l)){}
 bool scope(AuxiliaryDeliveryV67& d,std::string& e)const{
  d={scope_.actual_world.lock(),scope_.level.lock(),scope_.objects.lock(),scope_.properties};
  if(!d.actual_world||!d.level||!d.objects||!d.properties||!transport_.lock()||!journal_.lock()){e="Required live SAME generated room scope";return false;}return true;
 }
 static bool resolve(void* c,target_providers::Handle16& h,bool refresh,const world::CanonicalObjectBorrowV1*& out,std::string& e){auto& self=*static_cast<ModuleRoomZoneConnectionV91*>(c);AuxiliaryDeliveryV67 d;if(!self.scope(d,e))return false;return d.objects->resolve_handle_v4(h,refresh,out,self.leaves_.null_handle_assertion,e);}
 static bool condition(void* c,const world::CanonicalObjectBorrowV1& o,bool force,std::string& e){auto& self=*static_cast<ModuleRoomZoneConnectionV91*>(c);AuxiliaryDeliveryV67 d;if(!self.scope(d,e)||!self.leaves_.test_enable_condition){if(e.empty())e="Required actual generated room TestEnableCondition";return false;}return self.leaves_.test_enable_condition(o,force,e);}
 bool admit(const std::shared_ptr<world::CanonicalRoomZoneRecordV3>& actual,std::string& e){
  AuxiliaryDeliveryV67 d;if(!scope(d,e))return false;auto journal=journal_.lock();
  auto record=std::make_shared<AuxiliaryReleaseRecordV67>();record->record=actual;record->class_name="RoomZone";
  std::weak_ptr<world::CanonicalRoomZoneRecordV3> weak_record=actual;std::weak_ptr<ModuleRoomZoneConnectionV91> weak=shared_from_this();
  record->has_constructed_owner=[weak_record]{auto p=weak_record.lock();return p&&bool(p->receiver);};
  record->constructor_state=[weak_record]{auto p=weak_record.lock();if(!p)return CanonicalConstructorStateV89::failed;if(p->constructor_completed_v91)return CanonicalConstructorStateV89::completed;if(p->constructor_failed_v91)return CanonicalConstructorStateV89::failed;return p->constructor_started_v91?CanonicalConstructorStateV89::constructing:CanonicalConstructorStateV89::prepared;};
  record->identity=[weak_record]{auto p=weak_record.lock();return p&&p->receiver?p->receiver->base().identity():0;};
  record->destroy_source=[weak,weak_record](std::string& e){auto self=weak.lock();auto p=weak_record.lock();AuxiliaryDeliveryV67 d;if(!self||!p||!p->receiver||!self->scope(d,e)||!self->leaves_.destroy_source){if(e.empty())e="Required actual RoomZone D0 (not passive C++ expiry)";return false;}return self->leaves_.destroy_source(*p->receiver,e);};
  record->retire_storage_after_source_release_v91=[weak,weak_record](std::string& e){auto self=weak.lock();auto p=weak_record.lock();AuxiliaryDeliveryV67 d;if(!self||!p||!p->receiver||!self->scope(d,e))return false;const auto id=p->receiver->base().identity();self->spawn_->erased_after_source_release_v91(id);self->factory_->erased(id);e.clear();return true;};
  record->borrow_save_header=[weak_record](level::LevelSaveObjectBorrowV2& out,std::string& e){auto p=weak_record.lock();if(!p||!p->constructor_completed_v91||!p->receiver){e="Required SAME generated RoomZone OBJS header";return false;}auto& b=p->receiver->base();out={};out.identity=reinterpret_cast<const void*>(b.identity());out.checkpoint28=b.byte(0x28);out.gametype48=b.string(0x48);out.map_name=b.string(0x30);out.room64=&b.room64();out.disabled81=b.byte(0x81);out.receiver_lease_v86=p;e.clear();return true;};
  record->make_save_connection=[weak_record](NonCharacterRestoreServicesV89 leaves,std::shared_ptr<NonCharacterSaveConnectionV89>& out,std::string& e){auto p=weak_record.lock();if(!p||!p->constructor_completed_v91||!p->receiver){e="Required SAME generated RoomZone OBJS receiver";return false;}auto alias=std::shared_ptr<world::CanonicalRoomZoneV3>(p,p->receiver.get());return make_noncharacter_receiver_save_v89(alias,[](auto& room,auto& fields,std::string& e){return gameobject_save_fields_v89(room.base(),fields,e);},std::move(leaves),out,e);};
  // Native Spawn has no authored XML bytes. Pin the actual independent
  // constructor/service source authority, never fabricate an XML declaration.
  world::CanonicalSourceObjectRequestV1 request{};request.source_lease=scope_.services_owner;
  return journal->admit_auxiliary(d,record,request,e);
 }
public:
 static bool create(ScopeV67 scope,std::weak_ptr<CanonicalReceiverTransportV1> transport,std::weak_ptr<SourceReleaseJournalV69> journal,ModuleRoomZoneLeavesV91 leaves,std::shared_ptr<ModuleRoomZoneConnectionV91>& out,std::string& e){
  auto world=scope.actual_world.lock();auto level=scope.level.lock();auto manager=scope.objects.lock();
  auto same=[](const auto& a,const auto& b){return a&&b&&!a.owner_before(b)&&!b.owner_before(a);};
  if(out||!world||!level||!manager||!scope.properties||!scope.services_owner||!leaves.owner||!same(scope.services_owner,leaves.owner)||same(leaves.owner,world)||same(leaves.owner,level)||same(leaves.owner,manager)||!transport.lock()||!journal.lock()||!leaves.lend||!leaves.lend_runtime_v104||!leaves.module_visited_v104||!leaves.enroll||!leaves.test_enable_condition){e="Required independent actual generated RoomZone source owners/leaves";return false;}
  auto self=std::shared_ptr<ModuleRoomZoneConnectionV91>(new ModuleRoomZoneConnectionV91(std::move(scope),transport,journal,std::move(leaves)));
  std::weak_ptr<ModuleRoomZoneConnectionV91> weak=self;world::CanonicalRoomZoneConstructionV3 construction;construction.world=self->scope_.services_owner;
  construction.services=[weak](const auto& record,world::RoomZoneServicesV3& services,std::string& e){auto self=weak.lock();AuxiliaryDeliveryV67 d;return self&&self->scope(d,e)&&self->leaves_.lend(record,services,e);};
  construction.admit_before_c1_v91=[weak](const auto& record,const auto&,std::string& e){auto self=weak.lock();return self&&self->admit(record,e);};
  construction.admit_after_c1_v91=[weak](const auto& record,const auto& receiver,std::string& e){auto self=weak.lock();AuxiliaryDeliveryV67 d;if(!self||!self->scope(d,e))return false;auto transport=self->transport_.lock();if(!transport->admit_constructed_source_v91(receiver,e))return false;
   world::RoomZoneRuntimeServicesV104 runtime;if(!self->leaves_.lend_runtime_v104(record,runtime,e))return false;
   if(runtime.module_visited){e="Generated RoomZone runtime must use the SAME actual ModuleGraph visit borrower";return false;}
   runtime.module_visited=self->leaves_.module_visited_v104;
   if(!record->receiver->bind_runtime_v104(std::move(runtime),e))return false;
   return self->leaves_.enroll(record,e);};
  self->factory_=std::make_unique<world::CanonicalRoomZoneFactoryV3>(std::move(construction));
  world::CanonicalSpawnServicesV1 source{};source.context=self.get();source.resolve=resolve;source.test_enable_condition=condition;
  self->spawn_=std::make_unique<world::ModuleRoomZoneSpawnV3>(*manager,*self->scope_.properties,*self->factory_,source);
  out=std::move(self);e.clear();return true;
 }
 bool spawn(const std::string& name,target_providers::Handle16& out,std::uint32_t& type,std::string& e){AuxiliaryDeliveryV67 d;if(!scope(d,e))return false;return spawn_->spawn(name,out,type,e);}
 bool init(target_providers::Handle16& h,const std::array<float,6>& box,std::uintptr_t module,std::string& e){AuxiliaryDeliveryV67 d;if(!scope(d,e))return false;return spawn_->init(h,box,module,e);}
 bool borrow_room(std::uintptr_t id,std::shared_ptr<world::CanonicalRoomZoneV3>& out,std::string& e){AuxiliaryDeliveryV67 d;if(!scope(d,e))return false;auto record=factory_->record_v91(id);if(!record||!record->constructor_completed_v91||!record->receiver){e="Required SAME completed actual RoomZone C1 record";return false;}out=std::shared_ptr<world::CanonicalRoomZoneV3>(record,record->receiver.get());e.clear();return true;}
 bool init_final(std::uintptr_t id,bool& eligible,std::string& e){std::shared_ptr<world::CanonicalRoomZoneV3> room;if(!borrow_room(id,room,e))return false;return room->init_final(eligible,e);}
 // Original RoomZone.InitObjectList396c44 source orchestration. StageV38
 // owns append24 before this call; Main owns AddInitialObject body, not a copy.
 bool init_object_list(std::uintptr_t id,std::string& e){
  AuxiliaryDeliveryV67 d;if(!scope(d,e))return false;std::shared_ptr<world::CanonicalRoomZoneV3> room;if(!borrow_room(id,room,e))return false;
  auto initialized=room->source_byte(0x390);if(!initialized){e="Required actual RoomZone390 field";return false;}if(*initialized){e.clear();return true;}
  std::int32_t key{};const world::CanonicalObjectBorrowV1* entry{};bool more=d.objects->source_ordered_begin_v38(key,entry);
  while(more){std::uintptr_t game_object{};
   // Capture the scanned receiver before callbacks can alter the manager.
   // Stage10's outer key names this RoomZone, not its failing occupant.
   const auto scanned_identity=entry?entry->identity:0;
   const std::string scanned_class=entry&&entry->class_name20&&*entry->class_name20?*entry->class_name20:"<unavailable>";
   const auto scanned_type=entry&&entry->type_f4?std::to_string(*entry->type_f4):"<unavailable>";
   const auto scan_failure=[&](const char* operation){
    e+=" [RoomZone.InitObjectList scanned_key="+std::to_string(key)+
     " scanned_identity="+std::to_string(scanned_identity)+" scanned_class="+scanned_class+
     " scanned_type="+scanned_type+" game_object_identity="+std::to_string(game_object)+
     " room_identity="+std::to_string(id)+" scan_operation="+operation+"]";
    return false;
   };
   if(entry){auto pin=entry->lease;if(!entry->shared_handle){e="Required actual RoomZone InitObjectList GetHandle";return scan_failure("ObjectBase.GetHandle");}auto handle=*entry->shared_handle;const world::CanonicalObjectBorrowV1* object{};
    if(!d.objects->resolve_handle_v4(handle,false,object,leaves_.null_handle_assertion,e))return scan_failure("ObjectHandle.GetObject(false)");
    if(object){auto transport=transport_.lock();const world::CanonicalClassReceiverV1* receiver{};
     if(!transport||!transport->receiver(*object,receiver,e)||!receiver||!receiver->is_game_object){if(e.empty())e="Required actual GetHandle GameObject virtual20 conversion";return scan_failure("receiver transport");}
     bool converted{};auto body=receiver->is_game_object;auto receiver_pin=receiver->object.lease;if(!body(converted,e))return scan_failure("virtual IsGameObject");if(converted)game_object=object->identity;
    }
   }
   bool accepted{};if(!room->source_add_initial_object_v104(game_object,accepted,e))return scan_failure("RoomZone.AddInitialObject");
   // Native ignores semantic accepted/false, then advances the same map node.
   if(!d.objects->source_ordered_entry_v38(key,entry)){e="Original InitObjectList current map node erased during callback";return scan_failure("current manager node");}
   more=d.objects->source_ordered_next_v38(key,key,entry);
  }
  *initialized=1;e.clear();return true;
 }
 bool update(std::uintptr_t id,std::string& e){AuxiliaryDeliveryV67 d;if(!scope(d,e))return false;auto record=factory_->record_v91(id);if(!record||!record->receiver){e="Required SAME RoomZone Update receiver";return false;}return record->receiver->source_update_v104(e);}
 bool remove_object(std::uintptr_t room,std::uintptr_t object,std::string& e){AuxiliaryDeliveryV67 d;if(!scope(d,e))return false;auto record=factory_->record_v91(room);if(!record||!record->receiver){e="Required SAME RoomZone RemoveObject receiver";return false;}record->receiver->source_remove_object_v104(object);e.clear();return true;}
 bool restore_module_visited(target_providers::Handle16& h,std::uint8_t& value,std::string& e){AuxiliaryDeliveryV67 d;if(!scope(d,e))return false;const world::CanonicalObjectBorrowV1* object{};if(!resolve(this,h,false,object,e))return false;if(!object||!object->type_f4||*object->type_f4!=11)return true;auto record=factory_->record_v91(object->identity);if(!record||!record->receiver){e="Required actual Module400 RoomZone SetVisited receiver";return false;}return record->receiver->source_set_visited_byte_v104(value,e);}
};
}
