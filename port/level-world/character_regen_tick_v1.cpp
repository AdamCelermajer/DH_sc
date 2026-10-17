#include "character_regen_tick_v1.hpp"

namespace dh2::character_regen_tick_v1 { namespace {
struct Range{std::uintptr_t b,e;};
template<class T>bool range(const T* p,Range& r){const auto a=reinterpret_cast<std::uintptr_t>(p);if(!p||a%alignof(T)||a>UINTPTR_MAX-sizeof(T))return false;r={a,a+sizeof(T)};return true;}
bool overlap(Range a,Range b){return a.b<b.e&&b.b<a.e;}
}
Status tick(const State* state,const Globals* globals,std::uint32_t combat,const Services* services,Result* out){
    Range controls[4];
    if(!range(state,controls[0])||!range(globals,controls[1])||!range(services,controls[2])||!range(out,controls[3]))return Status::invalid_argument;
    for(unsigned i=0;i<4;++i)for(unsigned j=0;j<i;++j)if(overlap(controls[i],controls[j]))return Status::invalid_argument;
    const auto actor=*state;const auto debug=globals->debug_switches;const auto bound=*services;
    if(!actor.character||!actor.properties||!actor.resolved_sheet||!debug)return Status::invalid_argument;
    *out={};out->in_combat=combat;out->hp_property=combat?40:39;out->mp_property=combat?45:44;
    auto call=[&](const Request& q,Reply& r){
        if(!bound.invoke)return Status::service_unavailable;
        ++out->calls;out->last_operation=std::uint32_t(q.operation);r={};
        try{if(bound.invoke(bound.context,&q,&r))return Status::service_failed;}catch(...){return Status::service_failed;}
        return Status::complete;
    };
    Reply reply{};auto status=call({Operation::debug_load,debug,0,0,0,nullptr},reply);
    if(status!=Status::complete)return status;
    status=call({Operation::string_construct,0,0,0,0,"isTracingChar_Stats"},reply);
    if(status!=Status::complete)return status;
    const auto string=reply.identity;if(!string)return Status::service_failed;
    status=call({Operation::debug_query,debug,string,0,0,nullptr},reply);if(status!=Status::complete)return status;
    status=call({Operation::string_destroy,string,0,0,0,nullptr},reply);if(status!=Status::complete)return status;
    status=call({Operation::read_property,actor.properties,actor.resolved_sheet,out->hp_property,0,nullptr},reply);
    if(status!=Status::complete)return status;
    out->hp_amount=reply.word;
    status=call({Operation::regen_hp,actor.character,0,0,out->hp_amount,nullptr},reply);if(status!=Status::complete)return status;
    out->hp_completed=1;
    status=call({Operation::read_property,actor.properties,actor.resolved_sheet,out->mp_property,0,nullptr},reply);
    if(status!=Status::complete)return status;
    out->mp_amount=reply.word;
    status=call({Operation::regen_mp,actor.character,0,0,out->mp_amount,nullptr},reply);
    if(status==Status::complete)out->mp_completed=1;
    return status;
}
}
