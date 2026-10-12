#include "canonical_animated_decor_v23.hpp"
#include <algorithm>
#include <utility>

namespace dh2::world {
CanonicalAnimatedDecorV23::CanonicalAnimatedDecorV23(std::shared_ptr<void> world,
 actor::RuntimeState& runtime,GameObjectInitializationServicesV1 initialization,
 AnimatedDecorServicesV23 services):
 CanonicalDecorV15(std::move(world),runtime,std::move(initialization),std::move(services.decor)),
 services_(std::move(services)){
 // Original342680 changes the SAME inherited source375 after Decor stores.
 source_load_floor375()=0;
}
bool CanonicalAnimatedDecorV23::write_string(std::uint32_t offset,const std::string& value,std::string& error){
 if(offset==0x37c){startanim37c_=value;return true;}
 return CanonicalDecorV15::write_string(offset,value,error);
}
bool CanonicalAnimatedDecorV23::init_post(std::string& error){
 if(!services_.whole_init_post){error="Required actual AnimatedDecor InitPost389128";return false;}
 return services_.whole_init_post(*this,error);
}
bool CanonicalAnimatedDecorV23::destroy(std::string& error){
 if(destroyed_){error="AnimatedDecor destruction cannot replay";return false;}
 destroyed_=true;startanim37c_.clear();
 return CanonicalDecorV15::destroy(error);
}
CanonicalClassReceiverV1 CanonicalAnimatedDecorV23::factory_receiver(
 std::shared_ptr<CanonicalAnimatedDecorV23> owner,std::shared_ptr<const void> xml){
 auto result=canonical_class_receiver_v1(owner);result.source_lease=std::move(xml);
 result.init_post=[owner](std::string& error){return owner->init_post(error);};
 result.is_game_object=[](bool& value,std::string&){value=true;return true;};
 result.position=[owner](std::array<float,3>& value,std::string&){std::copy_n(owner->base().vector3(0x160),3,value.begin());return true;};
 result.set_position=[owner](const std::array<float,3>& value,bool destination,std::string& error){return owner->set_position(value,destination,error);};
 return result;
}
}
