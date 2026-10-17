#include "quest_reward_execution_v1.hpp"
#include <cstring>
namespace dh2::data::quest_reward_execution_v1 {namespace {
using Dispatch=quest_reward_factory_v1::Dispatch;using Definition=quest_reward_list_v1::Definition;
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y||(x&&y&&an&&bn&&x<y+bn&&y<x+an);}
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool output(const Runtime* self,const Record& q,const Result* out){return aligned(out)&&!overlap(out,sizeof(*out),self,sizeof(*self))&&!overlap(out,sizeof(*out),&q,sizeof(q));}
bool coherent(const Record& q){return q.ref.identity&&q.ref.fields==&q.fields;}
std::int32_t signed32(std::uint32_t n){std::int32_t value;std::memcpy(&value,&n,4);return value;}
struct Call {
 Record& q;const Services& services;const Result* output;Result result{};bool output_safe=true;
 bool fail(Status s,Operation op){result.status=s;result.last_operation=op;return false;}
 template<class F>bool send(Operation op,F f){result.last_operation=op;++result.service_calls;try{if(f())return fail(Status::service_failed,op);}catch(...){return fail(Status::service_failed,op);}return true;}
 bool read(const Definition& d,std::uint32_t source_offset,std::int32_t& value){
  result.last_operation=Operation::read_definition;dh2_quest_span span{};quest_table_bindings_v1::Span bytes;std::string error;
  if(!d.list||d.list->kind<2||d.list->kind>4||!d.view.list_record(*d.list,d.index,&span,error)||!d.view.bytes(span,&bytes,error)||bytes.size!=12||(source_offset!=8&&source_offset!=12))return fail(Status::source_fault,Operation::read_definition);
  if(overlap(output,sizeof(*output),bytes.data,bytes.size)){output_safe=false;return fail(Status::invalid_argument,Operation::read_definition);}
  const auto* p=bytes.data+source_offset-4;value=signed32(std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24));return true;
 }
 bool character(std::uintptr_t identity,CharacterRef*& ref){
  result.last_operation=Operation::resolve_character;
  if(!services.resolve_character)return fail(Status::service_unavailable,Operation::resolve_character);
  try{ref=services.resolve_character(services.context,identity);}catch(...){return fail(Status::service_failed,Operation::resolve_character);}
  if(!aligned(ref)||!identity||ref->identity!=identity)return fail(Status::source_fault,Operation::resolve_character);
  if(overlap(output,sizeof(*output),ref,sizeof(*ref))||(ref->inventory&&overlap(output,sizeof(*output),ref->inventory,sizeof(*ref->inventory)))||overlap(output,sizeof(*output),ref->reward_gold_1500,4)||overlap(output,sizeof(*output),ref->reward_xp_1504,4)){output_safe=false;return fail(Status::invalid_argument,Operation::resolve_character);}
  return true;
 }
 bool compile(){
  result.last_operation=Operation::compile;
  if(q.dispatch_0<Dispatch::gold||q.dispatch_0>Dispatch::consume_loot)return fail(Status::source_fault,Operation::compile);
  if(q.dispatch_0==Dispatch::consume_loot){std::int32_t loot_id=0;if(!read(q.fields.py_data_c,12,loot_id))return false;if(loot_id>=0){q.compiled_8=1;++result.field_stores;}return true;}
  q.compiled_8=1;q.compiled_py_data_14=q.fields.py_data_c;result.field_stores+=2;return true;
 }
 bool give(bool xp){
  if(q.dispatch_0!=(xp?Dispatch::xp:Dispatch::gold))return fail(Status::source_fault,xp?Operation::give_xp:Operation::add_gold);
  if(!q.compiled_8){result.value=0;return true;}
  // Source captures the compiled definition, then the Character identity.
  // The final receipt uses freshly loaded controls after the actual effect.
  const auto first=q.compiled_py_data_14;const auto identity=q.fields.character_10;std::int32_t amount=0;
  if(!read(first,8,amount))return false;
  CharacterRef* character_ref=nullptr;
  if(!character(identity,character_ref))return false;
  bool granted=true;
  if(xp){
   if(!services.give_xp)return fail(Status::service_unavailable,Operation::give_xp);
   const auto fixed=signed32(std::uint32_t(amount)<<8);
   if(!send(Operation::give_xp,[&]{return services.give_xp(services.context,q,*character_ref,fixed,1,&granted);}))return false;
  }else{
   if(!services.add_gold)return fail(Status::service_unavailable,Operation::add_gold);
   if(!send(Operation::add_gold,[&]{return services.add_gold(services.context,q,*character_ref,amount);}))return false;
  }
  if(!xp||granted){
   const auto fresh=q.compiled_py_data_14;const auto fresh_identity=q.fields.character_10;
   if(!read(fresh,8,amount)||!character(fresh_identity,character_ref))return false;
   auto* destination=xp?character_ref->reward_xp_1504:character_ref->reward_gold_1500;
   result.last_operation=Operation::store_receipt;
   if(!aligned(destination)||overlap(destination,sizeof(*destination),&q,sizeof(q)))return fail(Status::source_fault,Operation::store_receipt);
   *destination=xp?signed32(std::uint32_t(amount)<<8):amount;++result.field_stores;
  }
  result.value=1;return true;
 }
};
struct Guard{bool& busy;~Guard(){busy=false;}};
}
#define DH2_REWARD_EXEC(body) \
 if(!output(this,record_,out))return Status::invalid_argument; \
 if(busy_)return Status::reentrant; \
 busy_=true;Guard guard{busy_};Call call{record_,services_,out}; \
 if(!coherent(record_))call.fail(Status::projection_changed,Operation::none); \
 else if(body)call.result.last_operation=Operation::complete; \
 if(!call.output_safe)return Status::invalid_argument; \
 *out=call.result;return out->status
Status Runtime::compile(Result* out){DH2_REWARD_EXEC(call.compile());}
Status Runtime::give_gold(Result* out){DH2_REWARD_EXEC(call.give(false));}
Status Runtime::give_xp(Result* out){DH2_REWARD_EXEC(call.give(true));}
#undef DH2_REWARD_EXEC
}
