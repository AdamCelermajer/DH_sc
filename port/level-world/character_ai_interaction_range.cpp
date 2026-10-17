#include "character_ai_interaction_range.hpp"
#include <cmath>
#include <cstddef>
#include <cstring>

namespace dh2::character_ai_interaction_range { namespace {
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
bool preflight(State* state,const Services* services,Result* result,Range (&r)[3]) {
    if(!range(state,sizeof(*state),alignof(State),r[0]) ||
       !range(services,sizeof(*services),alignof(Services),r[1]) ||
       !range(result,sizeof(*result),alignof(Result),r[2]))return false;
    for(unsigned i=0;i<3;++i)for(unsigned j=0;j<i;++j)if(overlap(r[i],r[j]))return false;
    return state->ai && state->owner;
}
float number(std::uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
std::uint32_t bits(float f){std::uint32_t w;std::memcpy(&w,&f,4);return w;}
float sub(float a,float b){volatile float value=a-b;return value;}
float mul(float a,float b){volatile float value=a*b;return value;}
float add(float a,float b){volatile float value=a+b;return value;}
Status call(const Services& s,State* state,Result* result,Operation operation,
            std::uintptr_t subject,std::uintptr_t other,Response& response) {
    if(!s.invoke)return Status::service_unavailable;
    response={};++result->calls;const Request request{operation,subject,other};
    try {if(s.invoke(s.context,state,&request,&response))return Status::service_failed;}
    catch(...){return Status::service_failed;}
    return Status::complete;
}
}
Status evaluate_object(State* state,std::uintptr_t candidate,const Services* services,Result* result) {
    Range controls[3];if(!preflight(state,services,result,controls))return Status::invalid_argument;
    const auto bound=*services;const auto original_ai=state->ai;*result={};
    if(!candidate)candidate=state->target_40;
    result->candidate=candidate;if(!candidate)return Status::complete;
    Response response;
    auto status=call(bound,state,result,Operation::target_position,state->owner,0,response);
    if(status!=Status::complete)return status;
    const auto* owner_point=static_cast<const Point*>(response.view);
    status=call(bound,state,result,Operation::interaction_spot,candidate,0,response);
    if(status!=Status::complete)return status;
    const auto* target=static_cast<const ObjectFacts*>(response.view);
    const Point spot=response.point;
    if(!view(owner_point,controls) || !view(target,controls) || target->identity!=candidate)
        return Status::invalid_source_fact;
    // Source GetInteractionSpot returns by value. Its callback may change the
    // borrowed owner point; that owner backing is read only after the callback.
    const auto x=sub(number(owner_point->words[0]),number(spot.words[0]));
    const auto y=sub(number(owner_point->words[1]),number(spot.words[1]));
    const auto z=sub(number(owner_point->words[2]),number(spot.words[2]));
    const auto xx=mul(x,x),yy=mul(y,y),xy=add(xx,yy),zz=mul(z,z);
    const auto squared=add(xy,zz);result->distance_squared_word=bits(squared);
    volatile float rooted=std::sqrt(squared);const float distance=rooted;
    result->distance_word=bits(distance);
    result->node_present=target->interaction_node_2e8!=0;
    float owner_radius=0.f,target_radius=0.f,threshold=80.f;
    if(!result->node_present) {
        status=call(bound,state,result,Operation::melee_radius,original_ai,0,response);
        if(status!=Status::complete)return status;
        result->owner_radius_word=response.word;owner_radius=number(response.word);
        status=call(bound,state,result,Operation::interaction_radius,candidate,0,response);
        if(status!=Status::complete)return status;
        result->target_radius_word=response.word;target_radius=number(response.word);
        if(!state->owner)return Status::invalid_source_fact;
        status=call(bound,state,result,Operation::char_ai,state->owner,0,response);
        if(status!=Status::complete)return status;
        const auto* row=static_cast<const AiRow*>(response.view);
        if(!view(row,controls))return Status::invalid_source_fact;
        threshold=number(row->words[6]); // original returned AI row+0x18
    }
    result->threshold_word=bits(threshold);
    const auto remaining=sub(sub(distance,owner_radius),target_radius);
    result->remaining_word=bits(remaining);
    if(!state->owner)return Status::invalid_source_fact;
    status=call(bound,state,result,Operation::interaction_type,candidate,state->owner,response);
    if(status!=Status::complete)return status;
    result->interaction_type=response.word;
    result->value=remaining<=(response.word==8?0.f:threshold);
    return Status::complete;
}
} // namespace dh2::character_ai_interaction_range
