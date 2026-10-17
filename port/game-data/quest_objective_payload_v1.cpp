#include "quest_objective_payload_v1.hpp"
namespace dh2::data::quest_objective_payload_v1 {namespace {
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){
 const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
 return an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y||(x&&y&&an&&bn&&x<y+bn&&y<x+an);
}
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
struct Call {
 Record& q;StreamRef& stream;ReaderScratch& scratch;const Services& services;Result result{};
 std::uintptr_t identity=q.ref.action.identity,stream_identity=stream.identity;
 bool fail(Status status,Operation operation){result.status=status;result.last_operation=operation;return false;}
 bool coherent(){return identity&&q.ref.action.identity==identity&&q.ref.fields==&q.fields&&q.ref.action.character_10==&q.fields.character_10&&stream.identity==stream_identity&&stream_identity?true:fail(Status::projection_changed,result.last_operation);}
 template<class F>bool send(Operation op,F fn){result.last_operation=op;++result.service_calls;try{if(fn())return fail(Status::service_failed,op);}catch(...){return fail(Status::service_failed,op);}return coherent();}
 bool primitive(Primitive kind){
  const bool boolean=kind==Primitive::boolean;const auto op=boolean?Operation::read_boolean:Operation::read_signed;
  result.source_caller=boolean?0x429c38:0x336508;result.requested=boolean?1:4;result.returned=0;
  if(!services.read)return fail(Status::service_unavailable,op);
  void* destination=boolean?static_cast<void*>(&scratch.boolean):static_cast<void*>(&scratch.signed_word);
  ++result.read_calls;
  if(!send(op,[&]{return services.read(services.context,stream,destination,result.requested,&result.returned);}))return false;
  if(result.returned!=result.requested){
   result.source_caller=boolean?0x429c54:0x336524;
   if(!services.assertion_mode)return fail(Status::service_unavailable,Operation::assertion_mode);
   std::int32_t mode=0;++result.assertion_reads;
   if(!send(Operation::assertion_mode,[&]{return services.assertion_mode(services.context,&mode);}))return false;
   if(mode==2){result.source_caller=boolean?0x429c60:0x336530;return fail(Status::source_assertion,Operation::assertion_mode);}
   if(mode==1){
    result.source_caller=boolean?0x429cb4:0x336584;
    if(!services.log_assert)return fail(Status::service_unavailable,Operation::log_assert);
    const AssertionRequest request{kind,boolean?0x429c1cu:0x3364ecu,result.source_caller,0x44,
     "ASSERT(%s) FAILED: %s:%d\n","bytesRead == sizeof(T)","..\\..\\project_vs2005\\Game/..\\..\\sources/Utils/StreamReader.h"};
    ++result.assertion_logs;
    if(!send(Operation::log_assert,[&]{return services.log_assert(services.context,request);}))return false;
   }
  }
  result.value=boolean?scratch.boolean:scratch.signed_word;return true;
 }
 bool load(bool quantity){
  if(!primitive(Primitive::boolean))return false;
  result.last_operation=Operation::store_done;result.source_caller=0x47ab4c;
  q.done_14=std::uint8_t(result.value);++result.field_stores;
  if(quantity){
   if(!primitive(Primitive::signed_word))return false;
   result.last_operation=Operation::store_quantity;result.source_caller=0x47ab6c;
   q.quantity_20=result.value;++result.field_stores;
  }
  return true;
 }
};
struct Guard{bool& busy;~Guard(){busy=false;}};
}
Status Runtime::execute(StreamRef& stream,ReaderScratch& scratch,std::uint32_t method,Result* out){
 if(!aligned(out)||!aligned(&stream)||!aligned(&scratch)||
    overlap(out,sizeof(*out),&record_,sizeof(record_))||overlap(out,sizeof(*out),this,sizeof(*this))||
    overlap(out,sizeof(*out),&stream,sizeof(stream))||overlap(out,sizeof(*out),&scratch,sizeof(scratch))||
    overlap(&scratch,sizeof(scratch),&record_,sizeof(record_))||overlap(&scratch,sizeof(scratch),this,sizeof(*this))||
    overlap(&scratch,sizeof(scratch),&stream,sizeof(stream))||overlap(&stream,sizeof(stream),&record_,sizeof(record_))||
    overlap(&stream,sizeof(stream),this,sizeof(*this)))return Status::invalid_argument;
 if(busy_)return Status::reentrant;
 busy_=true;Guard guard{busy_};Call call{record_,stream,scratch,services_};
 if(call.coherent()&&(method<2?call.load(method==1):call.primitive(method==2?Primitive::boolean:Primitive::signed_word)))call.result.last_operation=Operation::complete;
 *out=call.result;return out->status;
}
Status Runtime::load_base(StreamRef& stream,ReaderScratch& scratch,Result* out){return execute(stream,scratch,0,out);}
Status Runtime::load_saved_quantity(StreamRef& stream,ReaderScratch& scratch,Result* out){return execute(stream,scratch,1,out);}
Status Runtime::read_bool(StreamRef& stream,ReaderScratch& scratch,Result* out){return execute(stream,scratch,2,out);}
Status Runtime::read_signed(StreamRef& stream,ReaderScratch& scratch,Result* out){return execute(stream,scratch,3,out);}
Status Runtime::load(StreamRef& stream,ReaderScratch& scratch,Result* out){
 using Dispatch=quest_objective_factory_v1::Dispatch;const auto dispatch=record_.dispatch_0;
 if(dispatch==Dispatch::base||dispatch==Dispatch::event_receiver||dispatch==Dispatch::move_in_zone||dispatch==Dispatch::automatic||dispatch==Dispatch::gather_loot)return load_base(stream,scratch,out);
 if(dispatch>=Dispatch::kill_enemies&&dispatch<=Dispatch::clear_enemy_template)return load_saved_quantity(stream,scratch,out);
 return Status::outside_domain;
}
}
