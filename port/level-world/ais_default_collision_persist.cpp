#include "ais_default_collision_persist.hpp"
#include <cstddef>
namespace dh2::ais_default_collision_persist { namespace {
struct Range {std::uintptr_t start,end;};
bool range(const void* p,std::size_t n,std::size_t alignment,Range& r) {
    const auto at=reinterpret_cast<std::uintptr_t>(p);
    if(!p || at%alignment || at>UINTPTR_MAX-n)return false;
    r={at,at+n};return true;
}
bool overlap(Range a,Range b){return a.start<b.end && b.start<a.end;}
template<class T> bool valid_view(const T* p,const Range (&controls)[4]) {
    Range r;if(!range(p,sizeof(*p),alignof(T),r))return false;
    for(auto c:controls)if(overlap(r,c))return false;
    return true;
}
}
Status persist(State* state,std::uintptr_t peer,std::uint32_t collision_flag,
               const Services* services,Result* result) {
    Range controls[4];
    if(!range(state,sizeof(*state),alignof(State),controls[0]) ||
       !range(services,sizeof(*services),alignof(Services),controls[1]) ||
       !range(result,sizeof(*result),alignof(Result),controls[2]))return Status::invalid_argument;
    if(!range(state->ais,sizeof(*state->ais),alignof(ais_external_update::State),controls[3]))return Status::invalid_argument;
    for(unsigned i=0;i<4;++i)for(unsigned j=0;j<i;++j)if(overlap(controls[i],controls[j]))return Status::invalid_argument;
    if(!state->ais->ais || !state->ais->owner)return Status::invalid_argument;
    auto* const ais=state->ais;const Services bound=*services;*result={};
    auto done=[&](Decision decision){result->decision=decision;return Status::complete;};
    auto call=[&](Operation op,std::uintptr_t subject,std::uintptr_t other,
                  std::uint32_t argument,std::uint32_t& value) {
        if(!subject)return Status::invalid_source_fact;
        if(!bound.invoke)return Status::service_unavailable;
        const Request request{op,subject,other,argument};value=0;++result->service_calls;
        if(op==Operation::set_target)++result->set_target_calls;
        if(op==Operation::cancel_sneaking)++result->cancel_sneaking_calls;
        try {if(bound.invoke(bound.context,state,&request,&value))return Status::service_failed;}
        catch(...){return Status::service_failed;}
        return Status::complete;
    };
    auto lookup=[&](View kind,std::uintptr_t id,const void** value) {
        if(kind!=View::application && !id)return Status::invalid_source_fact;
        if(!bound.view)return Status::service_unavailable;
        *value=nullptr;++result->view_reads;
        try {if(bound.view(bound.context,kind,id,value))return Status::service_failed;}
        catch(...){return Status::service_failed;}
        return Status::complete;
    };
    auto owner=[&](const OwnerFacts** out) {
        const auto id=ais->owner;const void* value=nullptr;
        const auto status=lookup(View::owner,id,&value);
        if(status!=Status::complete)return status;
        const auto* facts=static_cast<const OwnerFacts*>(value);
        if(!valid_view(facts,controls) || facts->identity!=id)return Status::invalid_source_fact;
        *out=facts;return Status::complete;
    };
    std::uint32_t value=0;
    auto status=call(Operation::state_is_moving,ais->owner,0,0,value);
    if(status!=Status::complete)return status;
    if(!value)return done(Decision::not_moving);
    const OwnerFacts* facts=nullptr;status=owner(&facts);if(status!=Status::complete)return status;
    const auto current=facts->target_408;result->captured_target=current;
    if(peer==current)return done(Decision::current_target);
    status=call(Operation::is_character,peer,0,0,value);if(status!=Status::complete)return status;
    if(value) {
        status=owner(&facts);if(status!=Status::complete)return status;
        const auto master=facts->master_418;result->captured_master=master;
        const auto player_owner=facts->identity;
        status=call(Operation::is_player,player_owner,0,0,value);if(status!=Status::complete)return status;
        // The original ARM pointer test is exactly master==null OR !=current.
        if(!value && (!master || master!=current)) {
            status=call(Operation::is_enemy,ais->owner,peer,0,value);if(status!=Status::complete)return status;
            if(value) {
                status=call(Operation::set_target,ais->owner,peer,0,value);
                if(status!=Status::complete)return status;
                return done(Decision::target_set); // original tail branch
            }
        }
        status=call(Operation::is_player,ais->owner,0,0,value);if(status!=Status::complete)return status;
        if(value) {
            status=call(Operation::is_enemy,ais->owner,peer,0,value);if(status!=Status::complete)return status;
            if(value) {
                status=call(Operation::cancel_sneaking,ais->owner,0,0,value);
                if(status!=Status::complete)return status;
            }
        }
    }
    if(!collision_flag)return done(Decision::collision_false);
    // Source performs a SECOND independent virtual IsCharacter query.
    status=call(Operation::is_character,peer,0,0,value);if(status!=Status::complete)return status;
    if(!value) {
        const void* view=nullptr;status=lookup(View::peer,peer,&view);if(status!=Status::complete)return status;
        const auto* facts_peer=static_cast<const PeerFacts*>(view);
        if(!valid_view(facts_peer,controls) || facts_peer->identity!=peer)return Status::invalid_source_fact;
        if(facts_peer->type_f4!=2 && facts_peer->type_f4!=21)return done(Decision::noncountable_peer);
    }
    const auto prior_frame=state->frame_c0; // source read precedes global App load
    const void* app_view=nullptr;status=lookup(View::application,0,&app_view);if(status!=Status::complete)return status;
    const auto* app=static_cast<const Application*>(app_view);
    if(!valid_view(app,controls) || !app->identity)return Status::invalid_source_fact;
    const auto frame=app->frame_74;result->frame_word=frame;
    if(prior_frame==frame)return done(Decision::same_frame);
    status=owner(&facts);if(status!=Status::complete)return status;
    if(facts->byte_3e0>255)return Status::invalid_source_fact;
    if(facts->byte_3e0)return done(Decision::owner_flagged);
    state->frame_c0=frame; // source store BEFORE +bc capture/GetDt
    const auto counter=ais->counter_bc;
    status=call(Operation::frame_delta,app->identity,0,0,value);if(status!=Status::complete)return status;
    result->delta_word=value;ais->counter_bc=counter+value;
    result->counter_added=1;return done(Decision::counter_added);
}
} // namespace dh2::ais_default_collision_persist
