#include "player_saved_quests_v1.hpp"
#include <cstring>
#include <stdexcept>
namespace dh2::data::player_saved_quests_v1 {namespace {
using Log=quest_savegame_v1::QuestSavegame;
struct Range{std::uintptr_t begin,end;};
bool overlap(Range a,Range b){return a.begin<b.end&&b.begin<a.end;}
template<class T>bool range(const T* p,Range& r){const auto a=reinterpret_cast<std::uintptr_t>(p);if(!p||a%alignof(T)||a>UINTPTR_MAX-sizeof(T))return false;r={a,a+sizeof(T)};return true;}
bool valid(const Bindings& b){Range s,a,c,t;if(!range(b.save,s)||!range(b.log_b8,a)||!range(b.log_118,c)||!range(b.stream,t)||!b.stream->identity)return false;if(b.log_b8!=&b.save->source_quest_log_b8()||b.log_118!=&b.save->source_quest_log_118())return false;if(overlap(a,c)||overlap(s,t)||overlap(a,t)||overlap(c,t))return false;return &b.log_b8->word_44==&b.save->quest_log_b8_act_words_v1()&&&b.log_118->word_44==&b.save->quest_log_118_act_words_v1();}
bool separate(const Bindings& b,Range r){
 const auto clear=[&](const auto* p){Range v;return range(p,v)&&!overlap(r,v);};
 if(!clear(b.save)||!clear(b.log_b8)||!clear(b.log_118)||!clear(b.stream))return false;
 for(const auto* log:{b.log_b8,b.log_118})for(const auto& v:log->quests){const auto p=reinterpret_cast<std::uintptr_t>(v.data());const auto n=v.size()*sizeof(v[0]);if(v.size()>65536||p>UINTPTR_MAX-n||overlap(r,{p,p+n}))return false;for(const auto* q:v)if(q){if(!clear(q))return false;if(q->fields&&!clear(q->fields))return false;}}
 return true;
}
std::int32_t signed_word(std::uint32_t value){std::int32_t result;std::memcpy(&result,&value,4);return result;}
struct Call {
 const Bindings& b;Result& out;std::string& error;
 Log& log(){return *(out.log?b.log_118:b.log_b8);}
 bool fail(const char* message){if(error.empty())error=message;return false;}
 bool send(Request request,Reply* answer=nullptr){
  out.operation=request.operation;out.source_caller=request.source_caller;
  if(!b.services.invoke)return fail("Actual source QEST service required");
  ++out.service_calls;Reply response{};
  request.stream=b.stream;request.log=&log();request.difficulty=std::int32_t(out.difficulty);
  request.ordinal=out.ordinal;request.index=out.index;
  if(b.services.invoke(b.services.context,request,response,error))return fail("QEST source service delivery failed");
  if(!valid(b))return fail("QEST canonical borrowed owners changed");
  if(answer)*answer=response;
  return true;
 }
 bool word(void* destination,bool unsigned_word,std::uint32_t caller){++out.read_words;Request request;request.operation=unsigned_word?Operation::read_unsigned:Operation::read_signed;request.source_caller=caller;request.destination=destination;return send(request);}
 bool unpack_one(std::int32_t ordinal,std::uint32_t flag){
  out.ordinal=ordinal;out.index=ordinal;
  if(!word(&out.index,false,0x46c3a0))return false;
  // The original reads this vector only AFTER the streamed index callback.
  auto& vector=log().quests[out.difficulty];
  if(out.index<0||std::uint32_t(out.index)>=vector.size())return fail("QEST source streamed index is outside actual vector backing");
  auto* const quest=vector[std::uint32_t(out.index)];
  if(quest){Range ref;if(!range(quest,ref)||!quest->identity)return fail("QEST actual Quest identity unavailable");++out.quest_calls;Request request;request.operation=Operation::quest_data;request.source_caller=0x46c3c8;request.quest=quest;request.flag=std::uint8_t(flag);return send(request);}
  ++out.null_quests;++out.assert_mode_reads;Request request;request.operation=Operation::assert_mode;request.source_caller=0x46c420;Reply response;if(!send(request,&response))return false;
  if(response.word==2)return fail("QEST source null Quest assertion-store boundary");
  if(response.word!=1)return true;
  ++out.assert_logs;request.operation=Operation::log_assert;request.source_caller=0x46c464;return send(request);
 }
 bool unpack_group(std::int32_t difficulty,std::uint32_t flag){
  if(difficulty<0||difficulty>=3)return fail("QEST source difficulty vector outside actual backing");
  out.difficulty=std::uint32_t(difficulty);
  const auto size=log().quests[out.difficulty].size();if(size>UINT32_MAX)return fail("QEST native vector length outside source word");
  out.captured_count=std::uint32_t(size);out.declared_count=0;
  if(!word(&out.declared_count,true,0x46c4bc))return false;
  if(out.declared_count!=out.captured_count){++out.mismatched_groups;return true;}
  ++out.matched_groups;if(out.declared_count>65536)return fail("QEST matching group count exceeds native bound");
  const auto count=out.declared_count;
  for(std::uint32_t ordinal=0;ordinal<count;++ordinal)if(!unpack_one(signed_word(ordinal),flag))return false;
  if(!word(&log().word_2c[out.difficulty],false,0x46c518))return false;
  ++out.tail_read_returns;
  if(!word(&log().word_38[out.difficulty],false,0x46c530))return false;
  ++out.tail_read_returns;
  if(!word(&log().word_44[out.difficulty],false,0x46c53c))return false;
  ++out.tail_read_returns;
  log().word_50[out.difficulty]=log().word_44[out.difficulty];++out.copied_word50;return true;
 }
 bool load_log(std::uint32_t which){out.log=which;for(std::int32_t d=0;d<3;++d)if(!unpack_group(d,0))return false;return true;}
 bool load(){
  out.log=0;Request request;request.operation=Operation::tell;request.source_caller=0x46948c;request.virtual_slot=0x24;Reply response;++out.tell_calls;
  if(!send(request,&response))return false;
  out.told_position=response.position;
  const auto captured=std::uint32_t(response.position);out.seek_position=captured;
  if(!load_log(0))return false;
  ++out.seek_calls;request.operation=Operation::seek;request.source_caller=0x4694b4;request.virtual_slot=0x20;request.offset=captured;
  if(!send(request))return false;
  return load_log(1);
 }
};
}
Runtime::Runtime(Bindings b):bindings_(b){if(!valid(bindings_))throw std::invalid_argument("Actual same-Save QEST logs/stream required");}
Status Runtime::execute(std::uint32_t method,std::uint32_t which,std::int32_t ordinal,std::int32_t difficulty,std::uint32_t flag,Result* out,std::string& error){
 if(busy_)return Status::busy;
 Range r,e,t;if(which>1||!valid(bindings_)||!range(out,r)||!range(&error,e)||!range(this,t)||overlap(r,e)||overlap(r,t)||overlap(e,t)||!separate(bindings_,r)||!separate(bindings_,e))return Status::invalid_argument;
 *out={};out->log=which;error.clear();busy_=true;struct Guard{bool& busy;~Guard(){busy=false;}}guard{busy_};Call call{bindings_,*out,error};
 try{bool ok=false;if(method==0)ok=call.load();else if(method==1)ok=call.load_log(which);else if(method==2)ok=call.unpack_group(difficulty,flag);else if(difficulty<0||difficulty>=3)ok=call.fail("QEST source difficulty vector outside actual backing");else{out->difficulty=std::uint32_t(difficulty);ok=call.unpack_one(ordinal,flag);}if(!ok)return Status::failed;out->operation=Operation::complete;return Status::complete;}
 catch(...){if(error.empty())error="QEST source provider threw";return Status::failed;}
}
Status Runtime::load(Result* out,std::string& error){return execute(0,0,0,0,0,out,error);}
Status Runtime::load_quests(std::uint32_t log,Result* out,std::string& error){return execute(1,log,0,0,0,out,error);}
Status Runtime::unpack_quests(std::uint32_t log,std::int32_t d,std::uint32_t flag,Result* out,std::string& error){return execute(2,log,0,d,flag,out,error);}
Status Runtime::unpack_quest(std::uint32_t log,std::int32_t ordinal,std::int32_t d,std::uint32_t flag,Result* out,std::string& error){return execute(3,log,ordinal,d,flag,out,error);}
}
