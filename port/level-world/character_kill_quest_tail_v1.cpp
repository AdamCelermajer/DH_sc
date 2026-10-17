#include "character_kill_quest_tail_v1.hpp"

#include <exception>
#include <stdexcept>

namespace dh2::character_kill_quest_tail_v1 {
namespace {
// IDA: Character::Kill (0x3a5b18), post-credit branch at 0x3a5e50.
// The common RaiseAsync calls are 0x3a5ed0/0x3a5f44; template calls are
// conditional on the fresh +2533 read at 0x3a5f4c (raises 0x3a5fb8/0x3a6014).
struct SourceEvent {Kind kind;const char* constant;};
constexpr SourceEvent common_order[]={
    {Kind::kill_enemies,"KillXEnemies"},
    {Kind::clear_enemies,"ClearEnemies"}
};
constexpr SourceEvent template_order[]={
    {Kind::kill_enemy_template,"KillEnemyTemplate"},
    {Kind::clear_enemy_template,"ClearEnemyTemplate"}
};
bool get_word_25(const Bindings& b,const Services& s,std::uintptr_t& value,std::string& error){
    if(!s.character_word_25(s.context,b.character,value,error)){
        if(error.empty())error="Character::Kill word+25 read failed";
        return false;
    }
    return true;
}
bool get_halfword(const Bindings& b,const Services& s,std::uint32_t index,
                  std::int16_t& value,std::string& error){
    if(!s.character_halfword(s.context,b.character,index,value,error)){
        if(error.empty())error="Character::Kill halfword read failed";
        return false;
    }
    return true;
}
bool emit_pair(const SourceEvent* order,const Bindings& b,const Services& s,
               std::uintptr_t level,std::uintptr_t word25,std::int16_t subject,
               std::uint32_t halfword_index,Result& result,std::string& error){
    for(unsigned i=0;i<2;++i){
        const auto& selected=order[i];
        Event event{};event.kind=selected.kind;
        result.last_kind=event.kind;++result.events_attempted;
        if(!s.get_constant(s.context,"v2QuestObjectiveType",selected.constant,
                           event.objective_type,error)){
            if(error.empty())error="Character::Kill objective constant lookup failed";
            return false;
        }
        event.killer=b.killer;event.character_word_25=word25;
        if(halfword_index==2532)event.character_halfword_2532=subject;
        else event.character_halfword_2533=subject;
        event.source_subject=-1;event.source_word_24=subject;
        event.flag0=event.flag1=0;
        result.last_objective_type=event.objective_type;
        if(!s.raise_async(s.context,level,event,error)){
            if(error.empty())error="Character::Kill objective RaiseAsync failed";
            return false;
        }
        ++result.events_raised;
    }
    return true;
}
}

Runtime::Runtime(Bindings b,Services s):bindings_(b),services_(s){
    if(!b.character||b.virtual_84_true>1||b.suppress_byte_5348>1||
       !s.current_level||!s.character_word_25||!s.character_halfword||
       !s.get_constant||!s.raise_async)
        throw std::invalid_argument("Invalid Character::Kill quest-tail bindings");
}

Status Runtime::run(Result* output,std::string& error){
    if(busy_)return Status::busy;
    if(consumed_)return Status::consumed;
    if(!output)return Status::invalid_argument;
    busy_=true;consumed_=true;
    struct Reset {bool& busy;~Reset(){busy=false;}} reset{busy_};
    *output={};error.clear();

    if(bindings_.virtual_84_true||bindings_.suppress_byte_5348){
        output->skipped=1;
        return Status::complete;
    }
    std::uintptr_t level=0;
    try{
        if(!services_.current_level(services_.context,level,error)){
            if(error.empty())error="Character::Kill current-Level query failed";
            return Status::provider_failed;
        }
    }catch(const std::exception& ex){error=ex.what();return Status::provider_failed;}
     catch(...){error="Character::Kill current-Level provider threw";return Status::provider_failed;}
    if(!level){
        error="Character::Kill quest tail reached without the current Level";
        return Status::provider_failed;
    }

    try{
        // The first pair snapshots +25 and +2532 once. Source event handlers
        // may run before the second pair; Character::Kill then rereads +2533
        // and +25 before deciding/constructing its template events.
        std::uintptr_t word25=0;std::int16_t halfword=0;
        if(!get_word_25(bindings_,services_,word25,error)||
           !get_halfword(bindings_,services_,2532,halfword,error))return Status::provider_failed;
        if(!emit_pair(common_order,bindings_,services_,level,word25,halfword,2532,*output,error))
            return Status::provider_failed;
        if(!get_halfword(bindings_,services_,2533,halfword,error))return Status::provider_failed;
        if(halfword!=-1){
            if(!get_word_25(bindings_,services_,word25,error))return Status::provider_failed;
            if(!emit_pair(template_order,bindings_,services_,level,word25,halfword,2533,*output,error))
                return Status::provider_failed;
        }
    }catch(const std::exception& ex){
        error=ex.what();
        if(error.empty())error="Character::Kill quest-tail provider threw";
        return Status::provider_failed;
    }catch(...){
        error="Character::Kill quest-tail provider threw";
        return Status::provider_failed;
    }
    error.clear();
    return Status::complete;
}

} // namespace dh2::character_kill_quest_tail_v1
