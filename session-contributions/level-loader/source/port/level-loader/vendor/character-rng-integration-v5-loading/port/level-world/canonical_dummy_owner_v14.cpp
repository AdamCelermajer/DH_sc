#include "canonical_dummy_owner_v14.hpp"
#include <algorithm>
#include <stdexcept>
namespace dh2::world {
CanonicalDummyOwnerV14::CanonicalDummyOwnerV14(std::shared_ptr<void> pin,actor::RuntimeState& runtime,
 GameObjectInitializationServicesV1 services,DummyContinuationServicesV14 continuation)
 :base_(reinterpret_cast<std::uintptr_t>(this),20,std::move(pin),runtime),services_(std::move(services)),initialization_(base_,services_),continuation_(std::move(continuation)){
 // Exact sole non-vptr Dummy constructor continuation3410d4.
 base_.lifecycle().static84=1;
}
bool CanonicalDummyOwnerV14::set_position(const std::array<float,3>& position,bool update,std::string& e){
 if(destroyed_){e="Required live SAME Dummy receiver";return false;}
 if(!services_.set_position){e="Required actual Dummy inherited GameObject SetPosition";return false;}
 return services_.set_position(position.data(),update,e);
}
bool CanonicalDummyOwnerV14::destroy(std::string& e){
 if(destroyed_){e="Dummy source destruction cannot replay";return false;}
 if(!continuation_.destroy_base){e="Required actual Dummy inherited GameObject destruction";return false;}
 // Mark before external destructive delivery: partial teardown cannot repeat.
 destroyed_=true;return continuation_.destroy_base(base_,e);
}
CanonicalClassReceiverV1 CanonicalDummyOwnerV14::factory_receiver(std::shared_ptr<CanonicalDummyOwnerV14> owner,std::shared_ptr<const void> xml){
 if(!owner)throw std::invalid_argument("Required actual Dummy constructor receiver");
 auto receiver=canonical_class_receiver_v1(owner);receiver.source_lease=std::move(xml);
 receiver.init_post=[owner](std::string& e){bool eligible{};return owner->init_post(eligible,e);}; // source InitPost has no filtering/publication substitute
 receiver.is_game_object=[](bool& out,std::string&){out=true;return true;}; // proven actual GameObjectC1 ancestry
 receiver.position=[owner](std::array<float,3>& out,std::string& e){auto* actual=owner->base_.vector3(0x160);if(!actual){e="Required SAME Dummy position160";return false;}std::copy(actual,actual+3,out.begin());return true;};
 receiver.set_position=[owner](const auto& value,bool update,std::string& e){return owner->set_position(value,update,e);};
 return receiver;
}
}
