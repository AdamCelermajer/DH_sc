#include "canonical_quest_move_zone_v31.hpp"
#include <algorithm>
#include <utility>

namespace dh2::world {
CanonicalQuestMoveZoneV31::CanonicalQuestMoveZoneV31(std::shared_ptr<void> world,
 actor::RuntimeState& runtime,GameObjectInitializationServicesV1 initialization,
 QuestMoveZoneServicesV31 services):base_(reinterpret_cast<std::uintptr_t>(this),20,std::move(world),runtime),
 initialization_services_(std::move(initialization)),initialization_(base_,initialization_services_),services_(std::move(services)){
 base_.lifecycle().static84=1;
}
CanonicalPropertyActorV1 CanonicalQuestMoveZoneV31::properties()noexcept{
 auto result=canonical_family_fields_v15(*this);
 result.fields.write_vector3=[](void* p,std::uint32_t offset,const std::array<float,3>& value,std::string& error){return static_cast<CanonicalQuestMoveZoneV31*>(p)->write_vector3(offset,value,error);};
 return result;
}
bool CanonicalQuestMoveZoneV31::read_bool(std::uint32_t offset,std::uint8_t& value,std::string& error){auto fields=base_.properties().fields;return fields.read_bool(fields.context,offset,value,error);}
bool CanonicalQuestMoveZoneV31::write_bool(std::uint32_t offset,std::uint8_t value,std::string& error){auto fields=base_.properties().fields;return fields.write_bool(fields.context,offset,value,error);}
bool CanonicalQuestMoveZoneV31::write_int(std::uint32_t offset,std::int32_t value,std::string& error){auto fields=base_.properties().fields;return fields.write_int(fields.context,offset,value,error);}
bool CanonicalQuestMoveZoneV31::write_string(std::uint32_t offset,const std::string& value,std::string& error){auto fields=base_.properties().fields;return fields.write_string(fields.context,offset,value,error);}
bool CanonicalQuestMoveZoneV31::write_vector3(std::uint32_t offset,const std::array<float,3>& value,std::string& error){
 if(offset==0x374){dimensions374_=value;return true;}
 auto fields=base_.properties().fields;return fields.write_vector3(fields.context,offset,value,error);
}
bool CanonicalQuestMoveZoneV31::init_post(std::string& error){
 return zone_init_post_v76(base_,initialization_,initialization_services_,dimensions374_,physical380_,trigger381_,colzone384_,colzone_lease_,services_.startup,error);
}
bool CanonicalQuestMoveZoneV31::collision_begin(std::uintptr_t object,std::string& error){
 if(!services_.whole_collision_begin){error="Required actual QuestMoveInZone collision/quest396120";return false;}
 return services_.whole_collision_begin(*this,object,error);
}
bool CanonicalQuestMoveZoneV31::set_position(const std::array<float,3>& value,bool destination,std::string& error){
 if(!initialization_services_.set_position){error="Required SAME QuestMoveInZone SetPosition";return false;}
 return initialization_services_.set_position(value.data(),destination,error);
}
bool CanonicalQuestMoveZoneV31::destroy(std::string& error){
 if(destroyed_){error="QuestMoveInZone destruction cannot replay";return false;}
 if(!services_.whole_destroy){error="Required actual QuestMoveInZone/Zone/GameObject destruction";return false;}
 destroyed_=true;return services_.whole_destroy(*this,error);
}
CanonicalClassReceiverV1 CanonicalQuestMoveZoneV31::factory_receiver(
 std::shared_ptr<CanonicalQuestMoveZoneV31> owner,std::shared_ptr<const void> xml){
 auto result=canonical_class_receiver_v1(owner);result.source_lease=std::move(xml);
 result.init_post=[owner](std::string& error){return owner->init_post(error);};
 result.is_game_object=[](bool& value,std::string&){value=true;return true;};
 result.position=[owner](std::array<float,3>& value,std::string&){std::copy_n(owner->base().vector3(0x160),3,value.begin());return true;};
 result.set_position=[owner](const std::array<float,3>& value,bool destination,std::string& error){return owner->set_position(value,destination,error);};
 return result;
}
}
