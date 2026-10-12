#include "canonical_trigger_trap_v37.hpp"
#include <algorithm>
#include <utility>
namespace dh2::world {
CanonicalTriggerTrapV37::CanonicalTriggerTrapV37(std::shared_ptr<void> world,actor::RuntimeState& runtime,
 GameObjectInitializationServicesV1 initialization,TriggerTrapServicesV37 services):
 base_(reinterpret_cast<std::uintptr_t>(this),15,std::move(world),runtime),
 initialization_services_(std::move(initialization)),services_(std::move(services)){
 base_.lifecycle().static84=1;base_.lifecycle().updating85=1;
}
CanonicalPropertyActorV1 CanonicalTriggerTrapV37::properties()noexcept{
 auto result=canonical_family_fields_v15(*this);
 result.fields.write_vector3=[](void* p,std::uint32_t o,const auto& v,std::string& e){return static_cast<CanonicalTriggerTrapV37*>(p)->write_vector3(o,v,e);};return result;
}
bool CanonicalTriggerTrapV37::read_bool(std::uint32_t o,std::uint8_t& v,std::string& e){auto f=base_.properties().fields;return f.read_bool(f.context,o,v,e);}
bool CanonicalTriggerTrapV37::write_bool(std::uint32_t o,std::uint8_t v,std::string& e){auto f=base_.properties().fields;return f.write_bool(f.context,o,v,e);}
bool CanonicalTriggerTrapV37::write_int(std::uint32_t o,std::int32_t v,std::string& e){auto f=base_.properties().fields;return f.write_int(f.context,o,v,e);}
bool CanonicalTriggerTrapV37::write_string(std::uint32_t o,const std::string& v,std::string& e){if(o==0x3a8){data3a8_=v;return true;}auto f=base_.properties().fields;return f.write_string(f.context,o,v,e);}
bool CanonicalTriggerTrapV37::write_vector3(std::uint32_t o,const std::array<float,3>& v,std::string& e){if(o==0x374){dimensions374_=v;return true;}auto f=base_.properties().fields;return f.write_vector3(f.context,o,v,e);}
std::int32_t* CanonicalTriggerTrapV37::source_integer(std::uint32_t o)noexcept{
 switch(o){case 0x3a0:return &zone_count3a0_;case 0x3a4:return &zone_count3a4_;case 0x3c0:return &data_id3c0_;case 0x3f4:return timer3f4_?&*timer3f4_:nullptr;case 0x3fc:return &damager3fc_;default:return base_.integer(o);}
}
std::uint8_t* CanonicalTriggerTrapV37::source_byte(std::uint32_t o)noexcept{switch(o){case 0x3c4:return &activated3c4_;case 0x400:return &byte400_;case 0x401:return &byte401_;default:return base_.byte(o);}}
std::string* CanonicalTriggerTrapV37::source_string(std::uint32_t o)noexcept{if(o==0x3a8)return &data3a8_;return base_.string(o);}
bool CanonicalTriggerTrapV37::init_post(std::string& e){if(!services_.whole_init_post){e="Required actual TriggerTrap whole InitPost39dee0";return false;}return services_.whole_init_post(*this,e);}
bool CanonicalTriggerTrapV37::init_final(std::string& e){if(!services_.whole_init_final){e="Required actual TriggerTrap inherited InitFinal38cd48";return false;}return services_.whole_init_final(*this,e);}
bool CanonicalTriggerTrapV37::update(std::string& e){if(!services_.whole_update){e="Required actual TriggerTrap Update39eedc";return false;}return services_.whole_update(*this,e);}
bool CanonicalTriggerTrapV37::set_position(const std::array<float,3>& value,bool destination,std::string& e){if(!initialization_services_.set_position){e="Required SAME TriggerTrap SetPosition393db4";return false;}return initialization_services_.set_position(value.data(),destination,e);}
bool CanonicalTriggerTrapV37::destroy(std::string& e){
 if(destroyed_){e="TriggerTrap destruction cannot replay";return false;}
 if(!services_.whole_destroy){e="Required actual TriggerTrap/ZoneEx/Zone/base destruction39e480";return false;}
 destroyed_=true;return services_.whole_destroy(*this,e);
}
CanonicalClassReceiverV1 CanonicalTriggerTrapV37::factory_receiver(std::shared_ptr<CanonicalTriggerTrapV37> owner,std::shared_ptr<const void> xml){
 auto result=canonical_class_receiver_v1(owner);result.source_lease=std::move(xml);
 result.init_post=[owner](std::string& e){return owner->init_post(e);};
 result.is_game_object=[](bool& value,std::string&){value=true;return true;};
 result.position=[owner](std::array<float,3>& value,std::string&){std::copy_n(owner->base().vector3(0x160),3,value.begin());return true;};
 result.set_position=[owner](const auto& value,bool destination,std::string& e){return owner->set_position(value,destination,e);};return result;
}
}
