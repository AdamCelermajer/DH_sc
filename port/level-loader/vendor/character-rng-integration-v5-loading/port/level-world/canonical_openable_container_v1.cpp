#include "canonical_openable_container_v1.hpp"
#include <stdexcept>
namespace dh2::world {
ContainerNetStructV1::ContainerNetStructV1(){
 // Actual empty tree header: left/right point to this owner's source10c.
 left114_=right118_=reinterpret_cast<std::uintptr_t>(&byte10c_);
 // Source39ff58 follows NetStruct8138f4 and DeclareMember81324c in this order.
 for(std::size_t i=0;i<members_.size();++i){members_[i].mask=i==0?1u:i==1?16u:32u;declared_[count104_++]=&members_[i];}
}
CanonicalOpenableContainerV1::CanonicalOpenableContainerV1(std::shared_ptr<void> pin,
 actor::RuntimeState& runtime,OpenableContainerServicesV1 services)
 :base_(reinterpret_cast<std::uintptr_t>(this),7,pin,runtime),
 property_(fields_,base_.properties().fields,pin),receiver_(fields_,std::move(services)){
 auto* byte28=base_.byte(0x28);auto* bytef8=base_.byte(0xf8);
 auto* net100=base_.pointer(0x100);auto* net104=base_.pointer(0x104);
 if(!byte28||!bytef8||!net100||!net104)throw std::runtime_error("Required SAME canonical Container constructor fields");
 *byte28=1;base_.lifecycle().static84=0;*bytef8=2;
 *net100=reinterpret_cast<std::uintptr_t>(&network_[0]);
 *net104=reinterpret_cast<std::uintptr_t>(&network_[1]);
}
CanonicalPropertyActorV1 CanonicalOpenableContainerV1::properties()noexcept{
 auto actor=base_.properties();actor.fields=property_.services();return actor;
}
}
