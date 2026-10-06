#include "canonical_door_v27.hpp"
#include <algorithm>
#include <utility>
namespace dh2::world {
CanonicalDoorV27::CanonicalDoorV27(std::shared_ptr<void> world,actor::RuntimeState& runtime,
 GameObjectInitializationServicesV1 initialization,DoorServicesV27 services):
 base_(reinterpret_cast<std::uintptr_t>(this),2,std::move(world),runtime),
 initialization_services_(std::move(initialization)),services_(std::move(services)){
 base_.lifecycle().static84=1;*base_.byte(0x28)=1;*base_.byte(0xf8)=3;
 *base_.pointer(0x100)=reinterpret_cast<std::uintptr_t>(&network_[0]);
 *base_.pointer(0x104)=reinterpret_cast<std::uintptr_t>(&network_[1]);
}
CanonicalPropertyActorV1 CanonicalDoorV27::properties()noexcept{
 auto result=canonical_family_fields_v15(*this);
 result.fields.write_vector3=[](void* p,std::uint32_t o,const auto& v,std::string& e){return static_cast<CanonicalDoorV27*>(p)->write_vector3(o,v,e);};return result;
}
bool CanonicalDoorV27::read_bool(std::uint32_t o,std::uint8_t& v,std::string& e){
 if(o==0x3a4){v=opened3a4_;return true;}if(o==0x3a5){v=collision3a5_;return true;}
 auto f=base_.properties().fields;return f.read_bool(f.context,o,v,e);
}
bool CanonicalDoorV27::write_bool(std::uint32_t o,std::uint8_t v,std::string& e){
 if(o==0x3a4){opened3a4_=v;return true;}if(o==0x3a5){collision3a5_=v;return true;}
 auto f=base_.properties().fields;return f.write_bool(f.context,o,v,e);
}
bool CanonicalDoorV27::write_int(std::uint32_t o,std::int32_t v,std::string& e){auto f=base_.properties().fields;return f.write_int(f.context,o,v,e);}
bool CanonicalDoorV27::write_string(std::uint32_t o,const std::string& v,std::string& e){
 if(o==0x388){data388_=v;return true;}auto f=base_.properties().fields;return f.write_string(f.context,o,v,e);
}
bool CanonicalDoorV27::write_vector3(std::uint32_t o,const std::array<float,3>& v,std::string& e){
 if(o==0x374){dimensions374_=v;return true;}auto f=base_.properties().fields;return f.write_vector3(f.context,o,v,e);
}
bool CanonicalDoorV27::init_post(std::string& e){
 if(!services_.whole_init_post){e="Required actual Door InitPost3e7da8";return false;}return services_.whole_init_post(*this,e);
}
bool CanonicalDoorV27::init_final(std::string& e){
 if(!services_.whole_init_final){e="Required actual Door InitFinal3e7c1c";return false;}return services_.whole_init_final(*this,e);
}
bool CanonicalDoorV27::set_position(const std::array<float,3>& p,bool destination,std::string& e){
 if(!initialization_services_.set_position){e="Required SAME Door SetPosition";return false;}return initialization_services_.set_position(p.data(),destination,e);
}
bool CanonicalDoorV27::destroy(std::string& e){
 if(destroyed_){e="Door destruction cannot replay";return false;}
 if(!services_.whole_destroy){e="Required actual Door/NetStruct/Zone/GameObject destruction";return false;}
 destroyed_=true;return services_.whole_destroy(*this,e);
}
CanonicalClassReceiverV1 CanonicalDoorV27::factory_receiver(std::shared_ptr<CanonicalDoorV27> owner,std::shared_ptr<const void> xml){
 auto result=canonical_class_receiver_v1(owner);result.source_lease=std::move(xml);
 result.init_post=[owner](std::string& e){return owner->init_post(e);};
 result.is_game_object=[](bool& value,std::string&){value=true;return true;};
 result.position=[owner](std::array<float,3>& value,std::string&){std::copy_n(owner->base().vector3(0x160),3,value.begin());return true;};
 result.set_position=[owner](const auto& value,bool destination,std::string& e){return owner->set_position(value,destination,e);};return result;
}
}
