#include "character_ai_ranged_range.hpp"
#include <cstddef>
#include <cstring>

namespace dh2::character_ai_ranged_range { namespace {
struct Range {std::uintptr_t start,end;};
bool range(const void* p,std::size_t n,std::size_t alignment,Range& r) {
    const auto at=reinterpret_cast<std::uintptr_t>(p);
    if(!p || at%alignment || at>UINTPTR_MAX-n)return false;
    r={at,at+n};return true;
}
bool overlap(Range a,Range b){return a.start<b.end && b.start<a.end;}
template<class T> bool view(const T* p,const Range (&controls)[3]) {
    Range r;if(!range(p,sizeof(*p),alignof(T),r))return false;
    for(auto c:controls)if(overlap(r,c))return false;
    return true;
}
float number(std::uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
std::uint32_t bits(float f){std::uint32_t w;std::memcpy(&w,&f,4);return w;}
float sub(float a,float b){volatile float r=a-b;return r;}
float mul(float a,float b){volatile float r=a*b;return r;}
float add(float a,float b){volatile float r=a+b;return r;}
float signed_number(std::uint32_t w) {
    std::int32_t signed_word;std::memcpy(&signed_word,&w,4);
    volatile float f=static_cast<float>(signed_word);return f;
}
Status evaluate(State* state,std::uintptr_t candidate,const Services* services,
                Result* result,bool close) {
    Range controls[3];
    if(!range(state,sizeof(*state),alignof(State),controls[0]) ||
       !range(services,sizeof(*services),alignof(Services),controls[1]) ||
       !range(result,sizeof(*result),alignof(Result),controls[2]))return Status::invalid_argument;
    for(unsigned i=0;i<3;++i)for(unsigned j=0;j<i;++j)
        if(overlap(controls[i],controls[j]))return Status::invalid_argument;
    if(!state->ai || !state->owner)return Status::invalid_argument;
    const auto bound=*services;const auto original_ai=state->ai;*result={};
    if(!candidate)candidate=state->target_40;
    result->candidate=candidate;if(!candidate)return Status::complete;
    Response response;
    auto invoke=[&](Operation op,std::uintptr_t subject,std::uintptr_t other=0,
                    unsigned key=0,std::uint32_t* minimum=nullptr,
                    std::uint32_t* maximum=nullptr,std::uint32_t* projectile=nullptr) {
        if(!bound.invoke)return Status::service_unavailable;
        response={};++result->calls;
        const Request request{op,subject,other,key,minimum,maximum,projectile};
        try {if(bound.invoke(bound.context,state,&request,&response))return Status::service_failed;}
        catch(...){return Status::service_failed;}
        return Status::complete;
    };
    Status status;
    if(close) {
        status=invoke(Operation::resolve_object,candidate);
        if(status!=Status::complete)return status;
        const auto* resolved=static_cast<const ResolvedObject*>(response.view);
        bool eligible=false;
        if(resolved) {
            if(!view(resolved,controls) || !resolved->identity)return Status::invalid_source_fact;
            result->resolved=resolved->identity;
            if(!resolved->word_f4) {
                if(!state->owner)return Status::invalid_source_fact;
                status=invoke(Operation::interaction_type,result->resolved,state->owner);
                if(status!=Status::complete)return status;
                eligible=response.word==8;
            }
        }
        if(!eligible) {
            result->used_interaction_range=1;
            status=invoke(Operation::interaction_range,original_ai,candidate);
            if(status==Status::complete)result->value=response.word;
            return status;
        }
    }
    if(!state->owner)return Status::invalid_source_fact;
    // Source leaves these locals uninitialized; zero is port storage hygiene,
    // not a source fact. False capability skips every output read below.
    std::uint32_t minimum=0,maximum=0,projectile=0;
    status=invoke(Operation::can_range_attack,state->owner,0,0,&minimum,&maximum,close?&maximum:&projectile);
    if(status!=Status::complete)return status;
    if(!response.word)return Status::complete;
    if(!state->owner)return Status::invalid_source_fact;
    status=invoke(Operation::target_position,state->owner);
    if(status!=Status::complete)return status;
    const auto* owner_point=static_cast<const Point*>(response.view);
    status=invoke(Operation::target_position,candidate);
    if(status!=Status::complete)return status;
    const auto* target_point=static_cast<const Point*>(response.view);
    if(!view(owner_point,controls) || !view(target_point,controls))return Status::invalid_source_fact;
    const auto x=sub(number(owner_point->words[0]),number(target_point->words[0]));
    const auto y=sub(number(owner_point->words[1]),number(target_point->words[1]));
    const auto z=sub(number(owner_point->words[2]),number(target_point->words[2]));
    const auto xx=mul(x,x),yy=mul(y,y),xy=add(xx,yy),zz=mul(z,z);
    const auto distance=add(xy,zz);result->distance_word=bits(distance);
    status=invoke(Operation::diagnostic_switch,0,0,1);
    if(status!=Status::complete)return status;
    if(!response.identity)return Status::invalid_source_fact;
    result->diagnostic_identity=response.identity;
    if(response.word) {
        status=invoke(Operation::diagnostic_switch,result->diagnostic_identity,0,2);
        if(status!=Status::complete)return status;
    }
    // Original stack words are read only after both diagnostic sequences.
    result->minimum_word=minimum;result->maximum_word=maximum;
    result->projectile_word=close?maximum:projectile;
    result->minimum_square_word=minimum*minimum;
    const auto minimum_limit=signed_number(result->minimum_square_word);
    result->minimum_limit_word=bits(minimum_limit);
    if(close)result->value=minimum_limit>distance;
    else if(minimum_limit<=distance) {
        result->maximum_square_word=maximum*maximum;
        const auto maximum_limit=signed_number(result->maximum_square_word);
        result->maximum_limit_word=bits(maximum_limit);
        result->value=maximum_limit>=distance;
    }
    return Status::complete;
}
}
Status evaluate_close(State* state,std::uintptr_t candidate,const Services* services,Result* result) {
    return evaluate(state,candidate,services,result,true);
}
Status evaluate_ranged(State* state,std::uintptr_t candidate,const Services* services,Result* result) {
    return evaluate(state,candidate,services,result,false);
}
} // namespace dh2::character_ai_ranged_range
