#include "../quest_condition_eval_v1.hpp"
#include "../quest_runtime_fields_v1.hpp"
#include "../../level-world/level_construction_fields.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
using namespace dh2::data::quest_condition_eval_v1;
namespace table=dh2::data::quest_table_bindings_v1;
namespace cl=dh2::data::quest_condition_list_v1;
namespace cf=dh2::data::quest_condition_factory_v1;
namespace qr=dh2::data::quest_runtime_fields_v1;
namespace {
unsigned checks=0;
void require(bool ok){++checks;if(!ok)throw std::runtime_error("condition Eval check "+std::to_string(checks));}
std::shared_ptr<std::vector<std::uint8_t>> file(const std::string& path){
 std::ifstream f(path,std::ios::binary|std::ios::ate);if(!f)throw std::runtime_error("cache missing");
 const auto size=f.tellg();auto bytes=std::make_shared<std::vector<std::uint8_t>>(std::size_t(size));f.seekg(0);
 if(!f.read(reinterpret_cast<char*>(bytes->data()),size))throw std::runtime_error("cache read");
 return bytes;
}
table::View definitions(const std::string& cache){
 auto packed=file(cache+"/v2quests_pyarray.bin"),names=file(cache+"/v2quests_pyarraynames.bin");table::Input input;
 require(!dh2_quests_open(&input.table,packed->data(),std::uint32_t(packed->size())));input.packed_owner=packed;
 input.names=names->data();input.names_size=names->size();input.names_owner=names;table::Owner owner;std::string error;
 require(owner.load(input,error));return owner.borrow();
}
std::array<std::int32_t,3> common(const cl::Definition& d){
 dh2_quest_span span{};table::Span bytes{};std::string error;
 if(!d.view.list_record(*d.list,d.index,&span,error)||!d.view.bytes(span,&bytes,error)||bytes.size!=12)throw std::runtime_error("definition resolution");
 std::array<std::int32_t,3> out{};for(unsigned i=0;i<3;++i){const auto* p=bytes.data+i*4;const std::uint32_t v=std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);std::memcpy(&out[i],&v,4);}return out;
}
struct Fixture {
 table::View view;Record record{0x10010000};qr::Record quest{0x10070000};
 dh2::level_construction_fields::State level{};
 std::uintptr_t character=0x10060000;
 PlayerRef player{0x10040000,&character};QuestStateRef quest_ref{quest.ref.identity,&quest.state_0};LevelRef level_ref{0x10050000,&level.level_list_index_3c};
 std::unique_ptr<Runtime> runtime;std::vector<std::array<long long,4>> trace;
 std::vector<int> args;bool reenter=false;Status nested=Status::complete;unsigned effects=0;
 explicit Fixture(table::View v):view(std::move(v)){}
 cl::Definition definition(unsigned row,unsigned index){return {view,view.list(*view.row(row),0),index};}
 int event(int op,long long a=0,long long b=0,long long c=0){
  trace.push_back({op,a,b,c});++effects;
  if(reenter){reenter=false;Result r;nested=runtime->evaluate(&r);}
  if(op==args[7]){record.fields.py_data_4=definition(unsigned(args[8]),unsigned(args[9]));record.comparator_8=args[10];quest.state_0=args[16];level.level_list_index_3c=args[16];}
  if(op==args[11]){if(args[12])throw std::runtime_error("declared provider failure");return 1;}return 0;
 }
 Services services(){Services s;s.context=this;
  s.application=[](void* raw,std::uintptr_t* out){auto& f=*static_cast<Fixture*>(raw);if(f.event(0))return 1;*out=0x99f72c;return 0;};
  s.player_manager=[](void* raw,std::uintptr_t application,std::uintptr_t* out){auto& f=*static_cast<Fixture*>(raw);if(f.event(1,application))return 1;*out=0x10020000;return 0;};
  s.local_player=[](void* raw,std::uintptr_t manager,std::int32_t ordinal,std::uint32_t character,PlayerRef** out){auto& f=*static_cast<Fixture*>(raw);if(f.event(2,manager,ordinal,character))return 1;*out=f.args[13]?nullptr:&f.player;return 0;};
  // Declared external leaf: this gate verifies caller arguments/order and
  // retained effects, not a substitute implementation of CompileQuests.
  s.quest_by_id=[](void* raw,std::uintptr_t character,std::int32_t id,std::int32_t difficulty,QuestStateRef** out){auto& f=*static_cast<Fixture*>(raw);if(f.event(3,character,id,difficulty))return 1;*out=f.args[6]?&f.quest_ref:nullptr;return 0;};
  s.current_level=[](void* raw,std::uintptr_t application,LevelRef** out){auto& f=*static_cast<Fixture*>(raw);if(f.event(4,application))return 1;*out=f.args[14]?nullptr:&f.level_ref;return 0;};return s;
 }
 void setup(const std::vector<int>& input){
  args=input;const auto d=definition(unsigned(args[0]),unsigned(args[1]));const auto words=common(d);
  cf::Services s;s.context=this;s.allocate=[](void* raw,std::uint32_t bytes,std::uint32_t tag,Record** out){auto& f=*static_cast<Fixture*>(raw);if((bytes!=8&&bytes!=12)||tag)throw std::runtime_error("factory request");*out=&f.record;return 0;};
  cf::Runtime factory(s);cf::Result result;if(factory.create(words[0],&result)!=cf::Status::complete||result.record!=&record)throw std::runtime_error("actual factory");
  record.fields.py_data_4=args[15]?cl::Definition{}:d;record.comparator_8=args[2];quest.state_0=args[3];level.level_list_index_3c=args[4];character=args[5]?0x10060000:0;runtime=std::make_unique<Runtime>(record,services());
 }
};
void output(const Fixture& f,const Result& r){
 int row=-1;if(f.record.fields.py_data_4.list)for(unsigned i=0;i<f.view.count();++i)if(f.view.list(*f.view.row(i),0)==f.record.fields.py_data_4.list)row=int(i);
 std::cout<<"{\"status\":"<<unsigned(r.status)<<",\"value\":"<<r.value<<",\"calls\":"<<r.service_calls<<",\"definition\":["<<row<<','<<(row<0?-1:int(f.record.fields.py_data_4.index))<<"],\"comparator\":"<<f.record.comparator_8<<",\"state\":"<<f.quest.state_0<<",\"level\":"<<f.level.level_list_index_3c<<",\"character\":"<<f.character<<",\"effects\":"<<f.effects<<",\"trace\":[";
 bool comma=false;for(const auto& t:f.trace){if(comma)std::cout<<',';comma=true;std::cout<<'['<<t[0]<<','<<t[1]<<','<<t[2]<<','<<t[3]<<']';}std::cout<<"]}";
}
void guards(table::View view){
 const std::vector<int> base{0,1,0,0,0,1,1,-1,0,0,0,-1,0,0,0,0,0};
 for(unsigned missing=0;missing<5;++missing){Fixture f(view);f.setup(base);auto s=f.services();if(missing==0)s.application=nullptr;if(missing==1)s.player_manager=nullptr;if(missing==2)s.local_player=nullptr;if(missing==3)s.quest_by_id=nullptr;if(missing==4){f.record.dispatch_0=cf::Dispatch::player_in_level;s.current_level=nullptr;}Runtime r(f.record,s);Result out;require(r.evaluate(&out)==Status::service_unavailable);}
 {Fixture f(view);f.setup(base);f.reenter=true;Result out;require(f.runtime->evaluate(&out)==Status::complete&&f.nested==Status::reentrant);}
 {Fixture f(view);f.setup(base);require(f.runtime->evaluate(reinterpret_cast<Result*>(&f.record))==Status::invalid_argument);require(f.trace.empty()&&f.record.fields.py_data_4.list);require(f.runtime->evaluate(reinterpret_cast<Result*>(f.runtime.get()))==Status::invalid_argument);}
 {Fixture f(view);f.setup(base);f.record.ref.fields=nullptr;Result out;require(f.runtime->evaluate(&out)==Status::projection_changed&&f.trace.empty());}
 {Fixture f(view);f.setup(base);f.record.dispatch_0=cf::Dispatch::event_in_state;Result out;require(f.runtime->evaluate(&out)==Status::outside_domain&&f.trace.empty());}
 {Fixture f(view);auto input=base;input[15]=1;input[5]=0;f.setup(input);Result out;require(f.runtime->evaluate(&out)==Status::complete&&!out.value&&f.trace.size()==3);}
 for(unsigned nullkind=0;nullkind<3;++nullkind){Fixture f(view);f.setup(base);if(nullkind==0)f.args[13]=1;if(nullkind==1)f.player.character_660=nullptr;if(nullkind==2)f.quest_ref.state_0=nullptr;Result out;require(f.runtime->evaluate(&out)==Status::source_fault);}
 {Fixture f(view);f.setup(base);f.record.dispatch_0=cf::Dispatch::player_in_level;f.args[14]=1;Result out;require(f.runtime->evaluate(&out)==Status::source_fault&&f.trace.size()==2);}
 {Fixture f(view);f.setup(base);f.record.fields.py_data_4={};Result out;require(f.runtime->evaluate(&out)==Status::source_fault&&f.trace.size()==3);}
 {Fixture f(view);f.setup(base);auto forged=*f.record.fields.py_data_4.list;f.record.fields.py_data_4.list=&forged;Result out;require(f.runtime->evaluate(&out)==Status::source_fault&&f.trace.size()==3);}
 // Controls/backing delivered by a provider must not receive Result bytes.
 {Fixture f(view);f.setup(base);const auto before=f.player;require(f.runtime->evaluate(reinterpret_cast<Result*>(&f.player))==Status::invalid_argument);require(f.player.identity==before.identity&&f.player.character_660==before.character_660);}
 {Fixture f(view);f.setup(base);const auto before=f.quest_ref;require(f.runtime->evaluate(reinterpret_cast<Result*>(&f.quest_ref))==Status::invalid_argument);require(f.quest_ref.identity==before.identity&&f.quest_ref.state_0==before.state_0);}
 {Fixture f(view);f.setup(base);f.record.dispatch_0=cf::Dispatch::player_in_level;const auto before=f.level_ref;require(f.runtime->evaluate(reinterpret_cast<Result*>(&f.level_ref))==Status::invalid_argument);require(f.level_ref.identity==before.identity&&f.level_ref.level_list_index_3c==before.level_list_index_3c);}
 {Fixture f(view);f.setup(base);const auto* list=f.record.fields.py_data_4.list;const auto before=*list;require(f.runtime->evaluate(reinterpret_cast<Result*>(const_cast<table::ListRef*>(list)))==Status::invalid_argument);require(list->row==before.row&&list->kind==before.kind&&list->definition==before.definition);}
 {Fixture f(view);f.setup(base);const auto* row=f.record.fields.py_data_4.list->row;const auto before=row->identity;require(f.runtime->evaluate(reinterpret_cast<Result*>(const_cast<table::PyDataRef*>(row)))==Status::invalid_argument);require(row->identity==before);}
 {Fixture f(view);f.setup(base);const auto* backing=f.record.fields.py_data_4.list->definition;const auto before=*backing;require(f.runtime->evaluate(reinterpret_cast<Result*>(const_cast<dh2_quest_list*>(backing)))==Status::invalid_argument);require(backing->count==before.count&&backing->offset==before.offset);}
 {Fixture f(view);f.setup(base);auto s=f.services();s.quest_by_id=[](void* raw,std::uintptr_t,std::int32_t,std::int32_t,QuestStateRef**){auto& f=*static_cast<Fixture*>(raw);f.quest.state_0=9;f.record.ref.identity=0;return 0;};Runtime changed(f.record,s);Result out;require(changed.evaluate(&out)==Status::projection_changed&&f.quest.state_0==9);}
 // Real ConditionList builds every shipped list through the actual factory;
 // its virtual Eval dispatch borrows each same Record and canonical backing.
 for(unsigned row=0;row<view.count();++row){Fixture f(view);f.setup(base);cl::List list;std::vector<std::unique_ptr<Record>> arena;std::unique_ptr<cl::Array> array;
  cl::Services s;
  // Noncapturing callbacks borrow one context with genuine allocator leases.
  struct Composition{Fixture* f;std::vector<std::unique_ptr<Record>>* arena;std::unique_ptr<cl::Array>* array;} c{&f,&arena,&array};s.context=&c;
  s.allocate=[](void* raw,cl::List&,std::uint32_t bytes,std::uint32_t tag,cl::Array** out){auto& c=*static_cast<Composition*>(raw);if(tag)return 1;*c.array=std::make_unique<cl::Array>();(*c.array)->identity=0x10090000;(*c.array)->slots.resize(bytes/4);*out=c.array->get();return 0;};
  s.factory=[](void* raw,cl::List&,std::int32_t kind,cl::ConditionRef** out){auto& c=*static_cast<Composition*>(raw);cf::Services fs;fs.context=&c;fs.allocate=[](void* opaque,std::uint32_t,std::uint32_t,Record** dest){auto& p=*static_cast<Composition*>(opaque);p.arena->push_back(std::make_unique<Record>(0x100a0000+p.arena->size()*0x100));*dest=p.arena->back().get();return 0;};cf::Runtime factory(fs);cf::Result r;if(factory.create(kind,&r)!=cf::Status::complete)return 1;*out=&r.record->ref;return 0;};
  s.eval_virtual8=[](void* raw,cl::List&,cl::ConditionRef* condition,std::uint32_t* out){auto& c=*static_cast<Composition*>(raw);Record* record=nullptr;for(auto& candidate:*c.arena)if(&candidate->ref==condition)record=candidate.get();if(!record)return 1;Runtime eval(*record,c.f->services());Result r;if(eval.evaluate(&r)!=Status::complete)return 1;*out=r.value;return 0;};
  cl::Runtime owner(list,s);cl::Result r;const auto d=f.definition(row,0);require(owner.assign_pydata(d,std::int32_t(d.list->definition->count),&r)==cl::Status::complete);require(owner.evaluate(&r)==cl::Status::complete);require(arena.size()==d.list->definition->count);
 }
}
}
int main(int argc,char** argv){try{if(argc!=3)return 2;auto view=definitions(argv[1]);std::ifstream input(argv[2]);std::string line;std::cout<<"{\"results\":[";bool comma=false;
 while(std::getline(input,line)){std::istringstream stream(line);std::vector<int> row;int value;while(stream>>value)row.push_back(value);if(row.size()!=17)throw std::runtime_error("Eval input width");Fixture f(view);f.setup(row);Result r;f.runtime->evaluate(&r);if(comma)std::cout<<',';comma=true;output(f,r);}guards(view);std::cout<<"],\"native_checks\":"<<checks<<"}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what();return 1;}}
