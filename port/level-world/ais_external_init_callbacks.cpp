#include "ais_external_init_callbacks.hpp"

#include <cstddef>

namespace dh2::ais_external_init_callbacks {
namespace {
struct Range {std::uintptr_t begin,end;};
bool range(const void* p,std::size_t bytes,std::size_t alignment,Range& r) noexcept {
    const auto n=reinterpret_cast<std::uintptr_t>(p);
    if(!p||n%alignment||n>UINTPTR_MAX-bytes)return false;
    r={n,n+bytes};return true;
}
bool overlap(Range a,Range b) noexcept {return a.begin<b.end&&b.begin<a.end;}
}

void default_init() noexcept {}
void default_post() noexcept {}
void default_final() noexcept {}

Status invoke(State* state,Callback callback,const Services* services,Result* result) {
    Range controls[3];
    if(!range(state,sizeof(*state),alignof(State),controls[0])||
       !range(services,sizeof(*services),alignof(Services),controls[1])||
       !range(result,sizeof(*result),alignof(Result),controls[2])||
       static_cast<std::uint32_t>(callback)>static_cast<std::uint32_t>(Callback::final))
        return Status::invalid_argument;
    for(unsigned i=0;i<3;++i)for(unsigned j=0;j<i;++j)
        if(overlap(controls[i],controls[j]))return Status::invalid_argument;
    if(!state->ais)return Status::invalid_argument;
    const auto identity=state->ais;
    const Services bound=*services;
    const char* name=callback==Callback::init?"OnInit":callback==Callback::post?"OnInitPost":"OnInitFinal";
    *result={identity,name,0,0};
    if(callback==Callback::init){default_init();result->default_init_completed=1;}
    if(!bound.call)return Status::service_unavailable;
    const Request request{identity,callback,name};
    ++result->calls;
    try{return bound.call(bound.context,state,&request)==0?Status::complete:Status::service_failed;}
    catch(...){return Status::service_failed;}
}
} // namespace dh2::ais_external_init_callbacks
