#include "../original_controller_commands.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh::foundation;
using namespace dh2::character;
static void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
struct Backend {
    bool remote=false;std::vector<unsigned> calls;float path[3]{};
    static int control(void* p,const CharacterControlRequest32* q,CharacterControlResponse16* out){
        auto& self=*static_cast<Backend*>(p);self.calls.push_back(q->service);
        if(q->service==control_is_remotely_updated)out->word=self.remote;
        if(q->service==control_target_position){out->position[0]=4;out->position[1]=5;out->position[2]=6;}
        if(q->service==control_path_to)for(unsigned i=0;i<3;++i)self.path[i]=q->position[i];
        if(q->service==control_character_event)check(q->argument==0x3f,"Stop event differs");
        return 1;
    }
    static int heading(void* p,std::uintptr_t owner,const float* direction){
        auto& self=*static_cast<Backend*>(p);check(owner==2&&direction,"heading body projection differs");
        self.calls.push_back(100);return 1;
    }
    static void attack(void* p,AttackState64*,ControllerAttackState32*,const AttackRequest32* q,AttackResponse16* out){
        auto& self=*static_cast<Backend*>(p);self.calls.push_back(q->service);
        if(q->service==attack_network_mode)out->word=0; // explicitly offline fixture backend
    }
    static std::int32_t animation(void* p,const AnimationEventRequest* q){
        auto& self=*static_cast<Backend*>(p);self.calls.push_back(q->service);
        if(q->service==animation_state_getter)return 5;
        return 1;
    }
};
int main(){try{
    std::string error;Backend backend;
    CharacterControlServices16 control{&backend,Backend::control};
    OriginalHeadingBodyServices heading{&backend,Backend::heading};
    AttackServices16 attack{&backend,Backend::attack};
    AnimationEventServices animation{&backend,Backend::animation};
    const float point[]={7,8,9};
    for(unsigned flags=0;flags<8;++flags){
        const unsigned global=flags&1,locked=(flags>>1)&1,forced=(flags>>2)&1;
        const bool admitted=forced||(!global&&!locked);
        ControllerCommandState32 state{1,2,global,locked,forced,0};
        for(auto command:{controller_move_object,controller_stop,controller_look_object}){
            backend.calls.clear();auto result=original_controller_object_command(state,command,3,control,error);
            check(result.admission==(admitted?OriginalCommandAdmission::admitted:OriginalCommandAdmission::blocked),"object gate differs");
            check(backend.calls.empty()!=admitted,"blocked command dispatched services");
        }
        backend.calls.clear();auto moved=original_controller_move_point(state,point,control,error);
        check(moved.admission==(admitted?OriginalCommandAdmission::admitted:OriginalCommandAdmission::blocked),"point gate differs");
        if(admitted)check(backend.calls==std::vector<unsigned>{control_is_remotely_updated,control_path_to}&&backend.path[0]==7,"point remote/path order differs");
        backend.calls.clear();auto headed=original_controller_heading(state,point,heading,error);
        check(headed.admission==(admitted?OriginalCommandAdmission::admitted:OriginalCommandAdmission::blocked),"heading gate differs");
        ControllerAttackState32 attack_controller{1,2,global,locked,forced,0};AttackState64 ai{};ai.owner=2;
        backend.calls.clear();auto attacked=original_controller_attack(attack_controller,&ai,3,attack,error);
        check(attacked.admission==(admitted?OriginalCommandAdmission::admitted:OriginalCommandAdmission::blocked),"attack gate differs");
        if(admitted)check(backend.calls==std::vector<unsigned>{attack_network_mode,attack_controllable_dispatch},"attack network/dispatch order differs");
        backend.calls.clear();check(original_controller_animation_event({0x22,global,locked,forced,19},animation,error),"animation22 failed");
        check(backend.calls==std::vector<unsigned>{animation_end_virtual,animation_state_event},"locked animation end was cleared");
        backend.calls.clear();check(original_controller_animation_event({0x26,global,locked,forced,0},animation,error),"animation26 failed");
        check(backend.calls==(admitted?std::vector<unsigned>{animation_state_getter,animation_attack_begin,animation_state_event}:
            std::vector<unsigned>{animation_state_event}),"animation gating/state forwarding differs");
    }
    ControllerCommandState32 state{1,2,0,0,0,0};
    backend.calls.clear();check(original_controller_object_command(state,controller_stop,0,control,error).admission==OriginalCommandAdmission::admitted,"Stop failed");
    check(backend.calls==std::vector<unsigned>{control_is_remotely_updated,control_stop_object,control_character_event},"Stop body/event order differs");
    backend.calls.clear();check(original_controller_object_command(state,controller_move_object,0,control,error).admission==OriginalCommandAdmission::admitted,"null move failed");
    check(backend.calls==std::vector<unsigned>{control_is_remotely_updated},"remote check must precede null target");
    backend.remote=true;backend.calls.clear();original_controller_object_command(state,controller_stop,0,control,error);
    check(backend.calls==std::vector<unsigned>{control_is_remotely_updated},"remote Stop mutated body/event");
    backend.calls.clear();original_controller_move_point(state,nullptr,control,error);
    check(backend.calls==std::vector<unsigned>{control_is_remotely_updated},"remote point command read point too early");
    backend.remote=false;state.global_blocked=255;state.owner=0;backend.calls.clear();
    check(original_controller_object_command(state,controller_stop,0,{},error).admission==OriginalCommandAdmission::blocked,"blocked missing owner/service incorrectly failed");
    check(original_controller_heading(state,nullptr,{},error).admission==OriginalCommandAdmission::blocked,"blocked missing heading body incorrectly failed");
    state.forced=255;check(original_controller_heading(state,nullptr,{},error).admission==OriginalCommandAdmission::failed,"forced missing body accepted");
    ControllerAttackState32 malformed{1,2,1,0,0,0};backend.calls.clear();
    check(original_controller_attack(malformed,nullptr,0,{},error).admission==OriginalCommandAdmission::failed,"missing attack backend under block accepted");
    state={1,2,256,0,0,0};check(original_controller_object_command(state,controller_stop,0,control,error).admission==OriginalCommandAdmission::failed,"invalid byte flag accepted");
    check(!original_controller_animation_event({0x21,0,0,0,0},animation,error),"unsupported animation event accepted");
    std::cout<<"original_controller_commands PASS: forced/global/local order, remote/null order, Stop body/event, attack network/dispatch, locked animation ends\n";
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
