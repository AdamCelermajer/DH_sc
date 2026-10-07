#include "character_controller_commands.hpp"
extern "C" int dh2_character_controller_command(
 const dh2::character::ControllerCommandState32* state,std::uint32_t command,
 std::uintptr_t target,const dh2::character::ControllerCommandServices16* services){
 using namespace dh2::character;
 if(!state||!services||!state->controller||command>controller_stop||state->reserved||
    state->global_blocked>255||state->locked>255||state->forced>255)return -1;
 if(!state->forced&&(state->global_blocked||state->locked))return 1;
 if(!state->owner||!services->invoke)return -1;
 const ControllerCommandRequest24 request{command,0,state->owner,target};
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
