#include "canonical_spawn_spot_v108.hpp"
#include <algorithm>
namespace dh2::world {
CanonicalSpawnSpotV108::CanonicalSpawnSpotV108(std::shared_ptr<void> p,actor::RuntimeState& r,GameObjectInitializationServicesV1 i,SpawnSpotServicesV108 s):
 base_(reinterpret_cast<std::uintptr_t>(this),20,std::move(p),r),init_services_(std::move(i)),initialization_(base_,init_services_),services_(std::move(s)){}
bool CanonicalSpawnSpotV108::read_bool(std::uint32_t o,std::uint8_t& v,std::string& e){auto f=base_.properties().fields;return f.read_bool(f.context,o,v,e);}
bool CanonicalSpawnSpotV108::write_bool(std::uint32_t o,std::uint8_t v,std::string& e){auto f=base_.properties().fields;return f.write_bool(f.context,o,v,e);}
bool CanonicalSpawnSpotV108::write_int(std::uint32_t o,std::int32_t v,std::string& e){auto f=base_.properties().fields;return f.write_int(f.context,o,v,e);}
bool CanonicalSpawnSpotV108::write_string(std::uint32_t o,const std::string& v,std::string& e){if(o==0x374){group374_=v;e.clear();return true;}auto f=base_.properties().fields;return f.write_string(f.context,o,v,e);}
bool CanonicalSpawnSpotV108::set_position(const std::array<float,3>& p,bool b,std::string& e){if(!init_services_.set_position){e="Required SAME SpawnSpot SetPosition";return false;}return init_services_.set_position(p.data(),b,e);}
bool CanonicalSpawnSpotV108::init_final(bool& eligible,std::string& e){
 eligible=false;if(destruction_started_||!init_services_.check_spawn_probability){e="Required live SpawnSpot CheckSpawnProbability";return false;}
 std::int32_t roll;if(!init_services_.check_spawn_probability(roll,e))return false;
 const auto* probability=base_.integer(0x274);if(!probability){e="Required SAME SpawnSpot probability274";return false;}
 if(roll>=*probability){e.clear();return true;}
 if(!initialization_.init_final(eligible,e))return false;
 //Qualified MeetCondition38ab60 is literal1, independent of disabled81.
 //Do not gate this original registration on base InitFinal's eligible result.
 if(!services_.insert){e="Required actual SpawnGroupManager.InsSpawn";return false;}
 return services_.insert(*this,e);
}
bool CanonicalSpawnSpotV108::place_object(std::uintptr_t id,std::string& e){if(!services_.set_object_position||!services_.set_object_position(id,base_.vector3(0x160),true,e))return false;if(!services_.set_object_rotation){e="Required SpawnSpot Object.SetRotation";return false;}return services_.set_object_rotation(id,base_.vector3(0x16c),e);}
bool CanonicalSpawnSpotV108::destroy(std::string& e){
 if(destruction_started_||destroyed_){e="SpawnSpot D1 cannot replay an interrupted source prefix";return false;}
 destruction_started_=true;
 if(!services_.erase||!services_.erase(*this,e))return false;
 group374_.clear(); //CString D1 follows DelSpawn, before qualified GameObjectD2
 if(!services_.destroy_base){e="Required SAME SpawnSpot qualified GameObjectD2";return false;}
 if(!services_.destroy_base(base_,e))return false;destroyed_=true;e.clear();return true;
}
CanonicalClassReceiverV1 CanonicalSpawnSpotV108::factory_receiver(std::shared_ptr<CanonicalSpawnSpotV108> r,std::shared_ptr<const void> xml){
 auto out=canonical_class_receiver_v1(r);out.source_lease=std::move(xml);
 out.init_post=[r](std::string& e){return r->init_post(e);};out.is_game_object=[](bool& value,std::string& e){value=true;e.clear();return true;};
 out.position=[r](auto& p,std::string& e){std::copy_n(r->base().vector3(0x160),3,p.begin());e.clear();return true;};
 out.set_position=[r](const auto& p,bool b,std::string& e){return r->set_position(p,b,e);};return out;
}
}
