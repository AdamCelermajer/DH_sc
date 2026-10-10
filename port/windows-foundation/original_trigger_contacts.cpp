#include "original_trigger_contacts.hpp"
#include <cmath>
#include <exception>
namespace dh::foundation {
OriginalTriggerPeerProvider original_trigger_peer_provider(OriginalTriggerActorLookup actors,OriginalTriggerPhysicalLookup physical){
 return [actors=std::move(actors),physical=std::move(physical)](std::uintptr_t id,dh2::world::ZonePeerBorrowV83&out,std::string&e){
  if(!id||!actors){e="Required original trigger actor receiver lookup";return false;}
  OriginalTriggerActorBorrow actor;if(!actors(id,actor,e))return false;
  if(!actor.receiver||actor.identity!=id||!actor.position160||!actor.absolute12c||!actor.physical2dc){e="Required SAME live actor position160/absolute12c/physical2dc fields";return false;}
  dh2::world::ZonePeerBorrowV83 result;result.receiver=actor.receiver;result.identity=id;
  result.position160=actor.position160;result.absolute12c=actor.absolute12c;result.physical2dc=actor.physical2dc;
  // NULL source physical is legitimate: IsInside returns false before radius.
  // Resolve its actual receiver only when source code reaches GetRadius.
  result.physical_radius=[actor,physical](float&radius,std::string&error){
   const auto identity=*actor.physical2dc;
   if(!identity||!physical){error="Required actual source PhysicalObject.GetRadius receiver";return false;}
   OriginalTriggerPhysicalBorrow body;if(!physical(identity,body,error))return false;
   if(!body.receiver||body.identity!=identity||!body.native||!body.native->body||*actor.physical2dc!=identity){error="Required SAME initialized physical2dc native body";return false;}
   // Original GetRadius46e750; NativeBody radius stores physics units.
   radius=body.native->radius*100.f;error.clear();return true;
  };
  out=std::move(result);e.clear();return true;
 };
}
std::shared_ptr<OriginalTriggerContacts::Slot> OriginalTriggerContacts::slot(const std::string&key,std::string&e)const{
 auto i=slots_.find(key);if(i==slots_.end()||!i->second->owner||!i->second->runtime){e="Unbound original TriggerZone occurrence: "+key;return {};}return i->second;
}
bool OriginalTriggerContacts::peer_services(const std::shared_ptr<Slot>&s,dh2::world::ZoneCollisionServicesV83&out,std::string&e){
 if(!s||!s->owner||!s->runtime||!s->peer){e="Original trigger owner/runtime/body provider unavailable";return false;}
 out=s->collision;out.peer=[s](std::uintptr_t id,dh2::world::ZonePeerBorrowV83&peer,std::string&error){
  if(!s->peer(id,peer,error))return false;
  // These are live SAME peer/body borrows, never synthetic center/radius bounds.
  if(!peer.receiver||peer.identity!=id||!peer.absolute12c||!peer.physical2dc){error="Original trigger requires actual peer body slot and absolute AABB backing";return false;}
  return true;
 };return true;
}
bool OriginalTriggerContacts::prepare(const std::string&key,std::optional<std::int32_t>room,OriginalTriggerPeerProvider peer,
 dh2::world::TriggerZoneServicesV22&services,std::string&e,dh2::world::ZoneCollisionServicesV83 collision){
 if(key.empty()||slots_.count(key)||!room||!peer){e="Original trigger needs unique instance key, captured constructor room and body provider";return false;}
 auto s=std::make_shared<Slot>();s->room=*room;s->peer=std::move(peer);s->collision=std::move(collision);std::weak_ptr<Slot> weak=s;
 services.touching=[weak](std::uintptr_t id,bool&hit,std::string&error){auto p=weak.lock();dh2::world::ZoneCollisionServicesV83 source;if(!peer_services(p,source,error))return false;return dh2::world::zone_is_touching_v83(p->owner->base(),id,source,hit,error);};
 services.zone_inside=[weak](std::uintptr_t id,bool&inside,std::string&error){auto p=weak.lock();dh2::world::ZoneCollisionServicesV83 source;if(!peer_services(p,source,error))return false;return dh2::world::zone_is_inside_v83(p->owner->base(),p->owner->source_colzone384_v83(),id,source,inside,error);};
 // Existing actual mesh-inside branch must not be replaced by an AABB guess.
 // A positive _colzone owner reaches the canonical kernel's explicit failure
 // unless the caller supplies its real selector/ray mesh_inside provider.
 slots_.emplace(key,s);order_.push_back(key);e.clear();return true;
}
bool OriginalTriggerContacts::attach(const std::string&key,std::shared_ptr<dh2::world::CanonicalTriggerZoneV22>owner,
 std::shared_ptr<void>runtime,std::string&e){
 auto i=slots_.find(key);if(i==slots_.end()||i->second->owner||!owner||!runtime){e="Invalid original trigger occurrence attachment";return false;}
 if(owner->base().room64()!=i->second->room){e="Canonical trigger room64 differs from captured constructor source";return false;}
 for(const auto&entry:slots_)if(entry.second->owner&&entry.second->owner.get()==owner.get()){e="Repeated occurrence must retain a distinct canonical TriggerZone owner";return false;}
 i->second->owner=std::move(owner);i->second->runtime=std::move(runtime);e.clear();return true;
}
bool OriginalTriggerContacts::source_dimensions(const std::string&key,std::optional<std::array<float,3>>authored,std::string&e){auto s=slot(key,e);if(!s)return false;auto d=authored.value_or(std::array<float,3>{200.f,200.f,200.f});for(float v:d)if(!std::isfinite(v)||v<0){e="Invalid source trigger dimensions";return false;}return s->owner->write_vector3(0x374,d,e);}
bool OriginalTriggerContacts::initialize(const std::string&key,std::string&e){auto s=slot(key,e);return s&&s->owner->init_post(e);}
bool OriginalTriggerContacts::update(const std::string&key,std::string&e){auto s=slot(key,e);return s&&s->owner->update(e);}
bool OriginalTriggerContacts::update_all(std::string&e){for(const auto&key:order_)if(!update(key,e))return false;e.clear();return true;}
bool OriginalTriggerContacts::collision_begin(const std::string&key,std::uintptr_t id,std::string&e){auto s=slot(key,e);return s&&s->owner->collision_begin(id,e);}
bool OriginalTriggerContacts::collision_end(const std::string&key,std::uintptr_t id,std::string&e){auto s=slot(key,e);return s&&s->owner->collision_end(id,e);}
bool OriginalTriggerContacts::touching(const std::string&key,std::uintptr_t id,bool&hit,std::string&e){auto s=slot(key,e);if(!s)return false;dh2::world::ZoneCollisionServicesV83 source;return peer_services(s,source,e)&&dh2::world::zone_is_touching_v83(s->owner->base(),id,source,hit,e);}
const dh2::world::CanonicalTriggerZoneV22* OriginalTriggerContacts::owner(const std::string&key)const{auto i=slots_.find(key);return i==slots_.end()?nullptr:i->second->owner.get();}
void OriginalTriggerContacts::clear(){order_.clear();slots_.clear();}
}
