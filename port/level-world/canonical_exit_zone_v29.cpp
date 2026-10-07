#include "canonical_exit_zone_v29.hpp"
#include <algorithm>
namespace dh2::world {
CanonicalExitZoneV29::CanonicalExitZoneV29(std::shared_ptr<void> world,actor::RuntimeState& runtime,ExitZoneServicesV29 services):
 CanonicalTriggerZoneV22(std::move(world),runtime,std::move(services.trigger),14),services_(std::move(services)){}
CanonicalPropertyActorV1 CanonicalExitZoneV29::properties()noexcept{
 auto a=canonical_family_fields_v15(*this);
 a.fields.write_vector3=[](void* p,std::uint32_t o,const auto& v,std::string& e){return static_cast<CanonicalExitZoneV29*>(p)->write_vector3(o,v,e);};return a;
}
bool CanonicalExitZoneV29::write_int(std::uint32_t o,std::int32_t v,std::string& e){
 switch(o){case 0x7d8:level_id7d8_=v;return true;case 0x7f4:entrypoint7f4_=v;return true;case 0x7f8:maploc7f8_=v;return true;case 0x7fc:question7fc_=v;return true;default:return CanonicalTriggerZoneV22::write_int(o,v,e);}
}
bool CanonicalExitZoneV29::write_string(std::uint32_t o,const std::string& v,std::string& e){if(o==0x7dc){level_name7dc_=v;return true;}if(o==0x800){fasttravel800_=v;return true;}return CanonicalTriggerZoneV22::write_string(o,v,e);}
std::int32_t* CanonicalExitZoneV29::source_integer(std::uint32_t o)noexcept{
 switch(o){case 0x7d4:return &level_list_id7d4_;case 0x7d8:return level_id7d8_?&*level_id7d8_:nullptr;case 0x7f4:return entrypoint7f4_?&*entrypoint7f4_:nullptr;case 0x7f8:return maploc7f8_?&*maploc7f8_:nullptr;case 0x7fc:return question7fc_?&*question7fc_:nullptr;default:return CanonicalTriggerZoneV22::source_integer(o);}
}
std::uint8_t* CanonicalExitZoneV29::source_byte(std::uint32_t o)noexcept{if(o==0x818)return &byte818_;if(o==0x819)return &byte819_;return CanonicalTriggerZoneV22::source_byte(o);}
std::string* CanonicalExitZoneV29::source_string(std::uint32_t o)noexcept{if(o==0x7dc)return &level_name7dc_;if(o==0x800)return &fasttravel800_;return CanonicalTriggerZoneV22::source_string(o);}
bool CanonicalExitZoneV29::init_post(std::string& e){
 // Whole original39c8c8: qualified parent first, then nonempty levelName
 // lookup into the engine's actual Arrays::LevelList. Empty skips lookup.
 if(!CanonicalTriggerZoneV22::init_post(e))return false;
 if(level_name7dc_.empty())return true;
 if(!services_.level_name_id){e="Required actual Arrays::LevelList GetMemberID39c2c8";return false;}
 return services_.level_name_id(level_name7dc_.c_str(),level_list_id7d4_,e);
}
bool CanonicalExitZoneV29::update(std::string& e){if(!services_.whole_update){e="Required actual ExitZone Update39c33c/transition";return false;}return services_.whole_update(*this,e);}
bool CanonicalExitZoneV29::destroy(std::string& e){
 if(destroyed_){e="ExitZone destruction cannot replay";return false;}
 if(!services_.whole_destroy){e="Required actual ExitZone/TriggerZone/base destruction";return false;}
 destroyed_=true;return services_.whole_destroy(*this,e);
}
bool CanonicalExitZoneV29::set_position(const std::array<float,3>& p,bool destination,std::string& e){return source_set_position_v29(p,destination,e);}
CanonicalClassReceiverV1 CanonicalExitZoneV29::factory_receiver(std::shared_ptr<CanonicalExitZoneV29> owner,std::shared_ptr<const void> xml){
 auto r=canonical_class_receiver_v1(owner);r.source_lease=std::move(xml);
 r.init_post=[owner](std::string& e){return owner->init_post(e);};r.is_game_object=[](bool& v,std::string&){v=true;return true;};
 r.position=[owner](std::array<float,3>& p,std::string&){std::copy_n(owner->base().vector3(0x160),3,p.begin());return true;};
 r.set_position=[owner](const auto& p,bool destination,std::string& e){return owner->set_position(p,destination,e);};return r;
}
}
