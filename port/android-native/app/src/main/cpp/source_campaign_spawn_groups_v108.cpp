#include "source_campaign_spawn_groups_v108.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_noncharacter_virtual_v105.hpp"
#include "source_campaign_object_update_bindings_v105.hpp"
#include "source_campaign_object_update_actor_v104.hpp"
#include "source_process_arrays_v101.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "source_campaign_script_actor_v96.hpp"
#include "model_renderer.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <canonical_spawn_owner_v1.hpp>
#include <canonical_receiver_transport_v1.hpp>
#include <production_noncharacter_catalog_v67.hpp>
#include <production_noncharacter_composition_v67.hpp>
#include <catalog_auxiliary_v67.hpp>
#include <application_spawn_random_owner_v4.hpp>
#include <canonical_character_spawn_select_v87.hpp>
#include <application_services_owner_v5.hpp>
#include <android/log.h>
#include <algorithm>
#include <cstring>
namespace model_renderer {namespace {
bool required(std::string& e,const char* leaf){if(e.empty())e=std::string("Required actual campaign SpawnGroups ")+leaf;return false;}
struct SpawnTransportV108 {
 std::weak_ptr<SourceWorldBorrowV61> world;
 std::shared_ptr<dh2::android_ui::SourceProcessArraysV101> arrays;
 bool scope(SourceCampaignCandidateBorrowV55& c,std::shared_ptr<SourceWorldBorrowV61>& w,std::string& e)const{
  w=world.lock();return w&&borrow_source_campaign_candidate_v55(c,e)&&c.actual_world==w->owner&&c.application==w->application&&c.objects==w->canonical_world->manager_lease?true:required(e,"SAME current source World/App/manager");
 }
 bool spot(std::uintptr_t id,std::shared_ptr<dh2::world::CanonicalSpawnSpotV108>& out,std::string& e)const{
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!scope(c,w,e)||!w->noncharacter_owners_v105||!w->noncharacter_owners_v105->auxiliary)return false;
  auto* families=w->noncharacter_owners_v105->auxiliary->source_families_v105();if(!families)return required(e,"actual SpawnSpot factory");
  for(const auto& r:families->spawn_spots_v108())if(r&&r->owner&&r->owner->base().identity()==id){
   out=std::shared_ptr<dh2::world::CanonicalSpawnSpotV108>(r,r->owner.get());e.clear();return true;
  }
  return required(e,"SAME retained SpawnSpot receiver");
 }
};
struct SpawnAttemptV108 {
 SourceCampaignCandidateBorrowV55 scope;std::string name;
 std::unique_ptr<dh2::world::CanonicalSpawnAttemptV1> attempt;
 std::shared_ptr<dh2::loader::CanonicalReceiverTransportV1> transport;
 static bool construct(void* p,const dh2::world::CanonicalFactoryEntryV1& entry,dh2::world::CanonicalClassReceiverV1& out,std::string& e){
  auto& self=*static_cast<SpawnAttemptV108*>(p);
  if(!construct_source_campaign_character_spawn_v68(self.scope.actual_world,entry,self.name.c_str(),out,e))return false;
  //Publish the SAME new C1 dispatch before canonical Add; later source
  //visitors/duplicate resolution borrow it, without C1/InitPost replay.
  self.transport=self.scope.receiver_transport_v69.lock();
  if(!self.transport)return required(e,"actual dynamic Spawn receiver transport");
  return self.transport->admit_constructed_source_v91(out,e);
 }
 static bool resolve(void* p,dh2::target_providers::Handle16& h,bool refresh,const dh2::world::CanonicalObjectBorrowV1*& out,std::string& e){auto& self=*static_cast<SpawnAttemptV108*>(p);return self.scope.objects->resolve_handle_v4(h,refresh,out,[](std::string& e){return required(e,"original source Spawn NULL assertion");},e);}
 static bool condition(void* p,const dh2::world::CanonicalObjectBorrowV1& a,bool mark,std::string& e){return source_campaign_object_test_condition_v105(static_cast<SpawnAttemptV108*>(p)->scope,a.identity,false,mark,e);}
 static bool updatable(void*,const dh2::world::CanonicalObjectBorrowV1& a,bool& value,std::string& e){if(!a.type_f4||*a.type_f4!=0)return required(e,"actual Character virtual38");value=true;e.clear();return true;}
 static bool pending(void* p,const dh2::world::CanonicalObjectBorrowV1& a,std::string& e){return static_cast<SpawnAttemptV108*>(p)->scope.objects->append_pending(a,e);}
 static bool receiver(void* p,const dh2::world::CanonicalObjectBorrowV1& a,const dh2::world::CanonicalClassReceiverV1*& out,std::string& e){auto& self=*static_cast<SpawnAttemptV108*>(p);if(!self.transport)self.transport=self.scope.receiver_transport_v69.lock();if(!self.transport)return required(e,"actual Spawn receiver transport lifetime");return self.transport->receiver(a,out,e);}
 static bool unknown(void*,const char* name,std::string& e){if(!name)return required(e,"source unknown type CString");__android_log_print(ANDROID_LOG_WARN,"DH2Native","Source unknown factory type | %s",name);e.clear();return true;}
 bool run(const char* type,const char* requested,bool deferred,bool network,std::uintptr_t& out,std::string& e){
  name=requested;dh2::world::CanonicalSpawnServicesV1 s{this,construct,unknown,resolve,condition,updatable,pending,receiver};
  attempt=std::make_unique<dh2::world::CanonicalSpawnAttemptV1>(*scope.objects,*scope.properties,s);
  if(!attempt->spawn(type,name.c_str(),deferred,network,e))return false;
  const dh2::world::CanonicalObjectBorrowV1* actual{};auto handle=attempt->handle();
  if(!scope.objects->resolve_handle_v4(handle,true,actual,[](std::string& e){return required(e,"post Spawn source handle");},e))return false;
  if(!actual){out=0;e.clear();return true;}std::uintptr_t character;
  if(!actual->as_character||!actual->as_character(actual->context,character,e))return false;out=character;e.clear();return true;
 }
};
std::int32_t signed_bits(std::uint32_t bits){std::int32_t out;std::memcpy(&out,&bits,4);return out;}
}
bool compose_source_campaign_spawn_groups_v108(const SourceCampaignCandidateBorrowV55& c,dh2::world::SpawnGroupServicesV108& out,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> w;std::shared_ptr<dh2::android_ui::SourceProcessArraysV101> arrays;
 if(!borrow_source_campaign_condition_world_v70(c,w,e)||!borrow_process_spawn_arrays_v108(arrays,e)||!arrays)return false;
 auto t=std::make_shared<SpawnTransportV108>();t->world=w;t->arrays=std::move(arrays);out={};out.owner=t;
 out.application_dt=[t](std::uint32_t& value,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!t->scope(c,w,e)||!w->application->source_loading_v55().native_dt_produced_v93)return required(e,"produced App8c");value=w->application->source_loading_v55().dt8c;e.clear();return true;};
 out.group_id=[t](const char* name,std::int32_t& value,std::string& e){auto* group=t->arrays->group("SpawnGroups");if(!name||!group||!group->names_loaded)return required(e,"source Arrays.SpawnGroups names");value=-1;for(std::size_t i=0;i<group->names.size();++i)if(!std::strcmp(name,group->names[i].c_str())){value=static_cast<std::int32_t>(i);break;}e.clear();return true;};
 out.definition=[t](std::int32_t index,dh2::world::SpawnGroupDefinitionV108& out,std::string& e){auto* group=t->arrays->group("SpawnGroups");if(!group||!group->records_loaded||index<0||std::size_t(index)>=group->rows.size())return required(e,"actual source SpawnGroup row");const auto& row=group->rows[std::size_t(index)];using K=dh2::android_ui::ProcessArrayValueV101::Kind;if(row.fields.size()!=3||row.fields[0].kind!=K::byte||row.fields[1].kind!=K::word||row.fields[2].kind!=K::vector)return required(e,"actual SpawnGroup bi[iii] schema");
  out.owner=t->arrays;out.local_only4=row.fields[0].bits!=0;out.delay8=signed_bits(row.fields[1].bits);out.count_c=static_cast<std::uint32_t>(row.fields[2].elements.size());
  out.choice=[t,index](std::uint32_t i,dh2::world::SpawnGroupChoiceV108& out,std::string& e){auto* group=t->arrays->group("SpawnGroups");if(!group||std::size_t(index)>=group->rows.size())return required(e,"retained immutable source group");const auto& list=group->rows[std::size_t(index)].fields[2].elements;if(i>=list.size()||list[i].fields.size()!=3)return required(e,"actual group choice index");const auto& row=list[i].fields;using K=dh2::android_ui::ProcessArrayValueV101::Kind;for(const auto& field:row)if(field.kind!=K::word)return required(e,"original SpawnGroup choice iii");out={signed_bits(row[0].bits),signed_bits(row[1].bits),signed_bits(row[2].bits)};e.clear();return true;};e.clear();return true;};
 out.is_zonable_c4=[t](std::uintptr_t id,bool& value,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;return t->scope(c,w,e)&&source_campaign_noncharacter_is_zonable_v105(c,id,value,e);};
 out.zone_flags=[t](std::uintptr_t id,std::uint8_t& first,std::uint8_t& second,std::string& e){std::shared_ptr<dh2::world::CanonicalSpawnSpotV108> spot;if(!t->spot(id,spot,e))return false;auto a=spot->base().byte(0x2ee),b=spot->base().byte(0x2f0);if(!a||!b)return required(e,"same SpawnSpot zone fields");first=*a;second=*b;e.clear();return true;};
 out.random=[t](std::int32_t range,std::int32_t& value,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!t->scope(c,w,e)||!w->application->source_random_v62())return false;return dh2_loot_v2_random(&w->application->source_random_v62()->channel(0),range,&value)==0?true:required(e,"original shared random");};
 out.spawn=[t](const char* type,const char* name,bool deferred,bool network,std::uintptr_t& out,std::string& e){SpawnAttemptV108 attempt;std::shared_ptr<SourceWorldBorrowV61> w;if(!name||!t->scope(attempt.scope,w,e))return false;return attempt.run(type,name,deferred,network,out,e);};
 out.init_spawned=[t](std::uintptr_t id,std::int32_t props,const float* p,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;return t->scope(c,w,e)&&source_campaign_character_init_spawned_v108(w->owner,id,props,p,e);};
 out.place=[t](std::uintptr_t spot,std::uintptr_t character,std::string& e){std::shared_ptr<dh2::world::CanonicalSpawnSpotV108> actual;return t->spot(spot,actual,e)&&actual->place_object(character,e);};
 out.log=[](const char* message,std::string& e){if(!message)return required(e,"source log CString");__android_log_print(ANDROID_LOG_DEBUG,"DH2Native","%s",message);e.clear();return true;};e.clear();return true;
}
bool bind_source_campaign_spawn_spot_v108(const std::shared_ptr<void>& world,dh2::world::CanonicalSpawnSpotV108& receiver,dh2::world::SpawnSpotServicesV108& out,std::string& e){
 SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!borrow_source_campaign_candidate_v55(c,e)||c.actual_world!=world||!borrow_source_campaign_condition_world_v70(c,w,e))return false;
 const std::weak_ptr<SourceWorldBorrowV61> weak=w;const auto id=receiver.base().identity();out={};out.owner=w->files_owner;
 auto operation=[weak,id](dh2::world::CanonicalSpawnSpotV108& actual,bool insert,std::string& e){auto w=weak.lock();SourceCampaignCandidateBorrowV55 c;dh2::world::SpawnGroupServicesV108 services;if(!w||actual.base().identity()!=id||!borrow_source_campaign_candidate_v55(c,e)||c.actual_world!=w->owner||!compose_source_campaign_spawn_groups_v108(c,services,e))return false;
  auto t=std::static_pointer_cast<SpawnTransportV108>(services.owner);std::shared_ptr<dh2::world::CanonicalSpawnSpotV108> same;if(!t->spot(id,same,e)||same.get()!=&actual)return required(e,"actual registered SpawnSpot owner");
  dh2::world::SpawnSpotBorrowV108 spot{same,id,&same->group(),same->base().vector3(0x160)};auto manager=dh2::world::process_spawn_group_manager_v108();return insert?manager->insert(spot,services,e):manager->erase(spot,services,e);};
 out.insert=[operation](auto& actual,std::string& e){return operation(actual,true,e);};out.erase=[operation](auto& actual,std::string& e){return operation(actual,false,e);};e.clear();return true;
}
bool source_campaign_character_init_spawned_v108(const std::shared_ptr<void>& world,std::uintptr_t id,std::int32_t props,const float* position,std::string& e){
 SourceCampaignCharacterBorrowV62 actual;if(!position||!borrow_source_campaign_character_v62(world,id,actual,e)||!actual.character->actor)return false;auto& r=*actual.character;
 if(!r.actor->init_post_fields(r.init_fields,e)||!r.init_fields.properties_id13c8)return false;
 *r.init_fields.properties_id13c8=static_cast<std::int16_t>(static_cast<std::uint16_t>(props));
 dh2::world::GameObjectInitializationFieldsV62 fields;if(!r.actor->inherited_initialization_fields_v62(actual.character,fields,e))return false;
 auto spawned=fields.byte(0x1481),zone=fields.byte(0x2f0);if(!spawned||!zone)return required(e,"actual source spawned1481/2f0 cells");*spawned=1;*zone=1;
 dh2::character::NpcInitPostResponseV1 result;
 if(!dh2::world::CanonicalCharacterCandidateRecordV60::init_service(&r,{0x3a58f4,0,0,id,reinterpret_cast<std::uintptr_t>(position),nullptr},result,e))return false;
 std::array<float,3> initial;std::copy_n(r.init_fields.initial_position1450,3,initial.begin());if(!r.set_position(initial,true,e)||!r.initialize(e)||!r.initialize_final_v70(e)||!source_campaign_character_set_visible_v96(world,id,true,e))return false;
 return dh2::character::source_character_select_spawn_v87(r,0,0,e);
}
}
