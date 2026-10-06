#include "world_item_pf_connection_v4.hpp"
#include <algorithm>
namespace dh2::character {
WorldItemPfConnectionV4::WorldItemPfConnectionV4(RetainedWorldItemObjectV1& item,
 std::shared_ptr<void> world,const navigation::CollisionWorld* geometry,
 navigation::ObstacleRegistry* obstacles):item_(item),geometry_(geometry),obstacles_(obstacles),world_(std::move(world)){}
bool WorldItemPfConnectionV4::update(std::string& e){
 if(!world_||!geometry_||!obstacles_){e="Required same Item candidate floor/PF registry";return false;}
 auto& base=item_.base();auto& object=item_.runtime().object;
 navigation::ProducerRequest request{geometry_,obstacles_,&object,base.identity(),nullptr};
 navigation::ProducerFields fields{};
 if(object.user){
  fields.type=navigation::ProducerClass::game_object;
  const auto* box=base.absolute_aabb12c();
  std::copy_n(box,2,fields.minimum);std::copy_n(box+3,2,fields.maximum);
  const auto* pointer=base.pointer(0x2dc);
  if(!pointer){e="Required same Item physical2dc backing";return false;}
  if(*pointer){
   if(!physical_||*pointer!=reinterpret_cast<std::uintptr_t>(physical_)||!physical_->native().body){
    e="Required same assigned POItem radius producer";return false;
   }
   fields.physical_present=1;fields.physical_radius=physical_->native().radius;
  }
  request.fields=&fields;
 }
 const int status=dh2_nav_update_game_object(&request);
 if(status){e=status==2?"Required shared Item PF obstacle capacity":"Source Item UpdatePFObject rejected";return false;}
 return true;
}
}
