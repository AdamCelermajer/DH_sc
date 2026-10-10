#include "../original_actor_animation_events.hpp"
#include <iostream>
#include <stdexcept>
#include <cstring>
using namespace dh::foundation;
using namespace dh2::character;
static void check(bool value,const char* error){if(!value)throw std::runtime_error(error);}
struct Backend{unsigned helper=0,forward=0;
    static std::int32_t invoke(void* p,AIEventState64*,const AIEventRequest40* request,std::uint32_t* output){
        auto& b=*static_cast<Backend*>(p);
        if(request->service==ai_event_helper){check(request->operation==0x3d4434&&request->event==0x28,
            "Authored marker mapped to numeric begin/end");
            check(std::strcmp(reinterpret_cast<const char*>(request->payload),"attack_mainhand")==0,"Authored payload differs");++b.helper;*output=1;}
        else if(request->service==ai_event_state_event){check(b.helper==b.forward+1,"State forwarded before named consumer");++b.forward;}
        else throw std::runtime_error("Unexpected named dispatcher service");
        return 0;
    }
};
int main(){try{std::string error;
    for(unsigned flags=0;flags<8;++flags){
        AIEventOwner48 owner{1,2,3,4,(flags>>2)&1,(flags>>1)&1,0,0};
        AIEventState64 state{5,&owner,nullptr,0,nullptr,0,0,flags&1,0,0,0};Backend backend;
        const AIEventServices24 services{&backend,Backend::invoke,(1u<<ai_event_helper)|(1u<<ai_event_state_event),0};
        RetainedAnimationEvent event;event.name="attack_mainhand";event.lag_ms=-17;
        check(route_original_named_animation_event(state,event,services,error),error.c_str());
        check(backend.helper==1&&backend.forward==1&&event.lag_ms==-17,"Command gates suppressed named damage event");
    }
    check(classify_original_named_animation_event("attack_mainhand",5,false)==OriginalNamedAnimationKind::mainhand,"Melee state5 mapping differs");
    check(classify_original_named_animation_event("attack_offhand",5,false)==OriginalNamedAnimationKind::offhand,"Offhand mapping differs");
    check(classify_original_named_animation_event("attack_mainhand",3,false)==OriginalNamedAnimationKind::ignored,"Idle string invented damage");
    check(classify_original_named_animation_event("attack_mainhand",5,true)==OriginalNamedAnimationKind::projectile,"Ranged mainhand invented melee");
    check(classify_original_named_animation_event("do_skill",6,false)==OriginalNamedAnimationKind::skill,"Skill state mapping differs");
    check(classify_original_named_animation_event("do_spell",7,false)==OriginalNamedAnimationKind::spell,"Spell state mapping differs");
    check(classify_original_named_animation_event("interact",13,false)==OriginalNamedAnimationKind::interaction,"Interaction state mapping differs");
    check(classify_original_named_animation_event("fx_anything",0,false)==OriginalNamedAnimationKind::mesh_fx,"Prefix must precede state gate");
    std::cout<<"original_actor_animation_events PASS: source28 all8 gate combinations and proven named-state classifications\n";
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
