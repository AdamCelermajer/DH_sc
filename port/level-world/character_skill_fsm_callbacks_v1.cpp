#include "character_skill_fsm_callbacks_v1.hpp"
#include <cstddef>

namespace dh2::character_skill_fsm_callbacks_v1 {
namespace {
struct Range {std::uintptr_t begin,end;};
bool range(const void* p,std::size_t n,std::size_t alignment,Range& out){
    auto at=reinterpret_cast<std::uintptr_t>(p);if(!p || at%alignment || at>UINTPTR_MAX-n)return false;
    out={at,at+n};return true;
}
bool overlap(Range a,Range b){return a.begin<b.end && b.begin<a.end;}
template<class T>bool append(const T* p,Range* ranges,unsigned& count){
    Range next;if(!range(p,sizeof(T),alignof(T),next))return false;
    for(unsigned i=0;i<count;++i)if(overlap(next,ranges[i]))return false;
    ranges[count++]=next;return true;
}
struct Run {
    State* state;Character character;std::uintptr_t debug;Services services;Result& out;
    Status call(Operation operation,std::uintptr_t subject,std::uint32_t a=0,
                std::uint32_t b=0,std::uint32_t c=0,std::uintptr_t string=0,Response* response=nullptr){
        if(!services.invoke)return Status::service_unavailable;
        Request request{operation,character.identity,subject,string,0,a,b,c,
            operation==Operation::string_construct?"isTracingCharState":nullptr};Response reply{};
        out.last_operation=operation;++out.calls;
        try{if(services.invoke(services.context,state,&request,&reply))return Status::service_failed;}
        catch(...){return Status::service_failed;}
        if(response)*response=reply;
        return Status::complete;
    }
    Status query(Operation op,std::uint32_t& value){Response r{};auto status=call(op,character.identity,0,0,0,0,&r);value=r.word;return status;}
    Status tracing(){
        auto status=call(Operation::debug_load,debug);if(status!=Status::complete)return status;
        Response r{};status=call(Operation::string_construct,0,0,0,0,0,&r);if(status!=Status::complete)return status;
        out.string=r.identity;if(!r.identity)return Status::invalid_source_fact;out.debug_constructed=1;
        status=call(Operation::debug_query,debug,0,0,0,out.string);if(status!=Status::complete)return status;
        status=call(Operation::string_destroy,out.string);if(status==Status::complete)out.debug_destroyed=1;
        return status;
    }
    Status classification(bool blur){
        std::uint32_t word;auto status=query(Operation::is_monster,word);if(status!=Status::complete || !word)return status;
        status=query(Operation::is_miniboss,word);if(status!=Status::complete || word)return status;
        status=query(Operation::is_boss,word);if(status!=Status::complete || word)return status;
        if(blur)*character.flags_520&=~0x10000u;else *character.flags_520|=0x10000u;
        ++out.flags_written;return Status::complete;
    }
    Status focus(){
        auto status=tracing();if(status!=Status::complete)return status;
        const auto prior=*character.flags_528;
        *character.flags_520=0x6341;*character.flags_528=prior&~0x140u;out.flags_written=2;
        status=call(Operation::raise_event,character.identity,0x1e);if(status!=Status::complete)return status;
        status=call(Operation::set_animation,character.machine,UINT32_MAX);if(status!=Status::complete)return status;
        status=call(Operation::set_speed,character.animator,0x3f800000);if(status!=Status::complete)return status;
        *character.ooi_intent_412=0;out.ooi_intent_cleared=1;
        status=call(Operation::cancel_sneaking,character.identity);if(status!=Status::complete)return status;
        const auto moving=*character.machine_moving_58;const auto physical=*character.physical_2dc;out.physical=physical;
        if(moving){*character.flags_528|=0x100u;++out.flags_written;}
        if(physical){status=call(Operation::unpin,physical);if(status!=Status::complete)return status;out.physical_called=1;}
        return classification(false);
    }
    Status blur(){
        auto status=tracing();if(status!=Status::complete)return status;
        status=call(Operation::sync_last_target,character.ai);if(status!=Status::complete)return status;
        status=call(Operation::stop,character.identity);if(status!=Status::complete)return status;
        status=call(Operation::raise_event,character.identity,0x1f);if(status!=Status::complete)return status;
        if(*character.flags_528&0x100u){
            out.timer_attempted=1;status=call(Operation::start_timer,character.timers,10,0,0x30);if(status!=Status::complete)return status;
        }else {
            const auto physical=*character.physical_2dc;out.physical=physical;
            if(physical){status=call(Operation::pin,physical);if(status!=Status::complete)return status;out.physical_called=1;}
        }
        return classification(true);
    }
};
}
Status execute(State* state,Callback callback,const Globals* globals,const Services* services,Result* out){
    Range ranges[10];unsigned count=0;
    if(callback>Callback::blur || !append(state,ranges,count) || !append(globals,ranges,count) ||
       !append(services,ranges,count) || !append(out,ranges,count) || !append(state->character,ranges,count))return Status::invalid_argument;
    const auto character=*state->character;
    if(!character.identity || !character.ai || !character.machine || !character.animator || !character.timers || !globals->debug_switches ||
       !append(character.flags_520,ranges,count) || !append(character.flags_528,ranges,count) ||
       !append(character.ooi_intent_412,ranges,count) || !append(character.machine_moving_58,ranges,count) ||
       !append(character.physical_2dc,ranges,count))return Status::invalid_argument;
    Run run{state,character,globals->debug_switches,*services,*out};*out={};out->character=character.identity;out->debug=run.debug;
    auto status=callback==Callback::focus?run.focus():run.blur();if(status==Status::complete)out->complete=1;return status;
}
} // namespace dh2::character_skill_fsm_callbacks_v1
