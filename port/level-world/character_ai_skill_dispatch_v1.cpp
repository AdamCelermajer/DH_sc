#include "character_ai_skill_dispatch_v1.hpp"
#include <cstddef>

namespace dh2::character_ai_skill_dispatch_v1 {
namespace {
bool span(const void* p,std::size_t n,std::size_t a){auto x=reinterpret_cast<std::uintptr_t>(p);return p && x%a==0 && x<=UINTPTR_MAX-n;}
bool overlap(const void* p,std::size_t n,const void* q,std::size_t m){auto a=reinterpret_cast<std::uintptr_t>(p),b=reinterpret_cast<std::uintptr_t>(q);return a<=b?b-a<n:a-b<m;}
struct Kernel {
    State& state;const Services* controls;Services services;Result& out;
    bool field(const void* p,std::size_t n,std::size_t a)const {
        return span(p,n,a) && !overlap(p,n,&out,sizeof(out)) &&
            !overlap(p,n,&state,sizeof(state)) && (!controls || !overlap(p,n,controls,sizeof(*controls)));
    }
    template<class T>bool optional(const T* p)const{return !p || field(p,sizeof(T),alignof(T));}
    bool vector()const {
        if(!state.skills)return true;
        if(!field(state.skills,sizeof(Skills),alignof(Skills)))return false;
        auto a=reinterpret_cast<std::uintptr_t>(state.skills->begin),b=reinterpret_cast<std::uintptr_t>(state.skills->end);
        if(!a && !b)return true;
        return a && b>=a && (b-a)%sizeof(std::uintptr_t)==0 &&
            (b-a)/sizeof(std::uintptr_t)<=65536 && field(state.skills->begin,b-a,alignof(std::uintptr_t));
    }
    bool valid()const {
        if(!optional(state.load_phase_28) || !optional(state.current_slot_cc) || !optional(state.assertion_level) || !vector())return false;
        if(!state.character)return true;
        if(!field(state.character,sizeof(Character),alignof(Character)) || !optional(state.character->flags_520) || !optional(state.character->machine))return false;
        return !state.character->machine || optional(state.character->machine->current_state_id);
    }
    Status call(Service op,std::uintptr_t subject,const character_skill_state_queries::Machine* machine,std::uint32_t slot,std::uint32_t& word){
        if(!services.invoke)return Status::service_unavailable;
        out.last_service=op;++out.service_calls;
        const bool log=op==Service::assertion_log;
        Request q{op,subject,machine,slot,log?181u:0u,
            log?"ASSERT(%s) FAILED: %s:%d\n":nullptr,
            log?"skillId < m_skillScripts.size()":nullptr,
            log?"..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Characters\\AI\\CharAI_Skills.cpp":nullptr};Response r{};
        try{if(services.invoke(services.context,&state,&q,&r))return Status::service_failed;}catch(...){return Status::service_failed;}
        word=r.word;return valid()?Status::complete:Status::invalid_source_fact;
    }
    Status table(std::uint32_t slot,bool usable){
        if(!state.skills || !vector())return Status::invalid_source_fact;
        // Usable initially loads begin/end; dispatcher initially end/begin. No
        // callback intervenes. The selected pointer survives the tail dispatch.
        const auto* begin=state.skills->begin;const auto* end=state.skills->end;
        out.count=begin?std::uint32_t((reinterpret_cast<std::uintptr_t>(end)-reinterpret_cast<std::uintptr_t>(begin))/sizeof(std::uintptr_t)):0;
        if(slot>=out.count){out.decision=Decision::out_of_range;return Status::complete;}
        out.skill=begin[slot];if(!out.skill){out.decision=Decision::null_skill;return Status::complete;}
        std::uint32_t word=0;const auto op=usable?Service::check_usable:out.last_service;
        auto status=call(op,out.skill,nullptr,slot,word);
        if(status==Status::complete){out.decision=Decision::dispatched;if(usable)out.value=word;}
        return status;
    }
    Status run(Operation op,std::uint32_t requested){
        if(op==Operation::loaded){if(!state.load_phase_28)return Status::invalid_source_fact;out.value=*state.load_phase_28>6;out.decision=Decision::loaded;return Status::complete;}
        if(op!=Operation::usable){
            if(!state.current_slot_cc)return Status::invalid_source_fact;
            out.slot=*state.current_slot_cc;out.last_service=op==Operation::focus?Service::pre:op==Operation::event?Service::use:Service::post;
            return table(out.slot,false);
        }
        out.slot=requested;
        auto* owner=state.character;if(!owner || !owner->identity || !owner->machine)return Status::invalid_source_fact;
        std::uint32_t word=0;auto status=call(Service::using_skill,owner->identity,owner->machine,requested,word);if(status!=Status::complete)return status;
        owner=state.character;if(!owner || !owner->identity || !owner->machine)return Status::invalid_source_fact;
        if(word){if(!owner->flags_520)return Status::invalid_source_fact;if(!(*owner->flags_520&0x8000)){out.decision=Decision::using_skill_locked;return Status::complete;}}
        status=call(Service::casting,owner->identity,owner->machine,requested,word);if(status!=Status::complete)return status;
        if(word){out.decision=Decision::casting;return Status::complete;}
        if(!state.load_phase_28)return Status::invalid_source_fact;
        if(*state.load_phase_28<=6){out.decision=Decision::not_loaded;return Status::complete;}
        if(!state.skills)return Status::invalid_source_fact;
        auto count=state.skills->begin?std::uint32_t((reinterpret_cast<std::uintptr_t>(state.skills->end)-reinterpret_cast<std::uintptr_t>(state.skills->begin))/sizeof(std::uintptr_t)):0;
        if(requested>=count){
            out.count=count;out.decision=Decision::out_of_range;if(!state.assertion_level)return Status::invalid_source_fact;
            const auto level=*state.assertion_level;
            if(level==2){out.decision=Decision::fatal_assertion;return Status::unsupported_source_assertion;}
            if(level!=1)return Status::complete;
            status=call(Service::assertion_log,state.ai,nullptr,requested,word);if(status!=Status::complete)return status;
        }
        return table(requested,true);
    }
};
}
Status execute(State* state,Operation op,std::uint32_t slot,const Services* services,Result* out){
    if(!span(state,sizeof(*state),alignof(State)) || !span(out,sizeof(*out),alignof(Result)) ||
       op>Operation::blur || overlap(state,sizeof(*state),out,sizeof(*out)) || !state->ai ||
       (services && (!span(services,sizeof(*services),alignof(Services)) || overlap(state,sizeof(*state),services,sizeof(*services)) || overlap(out,sizeof(*out),services,sizeof(*services)))))return Status::invalid_argument;
    Kernel kernel{*state,services,services?*services:Services{},*out};
    if(!kernel.valid())return Status::invalid_argument;
    *out={};return kernel.run(op,slot);
}
} // namespace dh2::character_ai_skill_dispatch_v1
