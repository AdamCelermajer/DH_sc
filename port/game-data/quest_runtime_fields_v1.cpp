#include "quest_runtime_fields_v1.hpp"
#include "player_saved_quests_v1.hpp"
#include <cstring>
#include <initializer_list>

namespace dh2::data::quest_runtime_fields_v1 {namespace {
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){
 auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
 return an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y||(an&&bn&&x<y+bn&&y<x+an);
}
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
std::int32_t signed_word(std::uintptr_t value){const auto word=std::uint32_t(value);std::int32_t result;std::memcpy(&result,&word,4);return result;}
struct Call {
 Record& q;const Services& services;Result result{};
 const std::uintptr_t identity=q.ref.identity;
 bool coherent(){if(q.ref.fields!=&q.fields||q.ref.identity!=identity||!identity||identity>UINTPTR_MAX-0x6c){result.status=Status::projection_changed;return false;}return true;}
 bool send(Operation op,std::uintptr_t target=0,std::uintptr_t value=0,std::int32_t count=0,
           const PyDataRef* row=nullptr,std::uint32_t offset=0,Response* answer=nullptr,
           player_saved_quests_v1::StreamRef* stream=nullptr,void* destination=nullptr){
  result.last_operation=op;
  if(!services.invoke){result.status=Status::service_unavailable;return false;}
  ++result.service_calls;Response response{};
  try{if(services.invoke(services.context,{op,&q,target,value,count,row,offset,stream,destination},&response)){result.status=Status::service_failed;return false;}}
  catch(...){result.status=Status::service_failed;return false;}
  if(!coherent())return false;
  if(answer)*answer=response;
  return true;
 }
 bool owner(ActionRef* action){
  if(!action)return true;
  if(!aligned(action)||!action->identity||!aligned(action->character_10)||
     overlap(action->character_10,sizeof(*action->character_10),&q,sizeof(q))||
     overlap(action->character_10,sizeof(*action->character_10),action,sizeof(*action))){result.status=Status::source_fault;return false;}
  *action->character_10=q.fields.character_60;++result.owner_stores;return true;
 }
 bool children(){
  if(!send(Operation::objective_owners,q.ref.identity+0x2c,q.fields.character_60)||
     !send(Operation::reward_owners,q.ref.identity+0x38,q.fields.character_60))return false;
  return owner(q.action_18)&&owner(q.action_1c);
 }
 bool word(std::uint32_t offset,std::uintptr_t& value){
  const auto* row=q.py_data_68;
  if(!aligned(row)||!row->identity||row->identity>UINTPTR_MAX-offset){result.status=Status::source_fault;return false;}
  Response response;
  if(!send(Operation::read_py_word,row->identity+offset,0,0,row,offset,&response))return false;
  value=response.value;return true;
 }
 bool list(Operation op,std::uint32_t destination,std::uint32_t count_offset,std::uint32_t data_offset){
  std::uintptr_t count=0,data=0;
  if(!word(count_offset,count)||!word(data_offset,data))return false;
  return send(op,q.ref.identity+destination,data,signed_word(count));
 }
 bool action(std::uint32_t offset,ActionRef*& destination){
  const auto* row=q.py_data_68;
  if(!aligned(row)||!row->identity||row->identity>UINTPTR_MAX-offset){result.status=Status::source_fault;return false;}
  Response response;if(!send(Operation::create_objective,row->identity+offset,0,0,row,offset,&response))return false;
  if(!aligned(response.action)||!response.action->identity){result.status=Status::source_fault;return false;}
  destination=response.action;return true;
 }
 bool construct(std::int32_t difficulty){
  q.difficulty_10=difficulty;q.fields.row_8=-1;q.state_0=-1;q.word_4=0;q.word_c=1;
  q.fields.definition_name_14=0;q.action_18=nullptr;q.action_1c=nullptr;
  if(!send(Operation::construct_conditions,q.ref.identity+0x20)||
     !send(Operation::construct_objectives,q.ref.identity+0x2c)||
     !send(Operation::construct_rewards,q.ref.identity+0x38))return false;
  q.byte_5c=1;q.py_data_68=nullptr;q.byte_5d=0;q.byte_64=0;return true;
 }
 bool assign(const PyDataRef& row){
  q.py_data_68=&row;
  if(!list(Operation::assign_conditions,0x20,0x14,0x18)||
     !list(Operation::assign_objectives,0x2c,0x1c,0x20))return false;
  if(q.difficulty_10>=0&&q.difficulty_10<=2){
   const auto offset=std::uint32_t(q.difficulty_10)*8;
   if(!list(Operation::assign_rewards,0x38,0x24+offset,0x28+offset))return false;
  }
  if(!action(0x3c,q.action_18)||!action(0x68,q.action_1c)||!children())return false;
  std::uintptr_t word_c=0;if(!word(0x118,word_c))return false;
  q.word_c=signed_word(word_c);return true;
 }
 bool cleanup(ActionRef* action,std::uint32_t offset,player_saved_quests_v1::StreamRef* stream=nullptr){
  if(!aligned(action)||!action->identity){result.status=Status::source_fault;return false;}
  return send(Operation::action_virtual,action->identity,0,0,nullptr,offset,nullptr,stream);
 }
 bool reinit(){
  switch(q.state_0){
   case 2:if(!cleanup(q.action_18,0x14))return false;break;
   case 3:if(!cleanup(q.action_18,0x1c))return false;break;
   case 5:if(!send(Operation::remove_markers,q.ref.identity+0x2c))return false;break;
   case 6:if(!send(Operation::unregister_objectives,q.ref.identity+0x2c))return false;break;
   case 8:if(!cleanup(q.action_1c,0x14))return false;break;
   case 9:if(!cleanup(q.action_1c,0x1c))return false;break;
   default:break;
  }
  std::uintptr_t state=0;if(!word(0x9c,state))return false;q.state_0=signed_word(state);return true;
 }
 bool load(player_saved_quests_v1::StreamRef& stream){
  if(!send(Operation::read_stream_word,stream.identity,q.ref.identity,0,nullptr,0,nullptr,&stream,&q.state_0)||
     !cleanup(q.action_18,0x28,&stream)||!cleanup(q.action_1c,0x28,&stream)||
     !send(Operation::load_objectives,q.ref.identity+0x2c,0,0,nullptr,0,nullptr,&stream))return false;
  if(is_volatile_state(q.state_0))q.byte_64=1;
  return true;
 }
 bool destroy(){
  if(q.action_18){if(!cleanup(q.action_18,4))return false;q.action_18=nullptr;}
  if(q.action_1c){if(!cleanup(q.action_1c,4))return false;q.action_1c=nullptr;}
  return send(Operation::destroy_rewards,q.ref.identity+0x38)&&
         send(Operation::destroy_objectives,q.ref.identity+0x2c)&&
         send(Operation::destroy_conditions,q.ref.identity+0x20);
 }
};
bool output(const Record& q,const Runtime* runtime,const Result* out){
 if(!aligned(out)||overlap(out,sizeof(*out),&q,sizeof(q))||overlap(out,sizeof(*out),runtime,sizeof(*runtime)))return false;
 for(const auto* action:{q.action_18,q.action_1c})if(action&&(!aligned(action)||
    overlap(out,sizeof(*out),action,sizeof(*action))||(action->character_10&&overlap(out,sizeof(*out),action->character_10,sizeof(*action->character_10)))))return false;
 return true;
}
struct Guard{bool& busy;~Guard(){busy=false;}};
}
#define DH2_QUEST_CALL(body) \
 if(!output(record_,this,out))return Status::invalid_argument; \
 if(busy_)return Status::reentrant; \
 busy_=true;Guard guard{busy_};Call call{record_,services_}; \
 if(call.coherent()&&(body)){call.result.last_operation=Operation::complete;} \
 *out=call.result;return out->status
Status Runtime::construct(std::int32_t difficulty,Result* out){DH2_QUEST_CALL(call.construct(difficulty));}
Status Runtime::owner_children(Result* out){DH2_QUEST_CALL(call.children());}
Status Runtime::assign_pydata(const PyDataRef& row,Result* out){
 if(!aligned(&row)||!row.identity||row.identity>UINTPTR_MAX-0x118)return Status::invalid_argument;
 DH2_QUEST_CALL(call.assign(row));
}
Status Runtime::reinit(Result* out){DH2_QUEST_CALL(call.reinit());}
Status Runtime::destroy(Result* out){DH2_QUEST_CALL(call.destroy());}
Status Runtime::load_quest_data(player_saved_quests_v1::StreamRef& stream,std::uint32_t flag,Result* out){
 (void)flag;
 if(!aligned(&stream)||!stream.identity||!aligned(out)||overlap(out,sizeof(*out),&stream,sizeof(stream)))return Status::invalid_argument;
 DH2_QUEST_CALL(call.load(stream));
}
bool is_volatile_state(std::int32_t state) noexcept {
 static constexpr std::uint8_t table[]{1,0,1,1,0,1,1,0,1,1};
 const auto index=std::uint32_t(state)-2u;
 return index<=9u&&table[index]!=0;
}
#undef DH2_QUEST_CALL
}
