#include "character_controller_commands.hpp"
#include <cmath>
namespace {
using namespace dh2::character;
bool issue(const CharacterControlServices16* services,std::uint32_t service,
 std::uint32_t argument,std::uintptr_t subject,const float* position,
 CharacterControlResponse16& response){
 if(!services||!services->invoke)return false;
 CharacterControlRequest32 request{service,argument,subject,{0,0,0},0};
 if(position)for(unsigned i=0;i<3;++i)request.position[i]=position[i];
 return services->invoke(services->context,&request,&response)==1;
}
int controller_gate(const ControllerCommandState32* state,std::uint32_t maximum,
                    std::uint32_t command){
 if(!state||!state->controller||command>maximum||state->reserved||
    state->global_blocked>255||state->locked>255||state->forced>255)return -1;
 return !state->forced&&(state->global_blocked||state->locked)?1:0;
}
bool valid_head_facts(std::uintptr_t owner,const CharacterHeadTowardsFacts24* facts){
 return facts&&facts->character==owner&&facts->focus_target_available==1&&facts->reserved==0;
}
int head_towards_point(std::uintptr_t owner,const float point[3],
 const CharacterHeadTowardsFacts24* facts,const CharacterControlServices16* services){
 if(!owner||!point||!services||!services->invoke)return -1;
 CharacterControlResponse16 response{};
 float squared=point[0]*point[0];squared=squared+point[1]*point[1];squared=squared+point[2]*point[2];
 if(squared<=0.0001f){
  if(!issue(services,control_head_null_enabled,0,owner,nullptr,response))return -1;
  if(!response.word)return 1;
  if(!issue(services,control_set_heading_direction,1,owner,point,response))return -1;
  return dh2_character_control(owner,controller_stop,0,services);
 }
 if(!issue(services,control_is_using_skill,0,owner,nullptr,response))return -1;
 if(response.word)return 1;
 if(!issue(services,control_is_casting,0,owner,nullptr,response))return -1;
 if(response.word)return 1;
 if(!valid_head_facts(owner,facts))return -1;
 if(facts->focus_target){
  if(!issue(services,control_target_position,0,facts->focus_target,nullptr,response))return -1;
  const float focus_position[3]{response.position[0],response.position[1],response.position[2]};
  if(!issue(services,control_target_position,0,owner,nullptr,response))return -1;
  const float focus_vector[3]{focus_position[0]-response.position[0],
   focus_position[1]-response.position[1],focus_position[2]-response.position[2]};
  float dot=point[0]*focus_vector[0];dot=dot+point[1]*focus_vector[1];dot=dot+point[2]*focus_vector[2];
  float first=point[0]*point[0];first=first+point[1]*point[1];first=first+point[2]*point[2];
  float second=focus_vector[0]*focus_vector[0];second=second+focus_vector[1]*focus_vector[1];second=second+focus_vector[2]*focus_vector[2];
  const float cosine=dot/(std::sqrt(first)*std::sqrt(second));
  if(std::acos(cosine)>1.5708f){
   if(!issue(services,control_clear_ai_target,0,owner,nullptr,response))return -1;
   if(!issue(services,control_sync_last_ai_target,0,owner,nullptr,response))return -1;
  }
 }
 if(!issue(services,control_set_heading_direction,1,owner,point,response))return -1;
 return issue(services,control_raise_event,0,owner,nullptr,response)?1:-1;
}
}
extern "C" int dh2_character_controller_command(
 const dh2::character::ControllerCommandState32* state,std::uint32_t command,
 std::uintptr_t target,const dh2::character::ControllerCommandServices16* services){
 using namespace dh2::character;
 if(!state||!services||!state->controller||command>controller_head_towards_object||state->reserved||
    state->global_blocked>255||state->locked>255||state->forced>255)return -1;
 if(!state->forced&&(state->global_blocked||state->locked))return 1;
 if(!state->owner||!services->invoke)return -1;
 const ControllerCommandRequest24 request{command,0,state->owner,target};
 return services->invoke(services->context,&request)==1?1:-1;
}
extern "C" int dh2_character_controller_point_command(
 const dh2::character::ControllerCommandState32* state,std::uint32_t command,
 const float point[3],const dh2::character::ControllerPointCommandServices16* services){
 using namespace dh2::character;
 if(!state||!services||!state->controller||command>controller_head_to_point||state->reserved||
    state->global_blocked>255||state->locked>255||state->forced>255)return -1;
 if(!state->forced&&(state->global_blocked||state->locked))return 1;
 if(!state->owner||!point||!services->invoke)return -1;
 const ControllerPointCommandRequest32 request{command,0,state->owner,
  {point[0],point[1],point[2]},0};
 return services->invoke(services->context,&request)==1?1:-1;
}
extern "C" int dh2_character_control(std::uintptr_t owner,std::uint32_t command,
 std::uintptr_t target,const dh2::character::CharacterControlServices16* services){
 using namespace dh2::character;
 if(!owner||!services||!services->invoke||command>controller_stop)return -1;
 auto call=[&](std::uint32_t service,std::uint32_t argument,std::uintptr_t subject,
               const float* position,CharacterControlResponse16& response){
  CharacterControlRequest32 request{service,argument,subject,{0,0,0},0};
  if(position)for(unsigned i=0;i<3;++i)request.position[i]=position[i];
  return services->invoke(services->context,&request,&response)==1;
 };
 CharacterControlResponse16 response{};
 if(command==controller_look_object){
  if(!target)return 1;
  if(!call(control_target_position,0,target,nullptr,response))return -1;
  const float position[3]{response.position[0],response.position[1],response.position[2]};
  return call(control_look_at_point,0,owner,position,response)?1:-1;
 }
 if(!call(control_is_remotely_updated,0,owner,nullptr,response))return -1;
 if(response.word)return 1;
 if(command==controller_move_object){
  if(!target)return 1;
  if(!call(control_target_position,0,target,nullptr,response))return -1;
  const float position[3]{response.position[0],response.position[1],response.position[2]};
  return call(control_path_to,0,owner,position,response)?1:-1;
 }
 if(!call(control_stop_object,0,owner,nullptr,response))return -1;
 return call(control_character_event,0x3f,owner,nullptr,response)?1:-1;
}
extern "C" int dh2_character_controller_character(
 const dh2::character::ControllerCommandState32* state,std::uint32_t command,
 std::uintptr_t target,const dh2::character::CharacterControlServices16* services){
 const dh2::character::ControllerCommandServices16 dispatch{
  const_cast<dh2::character::CharacterControlServices16*>(services),
  [](void* context,const dh2::character::ControllerCommandRequest24* request){
   return dh2_character_control(request->owner,request->command,request->target,
    static_cast<const dh2::character::CharacterControlServices16*>(context));
  }};
 return dh2_character_controller_command(state,command,target,&dispatch);
}
extern "C" int dh2_character_controller_character_point(
 const dh2::character::ControllerCommandState32* state,const float point[3],
 const dh2::character::CharacterHeadTowardsFacts24* facts,
 const dh2::character::CharacterControlServices16* services){
 using namespace dh2::character;
 const int gate=controller_gate(state,controller_head_towards_point,controller_head_towards_point);
 if(gate)return gate;
 return head_towards_point(state->owner,point,facts,services);
}
extern "C" int dh2_character_controller_character_head_towards(
 const dh2::character::ControllerCommandState32* state,std::uintptr_t target,
 const dh2::character::CharacterHeadTowardsFacts24* facts,
 const dh2::character::CharacterControlServices16* services){
 using namespace dh2::character;
 const int gate=controller_gate(state,controller_head_towards_object,controller_head_towards_object);
 if(gate)return gate;
 if(!state->owner||!services||!services->invoke)return -1;
 CharacterControlResponse16 response{};
 if(!issue(services,control_is_remotely_updated,0,state->owner,nullptr,response))return -1;
 if(response.word)return 1;
 float direction[3]{0,0,0};
 if(target){
  if(!issue(services,control_target_position,0,target,nullptr,response))return -1;
  const float target_position[3]{response.position[0],response.position[1],response.position[2]};
  if(!issue(services,control_target_position,0,state->owner,nullptr,response))return -1;
  for(unsigned i=0;i<3;++i)direction[i]=target_position[i]-response.position[i];
 }else{
  if(!issue(services,control_head_null_enabled,0,state->owner,nullptr,response))return -1;
  if(!response.word)return 1;
 }
 return head_towards_point(state->owner,direction,facts,services);
}
