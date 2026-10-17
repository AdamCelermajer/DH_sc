#include "character_ai_update_target.hpp"
#include <cstddef>

namespace dh2::character_ai_update_target { namespace {
struct Range { std::uintptr_t begin,end; };
bool range(const void* p,std::size_t n,std::size_t alignment,Range& r) {
    const auto at=reinterpret_cast<std::uintptr_t>(p);
    if(!p || at%alignment || at>UINTPTR_MAX-n) return false;
    r={at,at+n};return true;
}
bool overlap(Range a,Range b) {return a.begin<b.end && b.begin<a.end;}
bool owner_valid(const State* state,const Range (&r)[3]) {
    Range borrowed;
    if(!range(state->owner,sizeof(Owner),alignof(Owner),borrowed))return false;
    for(const auto& protected_range:r)if(overlap(borrowed,protected_range))return false;
    return state->owner->identity!=0;
}
struct Execution {
    State* state;const Services services;Result* out;
    const std::uintptr_t original_ai;
    const Range (&ranges)[3];
    Status query(Operation op,Subject kind,std::uintptr_t subject,
                 std::uintptr_t other,std::uint32_t& word,std::uint32_t event=0) {
        if(!services.invoke)return Status::service_unavailable;
        ++out->service_calls;Response response{};
        const Request request{op,kind,subject,other,event};
        try {if(services.invoke(services.context,state,&request,&response))return Status::service_failed;}
        catch(...) {return Status::service_failed;}
        word=response.word;return Status::complete;
    }
    Status owner(Operation op,std::uint32_t& word) {
        if(!owner_valid(state,ranges))return Status::invalid_source_fact;
        const auto captured=state->owner->identity;
        return query(op,Subject::owner,captured,0,word);
    }
    Status event(std::uint32_t id) {
        if(!owner_valid(state,ranges))return Status::invalid_source_fact;
        const auto owner=state->owner->identity;
        const auto target=state->target;
        ++out->event_calls;out->last_event=id;std::uint32_t ignored=0;
        return query(Operation::raise_event,Subject::owner,owner,target,ignored,id);
    }
    Status interactive(std::uint32_t& word) {
        if(!state->target || !owner_valid(state,ranges))return Status::invalid_source_fact;
        const auto target=state->target;
        const auto owner=state->owner->identity;
        return query(Operation::is_interactive,Subject::object,target,owner,word);
    }
    Status ai(Operation op,std::uint32_t& word) {
        return query(op,Subject::original_ai,original_ai,state->target,word);
    }
};
} // namespace
Status update(State* state,const Services* services,Result* out) {
    Range ranges[3];
    if(!range(state,sizeof(*state),alignof(State),ranges[0]) ||
       !range(services,sizeof(*services),alignof(Services),ranges[1]) ||
       !range(out,sizeof(*out),alignof(Result),ranges[2]))return Status::invalid_argument;
    for(unsigned i=0;i<3;++i)for(unsigned j=0;j<i;++j)
        if(overlap(ranges[i],ranges[j]))return Status::invalid_argument;
    if(!state->identity || !owner_valid(state,ranges))return Status::invalid_argument;
    Execution e{state,*services,out,state->identity,ranges};*out={};
    std::uint32_t word=0;
    auto status=e.owner(Operation::is_awaiting_spawn,word);
    if(status!=Status::complete)return status;
    if(word) {out->decision=Decision::awaiting_spawn;return Status::complete;}
    status=e.owner(Operation::is_in_limbus,word);
    if(status!=Status::complete)return status;
    if(word) {out->decision=Decision::in_limbus;return Status::complete;}
    if(!state->target) {out->decision=Decision::no_target;return Status::complete;}
    status=e.interactive(word);
    if(status!=Status::complete)return status;
    if(!word) {
        // Original captures owner BEFORE directly zeroing target and last target.
        if(!owner_valid(state,ranges))return Status::invalid_source_fact;
        const auto captured=state->owner->identity;
        state->target=state->last_target=0;
        ++out->event_calls;out->last_event=12;
        status=e.query(Operation::raise_event,Subject::owner,captured,0,word,12);
        if(status==Status::complete)out->decision=Decision::cleared_untargetable;
        return status;
    }
    if(!state->target) {out->decision=Decision::no_target_after_callback;return Status::complete;}
    status=e.owner(Operation::char_ai_id,word); // real, discarded source call
    if(status!=Status::complete)return status;
    if(!state->target)return Status::invalid_source_fact; // source would dereference null
    status=e.query(Operation::is_dead,Subject::object,state->target,0,word);
    if(status!=Status::complete)return status;
    const auto alive=static_cast<std::uint8_t>(word^1u); // XOR1 then UXTB, not !word
    out->computed_alive=alive;
    if(state->alive_snapshot) {
        if(!alive) {status=e.event(10);if(status!=Status::complete)return status;}
    } else if(alive) {status=e.event(11);if(status!=Status::complete)return status;}
    const auto sight_target=state->target; // source captures target before store48
    state->alive_snapshot=alive;
    if(!sight_target) {out->decision=Decision::no_target_after_callback;return Status::complete;}
    status=e.query(Operation::is_in_sight,Subject::original_ai,e.original_ai,sight_target,word);
    if(status!=Status::complete)return status;
    const auto sight=word; // keep full raw word through range decision
    out->computed_sight=sight;
    if(state->sight_snapshot) {
        if(!sight) {status=e.event(12);if(status!=Status::complete)return status;}
    } else if(sight) {status=e.event(13);if(status!=Status::complete)return status;}
    const auto range_target=state->target; // source captures target before store49
    state->sight_snapshot=static_cast<std::uint8_t>(sight);
    if(!range_target) {out->decision=Decision::no_target_after_callback;return Status::complete;}
    if(!sight) {out->decision=Decision::out_of_sight;return Status::complete;}
    status=e.interactive(word);
    if(status!=Status::complete)return status;
    if(!word) {out->decision=Decision::no_longer_interactive;return Status::complete;}
    status=e.owner(Operation::can_range_attack,word);
    if(status!=Status::complete)return status;
    std::uint32_t range_event=14;
    if(word) {
        status=e.ai(Operation::is_in_close_range,word);
        if(status!=Status::complete)return status;
        if(word)range_event=16;
        else {
            status=e.ai(Operation::is_in_range,word);
            if(status!=Status::complete)return status;
            if(word)range_event=15;
        }
    } else {
        status=e.ai(Operation::is_in_melee_range,word);
        if(status!=Status::complete)return status;
        if(word)range_event=17;
    }
    out->range_event=range_event;
    status=e.event(range_event);
    if(status==Status::complete)out->decision=Decision::range_event;
    return status;
}
} // namespace dh2::character_ai_update_target
