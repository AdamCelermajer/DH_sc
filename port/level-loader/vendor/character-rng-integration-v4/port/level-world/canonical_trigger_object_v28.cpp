#include "canonical_trigger_object_v28.hpp"
#include <algorithm>
#include <utility>
namespace dh2::world {
CanonicalTriggerObjectV28::CanonicalTriggerObjectV28(std::shared_ptr<void> world,actor::RuntimeState& runtime,
 GameObjectInitializationServicesV1 initialization,TriggerObjectServicesV28 services):
 base_(reinterpret_cast<std::uintptr_t>(this),20,std::move(world),runtime),
 initialization_services_(std::move(initialization)),services_(std::move(services)){
 base_.lifecycle().static84=0;base_.lifecycle().updating85=1;
 *base_.byte(0x28)=1;*base_.byte(0xf8)=4;
 *base_.pointer(0x100)=reinterpret_cast<std::uintptr_t>(&network_[0]);
 *base_.pointer(0x104)=reinterpret_cast<std::uintptr_t>(&network_[1]);
}
CanonicalPropertyActorV1 CanonicalTriggerObjectV28::properties()noexcept{
 auto result=canonical_family_fields_v15(*this);
 result.fields.write_vector3=[](void* p,std::uint32_t o,const auto& v,std::string& e){return static_cast<CanonicalTriggerObjectV28*>(p)->write_vector3(o,v,e);};return result;
}
bool CanonicalTriggerObjectV28::read_bool(std::uint32_t o,std::uint8_t& v,std::string& e){if(o==0x3b0){v=reset3b0_;return true;}auto f=base_.properties().fields;return f.read_bool(f.context,o,v,e);}
bool CanonicalTriggerObjectV28::write_bool(std::uint32_t o,std::uint8_t v,std::string& e){if(o==0x3b0){reset3b0_=v;return true;}auto f=base_.properties().fields;return f.write_bool(f.context,o,v,e);}
bool CanonicalTriggerObjectV28::write_int(std::uint32_t o,std::int32_t v,std::string& e){
 if(o==0x3a8){count3a8_=v;return true;}if(o==0x3ac){delay3ac_=v;return true;}
 auto f=base_.properties().fields;return f.write_int(f.context,o,v,e);
}
bool CanonicalTriggerObjectV28::write_string(std::uint32_t o,const std::string& v,std::string& e){
 const std::uint32_t offsets[]{0x718,0x734,0x750,0x76c};for(unsigned i=0;i<4;++i)if(o==offsets[i]){names_[i]=v;return true;}
 auto f=base_.properties().fields;return f.write_string(f.context,o,v,e);
}
bool CanonicalTriggerObjectV28::write_vector3(std::uint32_t o,const std::array<float,3>& v,std::string& e){if(o==0x374){dimensions374_=v;return true;}auto f=base_.properties().fields;return f.write_vector3(f.context,o,v,e);}
std::int32_t* CanonicalTriggerObjectV28::source_integer(std::uint32_t o)noexcept{
 switch(o){case 0x3a0:return &characters3a0_;case 0x3a4:return &players3a4_;case 0x3a8:return &count3a8_;case 0x3ac:return &delay3ac_;case 0x3b4:return &activated3b4_;case 0x3b8:return &timer3b8_;case 0x3c0:return &touching3c0_;case 0x730:return &data_id730_;case 0x74c:return &script_id74c_;case 0x768:return script_id768_?&*script_id768_:nullptr;default:return base_.integer(o);}
}
std::uint8_t* CanonicalTriggerObjectV28::source_byte(std::uint32_t o)noexcept{switch(o){case 0x3b0:return &reset3b0_;case 0x3bc:return &local_only3bc_;case 0x784:return &byte784_;default:return base_.byte(o);}}
std::string* CanonicalTriggerObjectV28::source_string(std::uint32_t o)noexcept{const std::uint32_t offsets[]{0x718,0x734,0x750,0x76c};for(unsigned i=0;i<4;++i)if(o==offsets[i])return &names_[i];return base_.string(o);}
bool CanonicalTriggerObjectV28::init_post(std::string& e){if(!services_.whole_init_post){e="Required actual TriggerObject InitPost399ff8";return false;}return services_.whole_init_post(*this,e);}
bool CanonicalTriggerObjectV28::init_final(std::string& e){if(!services_.whole_init_final){e="Required actual TriggerObject inherited InitFinal38cd48";return false;}return services_.whole_init_final(*this,e);}
bool CanonicalTriggerObjectV28::update(std::string& e){if(!services_.whole_update){e="Required actual TriggerObject Update3994a8";return false;}return services_.whole_update(*this,e);}
bool CanonicalTriggerObjectV28::interact(std::uintptr_t object,std::string& e){if(!services_.whole_interact){e="Required actual TriggerObject Interact399af0";return false;}return services_.whole_interact(*this,object,e);}
bool CanonicalTriggerObjectV28::set_position(const std::array<float,3>& value,bool destination,std::string& e){if(!initialization_services_.set_position){e="Required SAME TriggerObject SetPosition";return false;}return initialization_services_.set_position(value.data(),destination,e);}
bool CanonicalTriggerObjectV28::destroy(std::string& e){
 if(destroyed_){e="TriggerObject destruction cannot replay";return false;}
 if(!services_.whole_destroy){e="Required actual TriggerObject/Trigger/Zone/base destruction";return false;}
 destroyed_=true;return services_.whole_destroy(*this,e);
}
CanonicalClassReceiverV1 CanonicalTriggerObjectV28::factory_receiver(std::shared_ptr<CanonicalTriggerObjectV28> owner,std::shared_ptr<const void> xml){
 auto result=canonical_class_receiver_v1(owner);result.source_lease=std::move(xml);
 result.init_post=[owner](std::string& e){return owner->init_post(e);};
 result.is_game_object=[](bool& value,std::string&){value=true;return true;};
 result.position=[owner](std::array<float,3>& value,std::string&){std::copy_n(owner->base().vector3(0x160),3,value.begin());return true;};
 result.set_position=[owner](const auto& value,bool destination,std::string& e){return owner->set_position(value,destination,e);};return result;
}
}
