#include "player_saved_level_states_v1.hpp"
#include "level_tables.hpp"
#include "world_map_tables.hpp"
#include <cstring>
#include <stdexcept>

namespace dh2::data::player_saved_level_states_v1 {namespace {
struct Range {std::uintptr_t begin,end;};
bool overlap(Range a,Range b){return a.begin<b.end&&b.begin<a.end;}
template<class T>bool range(const T* p,Range& r){const auto a=reinterpret_cast<std::uintptr_t>(p);if(!p||a%alignof(T)||a>UINTPTR_MAX-sizeof(T))return false;r={a,a+sizeof(T)};return true;}
bool separate(const Bindings& b,Range r){
 Range save;if(!range(b.save,save)||overlap(save,r))return false;
 for(std::uint32_t d=0;d<3;++d)for(const auto* a:{b.save->source_level_states(d),b.save->source_world_map_states(d)}){
  const auto p=reinterpret_cast<std::uintptr_t>(a->words);const auto n=std::size_t(a->count)*4;
  if(a->count>65536||(a->words&&(p%4||p>UINTPTR_MAX-n||overlap(r,{p,p+n}))))return false;
 }
 return true;
}
std::int32_t signed_word(std::uint32_t value){std::int32_t result;std::memcpy(&result,&value,4);return result;}
struct Call {
 const Bindings& b;Bytes bytes;Result& out;std::string& error;
 bool fail(const char* message){if(error.empty())error=message;return false;}
 void stage(Stage value,std::uint32_t caller){out.stage=value;out.source_caller=caller;}
 bool read(std::uint32_t length,const std::uint8_t*& p){++out.read_calls;if(out.consumed>bytes.size||length>bytes.size-out.consumed)return fail("Saved levels reached truncated source stream");p=bytes.data+out.consumed;out.consumed+=length;return true;}
 bool word(std::int32_t& value){const std::uint8_t* p;if(!read(4,p))return false;value=signed_word(p[0]|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24);return true;}
 bool text(std::string& value){++out.string_reads;std::int32_t length;if(!word(length))return false;if(length<=0||length>1048576)return fail("Saved levels source string assertion/budget boundary");const std::uint8_t* p;if(!read(std::uint32_t(length),p))return false;if(p[length-1])return fail("Saved levels source string lacks terminator");value.assign(reinterpret_cast<const char*>(p),std::size_t(length)-1);return true;}
 bool count(SavedStateTableV1 table,std::uint32_t& value){++out.table_counts;if(!b.services.count)return fail("Actual source saved-level table count required");if(!b.services.count(b.services.context,table,&value,error))return fail("Saved-level table count provider failed");if(value>65536)return fail("Saved-level table count exceeds native bound");return true;}
 bool resolve(SavedStateTableV1 table,const std::string& value){
  std::uint32_t length=0;if(!count(table,length))return false;out.resolved_id=-1;if(!length)return true;
  NameArray names{};++out.name_arrays;if(!b.services.names)return fail("Actual source saved-level name array required");
  if(!b.services.names(b.services.context,table,&names,error))return fail("Saved-level name-array provider failed");
  if(!names.backing||!names.name)return fail("Saved-level source name-array backing unavailable");
  for(std::uint32_t row=0;row<length;++row){const auto* name=names.name(names.backing,row);if(!name)return fail("Saved-level source name row unavailable");++out.name_comparisons;if(!std::strcmp(value.c_str(),name)){out.resolved_id=std::int32_t(row);break;}}
  return true;
 }
 bool assertion(SavedStateTableV1 table,Assertion condition,std::int32_t id,std::int32_t value,std::int32_t difficulty){
  ++out.assert_mode_reads;if(!b.services.assert_mode)return fail("Actual source assertion-mode global required");std::int32_t mode=0;
  if(!b.services.assert_mode(b.services.context,&mode,error))return fail("Saved-level assertion-mode provider failed");
  if(mode==2)return fail("Saved-level source assertion null-store boundary");
  if(mode!=1)return true;
  ++out.assert_logs;const auto map=table==SavedStateTableV1::world_map;
  const std::uint32_t callers[]{map?0x466c04u:0x466f34u,map?0x466cc8u:0x466ff8u,map?0x466c54u:0x466f84u,map?0x466c94u:0x466fc4u};
  const AssertRequest request{table,condition,id,value,difficulty,callers[std::uint32_t(condition)],(map?0x7du:0x5bu)+std::uint32_t(condition)};
  if(!b.services.log_assert)return fail("Actual source saved-level assertion logger required");
  return b.services.log_assert(b.services.context,request,error)||fail("Saved-level assertion logger failed");
 }
 bool store(SavedStateTableV1 table,std::int32_t id,std::int32_t value,std::int32_t difficulty){
  out.table=std::uint32_t(table);out.difficulty=std::uint32_t(difficulty);out.resolved_id=id;out.last_state=value;
  if(id<0&&!assertion(table,Assertion::negative_id,id,value,difficulty))return false;
  std::uint32_t length=0;if(!count(table,length))return false;
  if(id>=std::int32_t(length)&&!assertion(table,Assertion::large_id,id,value,difficulty))return false;
  if(value<0){if(!assertion(table,Assertion::negative_state,id,value,difficulty))return false;}
  else if(value>(table==SavedStateTableV1::levels?1:2)&&!assertion(table,Assertion::large_state,id,value,difficulty))return false;
  auto* array=table==SavedStateTableV1::levels?b.save->source_level_states(std::uint32_t(difficulty)):b.save->source_world_map_states(std::uint32_t(difficulty));
  if(!array||!array->words||reinterpret_cast<std::uintptr_t>(array->words)%4||id<0||std::uint32_t(id)>=array->count)return fail("Saved-level source array store is outside actual published backing");
  array->words[id]=value;++out.stores;return true;
 }
 bool load(){
  for(std::uint32_t table=0;table<2;++table)for(std::uint32_t d=0;d<3;++d){
   out.table=table;out.difficulty=d;stage(Stage::count,table?0x46ab38:0x46aa58);if(!word(out.declared_entries))return false;
   if(out.declared_entries>65536)return fail("Saved-level stream count exceeds native bound");
   for(std::int32_t row=0;row<out.declared_entries;++row){
    std::string name;stage(Stage::name,table?0x46ab54:0x46aa74);if(!text(name))return false;
    stage(Stage::resolve,table?0x46ab64:0x46aa88);if(!resolve(SavedStateTableV1(table),name))return false;
    stage(Stage::value,table?0x46abac:0x46aacc);if(!word(out.last_state))return false;
    if(out.resolved_id!=-1){stage(Stage::set_state,table?0x46abc8:0x46aae8);if(!store(SavedStateTableV1(table),out.resolved_id,out.last_state,std::int32_t(d)))return false;}
    ++out.completed_entries;
   }
  }
  stage(Stage::complete,0x46abf8);return true;
 }
};
bool outputs(const Bindings& b,const void* self,Result* out,std::string& error){Range r,e,s;if(!range(out,r)||!range(&error,e)||!range(static_cast<const Runtime*>(self),s)||overlap(r,e)||overlap(r,s)||overlap(e,s)||!separate(b,r)||!separate(b,e))return false;return true;}
}
Services table_services(TableBindings& tables) noexcept {
 return {&tables,
  [](void* raw,SavedStateTableV1 table,std::uint32_t* value,std::string& error){const auto& b=*static_cast<TableBindings*>(raw);const auto n=table==SavedStateTableV1::levels?(b.levels?b.levels->levels.size():SIZE_MAX):(b.world_map?b.world_map->locations.size():SIZE_MAX);if(n>65536){error="Actual selected saved-level table owner required within native bound";return false;}*value=std::uint32_t(n);return true;},
  [](void* raw,SavedStateTableV1 table,NameArray* value,std::string& error){const auto& b=*static_cast<TableBindings*>(raw);if(table==SavedStateTableV1::levels){if(!b.levels){error="Actual LevelTables owner required";return false;}*value={b.levels->levels.data(),[](const void* rows,std::uint32_t row){return static_cast<const LevelDeclaration*>(rows)[row].name.c_str();}};}else{if(!b.world_map){error="Actual WorldMapTables owner required";return false;}*value={b.world_map->locations.data(),[](const void* rows,std::uint32_t row){return static_cast<const WorldMapLocation*>(rows)[row].name.c_str();}};}return true;},
  [](void* raw,std::int32_t* value,std::string& error){const auto& b=*static_cast<TableBindings*>(raw);if(!b.assertion_mode){error="Actual source assertion-mode global required";return false;}*value=*b.assertion_mode;return true;},
  [](void* raw,const AssertRequest& q,std::string& error){const auto& b=*static_cast<TableBindings*>(raw);if(!b.log_assert){error="Actual source saved-level assertion logger required";return false;}return b.log_assert(b.logger_context,q,error);}
 };
}
Runtime::Runtime(Bindings b):bindings_(b){Range save;if(!range(bindings_.save,save))throw std::invalid_argument("Actual borrowed saved-level Save required");}
Status Runtime::load(Bytes bytes,Result* out,std::string& error){
 if(busy_)return Status::busy;
 const auto p=reinterpret_cast<std::uintptr_t>(bytes.data);Range r,e,s;
 if(!outputs(bindings_,this,out,error)||(!bytes.data&&bytes.size)||bytes.size>67108864||p>UINTPTR_MAX-bytes.size)return Status::invalid_argument;
 if(bytes.size){if(!range(out,r)||!range(&error,e)||!range(this,s))return Status::invalid_argument;const Range input{p,p+bytes.size};if(overlap(input,r)||overlap(input,e)||overlap(input,s)||!separate(bindings_,input))return Status::invalid_argument;}
 *out={};error.clear();busy_=true;struct Guard{bool& busy;~Guard(){busy=false;}}guard{busy_};Call call{bindings_,bytes,*out,error};
 try{return call.load()?Status::complete:Status::failed;}catch(...){if(error.empty())error="Saved-level source provider threw";return Status::failed;}
}
Status Runtime::set_state(SavedStateTableV1 table,std::int32_t id,std::int32_t value,std::int32_t difficulty,Result* out,std::string& error){
 if(busy_)return Status::busy;
 if(std::uint32_t(table)>1||!outputs(bindings_,this,out,error))return Status::invalid_argument;
 *out={};out->stage=Stage::set_state;out->source_caller=table==SavedStateTableV1::levels?0x466e48:0x466b18;error.clear();busy_=true;struct Guard{bool& busy;~Guard(){busy=false;}}guard{busy_};Call call{bindings_,{nullptr,0},*out,error};
 try{if(!call.store(table,id,value,difficulty))return Status::failed;out->stage=Stage::complete;return Status::complete;}catch(...){if(error.empty())error="Saved-level source provider threw";return Status::failed;}
}
}
