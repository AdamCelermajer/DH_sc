#include "../quest_objective_factory_v1.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
using namespace dh2::data::quest_objective_factory_v1;
namespace {
constexpr std::uintptr_t ID=0x10001000;
using Input=std::array<std::int64_t,6>;using Event=std::array<std::int64_t,4>;
unsigned checks=0;void check(bool v){if(!v)throw std::runtime_error("objective factory check "+std::to_string(checks+1));++checks;}
Dispatch dispatch(int type){return type==13?Dispatch::base:Dispatch(std::uint32_t(type)+3);}
std::int64_t word24(const Record& q){return std::holds_alternative<std::int32_t>(q.derived_24)?std::uint32_t(std::get<std::int32_t>(q.derived_24)):std::get<dh2::data::quest_objective_list_v1::Definition>(q.derived_24).index;}
struct Fixture {
 Record q{ID};Input input{};std::vector<Event> trace;bool allocated=false,retired=false;Runtime* runtime=nullptr;Status nested=Status::complete;
 Fixture(){q.fields.type_4=std::int32_t(0xcccccccc);q.fields.character_10=0xcccccccc;q.fields.py_data_c.index=0xcccccccc;q.compiled_8=q.done_14=0xcc;q.allocation_padding_9.fill(0xcc);q.allocation_padding_15.fill(0xcc);q.allocation_padding_1c=q.quantity_20=q.derived_2c=0xcccccccc;q.derived_24=std::int32_t(0xcccccccc);q.derived_28=0xcccccccc;}
 bool event(int op,std::int64_t a=0,std::int64_t b=0,std::int64_t c=0){trace.push_back({op,a,b,c});if(input[4]>0&&trace.size()==std::size_t(input[4])){if(input[5])throw std::runtime_error("dependency exception");return false;}return true;}
 Services services(){return {this,
 [](void* raw,std::uint32_t bytes,std::uint32_t tag,Record** out)->std::int32_t{auto& f=*static_cast<Fixture*>(raw);if(!f.event(1,bytes,tag,ID))return 1;f.allocated=true;*out=&f.q;return 0;},
 [](void* raw,Record& q,const char* group,const char* name,std::int32_t* out)->std::int32_t{auto& f=*static_cast<Fixture*>(raw);check(std::string(group)=="v2QuestObjectiveType"&&std::string(name)=="Invalid");
  if(f.input[3]==1){q.fields.type_4=-7;q.compiled_8=7;q.done_14=9;q.fields.character_10=77;q.fields.py_data_c.index=55;}
  if(f.input[3]==2){Result result;f.nested=f.runtime->construct_base(q,&result);}
  if(!f.event(2))return 1;
  *out=std::int32_t(f.input[2]);return 0;},
 [](void* raw,Record* q)->std::int32_t{auto& f=*static_cast<Fixture*>(raw);if(f.input[3]==1)q->fields.character_10=77;if(!f.event(3,q->ref.action.identity))return 1;f.retired=true;return 0;}};}
};
template<class T>void values(const T& v){std::cout<<'[';bool comma=false;for(auto n:v){if(comma)std::cout<<',';comma=true;std::cout<<n;}std::cout<<']';}
void projection(const Fixture& f,Status status){
 std::cout<<"{\"status\":"<<unsigned(status)<<",\"fields\":";
 values(std::array<std::int64_t,18>{std::uint32_t(f.q.dispatch_0),f.q.fields.type_4,f.q.compiled_8,f.q.fields.py_data_c.index,std::int64_t(f.q.fields.character_10),f.q.done_14,std::uint32_t(f.q.secondary_18),f.q.allocation_padding_1c,f.q.quantity_20,word24(f.q),std::int64_t(f.q.derived_28),f.q.derived_2c,f.q.allocation_padding_9[0],f.q.allocation_padding_9[1],f.q.allocation_padding_9[2],f.q.allocation_padding_15[0],f.q.allocation_padding_15[1],f.q.allocation_padding_15[2]});
 std::cout<<",\"allocated\":"<<(f.allocated?1:0)<<",\"retired\":"<<(f.retired?1:0)<<",\"trace\":[";bool comma=false;for(const auto& e:f.trace){if(comma)std::cout<<',';comma=true;values(e);}std::cout<<"]}";
}
std::shared_ptr<std::vector<std::uint8_t>> file(const std::string& path){std::ifstream f(path,std::ios::binary|std::ios::ate);check(bool(f));auto size=f.tellg();auto bytes=std::make_shared<std::vector<std::uint8_t>>(std::size_t(size));f.seekg(0);check(bool(f.read(reinterpret_cast<char*>(bytes->data()),size)));return bytes;}
void guards(){
 {Fixture f;Runtime r(f.services());Result result;check(r.create(-1,&result)==Status::invalid_argument&&f.trace.empty());check(r.create(13,&result)==Status::invalid_argument&&f.trace.empty());check(r.create(0,nullptr)==Status::invalid_argument);}
 {Fixture f;Runtime r({});Result result;check(r.create(0,&result)==Status::service_unavailable);check(r.construct_base(f.q,&result)==Status::service_unavailable&&f.q.dispatch_0==Dispatch::base&&f.q.fields.type_4==std::int32_t(0xcccccccc));}
 {Fixture f;auto s=f.services();s.get_constant=nullptr;Runtime r(s);Result result;check(r.create(6,&result)==Status::service_unavailable&&f.q.dispatch_0==Dispatch::base&&f.q.fields.type_4==0&&f.q.done_14==0&&f.q.secondary_18==Dispatch::uninitialized);}
 {Fixture f;auto s=f.services();s.allocate=[](void*,std::uint32_t,std::uint32_t,Record**)->std::int32_t{return 0;};Runtime r(s);Result result;check(r.create(0,&result)==Status::source_fault);}
 {Fixture f;auto s=f.services();s.allocate=[](void* raw,std::uint32_t,std::uint32_t,Record** out)->std::int32_t{auto& x=*static_cast<Fixture*>(raw);x.q.ref.fields=nullptr;*out=&x.q;return 0;};Runtime r(s);Result result;check(r.create(0,&result)==Status::projection_changed);}
 {Fixture f;Runtime r(f.services());auto* alias=reinterpret_cast<Result*>(&f.q);check(r.create(0,alias)==Status::invalid_argument&&f.q.dispatch_0==Dispatch::uninitialized&&f.q.ref.fields==&f.q.fields);}
 {Fixture f;auto s=f.services();s.allocate=[](void* raw,std::uint32_t,std::uint32_t,Record** out)->std::int32_t{*out=&static_cast<Fixture*>(raw)->q;return 1;};Runtime r(s);check(r.create(0,reinterpret_cast<Result*>(&f.q))==Status::invalid_argument&&f.q.dispatch_0==Dispatch::uninitialized);}
 {Fixture f;auto s=f.services();s.allocate=[](void* raw,std::uint32_t,std::uint32_t,Record** out)->std::int32_t{*out=&static_cast<Fixture*>(raw)->q;throw std::runtime_error("allocation prefix");};Runtime r(s);check(r.create(0,reinterpret_cast<Result*>(&f.q))==Status::invalid_argument&&f.q.ref.fields==&f.q.fields);}
 {Fixture f;f.input[3]=2;Runtime r(f.services());f.runtime=&r;Result result;check(r.create(0,&result)==Status::complete&&f.nested==Status::reentrant);}
 {Fixture f;Runtime r(f.services());Result result;f.q.ref.action.character_10=nullptr;check(r.construct_base(f.q,&result)==Status::projection_changed);check(r.destroy(f.q,true,&result)==Status::projection_changed);}
 {Fixture f;auto s=f.services();s.get_constant=[](void*,Record& q,const char*,const char*,std::int32_t* out)->std::int32_t{q.ref.action.character_10=nullptr;*out=13;return 0;};Runtime r(s);Result result;check(r.create(0,&result)==Status::projection_changed&&f.q.fields.type_4==std::int32_t(0xcccccccc));}
 {Fixture f;Runtime r({});Result result;f.q.dispatch_0=Dispatch::kill_enemies;check(r.destroy(f.q,true,&result)==Status::service_unavailable&&f.q.dispatch_0==Dispatch::event_receiver&&f.q.secondary_18==Dispatch::event_receiver);f.q.dispatch_0=Dispatch::automatic;check(r.destroy(f.q,false,&result)==Status::complete&&f.q.dispatch_0==Dispatch::automatic);f.q.dispatch_0=Dispatch::uninitialized;check(r.destroy(f.q,false,&result)==Status::source_fault);}
}
std::array<unsigned,13> actual(const std::string& cache){
 using namespace dh2::data;namespace ol=quest_objective_list_v1;
 auto packed=file(cache+"/v2quests_pyarray.bin"),names=file(cache+"/v2quests_pyarraynames.bin"),constants=file(cache+"/v2quests_pycst.bin");
 dh2_quest_table table{};check(!dh2_quests_open(&table,packed->data(),std::uint32_t(packed->size())));quest_table_bindings_v1::Owner owner;std::string error;check(owner.load({table,packed,names->data(),names->size(),names},error));auto view=owner.borrow();check(view.count()==64);
 dh2_pycst_view c{};check(!dh2_pycst_open(&c,constants->data(),std::uint32_t(constants->size())));Constants borrowed{&c};Record base{ID};std::int32_t value=-1;
 check(!borrowed_constant(&borrowed,base,"v2QuestObjectiveType","Invalid",&value)&&value==13);check(!borrowed_constant(&borrowed,base,"v2QuestObjectiveType","Missing",&value)&&value==0);check(!borrowed_constant(&borrowed,base,"Missing","Invalid",&value)&&value==0);
 check(borrowed_constant(nullptr,base,"v2QuestObjectiveType","Invalid",&value)==1);Constants missing{};check(borrowed_constant(&missing,base,"v2QuestObjectiveType","Invalid",&value)==1);
 auto malformed=c;malformed.entries++;Constants broken{&malformed};value=-17;check(borrowed_constant(&broken,base,"v2QuestObjectiveType","Invalid",&value)==1&&value==-17);
 check(borrowed_constant(&borrowed,base,"v2QuestObjectiveType","Invalid",&base.fields.type_4)==1);
 const auto snapshot=*constants;check(borrowed_constant(&borrowed,base,"v2QuestObjectiveType","Invalid",reinterpret_cast<std::int32_t*>(constants->data()))==1&&*constants==snapshot);
 check(borrowed_constant(&borrowed,base,"v2QuestObjectiveType","Invalid",reinterpret_cast<std::int32_t*>(&c))==1);
 struct Arena {std::vector<std::unique_ptr<Record>> records;std::vector<std::unique_ptr<ol::Array>> arrays;Constants constants;unsigned queries=0,deletes=0,frees=0;Runtime* runtime=nullptr;std::array<unsigned,13> counts{};};Arena arena;arena.constants=borrowed;
 Services services{&arena,[](void* raw,std::uint32_t,std::uint32_t tag,Record** out)->std::int32_t{auto& a=*static_cast<Arena*>(raw);check(tag==0);a.records.push_back(std::make_unique<Record>(ID+a.records.size()*0x100));*out=a.records.back().get();return 0;},[](void* raw,Record& q,const char* group,const char* name,std::int32_t* out)->std::int32_t{auto& a=*static_cast<Arena*>(raw);++a.queries;return borrowed_constant(&a.constants,q,group,name,out);},[](void* raw,Record* q)->std::int32_t{auto& a=*static_cast<Arena*>(raw);check(q);++a.deletes;return 0;}};
 Runtime runtime(services);arena.runtime=&runtime;
 ol::Services list_services{&arena,
 [](void* raw,ol::List& list,std::uint32_t bytes,std::uint32_t tag,ol::Array** out)->std::int32_t{auto& a=*static_cast<Arena*>(raw);check(tag==0&&bytes==std::uint32_t(list.count_0)*4);a.arrays.push_back(std::make_unique<ol::Array>());auto& array=*a.arrays.back();array.identity=0x10008000+a.arrays.size()*0x100;array.slots.resize(std::size_t(list.count_0));*out=&array;return 0;},
 [](void* raw,ol::List&,const ol::Definition&,std::int32_t kind,ol::ObjectiveRef** out)->std::int32_t{auto& a=*static_cast<Arena*>(raw);Result result;check(a.runtime->create(kind,&result)==Status::complete&&result.record&&result.record->fields.type_4==13&&result.record->dispatch_0==dispatch(kind));++a.counts[std::size_t(kind)];*out=&result.record->ref;return 0;},nullptr,
 [](void* raw,ol::List&,ol::ObjectiveRef* ref)->std::int32_t{auto& a=*static_cast<Arena*>(raw);for(auto& q:a.records)if(&q->ref==ref){Result result;check(a.runtime->destroy(*q,true,&result)==Status::complete);return 0;}return 1;},
 [](void* raw,ol::List&,ol::Array*)->std::int32_t{++static_cast<Arena*>(raw)->frees;return 0;}};
 for(unsigned i=0;i<view.count();++i){const auto* row=view.row(i);check(row);const auto* list=view.list(*row,1);check(list);ol::List owned;ol::Runtime list_runtime(owned,list_services);ol::Result lr;check(list_runtime.construct(&lr)==ol::Status::complete);check(list_runtime.assign_pydata({view,list,0,nullptr},std::int32_t(list->definition->count),&lr)==ol::Status::complete);check(list_runtime.set_owner(ID+0x9000,&lr)==ol::Status::complete);
  for(auto* ref:owned.children_4?owned.children_4->slots:std::vector<ol::ObjectiveRef*>{})check(ref&&ref->fields&&ref->action.character_10==&ref->fields->character_10&&*ref->action.character_10==ID+0x9000&&ref->fields->py_data_c.list==list&&ref->fields->py_data_c.view.count()==64);
  for(unsigned offset:{0x3cu,0x68u}){const auto* stub=view.resolve_stub(row->identity+offset);check(stub);ol::ObjectiveRef* ref=nullptr;check(list_runtime.create_objective({view,nullptr,0,stub},&ref,&lr)==ol::Status::complete&&ref&&ref->fields->type_4==stub->definition->common[0]&&ref->fields->py_data_c.stub==stub);check(list_services.delete_virtual4(&arena,owned,ref)==0);}
  check(list_runtime.destroy(&lr)==ol::Status::complete&&owned.children_4==nullptr);
 }
 check(arena.counts==std::array<unsigned,13>{29,0,0,0,39,18,100,3,0,0,5,0,0});check(arena.queries==194&&arena.deletes==194);
 // Field-only compiled definition survives the published table owner and list.
 for(auto& q:arena.records)if(std::holds_alternative<ol::Definition>(q->derived_24))std::get<ol::Definition>(q->derived_24)=q->fields.py_data_c;
 owner=quest_table_bindings_v1::Owner{};view={};check(arena.records.front()->fields.py_data_c.view.count()==64);
 check(sizeof(std::uintptr_t)>=8);Record width{ID};width.derived_28=UINT64_C(0x12345678abcdef01);check(width.derived_28==UINT64_C(0x12345678abcdef01));
 return arena.counts;
}
}
int main(int argc,char** argv){try{
 check(argc==3);std::ifstream f(argv[1]);check(bool(f));Input input{};std::cout<<"{\"results\":[";bool comma=false;
 while(f>>input[0]>>input[1]>>input[2]>>input[3]>>input[4]>>input[5]){Fixture fixture;fixture.input=input;if(input[0]==3||input[0]==4)fixture.q.dispatch_0=dispatch(int(input[1]));Runtime runtime(fixture.services());fixture.runtime=&runtime;Result result;Status status=input[0]==0||input[0]>=5?runtime.create(std::int32_t(input[1]),&result):input[0]<3?runtime.construct_base(fixture.q,&result):runtime.destroy(fixture.q,input[0]==4,&result);if(input[0]>=5&&status==Status::complete)status=runtime.destroy(fixture.q,input[0]==6,&result);if(comma)std::cout<<',';comma=true;projection(fixture,status);}
 std::cout<<"],\"actual_objectives\":";values(actual(argv[2]));auto before=checks;guards();std::cout<<",\"native_guard_checks\":"<<checks-before<<",\"native_checks\":"<<checks<<"}";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}return 0;}
