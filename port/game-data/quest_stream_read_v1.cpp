#include "quest_stream_read_v1.hpp"
#include <cstddef>
namespace dh2::data::quest_stream_read_v1 {namespace {
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){
 const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
 return an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y||(x&&y&&an&&bn&&x<y+bn&&y<x+an);
}
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
struct Call {
 StreamRef& stream;const Services& services;void* destination;Primitive primitive;
 Result result{};std::uintptr_t identity=stream.identity;
 bool fail(Status status,Operation operation){result.status=status;result.last_operation=operation;return false;}
 bool coherent(){return identity&&stream.identity==identity?true:fail(Status::projection_changed,result.last_operation);}
 template<class F>bool send(Operation operation,F fn){
  result.last_operation=operation;++result.service_calls;
  try{if(fn())return fail(Status::service_failed,operation);}catch(...){return fail(Status::service_failed,operation);}
  return coherent();
 }
 bool run(){
  const auto source=primitive==Primitive::unsigned_word?0x313b48u:primitive==Primitive::signed_word?0x38b758u:0x459090u;
  result.source_function=source;result.source_caller=source+0x18;result.requested=4;
  if(!services.read)return fail(Status::service_unavailable,Operation::read);
  ++result.read_calls;
  if(!send(Operation::read,[&]{return services.read(services.context,stream,destination,4,&result.returned);}))return false;
  if(result.returned==4)return true;
  result.source_caller=source+0x34;
  if(!services.assertion_mode)return fail(Status::service_unavailable,Operation::assertion_mode);
  std::int32_t mode=0;++result.assertion_reads;
  if(!send(Operation::assertion_mode,[&]{return services.assertion_mode(services.context,&mode);}))return false;
  if(mode==2){result.source_caller=source+0x40;return fail(Status::source_assertion,Operation::assertion_mode);}
  if(mode==1){
   result.source_caller=source+0x90;
   if(!services.log_assert)return fail(Status::service_unavailable,Operation::log_assert);
   const bool quest=primitive==Primitive::quest_signed_word;
   const AssertionRequest request{primitive,source,result.source_caller,quest?0x50u:0x45u,
    "ASSERT(%s) FAILED: %s:%d\n","bytesRead == sizeof(T)",quest?
    "..\\..\\project_vs2005\\Game/..\\..\\sources/Utils/StreamReader.h":
    "..\\..\\project_vs2005\\Game/..\\..\\sources/Utils/IStream.h"};
   ++result.assertion_logs;
   if(!send(Operation::log_assert,[&]{return services.log_assert(services.context,request);}))return false;
  }
  return true;
 }
};
struct Guard {bool& busy;~Guard(){busy=false;}};
}
Status Runtime::execute(void* destination,Primitive primitive,Result* out){
 if(!aligned(out)||!aligned(&stream_)||!destination||reinterpret_cast<std::uintptr_t>(destination)%alignof(std::uint32_t)||
    overlap(out,sizeof(*out),this,sizeof(*this))||overlap(out,sizeof(*out),&stream_,sizeof(stream_))||
    overlap(out,sizeof(*out),destination,4)||overlap(destination,4,this,sizeof(*this))||
    overlap(destination,4,&stream_,sizeof(stream_))||overlap(&stream_,sizeof(stream_),this,sizeof(*this)))return Status::invalid_argument;
 if(busy_)return Status::reentrant;
 busy_=true;Guard guard{busy_};Call call{stream_,services_,destination,primitive};
 if(call.coherent()&&call.run())call.result.last_operation=Operation::complete;
 *out=call.result;return out->status;
}
Status Runtime::read_unsigned(std::uint32_t* destination,Result* out){return execute(destination,Primitive::unsigned_word,out);}
Status Runtime::read_signed(std::int32_t* destination,Result* out){return execute(destination,Primitive::signed_word,out);}
Status Runtime::read_quest_signed(std::int32_t* destination,Result* out){return execute(destination,Primitive::quest_signed_word,out);}
}
