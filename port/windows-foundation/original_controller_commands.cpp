#include "original_controller_commands.hpp"

namespace dh::foundation {
namespace {
struct ObjectBridge {
    const dh2::character::CharacterControlServices16* services;
    bool reached=false;
    static int call(void* p,const dh2::character::ControllerCommandRequest24* request){
        auto& self=*static_cast<ObjectBridge*>(p);self.reached=true;
        return dh2_character_control(request->owner,request->command,request->target,self.services);
    }
};
OriginalCommandResult result(int status,bool reached,const char* operation,std::string& error){
    if(status!=1){error=std::string("Original ")+operation+" boundary unavailable/malformed";
        return {OriginalCommandAdmission::failed,status};}
    error.clear();return {reached?OriginalCommandAdmission::admitted:OriginalCommandAdmission::blocked,status};
}
}
OriginalCommandResult original_controller_object_command(
    const dh2::character::ControllerCommandState32& state,dh2::character::ControllerCommand command,
    std::uintptr_t target,const dh2::character::CharacterControlServices16& services,std::string& error){
    ObjectBridge bridge{&services};
    const dh2::character::ControllerCommandServices16 dispatch{&bridge,ObjectBridge::call};
    const int status=dh2_character_controller_command(&state,command,target,&dispatch);
    return result(status,bridge.reached,"object command",error);
}
OriginalCommandResult original_controller_move_point(const dh2::character::ControllerCommandState32& state,
    const float* point,const dh2::character::CharacterControlServices16& services,std::string& error){
    struct Bridge {const float* point;const dh2::character::CharacterControlServices16* services;bool reached=false;
        static int call(void* p,const dh2::character::ControllerCommandRequest24* command){
            using namespace dh2::character;auto& self=*static_cast<Bridge*>(p);self.reached=true;
            if(!self.services->invoke)return -1;
            CharacterControlResponse16 response{};
            const CharacterControlRequest32 remote{control_is_remotely_updated,0,command->owner,{0,0,0},0};
            if(self.services->invoke(self.services->context,&remote,&response)!=1)return -1;
            if(response.word)return 1;
            if(!self.point)return -1;
            const CharacterControlRequest32 path{control_path_to,0,command->owner,
                {self.point[0],self.point[1],self.point[2]},0};
            return self.services->invoke(self.services->context,&path,&response)==1?1:-1;
        }} bridge{point,&services};
    const dh2::character::ControllerCommandServices16 dispatch{&bridge,Bridge::call};
    const int status=dh2_character_controller_command(&state,dh2::character::controller_move_object,0,&dispatch);
    return result(status,bridge.reached,"point MoveTo",error);
}
OriginalCommandResult original_controller_heading(const dh2::character::ControllerCommandState32& state,
    const float* direction,const OriginalHeadingBodyServices& services,std::string& error){
    struct Bridge {const float* direction;const OriginalHeadingBodyServices* services;bool reached=false;
        static int call(void* p,const dh2::character::ControllerCommandRequest24* command){
            auto& self=*static_cast<Bridge*>(p);self.reached=true;
            return self.direction&&self.services->invoke?
                self.services->invoke(self.services->context,command->owner,self.direction):-1;
        }} bridge{direction,&services};
    const dh2::character::ControllerCommandServices16 dispatch{&bridge,Bridge::call};
    // Only the verified wrapper gate is reused. No LookAt body is invoked.
    const int status=dh2_character_controller_command(&state,dh2::character::controller_look_object,0,&dispatch);
    return result(status,bridge.reached,"heading",error);
}
OriginalCommandResult original_controller_attack(dh2::character::ControllerAttackState32& controller,
    dh2::character::AttackState64* state,std::uintptr_t target,
    const dh2::character::AttackServices16& services,std::string& error){
    struct Bridge {const dh2::character::AttackServices16* services;bool reached=false;
        static void call(void* p,dh2::character::AttackState64* state,
            dh2::character::ControllerAttackState32* controller,
            const dh2::character::AttackRequest32* request,dh2::character::AttackResponse16* response){
            auto& self=*static_cast<Bridge*>(p);self.reached=true;
            self.services->invoke(self.services->context,state,controller,request,response);
        }} bridge{&services};
    // Preserve original malformed-service rejection BEFORE lock/forced gates.
    if(!services.invoke){error="Original attack backend missing";return {OriginalCommandAdmission::failed,1};}
    const dh2::character::AttackServices16 dispatch{&bridge,Bridge::call};
    const int status=dh2_character_cmd_attack(&controller,state,target,&dispatch);
    if(status!=0){error="Original attack boundary malformed/incomplete";return {OriginalCommandAdmission::failed,status};}
    error.clear();return {bridge.reached?OriginalCommandAdmission::admitted:OriginalCommandAdmission::blocked,status};
}
bool original_controller_animation_event(const dh2::character::AnimationEventFacts& facts,
    const dh2::character::AnimationEventServices& services,std::string& error){
    if(dh2_character_animation_event_route(&facts,&services)!=1){error="Original animation event boundary malformed/incomplete";return false;}
    error.clear();return true;
}
} // namespace dh::foundation
