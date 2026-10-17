#include "quest_compile_v1.hpp"
#include <cstring>

namespace dh2::data::quest_compile_v1 {namespace {
namespace o=quest_objective_list_v1;namespace r=quest_reward_list_v1;
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y||(x&&y&&an&&bn&&x<y+bn&&y<x+an);}
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
std::int32_t signed32(std::uint32_t n){std::int32_t value;std::memcpy(&value,&n,4);return value;}
struct Guard{bool& busy;~Guard(){busy=false;}};
struct Call {
    quest_instance_v1::Instance& instance;const quest_table_bindings_v1::View& definitions;const Services& services;const Result* output;
    Result result{};bool output_safe=true;
    bool fail(Status status,Operation op){result.status=status;result.last_operation=op;return false;}
    bool separate(const void* p,std::size_t size){if(overlap(output,sizeof(*output),p,size)){output_safe=false;return fail(Status::invalid_argument,result.last_operation);}return true;}
    template<class F>bool send(Operation op,F f){result.last_operation=op;++result.service_calls;try{if(f())return fail(Status::service_failed,op);}catch(...){return fail(Status::service_failed,op);}return true;}
    bool objective(ActionRef* ref,Objective*& object){
        result.last_operation=Operation::resolve_objective;
        if(!aligned(ref)||!ref->identity)return fail(Status::source_fault,result.last_operation);
        if(!separate(ref,sizeof(*ref)))return false;
        if(!services.resolve_objective)return fail(Status::service_unavailable,result.last_operation);
        try{object=services.resolve_objective(services.context,ref);}catch(...){return fail(Status::service_failed,result.last_operation);}
        if(!aligned(object))return fail(Status::source_fault,result.last_operation);
        if(!separate(object,sizeof(*object)))return false;
        if(&object->ref.action!=ref||object->ref.fields!=&object->fields||ref->character_10!=&object->fields.character_10)return fail(Status::projection_changed,result.last_operation);
        return true;
    }
    bool reward(r::RewardRef* ref,Reward*& object){
        result.last_operation=Operation::resolve_reward;
        if(!aligned(ref)||!ref->identity)return fail(Status::source_fault,result.last_operation);
        if(!separate(ref,sizeof(*ref)))return false;
        if(!services.resolve_reward)return fail(Status::service_unavailable,result.last_operation);
        try{object=services.resolve_reward(services.context,ref);}catch(...){return fail(Status::service_failed,result.last_operation);}
        if(!aligned(object))return fail(Status::source_fault,result.last_operation);
        if(!separate(object,sizeof(*object)))return false;
        if(&object->ref!=ref||ref->fields!=&object->fields)return fail(Status::projection_changed,result.last_operation);
        return true;
    }
    bool invalidate(ActionRef* ref){Objective* q=nullptr;if(!objective(ref,q))return false;result.last_operation=Operation::invalidate_objective;q->done_14=0;q->compiled_8=0;result.field_stores+=2;return true;}
    bool member(ActionRef* ref,const MemberCall& call){
        Objective* q=nullptr;if(!objective(ref,q))return false;
        if(!call.is_virtual&&call.adjustment==0&&call.function==0x479f70){result.last_operation=Operation::invalidate_objective;q->done_14=0;q->compiled_8=0;result.field_stores+=2;return true;}
        if(!services.objective_member)return fail(Status::service_unavailable,Operation::objective_member);
        return send(Operation::objective_member,[&]{return services.objective_member(services.context,*q,call);});
    }
    template<class Array,class Ref>bool slot(Array* backing,std::uint32_t index,Ref*& ref){
        if(!aligned(backing))return fail(Status::source_fault,result.last_operation);
        if(!separate(backing,sizeof(*backing))||!separate(backing->slots.data(),backing->slots.size()*sizeof(backing->slots[0])))return false;
        if(index>=backing->slots.size())return fail(Status::unsafe_storage,result.last_operation);
        ref=backing->slots[index];if(!aligned(ref))return fail(Status::source_fault,result.last_operation);return true;
    }
    bool loop(const MemberCall& call){
        auto& list=instance.objectives();if(list.count_0<=0)return true;
        std::uint32_t index=0;
        do{result.last_operation=Operation::objective_member;o::ObjectiveRef* ref=nullptr;if(!slot(list.children_4,index,ref))return false;++index;
            if(!member(&ref->action,call))return false;
            ++result.visited;
        }while(list.count_0>std::int64_t(index));
        return true;
    }
    bool markers(std::int32_t priority,std::int32_t state){return loop({0x10,0,true,priority,state});}
    bool invalidate_rewards(){
        auto& list=instance.rewards();result.last_operation=Operation::clear_reward_text;
        if(!separate(list.text_8.data(),list.text_8.size()+1))return false;
        list.text_8.clear();++result.field_stores;
        if(list.count_0<=0)return true;
        std::uint32_t index=0;
        do{r::RewardRef* ref=nullptr;if(!slot(list.children_4,index,ref))return false;++index;Reward* q=nullptr;if(!reward(ref,q))return false;
            result.last_operation=Operation::invalidate_reward;q->compiled_8=0;++result.field_stores;
        }while(list.count_0>std::int64_t(index));
        return true;
    }
    bool compile_rewards(){
        auto& list=instance.rewards();if(list.count_0<=0)return true;
        std::uint32_t index=0;
        do{result.last_operation=Operation::compile_reward;r::RewardRef* ref=nullptr;if(!slot(list.children_4,index,ref))return false;++index;Reward* q=nullptr;if(!reward(ref,q))return false;
            quest_reward_execution_v1::Runtime runtime(*q,{});quest_reward_execution_v1::Result receipt;
            result.last_operation=Operation::compile_reward;
            const auto status=runtime.compile(&receipt);
            result.field_stores+=receipt.field_stores;
            if(status!=quest_reward_factory_v1::Status::complete)return fail(status==quest_reward_factory_v1::Status::source_fault?Status::source_fault:Status::service_failed,Operation::compile_reward);
            ++result.visited;
        }while(list.count_0>std::int64_t(index));
        return true;
    }
    bool priority(const quest_runtime_fields_v1::PyDataRef* row,std::int32_t& value){
        result.last_operation=Operation::read_priority;
        if(!aligned(row))return fail(Status::source_fault,result.last_operation);
        if(!separate(row,sizeof(*row)))return false;
        const auto* actual=definitions.record(*row);
        if(!actual)return fail(Status::source_fault,result.last_operation);
        if(!separate(actual,sizeof(*actual)))return false;
        value=actual->priority;return true;
    }
    bool compile(){
        auto& q=instance.record();
        if(!invalidate(q.action_18)||!loop({0x479f70,0,false,0,0})||!invalidate(q.action_1c)||!invalidate_rewards())return false;
        if(!member(q.action_18,{8,0,true,0,0})||!loop({8,0,true,0,0})||!member(q.action_1c,{8,0,true,0,0}))return false;
        if(q.byte_5c&&!compile_rewards())return false;
        const auto state=q.state_0;
        if(state==6){
            if(!loop({0x18,0,true,0,0}))return false;
            const auto* row=q.py_data_68;const auto current=q.state_0;std::int32_t p=0;
            return priority(row,p)&&markers(p,current);
        }
        if(state==3||state==9){
            if(!member(state==3?q.action_18:q.action_1c,{0x18,0,true,0,0}))return false;
            auto* action=state==3?q.action_18:q.action_1c;const auto* row=q.py_data_68;const auto current=q.state_0;std::int32_t p=0;
            return priority(row,p)&&member(action,{0x10,0,true,p,current});
        }
        return true;
    }
};
bool output(const Runtime* runtime,quest_instance_v1::Instance& instance,const Result* out){
    if(!aligned(out)||overlap(out,sizeof(*out),runtime,sizeof(*runtime))||overlap(out,sizeof(*out),&instance,sizeof(instance)))return false;
    const auto array=[&](const auto* value){return !value||(!overlap(out,sizeof(*out),value,sizeof(*value))&&!overlap(out,sizeof(*out),value->slots.data(),value->slots.size()*sizeof(value->slots[0])));};
    return array(instance.conditions().children_4)&&array(instance.objectives().children_4)&&array(instance.rewards().children_4)&&!overlap(out,sizeof(*out),instance.rewards().text_8.data(),instance.rewards().text_8.size()+1);
}
}
#define DH2_COMPILE_CALL(body) \
    if(!output(this,instance_,out))return Status::invalid_argument; \
    if(busy_)return Status::reentrant; \
    busy_=true;Guard guard{busy_};Call call{instance_,definitions_,services_,out}; \
    if(body)call.result.last_operation=Operation::complete; \
    if(!call.output_safe)return Status::invalid_argument; \
    *out=call.result;return out->status
Status Runtime::compile(Result* out){DH2_COMPILE_CALL(call.compile());}
Status Runtime::invalidate_objectives(Result* out){DH2_COMPILE_CALL(call.loop({0x479f70,0,false,0,0}));}
Status Runtime::compile_objectives(Result* out){DH2_COMPILE_CALL(call.loop({8,0,true,0,0}));}
Status Runtime::register_objectives(Result* out){DH2_COMPILE_CALL(call.loop({0x18,0,true,0,0}));}
Status Runtime::install_markers(std::int32_t p,std::int32_t s,Result* out){DH2_COMPILE_CALL(call.markers(p,s));}
Status Runtime::invalidate_rewards(Result* out){DH2_COMPILE_CALL(call.invalidate_rewards());}
Status Runtime::compile_rewards(Result* out){DH2_COMPILE_CALL(call.compile_rewards());}
Status Runtime::loop_objectives(std::uint32_t function,std::int32_t encoded,Result* out){const auto adjustment=signed32((std::uint32_t(encoded)>>1)|(std::uint32_t(encoded)&0x80000000));DH2_COMPILE_CALL(call.loop({function,adjustment,bool(encoded&1),0,0}));}
#undef DH2_COMPILE_CALL
Status LogRuntime::compile_quests(bool force,Result* out){
    if(!aligned(out)||overlap(out,sizeof(*out),this,sizeof(*this))||overlap(out,sizeof(*out),&log_,sizeof(log_)))return Status::invalid_argument;
    for(const auto& vector:log_.quests)if(overlap(out,sizeof(*out),vector.data(),vector.size()*sizeof(vector[0])))return Status::invalid_argument;
    Result result{};
    bool safe=true;
    const auto fail=[&](Status status,Operation op){result.status=status;result.last_operation=op;return false;};
    const auto difficulty=[&](std::int32_t& value){
        result.last_operation=Operation::difficulty;
        if(!services_.difficulty)return fail(Status::service_unavailable,Operation::difficulty);
        ++result.service_calls;
        try{if(services_.difficulty(services_.context,log_.character_5c,&value))return fail(Status::service_failed,Operation::difficulty);}catch(...){return fail(Status::service_failed,Operation::difficulty);}
        return value>=0&&value<3?true:fail(Status::source_fault,Operation::difficulty);
    };
    // Source deliberately marks byte28 before compiling children. A reached
    // script/condition may ask SG_GetQuestByID, whose CompileQuests(false)
    // continuation must run this same fresh-difficulty guard and then succeed.
    // Do not reject that legitimate already-marked recursion as native busy.
    if(busy_){
        std::int32_t d=0;
        if(force)fail(Status::reentrant,Operation::none);
        else if(difficulty(d)){
            if(log_.byte_28[d])result.last_operation=Operation::complete;
            else fail(Status::reentrant,Operation::difficulty);
        }
        *out=result;return result.status;
    }
    busy_=true;Guard guard{busy_};
    const auto body=[&](){
        std::int32_t d=0;if(!force){if(!difficulty(d))return false;if(log_.byte_28[d])return true;}
        if(!difficulty(d))return false;
        result.last_operation=Operation::mark_compiled;log_.byte_28[d]=1;++result.field_stores;
        if(!difficulty(d))return false;
        const auto count=log_.quests[d].size();if(count>std::size_t(INT32_MAX))return fail(Status::unsafe_storage,Operation::resolve_quest);
        if(!count)return true;
        for(std::size_t index=0;index<count;++index){
            if(!difficulty(d))return false;
            auto& vector=log_.quests[d];result.last_operation=Operation::resolve_quest;
            if(overlap(out,sizeof(*out),vector.data(),vector.size()*sizeof(vector[0]))){safe=false;return fail(Status::invalid_argument,Operation::resolve_quest);}
            if(index>=vector.size())return fail(Status::unsafe_storage,Operation::resolve_quest);
            auto* ref=vector[index];if(!aligned(ref)||!ref->identity)return fail(Status::source_fault,Operation::resolve_quest);
            if(overlap(out,sizeof(*out),ref,sizeof(*ref))||(ref->fields&&overlap(out,sizeof(*out),ref->fields,sizeof(*ref->fields)))){safe=false;return fail(Status::invalid_argument,Operation::resolve_quest);}
            if(!services_.resolve_quest)return fail(Status::service_unavailable,Operation::resolve_quest);
            Runtime* runtime=nullptr;try{runtime=services_.resolve_quest(services_.context,ref);}catch(...){return fail(Status::service_failed,Operation::resolve_quest);}
            if(!aligned(runtime))return fail(Status::source_fault,Operation::resolve_quest);
            if(overlap(out,sizeof(*out),runtime,sizeof(*runtime))||overlap(out,sizeof(*out),&runtime->instance(),sizeof(runtime->instance()))){safe=false;return fail(Status::invalid_argument,Operation::resolve_quest);}
            if(&runtime->instance().record().ref!=ref)return fail(Status::projection_changed,Operation::resolve_quest);
            result.last_operation=Operation::compile_quest;Result child;
            const auto status=runtime->compile(&child);result.field_stores+=child.field_stores;result.service_calls+=child.service_calls;
            if(status!=Status::complete)return fail(status,Operation::compile_quest);
            ++result.visited;
        }
        return true;
    };
    if(body())result.last_operation=Operation::complete;
    if(!safe)return Status::invalid_argument;
    *out=result;return result.status;
}
}
