#include "character_regeneration.hpp"
#include <cstddef>
#include <cstring>

namespace dh2::character_regeneration { namespace {
struct Range {std::uintptr_t first,end;};
bool range(const void* p,std::size_t n,std::size_t alignment,Range& out) {
    const auto at=reinterpret_cast<std::uintptr_t>(p);
    if(!p || at%alignment || at>UINTPTR_MAX-n)return false;
    out={at,at+n};return true;
}
bool overlap(Range a,Range b){return a.first<b.end && b.first<a.end;}
std::int32_t signed_word(std::uint32_t x){std::int32_t v;std::memcpy(&v,&x,4);return v;}
Status invoke(const Services& bound,Result* out,const Request& request,Reply& reply) {
    if(!bound.invoke)return Status::service_unavailable;
    ++out->calls;out->last_operation=std::uint32_t(request.operation);reply={};
    try {if(bound.invoke(bound.context,&request,&reply))return Status::service_failed;}
    catch(...){return Status::service_failed;}
    return Status::complete;
}
Status regenerate(const State* state,const Globals* globals,std::uint32_t amount,const Services* services,Result* out,bool mana) {
    Range controls[4];
    if(!range(state,sizeof(*state),alignof(State),controls[0]) ||
       !range(globals,sizeof(*globals),alignof(Globals),controls[1]) ||
       !range(services,sizeof(*services),alignof(Services),controls[2]) ||
       !range(out,sizeof(*out),alignof(Result),controls[3]))return Status::invalid_argument;
    for(unsigned i=0;i<4;++i)for(unsigned j=0;j<i;++j)if(overlap(controls[i],controls[j]))return Status::invalid_argument;
    const auto owner=*state;const auto bound=*services;
    if(!owner.character || !owner.properties || !owner.resolved_sheet)return Status::invalid_argument;
    *out={};Reply reply{};const std::uint32_t current_id=mana?41u:36u,maximum_id=mana?43u:38u;
    auto status=invoke(bound,out,{Operation::read_property,owner.properties,owner.resolved_sheet,current_id,0,nullptr},reply);
    if(status!=Status::complete)return status;
    const auto current=reply.word;out->current=current;
    status=invoke(bound,out,{Operation::read_property,owner.properties,owner.resolved_sheet,maximum_id,0,nullptr},reply);
    if(status!=Status::complete)return status;
    const auto maximum=reply.word;out->maximum=maximum;
    if(signed_word(amount)<0)amount=maximum;
    if(signed_word(current+amount)>signed_word(maximum))amount=maximum-current;
    if(signed_word(amount)<=0)return Status::complete;
    out->positive_amount=amount;
    const auto debug=globals->debug_switches;
    if(!debug)return Status::invalid_argument;
    status=invoke(bound,out,{Operation::debug_load,debug,0,0,0,nullptr},reply);
    if(status!=Status::complete)return status;
    constexpr const char* key="isTracingChar_Stats";
    status=invoke(bound,out,{Operation::string_construct,0,0,0,0,key},reply);
    if(status!=Status::complete)return status;
    const auto string=reply.identity;
    if(!string)return Status::service_failed;
    status=invoke(bound,out,{Operation::debug_query,debug,string,0,0,nullptr},reply);
    if(status!=Status::complete)return status;
    status=invoke(bound,out,{Operation::string_destroy,string,0,0,0,nullptr},reply);
    if(status!=Status::complete)return status;
    status=invoke(bound,out,{Operation::add_property,owner.properties,0,current_id,amount,nullptr},reply);
    if(status==Status::complete)out->added=1;
    return status;
}
}
Status regen_hp(const State* state,const Globals* globals,std::uint32_t amount,const Services* services,Result* out){return regenerate(state,globals,amount,services,out,false);}
Status regen_mp(const State* state,const Globals* globals,std::uint32_t amount,const Services* services,Result* out){return regenerate(state,globals,amount,services,out,true);}
} // namespace dh2::character_regeneration
