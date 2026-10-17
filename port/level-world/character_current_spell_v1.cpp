#include "character_current_spell_v1.hpp"
#include <cstdio>
#include <cstring>

namespace dh2::character_current_spell_v1 {namespace {
struct Range {std::uintptr_t begin,end;};
bool range(const void* p,std::size_t n,std::size_t alignment,Range& out) noexcept {
    auto at=reinterpret_cast<std::uintptr_t>(p);
    if(!p || at%alignment || at>UINTPTR_MAX-n)return false;
    out={at,at+n};return true;
}
bool overlaps(Range a,Range b) noexcept {return a.begin<b.end && b.begin<a.end;}
template<class T>bool span(const T* p,Range& r) noexcept {return range(p,sizeof(T),alignof(T),r);}
int fail(char* text,std::size_t size,const char* why) noexcept {
    if(text&&size)std::snprintf(text,size,"%s",why);
    return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
}
int saved_invoke(void* raw,const Request* request,Response* response){
    auto* b=static_cast<SavedBindings*>(raw);Range controls[3];
    if(!span(b,controls[0]) || !span(request,controls[1]) || !span(response,controls[2]) ||
       overlaps(controls[0],controls[1]) || overlaps(controls[0],controls[2]) || overlaps(controls[1],controls[2]) ||
       !b->character || request->character!=b->character || request->difficulty!=-1)return -1;
    if(request->operation==Operation::validate_faery){
        Range list,globals,services;
        if(!span(b->faery_list_106c,list) || !span(b->faery_globals,globals) || !span(b->faery_services,services))return -1;
        character_faery_selection::Character character{request->character,*b->faery_list_106c};
        character_faery_selection::Result result{};
        std::int32_t id;static_assert(sizeof(id)==sizeof(request->id));
        // Original r1 is a raw word passed to the signed GetCharFaery index.
        std::memcpy(&id,&request->id,sizeof(id));
        return character_faery_selection::select(&character,id,b->faery_globals,b->faery_services,&result)==
            character_faery_selection::Status::complete?0:-1;
    }
    if(request->operation!=Operation::selected_faery && request->operation!=Operation::saved_level)return -1;
    Range slot;if(!span(b->savegame_14e8,slot))return -1;
    const auto* save=*b->savegame_14e8;
    if(!save){response->value=request->operation==Operation::selected_faery?0:-1;return 0;}
    Range owner,difficulty;
    if(!span(save,owner) || save->character()!=request->character || !span(b->difficulty,difficulty))return -1;
    const auto current=*b->difficulty;
    if(current<0 || current>=3)return -1;
    if(request->operation==Operation::selected_faery){response->value=save->current_faery(static_cast<std::uint32_t>(current));return 0;}
    if(!save->faeries_initialized()[static_cast<std::size_t>(current)] || request->id>=5)return -1;
    response->value=save->faery_level(request->id,static_cast<std::uint32_t>(current));return 0;
}
}
Status query(std::uintptr_t character,const Services* services,Result* result){
    Range s,r;
    if(!character || !span(services,s) || !span(result,r) || overlaps(s,r) || !services->invoke)return Status::invalid_argument;
    const auto bound=*services;*result={};result->character=character;
    const auto call=[&](Operation operation,std::uint32_t id,Response& response){
        result->last_operation=operation;++result->calls;
        const Request request{operation,id,-1,character};response={};
        try{return bound.invoke(bound.context,&request,&response)==0;}catch(...){return false;}
    };
    Response response{};
    if(!call(Operation::selected_faery,0,response))return Status::provider_failed;
    result->first_id=static_cast<std::uint32_t>(response.value);
    if(!call(Operation::validate_faery,result->first_id,response))return Status::provider_failed;
    if(!call(Operation::selected_faery,0,response))return Status::provider_failed;
    result->second_id=static_cast<std::uint32_t>(response.value);
    if(!call(Operation::saved_level,result->second_id,response))return Status::provider_failed;
    result->level=response.value;result->complete=1;return Status::complete;
}
int current_spell_info_v1(void* raw,const dh2_script_value*,std::uint32_t,
                         dh2_script_value* output,std::uint32_t capacity,std::uint32_t* returned,
                         char* text,std::size_t size) noexcept {
    auto* bindings=static_cast<Bindings*>(raw);Range b,r,v;
    if(!span(bindings,b) || !span(returned,r) || !capacity || !span(output,v) ||
       overlaps(b,r) || overlaps(b,v) || overlaps(r,v))return fail(text,size,"invalid CurrentSpell callback controls");
    *returned=0;
    Result result{};const auto status=query(bindings->character,&bindings->services,&result);
    if(status!=Status::complete)return fail(text,size,"CurrentSpell requires genuine selected-faery/table/saved-level providers");
    output[0]={};output[0].type=DH2_SCRIPT_NUMBER;output[0].number=static_cast<float>(result.level);*returned=1;return 0;
}
Services saved_services(SavedBindings* bindings) noexcept {return {bindings,saved_invoke};}
}
