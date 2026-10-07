#include "source_campaign_character_zoning_v108.hpp"
#include "model_renderer.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_noncharacter_owners_v105.hpp"
#include "source_campaign_object_update_actor_v104.hpp"
#include "source_campaign_character_fsm_v101.hpp"
#include "source_campaign_script_actor_v96.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <canonical_object_manager_v1.hpp>
#include <module_room_zone_connection_v91.hpp>
#include <catalog_generic_v70.hpp>
#include <gameobject_zoning_v108.hpp>
namespace model_renderer {
bool source_campaign_character_zoning_v108(const std::shared_ptr<void>& world,std::uintptr_t id,bool enabled,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;SourceCampaignCharacterBorrowV62 receiver;
 std::shared_ptr<SourceWorldBorrowV61> actual;
 if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world||
    !borrow_source_campaign_character_v62(world,id,receiver,e)||
    !borrow_source_campaign_condition_world_v70(candidate,actual,e))return false;
 dh2::world::ObjectUpdateActorV102 object;
 if(!borrow_source_campaign_object_update_actor_v104(candidate,id,object,e)||!object.byte||!object.pointer||
    !receiver.character||!receiver.character->actor)return false;
 dh2::world::GameObjectInitializationFieldsV62 fields;
 if(!receiver.character->actor->inherited_initialization_fields_v62(receiver.character,fields,e)||
    fields.source_identity!=id||!fields.byte||!fields.pointer)return false;
 // Lend the actual mutable retained cells. The frame projection's pointer
 // loan is read-only and must not be cast into a mutation authority.
 dh2::world::GameObjectZoningBorrowV108 f{fields.receiver,id,fields.byte(0x2ee),fields.byte(0x2f0),fields.pointer(0x2f4),fields.pointer(0x2d8)};
 auto room=[&](std::uintptr_t identity,std::shared_ptr<dh2::world::CanonicalRoomZoneV3>& out,std::string& error){
  if(actual->module_room_zones_v91){std::string ignored;if(actual->module_room_zones_v91->borrow_room(identity,out,ignored))return true;}
  std::shared_ptr<dh2::loader::ProductionNonCharacterOwnersV67> owners;
  if(!borrow_source_campaign_noncharacter_owners_v105(candidate,owners,error))return false;
  auto record=owners&&owners->generic?owners->generic->source_room_v105(identity):nullptr;
  if(!record||!record->constructor_completed_v91||!record->receiver){error="Required SAME retained Room2f4 zoning receiver";return false;}
  out=std::shared_ptr<dh2::world::CanonicalRoomZoneV3>(record,record->receiver.get());return true;
 };
 dh2::world::GameObjectZoningServicesV108 s;
 s.room_remove=[&](auto identity,auto actor,auto& error){std::shared_ptr<dh2::world::CanonicalRoomZoneV3> pin;if(!room(identity,pin,error))return false;pin->source_remove_object_v104(actor);return true;};
 s.room_add=[&](auto identity,auto actor,auto& error){std::shared_ptr<dh2::world::CanonicalRoomZoneV3> pin;return room(identity,pin,error)&&pin->source_add_object_v104(actor,error);};
 s.room_active389=[&](auto identity,std::uint8_t& value,auto& error){std::shared_ptr<dh2::world::CanonicalRoomZoneV3> pin;if(!room(identity,pin,error))return false;const auto* cell=pin->source_byte(0x389);if(!cell){error="Required actual Room389";return false;}value=*cell;return true;};
 s.add_no_room=[&](auto& error){return candidate.objects->source_add_no_room_object_v89(object.object,error);};
 s.remove_no_room=[&](auto& error){return candidate.objects->source_remove_no_room_object_v108(object.object,error);};
 s.is_zonable=[&](bool& value,auto& error){return source_campaign_character_is_zonable_v104(world,id,value,error);};
 s.set_updating=[&](std::uint8_t value,auto& error){auto* cell=object.byte(0x85);if(!cell){error="Required SAME ObjectBase setUpdating85";return false;}*cell=value;return true;};
 s.sync_visibility=[&](auto visual,auto& error){const auto current=receiver.character->actor->source_visual();if(current!=visual){error="Captured Visual2d8 replaced during zoning delivery";return false;}return source_campaign_character_sync_visibility_v86(world,id,error);};
 s.qualified_zone_event=[&](bool entered,auto& error){return entered?source_campaign_character_zone_entered_v102(world,id,error):source_campaign_character_zone_exited_v102(world,id,error);};
 return enabled?dh2::world::gameobject_enable_zoning_v108(f,s,e):dh2::world::gameobject_disable_zoning_v108(f,s,e);
}
}
