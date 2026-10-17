#include "quest_condition_factory_v1.hpp"
#include <array>

namespace dh2::data::quest_condition_factory_v1 {namespace {
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){
    const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
    return an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y||(x&&y&&an&&bn&&x<y+bn&&y<x+an);
}
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool output(const Runtime* runtime,const Result* out,const Record* q=nullptr){
    return aligned(out)&&!overlap(out,sizeof(*out),runtime,sizeof(*runtime))&&
           (!q||!overlap(out,sizeof(*out),q,sizeof(*q)));
}
bool coherent(const Record& q){return q.ref.identity&&q.ref.identity<=UINTPTR_MAX-12&&q.ref.fields==&q.fields;}
struct Call {
    const Services& services;Result result{};
    bool fail(Status s,Operation op){result.status=s;result.last_operation=op;return false;}
    void base(Record& q){q.fields.py_data_4={};q.dispatch_0=Dispatch::base;result.field_stores+=2;result.last_operation=Operation::base_construct;}
    bool create(std::int32_t type,const Runtime* runtime,Result* out){
        if(!services.allocate)return fail(Status::service_unavailable,Operation::allocate);
        result.last_operation=Operation::allocate;result.logical_bytes=type<3?12:8;++result.service_calls;
        Record* q=nullptr;
        try{if(services.allocate(services.context,result.logical_bytes,0,&q))return fail(Status::service_failed,Operation::allocate);}
        catch(...){return fail(Status::service_failed,Operation::allocate);}
        result.record=q;
        if(!aligned(q))return fail(Status::source_fault,Operation::allocate);
        if(!output(runtime,out,q))return fail(Status::invalid_argument,Operation::allocate);
        if(!coherent(*q))return fail(Status::projection_changed,Operation::allocate);
        if(type>=3){q->fields.py_data_4={};q->dispatch_0=Dispatch::null_vtable;result.field_stores+=2;}
        base(*q);
        constexpr std::array<Dispatch,7> dispatch{{Dispatch::quest_in_state,Dispatch::quest_state_lower,
            Dispatch::quest_state_higher,Dispatch::player_in_level,Dispatch::level_in_state,
            Dispatch::player_at_level,Dispatch::event_in_state}};
        if(type<3){q->comparator_8=type;++result.field_stores;}
        q->dispatch_0=dispatch[std::uint32_t(type)];++result.field_stores;result.last_operation=Operation::construct;
        return true;
    }
    bool destroy(Record& q,bool deleting){
        result.record=&q;result.last_operation=Operation::destruct;
        if(q.dispatch_0<Dispatch::base||q.dispatch_0>Dispatch::event_in_state)return fail(Status::source_fault,Operation::destruct);
        if(q.dispatch_0!=Dispatch::base){
            switch(q.dispatch_0){
                case Dispatch::quest_in_state:case Dispatch::quest_state_lower:case Dispatch::quest_state_higher:q.dispatch_0=Dispatch::generic;break;
                default:break;
            }
            // Each real derived D1/D0 writes its parent vtable, including when
            // that dispatch identity equals its already published identity.
            ++result.field_stores;
        }
        if(!deleting)return true;
        if(!services.deallocate)return fail(Status::service_unavailable,Operation::deallocate);
        result.last_operation=Operation::deallocate;++result.service_calls;
        try{if(services.deallocate(services.context,&q))return fail(Status::service_failed,Operation::deallocate);}
        catch(...){return fail(Status::service_failed,Operation::deallocate);}
        // Actual D0 does not access the freed object's fields after this call.
        return true;
    }
};
struct Guard{bool& busy;~Guard(){busy=false;}};
}
Status Runtime::create(std::int32_t type,Result* out){
    if(!output(this,out)||type<0||type>6)return Status::invalid_argument;
    if(busy_)return Status::reentrant;
    busy_=true;Guard guard{busy_};Call call{services_};
    if(call.create(type,this,out))call.result.last_operation=Operation::complete;
    // An allocator may reveal that the caller's output lies inside its actual
    // Record. Preserve that Record instead of publishing the rejection into it.
    if(!output(this,out,call.result.record))return Status::invalid_argument;
    *out=call.result;return out->status;
}
Status Runtime::construct_base(Record& q,Result* out){
    if(!output(this,out,&q))return Status::invalid_argument;
    if(busy_)return Status::reentrant;
    busy_=true;Guard guard{busy_};Call call{services_};call.result.record=&q;
    if(coherent(q)){call.base(q);call.result.last_operation=Operation::complete;}
    else call.fail(Status::projection_changed,Operation::base_construct);
    *out=call.result;return out->status;
}
Status Runtime::destroy(Record& q,bool deleting,Result* out){
    if(!output(this,out,&q))return Status::invalid_argument;
    if(busy_)return Status::reentrant;
    busy_=true;Guard guard{busy_};Call call{services_};
    if(!coherent(q))call.fail(Status::projection_changed,Operation::destruct);
    else if(call.destroy(q,deleting))call.result.last_operation=Operation::complete;
    *out=call.result;return out->status;
}
}
