#include "original_actor_motion_frame.hpp"
#include <cstring>
#include <exception>
namespace dh::foundation {
namespace {
bool overlap(const void* a,const void* b){const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<12:x-y<12;}
}
bool update_original_actor_motion_frame(const OriginalActorMotionFrameBorrow& borrow,
    const OriginalActorMotionFrameServices& services,OriginalActorMotionFrameResult& output,std::string& error) {
    if(!borrow.actor_lease||!borrow.actor||!borrow.identity||borrow.actor->id!=borrow.identity||!borrow.previous_position190||!borrow.previous_rotation19c||!services.runtime_lease){error="Motion-frame prefix requires SAME admitted actor/runtime and explicit previous-field borrows";return false;}
    const auto* position=borrow.actor->transform.position.data();const auto* rotation=borrow.actor->transform.rotation.data();
    const void* fields[]{position,rotation,borrow.previous_position190->data(),borrow.previous_rotation19c->data()};
    for(unsigned i=0;i<4;++i)for(unsigned j=i+1;j<4;++j)if(overlap(fields[i],fields[j])){error="Source current/previous motion-frame cells must be distinct";return false;}
    error.clear();output={borrow.identity,OriginalActorMotionFramePhase::snapshot,0};
    // Original copies words, preserving all IEEE payloads/sign bits. No
    // animation/floor normalization or snapshot from a separate motor is used.
    std::uint32_t p[3],r[3];std::memcpy(p,position,12);std::memcpy(r,rotation,12);
    const auto store=[](float* field,std::uint32_t word){std::memcpy(field,&word,4);};
    store(borrow.previous_rotation19c->data()+2,r[2]);
    store(borrow.previous_position190->data(),p[0]);
    store(borrow.previous_position190->data()+1,p[1]);
    store(borrow.previous_rotation19c->data(),r[0]);
    store(borrow.previous_rotation19c->data()+1,r[1]);
    store(borrow.previous_position190->data()+2,p[2]);output.completed_mask=1;
    struct Phase {OriginalActorMotionFramePhase phase;std::uint32_t bit;const char* name;const OriginalActorMotionFrameServices::Callback* callback;};
    const Phase phases[]{
        {OriginalActorMotionFramePhase::path,2,"UpdatePath",&services.update_path},
        {OriginalActorMotionFramePhase::rotation,4,"UpdateRotation",&services.update_rotation},
        {OriginalActorMotionFramePhase::subobjects,8,"UpdateSubObjects",&services.update_subobjects},
        {OriginalActorMotionFramePhase::target_position,16,"UpdateTargetPosition",&services.update_target_position}
    };
    for(const auto& phase:phases){
        output.phase=phase.phase;
        if(!*phase.callback){error=std::string("Reached original actor ")+phase.name+" callback is unbound";return false;}
        try {
            if(!(*phase.callback)(borrow,error)){if(error.empty())error=std::string("Original actor ")+phase.name+" callback failed";return false;}
        }catch(const std::exception& exception){error=std::string("Original actor ")+phase.name+" callback failed: "+exception.what();return false;}
        output.completed_mask|=phase.bit;
    }
    output.phase=OriginalActorMotionFramePhase::complete_prefix;error.clear();return true;
}
}
