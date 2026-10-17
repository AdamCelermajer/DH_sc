#include "character_equipped_faery_element_v1.hpp"
#include <cstdio>
#include <cstring>

namespace dh2::character_equipped_faery_element_v1 {namespace {
struct Range {std::uintptr_t b,e;};
bool range(const void* p,std::size_t n,std::size_t alignment,Range& r) noexcept {
    const auto at=reinterpret_cast<std::uintptr_t>(p);
    if(!p||at%alignment||at>UINTPTR_MAX-n)return false;
    r={at,at+n};return true;
}
template<class T>bool span(const T* p,Range& r) noexcept {return range(p,sizeof(T),alignof(T),r);}
bool overlap(Range a,Range b) noexcept {return a.b<b.e&&b.b<a.e;}
int fail(char* text,std::size_t n) noexcept {
    if(text&&n)std::snprintf(text,n,"EquippedFaeryElement requires genuine selected save/faery row providers");
    return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
}
int saved_invoke(void* raw,const Request* q,Response* out){
    auto* b=static_cast<SavedBindings*>(raw);Range br,qr,rr;
    if(!span(b,br)||!span(q,qr)||!span(out,rr)||overlap(br,qr)||overlap(br,rr)||overlap(qr,rr)||
       !b->character||q->character!=b->character||q->difficulty!=-1)return -1;
    if(q->operation==Operation::selected_faery){
        auto services=character_current_spell_v1::saved_services(b);
        const character_current_spell_v1::Request request{character_current_spell_v1::Operation::selected_faery,q->id,-1,q->character};
        character_current_spell_v1::Response response{};
        const auto code=services.invoke(services.context,&request,&response);
        if(code)return code;
        out->selected=response.value;return 0;
    }
    if(q->operation!=Operation::faery_row)return -1;
    Range lr,gr,sr;
    if(!span(b->faery_list_106c,lr)||!span(b->faery_globals,gr)||!span(b->faery_services,sr))return -1;
    character_faery_selection::Character character{q->character,*b->faery_list_106c};
    std::int32_t id;std::memcpy(&id,&q->id,4);
    character_faery_selection::Result result{};
    if(character_faery_selection::select(&character,id,b->faery_globals,b->faery_services,&result)!=
       character_faery_selection::Status::complete)return -1;
    out->row=result.row;return 0;
}
}
Status query(std::uintptr_t character,const Services* services,Result* result){
    Range sr,rr;
    if(!character||!span(services,sr)||!span(result,rr)||overlap(sr,rr)||!services->invoke)return Status::invalid_argument;
    const auto bound=*services;*result={};result->character=character;
    const auto call=[&](Operation operation,std::uint32_t id,Response& response){
        result->last_operation=operation;++result->calls;response={};
        const Request request{operation,id,-1,character};
        try{return bound.invoke(bound.context,&request,&response)==0;}catch(...){return false;}
    };
    Response response{};
    if(!call(Operation::selected_faery,0,response))return Status::provider_failed;
    result->selected=static_cast<std::uint32_t>(response.selected);
    if(!call(Operation::faery_row,result->selected,response))return Status::provider_failed;
    Range row;
    if(!span(response.row,row)||overlap(row,sr)||overlap(row,rr))return Status::provider_failed;
    // Original LDR reads the returned live row's word+8 after GetCharFaery.
    std::memcpy(&result->element,&response.row->words[2],4);
    result->complete=1;return Status::complete;
}
Services saved_services(SavedBindings* b) noexcept {return {b,saved_invoke};}
int equipped_faery_element_v1(void* raw,const dh2_script_value*,std::uint32_t,
                             dh2_script_value* output,std::uint32_t capacity,std::uint32_t* returned,
                             char* text,std::size_t n) noexcept {
    auto* b=static_cast<Bindings*>(raw);Range br,rr,vr;
    if(!span(b,br)||!span(returned,rr)||!capacity||!span(output,vr)||
       overlap(br,rr)||overlap(br,vr)||overlap(rr,vr))return fail(text,n);
    *returned=0;
    Result result{};
    if(query(b->character,&b->services,&result)!=Status::complete)return fail(text,n);
    output[0]={};output[0].type=DH2_SCRIPT_NUMBER;output[0].number=static_cast<float>(result.element);*returned=1;return 0;
}
}
