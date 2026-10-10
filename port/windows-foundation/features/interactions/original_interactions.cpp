#include "original_interactions.hpp"
#include <cmath>
#include <utility>
namespace dh::foundation::interactions {
namespace {
template<class F,class... A> bool call(const F& f,const char* name,std::string& e,A&&... a){
    if(!f){e=std::string("Interactions require ")+name;return false;}
    return f(std::forward<A>(a)...,e);
}
}
Outcome character_use(CharacterUseBorrow& b,std::uintptr_t explicit_target,std::string& e){
    e.clear();if(!b.owner){e="Interactions require live Character owner";return Outcome::failed;}
    bool remote=false;if(!call(b.remotely_updated,"Character remote virtual",e,remote))return Outcome::failed;
    if(remote)return Outcome::blocked;
    auto target=explicit_target;
    if(!target){if(!b.object_of_interest){e="Interactions require Character OOI5284";return Outcome::failed;}target=*b.object_of_interest;}
    if(!target)return Outcome::blocked;
    if(!b.ai_current_target){e="Interactions require CharAI current target1032";return Outcome::failed;}
    if(*b.ai_current_target)return Outcome::blocked;
    bool idle=false;if(!call(b.idle,"SM_IsIdle(0)",e,idle))return Outcome::failed;
    if(!idle){bool moving=false;if(!call(b.moving,"SM_IsMoving(0)",e,moving))return Outcome::failed;if(!moving)return Outcome::blocked;}
    if(!b.use_requested){e="Interactions require CharAI use byte1042";return Outcome::failed;}
    if(!explicit_target)target=*b.object_of_interest;
    if(!call(b.set_ai_target,"AI_SetTarget(target,false)",e,target,false))return Outcome::failed;
    *b.use_requested=1;return Outcome::completed;
}
Outcome controller_use(const ControllerUseBorrow& b,std::uintptr_t target,std::string& e){
    e.clear();if(!b.forced||!b.locked||!b.globally_blocked){e="Interactions require live controller flags";return Outcome::failed;}
    if(!*b.forced&&(*b.globally_blocked||*b.locked))return Outcome::blocked;
    bool online=false;if(!call(b.online,"GetOnline flag5",e,online))return Outcome::failed;
    if(online&&!call(b.online_prefix,"Cmd_UseOOI network prefix4057fc",e,target))return Outcome::failed;
    if(!b.controllable){e="Interactions require controllable Ctrl_UseOOI virtual80";return Outcome::failed;}
    return b.controllable(target,e);
}
bool in_interaction_range(const RangeBorrow& b,std::uintptr_t target,bool& out,std::string& e){
    e.clear();out=false;if(!target)target=b.current_target;if(!target)return true;
    std::array<float,3> p{},q{};
    if(!call(b.target_position,"owner GetTargetPosition",e,b.owner,p)||!call(b.interaction_spot,"target GetInteractionSpot",e,target,q))return false;
    const float dx=p[0]-q[0],dy=p[1]-q[1],dz=p[2]-q[2];
    const float distance=std::sqrt((dx*dx+dy*dy)+dz*dz);
    bool node=false;if(!call(b.has_interaction_node,"target node2e8",e,target,node))return false;
    float melee=0,radius=0,padding=80;
    if(!node&&(!call(b.melee_radius,"AI_GetMeleeRadius",e,melee)||!call(b.target_radius,"target virtual148",e,target,radius)||!call(b.interaction_padding,"Character AI padding24",e,padding)))return false;
    const float separation=(distance-melee)-radius;
    int type=0;if(!call(b.interaction_type,"target virtual144",e,target,b.owner,type))return false;
    // ARM 3d5094/3d50b8 use __aeabi_fcmple: unordered is rejected.
    // IDA renders the opposite > rejection and obscures this NaN difference.
    out=separation<=(type==8?0.0f:padding);return true;
}
bool Router::bind(std::uint32_t type,Handler handler,std::string& e){
    e.clear();if(!handler){e="Interactions require complete source type handler";return false;}
    if(!handlers_.emplace(type,std::move(handler)).second){e="Interactions source type already bound";return false;}return true;
}
Outcome Router::dispatch(const dh2::world::CanonicalObjectBorrowV1& object,std::uintptr_t actor,std::string& e)const{
    e.clear();if(!object.identity||!object.lease||!object.type_f4||!actor){e="Interactions require published canonical receiver and actor";return Outcome::failed;}
    auto i=handlers_.find(*object.type_f4);if(i==handlers_.end()){e="Unsupported original interaction source type "+std::to_string(*object.type_f4);return Outcome::unsupported;}
    return i->second(object,actor,e)?Outcome::completed:Outcome::failed;
}
Outcome openable(dh2::world::OpenableContainerOwnerV1& owner,dh2::world::OpenableContainerFieldsV1& fields,
 const dh2::world::OpenableContainerInteractionServicesV2& services,bool disabled,std::uintptr_t actor,std::string& e){
    e.clear();if(!owner.is_interactive(disabled))return Outcome::blocked;
    return dh2::world::openable_container_interact_v2(owner,fields,services,actor,e)?Outcome::completed:Outcome::failed;
}
}

