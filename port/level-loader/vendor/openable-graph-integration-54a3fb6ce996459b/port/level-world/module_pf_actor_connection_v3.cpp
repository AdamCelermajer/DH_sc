#include "module_pf_actor_connection_v3.hpp"
#include <algorithm>
namespace dh2::world {
bool ModulePFActorConnectionV3::init(bool flag,const float* position,float radius,std::uintptr_t identity,std::string& e){
 e.clear();if(!world_pin_||!geometry_||!obstacles_||identity!=base_.identity()||position!=base_.vector3(0x160)){
  e="Required SAME Module PF world/object/position owners";return false;
 }
 navigation::ObjectInitRequest request{};request.geometry=geometry_;request.object=&base_.runtime().object;request.user=identity;
 std::copy_n(position,3,request.position);request.radius=radius;request.flying=flag;
 if(dh2_nav_init_object(&request)){e="Source Module PF InitObject rejected";return false;}return true;
}
bool ModulePFActorConnectionV3::update(std::string& e){
 e.clear();if(!world_pin_||!geometry_||!obstacles_){e="Required SAME Module PF world owners";return false;}
 auto& object=base_.runtime().object;
 navigation::ProducerRequest request{};request.geometry=geometry_;request.registry=obstacles_;request.object=&object;request.key=base_.identity();
 if(!object.user){if(dh2_nav_update_game_object(&request)){e="Source Module null-user PF prefix rejected";return false;}return true;}
 // Module uses the actual GameObject b4/b8/bc leaves. The supported resource
 // path has no colbox/PODecor; a reached physical radius needs its real body.
 auto* physical=base_.pointer(0x2dc);if(!physical||*physical){e="Required actual Module physical radius continuation";return false;}
 navigation::ProducerFields fields{};fields.type=navigation::ProducerClass::game_object;
 const auto* box=base_.absolute_aabb12c();std::copy_n(box,2,fields.minimum);std::copy_n(box+3,2,fields.maximum);request.fields=&fields;
 const int result=dh2_nav_update_game_object(&request);if(result){e=result==2?"Module shared obstacle registry capacity required":"Source Module UpdatePFObject rejected";return false;}return true;
}
}
