#include "source_campaign_object_update_actor_v104.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "model_renderer.hpp"
#include "source_campaign_fx_v77.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_character_fsm_v101.hpp"
#include "source_campaign_noncharacter_owners_v105.hpp"
#include "source_campaign_module_rooms_v91.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <canonical_gameobject_base_owner_v1.hpp>
#include <character_design_services.hpp>
#include <character_update_pointers_v105.hpp>
namespace model_renderer {
bool source_campaign_character_update_pointers_v105(const SourceCampaignCandidateBorrowV55& scope,std::uintptr_t id,std::string& e){
 SourceCampaignCharacterBorrowV62 actual;
 if(!borrow_source_campaign_character_v62(scope.actual_world,id,actual,e)||!actual.character->actor)return false;
 auto& actor=*actual.character->actor;auto ai=actor.source_ai_pointers_v105();
 auto& kill=actor.kill_fields_v42();
 if(!ai||!kill.produced||!actor.object){e="Required produced SAME CharAI/Character pointer cells for UpdateAIPointers";return false;}
 return dh2::character::character_update_pointers_v105(actor.object->target,*ai,actor.source_ooi14a4,kill.killer144c,
  [&scope](std::uintptr_t other,const std::uint8_t*& value,std::string& error){
   dh2::world::ObjectUpdateActorV102 record;
   if(!borrow_source_campaign_object_update_actor_v104(scope,other,record,error))return false;
   value=record.byte?record.byte(0x81):nullptr;return value!=nullptr;
  },e);
}
bool borrow_source_campaign_character_room_v104(const SourceCampaignCandidateBorrowV55& scope,std::uintptr_t id,dh2::world::RoomObjectBorrowV104& out,std::string& e){
 SourceCampaignCharacterBorrowV62 actual;if(!scope.actual_world||!borrow_source_campaign_character_v62(scope.actual_world,id,actual,e)||!actual.character->actor)return false;
 dh2::world::GameObjectInitializationFieldsV62 fields;if(!actual.character->actor->inherited_initialization_fields_v62(actual.character,fields,e))return false;
 dh2::world::RoomObjectBorrowV104 next;next.owner=actual.character;next.identity=id;next.position160=actual.character->actor->source_position160_v7();next.room2f4=fields.pointer(0x2f4);next.initial_room2ef=fields.byte(0x2ef);
 if(!next.position160||!next.room2f4||!next.initial_room2ef){e="Required SAME source Character room fields";return false;}
 std::weak_ptr<void> world=scope.actual_world;next.is_zonable_c4=[world,id](bool& value,std::string& e){auto actual=world.lock();return actual&&source_campaign_character_is_zonable_v104(actual,id,value,e);};out=std::move(next);return true;
}
bool borrow_source_campaign_object_update_actor_v104(const SourceCampaignCandidateBorrowV55& scope,std::uintptr_t id,dh2::world::ObjectUpdateActorV102& out,std::string& e){
 if(!scope.actual_world||!scope.objects||!id){e="Required SAME ObjectManager update receiver scope";return false;}
 const dh2::world::CanonicalObjectBorrowV1* object{};std::int32_t key{};
 bool found=scope.objects->source_ordered_begin_v38(key,object);while(found&&(!object||object->identity!=id))found=scope.objects->source_ordered_next_v38(key,key,object);
 if(!found||!object||!object->lease||!object->shared_handle){e="Required published actual ObjectManager receiver";return false;}
 dh2::world::ObjectUpdateActorV102 next;next.object=*object;std::uintptr_t character{};
 if(!object->as_character||!object->as_character(object->context,character,e))return false;
 if(character){SourceCampaignCharacterBorrowV62 actual;if(character!=id||!borrow_source_campaign_character_v62(scope.actual_world,id,actual,e)||!actual.character->actor)return false;
  dh2::world::GameObjectInitializationFieldsV62 fields;auto actor=actual.character->actor;
  if(!actor->inherited_initialization_fields_v62(actual.character,fields,e))return false;
  next.byte=fields.byte;next.pointer=fields.pointer;next.integer=fields.integer;
  next.word=[actor](std::uint32_t offset)->std::uint32_t*{return offset==0x114?actor->source_network114_v70():nullptr;};
 }else if(object->type_f4&&*object->type_f4==5){
  // Module/Block retain their actual base in ModuleGraph, not V68.
  // Borrow the published SAME record; never register a second base owner.
  std::shared_ptr<SourceWorldBorrowV61> world;std::shared_ptr<void> pin;dh2::world::CanonicalModuleV1* module{};
  if(!borrow_source_campaign_condition_world_v70(scope,world,e)||
    !borrow_source_campaign_module_v91(scope.actual_world,id,pin,module,e)||!pin||!module||
    module->base().identity()!=id||pin.owner_before(object->lease)||object->lease.owner_before(pin)){
   if(e.empty())e="Required SAME published Module graph receiver";return false;
  }
  auto* base=&module->base();
  next.byte=[pin,base](auto offset){return base->byte(offset);};next.pointer=[pin,base](auto offset){return base->pointer(offset);};next.integer=[pin,base](auto offset){return base->integer(offset);};
  next.word=[](std::uint32_t)->std::uint32_t*{return nullptr;};
 }else if(object->type_f4&&*object->type_f4==19){
  std::shared_ptr<dh2::loader::ProductionNonCharacterOwnersV67> owners;std::shared_ptr<dh2::world::CanonicalLightPointV53> light;
  if(!borrow_source_campaign_noncharacter_owners_v105(scope,owners,e)||!owners->environment||!owners->environment->source_borrow_v113(id,light,e))return false;
  next.byte=[light](auto offset){std::string e;return light->source_save_byte_v89(offset,e);};
  next.pointer=[light](auto offset){return light->source_pointer_v113(offset);};next.integer=[light](auto offset){return light->source_integer_v113(offset);};
  next.word=[](std::uint32_t)->std::uint32_t*{return nullptr;};
 }else{std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  if(!borrow_source_campaign_object_base_v77(scope,id,pin,base,e)||!pin||!base||base->identity()!=id)return false;
  next.byte=[pin,base](auto offset){return base->byte(offset);};next.pointer=[pin,base](auto offset){return base->pointer(offset);};next.integer=[pin,base](auto offset){return base->integer(offset);};
  //Native generic114 is still its legacy uintptr scalar projection. Never
  //alias its64-bit storage as uint32; a positive remote family requires its
  //actual typed32-bit network elapsed producer before this pointer loan.
  next.word=[](std::uint32_t)->std::uint32_t*{return nullptr;};
 }
 out=std::move(next);return true;
}
bool source_campaign_resolve_update_handle_v104(const SourceCampaignCandidateBorrowV55& scope,const dh2::world::ObjectUpdateActorV102& actor,std::uintptr_t& id,std::string& e){
 if(!scope.objects||!actor.object.shared_handle||!actor.object.lease){e="Required source GetHandle receiver";return false;}
 auto local=*actor.object.shared_handle;const dh2::world::CanonicalObjectBorrowV1* resolved{};
 if(!scope.objects->resolve_handle_v4(local,false,resolved,{},e))return false;id=resolved?resolved->identity:0;return true;
}
bool source_campaign_object_update_debug_v104(const SourceCampaignCandidateBorrowV55& scope,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(scope,world,e)||!world||!world->debug||!world->debug_files)return false;
 for(const char* name:{"TraceUpdateGameObjectOnce","Give50Potions"}){
  if(dh2_character_debug_load(world->debug.get(),world->debug_files)!=1||dh2_character_debug_set_v102(world->debug.get(),name,0,world->debug_files)!=1){e="Required actual ObjectManager Debug.SetSwitch source tail";return false;}}
 return true;
}
}
