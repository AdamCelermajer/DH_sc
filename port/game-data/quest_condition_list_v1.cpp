#include "quest_condition_list_v1.hpp"
#include <cstring>

namespace dh2::data::quest_condition_list_v1 {namespace {
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){
    const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
    return an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y||(x&&y&&an&&bn&&x<y+bn&&y<x+an);
}
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool output(const List& list,const Runtime* runtime,const Result* out,const Definition* input=nullptr){
    if(!aligned(out)||overlap(out,sizeof(*out),&list,sizeof(list))||overlap(out,sizeof(*out),runtime,sizeof(*runtime))||
       (input&&overlap(out,sizeof(*out),input,sizeof(*input))))return false;
    const auto* array=list.children_4;
    if(array){
        if(!aligned(array)||overlap(out,sizeof(*out),array,sizeof(*array))||overlap(out,sizeof(*out),array->slots.data(),array->slots.size()*sizeof(ConditionRef*)))return false;
        for(const auto* child:array->slots)if(child&&(!aligned(child)||overlap(out,sizeof(*out),child,sizeof(*child))||
            (child->fields&&overlap(out,sizeof(*out),child->fields,sizeof(*child->fields)))))return false;
    }
    return true;
}
struct Call {
    List& list;const Services& services;Result result{};
    bool fail(Status status,Operation op){result.status=status;result.last_operation=op;return false;}
    template<class F>bool send(Operation op,F fn){
        result.last_operation=op;++result.service_calls;
        try{if(fn())return fail(Status::service_failed,op);}
        catch(...){return fail(Status::service_failed,op);}
        return true;
    }
    bool slot(Array* array,std::uint32_t index,ConditionRef*& value,Operation op){
        if(!aligned(array)||!array->identity||index>=array->slots.size())return fail(Status::source_fault,op);
        value=array->slots[index];return true;
    }
    bool store(Array* array,std::uint32_t index,ConditionRef* value,Operation op){
        ConditionRef* ignored=nullptr;if(!slot(array,index,ignored,op))return false;
        array->slots[index]=value;return true;
    }
    bool assign(const Definition& input,std::int32_t count){
        // r5 captures the caller's source stub pointer even when providers later
        // change the list's published +8 projection.
        Definition captured=input;list.py_data_8=input;list.count_0=count;
        if(count<=0)return true;
        if(!services.allocate)return fail(Status::service_unavailable,Operation::allocate);
        const auto bytes=std::uint32_t(count)<<2;
        Array* array=nullptr;
        if(!send(Operation::allocate,[&]{return services.allocate(services.context,list,bytes,0,&array);}))return false;
        list.children_4=array;
        if(list.count_0<=0)return true;
        // Source allocator arithmetic is low32; an impossible native storage
        // extent stops at its reached prefix rather than inventing a retry.
        if(std::uint64_t(std::uint32_t(count))*4!=bytes)return fail(Status::unsafe_storage,Operation::allocate);
        for(std::uint32_t i=0;;++i){
            if(i)array=list.children_4;
            result.last_operation=Operation::read_definition;
            if(!captured.list||captured.view.resolve_list(reinterpret_cast<std::uintptr_t>(captured.list))!=captured.list||
               captured.list->kind!=0||captured.index>UINT32_MAX-i)return fail(Status::source_fault,Operation::read_definition);
            dh2_quest_span span{};quest_table_bindings_v1::Span bytes_view{};std::string error;
            if(!captured.view.list_record(*captured.list,captured.index+i,&span,error)||
               !captured.view.bytes(span,&bytes_view,error)||bytes_view.size!=12)return fail(Status::source_fault,Operation::read_definition);
            const auto* p=bytes_view.data;const auto word=std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);
            std::int32_t kind;std::memcpy(&kind,&word,4);
            if(kind<0||kind>6)return fail(Status::source_fault,Operation::read_definition);
            if(!services.factory)return fail(Status::service_unavailable,Operation::factory);
            ConditionRef* child=nullptr;
            if(!send(Operation::factory,[&]{return services.factory(services.context,list,kind,&child);}))return false;
            if(!store(array,i,child,Operation::publish))return false;
            ++result.published;
            // Source reloads +4 and its slot after factory publication. That
            // actual object can differ from the factory's returned object.
            if(!slot(list.children_4,i,child,Operation::bind_definition))return false;
            if(!aligned(child)||!child->identity||!aligned(child->fields)||
               overlap(child->fields,sizeof(*child->fields),&list,sizeof(list))||
               overlap(child->fields,sizeof(*child->fields),child,sizeof(*child))||
               overlap(child->fields,sizeof(*child->fields),array,sizeof(*array))||
               overlap(child->fields,sizeof(*child->fields),list.children_4,sizeof(*list.children_4)))return fail(Status::source_fault,Operation::bind_definition);
            child->fields->py_data_4={captured.view,captured.list,captured.index+i};
            if(list.count_0<=0||std::uint32_t(list.count_0)<=i+1)return true;
        }
    }
    bool destroy(){
        auto count=list.count_0;auto* array=list.children_4;
        if(count>0)for(std::uint32_t i=0;;++i){
            ConditionRef* child=nullptr;if(!slot(array,i,child,Operation::delete_virtual4))return false;
            if(child){
                if(!aligned(child)||!child->identity)return fail(Status::source_fault,Operation::delete_virtual4);
                if(!services.delete_virtual4)return fail(Status::service_unavailable,Operation::delete_virtual4);
                if(!send(Operation::delete_virtual4,[&]{return services.delete_virtual4(services.context,list,child);}))return false;
                if(!store(array,i,nullptr,Operation::delete_virtual4))return false;
                ++result.deleted;count=list.count_0;array=list.children_4;
            }
            // Source null slots do not reload count or array.
            if(count<=0||std::uint32_t(count)<=i+1)break;
        }
        if(array){
            if(!aligned(array)||!array->identity)return fail(Status::source_fault,Operation::deallocate);
            if(!services.deallocate)return fail(Status::service_unavailable,Operation::deallocate);
            if(!send(Operation::deallocate,[&]{return services.deallocate(services.context,list,array);}))return false;
            list.children_4=nullptr;
        }
        return true;
    }
    bool evaluate(){
        if(list.count_0<=0)return true;
        for(std::uint32_t i=0;;++i){
            ConditionRef* child=nullptr;if(!slot(list.children_4,i,child,Operation::eval_virtual8))return false;
            if(!aligned(child)||!child->identity)return fail(Status::source_fault,Operation::eval_virtual8);
            if(!services.eval_virtual8)return fail(Status::service_unavailable,Operation::eval_virtual8);
            std::uint32_t value=0;
            if(!send(Operation::eval_virtual8,[&]{return services.eval_virtual8(services.context,list,child,&value);}))return false;
            ++result.evaluated;
            if(!value){result.evaluation=0;return true;}
            if(list.count_0<=0||std::uint32_t(list.count_0)<=i+1)return true;
        }
    }
};
struct Guard{bool& busy;~Guard(){busy=false;}};
}
#define DH2_CONDITION_CALL(body) \
    if(!output(list_,this,out))return Status::invalid_argument; \
    if(busy_)return Status::reentrant; \
    busy_=true;Guard guard{busy_};Call call{list_,services_}; \
    if(body)call.result.last_operation=Operation::complete; \
    *out=call.result;return out->status
Status Runtime::construct(Result* out){DH2_CONDITION_CALL((list_.py_data_8=Definition{},list_.count_0=0,list_.children_4=nullptr,true));}
Status Runtime::assign_pydata(const Definition& input,std::int32_t count,Result* out){
    if(!output(list_,this,out,&input))return Status::invalid_argument;
    DH2_CONDITION_CALL(call.assign(input,count));
}
Status Runtime::destroy(Result* out){DH2_CONDITION_CALL(call.destroy());}
Status Runtime::evaluate(Result* out){DH2_CONDITION_CALL(call.evaluate());}
#undef DH2_CONDITION_CALL
}
