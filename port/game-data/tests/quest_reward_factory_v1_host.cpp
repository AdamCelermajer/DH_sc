#include "../quest_reward_factory_v1.hpp"
#include "../quest_table_bindings_v1.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
using namespace dh2::data::quest_reward_factory_v1;
namespace {
constexpr std::uintptr_t ID=0x10001000;
using Input=std::array<std::int64_t,6>;using Event=std::array<std::int64_t,4>;
unsigned checks=0;void check(bool v){if(!v)throw std::runtime_error("reward factory guard "+std::to_string(checks+1));++checks;}
Dispatch dispatch(int type){return type==5?Dispatch::base:Dispatch(std::uint32_t(type)+2);}
struct Fixture {
 Record q{ID};Input input{};std::vector<Event> trace;bool allocated=false,retired=false;Runtime* runtime=nullptr;Status nested=Status::complete;
 Fixture(){q.dispatch_0=Dispatch::uninitialized;q.fields.type_4=std::int32_t(0xcccccccc);q.fields.character_10=0xcccccccc;q.fields.py_data_c.index=0xcccccccc;q.compiled_8=0xcc;q.allocation_padding_9.fill(0xcc);q.compiled_py_data_14.index=0xcccccccc;q.property_owner_18=0xcccccccc;}
 bool event(int op,std::int64_t a=0,std::int64_t b=0,std::int64_t c=0){
  trace.push_back({op,a,b,c});
  if(input[4]>0&&trace.size()==std::size_t(input[4])){if(input[5])throw std::runtime_error("declared dependency exception");return false;}return true;
 }
 Services services(){return {this,
 [](void* raw,std::uint32_t bytes,std::uint32_t tag,Record** out)->std::int32_t{auto& f=*static_cast<Fixture*>(raw);if(!f.event(1,bytes,tag,ID))return 1;f.allocated=true;*out=&f.q;return 0;},
 [](void* raw,Record& q,const char* group,const char* name,std::int32_t* out)->std::int32_t{auto& f=*static_cast<Fixture*>(raw);
  check(std::string(group)=="v2QuestRewardType"&&std::string(name)=="Invalid");
  if(f.input[3]==1){q.fields.type_4=-7;q.compiled_8=7;q.fields.character_10=77;q.fields.py_data_c.index=55;q.compiled_py_data_14.index=44;}
  if(f.input[3]==2){Result result;f.nested=f.runtime->construct_base(q,&result);}
  if(!f.event(2))return 1;
  *out=std::int32_t(f.input[2]);return 0;},
 [](void* raw,Record* q)->std::int32_t{auto& f=*static_cast<Fixture*>(raw);if(f.input[3]==1)q->fields.character_10=77;if(!f.event(3,q->ref.identity))return 1;f.retired=true;return 0;}};}
};
template<class T>void values(const T& v){std::cout<<'[';bool comma=false;for(auto n:v){if(comma)std::cout<<',';comma=true;std::cout<<n;}std::cout<<']';}
void projection(const Fixture& f,Status status){
 std::cout<<"{\"status\":"<<unsigned(status)<<",\"fields\":";
 values(std::array<std::int64_t,10>{std::int64_t(std::uint32_t(f.q.dispatch_0)),f.q.fields.type_4,f.q.compiled_8,f.q.fields.py_data_c.index,std::int64_t(f.q.fields.character_10),f.q.compiled_py_data_14.index,std::int64_t(f.q.property_owner_18),f.q.allocation_padding_9[0],f.q.allocation_padding_9[1],f.q.allocation_padding_9[2]});
 std::cout<<",\"allocated\":"<<(f.allocated?1:0)<<",\"retired\":"<<(f.retired?1:0)<<",\"trace\":[";
 bool comma=false;for(const auto& event:f.trace){if(comma)std::cout<<',';comma=true;values(event);}std::cout<<"]}";
}
std::shared_ptr<std::vector<std::uint8_t>> file(const std::string& path){std::ifstream f(path,std::ios::binary|std::ios::ate);check(bool(f));const auto size=f.tellg();auto bytes=std::make_shared<std::vector<std::uint8_t>>(std::size_t(size));f.seekg(0);check(bool(f.read(reinterpret_cast<char*>(bytes->data()),size)));return bytes;}
void guards(){
 {Fixture f;Runtime r(f.services());Result result;check(r.create(-1,&result)==Status::invalid_argument&&f.trace.empty());check(r.create(5,&result)==Status::invalid_argument&&f.trace.empty());check(r.create(0,nullptr)==Status::invalid_argument);}
 {Fixture f;Runtime r({});Result result;check(r.create(0,&result)==Status::service_unavailable);check(r.construct_base(f.q,&result)==Status::service_unavailable&&f.q.dispatch_0==Dispatch::base);}
 {Fixture f;auto s=f.services();s.allocate=[](void*,std::uint32_t,std::uint32_t,Record**)->std::int32_t{return 0;};Runtime r(s);Result result;check(r.create(0,&result)==Status::source_fault);}
 {Fixture f;auto s=f.services();s.allocate=[](void* raw,std::uint32_t,std::uint32_t,Record** out)->std::int32_t{auto& x=*static_cast<Fixture*>(raw);x.q.ref.fields=nullptr;*out=&x.q;return 0;};Runtime r(s);Result result;check(r.create(0,&result)==Status::projection_changed);}
 {Fixture f;Runtime r(f.services());auto* alias=reinterpret_cast<Result*>(&f.q);check(r.create(0,alias)==Status::invalid_argument&&f.q.dispatch_0==Dispatch::uninitialized);}
 {Fixture f;f.input[3]=2;Runtime r(f.services());f.runtime=&r;Result result;check(r.create(0,&result)==Status::complete&&f.nested==Status::reentrant);}
 {Fixture f;Runtime r(f.services());Result result;f.q.ref.fields=nullptr;check(r.construct_base(f.q,&result)==Status::projection_changed);check(r.destroy(f.q,true,&result)==Status::projection_changed);}
 {Fixture f;Runtime r({});Result result;f.q.dispatch_0=Dispatch::gold;check(r.destroy(f.q,true,&result)==Status::service_unavailable);check(r.destroy(f.q,false,&result)==Status::complete);f.q.dispatch_0=Dispatch::uninitialized;check(r.destroy(f.q,false,&result)==Status::source_fault);}
}
std::array<unsigned,5> actual(const std::string& cache){
 auto packed=file(cache+"/v2quests_pyarray.bin"),names=file(cache+"/v2quests_pyarraynames.bin"),constants=file(cache+"/v2quests_pycst.bin");
 dh2_quest_table table{};check(!dh2_quests_open(&table,packed->data(),std::uint32_t(packed->size())));dh2::data::quest_table_bindings_v1::Owner owner;std::string error;check(owner.load({table,packed,names->data(),names->size(),names},error));auto view=owner.borrow();check(view.count()==64);
 dh2_pycst_view c{};check(!dh2_pycst_open(&c,constants->data(),std::uint32_t(constants->size())));Constants borrow{&c};Record base{ID};std::int32_t value=-1;check(!borrowed_constant(&borrow,base,"v2QuestRewardType","Invalid",&value)&&value==5);check(!borrowed_constant(&borrow,base,"v2QuestRewardType","Missing",&value)&&value==0);
 const auto copy=*constants;check(borrowed_constant(&borrow,base,"v2QuestRewardType","Invalid",reinterpret_cast<std::int32_t*>(constants->data()))!=0&&*constants==copy);
 check(borrowed_constant(&borrow,base,"v2QuestRewardType","Invalid",reinterpret_cast<std::int32_t*>(&c))!=0);
 check(borrowed_constant(&borrow,base,"v2QuestRewardType","Invalid",&base.fields.type_4)!=0);
 struct Arena {std::vector<std::unique_ptr<Record>> records;Constants constants;unsigned queries=0;};Arena arena;arena.constants=borrow;
 Services services{&arena,[](void* raw,std::uint32_t,std::uint32_t tag,Record** out)->std::int32_t{auto& a=*static_cast<Arena*>(raw);check(tag==0);a.records.push_back(std::make_unique<Record>(ID+a.records.size()*0x100));*out=a.records.back().get();return 0;},[](void* raw,Record& q,const char* group,const char* name,std::int32_t* out)->std::int32_t{auto& a=*static_cast<Arena*>(raw);++a.queries;return borrowed_constant(&a.constants,q,group,name,out);},[](void*,Record*)->std::int32_t{return 0;}};
 Runtime runtime(services);std::array<unsigned,5> counts{};
 for(unsigned i=0;i<view.count();++i)for(unsigned kind=2;kind<5;++kind){const auto* list=view.list(*view.row(i),kind);check(list);for(unsigned j=0;j<list->definition->count;++j){dh2_quest_span span{};dh2::data::quest_table_bindings_v1::Span bytes;check(view.list_record(*list,j,&span,error)&&view.bytes(span,&bytes,error)&&bytes.size==12);const auto type=std::int32_t(std::uint32_t(bytes.data[0])|(std::uint32_t(bytes.data[1])<<8)|(std::uint32_t(bytes.data[2])<<16)|(std::uint32_t(bytes.data[3])<<24));check(type>=0&&type<5);++counts[unsigned(type)];Result result;check(runtime.create(type,&result)==Status::complete&&result.record&&result.record->ref.fields==&result.record->fields&&result.record->fields.type_4==5&&result.record->dispatch_0==dispatch(type));}}
 check(arena.queries==222&&counts==std::array<unsigned,5>{30,192,0,0,0});return counts;
}
}
int main(int argc,char** argv){try{
 check(argc==3);std::ifstream f(argv[1]);check(bool(f));Input input{};std::cout<<"{\"results\":[";bool comma=false;
 while(f>>input[0]>>input[1]>>input[2]>>input[3]>>input[4]>>input[5]){Fixture fixture;fixture.input=input;if(input[0]>=3)fixture.q.dispatch_0=dispatch(int(input[1]));Runtime runtime(fixture.services());fixture.runtime=&runtime;Result result;Status status=input[0]==0?runtime.create(std::int32_t(input[1]),&result):input[0]<3?runtime.construct_base(fixture.q,&result):runtime.destroy(fixture.q,input[0]==4,&result);if(comma)std::cout<<',';comma=true;projection(fixture,status);}
 std::cout<<"],\"actual_rewards\":";values(actual(argv[2]));const auto before=checks;guards();std::cout<<",\"native_guard_checks\":"<<checks-before<<",\"native_checks\":"<<checks<<"}";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}return 0;}
