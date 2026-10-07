#include "catalog_generic_v70.hpp"
#include <cstring>
#include <exception>
#include <tuple>
#include <utility>
#include <type_traits>
namespace dh2::loader {
struct GenericStateV70 {
 ScopeV67 scope;std::shared_ptr<const CatalogGenericServicesV70> leaves;
 const world::CanonicalSourceObjectRequestV1* source{};
 bool busy{},failed{};
 std::shared_ptr<GenericReleaseRecordV70> pending;
 std::function<void()> pending_erase;
 std::map<std::uintptr_t,std::function<void()>> erase;
};
namespace {
template<class A,class B>bool same_owner(const std::shared_ptr<A>& a,const std::shared_ptr<B>& b){return a&&b&&!a.owner_before(b)&&!b.owner_before(a);}
template<class A>bool independent(const ScopeV67& s,const std::shared_ptr<A>& p){return !p||(!same_owner(p,s.actual_world.lock())&&!same_owner(p,s.level.lock())&&!same_owner(p,s.objects.lock()));}
bool borrow(const GenericStateV70& s,GenericDeliveryV70& b,std::string& e,bool release=false){
 if(s.failed&&!release){e="Generic constructor candidate failed; discard required";return false;}
 b={s.scope.actual_world.lock(),s.scope.level.lock(),s.scope.objects.lock(),s.scope.properties};
 if(!b.actual_world||!b.level||!b.objects||!b.properties){e="Generic actual World/Level/manager lifetime expired";return false;}
 if(!s.leaves->validate_current(b,e))return false;
 if(s.failed&&!release){e="Generic constructor candidate failed during scope delivery";return false;}return true;
}
world::CanonicalRoomZoneV3* body(const std::shared_ptr<world::CanonicalRoomZoneRecordV3>& r){return r?r->receiver.get():nullptr;}
world::CanonicalTriggerTrapV37* body(const std::shared_ptr<CanonicalTriggerTrapRecordV37>& r){return r?r->owner.get():nullptr;}
template<class Record>bool receiver(std::weak_ptr<Record> weak,std::shared_ptr<Record>& r,std::string& e){r=weak.lock();if(!body(r)){e="Required SAME produced generic C1/runtime";return false;}return true;}
template<class Record,class... Args>
std::function<bool(Args...)> init_member(const std::shared_ptr<GenericStateV70>& s,std::weak_ptr<Record> w,
 std::function<bool(Args...)> world::GameObjectInitializationServicesV1::* field,const char* name){
 return [s,w,field,name](Args... args){auto& e=std::get<sizeof...(Args)-1>(std::forward_as_tuple(args...));
  GenericDeliveryV70 b;std::shared_ptr<Record> r;if(!borrow(*s,b,e)||!receiver(w,r,e))return false;
  world::GameObjectInitializationServicesV1 current;
  if(!s->leaves->lend_initialization(b,body(r)->base(),current,e))return false;
  if(!same_owner(current.owner,s->scope.services_owner)){e="Generic initialization replaced independent authority";return false;}
  GenericDeliveryV70 after;if(!borrow(*s,after,e))return false;
  if(!(current.*field)){e=std::string("Required genuine generic initialization leaf: ")+name;return false;}
  if(!(current.*field)(std::forward<Args>(args)...))return false;return borrow(*s,after,e);
 };
}
template<class Record>void initialization(const std::shared_ptr<GenericStateV70>& s,std::weak_ptr<Record> w,world::GameObjectInitializationServicesV1& out){
 out.owner=s->scope.services_owner;out.difficulty_names=s->leaves->difficulty_names.get();out.sound_names=s->leaves->sound_names.get();
 using S=world::GameObjectInitializationServicesV1;
#define INIT_LEAF(name) out.name=init_member(s,w,&S::name,#name)
 INIT_LEAF(condition_init);INIT_LEAF(check_spawn_probability);INIT_LEAF(device_high_performance);INIT_LEAF(load_visual);INIT_LEAF(visual_sync);
 INIT_LEAF(set_visible);INIT_LEAF(init_pf_object);INIT_LEAF(light_set_id);INIT_LEAF(visual_set_light_set);INIT_LEAF(visual_root);INIT_LEAF(node_from_name);INIT_LEAF(update_pf_object);
#undef INIT_LEAF
 out.set_position=[s,w](const float* p,bool destination,std::string& e){GenericDeliveryV70 b;std::shared_ptr<Record> r;
  if(!borrow(*s,b,e)||!receiver(w,r,e))return false;world::GameObjectSetPositionServicesV2 current;
  if(!s->leaves->lend_position(b,body(r)->base(),current,e))return false;
  if(!same_owner(current.owner,s->scope.services_owner)){e="Generic SetPosition replaced independent authority";return false;}
  GenericDeliveryV70 after;if(!borrow(*s,after,e))return false;
  if(!world::game_object_set_position_v2(body(r)->base(),p,destination,current,e))return false;return borrow(*s,after,e);
 };
}
world::GameObjectSetPositionServicesV2 room_position(const std::shared_ptr<GenericStateV70>& s,std::weak_ptr<world::CanonicalRoomZoneRecordV3> w){
 world::GameObjectSetPositionServicesV2 out;out.owner=s->scope.services_owner;
 // V3 InitBounds/transport SetPosition still executes the same loader kernel.
 // Only its three genuine engine leaves are borrowed from the live graph.
 auto obtain=[s,w](world::GameObjectSetPositionServicesV2& current,GenericDeliveryV70& b,std::string& e){
  std::shared_ptr<world::CanonicalRoomZoneRecordV3> r;if(!borrow(*s,b,e)||!receiver(w,r,e))return false;
  if(!s->leaves->lend_position(b,body(r)->base(),current,e))return false;
  if(!same_owner(current.owner,s->scope.services_owner)){e="RoomZone position replaced independent authority";return false;}
  GenericDeliveryV70 after;return borrow(*s,after,e);
 };
 out.attached_position=[s,obtain](std::uintptr_t id,float*& value,std::string& e){world::GameObjectSetPositionServicesV2 current;GenericDeliveryV70 b;
  if(!obtain(current,b,e))return false;if(!current.attached_position){e="Required RoomZone attached position leaf";return false;}
  if(!current.attached_position(id,value,e))return false;return borrow(*s,b,e);
 };
 out.physical_position=[s,obtain](std::uintptr_t id,float x,float y,std::string& e){world::GameObjectSetPositionServicesV2 current;GenericDeliveryV70 b;
  if(!obtain(current,b,e))return false;if(!current.physical_position){e="Required RoomZone physical position leaf";return false;}
  if(!current.physical_position(id,x,y,e))return false;return borrow(*s,b,e);
 };
 out.visual_sync=[s,obtain](std::uintptr_t id,std::string& e){world::GameObjectSetPositionServicesV2 current;GenericDeliveryV70 b;
  if(!obtain(current,b,e))return false;if(!current.visual_sync){e="Required RoomZone visual sync leaf";return false;}
  if(!current.visual_sync(id,e))return false;return borrow(*s,b,e);
 };return out;
}
std::function<bool(world::CanonicalTriggerTrapV37&,std::string&)> trap_member(const std::shared_ptr<GenericStateV70>& s,
 std::weak_ptr<CanonicalTriggerTrapRecordV37> w,std::function<bool(world::CanonicalTriggerTrapV37&,std::string&)> world::TriggerTrapServicesV37::* field,const char* name,bool release=false){
 return [s,w,field,name,release](world::CanonicalTriggerTrapV37& supplied,std::string& e){GenericDeliveryV70 b;std::shared_ptr<CanonicalTriggerTrapRecordV37> r;
  if(!borrow(*s,b,e,release)||!receiver(w,r,e))return false;if(body(r)!=&supplied){e="TriggerTrap method receiver changed";return false;}
  world::TriggerTrapServicesV37 current;if(!s->leaves->lend_trigger_trap(b,supplied,current,e))return false;
  if(!same_owner(current.owner,s->scope.services_owner)){e="TriggerTrap method replaced independent authority";return false;}
  GenericDeliveryV70 after;if(!borrow(*s,after,e,release))return false;
  if(!(current.*field)){e=std::string("Required genuine TriggerTrap source method: ")+name;return false;}
  if(!(current.*field)(supplied,e))return false;return borrow(*s,after,e,release);
 };
}
template<class Record>bool admit(const std::shared_ptr<GenericStateV70>& s,const std::shared_ptr<Record>& actual,const char* name,std::string& e){
 if(!s->source){e="Generic release admission requires the current source declaration";return false;}
 GenericDeliveryV70 b;if(!borrow(*s,b,e))return false;
 auto journal=std::make_shared<GenericReleaseRecordV70>();journal->record=actual;journal->source_lease=s->source->source_lease;journal->class_name=name;std::weak_ptr<Record> w=actual;
 journal->identity=[w](){auto r=w.lock();return body(r)?body(r)->base().identity():0;};
 auto release_done=std::make_shared<bool>(false);
 journal->destroy_source=[s,w,release_done](std::string& e){
  if(*release_done){e.clear();return true;}
  // Failure of construction is sticky for forward loading, but a retained
  // native release prefix must remain drainable. Release does not call borrow's
  // forward-load failure gate; it still validates every actual owner.
  GenericDeliveryV70 b{s->scope.actual_world.lock(),s->scope.level.lock(),s->scope.objects.lock(),s->scope.properties};
  std::shared_ptr<Record> r;if(!b.actual_world||!b.level||!b.objects||!b.properties||!receiver(w,r,e)){e="Required live generic release receiver/scope";return false;}
  if(!s->leaves->validate_current(b,e))return false;
  bool done=false;
  if constexpr(std::is_same_v<Record,world::CanonicalRoomZoneRecordV3>)done=s->leaves->destroy_room_zone(b,*body(r),e);
  else done=body(r)->destroy(e);
  if(!done)return false;*release_done=true;return s->leaves->validate_current(b,e);
 };
 if(!s->leaves->retain_release_record(b,journal,*s->source,e))return false;
 GenericDeliveryV70 after;if(!borrow(*s,after,e))return false;
 s->pending=std::move(journal);
 s->pending_erase=[w](){if(auto r=w.lock()){
  if constexpr(std::is_same_v<Record,world::CanonicalRoomZoneRecordV3>)r->receiver.reset();else r->owner.reset();
 }};return true;
}
}
bool CatalogGenericV70::create(ScopeV67 scope,std::shared_ptr<const CatalogGenericServicesV70> leaves,
 std::shared_ptr<CatalogGenericV70>& output,PartV67& part,std::string& e){
 if(!scope.services_owner||!leaves||!same_owner(leaves->owner,scope.services_owner)||!leaves->validate_current||
 !leaves->lend_initialization||!leaves->lend_position||!leaves->lend_trigger_trap||!leaves->destroy_room_zone||
 !leaves->retain_release_record||!leaves->release_completed||!leaves->difficulty_names||!leaves->sound_names){e="Generic production catalog requires actual typed engine/Arrays/release services";return false;}
 if(!independent(scope,leaves->owner)||!independent(scope,leaves->difficulty_names)||!independent(scope,leaves->sound_names)){e="Generic service authority/Arrays cannot retain containing owners";return false;}
 try{
  auto s=std::make_shared<GenericStateV70>();s->scope=scope;s->leaves=leaves;GenericDeliveryV70 b;if(!borrow(*s,b,e))return false;
  PartV67 selected;for(const auto& entry:world::canonical_factories_v1())if(entry.name&&
   ((!std::strcmp(entry.name,"RoomZone")&&entry.original_address==0x340f74)||(!std::strcmp(entry.name,"TriggerTrap")&&entry.original_address==0x340e7c)))selected.entries.push_back(entry);
  if(selected.entries.size()!=2){e="Required exact original RoomZone/TriggerTrap source entries";return false;}
  auto catalog=std::shared_ptr<CatalogGenericV70>(new CatalogGenericV70);catalog->state_=s;
  world::CanonicalRoomZoneConstructionV3 rooms;rooms.world=scope.services_owner;
  rooms.services=[s](const auto& actual,world::RoomZoneServicesV3& services,std::string& e){
   std::weak_ptr<world::CanonicalRoomZoneRecordV3> w=actual;initialization(s,w,services.game_object);services.position=room_position(s,w);return admit(s,actual,"RoomZone",e);
  };catalog->rooms_=std::make_unique<world::CanonicalRoomZoneFactoryV3>(std::move(rooms));
  CanonicalAuxiliaryInputsV16 traps;traps.world=scope.services_owner;
  traps.trigger_trap=[s](const auto&,const auto& actual,auto& init,world::TriggerTrapServicesV37& services,std::string& e){
   std::weak_ptr<CanonicalTriggerTrapRecordV37> w=actual;initialization(s,w,init);services.owner=s->scope.services_owner;using S=world::TriggerTrapServicesV37;
   services.whole_init_post=trap_member(s,w,&S::whole_init_post,"InitPost39dee0");services.whole_init_final=trap_member(s,w,&S::whole_init_final,"InitFinal38cd48");
   services.whole_update=trap_member(s,w,&S::whole_update,"Update39eedc");services.whole_destroy=trap_member(s,w,&S::whole_destroy,"Dtor39e480",true);return admit(s,actual,"TriggerTrap",e);
  };catalog->traps_=std::make_unique<CanonicalAuxiliaryFamiliesV16>(std::move(traps));
  selected.owner=catalog;selected.construct=[catalog](const auto& entry,const auto& source,auto& out,std::string& e){return catalog->construct(entry,source,out,e);};
  output=std::move(catalog);part=std::move(selected);e.clear();return true;
 }catch(const std::exception& ex){e=std::string("Generic catalog allocation failed: ")+ex.what();return false;}
}
bool CatalogGenericV70::construct(const world::CanonicalFactoryEntryV1& entry,const world::CanonicalSourceObjectRequestV1& source,
 world::CanonicalClassReceiverV1& output,std::string& e){
 auto& s=*state_;if(s.busy){s.failed=true;e="Generic construction cannot reenter";return false;}
 GenericDeliveryV70 b;if(!borrow(s,b,e))return false;
 if(!source.source_lease||!source.element||!source.attribute){e="Generic construction requires actual source XML/lease";return false;}
 const bool room=entry.name&&!std::strcmp(entry.name,"RoomZone")&&entry.original_address==0x340f74;
 const bool trap=entry.name&&!std::strcmp(entry.name,"TriggerTrap")&&entry.original_address==0x340e7c;
 if(!room&&!trap){e="Generic part only admits exact RoomZone/TriggerTrap original factories";return false;}
 s.busy=true;s.source=&source;
 struct Finish {GenericStateV70& s;~Finish(){s.source=nullptr;s.pending.reset();s.pending_erase={};s.busy=false;}} finish{s};
 try{
  world::CanonicalClassReceiverV1 result;bool ok=room?rooms_->construct(entry,result,e):traps_->construct(entry,source,result,e);
  if(!ok||s.failed){s.failed=true;if(e.empty())e="Generic source C1 failed; discard required";return false;}
  if(!s.pending||!s.pending->identity||s.pending->identity()!=result.object.identity||!s.pending_erase){s.failed=true;e="Generic C1 is not the admitted native release journal receiver";return false;}
  if(!borrow(s,b,e)){s.failed=true;return false;}
  // RoomZone V3 factory predates the request API; retain the exact declaration
  // on the same transport, in addition to its pre-C1 journal lease.
  result.source_lease=source.source_lease;s.erase.emplace(result.object.identity,std::move(s.pending_erase));
  output=std::move(result);e.clear();return true;
 }catch(const std::exception& ex){s.failed=true;e=std::string("Generic source C1 failed: ")+ex.what();return false;}
}
bool CatalogGenericV70::erase_after_source_release(std::uintptr_t identity,std::string& e){
 auto& s=*state_;if(s.busy){e="Generic release cannot erase during construction";return false;}
 auto found=s.erase.find(identity);if(found==s.erase.end()){e="Generic release identity is not retained";return false;}
 s.busy=true;struct Releasing {GenericStateV70& s;~Releasing(){s.busy=false;}} releasing{s};
 GenericDeliveryV70 b{s.scope.actual_world.lock(),s.scope.level.lock(),s.scope.objects.lock(),s.scope.properties};
 if(!b.actual_world||!b.level||!b.objects||!b.properties){e="Generic release actual scope expired";return false;}
 if(!s.leaves->validate_current(b,e)||!s.leaves->release_completed(b,identity,e))return false;
 if(!s.leaves->validate_current(b,e))return false;
 found->second();rooms_->erased(identity);s.erase.erase(found);e.clear();return true;
}
bool make_generic_catalog_part_v70(ScopeV67 scope,CatalogGenericServicesV70 leaves,PartV67& part,std::string& e){
 auto owner=std::make_shared<const CatalogGenericServicesV70>(std::move(leaves));std::shared_ptr<CatalogGenericV70> result;
 return CatalogGenericV70::create(std::move(scope),std::move(owner),result,part,e);
}
}

