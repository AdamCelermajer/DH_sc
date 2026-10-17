#include "character_ai_skill_script_update.hpp"
#include <limits>
namespace dh2::character_ai_skill_script_update {namespace {
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
        case Operation::call_on_skill_update:return Phase::update;
        case Operation::call_on_skill_cleanup:return Phase::cleanup;
        case Operation::destroy_values:return Phase::destroy;
    }
    return Phase::not_started;
}
Status invoke(const Services& bound,State* state,ReturnValues& values,Result& out,
              Operation op,std::uintptr_t skill,std::uintptr_t script=0,
              std::uintptr_t args=0,std::uintptr_t first=0,std::uintptr_t last=0){
    out.phase=phase(op);if(!bound.invoke)return Status::service_unavailable;
    const char* fn=op==Operation::call_set_skill?"SetSkill":
        op==Operation::call_on_skill_update?"OnSkillUpdate":
        op==Operation::call_on_skill_cleanup?"OnSkillCleanUp":nullptr;
    const Request request{op,skill,script,args,out.resource,first,last,fn};
    ++out.service_calls;
    try{if(bound.invoke(bound.context,state,&request,&values))return Status::service_failed;}
    catch(...){return Status::service_failed;}
    return Status::complete;
}
}
Status execute(State* state,const Services* services,Result* out,bool cleanup_call){
    Range controls[3];
    if(!range(state,controls[0])||!range(services,controls[1])||!range(out,controls[2]))return Status::invalid_argument;
    for(unsigned i=0;i<3;++i)for(unsigned j=0;j<i;++j)if(overlap(controls[i],controls[j]))return Status::invalid_argument;
    const auto skill=state->identity;
    if(!skill||skill>std::numeric_limits<std::uintptr_t>::max()-0xcu)return Status::invalid_argument;
    const auto bound=*services;*out={};ReturnValues values{};
    auto status=invoke(bound,state,values,*out,Operation::construct_values,skill);
    if(status!=Status::complete)return status;
    out->resource=values.resource;
    if(!out->resource)return Status::invalid_source_fact;
    out->constructed=1;
    if(!live(state->owner,controls)||!state->owner->identity)return Status::invalid_source_fact;
    const auto script=state->owner->lua_script;
    if(!script)out->decision=Decision::no_script;
    else{
        status=invoke(bound,state,values,*out,Operation::call_set_skill,skill,script,skill+0xcu);
        if(status!=Status::complete)return status;
        if(values.resource!=out->resource)return Status::invalid_source_fact;
        out->set_skill_error=values.error;
        if(values.error)out->decision=Decision::set_skill_error;
        else{
            if(!live(values.values,controls))return Status::invalid_source_fact;
            Range owner_range,vector_range,temporary_range;
            if(!range(values.values,vector_range)||!range(&values,temporary_range)||overlap(vector_range,temporary_range))return Status::invalid_source_fact;
            // Owner may have changed during SetSkill. It is not read yet, but
            // live vector metadata must never alias a retained Character.
            if(state->owner&&range(state->owner,owner_range)&&range(values.values,vector_range)&&overlap(owner_range,vector_range))return Status::invalid_source_fact;
            const auto first=values.values->begin,last=values.values->end;
            if(first!=last){
                if(!first||!last||last<first)return Status::invalid_source_fact;
                status=invoke(bound,state,values,*out,Operation::erase_values,skill,0,0,first,last);
                if(status!=Status::complete)return status;
                if(values.resource!=out->resource)return Status::invalid_source_fact;
                out->erased=1;
            }
            if(!live(state->owner,controls)||!state->owner->identity||!state->owner->lua_script)return Status::invalid_source_fact;
            status=invoke(bound,state,values,*out,
                cleanup_call?Operation::call_on_skill_cleanup:Operation::call_on_skill_update,
                skill,state->owner->lua_script);
            if(status!=Status::complete)return status;
            if(values.resource!=out->resource)return Status::invalid_source_fact;
            if(cleanup_call){out->cleaned=1;out->decision=Decision::cleaned;}
            else out->updated=1;
        }
    }
    status=invoke(bound,state,values,*out,Operation::destroy_values,skill);
    if(status!=Status::complete)return status;
    out->destroyed=1;out->phase=Phase::complete;
    return Status::complete;
}
Status update(State* state,const Services* services,Result* out){
    return execute(state,services,out,false);
}
Status cleanup(State* state,const Services* services,Result* out){
    return execute(state,services,out,true);
}
} // namespace dh2::character_ai_skill_script_update
