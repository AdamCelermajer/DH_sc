#include "source_campaign_noncharacter_virtual_v105.hpp"
#include "source_campaign_frame_services_v106.hpp"
#include "source_campaign_noncharacter_owners_v105.hpp"
#include "source_campaign_object_update_actor_v104.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_fx_v77.hpp"
#include "source_campaign_physical_filter_v105.hpp"
#include "source_campaign_projectile_methods_v112.hpp"
#include "source_campaign_module_rooms_v91.hpp"
#include <module_room_zone_connection_v91.hpp>
#include <canonical_gameobject_graph_v68.hpp>
#include <cstring>
namespace model_renderer {namespace {
using Owners=dh2::loader::ProductionNonCharacterOwnersV67;
template<class Rows>auto receiver(const Rows& rows,std::uintptr_t id)->decltype(rows.front()->owner.get()){
 for(const auto& record:rows)if(record&&record->owner&&record->owner->base().identity()==id)return record->owner.get();return nullptr;
}
bool object(const SourceCampaignCandidateBorrowV55& scope,std::uintptr_t id,dh2::world::ObjectUpdateActorV102& a,const char*& name,std::string& e){
 if(!borrow_source_campaign_object_update_actor_v104(scope,id,a,e)||!a.object.class_name20||!(name=*a.object.class_name20)){if(e.empty())e="Required actual catalog20 for selected nonCharacter virtual";return false;}return true;
}
bool named(const char* value,const char* name){return std::strcmp(value,name)==0;}
}
bool source_campaign_noncharacter_is_zonable_v105(const SourceCampaignCandidateBorrowV55& scope,std::uintptr_t id,bool& value,std::string& e){
 dh2::world::ObjectUpdateActorV102 a;const char* name{};if(!object(scope,id,a,name,e))return false;
 // Exact captured primary vptr+c4. Container/AnimatedDecor branch tail-
 // calls qualified GameObject.MeetCondition38ab60 (literal1).
 if(named(name,"Dummy")||named(name,"SpawnPoint")||named(name,"SpawnSpot")||named(name,"RoomZone")||named(name,"Projectile")||named(name,"LaserTypeProjectile")){value=false;return true;}
 if(named(name,"AnimatedDecor")||named(name,"OpenableContainer")||named(name,"TriggerZone")||named(name,"TriggerZoneExitLevel")||named(name,"TriggerObject")||named(name,"TriggerTrap")||named(name,"SoundEmitter")||named(name,"Door")){value=true;return true;}
 // DestructibleContainer inherits Container::IsZonable (IDA 0x39f534),
 // which delegates to GameObject::MeetCondition (IDA 0x38ab60, true).
 if(named(name,"DestructibleContainer")){value=true;return true;}
 // Module vptr964518+c4 selects inherited GameObject.IsZonable3883b8.
 // Its typed graph borrower above preserves the actual authored byte2ed.
 if(named(name,"Module")||named(name,"Block")||named(name,"Decor")||named(name,"CheckpointZone")||named(name,"QuestMoveInZone")){
  const auto raw=a.byte(0x2ed);if(!raw){e="Required actual GameObject.IsZonable2ed";return false;}value=(*raw^1u)!=0;return true;}
 e=std::string("Required selected nonCharacter IsZonable c4 for ")+name;return false;
}
bool source_campaign_noncharacter_remote_v105(const SourceCampaignCandidateBorrowV55& scope,std::uintptr_t id,bool& value,std::string& e){
 dh2::world::ObjectUpdateActorV102 a;const char* name{};if(!object(scope,id,a,name,e))return false;
 if(named(name,"TriggerZone")||named(name,"TriggerZoneExitLevel")||named(name,"TriggerObject")){
  std::shared_ptr<Owners> owners;if(!borrow_source_campaign_noncharacter_owners_v105(scope,owners,e)||!owners->auxiliary||!owners->auxiliary->source_families_v105())return false;
  const auto& f=*owners->auxiliary->source_families_v105();std::uint8_t* local{};
  if(auto r=receiver(f.triggers(),id))local=r->source_byte(0x3bc);
  else if(auto r=receiver(f.exit_zones(),id))local=r->source_byte(0x3bc);
  else if(auto r=receiver(f.trigger_objects(),id))local=r->source_byte(0x3bc);
  if(!local){e="Required SAME Trigger.local_only3bc";return false;}if(*local){value=false;return true;}
 }
 const auto network=a.integer?a.integer(0x110):nullptr;const auto remote=a.byte(0x118);
 if(!network||(*network==-1&&!remote)){e="Required actual ObjectBase.IsRemotelyUpdated110/118";return false;}value=*network!=-1||*remote;return true;
}
bool source_campaign_noncharacter_sync_visibility_v105(const SourceCampaignCandidateBorrowV55& scope,std::uintptr_t id,std::string& e){
 dh2::world::ObjectUpdateActorV102 a;const char* name{};if(!object(scope,id,a,name,e))return false;
 if(named(name,"Module")||named(name,"Block"))return source_campaign_module_sync_visibility_v94(scope.actual_world,id,e);
 const auto slot=a.pointer(0x2d8);if(!slot){e="Required SAME VisualObject2d8";return false;}if(!*slot)return true;
 const auto visible=a.byte(0x80);if(!visible){e="Required produced actual visible80";return false;}
 bool value=*visible!=0;
 if(value){bool zonable{};if(!source_campaign_noncharacter_is_zonable_v105(scope,id,zonable,e))return false;
  if(zonable){auto zoning=a.byte(0x2ee);if(!zoning){e="Required actual visibility zoning2ee";return false;}if(*zoning){auto inside=a.byte(0x2f0);if(!inside){e="Required actual visibility entered2f0";return false;}if(!*inside)value=false;}}}
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(scope,world,e)||!world->gameobject_graph_v68)return false;
 std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;
 if(!world->gameobject_graph_v68->borrow_visual(*slot,visual,e)||!visual||reinterpret_cast<std::uintptr_t>(visual.get())!=*slot){if(e.empty())e="Required SAME attached VisualObject for source visibility";return false;}
 return visual->set_root_local_visibility_v3(value,e);
}
bool source_campaign_noncharacter_virtual_v105(const SourceCampaignCandidateBorrowV55& scope,std::uintptr_t id,std::uint32_t selector,std::string& e){
 dh2::world::ObjectUpdateActorV102 a;const char* name{};if(!object(scope,id,a,name,e))return false;
 std::shared_ptr<Owners> owners;if(!borrow_source_campaign_noncharacter_owners_v105(scope,owners,e))return false;
 if(named(name,"LightPoint")){
  if(!owners->environment){e="Actual LightPoint catalog owner absent";return false;}
  if(selector==0x2c)return owners->environment->source_frame_update_v113(id,e);
  if(selector==0x44||selector==0x48){std::shared_ptr<dh2::world::CanonicalLightPointV53> actual;if(!owners->environment->source_borrow_v113(id,actual,e))return false;return actual->source_enabled_event_v96(selector==0x44,e);}
  e="Unrecovered selected LightPoint source virtual";return false;
 }
 const auto families=owners->auxiliary?owners->auxiliary->source_families_v105():nullptr;
 if(selector==0x38c710||selector==0x38c69c){
  auto entered=a.byte(0x2f0),zoning=a.byte(0x2ee);auto visual=a.pointer(0x2d8);if(!entered||!zoning||!visual){e="Required SAME ZoneEntered/Exited source cells";return false;}
  const bool entering=selector==0x38c710;*entered=entering?1:0;
  if(*zoning&&*visual){auto visible=a.byte(0x80);if(!visible){e="Required source ZoneEntered visible80";return false;}
   if((!entering||*visible)&&!source_campaign_noncharacter_sync_visibility_v105(scope,id,e))return false;}
  bool zonable{};if(!source_campaign_noncharacter_is_zonable_v105(scope,id,zonable,e))return false;
  const bool update=(zonable&&*zoning)?*entered!=0:true;
  // Captured selected3c: TriggerZone39b26c also clears active7b4 onfalse.
  if(families){if(auto trigger=receiver(families->triggers(),id))return trigger->set_updating(update,e);
   if(auto exit=receiver(families->exit_zones(),id))return exit->set_updating(update,e);}
  auto updating=a.byte(0x85);if(!updating){e="Required selected ObjectBase.setUpdating85";return false;}*updating=update;return true;
 }
 if(selector==0x44||selector==0x48){
  const bool enabled=selector==0x44;auto updating=a.byte(0x85),always=a.byte(0x8a);
  if(!updating||(enabled&&!always)){e="Required SAME ObjectBase Enabled/Disabled85/8a";return false;}
  std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  if(!borrow_source_campaign_object_base_v77(scope,id,pin,base,e)||!base)return false;
  if(!enabled)*updating=0;if(!base->store_byte(0x80,enabled?*always:0,e))return false;
  if(!source_campaign_noncharacter_sync_visibility_v105(scope,id,e))return false;
  if(enabled)*updating=1;
  auto& flags=base->runtime().object.motion.object_flags;if(enabled)flags|=8u;else flags&=~8u;
  const auto physical=a.pointer(0x2dc);if(!physical){e="Required SAME GameObject physical2dc";return false;}
  if(*physical){dh2::physical::NativePhysicalFilterBorrowV1 filter;std::shared_ptr<void> body_pin;
   if(!borrow_source_campaign_physical_filter_v105(scope,id,body_pin,filter,e)||!body_pin||!dh2::physical::native_physical_filter_v1(filter,enabled,e))return false;}
  auto disabled=a.byte(0x373);if(!disabled){e="Required SAME GameObject disabled373";return false;}*disabled=enabled?0:1;
  if(families)if(auto door=receiver(families->doors(),id))return door->source_enabled_tail_v105(enabled,e);
  return true;
 }
 if(selector!=0x2c){e="Unknown selected nonCharacter source method";return false;}
 if(named(name,"Projectile")||named(name,"LaserTypeProjectile"))return source_campaign_projectile_update_v112(scope,id,e);
 if(named(name,"Door")){
  // Whole selected Door.Update3e7560: idle sound FIRST, then a fresh visual
  // slot read/Sync, then RequireOnlineUpdate. No inherited runtime/collision.
  std::shared_ptr<void> base_pin,service_pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  dh2::loader::GameObjectSourceFrameServicesV74 frame;
  if(!borrow_source_campaign_object_base_v77(scope,id,base_pin,base,e)||!base_pin||!base||
    !borrow_source_campaign_frame_services_v106(scope,id,service_pin,frame,e)||!service_pin)return false;
  auto raw=base->integer(0x370);if(!raw){e="Required actual Door idle sound370";return false;}
  const auto bits=static_cast<std::uint16_t>(*raw);const auto sound=bits<32768?static_cast<std::int32_t>(bits):static_cast<std::int32_t>(bits)-65536;
  if(sound>=0&&(!frame.idle_sound||!frame.idle_sound(*base,static_cast<std::int16_t>(sound),e))){if(e.empty())e="Required actual Door.UpdateIdleSound";return false;}
  auto slot=base->pointer(0x2d8);if(!slot){e="Required actual Door visual2d8";return false;}
  const auto captured=*slot;
  if(captured){std::shared_ptr<SourceWorldBorrowV61> world;std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;
   if(!borrow_source_campaign_condition_world_v70(scope,world,e)||!world->gameobject_graph_v68||
     !world->gameobject_graph_v68->borrow_visual(captured,visual,e)||!visual||reinterpret_cast<std::uintptr_t>(visual.get())!=captured){if(e.empty())e="Required captured SAME Door VisualObject.Sync";return false;}
   if(!visual->sync(e))return false;
  }
  return dh2::world::gameobject_require_online_update_v5(*base,frame.online,e);
 }
 if(owners->generic){if(auto room=owners->generic->source_room_v105(id))return room->receiver&&room->receiver->source_update_v104(e);
  if(auto traps=owners->generic->source_traps_v105())if(auto trap=receiver(traps->trigger_traps(),id))return trap->update(e);}
 if(named(name,"RoomZone")){std::shared_ptr<SourceWorldBorrowV61> world;
  if(!borrow_source_campaign_condition_world_v70(scope,world,e)||!world->module_room_zones_v91){e="Required generated RoomZone source update owner";return false;}
  return world->module_room_zones_v91->update(id,e);}
 if(families){
  if(auto trigger=receiver(families->triggers(),id))return trigger->update(e);
  if(auto trigger=receiver(families->trigger_objects(),id))return trigger->update(e);
  if(auto exit=receiver(families->exit_zones(),id))return exit->update(e);
  if(auto sound=receiver(families->sound_emitters(),id))return sound->update(e);
 }
 if(named(name,"Decor")||named(name,"AnimatedDecor")||named(name,"Dummy")||named(name,"SpawnPoint")||named(name,"SpawnSpot")||named(name,"CheckpointZone")||named(name,"QuestMoveInZone")||named(name,"OpenableContainer")||named(name,"DestructibleContainer")){
  std::shared_ptr<SourceWorldBorrowV61> world;return borrow_source_campaign_condition_world_v70(scope,world,e)&&update_source_campaign_noncharacter_v104(world,id,e);
 }
 e=std::string("Required original selected virtual2c for ")+name;return false;
}
}
