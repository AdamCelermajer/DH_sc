// Adapted from AdamCelermajer/DH_sc c3ae797332a82a30a586b9156cddc25445e36a4c,
// port/level-world/character_script_player_vcb_v2.cpp. Default caller is reused.
#include "ais_player_init_vcb.hpp"
#include <cstddef>

namespace dh2::ais_player_init_vcb { namespace {
struct Range {std::uintptr_t first,end;};
bool range(const void* p,std::size_t n,std::size_t alignment,Range& r) {
    const auto at=reinterpret_cast<std::uintptr_t>(p);
    if(!p || at%alignment || at>UINTPTR_MAX-n)return false;
    r={at,at+n};return true;
}
bool overlap(Range a,Range b){return a.first<b.end && b.first<a.end;}
}
Status initialize(State* state,const Services* services,Result* result) {
    Range controls[3];
    if(!range(state,sizeof(*state),alignof(State),controls[0]) ||
       !range(services,sizeof(*services),alignof(Services),controls[1]) ||
       !range(result,sizeof(*result),alignof(Result),controls[2]))return Status::invalid_argument;
    for(unsigned i=0;i<3;++i)for(unsigned j=0;j<i;++j)
        if(overlap(controls[i],controls[j]))return Status::invalid_argument;
    if(!state->ais)return Status::invalid_argument;
    const auto bound=*services;
    const auto status=ais_external_init_vcb::initialize_default(state,&bound,result);
    if(status!=Status::complete)return status;
    const auto base=state->flags_b8; // ldr r5,[r4,#0xb8] before membership callback
    ++result->service_calls;result->last_bit=0x400;
    bool member=false;
    try {if(bound.contains(bound.context,state,"OnKill",&member))return Status::service_failed;}
    catch(...){return Status::service_failed;}
    state->flags_b8=base|(member?0x400u:0u);++result->writes;
    return Status::complete;
}
}
