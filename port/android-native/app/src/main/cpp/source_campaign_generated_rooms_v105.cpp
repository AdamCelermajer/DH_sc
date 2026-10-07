#include "source_campaign_generated_rooms_v105.hpp"
#include "model_renderer.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "renderer_campaign_generated_rooms_v92.hpp"
#include "source_campaign_noncharacter_virtual_v105.hpp"
#include "source_campaign_noncharacter_owners_v105.hpp"
#include "source_campaign_object_update_bindings_v105.hpp"
#include "source_campaign_class_release_v106.hpp"
#include "renderer_character_campaign_v62.hpp"
#include <module_room_zone_connection_v91.hpp>
#include <catalog_generic_v70.hpp>
#include <source_assertion_process_v76.hpp>
#include <source_object_handle_assertion_v105.hpp>
namespace model_renderer {namespace {
struct NativeGeneratedRoomPrimitivesV105 {
 std::weak_ptr<SourceWorldBorrowV61> source;
 bool current(std::shared_ptr<SourceWorldBorrowV61>& world,SourceCampaignCandidateBorrowV55& actual,std::string& e)const{
  world=source.lock();if(!world||!world->owner||!world->files_owner||!borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world.get()!=world->owner.get()||actual.actual_world.owner_before(world->owner)||world->owner.owner_before(actual.actual_world)||actual.application!=world->application||!world->canonical_world||actual.objects!=world->canonical_world->manager_lease){if(e.empty())e="Released SAME generated-room primitive World/manager";return false;}return true;
 }
 bool zonable(std::uintptr_t id,bool& value,std::string& e){std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;return current(world,actual,e)&&source_campaign_noncharacter_is_zonable_v105(actual,id,value,e);}
 bool transition(std::uintptr_t id,bool entered,std::string& e){std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;return current(world,actual,e)&&source_campaign_noncharacter_virtual_v105(actual,id,entered?0x38c710u:0x38c69cu,e);}
 bool condition(const dh2::world::CanonicalObjectBorrowV1& object,bool mark,std::string& e){std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;
  if(!current(world,actual,e))return false;const auto* published=object.shared_handle?actual.objects->object(object.shared_handle->key):nullptr;
  if(!published||published->identity!=object.identity||!object.lease||published->lease.owner_before(object.lease)||object.lease.owner_before(published->lease)){e="Foreign Room TestEnableCondition source receiver";return false;}
  return source_campaign_object_test_condition_v105(actual,object.identity,false,mark,e);
 }
 bool room_objects(const std::list<std::uintptr_t>& list,bool active,std::string& e){std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;if(!current(world,actual,e))return false;
  std::shared_ptr<dh2::loader::ProductionNonCharacterOwnersV67> owners;if(!borrow_source_campaign_noncharacter_owners_v105(actual,owners,e))return false;
  std::shared_ptr<dh2::world::CanonicalRoomZoneV3> pin;
  //Source caller's SAME live occupants394 list. Neither actor contents nor
  //the pointer list80 are copied into an independent room coordinator.
  for(auto id:actual.objects->source_rooms24_v104()){
   std::shared_ptr<dh2::world::CanonicalRoomZoneV3> room;
   if(world->module_room_zones_v91){std::string ignored;world->module_room_zones_v91->borrow_room(id,room,ignored);}
   if(!room&&owners&&owners->generic){auto record=owners->generic->source_room_v105(id);if(record&&record->constructor_completed_v91&&record->receiver)room=std::shared_ptr<dh2::world::CanonicalRoomZoneV3>(record,record->receiver.get());}
   if(room&&&room->source_occupants394_v104()==&list){pin=std::move(room);break;}
  }
  if(!pin){e="Required SAME retained Room occupants394 list for ObjectManager80";return false;}
  return active?actual.objects->source_add_room_objects_v105(&list,e):actual.objects->source_del_room_objects_v105(&list,e);
 }
};
}
bool bind_native_generated_room_primitives_v105(const std::shared_ptr<SourceWorldBorrowV61>& world,std::string& e){
 if(!world||!world->owner||!world->files_owner||!world->canonical_world){e="Required actual source World before Room primitive enrollment";return false;}
 if(world->generated_room_native_v92){e.clear();return true;} //Preserve supplied genuine platform transport.
 auto provider=std::make_shared<NativeGeneratedRoomPrimitivesV105>();provider->source=world;
 world->generated_room_native_v92=[provider](const SourceCampaignCandidateBorrowV55& expected,CampaignGeneratedRoomNativeV92& out,std::string& e){std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;
  if(!provider->current(world,actual,e)||actual.actual_world!=expected.actual_world||actual.level!=expected.level||actual.objects!=expected.objects)return false;
  CampaignGeneratedRoomNativeV92 s;s.owner=provider;
  s.is_zonable=[provider](auto id,bool& value,auto& e){return provider->zonable(id,value,e);};
  s.zone_transition=[provider](const auto& object,bool entered,auto& e){if(!object.owner||!object.identity){e="Required actual Room object transition receiver";return false;}return provider->transition(object.identity,entered,e);};
  s.occupant_transition=[provider](auto id,bool entered,auto& e){return provider->transition(id,entered,e);};
  s.room_objects=[provider](const auto& list,bool active,auto& e){return provider->room_objects(list,active,e);};
  s.test_enable_condition=[provider](const auto& object,bool mark,auto& e){return provider->condition(object,mark,e);};
  s.null_handle_assertion=dh2::world::source_object_handle_null_assertion_v105;
  s.destroy_source=[provider](auto& room,std::string& e){std::shared_ptr<SourceWorldBorrowV61> source;SourceCampaignCandidateBorrowV55 current;return provider->current(source,current,e)&&source_campaign_room_destroy_v106(current.actual_world,room,e);};
  //V93 wraps actual Debug/Character methods; qualified Room destruction keeps
  //its own class barrier/node384/whole GameObjectD2 protocol and failure order.
  out=std::move(s);e.clear();return true;
 };e.clear();return true;
}
}
