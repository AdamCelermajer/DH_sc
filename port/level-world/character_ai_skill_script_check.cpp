#include "character_ai_skill_script_check.hpp"
#include <limits>
namespace dh2::character_ai_skill_script_check {namespace {
struct Range {std::uintptr_t a,b;};
template<class T>bool range(const T* p,Range& r){
    const auto a=reinterpret_cast<std::uintptr_t>(p);
    if(!p||a%alignof(T)||a>std::numeric_limits<std::uintptr_t>::max()-sizeof(T))return false;
    r={a,a+sizeof(T)};return true;
}
bool overlap(Range a,Range b){return a.a<b.b&&b.a<a.b;}
template<class T>bool live(const T* p,const Range* controls){
    Range r;if(!range(p,r))return false;
    for(unsigned i=0;i<3;++i)if(overlap(r,controls[i]))return false;
    return true;
}
Phase phase(Operation op){
    switch(op){
        case Operation::construct_values:return Phase::construct;
        case Operation::call_set_skill:return Phase::set_skill;
        case Operation::erase_values:return Phase::erase;
        case Operation::call_skill_check:return Phase::check;
        case Operation::operator_index:return Phase::index;
        case Operation::get_bool:return Phase::boolean;
        case Operation::destroy_values:return Phase::destroy;
    }
    return Phase::not_started;
}
Status invoke(const Services& bound,State* state,ReturnValues& values,Result& out,
              Check kind,Operation op,std::uintptr_t skill,Response& reply,
              std::uintptr_t script=0,std::uintptr_t args=0,std::uintptr_t first=0,
              std::uintptr_t last=0,std::uintptr_t value=0){
    out.phase=phase(op);if(!bound.invoke)return Status::service_unavailable;
    const char* fn=op==Operation::call_set_skill?"SetSkill":op==Operation::call_skill_check?"OnSkillCheck":nullptr;
    const Request request{op,kind,skill,script,args,out.resource,first,last,value,0,fn};
    reply={};++out.service_calls;
    try{if(bound.invoke(bound.context,state,&request,&values,&reply))return Status::service_failed;}
    catch(...){return Status::service_failed;}
    if(op!=Operation::construct_values&&op!=Operation::destroy_values&&values.resource!=out.resource)return Status::invalid_source_fact;
    return Status::complete;
}
Status vector(const ReturnValues& values,const Range* controls,State* state,
              std::uintptr_t& first,std::uintptr_t& last){
    if(!live(values.values,controls))return Status::invalid_source_fact;
    Range metadata,temporary,owner;
    if(!range(values.values,metadata)||!range(&values,temporary)||overlap(metadata,temporary))return Status::invalid_source_fact;
    if(state->owner&&range(state->owner,owner)&&overlap(metadata,owner))return Status::invalid_source_fact;
    first=values.values->begin;last=values.values->end;
    if(first!=last&&(!first||!last||last<first))return Status::invalid_source_fact;
    if(first!=last){
        const Range data{first,last};
        for(unsigned i=0;i<3;++i)if(overlap(data,controls[i]))return Status::invalid_source_fact;
        if(overlap(data,metadata)||overlap(data,temporary)||(state->owner&&range(state->owner,owner)&&overlap(data,owner)))return Status::invalid_source_fact;
    }
    return Status::complete;
}
}
Status check(State* state,Check kind,const Services* services,Result* out){
    Range controls[3];
    if((kind!=Check::usable&&kind!=Check::active)||!range(state,controls[0])||!range(services,controls[1])||!range(out,controls[2]))return Status::invalid_argument;
    for(unsigned i=0;i<3;++i)for(unsigned j=0;j<i;++j)if(overlap(controls[i],controls[j]))return Status::invalid_argument;
    const auto skill=state->identity;const auto bound=*services;
    if(!skill||skill>std::numeric_limits<std::uintptr_t>::max()-0xcu||!bound.native_value_stride||bound.native_value_stride>4096)return Status::invalid_argument;
    *out={};ReturnValues values{};Response reply{};
    auto status=invoke(bound,state,values,*out,kind,Operation::construct_values,skill,reply);
    if(status!=Status::complete)return status;
    out->resource=values.resource;if(!out->resource)return Status::invalid_source_fact;out->constructed=1;
    if(!live(state->owner,controls)||!state->owner->identity)return Status::invalid_source_fact;
    const auto script=state->owner->lua_script;
    if(!script)out->decision=Decision::no_script;
    else{
        status=invoke(bound,state,values,*out,kind,Operation::call_set_skill,skill,reply,script,skill+0xcu);
        if(status!=Status::complete)return status;
        out->set_skill_error=values.error;
        if(values.error)out->decision=Decision::set_skill_error;
        else{
            std::uintptr_t first=0,last=0;
            status=vector(values,controls,state,first,last);if(status!=Status::complete)return status;
            if(first!=last){
                status=invoke(bound,state,values,*out,kind,Operation::erase_values,skill,reply,0,0,first,last);
                if(status!=Status::complete)return status;
                out->erased=1;
            }
            if(!live(state->owner,controls)||!state->owner->identity||!state->owner->lua_script)return Status::invalid_source_fact;
            status=invoke(bound,state,values,*out,kind,Operation::call_skill_check,skill,reply,state->owner->lua_script);
            if(status!=Status::complete)return status;
            out->check_error=values.error;
            if(values.error)out->decision=Decision::check_error;
            else{
                status=vector(values,controls,state,first,last);if(status!=Status::complete)return status;
                const auto bytes=last-first;
                if(bytes%bound.native_value_stride||bytes/bound.native_value_stride>1000000)return Status::invalid_source_fact;
                out->value_count=std::uint32_t(bytes/bound.native_value_stride);
                const auto index=kind==Check::usable?0u:1u;
                if(out->value_count<=index)out->decision=Decision::insufficient_values;
                else{
                    if(kind==Check::usable){
                        status=invoke(bound,state,values,*out,kind,Operation::operator_index,skill,reply);
                        if(status!=Status::complete)return status;
                        if(!reply.value)return Status::invalid_source_fact;
                        out->value=reply.value;
                    }else out->value=first+bound.native_value_stride;
                    status=invoke(bound,state,values,*out,kind,Operation::get_bool,skill,reply,0,0,0,0,out->value);
                    if(status!=Status::complete)return status;
                    if(reply.boolean>1)return Status::invalid_source_fact;
                    out->boolean=reply.boolean;out->decision=Decision::converted;
                }
            }
        }
    }
    status=invoke(bound,state,values,*out,kind,Operation::destroy_values,skill,reply);
    if(status!=Status::complete)return status;
    out->destroyed=1;out->phase=Phase::complete;return Status::complete;
}
} // namespace dh2::character_ai_skill_script_check
