#include "character_script_set_level.hpp"
#include <cstddef>
#include <cstring>
#include <initializer_list>

namespace dh2::character_script_set_level { namespace {
struct Range {std::uintptr_t first,end;};
bool range(const void* p,std::size_t size,std::size_t alignment,Range& out) {
    const auto at=reinterpret_cast<std::uintptr_t>(p);
    if(!p || at%alignment || at>UINTPTR_MAX-size)return false;
    out={at,at+size};return true;
}
bool overlap(Range a,Range b){return a.first<b.end && b.first<a.end;}
std::int32_t signed_word(std::uint32_t bits){std::int32_t out;std::memcpy(&out,&bits,4);return out;}
Status invoke(Character* owner,const Services& services,Result* out,const Request& request,std::uint32_t* value) {
    if(!services.invoke)return Status::service_unavailable;
    ++out->calls;out->last_operation=std::uint32_t(request.operation);
    try {if(services.invoke(services.context,owner,&request,value))return Status::service_failed;}
    catch(...){return Status::service_failed;}
    return Status::complete;
}
}
Status set_level(Character* owner,const Arguments* arguments,const Globals* globals,const Services* services,Result* out) {
    Range controls[5];
    if(!range(owner,sizeof(*owner),alignof(Character),controls[0]) ||
       !range(arguments,sizeof(*arguments),alignof(Arguments),controls[1]) ||
       !range(globals,sizeof(*globals),alignof(Globals),controls[2]) ||
       !range(services,sizeof(*services),alignof(Services),controls[3]) ||
       !range(out,sizeof(*out),alignof(Result),controls[4]))return Status::invalid_argument;
    for(unsigned i=0;i<5;++i)for(unsigned j=0;j<i;++j)if(overlap(controls[i],controls[j]))return Status::invalid_argument;
    if(!owner->identity || !owner->properties_identity || !arguments->identity)return Status::invalid_argument;
    const auto character=owner->identity,properties=owner->properties_identity,args=arguments->identity;
    const auto bound=*services;*out={};
    if(!arguments->count || arguments->first_type!=3)return Status::complete;
    std::uint32_t number=0,maximum=0,converted=0;
    auto status=invoke(owner,bound,out,{Operation::get_number,args,0,0,nullptr,nullptr},&number);
    if(status!=Status::complete)return status;
    const auto application=globals->application;Range app;
    if(!range(application,sizeof(*application),alignof(Application),app))return Status::invalid_argument;
    for(auto control:controls)if(overlap(control,app))return Status::invalid_argument;
    if(!application->identity || !application->design_manager)return Status::invalid_argument;
    constexpr const char* category="CharacterDesign";constexpr const char* key="MaxLevelDVeryHard";
    status=invoke(owner,bound,out,{Operation::design_max_level,application->design_manager,0,0,category,key},&maximum);
    if(status!=Status::complete)return status;
    const auto first_limit=maximum<<8;
    status=invoke(owner,bound,out,{Operation::float_to_signed,0,0,number,nullptr,nullptr},&converted);
    if(status!=Status::complete)return status;
    const bool clamped=signed_word(first_limit)<signed_word(converted);
    if(clamped) {
        if(!application->design_manager)return Status::invalid_argument;
        status=invoke(owner,bound,out,{Operation::design_max_level,application->design_manager,0,0,category,key},&maximum);
        if(status!=Status::complete)return status;
        converted=maximum<<8;
    }else {
        status=invoke(owner,bound,out,{Operation::get_number,args,0,0,nullptr,nullptr},&number);
        if(status!=Status::complete)return status;
        status=invoke(owner,bound,out,{Operation::float_to_signed,0,0,number,nullptr,nullptr},&converted);
        if(status!=Status::complete)return status;
    }
    owner->base_level_5b8=converted;out->applied=1;out->clamped=std::uint32_t(clamped);
    for(const auto op:{Operation::recalc_properties,Operation::regen_hp,Operation::regen_mp}) {
        const bool recalc=op==Operation::recalc_properties;
        std::uint32_t ignored=0;
        status=invoke(owner,bound,out,{op,recalc?properties:character,recalc?1u:UINT32_MAX,0,nullptr,nullptr},&ignored);
        if(status!=Status::complete)return status;
    }
    return Status::complete;
}
} // namespace dh2::character_script_set_level
