#include "character_interactive.hpp"
#include <cstddef>

namespace dh2::character_interactive { namespace {
struct Range {std::uintptr_t first,end;};
bool range(const void* pointer,std::size_t bytes,std::size_t alignment,Range& out) {
    const auto at=reinterpret_cast<std::uintptr_t>(pointer);
    if(!pointer || at%alignment || at>UINTPTR_MAX-bytes)return false;
    out={at,at+bytes};return true;
}
bool overlaps(Range a,Range b){return a.first<b.end && b.first<a.end;}
}
Status evaluate(Character* character,std::uintptr_t interacting_object,
                const Services* services,Result* result) {
    Range controls[3];
    if(!range(character,sizeof(*character),alignof(Character),controls[0]) ||
       !range(services,sizeof(*services),alignof(Services),controls[1]) ||
       !range(result,sizeof(*result),alignof(Result),controls[2]))return Status::invalid_argument;
    for(unsigned i=0;i<3;++i)for(unsigned j=0;j<i;++j)
        if(overlaps(controls[i],controls[j]))return Status::invalid_argument;
    if(!character->identity || !character->ai)return Status::invalid_argument;
    const auto identity=character->identity,ai=character->ai;
    const auto bound=*services;*result={};std::uint32_t value=0;
    auto query=[&](Query operation,std::uintptr_t subject,std::uintptr_t other=0) {
        if(!bound.invoke)return Status::service_unavailable;
        ++result->service_calls;result->last_query=static_cast<std::uint32_t>(operation);value=0;
        try {if(bound.invoke(bound.context,character,operation,subject,other,&value))return Status::service_failed;}
        catch(...){return Status::service_failed;}
        return Status::complete;
    };
    auto status=query(Query::is_dead,identity);
    if(status!=Status::complete)return status;
    if(value && interacting_object) {
        status=query(Query::is_friend,ai,interacting_object);
        if(status!=Status::complete)return status;
        if(value) {
            status=query(Query::is_monster,identity);
            if(status!=Status::complete)return status;
            if(!value){result->value=1;return Status::complete;}
        }
    }
    if(character->deleted_81 || !character->enabled_8a)return Status::complete;
    status=query(Query::is_faerie,identity);
    if(status!=Status::complete || value)return status;
    status=query(Query::is_summoned,identity);
    if(status!=Status::complete || value)return status;
    status=query(Query::is_dead,identity);
    if(status!=Status::complete || value)return status;
    if(character->flags_520&0x2000)result->value=character->interactive_415;
    return Status::complete;
}
} // namespace dh2::character_interactive
