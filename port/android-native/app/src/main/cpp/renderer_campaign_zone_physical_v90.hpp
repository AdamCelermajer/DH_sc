#pragma once
#include <canonical_zone_physical_v82.hpp>
#include <algorithm>
#include <canonical_character_candidate_v60.hpp>
#include "renderer_character_campaign_v62.hpp"
#include <loot_root_publishers_v49.hpp>
#include <world_item_live_owner_v5.hpp>
namespace model_renderer {
// Typed projections over existing C1 owners/associations. No callback address
// is cast to ObjectBase, no NULL owner is guessed, and no peer registry exists.
template<class Self>
void bind_campaign_zone_physical_v90(std::weak_ptr<Self> weak,dh2::world::CanonicalZonePhysicalServicesV82& out){
 auto original_peer=out.peer_owner;
 out.peer_owner=[weak,original_peer](void* address,std::uintptr_t& id,std::string& e){
  auto self=weak.lock();if(!self||!self->current(e))return false;
  const auto zone=self->zone_resources.find(reinterpret_cast<std::uintptr_t>(address));
  if(zone!=self->zone_resources.end()){auto prefix=zone->second.lock();
   if(!prefix||!prefix->receiver||prefix->released||!prefix->source_owner8){e="Retired/unproduced SAME Zone owner8";return false;}
   id=*prefix->source_owner8;e.clear();return true;}
  if(self->physical_associations&&self->physical_associations->recognizes(address)){
   dh2::character::LootPhysicalPeerBorrowV44 actual;if(!self->physical_associations->peer(address,actual,e))return false;
   id=actual.object?actual.object->identity:0;return true; // actual registered nullable8 only
  }
  for(const auto& entry:self->openable_graphs_v89){auto graph=entry.second.lock();if(!graph)continue;
   bool handled{};std::shared_ptr<void> pin;
   if(!graph->physical_peer_v90(address,id,pin,handled,e))return false;if(handled)return true;}
  auto objects=self->scope.objects.lock();auto world=self->scope.actual_world.lock();
  if(!objects||!world){e="Released actual physical peer manager";return false;}
  for(auto actor:objects->characters()){
   SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,actor,actual,e))return false;
   const auto record=actual.character;
   if(record&&record->physical_owner_v62&&record->physical_owner_v62->world_object().context==address){
    if(!record->actor||!record->actor->object){e="Missing actual POCharacter owner8 producer";return false;}id=record->actor->object->identity;e.clear();return true;}
  }
  if(auto items=self->native.source.containers.actual_items.lock()){
   std::int32_t key{};const dh2::world::CanonicalObjectBorrowV1* object{};bool found=objects->source_ordered_begin_v38(key,object);
   while(found){if(object&&object->type_f4&&*object->type_f4==3){auto graph=items->graph(object->identity);
     if(graph&&graph->physical().world_object_v90().context==address){id=graph->receiver_v4().base().identity();e.clear();return true;}}
    found=objects->source_ordered_next_v38(key,key,object);}
  }
  // A native supplied ownerless/other physical family remains its genuine
  // typed source projection. Unknown userdata cannot masquerade as owner0.
  if(original_peer)return original_peer(address,id,e);
  e="Unknown actual PhysicalWorld callback context; required typed owner8 producer";return false;
 };
 auto original_visible=out.peer_visible80;
 out.peer_visible80=[weak,original_visible](std::uintptr_t id,std::uint8_t& value,std::string& e){
  auto self=weak.lock();if(!self||!self->current(e))return false;std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  std::string ignored;if(self->borrow(id,pin,base,ignored)){
   if(!base->lifecycle().visible_written){e="Unproduced actual non-Character visible80";return false;}
   const auto byte=base->byte(0x80);if(!byte){e="Missing SAME visible80 cell";return false;}value=*byte;e.clear();return true;
  }
  auto objects=self->scope.objects.lock();auto world=self->scope.actual_world.lock();
  if(objects&&world&&std::find(objects->characters().begin(),objects->characters().end(),id)!=objects->characters().end()){
   SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,e)||!actual.character||!actual.character->actor)return false;
   auto actor=actual.character->actor;dh2::world::GameObjectInitializationFieldsV62 fields;
   if(!actor->inherited_initialization_fields_v62(actor,fields,e)||!fields.byte||!actor->source_bool_field(0x80)){if(e.empty())e="Unproduced SAME Character visible80";return false;}
   auto byte=fields.byte(0x80);if(!byte){e="Missing SAME Character byte80";return false;}value=*byte;e.clear();return true;
  }
  if(auto items=self->native.source.containers.actual_items.lock()){auto item=items->factory().find(id);
   if(item){auto& base=item->base();if(!base.lifecycle().visible_written){e="Unproduced SAME Item visible80";return false;}auto byte=base.byte(0x80);if(!byte){e="Missing SAME Item byte80";return false;}value=*byte;e.clear();return true;}}
  if(original_visible)return original_visible(id,value,e);
  e="Required actual peer visible80 owner";return false;
 };
 auto original_handle=out.resolve_peer_handle;
 out.resolve_peer_handle=[weak,original_handle](std::uintptr_t id,std::uintptr_t& result,std::string& e){
  auto self=weak.lock();if(!self||!self->current(e))return false;auto objects=self->scope.objects.lock();auto world=self->scope.actual_world.lock();
  if(!objects||!world){e="Released SAME peer handle manager";return false;}
  std::shared_ptr<void> pin;dh2::target_providers::Handle16* handle{};dh2::world::CanonicalGameObjectBaseOwnerV1* base{};std::string ignored;
  if(self->borrow(id,pin,base,ignored))handle=&base->shared_handle();
  else if(std::find(objects->characters().begin(),objects->characters().end(),id)!=objects->characters().end()){
   SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,e)||!actual.character||!actual.character->actor)return false;
   auto canonical=actual.character->actor->canonical(actual.character);pin=actual.character;handle=canonical.shared_handle;
  }else if(auto items=self->native.source.containers.actual_items.lock()){auto item=items->factory().find(id);if(item){pin=item;handle=&item->base().shared_handle();}}
  if(!pin||!handle){if(original_handle)return original_handle(id,result,e);e="Required SAME physical peer GetHandle owner";return false;}
  auto source=*handle;const dh2::world::CanonicalObjectBorrowV1* object{};
  if(!objects->resolve_handle_v4(source,false,object,{},e))return false;
  result=object?object->identity:0;e.clear();return true; // original NULL GetObject skips virtual
 };
}
}
