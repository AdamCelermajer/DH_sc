// Native platform composition, not another ARM oracle or current APK receipt.
#include "../app/src/main/cpp/native_quest_cursor.hpp"
#include "../../game-data/quest_objective_payload_v1.hpp"
#include "../../game-data/quest_stream_read_v1.hpp"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>
namespace d=dh2::data;
namespace n=dh2::native::quests;
namespace of=d::quest_objective_factory_v1;
namespace ol=d::quest_objective_list_v1;
namespace payload=d::quest_objective_payload_v1;
namespace primitive=d::quest_stream_read_v1;
namespace table=d::quest_table_bindings_v1;
namespace scalar=d::quest_runtime_fields_v1;
namespace saved=d::player_saved_quests_v1;
namespace {
unsigned checks=0;
void check(bool value){if(!value)throw std::runtime_error("Quest payload stream check "+std::to_string(checks+1));++checks;}
using Bytes=std::vector<std::uint8_t>;
std::shared_ptr<Bytes> file(const std::string& path){std::ifstream f(path,std::ios::binary|std::ios::ate);if(!f)throw std::runtime_error("Actual cache missing");const auto size=f.tellg();auto data=std::make_shared<Bytes>(std::size_t(size));f.seekg(0);if(!f.read(reinterpret_cast<char*>(data->data()),size))throw std::runtime_error("Cache read");return data;}
void word(Bytes& bytes,std::uint32_t value){for(unsigned i=0;i<4;++i)bytes.push_back(std::uint8_t(value>>(i*8)));}
std::int32_t signed_word(std::uint32_t value){std::int32_t result;std::memcpy(&result,&value,4);return result;}
struct Cache {
 table::View view;std::shared_ptr<Bytes> constants;dh2_pycst_view constant_view{};
 explicit Cache(const std::string& path){auto packed=file(path+"/v2quests_pyarray.bin"),names=file(path+"/v2quests_pyarraynames.bin");table::Input input;check(!dh2_quests_open(&input.table,packed->data(),std::uint32_t(packed->size())));input.packed_owner=packed;input.names=names->data();input.names_size=names->size();input.names_owner=names;table::Owner owner;std::string error;check(owner.load(input,error));view=owner.borrow();check(view.count()==64);constants=file(path+"/v2quests_pycst.bin");check(!dh2_pycst_open(&constant_view,constants->data(),std::uint32_t(constants->size())));}
};
std::unique_ptr<n::Cursor> campaign(const Bytes& packet,const Bytes* trailer=nullptr){
 Bytes bytes;word(bytes,trailer?3:2);word(bytes,4);for(char c:std::string("PNAM"))bytes.push_back(std::uint8_t(c));word(bytes,0x12345678);
 word(bytes,std::uint32_t(packet.size()));for(char c:std::string("QEST"))bytes.push_back(std::uint8_t(c));bytes.insert(bytes.end(),packet.begin(),packet.end());
 if(trailer){word(bytes,std::uint32_t(trailer->size()));for(char c:std::string("FTVL"))bytes.push_back(std::uint8_t(c));bytes.insert(bytes.end(),trailer->begin(),trailer->end());}
 d::PlayerProfileIndexV1 index;std::string error;check(index.load({bytes.data(),bytes.size()},error));auto cursor=std::make_unique<n::Cursor>(index.borrow(),"QEST");return cursor;
 // Local input/index are destroyed; Cursor retains their existing snapshot.
}
struct Platform {
 n::Cursor* cursor=nullptr;std::int32_t mode=2;unsigned reads=0,modes=0,logs=0;
 std::uint64_t bytes=0;int fail_after_read=-1;
 static std::int32_t read(void* raw,saved::StreamRef& stream,void* destination,std::uint64_t requested,std::uint64_t* count){auto& s=*static_cast<Platform*>(raw);++s.reads;if(!s.cursor||!s.cursor->read(stream,destination,requested,count))return 1;s.bytes+=*count;return s.fail_after_read==int(requested)?1:0;}
 static std::int32_t assertion(void* raw,std::int32_t* mode){auto& s=*static_cast<Platform*>(raw);++s.modes;*mode=s.mode;return 0;}
 template<class Request>static std::int32_t log(void* raw,const Request& request){auto& s=*static_cast<Platform*>(raw);++s.logs;return !request.line||std::strcmp(request.format,"ASSERT(%s) FAILED: %s:%d\n")||std::strcmp(request.condition,"bytesRead == sizeof(T)")||!request.file;}
 payload::Services payload_services(){return {this,read,assertion,log<payload::AssertionRequest>};}
 primitive::Services direct_services(){return {this,read,assertion,log<primitive::AssertionRequest>};}
 std::uint64_t position(){std::uint64_t p=0;check(cursor->tell(cursor->stream(),&p));return p;}
};
struct Graph {
 struct Row {
  scalar::Record record;ol::List list;std::unique_ptr<ol::Runtime> list_runtime;std::unique_ptr<scalar::Runtime> runtime;
  explicit Row(std::uintptr_t identity):record(identity){}
 };
 Cache& cache;d::PlayerSavegameV1 save;Platform platform;
 std::unique_ptr<n::Cursor> cursor;
 std::map<std::uintptr_t,std::unique_ptr<of::Record>> objectives;
 std::map<std::uintptr_t,std::unique_ptr<ol::Array>> arrays;
 std::map<std::uintptr_t,std::unique_ptr<Row>> rows;
 of::Runtime factory;
 unsigned payload_calls=0,quest_calls=0;
 explicit Graph(Cache& data):cache(data),factory(factory_services()){
  // Existing-state fixture: these are the canonical Save vectors/Quest fields;
  // actual native InitQuests/construct/Assign/ReInit has separate startup gates.
  for(unsigned log=0;log<2;++log)for(unsigned difficulty=0;difficulty<3;++difficulty){auto& vector=store(log).quests[difficulty];
   for(unsigned index=0;index<cache.view.count();++index){auto row=std::make_unique<Row>(1);auto* raw=row.get();raw->record.ref.identity=reinterpret_cast<std::uintptr_t>(raw);raw->record.fields.row_8=std::int32_t(index);raw->record.difficulty_10=std::int32_t(difficulty);raw->record.py_data_68=cache.view.row(index);raw->record.state_0=-99;
    raw->list_runtime=std::make_unique<ol::Runtime>(raw->list,list_services());ol::Result result;const auto* actual=cache.view.row(index);const auto* list=cache.view.list(*actual,1);
    check(raw->list_runtime->assign_pydata({cache.view,list,0,nullptr},std::int32_t(list->definition->count),&result)==ol::Status::complete);
    for(unsigned offset:{0x3cu,0x68u}){ol::ObjectiveRef* action=nullptr;check(raw->list_runtime->create_objective({cache.view,nullptr,0,cache.view.resolve_stub(actual->identity+offset)},&action,&result)==ol::Status::complete);if(offset==0x3c)raw->record.action_18=&action->action;else raw->record.action_1c=&action->action;}
    raw->runtime=std::make_unique<scalar::Runtime>(raw->record,scalar_services());vector.push_back(&raw->record.ref);rows.emplace(raw->record.ref.identity,std::move(row));
   }
  }
  check(rows.size()==384&&objectives.size()==1164);
 }
 d::quest_savegame_v1::QuestSavegame& store(unsigned log){return log?save.source_quest_log_118():save.source_quest_log_b8();}
 of::Services factory_services(){of::Services s;s.context=this;
  s.allocate=[](void* raw,std::uint32_t,std::uint32_t tag,of::Record** out){auto& g=*static_cast<Graph*>(raw);if(tag)return 1;auto record=std::make_unique<of::Record>(1);record->ref.action.identity=reinterpret_cast<std::uintptr_t>(record.get());*out=record.get();return g.objectives.emplace(record->ref.action.identity,std::move(record)).second?0:1;};
  s.get_constant=[](void* raw,of::Record& record,const char* group,const char* name,std::int32_t* out){auto& g=*static_cast<Graph*>(raw);of::Constants constants{&g.cache.constant_view};return of::borrowed_constant(&constants,record,group,name,out);};
  s.deallocate=[](void* raw,of::Record* record){auto& g=*static_cast<Graph*>(raw);auto found=g.objectives.find(record->ref.action.identity);if(found==g.objectives.end()||found->second.get()!=record)return 1;g.objectives.erase(found);return 0;};return s;
 }
 ol::Services list_services(){ol::Services s;s.context=this;
  s.allocate=[](void* raw,ol::List&,std::uint32_t bytes,std::uint32_t tag,ol::Array** out){auto& g=*static_cast<Graph*>(raw);if(tag||bytes%4)return 1;auto array=std::make_unique<ol::Array>();array->identity=reinterpret_cast<std::uintptr_t>(array.get());array->slots.resize(bytes/4);*out=array.get();return g.arrays.emplace(array->identity,std::move(array)).second?0:1;};
  s.factory=[](void* raw,ol::List&,const ol::Definition&,std::int32_t kind,ol::ObjectiveRef** out){auto& g=*static_cast<Graph*>(raw);of::Result r;if(g.factory.create(kind,&r)!=of::Status::complete)return 1;*out=&r.record->ref;return 0;};
  s.stream_member=[](void* raw,ol::List&,const ol::StreamCall& call){auto& g=*static_cast<Graph*>(raw);if(!call.objective||!call.virtual_call||call.function!=0x28||call.encoded_adjustment!=1||call.adjusted_target!=call.objective->action.identity)return 1;return g.load_objective(call.adjusted_target,call.stream,call.objective);};
  s.delete_virtual4=[](void* raw,ol::List&,ol::ObjectiveRef* ref){auto& g=*static_cast<Graph*>(raw);auto found=g.objectives.find(ref->action.identity);if(found==g.objectives.end()||&found->second->ref!=ref)return 1;of::Result r;return g.factory.destroy(*found->second,true,&r)==of::Status::complete?0:1;};
  s.deallocate=[](void* raw,ol::List&,ol::Array* array){auto& g=*static_cast<Graph*>(raw);auto found=g.arrays.find(array->identity);if(found==g.arrays.end()||found->second.get()!=array)return 1;g.arrays.erase(found);return 0;};return s;
 }
 int load_objective(std::uintptr_t identity,saved::StreamRef* stream,const ol::ObjectiveRef* expected=nullptr){auto found=objectives.find(identity);if(!platform.cursor||stream!=&platform.cursor->stream()||found==objectives.end()||(expected&&&found->second->ref!=expected))return 1;payload::Runtime loader(*found->second,platform.payload_services());payload::ReaderScratch scratch{0xa5,0xa5a5a5a5};payload::Result result;++payload_calls;return loader.load(*stream,scratch,&result)==payload::Status::complete?0:1;}
 scalar::Services scalar_services(){return {this,[](void* raw,const scalar::Request& request,scalar::Response*){auto& g=*static_cast<Graph*>(raw);if(!g.platform.cursor||request.stream!=&g.platform.cursor->stream())return 1;
  if(request.operation==scalar::Operation::read_stream_word){primitive::Runtime reader(*request.stream,g.platform.direct_services());primitive::Result out;return reader.read_quest_signed(static_cast<std::int32_t*>(request.destination),&out)==primitive::Status::complete?0:1;}
  if(request.operation==scalar::Operation::action_virtual&&request.offset==0x28)return g.load_objective(request.target,request.stream);
  if(request.operation==scalar::Operation::load_objectives){auto found=g.rows.find(request.quest->ref.identity);if(found==g.rows.end()||&found->second->record!=request.quest||request.target!=request.quest->ref.identity+0x2c)return 1;ol::Result out;return found->second->list_runtime->load(*request.stream,&out)==ol::Status::complete?0:1;}
  return 1;}};}
 Row* resolve(const d::quest_savegame_v1::QuestRef* ref){auto found=rows.find(ref->identity);return found!=rows.end()&&&found->second->record.ref==ref?found->second.get():nullptr;}
 saved::Services saved_services(){return {this,[](void* raw,const saved::Request& request,saved::Reply& reply,std::string&)->std::int32_t{auto& g=*static_cast<Graph*>(raw);if(!g.platform.cursor||request.stream!=&g.platform.cursor->stream())return 1;
  if(request.operation==saved::Operation::tell)return g.platform.cursor->tell(*request.stream,&reply.position)?0:1;
  if(request.operation==saved::Operation::seek)return g.platform.cursor->seek(*request.stream,request.offset)?0:1;
  if(request.operation==saved::Operation::read_unsigned||request.operation==saved::Operation::read_signed){primitive::Runtime reader(*request.stream,g.platform.direct_services());primitive::Result r;return (request.operation==saved::Operation::read_unsigned?reader.read_unsigned(static_cast<std::uint32_t*>(request.destination),&r):reader.read_signed(static_cast<std::int32_t*>(request.destination),&r))==primitive::Status::complete?0:1;}
  if(request.operation==saved::Operation::quest_data){auto* row=g.resolve(request.quest);if(!row)return 1;++g.quest_calls;scalar::Result r;return row->runtime->load_quest_data(*request.stream,request.flag,&r)==scalar::Status::complete?0:1;}
  return 1;}};}
 static bool quantity(const of::Record& record){return record.fields.type_4!=4&&record.fields.type_4!=6&&record.fields.type_4!=12;}
 static std::uint8_t done(unsigned row,unsigned slot){return std::uint8_t(0x80|((row*3+slot)&0x7f));}
 static std::uint32_t amount(unsigned row,unsigned slot){return 0xf1234000u|(row<<4)|slot;}
 of::Record& objective(std::uintptr_t id){return *objectives.at(id);}
 void append_objective(Bytes& bytes,std::uintptr_t id,unsigned index,unsigned slot){const auto& record=objective(id);bytes.push_back(done(index,slot));if(quantity(record))word(bytes,amount(index,slot));}
 Bytes packet(){Bytes bytes;for(unsigned difficulty=0;difficulty<3;++difficulty){word(bytes,64);for(unsigned i=0;i<64;++i){const auto index=63-i;auto* row=resolve(store(0).quests[difficulty][index]);word(bytes,index);word(bytes,5);append_objective(bytes,row->record.action_18->identity,index,0);append_objective(bytes,row->record.action_1c->identity,index,1);if(row->list.children_4)for(unsigned n=0;n<row->list.children_4->slots.size();++n)append_objective(bytes,row->list.children_4->slots[n]->action.identity,index,2+n);}word(bytes,0x1122+difficulty);word(bytes,0x2233+difficulty);word(bytes,0x3344+difficulty);}check(bytes.size()==2826);return bytes;}
 void verify_objective(std::uintptr_t id,unsigned index,unsigned slot){const auto& record=objective(id);check(record.done_14==done(index,slot));if(quantity(record))check(record.quantity_20==amount(index,slot));}
 void run(){const auto bytes=packet();const Bytes trailer{0xde,0xad,0xbe,0xef};cursor=campaign(bytes,&trailer);platform.cursor=cursor.get();saved::Runtime reader({&save,&save.source_quest_log_b8(),&save.source_quest_log_118(),&cursor->stream(),saved_services()});saved::Result result;std::string error;check(reader.load(&result,error)==saved::Status::complete);
  check(quest_calls==384&&payload_calls==1164&&platform.reads==2286&&platform.bytes==5652&&platform.modes==0&&platform.logs==0);check(platform.position()==24+bytes.size());check(result.told_position==24&&result.seek_position==24);
  for(unsigned log=0;log<2;++log)for(unsigned difficulty=0;difficulty<3;++difficulty){auto& saved=store(log);check(saved.word_2c[difficulty]==std::int32_t(0x1122+difficulty)&&saved.word_38[difficulty]==std::int32_t(0x2233+difficulty)&&saved.word_44[difficulty]==std::int32_t(0x3344+difficulty)&&saved.word_50[difficulty]==saved.word_44[difficulty]);for(unsigned index=0;index<64;++index){auto* row=resolve(saved.quests[difficulty][index]);check(row->record.state_0==5&&row->record.byte_64==1);verify_objective(row->record.action_18->identity,index,0);verify_objective(row->record.action_1c->identity,index,1);if(row->list.children_4)for(unsigned n=0;n<row->list.children_4->slots.size();++n)verify_objective(row->list.children_4->slots[n]->action.identity,index,2+n);}}
  for(auto& row:rows){ol::Result out;check(row.second->list_runtime->destroy(&out)==ol::Status::complete);for(auto* action:{row.second->record.action_18,row.second->record.action_1c}){auto found=objectives.find(action->identity);of::Result r;check(found!=objectives.end()&&factory.destroy(*found->second,true,&r)==of::Status::complete);}row.second->record.action_18=nullptr;row.second->record.action_1c=nullptr;}
  check(arrays.empty()&&objectives.empty());
 }
};
void prefixes(Cache& cache){Graph graph(cache);auto* row=graph.resolve(graph.store(0).quests[0][0]);check(row->list.children_4&&row->list.children_4->slots.size()==1);auto& record=graph.objective(row->list.children_4->slots[0]->action.identity);check(record.fields.type_4==0);
 // A real QEST caller must retain the completed action/bool prefix when the
 // same cursor reaches a short SavedQty word. Its second log and tails remain
 // untouched because the first Quest provider has not returned successfully.
 {Bytes bytes;word(bytes,64);word(bytes,0);word(bytes,5);graph.append_objective(bytes,row->record.action_18->identity,0,0);graph.append_objective(bytes,row->record.action_1c->identity,0,1);bytes.push_back(Graph::done(0,2));bytes.push_back(0x78);bytes.push_back(0x56);
  const auto old_tail=graph.store(0).word_44[0];record.quantity_20=0xcccccccc;graph.cursor=campaign(bytes);graph.platform.cursor=graph.cursor.get();saved::Runtime reader({&graph.save,&graph.save.source_quest_log_b8(),&graph.save.source_quest_log_118(),&graph.cursor->stream(),graph.saved_services()});saved::Result out;std::string error;
  check(reader.load(&out,error)==saved::Status::failed);check(out.operation==saved::Operation::quest_data&&out.log==0&&out.difficulty==0&&out.index==0);check(graph.quest_calls==1&&graph.payload_calls==3&&graph.platform.modes==1&&graph.platform.logs==0);check(row->record.state_0==5&&row->record.byte_64==0);graph.verify_objective(row->record.action_18->identity,0,0);graph.verify_objective(row->record.action_1c->identity,0,1);check(record.done_14==Graph::done(0,2)&&record.quantity_20==0xcccccccc);check(graph.resolve(graph.store(1).quests[0][0])->record.state_0==-99&&graph.store(0).word_44[0]==old_tail);check(graph.platform.position()==24+bytes.size());}
 for(int mode:{2,1}){auto cursor=campaign({128,0x78,0x56});Platform platform;platform.cursor=cursor.get();platform.mode=mode;record.done_14=0x7f;record.quantity_20=0xcccccccc;payload::Runtime reader(record,platform.payload_services());payload::ReaderScratch scratch{0xa5,0xa5a5a5a5};payload::Result out;const auto status=reader.load(cursor->stream(),scratch,&out);
  check(status==(mode==2?payload::Status::source_assertion:payload::Status::complete));check(record.done_14==128&&scratch.signed_word==0xa5a55678);check(record.quantity_20==(mode==2?0xcccccccc:0xa5a55678));check(platform.position()==27&&platform.reads==2&&platform.modes==1&&platform.logs==unsigned(mode==1));}
 {auto cursor=campaign({128,0x78,0x56});Platform platform;platform.cursor=cursor.get();platform.fail_after_read=4;record.done_14=0x7f;record.quantity_20=0xcccccccc;payload::Runtime reader(record,platform.payload_services());payload::ReaderScratch scratch{0xa5,0xa5a5a5a5};payload::Result out;check(reader.load(cursor->stream(),scratch,&out)==payload::Status::service_failed);check(record.done_14==128&&record.quantity_20==0xcccccccc&&scratch.signed_word==0xa5a55678);check(platform.position()==27&&platform.modes==0);}
 for(int failed:{0,1}){auto cursor=campaign({0x78,0x56});Platform platform;platform.cursor=cursor.get();if(failed)platform.fail_after_read=4;row->record.state_0=signed_word(0xa5a5a5a5);primitive::Runtime reader(cursor->stream(),platform.direct_services());primitive::Result out;check(reader.read_quest_signed(&row->record.state_0,&out)==(failed?primitive::Status::service_failed:primitive::Status::source_assertion));check(std::uint32_t(row->record.state_0)==0xa5a55678&&platform.position()==26&&platform.modes==unsigned(!failed));}
 {auto cursor=campaign({});Platform platform;platform.cursor=cursor.get();record.done_14=0x7f;record.quantity_20=0xcccccccc;payload::Runtime reader(record,platform.payload_services());payload::ReaderScratch scratch{0xa5,0xa5a5a5a5};payload::Result out;check(reader.load(cursor->stream(),scratch,&out)==payload::Status::source_assertion);check(record.done_14==0x7f&&record.quantity_20==0xcccccccc&&platform.position()==24);}
 // QEST size does not impose a stream limit: the next real header word can be
 // read by the source typed protocol. This is not semantic packet validation.
 {const Bytes trailer{9,8,7,6};auto cursor=campaign({128},&trailer);Platform platform;platform.cursor=cursor.get();record.quantity_20=0xcccccccc;payload::Runtime reader(record,platform.payload_services());payload::ReaderScratch scratch{0xa5,0xa5a5a5a5};payload::Result out;check(reader.load(cursor->stream(),scratch,&out)==payload::Status::complete);check(record.done_14==128&&record.quantity_20==4&&platform.position()==29&&platform.modes==0);}
 {auto cursor=campaign({1,2,3,4});Platform platform;platform.cursor=cursor.get();auto forged=cursor->stream();primitive::Runtime reader(forged,platform.direct_services());primitive::Result out;std::uint32_t destination=0xcccccccc;check(reader.read_unsigned(&destination,&out)==primitive::Status::service_failed);check(destination==0xcccccccc&&platform.position()==24);std::uint64_t delivered=77;check(!cursor->read(cursor->stream(),&delivered,4,&delivered)&&delivered==77&&platform.position()==24);check(!cursor->seek(cursor->stream(),UINT64_MAX)&&platform.position()==24);}
 // Prefix graph teardown uses the actual source D0/list paths, not implicit
 // Record destruction as a substitute for virtual cleanup.
 for(auto& value:graph.rows){ol::Result out;check(value.second->list_runtime->destroy(&out)==ol::Status::complete);for(auto* action:{value.second->record.action_18,value.second->record.action_1c}){auto found=graph.objectives.find(action->identity);of::Result r;check(found!=graph.objectives.end()&&graph.factory.destroy(*found->second,true,&r)==of::Status::complete);}value.second->record.action_18=nullptr;value.second->record.action_1c=nullptr;}check(graph.arrays.empty()&&graph.objectives.empty());
}
}
int main(int argc,char** argv){try{if(argc!=2)return 2;Cache cache(argv[1]);Graph complete(cache);complete.run();prefixes(cache);std::printf("{\"validation\":\"PASS\",\"checks\":%u,\"canonical_quests\":384,\"canonical_objectives_actions\":1164,\"full_qest_bytes\":2826,\"typed_read_calls\":2286,\"one_retained_absolute_cursor\":true,\"selected_qest_quest_factory_list\":true,\"selected_typed_readers\":true,\"native_policy_guards\":true,\"android_compilation\":false,\"live_gameplay\":false}\n",checks);return 0;}catch(const std::exception& error){std::fprintf(stderr,"%s\n",error.what());return 1;}}
