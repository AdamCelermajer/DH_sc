#include "character_ai_melee_range.hpp"
#include <cstddef>
#include <cstring>
namespace dh2::character_ai_melee_range { namespace {
struct Range {std::uintptr_t start,end;};
bool range(const void* p,std::size_t n,std::size_t a,Range& r) {
    const auto at=reinterpret_cast<std::uintptr_t>(p);
    if(!p || at%a || at>UINTPTR_MAX-n)return false;
    r={at,at+n};return true;
}
bool overlap(Range a,Range b){return a.start<b.end && b.start<a.end;}
template<class T> bool view(const T* p,const Range (&controls)[3]) {
    Range r;if(!range(p,sizeof(*p),alignof(T),r))return false;
    for(auto c:controls)if(overlap(r,c))return false;
    return true;
}
template<class S,class R> bool preflight(State* state,const S* services,R* result,Range (&ranges)[3]) {
    if(!range(state,sizeof(*state),alignof(State),ranges[0]) ||
       !range(services,sizeof(*services),alignof(S),ranges[1]) ||
       !range(result,sizeof(*result),alignof(R),ranges[2]))return false;
    for(unsigned i=0;i<3;++i)for(unsigned j=0;j<i;++j)if(overlap(ranges[i],ranges[j]))return false;
    return state->ai && state->owner;
}
float number(std::uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
std::uint32_t bits(float f){std::uint32_t w;std::memcpy(&w,&f,4);return w;}
float sub(float a,float b){volatile float r=a-b;return r;}
float mul(float a,float b){volatile float r=a*b;return r;}
float add(float a,float b){volatile float r=a+b;return r;}
Status call(const Services& services,State* state,Result* result,Operation operation,
            Subject kind,std::uintptr_t subject,std::uintptr_t other,Response& response,unsigned key=0) {
    if(!services.invoke)return Status::service_unavailable;
    response={};++result->calls;const Request q{operation,kind,subject,other,key};
    try {if(services.invoke(services.context,state,&q,&response))return Status::service_failed;}
    catch(...){return Status::service_failed;}
    return Status::complete;
}
}
Status evaluate_object(State* state,std::uintptr_t candidate,const Services* services,Result* result) {
    Range controls[3];if(!preflight(state,services,result,controls))return Status::invalid_argument;
    const auto bound=*services;const auto original_ai=state->ai;*result={};
    if(!candidate)candidate=state->target_40;
    result->candidate=candidate;if(!candidate)return Status::complete;
    Response r;
    auto status=call(bound,state,result,Operation::resolve_object,Subject::object,candidate,0,r);
    if(status!=Status::complete)return status;
    const auto* resolved=static_cast<const ResolvedObject*>(r.view);
    bool eligible=false;
    if(resolved) {
        if(!view(resolved,controls) || !resolved->identity)return Status::invalid_source_fact;
        result->resolved=resolved->identity;
        if(!resolved->word_f4) {
            if(!state->owner)return Status::invalid_source_fact;
            status=call(bound,state,result,Operation::interaction_type,Subject::object,resolved->identity,state->owner,r);
            if(status!=Status::complete)return status;
            eligible=r.word==8;
        }
    }
    if(!eligible) {
        result->used_interaction_range=1;
        status=call(bound,state,result,Operation::interaction_range,Subject::original_ai,original_ai,candidate,r);
        if(status==Status::complete)result->value=r.word;
        return status;
    }
    if(!state->owner)return Status::invalid_source_fact;
    const auto owner=state->owner;
    status=call(bound,state,result,Operation::target_position,Subject::object,owner,0,r);
    if(status!=Status::complete)return status;
    const auto* owner_point=static_cast<const Point*>(r.view);
    status=call(bound,state,result,Operation::target_position,Subject::object,candidate,0,r);
    if(status!=Status::complete)return status;
    const auto* target_point=static_cast<const Point*>(r.view);
    if(!view(owner_point,controls) || !view(target_point,controls))return Status::invalid_source_fact;
    // Both live point values are read AFTER the second source callback.
    const auto x=sub(number(owner_point->words[0]),number(target_point->words[0]));
    const auto y=sub(number(owner_point->words[1]),number(target_point->words[1]));
    const auto z=sub(number(owner_point->words[2]),number(target_point->words[2]));
    const auto xx=mul(x,x),yy=mul(y,y),xy=add(xx,yy),zz=mul(z,z);
    const auto distance=add(xy,zz);result->distance_word=bits(distance);
    status=call(bound,state,result,Operation::melee_radius,Subject::original_ai,original_ai,0,r);
    if(status!=Status::complete)return status;
    result->owner_radius_word=r.word;
    status=call(bound,state,result,Operation::melee_radius,Subject::resolved_character_ai,result->resolved,0,r);
    if(status!=Status::complete)return status;
    result->target_radius_word=r.word;
    const auto sum=add(number(result->owner_radius_word),number(r.word));result->sum_word=bits(sum);
    status=call(bound,state,result,Operation::diagnostic_switch,Subject::global,0,0,r,1);
    if(status!=Status::complete)return status;
    if(r.word) {
        status=call(bound,state,result,Operation::diagnostic_switch,Subject::global,0,0,r,2);
        if(status!=Status::complete)return status;
    }
    const auto square=mul(sum,sum);result->square_word=bits(square);
    result->value=square>distance;
    return Status::complete;
}
Status get_radius(State* state,const RadiusServices* services,RadiusResult* result) {
    Range controls[3];if(!preflight(state,services,result,controls))return Status::invalid_argument;
    const auto bound=*services;*result={};
    auto invoke=[&](RadiusOperation operation,std::uintptr_t owner,RadiusResponse& response) {
        if(!bound.invoke)return Status::service_unavailable;
        response={};++result->calls;const RadiusRequest q{operation,owner};
        try {if(bound.invoke(bound.context,state,&q,&response))return Status::service_failed;}
        catch(...){return Status::service_failed;}
        return Status::complete;
    };
    RadiusResponse response;
    auto status=invoke(RadiusOperation::inventory_can_melee_attack,state->owner,response);
    if(status!=Status::complete)return status;
    result->equipment_word=response.word;
    if(!state->owner)return Status::invalid_source_fact;
    const auto owner=state->owner; // source second owner precedes global table capture
    status=invoke(RadiusOperation::ai_table,0,response);
    if(status!=Status::complete)return status;
    const auto* table=response.table;
    if(!view(table,controls) || !table->count || table->count>65536)return Status::invalid_source_fact;
    const auto* rows=table->rows;const auto count=table->count;Range backing,metadata;
    if(!range(rows,count*sizeof(AiRow),alignof(AiRow),backing))return Status::invalid_source_fact;
    for(auto c:controls)if(overlap(backing,c))return Status::invalid_source_fact;
    range(table,sizeof(*table),alignof(AiTable),metadata);
    if(overlap(backing,metadata))return Status::invalid_source_fact;
    status=invoke(RadiusOperation::char_ai_id,owner,response);
    if(status!=Status::complete)return status;
    if(response.word>=count)return Status::invalid_source_fact;
    result->ai_id=response.word;
    std::int32_t equipment;std::memcpy(&equipment,&result->equipment_word,4);
    volatile float converted=static_cast<float>(equipment);
    result->row_word=rows[result->ai_id].words[8]; // original captured row+20 read late
    result->value_word=bits(add(converted,number(result->row_word)));
    return Status::complete;
}
} // namespace dh2::character_ai_melee_range
