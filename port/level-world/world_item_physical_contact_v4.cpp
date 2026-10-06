#include "world_item_physical_contact_v4.hpp"
namespace dh2::character {
bool world_item_physical_contact_v4(WorldItemGraphV3& graph,navigation::PhysicalContact& out,std::string& e){
 navigation::PhysicalContact value{};auto& receiver=graph.receiver_v4();
 const auto* pointer=receiver.base().pointer(0x2dc);const auto* visible=receiver.base().byte(0x80);
 if(!pointer||!visible){e="Required actual Item physical2dc/visible80 for contact query";return false;}
 auto& physical=graph.physical();
 if(*pointer&&*pointer!=reinterpret_cast<std::uintptr_t>(&physical)){e="Item contact physical identity differs from assigned receiver";return false;}
 value.present=physical.native().body!=nullptr;value.disabled=graph.source_filter_disabled26_v4()!=0;
 value.owner_present=1;value.owner_enabled=*visible!=0;
 // Source POItem creates only its sensor at PhysicalObject+1c; +18 remains
 // constructor NULL. Read the actual live shape filter after enable/disable.
 if(auto* sensor=physical.secondary_shape()){
  const auto filter=sensor->GetFilterData();
  value.secondary={filter.groupIndex,filter.categoryBits,filter.maskBits,1};
 }
 out=value;return true;
}
}
