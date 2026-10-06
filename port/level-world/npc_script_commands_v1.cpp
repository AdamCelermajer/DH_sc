#include "npc_script_commands_v1.hpp"
#include <stdexcept>
#include <cstring>
namespace dh2::character {
NpcScriptCommandsV1::NpcScriptCommandsV1(NpcScriptCommandBorrowV1 b,NpcScriptCommandServicesV1 s):b_(b),s_(s){
 if(!b_.identity||!b_.controller||!b_.runtime||!b_.target||!b_.actual_position||!b_.source_vec3_k||!b_.static84||!b_.path_limit26c||!b_.heading||!b_.floors||!s_.remote||!s_.position||!s_.look_vector||!s_.path_policy)throw std::invalid_argument("Required same NPC command/PF ownership");
 auto& path=b_.runtime->path;if(path.count||path.segments)throw std::invalid_argument("NPC command owner must adopt fresh source route storage once");
 if(!b_.floors->sewn)throw std::invalid_argument("Required complete actual floor navigation graph");
 segments_.resize(b_.floors->graph.node_count+1);path.segments=segments_.data();path.capacity=std::uint32_t(segments_.size());
 route_ids_.resize(segments_.size());route_result_.search.path=route_ids_.data();route_result_.search.path_capacity=std::uint32_t(route_ids_.size());
 bindings_={&state_,{this,service,number},nullptr};if(!refresh())throw std::invalid_argument(error_);
}
bool NpcScriptCommandsV1::refresh(){
 if(b_.controller->owner!=b_.identity||!b_.target->owner||b_.target->owner->identity!=b_.identity){error_="NPC command target/controller ownership mismatch";return false;}
 state_={b_.identity,b_.controller->controller,b_.target->target,b_.actual_position,b_.source_vec3_k,b_.runtime->path.count,0};return true;
}
bool NpcScriptCommandsV1::suspend_navigation(){
 if(dh2_nav_drop_path(&b_.runtime->path)){error_="Cannot retire NPC route storage";return false;}
 pf_ready_=false;b_.floors=nullptr;route_result_.found=0;return refresh();
}
bool NpcScriptCommandsV1::rebind_floor(floors::World* floor){
 if(pf_ready_||b_.runtime->path.count){error_="Suspend NPC navigation before floor replacement";return false;}
 if(!floor||!floor->sewn){error_="Required complete replacement NPC floor";return false;}
 std::vector<navigation::PathSegment> segments(floor->graph.node_count+1);
 std::vector<std::uint32_t> ids(segments.size());
 segments_.swap(segments);route_ids_.swap(ids);
 auto& path=b_.runtime->path;path.segments=segments_.data();path.capacity=std::uint32_t(segments_.size());
 route_result_={};route_result_.search.path=route_ids_.data();route_result_.search.path_capacity=std::uint32_t(route_ids_.size());
 b_.floors=floor;return refresh();
}
bool NpcScriptCommandsV1::permitted()const noexcept{return b_.controller->forced||(!b_.controller->global_blocked&&!b_.controller->locked);}
int NpcScriptCommandsV1::number(void* p,const dh2_script_value* value,float* out){
 auto& t=*static_cast<NpcScriptCommandsV1*>(p);return t.s_.number?t.s_.number(t.s_.context,value,out):-1;
}
int NpcScriptCommandsV1::service(void* p,ScriptCommandState48* state,const ScriptCommandRequest40* q,const float** out){
 auto& t=*static_cast<NpcScriptCommandsV1*>(p);if(state!=&t.state_||!q||!out||!t.refresh())return -1;
 const int status=t.deliver(*q,out);t.refresh();if(status&&t.error_.empty())t.error_="Required actual NPC command service "+std::to_string(q->service);return status;
}
int NpcScriptCommandsV1::find_path(void* p,const PathToRequest32* q,std::uint32_t* out){
 auto& t=*static_cast<NpcScriptCommandsV1*>(p);if(!q||!out||q->owner!=t.b_.identity)return -1;
 auto& r=*t.b_.runtime;
 if(!t.pf_ready_||!t.b_.floors){t.error_="NPC navigation suspended or awaiting actual PF initialization";return -1;}
 auto& world=*t.b_.floors;
 // SAME PFObject split logical projections: refresh real flags/radius/position
 // before native PFWorld FindPath; retained route owns only its path storage.
 if(r.object.user!=t.b_.identity){t.error_="Required actual source InitPFObject before NPC FindPath";return -1;}
 r.path.route.flags=r.object.motion.flags;r.path.route.radius=r.object.radius;
 std::memcpy(r.path.position,t.b_.actual_position,12);
 navigation::FindRequest request{&world.route_world,&world.collision_world,&r.path,&t.route_result_,&world.route_workspace,{},q->limit,0,0};
 std::memcpy(request.target,q->target,12);
 const navigation::FindSourceServicesV1 services{t.s_.context,t.s_.path_policy};
 const int status=dh2_nav_find_path_source_v1(&request,&services);if(status){t.error_="Actual NPC PFWorld FindPath failed "+std::to_string(status);return -1;}
 *out=t.route_result_.found;return 0;
}
int NpcScriptCommandsV1::path_to(const float* point){
 auto& p=b_.runtime->path;PathToState40 state{b_.identity,*b_.static84,p.count!=0,*b_.path_limit26c,0,{},0};
 std::memcpy(state.path_target,p.target,12);PathToResult16 out{};const PathToServices16 services{this,find_path};
 return dh2_character_path_to(&out,&state,point,&services)==0?0:-1;
}
int NpcScriptCommandsV1::deliver(const ScriptCommandRequest40& q,const float** out){
 switch(q.service){
 case script_target_position:return s_.position(s_.context,q.subject,*out);
 case script_look_vector:return s_.look_vector(s_.context,*out);
 case script_controller_stop:return b_.heading->command_stop(error_)?0:-1;
 case script_controller_head_point:return b_.heading->command_head_towards(q.point,error_)?0:-1;
 case script_controller_attack:return s_.attack?s_.attack(s_.context,q.target,bindings_.scope):-1;
 case script_controller_move_object:
 case script_controller_move_point:{
  if(!permitted())return 0;
  bool remote{};if(s_.remote(s_.context,remote))return -1;if(remote)return 0;
  if(q.service==script_controller_move_object){if(!q.target)return 0;const float* position{};if(s_.position(s_.context,q.target,position)||!position)return -1;return path_to(position);}
  return path_to(q.point);
 }
 default:return -1;
 }
}
bool NpcScriptCommandsV1::initialize_pf(navigation::ObstacleRegistry& registry,const navigation::ProducerFields& fields){
 auto& r=*b_.runtime;
 if(pf_ready_){error_="NPC source PF initialization already adopted";return false;}
 if(!b_.floors){error_="NPC floor is suspended";return false;}
 if(r.object.user&&r.object.user!=b_.identity){error_="NPC PFObject belongs to a different actor";return false;}
 auto& geometry=b_.floors->collision_world;
 const auto capabilities=r.object.motion.flags;
 if(dh2_nav_object_defaults(&r.object)){error_="Actual source PFObject constructor failed";return false;}
 // Registry/floor retirement clears parent/cache bookkeeping, not actual
 // SetFlying/SetSwimming capabilities belonging to the retained actor.
 if(pf_constructed_)r.object.motion.flags=capabilities;
 pf_constructed_=true;
 const float x=fields.maximum[0]-fields.minimum[0],y=fields.maximum[1]-fields.minimum[1];
 // GameObject InitPost38cd94..38cde4 initializes full maximum XY extent;
 // UpdatePFObject subsequently produces the actual obstacle/body radius.
 navigation::ObjectInitRequest init{&geometry,&r.object,b_.identity,{},x<y?y:x,*b_.static84,0};std::memcpy(init.position,b_.actual_position,12);
 if(dh2_nav_init_object(&init)){error_="Actual NPC PFObject InitObject failed";return false;}
 const navigation::ProducerRequest update{&geometry,&registry,&r.object,b_.identity,&fields};
 if(dh2_nav_update_game_object(&update)){error_="Actual NPC UpdatePFObject bounds/body/registry failed";return false;}
 pf_ready_=true;return true;
}
}
