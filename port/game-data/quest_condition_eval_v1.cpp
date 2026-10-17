#include "quest_condition_eval_v1.hpp"
#include <cstring>

namespace dh2::data::quest_condition_eval_v1 {namespace {
using Definition=quest_condition_list_v1::Definition;
using Dispatch=quest_condition_factory_v1::Dispatch;
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){
    const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
    return an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y||(x&&y&&an&&bn&&x<y+bn&&y<x+an);
}
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
struct Call {
    Record& record;const Services& services;Result* output;Result result{};
    std::uintptr_t identity=record.ref.identity;bool publish=true;
    bool fail(Status status,Operation op){result.status=status;result.last_operation=op;return false;}
    bool coherent(){return record.ref.identity==identity&&identity&&record.ref.fields==&record.fields?true:fail(Status::projection_changed,result.last_operation);}
    template<class F>bool send(Operation op,F callback){
        result.last_operation=op;++result.service_calls;
        try{if(callback())return fail(Status::service_failed,op);}
        catch(...){return fail(Status::service_failed,op);}
        return coherent();
    }
    template<class T>bool borrow(const T* control,const void* backing,std::size_t bytes,Operation op){
        if(!aligned(control)||!control->identity||!backing)return fail(Status::source_fault,op);
        if(overlap(output,sizeof(*output),control,sizeof(*control))||overlap(output,sizeof(*output),backing,bytes)){
            publish=false;return fail(Status::invalid_argument,op);
        }
        return true;
    }
    bool word(const Definition& definition,unsigned word,std::int32_t& value,Operation op){
        result.last_operation=op;const auto* list=definition.list;
        if(!list||definition.view.resolve_list(reinterpret_cast<std::uintptr_t>(list))!=list)return fail(Status::source_fault,op);
        if(overlap(output,sizeof(*output),list,sizeof(*list))||
           overlap(output,sizeof(*output),list->row,sizeof(*list->row))||
           overlap(output,sizeof(*output),list->definition,sizeof(*list->definition))){
            publish=false;return fail(Status::invalid_argument,op);
        }
        if(list->kind!=0)return fail(Status::source_fault,op);
        dh2_quest_span span{};quest_table_bindings_v1::Span bytes{};std::string error;
        if(!definition.view.list_record(*list,definition.index,&span,error)||!definition.view.bytes(span,&bytes,error)||bytes.size!=12)return fail(Status::source_fault,op);
        if(overlap(output,sizeof(*output),bytes.data,bytes.size)){publish=false;return fail(Status::invalid_argument,op);}
        const auto* p=bytes.data+word*4;const auto raw=std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);
        std::memcpy(&value,&raw,4);return true;
    }
    bool app(std::uintptr_t& application){
        if(!services.application)return fail(Status::service_unavailable,Operation::application);
        if(!send(Operation::application,[&]{return services.application(services.context,&application);}))return false;
        return application?true:fail(Status::source_fault,Operation::application);
    }
    bool generic(){
        std::uintptr_t application=0,manager=0;if(!app(application))return false;
        if(!services.player_manager)return fail(Status::service_unavailable,Operation::player_manager);
        if(!send(Operation::player_manager,[&]{return services.player_manager(services.context,application,&manager);}))return false;
        if(!manager)return fail(Status::source_fault,Operation::player_manager);
        // The original captures Condition+4 after Application+40 and before
        // GetLocalPlayer; later definition replacement cannot change r5.
        const Definition definition=record.fields.py_data_4;
        if(!services.local_player)return fail(Status::service_unavailable,Operation::local_player);
        PlayerRef* player=nullptr;
        if(!send(Operation::local_player,[&]{return services.local_player(services.context,manager,0,1,&player);}))return false;
        result.last_operation=Operation::character_660;
        if(!aligned(player)||!aligned(player->character_660)||!borrow(player,player->character_660,sizeof(*player->character_660),Operation::character_660))return fail(result.status==Status::complete?Status::source_fault:result.status,Operation::character_660);
        const auto character=*player->character_660;if(!character)return true;
        std::int32_t quest_id=0;if(!word(definition,1,quest_id,Operation::definition_id))return false;
        if(!services.quest_by_id)return fail(Status::service_unavailable,Operation::quest_by_id);
        QuestStateRef* quest=nullptr;
        if(!send(Operation::quest_by_id,[&]{return services.quest_by_id(services.context,character,quest_id,-1,&quest);}))return false;
        if(!quest)return true;
        result.last_operation=Operation::quest_state;
        if(!aligned(quest)||!aligned(quest->state_0)||!borrow(quest,quest->state_0,sizeof(*quest->state_0),Operation::quest_state))return fail(result.status==Status::complete?Status::source_fault:result.status,Operation::quest_state);
        const auto comparator=record.comparator_8,state=*quest->state_0;
        if(comparator<0||comparator>2)return true;
        std::int32_t required=0;if(!word(definition,2,required,Operation::definition_state))return false;
        result.value=compare_quest_state(comparator,state,required);return true;
    }
    bool level(){
        const Definition definition=record.fields.py_data_4;
        std::uintptr_t application=0;if(!app(application))return false;
        if(!services.current_level)return fail(Status::service_unavailable,Operation::current_level);
        LevelRef* level=nullptr;
        if(!send(Operation::current_level,[&]{return services.current_level(services.context,application,&level);}))return false;
        std::int32_t required=0;if(!word(definition,1,required,Operation::definition_id))return false;
        result.last_operation=Operation::level_index;
        if(!aligned(level)||!aligned(level->level_list_index_3c)||!borrow(level,level->level_list_index_3c,sizeof(*level->level_list_index_3c),Operation::level_index))return fail(result.status==Status::complete?Status::source_fault:result.status,Operation::level_index);
        result.value=required==*level->level_list_index_3c;return true;
    }
};
struct Guard{bool& busy;~Guard(){busy=false;}};
}
Status Runtime::evaluate(Result* out){
    if(!aligned(out)||overlap(out,sizeof(*out),&record_,sizeof(record_))||overlap(out,sizeof(*out),this,sizeof(*this)))return Status::invalid_argument;
    if(busy_)return Status::reentrant;
    const auto dispatch=record_.dispatch_0;
    const bool generic=dispatch==Dispatch::generic||dispatch==Dispatch::quest_in_state||dispatch==Dispatch::quest_state_lower||dispatch==Dispatch::quest_state_higher;
    if(!generic&&dispatch!=Dispatch::player_in_level)return Status::outside_domain;
    busy_=true;Guard guard{busy_};Call call{record_,services_,out};
    if(call.coherent()&&(generic?call.generic():call.level()))call.result.last_operation=Operation::complete;
    if(call.publish)*out=call.result;
    return call.result.status;
}
}
