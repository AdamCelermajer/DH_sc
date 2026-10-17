#include "character_init_hp_mp.hpp"
#include <limits>
namespace dh2::character_init_hp_mp {namespace {
struct Range {std::uintptr_t a,b;};
template<class T>bool range(const T* p,Range& out){
    const auto a=std::uintptr_t(p);
    if(!p||a%alignof(T)||a>std::numeric_limits<std::uintptr_t>::max()-sizeof(T))return false;
    out={a,a+sizeof(T)};return true;
}
bool overlap(Range a,Range b){return a.a<b.b&&b.a<a.b;}
Status invoke(const Services& services,Operation op,const Request& request,Result& out){
    out.last_operation=std::uint32_t(op);
    const auto fn=op==Operation::hp?services.regen_hp:services.regen_mp;
    if(!fn)return Status::service_unavailable;
    ++out.calls;
    try{if(fn(services.context,&request))return Status::service_failed;}
    catch(...){return Status::service_failed;}
    return Status::complete;
}
}
Status execute(const Owner* owner,const Services* services,Result* out){
    Range controls[3];
    if(!range(owner,controls[0])||!range(services,controls[1])||!range(out,controls[2]))return Status::invalid_argument;
    for(unsigned i=0;i<3;++i)for(unsigned j=0;j<i;++j)if(overlap(controls[i],controls[j]))return Status::invalid_argument;
    const auto character=owner->character;const auto bound=*services;
    if(!character)return Status::invalid_argument;
    *out={character,0,0,0,0};const Request request{character,0xffffffffu};
    auto status=invoke(bound,Operation::hp,request,*out);if(status!=Status::complete)return status;++out->hp_completed;
    status=invoke(bound,Operation::mp,request,*out);if(status!=Status::complete)return status;++out->mp_completed;
    return Status::complete;
}
} // namespace dh2::character_init_hp_mp
