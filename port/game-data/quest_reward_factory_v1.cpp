#include "quest_reward_factory_v1.hpp"
namespace dh2::data::quest_reward_factory_v1 {namespace {
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){
 const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
 return an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y||(x&&y&&an&&bn&&x<y+bn&&y<x+an);
}
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool output(const Runtime* self,const Result* out,const Record* q=nullptr){return aligned(out)&&!overlap(out,sizeof(*out),self,sizeof(*self))&&(!q||!overlap(out,sizeof(*out),q,sizeof(*q)));}
bool coherent(const Record& q){return q.ref.identity&&q.ref.identity<=UINTPTR_MAX-28&&q.ref.fields==&q.fields;}
struct Call {
 const Services& services;Result result{};
 bool fail(Status s,Operation op){result.status=s;result.last_operation=op;return false;}
 template<class F>bool send(Operation op,F f){result.last_operation=op;++result.service_calls;try{if(f())return fail(Status::service_failed,op);}catch(...){return fail(Status::service_failed,op);}return true;}
 bool base(Record& q){
  q.dispatch_0=Dispatch::base;++result.field_stores;result.last_operation=Operation::base_construct;
  if(!services.get_constant)return fail(Status::service_unavailable,Operation::get_constant);
  std::int32_t value=0;
  if(!send(Operation::get_constant,[&]{return services.get_constant(services.context,q,"v2QuestRewardType","Invalid",&value);}))return false;
  q.fields.type_4=value;q.fields.character_10=0;q.compiled_8=0;q.fields.py_data_c={};result.field_stores+=4;
  return true;
 }
 bool create(std::int32_t type,const Runtime* self,Result* out){
  if(!services.allocate)return fail(Status::service_unavailable,Operation::allocate);
  result.logical_bytes=type==2?28:type==4?20:24;Record* q=nullptr;
  if(!send(Operation::allocate,[&]{return services.allocate(services.context,result.logical_bytes,0,&q);}))return false;
  result.record=q;
  if(!aligned(q))return fail(Status::source_fault,Operation::allocate);
  if(!output(self,out,q))return fail(Status::invalid_argument,Operation::allocate);
  if(!coherent(*q))return fail(Status::projection_changed,Operation::allocate);
  // All factories value-initialize the source footprint before base C2. Three
  // padding bytes after the byte +8 are never written by either constructor.
  if(type==2){q->property_owner_18=0;++result.field_stores;}
  if(type!=4){q->compiled_py_data_14={};++result.field_stores;}
  q->dispatch_0=Dispatch::null_vtable;q->fields.type_4=0;q->compiled_8=0;q->fields.py_data_c={};q->fields.character_10=0;result.field_stores+=5;
  result.last_operation=Operation::default_fields;
  if(!base(*q))return false;
  constexpr std::array<Dispatch,5> types{{Dispatch::gold,Dispatch::xp,Dispatch::character_props,Dispatch::loot,Dispatch::consume_loot}};
  q->dispatch_0=types[std::uint32_t(type)];++result.field_stores;result.last_operation=Operation::construct;
  return true;
 }
 bool destroy(Record& q,bool deleting){
  result.record=&q;result.last_operation=Operation::destruct;
  if(q.dispatch_0<Dispatch::base||q.dispatch_0>Dispatch::consume_loot)return fail(Status::source_fault,Operation::destruct);
  // Every derived D1/D0 publishes its own vtable then calls the empty base D2.
  if(q.dispatch_0!=Dispatch::base)++result.field_stores;
  if(!deleting)return true;
  if(!services.deallocate)return fail(Status::service_unavailable,Operation::deallocate);
  return send(Operation::deallocate,[&]{return services.deallocate(services.context,&q);});
 }
};
struct Guard{bool& busy;~Guard(){busy=false;}};
}
Status Runtime::create(std::int32_t type,Result* out){
 if(type<0||type>4||!output(this,out))return Status::invalid_argument;
 if(busy_)return Status::reentrant;
 busy_=true;Guard guard{busy_};Call call{services_};
 if(call.create(type,this,out))call.result.last_operation=Operation::complete;
 if(!output(this,out,call.result.record))return Status::invalid_argument;
 *out=call.result;return out->status;
}
Status Runtime::construct_base(Record& q,Result* out){
 if(!output(this,out,&q))return Status::invalid_argument;
 if(busy_)return Status::reentrant;
 busy_=true;Guard guard{busy_};Call call{services_};call.result.record=&q;
 if(!coherent(q))call.fail(Status::projection_changed,Operation::base_construct);
 else if(call.base(q))call.result.last_operation=Operation::complete;
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
