#include "../quest_objective_payload_v1.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
using namespace dh2::data::quest_objective_payload_v1;
namespace of=dh2::data::quest_objective_factory_v1;
namespace ol=dh2::data::quest_objective_list_v1;
namespace table=dh2::data::quest_table_bindings_v1;
namespace {
unsigned checks=0;
void require(bool ok){++checks;if(!ok)throw std::runtime_error("Objective payload check "+std::to_string(checks));}
std::shared_ptr<std::vector<std::uint8_t>> file(const std::string& path){std::ifstream f(path,std::ios::binary|std::ios::ate);if(!f)throw std::runtime_error("cache missing");const auto size=f.tellg();auto bytes=std::make_shared<std::vector<std::uint8_t>>(std::size_t(size));f.seekg(0);if(!f.read(reinterpret_cast<char*>(bytes->data()),size))throw std::runtime_error("cache read");return bytes;}
struct Data {
 std::shared_ptr<std::vector<std::uint8_t>> constants;dh2_pycst_view constant_view{};table::View definitions;
 explicit Data(const std::string& path){constants=file(path+"/v2quests_pycst.bin");require(!dh2_pycst_open(&constant_view,constants->data(),std::uint32_t(constants->size())));auto packed=file(path+"/v2quests_pyarray.bin"),names=file(path+"/v2quests_pyarraynames.bin");table::Input input;require(!dh2_quests_open(&input.table,packed->data(),std::uint32_t(packed->size())));input.packed_owner=packed;input.names=names->data();input.names_size=names->size();input.names_owner=names;table::Owner owner;std::string error;require(owner.load(input,error));definitions=owner.borrow();}
};
struct Fixture {
 Data& data;Record record{0x10010000};StreamRef stream{0x10020000};ReaderScratch scratch{0xa5,0xa5a5a5a5};
 std::unique_ptr<Runtime> runtime;std::vector<std::array<long long,4>> trace;std::vector<long long> args;
 std::vector<std::uint8_t> bytes;std::size_t cursor=0;unsigned kind=0,effects=0,seen=0;
 bool reenter=false;Status nested=Status::complete;
 explicit Fixture(Data& d):data(d){}
 int event(unsigned op,long long a=0,long long b=0,long long c=0){
  trace.push_back({op,a,b,c});++effects;
  if(reenter){reenter=false;Result r;nested=runtime->load(stream,scratch,&r);}
  if(op==unsigned(args[15])){record.done_14=std::uint8_t(args[16]);record.quantity_20=std::uint32_t(args[17]);record.dispatch_0=of::Dispatch(args[18]);scratch.boolean=std::uint8_t(args[19]);scratch.signed_word=std::uint32_t(args[20]);}
  if(op==unsigned(args[12])&&++seen==unsigned(args[13])){if(args[14])throw std::runtime_error("declared stream/log failure");return 1;}return 0;
 }
 Services services(){Services s;s.context=this;
  s.read=[](void* raw,StreamRef& stream,void* destination,std::uint64_t requested,std::uint64_t* returned){auto& f=*static_cast<Fixture*>(raw);if(&stream!=&f.stream||!destination||!returned||(requested!=1&&requested!=4))return 1;f.kind=requested==1?0:1;
   const auto available=f.bytes.size()-f.cursor;const auto count=std::size_t(std::min<std::uint64_t>(requested,available));if(count)std::memcpy(destination,f.bytes.data()+f.cursor,count);f.cursor+=count;
   const auto low=f.args[f.kind?7:5],high=f.args[f.kind?8:6];*returned=low==-9?count:(std::uint64_t(std::uint32_t(high))<<32)|std::uint32_t(low);
   return f.event(f.kind,requested,std::uint32_t(*returned),std::uint32_t(*returned>>32));};
  s.assertion_mode=[](void* raw,std::int32_t* out){auto& f=*static_cast<Fixture*>(raw);*out=std::int32_t(f.args[9]);return f.event(2,f.kind,*out);};
  s.log_assert=[](void* raw,const AssertionRequest& request){auto& f=*static_cast<Fixture*>(raw);if(unsigned(request.primitive)!=f.kind||std::strcmp(request.format,"ASSERT(%s) FAILED: %s:%d\n")||std::strcmp(request.condition,"bytesRead == sizeof(T)")||std::strcmp(request.file,"..\\..\\project_vs2005\\Game/..\\..\\sources/Utils/StreamReader.h"))return 1;return f.event(3,f.kind,request.source_function,request.line);};return s;
 }
 void setup(const std::vector<long long>& input){args=input;of::Services s;s.context=this;
  s.allocate=[](void* raw,std::uint32_t,std::uint32_t tag,Record** out){auto& f=*static_cast<Fixture*>(raw);if(tag)return 1;*out=&f.record;return 0;};
  s.get_constant=[](void* raw,Record& record,const char* group,const char* key,std::int32_t* out){auto& f=*static_cast<Fixture*>(raw);of::Constants constants{&f.data.constant_view};return of::borrowed_constant(&constants,record,group,key,out);};
  of::Runtime factory(s);of::Result result;if(factory.create(std::int32_t(args[1]),&result)!=of::Status::complete)throw std::runtime_error("actual factory");
  record.done_14=0x7f;record.quantity_20=0xcccccccc;scratch.boolean=std::uint8_t(args[10]);scratch.signed_word=std::uint32_t(args[11]);
  if(args[0]!=4)bytes.push_back(std::uint8_t(args[2]));
  for(unsigned i=0;i<4;++i)bytes.push_back(std::uint8_t(std::uint32_t(args[3])>>(i*8)));
  bytes.resize(std::min<std::size_t>(std::size_t(args[4]),bytes.size()));runtime=std::make_unique<Runtime>(record,services());
 }
 Status run(Result* out){switch(args[0]){case 0:return runtime->load(stream,scratch,out);case 1:return runtime->load_base(stream,scratch,out);case 2:return runtime->load_saved_quantity(stream,scratch,out);case 3:return runtime->read_bool(stream,scratch,out);default:return runtime->read_signed(stream,scratch,out);}}
};
void output(const Fixture& f,const Result& r){std::cout<<"{\"status\":"<<unsigned(r.status)<<",\"value\":"<<r.value<<",\"done\":"<<unsigned(f.record.done_14)<<",\"quantity\":"<<f.record.quantity_20<<",\"dispatch\":"<<unsigned(f.record.dispatch_0)<<",\"scratch\":["<<unsigned(f.scratch.boolean)<<','<<f.scratch.signed_word<<"],\"cursor\":"<<f.cursor<<",\"calls\":"<<r.service_calls<<",\"reads\":"<<r.read_calls<<",\"assertions\":"<<r.assertion_reads<<",\"logs\":"<<r.assertion_logs<<",\"stores\":"<<r.field_stores<<",\"effects\":"<<f.effects<<",\"trace\":[";bool comma=false;for(const auto& t:f.trace){if(comma)std::cout<<',';comma=true;std::cout<<'['<<t[0]<<','<<t[1]<<','<<t[2]<<','<<t[3]<<']';}std::cout<<"]}";}
void guards(Data& data){const std::vector<long long> base{0,0,255,-1,5,-9,0,-9,0,0,0xa5,-1515870811,-1,1,0,-1,55,99,7,128,42};
 for(unsigned missing=0;missing<3;++missing){Fixture f(data);f.setup(base);auto s=f.services();if(missing==0)s.read=nullptr;if(missing==1){s.assertion_mode=nullptr;f.args[5]=0;}if(missing==2){s.log_assert=nullptr;f.args[5]=0;f.args[9]=1;}Runtime r(f.record,s);Result out;require(r.load(f.stream,f.scratch,&out)==Status::service_unavailable);require(f.record.done_14==0x7f&&f.record.quantity_20==0xcccccccc);}
 {Fixture f(data);f.setup(base);f.reenter=true;Result out;require(f.run(&out)==Status::complete&&f.nested==Status::reentrant);}
 {Fixture f(data);f.setup(base);Result out;f.record.ref.action.character_10=nullptr;require(f.run(&out)==Status::projection_changed&&f.trace.empty());}
 {Fixture f(data);f.setup(base);Result out;f.stream.identity=0;require(f.run(&out)==Status::projection_changed&&f.trace.empty());}
 {Fixture f(data);f.setup(base);Result out;f.record.dispatch_0=of::Dispatch::uninitialized;require(f.run(&out)==Status::outside_domain&&f.trace.empty());}
 {Fixture f(data);f.setup(base);require(f.run(reinterpret_cast<Result*>(&f.record))==Status::invalid_argument&&f.trace.empty());require(f.record.done_14==0x7f);require(f.run(reinterpret_cast<Result*>(&f.stream))==Status::invalid_argument&&f.stream.identity==0x10020000);require(f.run(reinterpret_cast<Result*>(&f.scratch))==Status::invalid_argument&&f.scratch.boolean==0xa5);require(f.run(reinterpret_cast<Result*>(f.runtime.get()))==Status::invalid_argument);}
 {Fixture f(data);f.setup(base);auto s=f.services();s.read=[](void* raw,StreamRef& stream,void* destination,std::uint64_t,std::uint64_t* count){auto& f=*static_cast<Fixture*>(raw);*static_cast<std::uint8_t*>(destination)=128;*count=1;stream.identity=99;f.record.quantity_20=77;return 0;};Runtime changed(f.record,s);Result out;require(changed.load(f.stream,f.scratch,&out)==Status::projection_changed);require(f.scratch.boolean==128&&f.record.done_14==0x7f&&f.record.quantity_20==77);}
 // Whole selected ObjectiveList loop and actual factory objects share this
 // single stream/cursor. Each actual accept/end action uses the same loader.
 unsigned shipped=0;for(unsigned row=0;row<data.definitions.count();++row){
  Fixture f(data);f.setup(base);ol::List list;std::unique_ptr<ol::Array> array;std::vector<std::unique_ptr<Record>> records;
  struct Composition{Fixture* f;std::unique_ptr<ol::Array>* array;std::vector<std::unique_ptr<Record>>* records;} c{&f,&array,&records};
  ol::Services s;s.context=&c;s.allocate=[](void* raw,ol::List&,std::uint32_t bytes,std::uint32_t tag,ol::Array** out){auto& c=*static_cast<Composition*>(raw);if(tag)return 1;*c.array=std::make_unique<ol::Array>();(*c.array)->identity=0x10090000;(*c.array)->slots.resize(bytes/4);*out=c.array->get();return 0;};
  s.factory=[](void* raw,ol::List&,const ol::Definition&,std::int32_t kind,ol::ObjectiveRef** out){auto& c=*static_cast<Composition*>(raw);of::Services fs;fs.context=&c;fs.allocate=[](void* raw,std::uint32_t,std::uint32_t,Record** out){auto& c=*static_cast<Composition*>(raw);c.records->push_back(std::make_unique<Record>(0x100a0000+c.records->size()*0x100));*out=c.records->back().get();return 0;};fs.get_constant=[](void* raw,Record& q,const char* group,const char* key,std::int32_t* value){auto& c=*static_cast<Composition*>(raw);of::Constants constants{&c.f->data.constant_view};return of::borrowed_constant(&constants,q,group,key,value);};of::Runtime factory(fs);of::Result r;if(factory.create(kind,&r)!=of::Status::complete)return 1;*out=&r.record->ref;return 0;};
  s.stream_member=[](void* raw,ol::List&,const ol::StreamCall& call){auto& c=*static_cast<Composition*>(raw);Record* q=nullptr;for(auto& candidate:*c.records)if(&candidate->ref==call.objective)q=candidate.get();if(!q||call.stream!=&c.f->stream||!call.virtual_call||call.function!=0x28||call.encoded_adjustment!=1||call.adjusted_target!=q->ref.action.identity)return 1;Runtime loader(*q,c.f->services());ReaderScratch scratch{0xa5,0xa5a5a5a5};Result out;return loader.load(*call.stream,scratch,&out)==Status::complete?0:1;};
  ol::Runtime owner(list,s);ol::Result out;const auto* actual=data.definitions.row(row);ol::Definition definition{data.definitions,data.definitions.list(*actual,1),0,nullptr};require(owner.assign_pydata(definition,std::int32_t(definition.list->definition->count),&out)==ol::Status::complete);
  for(unsigned offset:{0x3cu,0x68u}){ol::Definition action{data.definitions,nullptr,0,data.definitions.resolve_stub(actual->identity+offset)};ol::ObjectiveRef* ref=nullptr;require(owner.create_objective(action,&ref,&out)==ol::Status::complete);require(ref&&records.back()->ref.fields==&records.back()->fields);}
  f.bytes.clear();for(const auto& q:records){f.bytes.push_back(128);if(q->dispatch_0!=of::Dispatch::automatic&&q->dispatch_0!=of::Dispatch::move_in_zone&&q->dispatch_0!=of::Dispatch::gather_loot)for(unsigned i=0;i<4;++i)f.bytes.push_back(std::uint8_t(0xf1234567u>>(i*8)));}
  require(owner.load(f.stream,&out)==ol::Status::complete);
  for(unsigned i=unsigned(list.count_0);i<records.size();++i){Runtime action(*records[i],f.services());ReaderScratch scratch{0xa5,0xa5a5a5a5};Result r;require(action.load(f.stream,scratch,&r)==Status::complete);}
  require(f.cursor==f.bytes.size());for(const auto& q:records){require(q->done_14==128);if(q->dispatch_0!=of::Dispatch::automatic&&q->dispatch_0!=of::Dispatch::move_in_zone&&q->dispatch_0!=of::Dispatch::gather_loot)require(q->quantity_20==0xf1234567u);++shipped;}
 }require(shipped==194);
}
}
int main(int argc,char** argv){try{if(argc!=3)return 2;Data data(argv[1]);std::ifstream input(argv[2]);std::string line;std::cout<<"{\"results\":[";bool comma=false;while(std::getline(input,line)){std::istringstream stream(line);std::vector<long long> args;long long value;while(stream>>value)args.push_back(value);if(args.size()!=21)throw std::runtime_error("payload width");Fixture f(data);f.setup(args);Result r;f.run(&r);if(comma)std::cout<<',';comma=true;output(f,r);}guards(data);std::cout<<"],\"native_checks\":"<<checks<<"}\n";return 0;}catch(const std::exception& e){std::cerr<<e.what();return 1;}}
