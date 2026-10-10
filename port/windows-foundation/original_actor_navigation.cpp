#include "original_actor_navigation.hpp"
#include "../level-world/module_floor_append_v2.hpp"
#include <cmath>
#include <algorithm>
#include "../level-world/move_state.hpp"
namespace dh::foundation {
bool append_original_module_floors(const dh2::resources::BresView&view,const dh2::scene::Scene&scene,const std::vector<OriginalSourceFloorBinding>&bindings,dh2::floors::World&world,std::string&e){
 if(world.sewn||bindings.empty()){e="Source floor append requires unsewn world and actual bindings";return false;}
 for(const auto&b:bindings){if(b.instance>=scene.instances.size()||b.room>=512){e="Source floor binding out of range";return false;}for(float v:b.mesh_local_quaternion)if(!std::isfinite(v)){e="Nonfinite actual mesh quaternion";return false;}for(float v:b.mesh_local_scale)if(!std::isfinite(v)){e="Nonfinite actual mesh scale";return false;}
  if(!dh2::world::module_floor_append_pose_v2(view,scene,scene.instances[b.instance],b.room,b.mesh_local_quaternion.data(),b.mesh_local_scale.data(),world,e))return false;
 }e.clear();return true;
}
OriginalActorNavigation::OriginalActorNavigation(OriginalActorNavigationBindings b):bindings_(std::move(b)){}
bool OriginalActorNavigation::construct_defaults(std::string&e){if(constructed_||!bindings_.actor_lease||!bindings_.object){e="PF constructor requires fresh SAME actor/PFObject";return false;}if(dh2_nav_object_defaults(bindings_.object)){e="Original PFObject constructor failed";return false;}constructed_=true;e.clear();return true;}
bool OriginalActorNavigation::attach_world(OriginalActorNavigationWorldBindings world,const float* relative,const float* absolute,std::string& error) {
 if(!constructed_||initialized_||world_backing_.floor_lease||!world.floor_lease||!world.obstacle_lease||!world.floors||!world.floors->sewn||!world.obstacles||!relative||!absolute){error="Late navigation attachment requires fresh SAME PFObject and actual sewn floor/obstacle leases/bounds";return false;}
 bindings_.floors=world.floors;bindings_.obstacles=world.obstacles;bindings_.relative144=relative;bindings_.absolute12c=absolute;world_backing_=std::move(world);error.clear();return true;
}
bool OriginalActorNavigation::initialize_pf(std::string&e){
 if(!constructed_||initialized_||!bindings_.actor_lease||!bindings_.world_lease||!bindings_.identity||!bindings_.position160||!bindings_.relative144||!bindings_.floors||!bindings_.floors->sewn||!bindings_.static84){e="Original InitPF requires SAME source fields and completed actual floor world";return false;}
 std::uint8_t stat=0;if(!bindings_.static84(stat,e))return false;const auto*b=bindings_.relative144;const float x=b[3]-b[0],y=b[4]-b[1];
 dh2::navigation::ObjectInitRequest q{&bindings_.floors->collision_world,bindings_.object,bindings_.identity,{bindings_.position160[0],bindings_.position160[1],bindings_.position160[2]},x<y?y:x,unsigned(stat!=0),0};
 if(dh2_nav_init_object(&q)){e="Original PFWorld.InitObject rejected actual fields/floors";return false;}initialized_=true;e.clear();return true;
}
bool OriginalActorNavigation::update_pf(std::uintptr_t id,const dh2::physical::NativeBody&body,const dh2::physical::CharacterBodyConfig&,std::string&e){
 if(!constructed_||!bindings_.actor_lease||!bindings_.object||id!=bindings_.identity){e="Required SAME constructed actor/PFObject callback";return false;}
 // Exact user0 early-return precedes geometry/field/registry validation.
 if(!bindings_.object->user){dh2::navigation::ProducerRequest q{};q.object=bindings_.object;const int status=dh2_nav_update_game_object(&q);if(status){e="Original constructor-null PF producer rejected receiver";return false;}e.clear();return true;}
 if(!initialized_||!bindings_.world_lease||!bindings_.floors||!bindings_.floors->sewn||!bindings_.obstacles||!bindings_.absolute12c){e="Original UpdatePF requires initialized source floors/obstacle/bounds owner";return false;}
 // Character source UpdatePFObject393ea0: absent floor returns before traits.
 if(bindings_.object->motion.floor==UINT32_MAX){e.clear();return true;}
 dh2::navigation::ProducerFields fields{};fields.type=dh2::navigation::ProducerClass::character;fields.physical_present=body.body!=nullptr;fields.physical_radius=body.radius;fields.minimum[0]=bindings_.absolute12c[0];fields.minimum[1]=bindings_.absolute12c[1];fields.maximum[0]=bindings_.absolute12c[3];fields.maximum[1]=bindings_.absolute12c[4];
 dh2::navigation::ProducerRequest q{&bindings_.floors->collision_world,bindings_.obstacles,bindings_.object,bindings_.identity,&fields};const int status=dh2_nav_update_game_object(&q);if(status){e="Original UpdatePF source obstacle/radius producer failed: "+std::to_string(status);return false;}e.clear();return true;
}
std::function<bool(std::uintptr_t,const dh2::physical::NativeBody&,const dh2::physical::CharacterBodyConfig&,std::string&)> OriginalActorNavigation::physical_callback(){return [this](auto id,const auto&body,const auto&config,auto&e){return update_pf(id,body,config,e);};}
bool OriginalActorNavigation::validate_direction(Vec3 current,Vec3 delta,Vec3& admitted,bool& allowed,std::string& error) {
 if(!initialized_||!bindings_.floors||!bindings_.floors->sewn||!bindings_.object||!bindings_.position160){error="Direction validation requires actual initialized PF/floor owner";return false;}
 const float position[]{current.x,current.y,current.z};float direction[]{delta.x,delta.y,delta.z};
 for(unsigned k=0;k<3;++k)if(position[k]!=bindings_.position160[k]){error="Motion current position must match SAME admitted actor160";return false;}
 const dh2::navigation::DirectionRequest request{&bindings_.floors->collision_world,position,bindings_.object->radius,bindings_.object->motion.flags,0};std::uint32_t accepted=0;
 const auto status=dh2_nav_validate_direction(&accepted,direction,&request);if(status){error="Original direction validation malformed: "+std::to_string(status);return false;}
 admitted={direction[0],direction[1],direction[2]};allowed=accepted!=0;error.clear();return true;
}
bool OriginalActorNavigation::validate_position(Vec3 requested,OriginalActorNavigationMoveResult& output,std::string& error) {
 if(!initialized_||!bindings_.floors||!bindings_.floors->sewn||!bindings_.obstacles||!bindings_.object||!bindings_.position160){error="Position validation requires SAME initialized floor/PF/obstacle owner";return false;}
 float point[]{requested.x,requested.y,requested.z};dh2::navigation::PositionResult result{};
 const dh2::navigation::ObjectPositionRequest request{&bindings_.floors->collision_world,bindings_.obstacles,bindings_.object,bindings_.identity,point,&bindings_.floors->source_motion_policy_v95};
 const auto status=dh2_nav_validate_object_position(&result,&request);if(status){error="Original object position admission malformed/service failure: "+std::to_string(status);return false;}
 OriginalActorNavigationMoveResult value;value.position={point[0],point[1],point[2]};value.position_valid=result.valid!=0;value.kind=result.kind;value.grounded=bindings_.object->motion.floor!=UINT32_MAX;output=value;error.clear();return true;
}
bool OriginalActorNavigation::update_path(OriginalActorPathBindings& source,dh2::navigation::ControllerResult& output,std::string& error) {
 if(!constructed_||!bindings_.actor_lease||!bindings_.object||!bindings_.position160||!source.controller_lease||!source.controller||!source.path||!source.workspace||!source.source_policy||!source.publish){error="Source UpdatePath requires SAME actor/PF and borrowed controller/path/workspace/policy/publication";return false;}
 // Same GameObject current position, projected into its borrowed coordinator.
 std::copy_n(bindings_.position160,3,source.controller->position);
 std::copy_n(bindings_.position160,3,bindings_.object->motion.position);
 dh2::navigation::ControllerPolicy policy{};if(!source.source_policy(policy,error))return false;
 if(policy.update_path&&(policy.avoid_obstacles||policy.update_physics)&&!source.avoidance){error="Source UpdatePath policy requires actual avoidance/physical-presence scene";return false;}
 struct Context {OriginalActorPathBindings* source;std::string* error;};Context context{&source,&error};
 const dh2::navigation::ControllerSourceServicesV69 services{&context,[](void* raw,std::uint32_t* value){auto& c=*static_cast<Context*>(raw);if(!c.source->skip_boundary){*c.error="Reached source boundary Debug provider unbound";return -1;}return c.source->skip_boundary(*value,*c.error)?0:-1;}};
 const dh2::navigation::ControllerRequest request{source.controller,source.path,bindings_.object,bindings_.floors?&bindings_.floors->collision_world:nullptr,bindings_.floors?&bindings_.floors->graph:nullptr,source.avoidance,&policy,source.workspace,bindings_.identity};
 dh2::navigation::ControllerResult result{};const auto status=dh2_nav_update_path_source_v69(&result,&request,&services);if(status){if(error.empty())error="Original UpdatePath source coordinator failed: "+std::to_string(status);return false;}
 if(result.physical_stop_requested&&(!source.physical_stop||!source.physical_stop(error))){if(error.empty())error="Reached source physical Stop provider unbound";return false;}
 if(!source.publish(error))return false;output=result;error.clear();return true;
}
bool OriginalActorNavigation::update_manual_heading(const OriginalManualHeadingBindings& source,OriginalManualHeadingResult& output,std::string& error) {
 if(!constructed_||!bindings_.actor_lease||!bindings_.object||!bindings_.position160||!source.controller_lease||!source.source_policy){error="Manual source heading requires SAME actor/PF/controller and actual flags/boundary provider";return false;}
 // This source UpdatePath prefix executes even when IsUpdatingPath is false.
 std::copy_n(bindings_.position160,3,bindings_.object->motion.position);
 std::uint32_t flags=0,boundary=0;if(!source.source_policy(flags,boundary,error))return false;
 dh2::move::Policy policy{};if(boundary>1||dh2_move_policy(&policy,&flags)){error="Actual source heading policy invalid";return false;}
 OriginalManualHeadingResult result;result.path_policy_enabled=policy.update_path!=0;
 if(!policy.update_path){output=result;error.clear();return true;}
 if(!source.read_heading||!source.read_heading(result.heading,error)){if(error.empty())error="Required actual manual controller heading borrow";return false;}
 auto& heading=result.heading;
 if(heading.active>1||heading.reserved||!std::isfinite(heading.angle)){error="Malformed source manual heading fields";return false;}
 for(float component:heading.direction)if(!std::isfinite(component)){error="Nonfinite source manual heading";return false;}
 if(heading.active){
  bindings_.object->motion.object_flags|=2;
  if(boundary){
   std::uint32_t skip=0;if(!source.skip_boundary||!source.skip_boundary(skip,error)){if(error.empty())error="Reached original manual-heading boundary Debug provider unbound";return false;}
   if(skip>1){error="Invalid source boundary Debug result";return false;}
   if(!skip){
    if(!initialized_||!bindings_.floors||!bindings_.floors->sewn){error="Reached original manual-heading boundary requires actual initialized floor world";return false;}
    const dh2::navigation::DirectionRequest request{&bindings_.floors->collision_world,bindings_.object->motion.position,bindings_.object->radius,bindings_.object->motion.flags,0};
    if(dh2_nav_validate_direction(&result.direction_valid,heading.direction,&request)){error="Original manual-heading direction kernel malformed";return false;}
    result.boundary_checked=true;
    // Original call is unconditional after ValidateDirection, including false.
    if(dh2_nav_set_heading(&heading,heading.direction,1)){error="Original manual-heading final SetHeadingDirection failed";return false;}
    if(!source.publish_heading||!source.publish_heading(heading,error)){if(error.empty())error="Required SAME host manual heading publication";return false;}
   }
  }
 }else bindings_.object->motion.object_flags&=~2u;
 output=result;error.clear();return true;
}
}
