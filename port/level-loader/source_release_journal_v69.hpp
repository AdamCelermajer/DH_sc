#pragma once
#include <production_noncharacter_composition_v67.hpp>
#include <canonical_receiver_transport_v1.hpp>
#include <canonical_object_lifecycle_v1.hpp>
#include <algorithm>
#include <exception>
#include <type_traits>
#include <vector>
namespace dh2::loader {
struct SourceConstructorPrefixBorrowV89 {
 std::shared_ptr<void> record;std::shared_ptr<const void> declaration;const char* class_name{};
 CanonicalConstructorStateV89 constructor_state{};
 std::shared_ptr<AuxiliaryReleaseRecordV67> auxiliary_v92;
};
struct SourceReleaseJournalLeavesV69 {
 std::shared_ptr<void> owner; // independent genuine release primitives
 // Failed started C1 has no completed unique owner, but may retain genuine
 // resource prefixes. Qualified Main unwind/alias absence must name THIS
 // record; never call an invented class D0 for identity0.
 std::function<bool(const SourceConstructorPrefixBorrowV89&,std::string&)> unwind_failed_constructor_prefix;
 // Actual draw/native/observer/condition aliases exhausted after real D0.
 // No fake quiescence receipt; absent leaf fails at reached retirement only.
 std::function<bool(std::uintptr_t,std::string&)> require_native_aliases_unpublished;
 // V21 graph.release alone does NOT include whole original class D0. Main
 // lends genuine derived/base/ConditionData/Lua/network continuation here.
 std::function<bool(const std::shared_ptr<world::CanonicalOpenableGraphV21>&,std::string&)> openable_d0;
};
// One existing-source release journal, NOT an ObjectManager/type registry/pool.
// Retain it as sibling of independent provider authority in the external release
// composition. Never make provider authority own this journal: actual records
// themselves retain that authority through their real class services.
class SourceReleaseJournalV69 final:public std::enable_shared_from_this<SourceReleaseJournalV69> {
 enum class Kind {auxiliary,light,openable,destructible};
 struct Entry {
  Kind kind;std::shared_ptr<const void> source;
  std::shared_ptr<AuxiliaryReleaseRecordV67> auxiliary;
  std::shared_ptr<CanonicalLightPointRecordV53> light;
  std::shared_ptr<world::CanonicalOpenableGraphV21> openable;
  std::shared_ptr<world::CanonicalDestructibleContainerV16> destructible;
  world::CanonicalObjectBorrowV1 orphan;
  std::shared_ptr<NonCharacterSaveConnectionV89> save_connection;
  bool busy{},attempted{},destroyed{};std::string failure;
  std::uintptr_t identity()const{
   if(auxiliary)return auxiliary->identity?auxiliary->identity():0;
   if(light)return light->owner?light->owner->identity():0;
   if(openable)return openable->receiver().base().identity();
   return destructible?destructible->base().identity():0;
  }
 };
 std::weak_ptr<void> actual_world_,services_owner_;
 std::weak_ptr<CanonicalLevelContextV1> level_;
 std::weak_ptr<world::CanonicalObjectManagerV1> manager_;
 std::weak_ptr<CanonicalReceiverTransportV1> transport_;
 std::weak_ptr<CatalogAuxiliaryV67> auxiliary_;
 std::weak_ptr<CatalogEnvironmentV67> environment_;
 std::weak_ptr<ContainerCatalogOwnerV67> containers_;
 SourceReleaseJournalLeavesV69 leaves_;
 std::vector<std::unique_ptr<Entry>> entries_;bool release_started_{},body_running_{},retirement_running_{};
 template<class A,class B>static bool same(const std::shared_ptr<A>& a,const std::shared_ptr<B>& b){return a&&b&&!a.owner_before(b)&&!b.owner_before(a);}
 template<class Delivery>bool delivery(const Delivery& d,std::string& e)const{
  auto world=actual_world_.lock();auto level=level_.lock();auto manager=manager_.lock();auto authority=services_owner_.lock();
  if(!same(world,d.actual_world)||world.get()!=d.actual_world.get()||!same(level,d.level)||level.get()!=d.level.get()||!same(manager,d.objects)||manager.get()!=d.objects.get()||!authority){e="Required SAME live source release World/Level/manager/authority";return false;}
  if(same(authority,world)||same(authority,level)||same(authority,manager)){e="Source release authority must remain independent of containing scopes";return false;}return true;
 }
 bool scope(std::shared_ptr<world::CanonicalObjectManagerV1>& manager,std::shared_ptr<CanonicalReceiverTransportV1>& transport,std::string& e)const{
  manager=manager_.lock();transport=transport_.lock();if(!actual_world_.lock()||!level_.lock()||!services_owner_.lock()||!manager||!transport){e="Required SAME live manager/transport release scope";return false;}return true;
 }
 Entry* find(std::uintptr_t id){if(!id)return nullptr;for(auto& entry:entries_)if(entry->identity()==id)return entry.get();return nullptr;}
 bool manager_absent(const world::CanonicalObjectManagerV1& m,std::uintptr_t id,std::string& e)const{
  std::int32_t key{};const world::CanonicalObjectBorrowV1* actor{};bool more=m.source_ordered_begin_v38(key,actor);
  while(more){if(actor&&actor->identity==id){e="Actual manager still publishes source receiver";return false;}more=m.source_ordered_next_v38(key,key,actor);}return true;
 }
 bool completed(Entry& entry,std::string& e)const{
  if(entry.kind==Kind::light){if(!entry.light||!entry.light->owner||entry.light->owner->source_teardown_state()!=world::LightTeardownStateV67::ObjectBaseCompleted){e="Actual LightPoint source D0 incomplete";return false;}return true;}
  if(!entry.destroyed){e="Genuine class destroy_source/catalog teardown not completed; preserve C1 journal";return false;}return true;
 }
 SourceReleaseJournalV69(const ScopeV67& scope,std::weak_ptr<CanonicalReceiverTransportV1> transport,SourceReleaseJournalLeavesV69 leaves):actual_world_(scope.actual_world),services_owner_(scope.services_owner),level_(scope.level),manager_(scope.objects),transport_(std::move(transport)),leaves_(std::move(leaves)){}
public:
 static bool create(const ScopeV67& scope,std::weak_ptr<CanonicalReceiverTransportV1> transport,SourceReleaseJournalLeavesV69 leaves,std::shared_ptr<SourceReleaseJournalV69>& out,std::string& e){
  auto world=scope.actual_world.lock();auto level=scope.level.lock();auto manager=scope.objects.lock();
  if(out||!world||!level||!manager||!transport.lock()||!scope.services_owner||!leaves.owner||same(scope.services_owner,world)||same(scope.services_owner,level)||same(scope.services_owner,manager)||same(leaves.owner,world)||same(leaves.owner,level)||same(leaves.owner,manager)){e="Required new independent release journal over actual live source scopes";return false;}
  out=std::shared_ptr<SourceReleaseJournalV69>(new SourceReleaseJournalV69(scope,std::move(transport),std::move(leaves)));e.clear();return true;
 }
 // Attach actual catalog owners returned by compose_campaign_noncharacter_v69
 // BEFORE XML constructors. Weak fields cannot retain enclosing catalog/World.
 bool attach_catalog_owners(const ProductionNonCharacterOwnersV67& owners,std::string& e){
  if(!auxiliary_.expired()||!environment_.expired()||!containers_.expired()||!entries_.empty()||release_started_){e="Attach SAME catalog release owners once before C1 admission";return false;}
  if(!owners.auxiliary||!owners.containers){e="Required actual auxiliary/container catalog release owners";return false;}
  auxiliary_=owners.auxiliary;environment_=owners.environment;containers_=owners.containers;e.clear();return true;
 }
 bool admit_auxiliary(const AuxiliaryDeliveryV67& d,const std::shared_ptr<AuxiliaryReleaseRecordV67>& record,const world::CanonicalSourceObjectRequestV1& source,std::string& e){
  if(!delivery(d,e))return false;if(release_started_||!record||!record->record||!record->class_name||!record->has_constructed_owner||!record->constructor_state||!record->identity||!record->destroy_source||!source.source_lease){e="Required actual pre-C1 auxiliary/runtime/declaration release record";return false;}
  for(auto& entry:entries_)if(entry->auxiliary&&same(entry->auxiliary,record)){e="Actual auxiliary prefix already admitted";return false;}
  auto entry=std::make_unique<Entry>();entry->kind=Kind::auxiliary;entry->source=source.source_lease;entry->auxiliary=record;entries_.push_back(std::move(entry));e.clear();return true;
 }
 bool admit_light(const LightDeliveryV67& d,const std::shared_ptr<CanonicalLightPointRecordV53>& record,const world::CanonicalSourceObjectRequestV1& source,std::string& e){
  if(!delivery(d,e))return false;if(release_started_||!record||!source.source_lease){e="Required actual pre-C1 light/declaration release record";return false;}
  for(auto& entry:entries_)if(entry->light&&same(entry->light,record)){e="Actual light prefix already admitted";return false;}
  auto entry=std::make_unique<Entry>();entry->kind=Kind::light;entry->source=source.source_lease;entry->light=record;entries_.push_back(std::move(entry));e.clear();return true;
 }
 template<class T>bool admit_container(const ScopeV67& scope,const world::CanonicalSourceObjectRequestV1& source,const std::shared_ptr<T>& actual,std::string& e){
  AuxiliaryDeliveryV67 d{scope.actual_world.lock(),scope.level.lock(),scope.objects.lock(),scope.properties};if(!delivery(d,e))return false;
  if(release_started_||!actual||!source.source_lease){e="Required SAME post-C1/pre-InitPost container admission";return false;}
  auto entry=std::make_unique<Entry>();entry->source=source.source_lease;
  if constexpr(std::is_same_v<T,world::CanonicalOpenableGraphV21>){entry->kind=Kind::openable;entry->openable=actual;}
  else {static_assert(std::is_same_v<T,world::CanonicalDestructibleContainerV16>);entry->kind=Kind::destructible;entry->destructible=actual;}
  if(!entry->identity()||find(entry->identity())){e="Actual container C1 identity absent/already admitted";return false;}entries_.push_back(std::move(entry));e.clear();return true;
 }
 bool openable_native_body(const std::shared_ptr<world::CanonicalOpenableGraphV21>& graph,std::string& e){
  auto* entry=graph?find(graph->receiver().base().identity()):nullptr;if(!entry||entry->kind!=Kind::openable||!same(entry->openable,graph)||entry->openable.get()!=graph.get()){e="Required SAME admitted Openable typed graph";return false;}
  if(!leaves_.openable_d0){e="Required genuine Main Openable class D0 including base/ConditionData/Lua/network; graph.release alone is incomplete";return false;}return leaves_.openable_d0(graph,e);
 }
 // Main performs actual owning-thread barrier acquisition. This invokes SAME
 // environment's real all-record prepare pass, never fabricated quiescence.
 bool prepare_lights(std::shared_ptr<world::LightQuiescenceLeaseV67> lease,std::string& e){
  auto owner=environment_.lock();if(!owner){e="Required SAME LightPoint environment and genuine teardown barrier";return false;}release_started_=true;return owner->prepare_source_release(std::move(lease),e);
 }
 // Called by genuine ObjectManager class D0 dispatch while its map cell lives.
 // Unknown identities (Character/Module/etc) stay Main-owned and fail routing.
 bool destroy_source(std::uintptr_t id,std::shared_ptr<world::LightQuiescenceLeaseV67> light_lease,std::string& e){
  if(body_running_||retirement_running_){e="Source class destruction cannot reenter native journal delivery";return false;}
  auto* entry=find(id);if(!entry){e="No admitted actual non-Character C1 release record";return false;}
  if(entry->busy){e="Actual source D0 reentered";return false;}if(entry->destroyed){e.clear();return true;}if(entry->attempted){e=entry->failure.empty()?"Reached class D0 prefix cannot replay":entry->failure;return false;}
  std::shared_ptr<world::CanonicalObjectManagerV1> manager;std::shared_ptr<CanonicalReceiverTransportV1> transport;if(!scope(manager,transport,e))return false;
  body_running_=true;entry->busy=true;entry->attempted=true;struct Guard{bool& busy;bool& running;~Guard(){busy=false;running=false;}}guard{entry->busy,body_running_};bool result=false;
  try{
   if(entry->kind==Kind::auxiliary)result=entry->auxiliary->destroy_source(e);
   else if(entry->kind==Kind::light){auto owner=environment_.lock();if(!owner)e="Required attached actual light catalog";else result=owner->execute_source_release(id,std::move(light_lease),e);}
   else {auto owner=containers_.lock();if(!owner)e="Required attached actual container catalog";else result=owner->teardown(id,e);}
  }catch(const std::exception& ex){e=ex.what();}catch(...){e="Actual source D0 threw; release record retained";}
  if(!result){entry->failure=e.empty()?"Required genuine actual class D0":e;e=entry->failure;return false;}entry->destroyed=true;e.clear();return true;
 }
 template<class Delivery>bool release_completed(const Delivery& d,std::uintptr_t id,std::string& e){
  if(!delivery(d,e))return false;auto* entry=find(id);if(!entry||entry->busy||!completed(*entry,e)){if(e.empty())e="Required completed actual class release record";return false;}
  std::shared_ptr<world::CanonicalObjectManagerV1> manager;std::shared_ptr<CanonicalReceiverTransportV1> transport;if(!scope(manager,transport,e)||!manager_absent(*manager,id,e))return false;
  if(transport->contains_identity_source_v69(id)){e="Actual SAME receiver transport still publishes source identity";return false;}
  if(!leaves_.require_native_aliases_unpublished){e="Required real native/draw/condition/observer alias retirement proof";return false;}
  if(!leaves_.require_native_aliases_unpublished(id,e))return false;
  // Reread publication after callback; no fresh object/transport snapshot.
  if(!manager_absent(*manager,id,e)||transport->contains_identity_source_v69(id)){if(e.empty())e="Source receiver republished during native alias query";return false;}e.clear();return true;
 }
 bool transport_unpublished(const world::CanonicalObjectBorrowV1& actual,bool& absent,std::string& e){
  absent=false;std::shared_ptr<world::CanonicalObjectManagerV1> manager;std::shared_ptr<CanonicalReceiverTransportV1> transport;if(!scope(manager,transport,e)||!actual.identity||!actual.lease){if(e.empty())e="Required actual container transport borrow";return false;}absent=!transport->contains_identity_source_v69(actual.identity);e.clear();return true;
 }
 // Host storage continuation after genuine native D0 + manager unpublication.
 // This erases only SAME transport entry; then typed catalog clears its actual
 // C1 allocation. Required alias leaf precedes both; failed prefixes retain pins.
 bool retire_source_storage(std::uintptr_t id,std::string& e){
  if(body_running_||retirement_running_){e="Source storage retirement cannot reenter native journal delivery";return false;}
  retirement_running_=true;struct Guard{bool& running;~Guard(){running=false;}}guard{retirement_running_};
  auto* entry=find(id);if(!entry||entry->busy||!completed(*entry,e)){if(e.empty())e="Required completed actual source C1 for retirement";return false;}
  std::shared_ptr<world::CanonicalObjectManagerV1> manager;std::shared_ptr<CanonicalReceiverTransportV1> transport;if(!scope(manager,transport,e)||!manager_absent(*manager,id,e))return false;
  if(!leaves_.require_native_aliases_unpublished||!leaves_.require_native_aliases_unpublished(id,e)){if(e.empty())e="Required real native/draw alias quiescence before transport erase";return false;}
  if(!manager_absent(*manager,id,e))return false;transport->erased(id);
  bool result=false;if(entry->kind==Kind::auxiliary){if(entry->auxiliary->retire_storage_after_source_release_v91)result=entry->auxiliary->retire_storage_after_source_release_v91(e);else {auto owner=auxiliary_.lock();if(owner)result=owner->erase_after_source_release(id,e);else e="Required SAME auxiliary catalog storage continuation";}}
  else if(entry->kind==Kind::light){auto owner=environment_.lock();if(owner)result=owner->erase_after_source_release(id,e);else e="Required SAME light catalog storage continuation";}
  else {auto owner=containers_.lock();if(owner)result=owner->retire_after_unpublication(id,e);else e="Required SAME container catalog storage continuation";}
  if(!result)return false;auto i=std::find_if(entries_.begin(),entries_.end(),[&](const auto& value){return value.get()==entry;});if(i==entries_.end()){e="Actual release entry changed during storage continuation";return false;}entries_.erase(i);e.clear();return true;
 }
 // Retire by SAME admitted record/control block, never lookup key0. Prepared
 // means actual C1 never ran: dropping host runtime/declaration/service wrappers
 // is not claimed native D0. Failed-started C1 requires real qualified unwind.
 bool retire_failed_constructor_prefix(const std::shared_ptr<void>& record,std::string& e){
  if(body_running_||retirement_running_||!record){e="Constructor prefix retirement requires nonrunning SAME record";return false;}
  auto i=std::find_if(entries_.begin(),entries_.end(),[&](const auto& entry){return entry->auxiliary?same(entry->auxiliary->record,record)&&entry->auxiliary->record.get()==record.get():entry->light&&same(entry->light,record)&&entry->light.get()==record.get();});
  if(i==entries_.end()){e="No actual constructor-admitted prefix for this record";return false;}auto& entry=**i;
  auto state=[&]{return entry.auxiliary?entry.auxiliary->constructor_state():entry.light->constructor_state;};
  auto constructed=[&]{return entry.auxiliary?entry.auxiliary->has_constructed_owner():bool(entry.light->owner);};
  if(entry.busy||entry.attempted||entry.destroyed||entry.identity()||constructed()){e="Positive/completed class must take genuine typed D0/storage continuation";return false;}
  const auto before=state();if(before==CanonicalConstructorStateV89::constructing||before==CanonicalConstructorStateV89::completed){e="Active/completed C1 cannot be discarded as an unconstructed prefix";return false;}
  retirement_running_=true;struct Guard{bool& running;~Guard(){running=false;}}guard{retirement_running_};
  if(before==CanonicalConstructorStateV89::failed){
   if(!leaves_.unwind_failed_constructor_prefix){e="Required genuine record-qualified failed C1 resource unwind/alias absence";return false;}
   SourceConstructorPrefixBorrowV89 actual{record,entry.source,entry.auxiliary?entry.auxiliary->class_name:"LightPoint",before,entry.auxiliary};
   if(!leaves_.unwind_failed_constructor_prefix(actual,e))return false;
  }
  // Constructor completion/owner are reread after Main unwind. The factory's
  // pending scope is already cleared and it never retained an absent C1 owner.
  if(state()!=before||constructed()||entry.identity()){e="Actual constructor prefix changed during qualified unwind; retain record";return false;}
  entries_.erase(i);e.clear();return true;
 }
 bool retire_failed_constructor_prefixes(std::string& e){
  for(std::size_t i=0;i<entries_.size();){auto& entry=*entries_[i];if(entry.identity()){++i;continue;}
   std::shared_ptr<void> record=entry.auxiliary?entry.auxiliary->record:std::shared_ptr<void>(entry.light);
   if(!record){e="Zero-identity container cannot bypass genuine class teardown";return false;}
   if(!retire_failed_constructor_prefix(record,e))return false;
  }e.clear();return true;
 }
 bool save_header(std::uintptr_t id,level::LevelSaveObjectBorrowV2& out,std::string& e){
  if(!validate_serialization_identity(id,e))return false;auto* entry=find(id);if(entry->auxiliary){if(!entry->auxiliary->borrow_save_header){e="Required actual auxiliary OBJS header getter";return false;}return entry->auxiliary->borrow_save_header(out,e);}
  out={};out.identity=reinterpret_cast<const void*>(id);
  if(entry->light){auto& actual=*entry->light->owner;auto pin=std::shared_ptr<void>(entry->light,entry->light->owner.get());auto canonical=actual.canonical(pin);out.checkpoint28=actual.byte(0x28);out.gametype48=actual.string(0x48);out.map_name=actual.string(0x30);out.room64=canonical.room64;out.disabled81=actual.byte(0x81);out.receiver_lease_v86=std::move(pin);}
  else {world::CanonicalGameObjectBaseOwnerV1* base{};if(entry->openable){base=&entry->openable->receiver().base();out.receiver_lease_v86=entry->openable;}else {base=&entry->destructible->base();out.receiver_lease_v86=entry->destructible;}out.checkpoint28=base->byte(0x28);out.gametype48=base->string(0x48);out.map_name=base->string(0x30);out.room64=&base->room64();out.disabled81=base->byte(0x81);}
  e.clear();return true;
 }
 bool save_connection(std::uintptr_t id,NonCharacterRestoreServicesV89 leaves,std::shared_ptr<NonCharacterSaveConnectionV89>& out,std::string& e){
  if(!validate_serialization_identity(id,e))return false;auto* entry=find(id);if(entry->save_connection){out=entry->save_connection;e.clear();return true;}
  bool result=false;
  if(entry->auxiliary){if(!entry->auxiliary->make_save_connection){e="Required typed actual auxiliary save projection";return false;}result=entry->auxiliary->make_save_connection(std::move(leaves),entry->save_connection,e);}
  else if(entry->light)result=make_noncharacter_record_save_v89(entry->light,[](auto& actual,auto& fields,std::string& e){return light_save_fields_v89(actual,fields,e);},std::move(leaves),entry->save_connection,e);
  else if(entry->openable){auto actual=std::shared_ptr<world::CanonicalOpenableContainerV1>(entry->openable,&entry->openable->receiver());result=make_noncharacter_receiver_save_v89(actual,[](auto& actual,auto& fields,std::string& e){return container_save_fields_v89(actual,fields,e);},std::move(leaves),entry->save_connection,e);}
  else result=make_noncharacter_receiver_save_v89(entry->destructible,[](auto& actual,auto& fields,std::string& e){return container_save_fields_v89(actual,fields,e);},std::move(leaves),entry->save_connection,e);
  if(!result)return false;out=entry->save_connection;e.clear();return true;
 }
 bool validate_serialization_identity(std::uintptr_t id,std::string& e){auto* entry=find(id);if(!entry||entry->busy||entry->attempted||entry->destroyed){e="Actual source receiver is not live for serialization";return false;}std::shared_ptr<world::CanonicalObjectManagerV1> manager;std::shared_ptr<CanonicalReceiverTransportV1> transport;return scope(manager,transport,e);}
 bool handles_identity(std::uintptr_t id){return find(id)!=nullptr;}
 bool light_identity(std::uintptr_t id){auto* entry=find(id);return entry&&entry->kind==Kind::light;}
 bool same_manager(world::CanonicalObjectManagerV1& actual)const noexcept{auto manager=manager_.lock();return manager&&manager.get()==&actual;}
 bool validate_class_borrow(const world::CanonicalObjectBorrowV1& actual,std::string& e){
  auto* entry=find(actual.identity);if(!entry||!actual.lease){e="Required admitted SAME class D0 receiver";return false;}
  std::shared_ptr<void> record;
  if(entry->auxiliary)record=entry->auxiliary->record;else if(entry->light)record=entry->light;else if(entry->openable)record=entry->openable;else record=entry->destructible;
  if(!same(record,actual.lease)){e="Class D0 lease differs from actual constructor-admitted record";return false;}e.clear();return true;
 }
 bool admit_orphan(const world::CanonicalObjectBorrowV1& actual,std::string& e){
  if(!validate_class_borrow(actual,e))return false;auto* entry=find(actual.identity);
  if(entry->busy||entry->attempted||entry->destroyed){e="Cannot admit already destructed native orphan";return false;}
  // SAME field/lease borrow supplied by real Remove before its map erase; no
  // actor allocation, receiver lookup registry or property/runtime snapshot.
  entry->orphan=actual;e.clear();return true;
 }
 bool orphan_receiver(std::uintptr_t id,world::CanonicalObjectBorrowV1& actual,std::string& e){
  auto* entry=find(id);if(!entry||!entry->orphan.identity||!entry->orphan.lease){e="Required original Remove orphan admission before native map erase";return false;}actual=entry->orphan;e.clear();return true;
 }
 bool retire_completed_for_manager(world::CanonicalObjectManagerV1& actual,std::string& e){
  if(!same_manager(actual)){e="Source release journal differs from native manager";return false;}
  // A single Remove leaves other live source objects; retire ONLY genuine D0
  // completions whose source map publication is absent. Zero pre-C1 prefixes
  // remain explicit, not fabricated native destructors/completion receipts.
  for(std::size_t i=0;i<entries_.size();){auto& entry=*entries_[i];const auto id=entry.identity();
   bool done=entry.destroyed||(entry.light&&entry.light->owner&&entry.light->owner->source_teardown_state()==world::LightTeardownStateV67::ObjectBaseCompleted);
   if(!id||!done){++i;continue;}std::string absence;if(!manager_absent(actual,id,absence)){++i;continue;}
   if(!retire_source_storage(id,e))return false;
  }e.clear();return true;
 }
 std::size_t retained_prefix_count()const noexcept{return entries_.size();}
};
// Populate ONLY real constructor-admission/release seams. Engine leaf producers,
// immutable source arrays and actual factory catalog remain Parent/Main-owned.
// Caller retains journal separately; every service callback captures it weakly.
inline bool bind_source_release_journal_v69(ProductionNonCharacterServicesV67& source,const std::shared_ptr<SourceReleaseJournalV69>& journal,std::string& e){
 if(!journal||source.auxiliary.retain_release_record||source.auxiliary.release_completed||source.containers.admit_openable||source.containers.admit_destructible||source.containers.teardown_openable||source.containers.transport_unpublished){e="Require unbound actual source admission/release authority";return false;}
 if(source.lights&&(source.lights->retain_release_record||source.lights->release_completed)){e="Require unbound actual LightPoint release authority";return false;}
 std::weak_ptr<SourceReleaseJournalV69> weak=journal;
 source.auxiliary.retain_release_record=[weak](const auto& d,const auto& r,const auto& q,std::string& e){auto p=weak.lock();if(!p){e="Required retained actual source release journal";return false;}return p->admit_auxiliary(d,r,q,e);};
 source.auxiliary.release_completed=[weak](const auto& d,std::uintptr_t id,std::string& e){auto p=weak.lock();if(!p){e="Required retained actual source release journal";return false;}return p->release_completed(d,id,e);};
 source.containers.admit_openable=[weak](const auto& s,const auto& q,const auto& a,std::string& e){auto p=weak.lock();if(!p){e="Required retained actual source release journal";return false;}return p->admit_container(s,q,a,e);};
 source.containers.admit_destructible=[weak](const auto& s,const auto& q,const auto& a,std::string& e){auto p=weak.lock();if(!p){e="Required retained actual source release journal";return false;}return p->admit_container(s,q,a,e);};
 source.containers.teardown_openable=[weak](const auto& a,std::string& e){auto p=weak.lock();if(!p){e="Required retained actual source release journal";return false;}return p->openable_native_body(a,e);};
 source.containers.transport_unpublished=[weak](const auto& a,bool& absent,std::string& e){auto p=weak.lock();if(!p){e="Required retained actual source release journal";return false;}return p->transport_unpublished(a,absent,e);};
 if(source.lights){auto lights=std::make_shared<LightEnvironmentLeavesV67>(*source.lights);
  lights->retain_release_record=[weak](const auto& d,const auto& r,const auto& q,std::string& e){auto p=weak.lock();if(!p){e="Required retained actual source release journal";return false;}return p->admit_light(d,r,q,e);};
  lights->release_completed=[weak](const auto& d,std::uintptr_t id,std::string& e){auto p=weak.lock();if(!p){e="Required retained actual source release journal";return false;}return p->release_completed(d,id,e);};source.lights=std::move(lights);
 }
 e.clear();return true;
}
// Before Parent compose: uses exact incoming input.source auxiliary authority,
// not a complete retained SourceCampaignCandidateBorrow/World receipt. Parent's
// after_catalog_composition_v89 hook runs after real catalog compose, before XML.
template<class ActualCandidate,class Inputs>
bool prepare_source_release_journal_inputs_v89(const ActualCandidate& actual,Inputs& inputs,
 SourceReleaseJournalLeavesV69 leaves,std::shared_ptr<SourceReleaseJournalV69>& journal,std::string& e){
 ScopeV67 scope{actual.actual_world,actual.level,actual.objects,actual.properties,inputs.source.auxiliary.owner};
 if(!SourceReleaseJournalV69::create(scope,actual.receiver_transport_v69,std::move(leaves),journal,e))return false;
 if(!bind_source_release_journal_v69(inputs.source,journal,e))return false;
 auto prior=std::move(inputs.after_catalog_composition_v89);std::weak_ptr<SourceReleaseJournalV69> weak=journal;
 inputs.after_catalog_composition_v89=[weak,prior=std::move(prior)](ProductionNonCharacterOwnersV67& owners,std::string& e){
  auto same=weak.lock();if(!same){e="Required external sibling source release journal before XML";return false;}
  if(prior&&!prior(owners,e))return false;return same->attach_catalog_owners(owners,e);
 };e.clear();return true;
}
// Adds only actual class-D0/orphan/journal routes to Main's native lifecycle.
// Native storage, quiescence, game/AI/online leaves stay genuine incoming Main
// methods. Unknown classes use the existing real route; no fallback after any
// reached journal-owned failure. Callback captures keep this journal weak.
inline bool bind_source_journal_manager_lifecycle_v89(const std::shared_ptr<SourceReleaseJournalV69>& journal,
 world::CanonicalObjectLifecycleV1& native,
 std::function<bool(std::shared_ptr<world::LightQuiescenceLeaseV67>&,std::string&)> actual_light_lease,std::string& e){
 if(!journal||!native.owner){e="Required genuine Main lifecycle authority and external journal sibling";return false;}
 std::weak_ptr<SourceReleaseJournalV69> weak=journal;auto other_d0=std::move(native.class_d0);auto other_orphan=std::move(native.orphan_admit);auto other_receiver=std::move(native.native_receiver);auto other_retire=std::move(native.retire_after_unpublication);
 native.class_d0=[weak,other_d0=std::move(other_d0),actual_light_lease=std::move(actual_light_lease)](const auto& actual,std::string& e){
  auto same=weak.lock();if(!same){e="Required external source class release journal";return false;}
  if(!same->handles_identity(actual.identity)){if(other_d0)return other_d0(actual,e);e="Required genuine Main D0 for nonjournal Character/Module/other receiver";return false;}
  if(!same->validate_class_borrow(actual,e))return false;std::shared_ptr<world::LightQuiescenceLeaseV67> lease;
  if(same->light_identity(actual.identity)&&(!actual_light_lease||!actual_light_lease(lease,e)||!lease)){if(e.empty())e="Required genuine prepared LightPoint owning-thread lease";return false;}
  return same->destroy_source(actual.identity,std::move(lease),e);
 };
 native.orphan_admit=[weak,other=std::move(other_orphan)](const auto& actual,std::string& e){auto same=weak.lock();if(!same){e="Required external source orphan release journal";return false;}if(same->handles_identity(actual.identity))return same->admit_orphan(actual,e);if(other)return other(actual,e);e="Required Main native orphan admission for nonjournal receiver";return false;};
 native.native_receiver=[weak,other=std::move(other_receiver)](std::uintptr_t id,auto& actual,std::string& e){auto same=weak.lock();if(!same){e="Required external source orphan release journal";return false;}if(same->handles_identity(id))return same->orphan_receiver(id,actual,e);if(other)return other(id,actual,e);e="Required Main actual orphan receiver outside source journal";return false;};
 native.retire_after_unpublication=[weak,other=std::move(other_retire)](world::CanonicalObjectManagerV1& actual,std::string& e){auto same=weak.lock();if(!same||!same->retire_completed_for_manager(actual,e))return false;if(!other){e="Required Main remaining native manager/class journal retirement";return false;}return other(actual,e);};e.clear();return true;
}
}
