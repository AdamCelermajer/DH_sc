#include "quest_objective_list_v1.hpp"
#include <cstring>
namespace dh2::data::quest_objective_list_v1 {namespace {
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y||(x&&y&&an&&bn&&x<y+bn&&y<x+an);}
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool output_range(const List& list,const Runtime* runtime,const void* p,std::size_t n,const Definition* definition=nullptr){
 if(overlap(p,n,&list,sizeof(list))||overlap(p,n,runtime,sizeof(*runtime))||(definition&&overlap(p,n,definition,sizeof(*definition))))return false;
 const auto* array=list.children_4;
 if(array){if(!aligned(array)||overlap(p,n,array,sizeof(*array))||overlap(p,n,array->slots.data(),array->slots.size()*sizeof(ObjectiveRef*)))return false;for(const auto* child:array->slots)if(child&&(!aligned(child)||overlap(p,n,child,sizeof(*child))||(child->fields&&overlap(p,n,child->fields,sizeof(*child->fields)))))return false;}
 return true;
}
bool output(const List& list,const Runtime* runtime,const Result* out,const Definition* input=nullptr){return aligned(out)&&output_range(list,runtime,out,sizeof(*out),input);}
struct Call {
 List& list;const Services& services;Result result{};
 bool fail(Status s,Operation op){result.status=s;result.last_operation=op;return false;}
 template<class F>bool send(Operation op,F fn){result.last_operation=op;++result.service_calls;try{if(fn())return fail(Status::service_failed,op);}catch(...){return fail(Status::service_failed,op);}return true;}
 bool slot(Array* array,std::uint32_t i,ObjectiveRef*& child,Operation op){if(!aligned(array)||!array->identity||i>=array->slots.size())return fail(Status::source_fault,op);child=array->slots[i];return true;}
 bool store(Array* array,std::uint32_t i,ObjectiveRef* child,Operation op){ObjectiveRef* old=nullptr;if(!slot(array,i,old,op))return false;array->slots[i]=child;return true;}
 bool coherent(ObjectiveRef* child,Operation op){if(!aligned(child)||!child->action.identity||!aligned(child->fields)||child->action.character_10!=&child->fields->character_10||overlap(child->fields,sizeof(*child->fields),&list,sizeof(list)))return fail(Status::source_fault,op);return true;}
 bool make(const Definition& input,ObjectiveRef*& child){
  result.last_operation=Operation::read_definition;Definition definition=input;dh2_quest_objective value{};std::string error;
  if(definition.event){if(definition.stub||definition.list||definition.index>=definition.event->triggers.size())return fail(Status::source_fault,Operation::read_definition);value.common[0]=definition.event->triggers[definition.index].type;}
  else if(definition.stub){if(definition.list||!definition.view||!definition.stub->row||definition.view.resolve_stub(definition.stub->row->identity+definition.stub->offset)!=definition.stub||!definition.stub->definition)return fail(Status::source_fault,Operation::read_definition);value=*definition.stub->definition;}
  else if(!definition.list||definition.list->kind!=1||!definition.view.objective(*definition.list,definition.index,&value,error))return fail(Status::source_fault,Operation::read_definition);
  // Source stub +0 is its vtable; decoded common[0] is the +4 selector.
  const auto kind=value.common[0];
  if(kind<0||kind>=13)return fail(Status::source_fault,Operation::factory);
  if(!services.factory)return fail(Status::service_unavailable,Operation::factory);
  if(!send(Operation::factory,[&]{return services.factory(services.context,list,definition,kind,&child);}))return false;
  if(!coherent(child,Operation::bind_definition))return false;
  child->fields->py_data_c=definition;child->fields->type_4=kind;return true;
 }
 bool assign(const Definition& input,std::int32_t count){
  Definition captured=input;list.py_data_8=input;list.count_0=count;
  if(count<=0)return true;
  if(!services.allocate)return fail(Status::service_unavailable,Operation::allocate);
  const auto bytes=std::uint32_t(count)<<2;Array* array=nullptr;
  if(!send(Operation::allocate,[&]{return services.allocate(services.context,list,bytes,0,&array);}))return false;
  list.children_4=array;
  if(list.count_0<=0)return true;
  if(std::uint64_t(std::uint32_t(count))*4!=bytes)return fail(Status::unsafe_storage,Operation::allocate);
  for(std::uint32_t i=0;;++i){
   if(i)array=list.children_4;
   if(captured.index>UINT32_MAX-i)return fail(Status::source_fault,Operation::read_definition);
   Definition current=captured;current.index+=i;ObjectiveRef* child=nullptr;
   if(!make(current,child)||!store(array,i,child,Operation::publish))return false;
   ++result.published;
   if(list.count_0<=0||std::uint32_t(list.count_0)<=i+1)return true;
  }
 }
 bool owner(std::uintptr_t character){if(list.count_0<=0)return true;for(std::uint32_t i=0;;++i){ObjectiveRef* child=nullptr;if(!slot(list.children_4,i,child,Operation::set_owner)||!coherent(child,Operation::set_owner))return false;child->fields->character_10=character;++result.owner_stores;if(list.count_0<=0||std::uint32_t(list.count_0)<=i+1)return true;}}
 bool stream(std::uint32_t function,std::int32_t encoded,player_saved_quests_v1::StreamRef& source){
  if(list.count_0<=0)return true;
  const auto adjustment=encoded>=0?std::int64_t(encoded)/2:-((-std::int64_t(encoded)+1)/2);
  for(std::uint32_t i=0;;++i){
   ObjectiveRef* child=nullptr;if(!slot(list.children_4,i,child,Operation::stream_member)||!coherent(child,Operation::stream_member))return false;
   const auto address=child->action.identity;
   if((adjustment>=0&&std::uint64_t(adjustment)>UINTPTR_MAX-address)||(adjustment<0&&std::uint64_t(-adjustment)>address))return fail(Status::unsafe_storage,Operation::stream_member);
   if(!services.stream_member)return fail(Status::service_unavailable,Operation::stream_member);
   StreamCall call{child,&source,adjustment>=0?address+std::uintptr_t(adjustment):address-std::uintptr_t(-adjustment),function,encoded,(std::uint32_t(encoded)&1u)!=0};
   if(!send(Operation::stream_member,[&]{return services.stream_member(services.context,list,call);}))return false;
   ++result.loaded;if(list.count_0<=0||std::uint32_t(list.count_0)<=i+1)return true;
  }
 }
 bool members(MemberOperation operation,std::uint32_t function,std::int32_t encoded,std::string& error){
  if(list.count_0<=0)return true;
  const auto adjustment=encoded>=0?std::int64_t(encoded)/2:-((-std::int64_t(encoded)+1)/2);
  for(std::uint32_t i=0;;++i){
   ObjectiveRef* child=nullptr;if(!slot(list.children_4,i,child,Operation::member_call)||!coherent(child,Operation::member_call))return false;
   const auto address=child->action.identity;
   if((adjustment>=0&&std::uint64_t(adjustment)>UINTPTR_MAX-address)||(adjustment<0&&std::uint64_t(-adjustment)>address))return fail(Status::unsafe_storage,Operation::member_call);
   if(!services.member)return fail(Status::service_unavailable,Operation::member_call);
   MemberCall call{child,operation,adjustment>=0?address+std::uintptr_t(adjustment):address-std::uintptr_t(-adjustment),function,encoded,(std::uint32_t(encoded)&1u)!=0,&error};
   if(!send(Operation::member_call,[&]{return services.member(services.member_context?services.member_context:services.context,list,call);})){
    if(error.empty())error="ObjectiveList member provider failed";
    return false;
   }
   ++result.member_calls;if(list.count_0<=0||std::uint32_t(list.count_0)<=i+1)return true;
  }
 }
 bool destroy(){
  auto count=list.count_0;auto* array=list.children_4;
  if(count>0)for(std::uint32_t i=0;;++i){
   ObjectiveRef* child=nullptr;if(!slot(array,i,child,Operation::delete_virtual4))return false;
   if(child){if(!coherent(child,Operation::delete_virtual4))return false;if(!services.delete_virtual4)return fail(Status::service_unavailable,Operation::delete_virtual4);if(!send(Operation::delete_virtual4,[&]{return services.delete_virtual4(services.context,list,child);}))return false;if(!store(array,i,nullptr,Operation::delete_virtual4))return false;++result.deleted;count=list.count_0;array=list.children_4;}
   if(count<=0||std::uint32_t(count)<=i+1)break;
  }
  if(array){if(!aligned(array)||!array->identity)return fail(Status::source_fault,Operation::deallocate);if(!services.deallocate)return fail(Status::service_unavailable,Operation::deallocate);if(!send(Operation::deallocate,[&]{return services.deallocate(services.context,list,array);}))return false;list.children_4=nullptr;}
  return true;
 }
};
struct Guard{bool& busy;~Guard(){busy=false;}};
}
#define DH2_OBJECTIVE_CALL(body) \
 if(!output(list_,this,out))return Status::invalid_argument; \
 if(busy_)return Status::reentrant; \
 busy_=true;Guard guard{busy_};Call call{list_,services_}; \
 if(body)call.result.last_operation=Operation::complete; \
 *out=call.result;return out->status
Status Runtime::construct(Result* out){DH2_OBJECTIVE_CALL((list_.py_data_8=Definition{},list_.count_0=0,list_.children_4=nullptr,true));}
Status Runtime::assign_pydata(const Definition& input,std::int32_t count,Result* out){if(!output(list_,this,out,&input))return Status::invalid_argument;DH2_OBJECTIVE_CALL(call.assign(input,count));}
Status Runtime::create_objective(const Definition& input,ObjectiveRef** created,Result* out){if(!aligned(created)||!output(list_,this,out,&input)||!output_range(list_,this,created,sizeof(*created),&input)||overlap(created,sizeof(*created),out,sizeof(*out)))return Status::invalid_argument;DH2_OBJECTIVE_CALL(([&]{ObjectiveRef* child=nullptr;if(!call.make(input,child))return false;*created=child;return true;})());}
Status Runtime::set_owner(std::uintptr_t character,Result* out){DH2_OBJECTIVE_CALL(call.owner(character));}
Status Runtime::loop_stream(std::uint32_t function,std::int32_t encoded,player_saved_quests_v1::StreamRef& stream,Result* out){if(!aligned(&stream)||!stream.identity||!output(list_,this,out)||overlap(out,sizeof(*out),&stream,sizeof(stream)))return Status::invalid_argument;DH2_OBJECTIVE_CALL(call.stream(function,encoded,stream));}
Status Runtime::load(player_saved_quests_v1::StreamRef& stream,Result* out){return loop_stream(0x28,1,stream,out);}
Status Runtime::loop_member(MemberOperation operation,std::uint32_t function,std::int32_t encoded,Result* out,std::string& error){
 if(!output(list_,this,out))return Status::invalid_argument;
 if(busy_)return Status::reentrant;
 busy_=true;Guard guard{busy_};Call call{list_,services_};
 const bool ok=call.members(operation,function,encoded,error);
 if(ok)call.result.last_operation=Operation::complete;
 *out=call.result;if(ok)error.clear();return out->status;
}
Status Runtime::reset_data(Result* out,std::string& error){return loop_member(MemberOperation::reset_data,0x30,1,out,error);}
// Original ELF Objective::InvalidateCompile entry is 0x479f70. Preserve that
// nonvirtual member target as metadata; the provider resolves the same Record.
Status Runtime::invalidate_compile(Result* out,std::string& error){return loop_member(MemberOperation::invalidate_compile,0x479f70,0,out,error);}
Status Runtime::compile(Result* out,std::string& error){return loop_member(MemberOperation::compile,0x8,1,out,error);}
Status Runtime::register_objectives(Result* out,std::string& error){return loop_member(MemberOperation::register_objectives,0x18,1,out,error);}
Status Runtime::destroy(Result* out){DH2_OBJECTIVE_CALL(call.destroy());}
#undef DH2_OBJECTIVE_CALL
}
