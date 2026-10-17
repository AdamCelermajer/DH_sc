#include "character_skill_state_queries.hpp"
#include <limits>
namespace dh2::character_skill_state_queries {namespace {
struct Range {std::uintptr_t a,b;};
template<class T>bool range(const T* p,Range& r){
    const auto a=reinterpret_cast<std::uintptr_t>(p);
    if(!p||a%alignof(T)||a>std::numeric_limits<std::uintptr_t>::max()-sizeof(T))return false;
    r={a,a+sizeof(T)};return true;
}
bool overlap(Range a,Range b){return a.a<b.b&&b.a<a.b;}
}
Status query(Query kind,const Machine* machine,Result* out){
    Range control,result;
    if((kind!=Query::using_skill&&kind!=Query::casting)||!range(machine,control)||
       !range(out,result)||overlap(control,result))return Status::invalid_argument;
    const auto current=machine->current_state_id;
    if(current){
        Range state;
        if(!range(current,state)||overlap(state,control)||overlap(state,result))return Status::invalid_argument;
    }
    const auto word=current?static_cast<std::uint32_t>(*current):0xffffffffu;
    const auto expected=kind==Query::using_skill?6u:7u;
    *out={word,word==expected?1u:0u};
    return Status::complete;
}
} // namespace dh2::character_skill_state_queries
