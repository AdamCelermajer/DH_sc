#include "ais_external_init_vcb.hpp"
#include <cstddef>

namespace dh2::ais_external_init_vcb { namespace {
struct Range{std::uintptr_t first,end;};
bool range(const void* p,std::size_t bytes,std::size_t alignment,Range& out) {
    const auto at=reinterpret_cast<std::uintptr_t>(p);
    if(!p || at%alignment || at>UINTPTR_MAX-bytes)return false;
    out={at,at+bytes};return true;
}
bool overlap(Range a,Range b){return a.first<b.end && b.first<a.end;}
struct Entry{const char* name;std::uint32_t bit;};
constexpr Entry entries[]={{"OnTargetHit",0x800},{"OnTargetMissed",0x1000},
    {"OnUpdate",1},{"OnFriendSpotted",2},{"OnTargetOutOfRange",4},
    {"OnTargetInRangedRange",8},{"OnTargetInCloseRange",0x10},
    {"OnTargetInMeleeRange",0x20},{"OnMasterOutOfRange",0x40},
    {"OnMasterInRangedRange",0x80},{"OnMasterInCloseRange",0x100},
    {"OnMasterInMeleeRange",0x200}};
Status initialize(State* state,const Services* services,Result* result,bool external) {
    Range controls[3];
    if(!range(state,sizeof(*state),alignof(State),controls[0]) ||
       !range(services,sizeof(*services),alignof(Services),controls[1]) ||
       !range(result,sizeof(*result),alignof(Result),controls[2]))return Status::invalid_argument;
    for(unsigned i=0;i<3;++i)for(unsigned j=0;j<i;++j)
        if(overlap(controls[i],controls[j]))return Status::invalid_argument;
    if(!state->ais)return Status::invalid_argument;
    const auto bound=*services;*result={};
    state->flags_b8=0;++result->writes;
    std::uint32_t accumulator=0;
    const unsigned count=external?12:2;
    for(unsigned index=0;index<count;++index) {
        if(index==2)accumulator=state->flags_b8; // original External reload after Default
        if(!bound.contains)return Status::service_unavailable;
        ++result->service_calls;result->last_bit=entries[index].bit;
        bool member=false;
        try {if(bound.contains(bound.context,state,entries[index].name,&member))return Status::service_failed;}
        catch(...){return Status::service_failed;}
        if(index==0)accumulator=member?entries[index].bit:0;
        else if(member)accumulator|=entries[index].bit;
        state->flags_b8=accumulator;++result->writes;
    }
    return Status::complete;
}
}
Status initialize_default(State* state,const Services* services,Result* result) {
    return initialize(state,services,result,false);
}
Status initialize_external(State* state,const Services* services,Result* result) {
    return initialize(state,services,result,true);
}
} // namespace dh2::ais_external_init_vcb
