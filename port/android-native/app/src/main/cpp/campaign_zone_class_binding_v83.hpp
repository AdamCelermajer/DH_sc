#pragma once
#include "source_campaign_zones_v83.hpp"
#include <canonical_zone_physical_v82.hpp>
#include <type_traits>
namespace model_renderer {
// For the existing auxiliary lenders, which receive a typed class reference
// rather than its hidden record: acquire that record through Self.borrow and
// retain only its weak control-block witness. This is not an ownership map.
template<class Self,class Zone>bool bind_campaign_zone_lender_contacts_v83(
 std::weak_ptr<Self> weak,Zone& zone,const dh2::world::ZoneCollisionServicesV83& common,
 dh2::world::CanonicalZonePhysicalServicesV82& out,std::string& e){
 auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
 const auto id=zone.base().identity();if(!self||!self->borrow(id,pin,base,e)||base!=&zone.base()){if(e.empty())e="Required actual typed Zone lender record";return false;}
 std::weak_ptr<void> record=pin;Zone* typed=&zone;
 out.collision_test_notification=[](auto& actual,auto peer,auto& e){return dh2::world::zone_collision_notification_v83(actual,peer,e);};
 out.contact=[weak,record,typed,id,common](auto& actual,auto event,auto peer,auto& e){
  auto self=weak.lock();auto witness=record.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  if(!self||!witness||!self->borrow(id,pin,base,e)||!pin||pin.owner_before(witness)||witness.owner_before(pin)||base!=&actual||base!=&typed->base()){
   if(e.empty())e="Released/replaced actual typed Zone physical contact";return false;
  }
  using dh2::physical::ContactEvent;
  if constexpr(std::is_same_v<Zone,dh2::world::CanonicalCheckpointZoneV26>){if(event==ContactEvent::add)return dh2::world::checkpoint_collision_begin_v83(*typed,peer,common,e);}
  else if constexpr(std::is_same_v<Zone,dh2::world::CanonicalQuestMoveZoneV31>){if(event==ContactEvent::add)return dh2::world::quest_move_collision_begin_v83(*typed,peer,common,e);}
  else if constexpr(std::is_base_of_v<dh2::world::CanonicalTriggerZoneV22,Zone>){if(event==ContactEvent::add)return typed->collision_begin(peer,e);if(event==ContactEvent::remove)return typed->collision_end(peer,e);}
  else {e="Unrecovered concrete Zone physical contact family";return false;}
  e.clear();return true;
 };
 e.clear();return true;
}
// Attach directly to the loader's SAME constructor record. Record owns the
// unique canonical class; callbacks own only weak record leases, never Self,
// World, another contact set or a detached class pointer.
template<class Record>bool bind_campaign_zone_contacts_v83(std::weak_ptr<Record> record,
 const dh2::world::ZoneCollisionServicesV83& common,
 dh2::world::CanonicalZonePhysicalServicesV82& out,std::string& e){
 auto actual=record.lock();if(!actual||!actual->owner){e="Required existing canonical Zone record before contact binding";return false;}
 using Zone=typename decltype(actual->owner)::element_type;
 out.collision_test_notification=[](auto& base,auto peer,auto& e){return dh2::world::zone_collision_notification_v83(base,peer,e);};
 out.contact=[record,common](auto& base,auto event,auto peer,auto& e){
  auto r=record.lock();if(!r||!r->owner||&r->owner->base()!=&base){e="Released/replaced SAME Zone contact receiver";return false;}
  auto& zone=*r->owner;using dh2::physical::ContactEvent;
  if constexpr(std::is_same_v<Zone,dh2::world::CanonicalCheckpointZoneV26>){
   if(event==ContactEvent::add)return dh2::world::checkpoint_collision_begin_v83(zone,peer,common,e);
  }else if constexpr(std::is_same_v<Zone,dh2::world::CanonicalQuestMoveZoneV31>){
   if(event==ContactEvent::add)return dh2::world::quest_move_collision_begin_v83(zone,peer,common,e);
  }else if constexpr(std::is_base_of_v<dh2::world::CanonicalTriggerZoneV22,Zone>){
   if(event==ContactEvent::add)return zone.collision_begin(peer,e);
   if(event==ContactEvent::remove)return zone.collision_end(peer,e);
  }else {e="Unrecovered concrete Zone collision virtual family";return false;}
  // All these selected classes inherit395944 literal persists. Checkpoint
  // and QuestMove also inherit395948 literal end. Result has no cc/d0/d4.
  e.clear();return true;
 };
 e.clear();return true;
}
template<class Record>bool bind_campaign_checkpoint_record_v83(std::weak_ptr<Record> record,
 const dh2::world::ZoneCollisionServicesV83& common,dh2::world::CheckpointZoneServicesV26& out,std::string& e){
 auto actual=record.lock();if(!actual||!actual->owner){e="Required SAME Checkpoint C1 record";return false;}
 out.whole_collision_begin=[record,common](auto& zone,auto peer,auto& e){auto r=record.lock();if(!r||r->owner.get()!=&zone){e="Released Checkpoint collision receiver";return false;}return dh2::world::checkpoint_collision_begin_v83(zone,peer,common,e);};e.clear();return true;
}
template<class Record>bool bind_campaign_quest_move_record_v83(std::weak_ptr<Record> record,
 const dh2::world::ZoneCollisionServicesV83& common,dh2::world::QuestMoveZoneServicesV31& out,std::string& e){
 auto actual=record.lock();if(!actual||!actual->owner){e="Required SAME QuestMove C1 record";return false;}
 out.whole_collision_begin=[record,common](auto& zone,auto peer,auto& e){auto r=record.lock();if(!r||r->owner.get()!=&zone){e="Released QuestMove collision receiver";return false;}return dh2::world::quest_move_collision_begin_v83(zone,peer,common,e);};e.clear();return true;
}
template<class Record>bool bind_campaign_trigger_record_v83(const SourceCampaignCandidateBorrowV55& candidate,
 std::weak_ptr<Record> record,const dh2::world::ZoneCollisionServicesV83& common,
 dh2::world::TriggerZoneServicesV22& out,std::string& e){
 auto actual=record.lock();if(!actual||!actual->owner){e="Required SAME Trigger-derived C1 record";return false;}
 if(!bind_source_campaign_trigger_services_v83(candidate,actual->owner->base().identity(),common,out,e))return false;
 out.zone_inside=[record,common](auto peer,auto& inside,auto& e){auto r=record.lock();if(!r||!r->owner){e="Released actual Trigger.IsInside receiver";return false;}
  return dh2::world::zone_is_inside_v83(r->owner->base(),r->owner->source_colzone384_v83(),peer,common,inside,e);};e.clear();return true;
}
template<class Record>bool bind_campaign_exit_record_v83(std::weak_ptr<Record> record,
 const dh2::world::ExitZoneRuntimeServicesV83& services,dh2::world::ExitZoneServicesV29& out,std::string& e){
 auto actual=record.lock();if(!actual||!actual->owner){e="Required SAME Exit C1 record";return false;}
 out.whole_update=[record,services](auto& zone,auto& e){auto r=record.lock();if(!r||r->owner.get()!=&zone){e="Released actual Exit Update receiver";return false;}return dh2::world::exit_zone_update_v83(zone,services,e);};e.clear();return true;
}
}
