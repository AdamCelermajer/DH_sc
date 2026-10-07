#include "world_loot_canonical_bindings_v44.hpp"
#include <stdexcept>

namespace dh2::character {
bool world_loot_design_constant_v44(const dh2_script_design_bindings* source,
 const char* group,const char* key,std::int32_t& out,std::string& e){
 if(!source||!source->context||!source->lookup||source->reserved||!group||!key){
  e="Required SAME loot PyDataConstants source receiver";return false;
 }
 if(source->lookup(source->context,0,group,key,&out)){
  e="Actual loot PyDataConstants lookup delivery failed";return false;
 }
 return true;
}
namespace {
bool required(bool value,const char* name,std::string& e){
 if(value)return true;
 e=std::string("Required SAME source loot V44 ")+name;return false;
}
}
WorldLootCanonicalBindingsV44::WorldLootCanonicalBindingsV44(
 world::CanonicalObjectManagerV1& manager,player::PlayerManagerOwnerV1& players,
 loader::CanonicalGSLevelGlobalSlotV1 gs,WorldLootCanonicalServicesV44 s)
 :manager_(manager),players_(players),gs_(std::move(gs)),services_(std::move(s)){
 if(!services_.application_lease)throw std::invalid_argument("Required source loot V44 Application lifetime");
}
world::CanonicalItemFactoryServicesV2 WorldLootCanonicalBindingsV44::factory_services(
 world::CanonicalItemFactoryServicesV2 upstream){
 upstream_factory_=std::move(upstream);
 auto s=upstream_factory_;s.context=this;s.resolve=resolve;
 s.test_enable_condition=condition;s.unknown_type_debug=debug;
 // Item operation/notification callbacks have their own source context.
 // Preserve it separately from the factory Handle/Spawn context.
 return s;
}
bool WorldLootCanonicalBindingsV44::resolve(void* raw,target_providers::Handle16& h,
 bool assert_null,const world::CanonicalObjectBorrowV1*& out,std::string& e){
 auto& self=*static_cast<WorldLootCanonicalBindingsV44*>(raw);
 return self.manager_.resolve_handle_v4(h,assert_null,out,[&self](std::string& error){
  if(!required(bool(self.services_.null_handle_assertion),"ObjectHandle NULL assertion",error))return false;
  return self.services_.null_handle_assertion(self.services_.context,error);
 },e);
}
bool WorldLootCanonicalBindingsV44::canonical(std::uintptr_t id,
 const world::CanonicalObjectBorrowV1*& out,std::string& e){
 auto* items=services_.items?services_.items(services_.context):nullptr;
 auto item=items?items->factory().find(id):nullptr;
 if(!required(bool(item),"published Item receiver",e))return false;
 auto& h=item->base().shared_handle();
 if(!resolve(this,h,false,out,e))return false;
 return required(out&&out->identity==id&&out->lease&&out->shared_handle==&h&&
  out->type_f4&&*out->type_f4==3,"canonical Item Handle/type3 authority",e);
}
bool WorldLootCanonicalBindingsV44::condition(void* raw,
 const world::CanonicalObjectBorrowV1& object,bool mark,std::string& e){
 return static_cast<WorldLootCanonicalBindingsV44*>(raw)->test_enable_condition(object,mark,e);
}
bool WorldLootCanonicalBindingsV44::test_enable_condition(
 const world::CanonicalObjectBorrowV1& object,bool mark,std::string& e){
 const world::CanonicalObjectBorrowV1* actual{};
 if(!canonical(object.identity,actual,e))return false;
 if(!required(actual==&object,"Spawn resolved receiver",e))return false;
 auto* items=services_.items(services_.context);
 auto item=items->factory().find(object.identity);auto& b=item->base();
 ConditionScope scope{*this,*item};
 world::ObjectEnableConditionBorrowV2 fields{b.byte(0x8a),b.integer(0xec),
  b.byte(0xf1),b.pointer(0xa8),b.byte(0xac)};
 world::ObjectEnableConditionServicesV2 source{&scope,local_profile,level_condition,
  condition_truth,enabled_event};
 bool enabled{};return world::object_test_enable_condition_v2(fields,source,mark,enabled,e);
}
bool WorldLootCanonicalBindingsV44::local_profile(void* raw,bool& character,
 bool& save,std::uint8_t& byte14,std::string& e){
 auto& self=static_cast<ConditionScope*>(raw)->owner;
 player::PlayerInfoFieldsV1* info{};
 if(!self.players_.get_local_player(0,true,info,e))return false;
 if(!required(info!=nullptr,"GetLocalPlayer(0,true) record",e))return false;
 character=info->character660!=0;save=false;byte14=0;
 if(!character)return true; // actual NULL-character source branch
 if(!required(bool(self.services_.character_save),"Character+14e8 Save borrow",e))return false;
 LootCharacterSaveBorrowV44 borrow;
 if(!self.services_.character_save(self.services_.context,info->character660,borrow,e))return false;
 if(!required(bool(borrow.character_lease),"local Character lifetime",e))return false;
 if(!borrow.save)return true; // actual NULL Save, source early return
 if(!required(bool(borrow.save_lease),"local Save lifetime",e))return false;
 save=true;byte14=borrow.save->source_quest_sync_ready14_v3();return true;
}
bool WorldLootCanonicalBindingsV44::current_level(LootCurrentLevelV23& out,std::string& e){
 loader::CanonicalCurrentLevelBorrowV1 borrow;
 if(!loader::borrow_current_canonical_level_v1(gs_,borrow,e))return false;
 LootCurrentLevelV23 next;
 if(borrow){
  loader::CanonicalLevelContextV1::LoadingFieldsV26 fields;
  // Validates completed C1, independent of loading phase. Legal phase0 is
  // published by the genuine GS constructor; neither this nor loot writes38.
  if(!borrow.level()->loading_fields_v26(fields,e))return false;
  const auto& c1=borrow.level()->constructor_fields_v3();
  next.identity=borrow.identity();next.difficulty118=&c1.mode118;
  next.phase130=fields.state130;next.receiver_lease=borrow.level();
 }
 out=std::move(next);return true;
}
bool WorldLootCanonicalBindingsV44::current_level_callback(void* raw,LootCurrentLevelV23& out,std::string& e){
 return static_cast<WorldLootCanonicalBindingsV44*>(raw)->current_level(out,e);
}
bool WorldLootCanonicalBindingsV44::level_condition(void* raw,bool& present,std::int32_t& difficulty,std::string& e){
 auto& self=static_cast<ConditionScope*>(raw)->owner;LootCurrentLevelV23 borrow;
 if(!self.current_level(borrow,e))return false;
 present=borrow.identity!=0;difficulty=present?*borrow.difficulty118:0;return true;
}
bool WorldLootCanonicalBindingsV44::condition_truth(void* raw,std::uintptr_t condition_id,bool& result,std::string& e){
 auto& self=static_cast<ConditionScope*>(raw)->owner;
 if(!required(bool(self.services_.condition_is_true),"compiled Condition::IsTrue",e))return false;
 return self.services_.condition_is_true(self.services_.context,condition_id,result,e);
}
bool WorldLootCanonicalBindingsV44::enabled_event(void* raw,bool enabled,std::string& e){
 auto& scope=*static_cast<ConditionScope*>(raw);auto& self=scope.owner;
 auto* live=self.services_.items(self.services_.context);
 auto* graph=live?live->graph(scope.item.base().identity()):nullptr;
 if(graph)return graph->source_enabled_event_v44(enabled,e);
 // Spawn TestEnableCondition precedes InitOnce/graph construction. Only the
 // exact constructor-NULL visual/physical branch can run without the graph.
 auto& base=scope.item.base();auto* visual=base.pointer(0x2d8);auto* body=base.pointer(0x2dc);
 if(!required(visual&&body&&!*visual&&!*body,"constructor-NULL enabled event or live Item graph",e))return false;
 if(enabled){
  if(!base.store_byte(0x80,base.lifecycle().enabled8a?1:0,e))return false;
  base.lifecycle().updating85=1;
 }else{
  base.lifecycle().updating85=0;
  if(!base.store_byte(0x80,0,e))return false;
 }
 auto& flags=base.runtime().object.motion.object_flags;
 if(enabled)flags|=8u;else flags&=~8u;
 base.lifecycle().disabled373=enabled?0:1;return true;
}
bool WorldLootCanonicalBindingsV44::debug(void* raw,const char* type,std::string& e){
 auto& self=*static_cast<WorldLootCanonicalBindingsV44*>(raw);
 if(!required(bool(self.services_.unknown_type_debug),"unknown-type Debug continuation",e))return false;
 return self.services_.unknown_type_debug(self.services_.context,type,e);
}
bool WorldLootCanonicalBindingsV44::validate_peer(const LootPhysicalPeerBorrowV44& p,std::string& e){
 if(!required(p.object&&p.receiver_lease&&p.visible80&&p.object->shared_handle,
  "physical peer canonical fields/lifetime",e))return false;
 auto* published=manager_.object(p.object->shared_handle->key);
 return required(published==p.object&&published->identity&&published->lease,
  "physical peer SAME ObjectManager publication",e);
}
bool WorldLootCanonicalBindingsV44::peer(void* address,LootPhysicalPeerBorrowV44& out,std::string& e){
 if(!required(bool(services_.physical_peer),"mixed-world physical peer producer",e))return false;
 LootPhysicalPeerBorrowV44 next;
 if(!services_.physical_peer(services_.context,address,next,e))return false;
 if(!next.object){
  // A recognized PhysicalObject with actual owner8=NULL is source valid.
  if(!required(bool(next.receiver_lease),"NULL-owner physical peer lifetime",e))return false;
 }else if(!validate_peer(next,e))return false;
 out=std::move(next);return true;
}
bool WorldLootCanonicalBindingsV44::physical_services(RetainedWorldItemObjectV1& item,
 WorldItemPhysicalServicesV2& out,std::string& e){
 const world::CanonicalObjectBorrowV1* actual{};
 if(!canonical(item.base().identity(),actual,e))return false;
 auto* live=services_.items(services_.context);
 if(!required(live->factory().find(actual->identity).get()==&item,"physical Item receiver",e))return false;
 // Preserve source Debug and UpdatePF installed by the upstream graph/V5.
 out.owner=services_.application_lease;
 out.destroy_previous=[this](std::uintptr_t id,std::string& error){
  if(!required(bool(services_.destroy_previous_physical),"previous PhysicalObject destructor",error))return false;
  return services_.destroy_previous_physical(services_.context,id,error);
 };
 out.resolve=[this,&item](std::uintptr_t id,std::uintptr_t& object,std::uint32_t& type,std::string& error){
  if(!required(id==item.base().identity(),"POItem GetHandle receiver",error))return false;
  const world::CanonicalObjectBorrowV1* actual{};
  if(!resolve(this,item.base().shared_handle(),false,actual,error))return false;
  if(!actual){object=0;type=0;return true;}
  if(!required(actual->type_f4!=nullptr,"POItem canonical type_f4",error))return false;
  object=actual->identity;type=*actual->type_f4;return true;
 };
 out.peer_owner=[this](void* address,std::uintptr_t& owner,std::string& error){
  LootPhysicalPeerBorrowV44 p;if(!peer(address,p,error))return false;
  owner=p.object?p.object->identity:0;return true;
 };
 // Visibility and OOI are borrowed anew by canonical identity; no stale
 // captured pointer survives a callback or canonical receiver removal.
 out.peer_visible=[this](std::uintptr_t id,std::uint8_t& visible,std::string& error){
  if(!required(bool(services_.character_fields),"peer visible80 field producer",error))return false;
  LootPhysicalPeerBorrowV44 p;
  if(!services_.character_fields(services_.context,id,p,error)||!validate_peer(p,error))return false;
  if(!required(p.object->identity==id,"peer visible80 identity",error))return false;
  visible=*p.visible80;return true;
 };
 out.as_character=[this](std::uintptr_t id,std::uintptr_t& character,std::string& error){
  if(!required(bool(services_.character_fields),"peer Character cast producer",error))return false;
  LootPhysicalPeerBorrowV44 p;
  if(!services_.character_fields(services_.context,id,p,error)||!validate_peer(p,error))return false;
  if(!required(p.object->identity==id&&p.object->as_character,"peer actual AsCharacter receiver",error))return false;
  return p.object->as_character(p.object->context,character,error);
 };
 out.character_ooi=[this](std::uintptr_t id,std::uintptr_t& ooi,std::string& error){
  if(!required(bool(services_.character_fields),"Character OOI14a4 producer",error))return false;
  LootPhysicalPeerBorrowV44 p;
  if(!services_.character_fields(services_.context,id,p,error)||!validate_peer(p,error))return false;
  if(!required(p.object->identity==id&&p.character_ooi14a4,"SAME Character OOI14a4",error))return false;
  ooi=*p.character_ooi14a4;return true;
 };
 return true;
}
bool WorldLootCanonicalBindingsV44::outer_item(const WorldItemRequestV1& q,
 std::int32_t& result,bool& handled,std::string& e){
 handled=q.operation==WorldItemOperationV1::is_moving;if(!handled)return true;
 const world::CanonicalObjectBorrowV1* item{};
 if(!canonical(q.object,item,e))return false;
 if(!required(q.character&&bool(services_.character_state),"SM_IsMoving(false) live Character producer",e))return false;
 const std::int32_t* state_id{};std::shared_ptr<void> lease;
 if(!services_.character_state(services_.context,q.character,state_id,lease,e))return false;
 if(!required(bool(lease),"live Character state machine lifetime",e))return false;
 // Whole SM_IsMoving3c029c..2cc, called with false by Item3ec0d0.
 // The source state19 returns !mode; this is not an animation predicate.
 const auto state=state_id?*state_id:-1;result=state==4||state==19;return true;
}
}
