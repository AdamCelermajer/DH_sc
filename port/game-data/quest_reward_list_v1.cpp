#include "quest_reward_list_v1.hpp"
#include <cstring>
namespace dh2::data::quest_reward_list_v1 {namespace {
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){
 const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
 return an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y||(x&&y&&an&&bn&&x<y+bn&&y<x+an);
}
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool output(const List& list,const Runtime* self,const Result* out,const Definition* input=nullptr){
 if(!aligned(out)||overlap(out,sizeof(*out),&list,sizeof(list))||overlap(out,sizeof(*out),self,sizeof(*self))||
    (input&&overlap(out,sizeof(*out),input,sizeof(*input)))||overlap(out,sizeof(*out),list.text_8.data(),list.text_8.size()+1))return false;
 const auto* a=list.children_4;
 if(a){if(!aligned(a)||overlap(out,sizeof(*out),a,sizeof(*a))||overlap(out,sizeof(*out),a->slots.data(),a->slots.size()*sizeof(RewardRef*)))return false;
  for(const auto* child:a->slots)if(child&&(!aligned(child)||overlap(out,sizeof(*out),child,sizeof(*child))||
     (child->fields&&overlap(out,sizeof(*out),child->fields,sizeof(*child->fields)))))return false;
 }
 return true;
}
struct Call {
 List& list;const Services& services;Result result{};
 bool fail(Status status,Operation op){result.status=status;result.last_operation=op;return false;}
 template<class F>bool send(Operation op,F fn){result.last_operation=op;++result.service_calls;try{if(fn())return fail(Status::service_failed,op);}catch(...){return fail(Status::service_failed,op);}return true;}
 bool slot(Array* array,std::uint32_t i,RewardRef*& value,Operation op){
  if(!aligned(array)||!array->identity||i>=array->slots.size())return fail(Status::source_fault,op);
  value=array->slots[i];return true;
 }
 bool store(Array* array,std::uint32_t i,RewardRef* value,Operation op){RewardRef* ignored=nullptr;if(!slot(array,i,ignored,op))return false;array->slots[i]=value;return true;}
 bool child(RewardRef* value,Operation op){
  return aligned(value)&&value->identity&&aligned(value->fields)&&
   !overlap(value->fields,sizeof(*value->fields),&list,sizeof(list))?
   true:fail(Status::source_fault,op);
 }
 bool read(const Definition& d,std::uint32_t i,std::int32_t& type){
  result.last_operation=Operation::read_definition;
  if(!d.list||d.list->kind<2||d.list->kind>4||d.index>UINT32_MAX-i)return fail(Status::source_fault,Operation::read_definition);
  dh2_quest_span span{};quest_table_bindings_v1::Span bytes{};std::string error;
  if(!d.view.list_record(*d.list,d.index+i,&span,error)||!d.view.bytes(span,&bytes,error)||bytes.size!=12)return fail(Status::source_fault,Operation::read_definition);
  const auto* p=bytes.data;const auto word=std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);
  std::memcpy(&type,&word,4);return true;
 }
 bool construct(){
  // Source publishes its two inline-string pointers before _M_allocate_block.
  // The valid native projection is already empty at that dependency boundary.
  list.text_8.clear();list.count_0=0;list.children_4=nullptr;
  if(!services.text_allocate16)return fail(Status::service_unavailable,Operation::text_allocate16);
  if(!send(Operation::text_allocate16,[&]{return services.text_allocate16(services.context,list);}))return false;
  list.py_data_20={};return true;
 }
 bool assign(const Definition& input,std::int32_t count){
  Definition captured=input;list.py_data_20=input;list.count_0=count;
  if(!services.text_clear)return fail(Status::service_unavailable,Operation::text_clear);
  if(!send(Operation::text_clear,[&]{return services.text_clear(services.context,list);}))return false;
  if(list.count_0<=0)return true;
  const auto bytes=std::uint32_t(list.count_0)<<2;const auto allocation_count=list.count_0;
  if(!services.allocate)return fail(Status::service_unavailable,Operation::allocate);
  Array* array=nullptr;
  if(!send(Operation::allocate,[&]{return services.allocate(services.context,list,bytes,0,&array);}))return false;
  list.children_4=array;
  if(list.count_0<=0)return true;
  if(std::uint64_t(std::uint32_t(allocation_count))*4!=bytes)return fail(Status::unsafe_storage,Operation::allocate);
  for(std::uint32_t i=0;;++i){
   if(i)array=list.children_4;
   std::int32_t type=0;if(!read(captured,i,type))return false;
   if(!services.factory)return fail(Status::service_unavailable,Operation::factory);
   RewardRef* reward=nullptr;
   if(!send(Operation::factory,[&]{return services.factory(services.context,list,type,&reward);}))return false;
   if(!store(array,i,reward,Operation::publish))return false;
   ++result.published;
   if(!slot(list.children_4,i,reward,Operation::bind_definition)||!child(reward,Operation::bind_definition))return false;
   reward->fields->py_data_c={captured.view,captured.list,captured.index+i};
   // The source loads the current +4 array before re-reading the stub type,
   // then reloads its slot. Native immutable definitions have no read callback.
   auto* current=list.children_4;
   if(!read(captured,i,type)||!slot(current,i,reward,Operation::bind_type)||!child(reward,Operation::bind_type))return false;
   reward->fields->type_4=type;
   if(list.count_0<=0||std::uint32_t(list.count_0)<=i+1)return true;
  }
 }
 bool owner(std::uintptr_t character){
  if(list.count_0<=0)return true;
  for(std::uint32_t i=0;;++i){RewardRef* reward=nullptr;
   if(!slot(list.children_4,i,reward,Operation::set_owner)||!child(reward,Operation::set_owner))return false;
   reward->fields->character_10=character;++result.owner_stores;
   if(list.count_0<=0||std::uint32_t(list.count_0)<=i+1)return true;
  }
 }
 bool destroy(){
  auto count=list.count_0;auto* array=list.children_4;
  if(count>0)for(std::uint32_t i=0;;++i){RewardRef* reward=nullptr;
   if(!slot(array,i,reward,Operation::delete_virtual4))return false;
   if(reward){
    if(!aligned(reward)||!reward->identity)return fail(Status::source_fault,Operation::delete_virtual4);
    if(!services.delete_virtual4)return fail(Status::service_unavailable,Operation::delete_virtual4);
    if(!send(Operation::delete_virtual4,[&]{return services.delete_virtual4(services.context,list,reward);}))return false;
    if(!store(array,i,nullptr,Operation::delete_virtual4))return false;
    ++result.deleted;
    count=list.count_0;array=list.children_4;
   }
   if(count<=0||std::uint32_t(count)<=i+1)break;
  }
  if(array){
   if(!aligned(array)||!array->identity)return fail(Status::source_fault,Operation::deallocate);
   if(!services.deallocate)return fail(Status::service_unavailable,Operation::deallocate);
   if(!send(Operation::deallocate,[&]{return services.deallocate(services.context,list,array);}))return false;
   list.children_4=nullptr;
  }
  if(!services.text_destroy)return fail(Status::service_unavailable,Operation::text_destroy);
  return send(Operation::text_destroy,[&]{return services.text_destroy(services.context,list);});
 }
};
struct Guard{bool& busy;~Guard(){busy=false;}};
}
#define DH2_REWARD_CALL(body) \
 if(!output(list_,this,out))return Status::invalid_argument; \
 if(busy_)return Status::reentrant; \
 busy_=true;Guard guard{busy_};Call call{list_,services_}; \
 if(body)call.result.last_operation=Operation::complete; \
 *out=call.result;return out->status
Status Runtime::construct(Result* out){DH2_REWARD_CALL(call.construct());}
Status Runtime::assign_pydata(const Definition& input,std::int32_t count,Result* out){
 if(!output(list_,this,out,&input))return Status::invalid_argument;
 DH2_REWARD_CALL(call.assign(input,count));
}
Status Runtime::set_owner(std::uintptr_t character,Result* out){DH2_REWARD_CALL(call.owner(character));}
Status Runtime::destroy(Result* out){DH2_REWARD_CALL(call.destroy());}
#undef DH2_REWARD_CALL
}
