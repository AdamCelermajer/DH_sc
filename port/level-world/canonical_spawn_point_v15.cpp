#include "canonical_spawn_point_v15.hpp"
#include <algorithm>
namespace dh2::world {
CanonicalSpawnPointV15::CanonicalSpawnPointV15(std::shared_ptr<void> p,actor::RuntimeState& r,GameObjectInitializationServicesV1 i,SpawnPointServicesV15 s):base_(reinterpret_cast<std::uintptr_t>(this),13,std::move(p),r),init_services_(std::move(i)),initialization_(base_,init_services_),services_(std::move(s)){}
bool CanonicalSpawnPointV15::read_bool(std::uint32_t o,std::uint8_t& v,std::string& e){auto a=base_.properties().fields;return a.read_bool(a.context,o,v,e);}
bool CanonicalSpawnPointV15::write_bool(std::uint32_t o,std::uint8_t v,std::string& e){auto a=base_.properties().fields;return a.write_bool(a.context,o,v,e);}
bool CanonicalSpawnPointV15::write_int(std::uint32_t o,std::int32_t v,std::string& e){if(o==0x374){entrypoint374_=v;return true;}auto a=base_.properties().fields;return a.write_int(a.context,o,v,e);}
bool CanonicalSpawnPointV15::write_string(std::uint32_t o,const std::string& v,std::string& e){if(o==0x378){script378_=v;return true;}auto a=base_.properties().fields;return a.write_string(a.context,o,v,e);}
bool CanonicalSpawnPointV15::init_post(std::string& e){bool eligible{};if(!initialization_.init_post(eligible,e))return false;if(!services_.script_id){e="Required ScriptManager GetIDFromName4591f0";return false;}return services_.script_id(script378_,false,script390_,e);}
bool CanonicalSpawnPointV15::set_position(const std::array<float,3>& p,bool b,std::string& e){if(!init_services_.set_position){e="Required SAME SpawnPoint SetPosition";return false;}return init_services_.set_position(p.data(),b,e);}
bool CanonicalSpawnPointV15::place_object(std::uintptr_t object,std::string& e){
 if(!object||!services_.set_object_position){e="Required actual SpawnPoint object SetPosition393db4";return false;}
 if(!services_.set_object_position(object,base_.vector3(0x160),true,e))return false;
 if(!services_.set_object_rotation){e="Required actual SpawnPoint object SetRotation3938a0";return false;}
 if(!services_.set_object_rotation(object,base_.vector3(0x16c),e))return false;
 if(script390_==-1)return true;if(!services_.start_script){e="Required ScriptManager StartScript4605c0";return false;}return services_.start_script(script390_,base_.room64(),false,e);
}
bool CanonicalSpawnPointV15::destroy(std::string& e){if(destroyed_){e="SpawnPoint destruction cannot replay";return false;}if(!services_.destroy_base){e="Required actual SpawnPoint GameObject destruction";return false;}destroyed_=true;script378_.clear();return services_.destroy_base(base_,e);}
CanonicalClassReceiverV1 CanonicalSpawnPointV15::factory_receiver(std::shared_ptr<CanonicalSpawnPointV15> o,std::shared_ptr<const void> xml){auto a=canonical_class_receiver_v1(o);a.source_lease=std::move(xml);a.init_post=[o](std::string& e){return o->init_post(e);};a.is_game_object=[](bool& b,std::string&){b=true;return true;};a.position=[o](std::array<float,3>& p,std::string&){std::copy_n(o->base().vector3(0x160),3,p.begin());return true;};a.set_position=[o](const auto& p,bool b,std::string& e){return o->set_position(p,b,e);};return a;}
}
