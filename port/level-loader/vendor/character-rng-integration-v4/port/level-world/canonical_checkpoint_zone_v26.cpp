#include "canonical_checkpoint_zone_v26.hpp"
#include <algorithm>
#include <utility>

namespace dh2::world {
CanonicalCheckpointZoneV26::CanonicalCheckpointZoneV26(std::shared_ptr<void> world,
 actor::RuntimeState& runtime,GameObjectInitializationServicesV1 initialization,
 CheckpointZoneServicesV26 services):base_(reinterpret_cast<std::uintptr_t>(this),12,std::move(world),runtime),
 initialization_services_(std::move(initialization)),initialization_(base_,initialization_services_),services_(std::move(services)){
 base_.lifecycle().static84=1;
}
CanonicalPropertyActorV1 CanonicalCheckpointZoneV26::properties()noexcept{
 auto result=canonical_family_fields_v15(*this);
 result.fields.write_vector3=[](void* p,std::uint32_t offset,const std::array<float,3>& value,std::string& error){return static_cast<CanonicalCheckpointZoneV26*>(p)->write_vector3(offset,value,error);};
 return result;
}
bool CanonicalCheckpointZoneV26::read_bool(std::uint32_t offset,std::uint8_t& value,std::string& error){auto fields=base_.properties().fields;return fields.read_bool(fields.context,offset,value,error);}
bool CanonicalCheckpointZoneV26::write_bool(std::uint32_t offset,std::uint8_t value,std::string& error){auto fields=base_.properties().fields;return fields.write_bool(fields.context,offset,value,error);}
bool CanonicalCheckpointZoneV26::write_int(std::uint32_t offset,std::int32_t value,std::string& error){auto fields=base_.properties().fields;return fields.write_int(fields.context,offset,value,error);}
bool CanonicalCheckpointZoneV26::write_string(std::uint32_t offset,const std::string& value,std::string& error){auto fields=base_.properties().fields;return fields.write_string(fields.context,offset,value,error);}
bool CanonicalCheckpointZoneV26::write_vector3(std::uint32_t offset,const std::array<float,3>& value,std::string& error){
 if(offset==0x374){dimensions374_=value;return true;}
 auto fields=base_.properties().fields;return fields.write_vector3(fields.context,offset,value,error);
}
bool CanonicalCheckpointZoneV26::init_post(std::string& error){
 if(!services_.whole_zone_init_post){error="Required actual Checkpoint Zone InitPost39771c";return false;}
 return services_.whole_zone_init_post(*this,error);
}
bool CanonicalCheckpointZoneV26::collision_begin(std::uintptr_t object,std::string& error){
 if(!services_.whole_collision_begin){error="Required actual Checkpoint collision/save395f04";return false;}
 return services_.whole_collision_begin(*this,object,error);
}
bool CanonicalCheckpointZoneV26::set_position(const std::array<float,3>& value,bool destination,std::string& error){
 if(!initialization_services_.set_position){error="Required SAME Checkpoint SetPosition";return false;}
 return initialization_services_.set_position(value.data(),destination,error);
}
bool CanonicalCheckpointZoneV26::destroy(std::string& error){
 if(destroyed_){error="Checkpoint destruction cannot replay";return false;}
 if(!services_.whole_destroy){error="Required actual Checkpoint/Zone/GameObject destruction";return false;}
 destroyed_=true;return services_.whole_destroy(*this,error);
}
CanonicalClassReceiverV1 CanonicalCheckpointZoneV26::factory_receiver(
 std::shared_ptr<CanonicalCheckpointZoneV26> owner,std::shared_ptr<const void> xml){
 auto result=canonical_class_receiver_v1(owner);result.source_lease=std::move(xml);
 result.init_post=[owner](std::string& error){return owner->init_post(error);};
 result.is_game_object=[](bool& value,std::string&){value=true;return true;};
 result.position=[owner](std::array<float,3>& value,std::string&){std::copy_n(owner->base().vector3(0x160),3,value.begin());return true;};
 result.set_position=[owner](const std::array<float,3>& value,bool destination,std::string& error){return owner->set_position(value,destination,error);};
 return result;
}
}
