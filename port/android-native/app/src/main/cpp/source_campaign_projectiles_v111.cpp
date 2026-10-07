#include "source_campaign_projectiles_v111.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_retirement_v88.hpp"
#include "source_campaign_noncharacter_virtual_v105.hpp"
#include "source_campaign_object_update_bindings_v105.hpp"
#include "model_renderer.hpp"
#include "projectile_manager_preload_v96.hpp"
#include <canonical_receiver_transport_v1.hpp>
#include <canonical_spawn_owner_v1.hpp>
#include <canonical_gameobject_graph_v68.hpp>
#include <gameobject_stop_source_v111.hpp>
#include <source_assertion_process_v76.hpp>
#include <source_release_journal_v69.hpp>
#include <android/log.h>
#include <cstring>
#include <list>
namespace model_renderer {namespace {
bool required(const char* leaf,std::string& e){if(e.empty())e=std::string("Required actual campaign projectile ")+leaf;return false;}
constexpr const char* assertion_file="..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Projectiles\\ProjectileManager.cpp";
struct ProjectileBackendV111;
struct SpawnPrefixV111 {
 std::weak_ptr<ProjectileBackendV111> backend;
 std::shared_ptr<const std::string> name;
 std::unique_ptr<dh2::world::CanonicalSpawnAttemptV1> source;
 static bool construct(void*,const dh2::world::CanonicalFactoryEntryV1&,dh2::world::CanonicalClassReceiverV1&,std::string&);
 static bool unknown(void*,const char*,std::string&);
 static bool resolve(void*,dh2::target_providers::Handle16&,bool,const dh2::world::CanonicalObjectBorrowV1*&,std::string&);
 static bool condition(void*,const dh2::world::CanonicalObjectBorrowV1&,bool,std::string&);
 static bool updatable(void*,const dh2::world::CanonicalObjectBorrowV1&,bool&,std::string&);
 static bool pending(void*,const dh2::world::CanonicalObjectBorrowV1&,std::string&);
 static bool receiver(void*,const dh2::world::CanonicalObjectBorrowV1&,const dh2::world::CanonicalClassReceiverV1*&,std::string&);
};
struct ProjectileBackendV111:std::enable_shared_from_this<ProjectileBackendV111> {
 std::weak_ptr<SourceWorldBorrowV61> world;
 SourceCampaignProjectileClassServicesV111 classes;
 std::shared_ptr<dh2::world::SourceAssertionProcessV76> assertion;
 //Failed source attempt owns only its actual name/dispatch and borrowed
 //manager references. It cannot retain the containing World or form a cycle.
 std::list<std::shared_ptr<SpawnPrefixV111>> failed_prefixes;
 bool scope(SourceCampaignCandidateBorrowV55& c,std::shared_ptr<SourceWorldBorrowV61>& w,std::string& e,bool cleanup=false)const{
  w=world.lock();if(!w||!w->owner||!w->canonical_world||!w->application||
    !borrow_source_campaign_candidate_runtime_v61(c,e)||c.actual_world!=w->owner||
    c.application!=w->application||c.objects!=w->canonical_world->manager_lease||
    c.properties!=&w->canonical_world->properties||(!cleanup&&source_campaign_retirement_requested_v88()))
   return required("SAME current World/App/ObjectManager38",e);
  e.clear();return true;
 }
 bool current(std::string& e)const{SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;return scope(c,w,e);}
 bool spawn(dh2::world::CanonicalObjectManagerV1& manager,const char* type,const char* name,
  std::int32_t deferred,bool network,dh2::target_providers::Handle16& out,std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!scope(c,w,e)||&manager!=c.objects.get()||!name||!type||(deferred!=0&&deferred!=1))
   return required("actual Spawn receiver/CStrings/arguments",e);
  auto prefix=std::make_shared<SpawnPrefixV111>();prefix->backend=shared_from_this();
  prefix->name=std::make_shared<const std::string>(name);
  dh2::world::CanonicalSpawnServicesV1 services{prefix.get(),SpawnPrefixV111::construct,
   SpawnPrefixV111::unknown,SpawnPrefixV111::resolve,SpawnPrefixV111::condition,
   SpawnPrefixV111::updatable,SpawnPrefixV111::pending,SpawnPrefixV111::receiver};
  prefix->source=std::make_unique<dh2::world::CanonicalSpawnAttemptV1>(manager,*c.properties,services);
  //Reserve the independent prefix holder BEFORE invoking actual constructor.
  failed_prefixes.push_back(prefix);
  if(!prefix->source->spawn(type,prefix->name->c_str(),deferred!=0,network,e))return false;
  out=prefix->source->handle();failed_prefixes.remove(prefix);e.clear();return true;
 }
 bool row(const dh2::world::CanonicalObjectBorrowV1& object,bool laser,
  dh2::world::ProjectileManagerOwnerV108::Row& out,std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!scope(c,w,e)||!classes.owner||!classes.borrow||!object.type_f4||
    *object.type_f4!=(laser?10u:9u))return required("actual type9/10 canonical class projection",e);
  SourceCampaignProjectileClassLoanV111 actual;
  if(!classes.borrow(c,object,laser,actual,e)||!actual.receiver||!actual.base||
    actual.base->identity()!=object.identity||actual.base->type_f4()!=*object.type_f4||
    actual.receiver.owner_before(object.lease)||object.lease.owner_before(actual.receiver)||
    !actual.set_manager3e4fe4)return required("SAME source Projectile base/SetManager378",e);
  if(!w->gameobject_graph_v68)return required("actual admitted Projectile resource graph",e);
  std::shared_ptr<void> base_pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  if(!w->gameobject_graph_v68->borrow_base_v77(object.identity,base_pin,base,e)||base!=actual.base||
    base_pin.owner_before(actual.receiver)||actual.receiver.owner_before(base_pin))
   return required("SAME canonical Projectile graph base",e);
  dh2::world::ProjectileManagerOwnerV108::Row result;
  result.receiver=actual.receiver;result.identity=object.identity;result.byte85=base->byte(0x85);
  if(!result.byte85)return required("actual source updating85 cell",e);
  const auto weak=std::weak_ptr<ProjectileBackendV111>(shared_from_this());
  const auto receiver=std::weak_ptr<void>(actual.receiver);const auto id=object.identity;
  result.visible40=[weak,receiver,base,id](bool value,std::string& e){
   auto backend=weak.lock();auto pin=receiver.lock();SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
   if(!backend||!pin||!backend->scope(c,w,e,true))return required("live original virtual40 receiver",e);
   if(!base->store_byte(0x80,value?base->lifecycle().enabled8a:0,e))return false;
   return source_campaign_noncharacter_sync_visibility_v105(c,id,e);
  };
  result.object_delete=[receiver,base](std::string& e){auto pin=receiver.lock();if(!pin)return required("live ObjectBase.Delete receiver",e);base->source_delete_v4();e.clear();return true;};
  result.stop3938f8=[weak,receiver,base,id](std::string& e){
   auto backend=weak.lock();auto pin=receiver.lock();SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
   if(!backend||!pin||!backend->scope(c,w,e)||!w->gameobject_graph_v68)return false;
   dh2::world::GameObjectStopServicesV111 s;
   //Both actual Projectile vtables+64 select GameObject34006c literal1.
   s.updating_position_from_physics64=[](bool& value,std::string& e){value=true;e.clear();return true;};
   s.physical=[graph=w->gameobject_graph_v68,id](std::shared_ptr<void>& pin,dh2::physical::NativeBody*& body,std::string& e){return graph->borrow_native_body_v90(id,pin,body,e);};
   return dh2::world::gameobject_stop_source_v111(*base,s,e);
  };
  result.set_manager3e4fe4=std::move(actual.set_manager3e4fe4);
  out=std::move(result);e.clear();return true;
 }
 bool missing(std::uintptr_t id,bool,std::string& e){
  if(!assertion||!assertion->source_level())return required("actual process assertion mode",e);
  auto mode=*assertion->source_level();
  if(mode==2){e="Original Projectile DeSpawn NULL/not-found deliberate assertion fault";return false;}
  if(!id&&mode==1){if(!assertion->report(assertion_file,0x11a,"p",e))return false;mode=*assertion->source_level();}
  if(mode==2){e="Original Projectile DeSpawn deliberate assertion fault after NULL diagnostic";return false;}
  if(mode==1&&!assertion->report(assertion_file,0x13e,"!\"ProjectileManager :: WTF!\"",e))return false;
  e.clear();return true;
 }
 bool retire(std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!scope(c,w,e,true)||!require_source_campaign_quiescence_v104(w->owner,e))return false;
  auto transport=c.receiver_transport_v69.lock();auto journal=w->noncharacter_release_journal_v89;
  const auto absent=[manager=c.objects,transport,journal](std::string& e){
   if(!manager||manager->source_count50()!=0||manager->source_map_size1c_v38()!=1||
     !manager->source_active2c_v102().empty()||!manager->source_pending34_v102().empty()||
     !manager->source_deletion3c_v102().empty()||!transport||transport->retained_count()!=0||
     !journal||journal->retained_prefix_count()!=0){
    e="Projectile receipts still have actual source manager/transport/class publication";return false;
   }
   e.clear();return true;
  };
  auto manager=dh2::world::projectile_manager_process_v108();
  if(!absent(e)||!manager->retire_constructor_receipts_v111(c.objects,absent,e))return false;
  failed_prefixes.clear();e.clear();return true;
 }
};
bool SpawnPrefixV111::construct(void* raw,const dh2::world::CanonicalFactoryEntryV1& entry,
 dh2::world::CanonicalClassReceiverV1& out,std::string& e){
 auto& prefix=*static_cast<SpawnPrefixV111*>(raw);auto backend=prefix.backend.lock();
 SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
 if(!backend||!backend->scope(c,w,e))return false;auto transport=c.receiver_transport_v69.lock();
 if(!transport)return required("actual canonical Spawn receiver transport",e);
 return transport->construct_native_spawn_v111(entry,prefix.name,out,e);
}
bool SpawnPrefixV111::unknown(void*,const char* name,std::string& e){if(!name)return required("unknown-type CString",e);__android_log_print(ANDROID_LOG_WARN,"DH2Native","Source unknown factory type | %s",name);e.clear();return true;}
bool SpawnPrefixV111::resolve(void* raw,dh2::target_providers::Handle16& handle,bool refresh,
 const dh2::world::CanonicalObjectBorrowV1*& out,std::string& e){
 auto backend=static_cast<SpawnPrefixV111*>(raw)->backend.lock();SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
 return backend&&backend->scope(c,w,e)&&c.objects->resolve_handle_v4(handle,refresh,out,{},e);
}
bool SpawnPrefixV111::condition(void* raw,const dh2::world::CanonicalObjectBorrowV1& object,bool mark,std::string& e){
 auto backend=static_cast<SpawnPrefixV111*>(raw)->backend.lock();SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
 return backend&&backend->scope(c,w,e)&&source_campaign_object_test_condition_v105(c,object.identity,false,mark,e);
}
bool SpawnPrefixV111::updatable(void*,const dh2::world::CanonicalObjectBorrowV1& object,bool& value,std::string& e){
 if(!object.type_f4||(*object.type_f4!=9&&*object.type_f4!=10))return required("selected Projectile virtual38",e);
 value=true;e.clear();return true; //actual shared3e3e0c
}
bool SpawnPrefixV111::pending(void* raw,const dh2::world::CanonicalObjectBorrowV1& object,std::string& e){
 auto backend=static_cast<SpawnPrefixV111*>(raw)->backend.lock();SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
 return backend&&backend->scope(c,w,e)&&c.objects->append_pending(object,e);
}
bool SpawnPrefixV111::receiver(void* raw,const dh2::world::CanonicalObjectBorrowV1& object,
 const dh2::world::CanonicalClassReceiverV1*& out,std::string& e){
 auto backend=static_cast<SpawnPrefixV111*>(raw)->backend.lock();SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
 if(!backend||!backend->scope(c,w,e))return false;auto transport=c.receiver_transport_v69.lock();
 return transport&&transport->receiver(object,out,e);
}
}
bool install_source_campaign_projectile_backend_v111(const SourceCampaignCandidateBorrowV55& c,
 SourceCampaignProjectileClassServicesV111 classes,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> w;if(!borrow_source_campaign_condition_world_v70(c,w,e)||
   !classes.owner||!classes.borrow||w->projectile_precache_v96||w->projectile_retire_receipts_v111)return required("fresh actual class borrower/native packet",e);
 auto backend=std::make_shared<ProjectileBackendV111>();backend->world=w;
 backend->classes=std::move(classes);backend->assertion=dh2::world::SourceAssertionProcessV76::borrow();
 dh2::world::ProjectileManagerOwnerV108::CreateNativeV96 native;native.owner=backend;
 native.object_manager=[backend](std::shared_ptr<dh2::world::CanonicalObjectManagerV1>& out,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!backend->scope(c,w,e))return false;out=c.objects;return true;};
 native.spawn=[backend](auto& manager,const char* type,const char* name,std::int32_t deferred,bool network,auto& handle,std::string& e){return backend->spawn(manager,type,name,deferred,network,handle,e);};
 native.row=[backend](const auto& object,bool laser,auto& row,std::string& e){return backend->row(object,laser,row,e);};
 auto packet=std::make_shared<dh2::loader::ProjectilePrecacheSourcesV96>();
 if(!bind_actual_projectile_manager_preload_v96(std::move(native),
   [backend](std::string& e){return backend->current(e);},
   [backend](auto id,bool laser,std::string& e){return backend->missing(id,laser,e);},*packet,e))return false;
 w->projectile_precache_v96=std::move(packet);
 w->projectile_retire_receipts_v111=[backend](std::string& e){return backend->retire(e);};
 e.clear();return true;
}
}
