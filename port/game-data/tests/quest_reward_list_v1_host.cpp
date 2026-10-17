#include "../quest_reward_list_v1.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
using namespace dh2::data::quest_reward_list_v1;
using Table=dh2::data::quest_table_bindings_v1::Owner;
namespace {
constexpr std::uintptr_t OLD=0x10003000,NEW=0x10004000,OTHER=0x10004500,STUB=0x10002000;
using Input=std::array<std::int64_t,8>;using Event=std::array<std::int64_t,4>;
unsigned checks=0;void check(bool v){if(!v)throw std::runtime_error("reward guard "+std::to_string(checks+1));++checks;}
void word(std::vector<std::uint8_t>& v,std::uint32_t n){for(unsigned i=0;i<4;++i)v.push_back(std::uint8_t(n>>(8*i)));}
Definition definition(){
 auto raw=std::make_shared<std::vector<std::uint8_t>>();word(*raw,1);for(unsigned i=0;i<4;++i)word(*raw,0);
 word(*raw,0);word(*raw,0);
 for(unsigned kind=0;kind<3;++kind){word(*raw,64);for(unsigned i=0;i<64;++i){word(*raw,i%5);word(*raw,100+i);word(*raw,200+i);}}
 for(unsigned i=0;i<2;++i){for(unsigned j=0;j<8;++j)word(*raw,0);}
 word(*raw,0);raw->push_back(0);word(*raw,0);for(unsigned i=0;i<14;++i)word(*raw,0);word(*raw,0);word(*raw,1);
 auto names=std::make_shared<std::vector<std::uint8_t>>();word(*names,1);word(*names,1);names->push_back('R');
 dh2_quest_table table{};check(!dh2_quests_open(&table,raw->data(),std::uint32_t(raw->size())));Table owner;std::string error;
 check(owner.load({table,raw,names->data(),names->size(),names},error));auto view=owner.borrow();return {view,view.list(*view.row(0),2),0};
}
struct Object {RewardFields fields;RewardRef ref;int phase=0;Object(std::uintptr_t id):ref{id,&fields}{fields.type_4=-1;fields.character_10=0xcccccccc;}};
struct Fixture {
 List list;Definition data;Input input{};std::map<std::uintptr_t,std::unique_ptr<Array>> arrays;std::map<std::uintptr_t,std::unique_ptr<Object>> objects;
 std::vector<Event> trace;unsigned factories=0;bool text_alive=true;Runtime* runtime=nullptr;Status nested=Status::complete;
 explicit Fixture(const Definition& d):data(d){}
 Object& object(std::uintptr_t id){auto& p=objects[id];if(!p){p=std::make_unique<Object>(id);p->fields.py_data_c={data.view,data.list,50};}return *p;}
 Array& array(std::uintptr_t id,unsigned length){auto& p=arrays[id];if(!p){p=std::make_unique<Array>();p->identity=id;p->slots.resize(length);for(unsigned i=0;i<length;++i)p->slots[i]=&object(id+0x4000+i*0x100).ref;}return *p;}
 void setup(const Input& row){input=row;list.count_0=std::int32_t(row[1]);list.py_data_20={data.view,data.list,5};list.text_8.assign(std::size_t(row[7]),'x');
  if(row[2]>=0)list.children_4=&array(OLD,unsigned(row[2]));
  array(OTHER,12);
 }
 bool event(unsigned op,std::int64_t a=0,std::int64_t b=0,std::int64_t c=0){trace.push_back({op,a,b,c});if(input[5]>0&&trace.size()==std::size_t(input[5])){if(input[6])throw std::runtime_error("provider");return false;}return true;}
 Services services(){return {this,
  [](void* raw,List&)->std::int32_t{auto& f=*static_cast<Fixture*>(raw);return f.event(1,16)?0:1;},
  [](void* raw,List& list)->std::int32_t{auto& f=*static_cast<Fixture*>(raw);if(!f.event(2))return 1;list.text_8.clear();if(f.input[4]==8)list.count_0=2;if(f.input[4]==9){Result out;f.nested=f.runtime->assign_pydata(f.data,1,&out);}return 0;},
  [](void* raw,List& list,std::uint32_t bytes,std::uint32_t tag,Array** out)->std::int32_t{auto& f=*static_cast<Fixture*>(raw);if(!f.event(3,bytes,tag,NEW))return 1;
   auto a=std::make_unique<Array>();a->identity=NEW;a->slots.resize(bytes/4,nullptr);f.arrays[NEW]=std::move(a);*out=f.arrays[NEW].get();if(f.input[4]==1)list.count_0=0;if(f.input[4]==2)list.count_0=1;return 0;},
  [](void* raw,List& list,std::int32_t type,RewardRef** out)->std::int32_t{auto& f=*static_cast<Fixture*>(raw);const auto id=0x10010000+f.factories*0x100;
   if(!f.event(5,type,id))return 1;
   auto& q=f.object(id);q.phase=1;*out=&q.ref;++f.factories;
   if(f.input[4]==3)list.children_4=f.arrays[OTHER].get();
   if(f.input[4]==4)list.count_0=1;
   return 0;},
  [](void* raw,List& list,RewardRef* ref)->std::int32_t{auto& f=*static_cast<Fixture*>(raw);if(!f.event(10,ref->identity))return 1;f.object(ref->identity).phase=2;
   if(f.input[4]==5)list.count_0=1;
   if(f.input[4]==6)list.children_4=f.arrays[OTHER].get();
   return 0;},
  [](void* raw,List& list,Array* a)->std::int32_t{auto& f=*static_cast<Fixture*>(raw);if(!f.event(11,a->identity))return 1;if(f.input[4]==7)list.children_4=f.arrays[OTHER].get();return 0;},
  [](void* raw,List& list)->std::int32_t{auto& f=*static_cast<Fixture*>(raw);if(!f.event(12))return 1;f.text_alive=false;list.text_8.clear();return 0;}
 };}
 Status run(){Runtime r(list,services());runtime=&r;Result out;
  switch(input[0]){case 0:case 4:return r.construct(&out);case 1:return r.assign_pydata(data,std::int32_t(input[1]),&out);case 2:return r.set_owner(std::uintptr_t(input[3]),&out);default:return r.destroy(&out);}}
};
template<class A>void print_array(const A& values){std::cout<<'[';bool first=true;for(const auto& value:values){if(!first)std::cout<<',';first=false;std::cout<<value;}std::cout<<']';}
void print(const Fixture& f,Status s){
 std::cout<<"{\"status\":"<<unsigned(s)<<",\"fields\":";print_array(std::array<std::int64_t,5>{f.list.count_0,std::int64_t(f.list.children_4?f.list.children_4->identity:0),std::int64_t(f.list.py_data_20.list?STUB+f.list.py_data_20.index*16:0),std::int64_t(f.text_alive?f.list.text_8.size():0),f.text_alive?1:0});
 std::cout<<",\"arrays\":[";bool first=true;for(const auto& pair:f.arrays){if(!first)std::cout<<',';first=false;std::cout<<'['<<pair.first<<",[";bool a=true;for(const auto* q:pair.second->slots){if(!a)std::cout<<',';a=false;std::cout<<(q?q->identity:0);}std::cout<<"]]";}std::cout<<"],\"objects\":[";
 first=true;for(const auto& pair:f.objects){if(!first)std::cout<<',';first=false;const auto& q=*pair.second;print_array(std::array<std::int64_t,5>{std::int64_t(pair.first),q.fields.type_4,std::int64_t(q.fields.py_data_c.list?STUB+q.fields.py_data_c.index*16:0),std::int64_t(q.fields.character_10),q.phase});}
 std::cout<<"],\"trace\":[";first=true;for(const auto& e:f.trace){if(!first)std::cout<<',';first=false;print_array(e);}std::cout<<"]}";
}
void guards(const Definition& d){
 const Input in{1,3,-1,123,0,0,0,20};
 {Fixture f(d);f.setup(in);Runtime r(f.list,{});Result out;check(r.assign_pydata(d,3,&out)==Status::service_unavailable);check(f.list.count_0==3&&f.list.py_data_20.index==0&&f.list.text_8.size()==20);}
 {Fixture f(d);f.setup(in);Runtime r(f.list,f.services());check(r.construct(nullptr)==Status::invalid_argument&&f.trace.empty());}
 {Fixture f(d);f.setup(in);Runtime r(f.list,f.services());check(r.construct(reinterpret_cast<Result*>(&f.list))==Status::invalid_argument&&f.trace.empty());}
 {Fixture f(d);auto row=in;row[4]=9;f.setup(row);check(f.run()==Status::complete&&f.nested==Status::reentrant);}
 {Fixture f(d);auto row=in;row[0]=2;row[2]=3;f.setup(row);f.list.children_4->slots[1]=nullptr;check(f.run()==Status::source_fault);check(f.list.children_4->slots[0]->fields->character_10==123);}
 {Fixture f(d);auto row=in;row[0]=2;row[2]=1;f.setup(row);check(f.run()==Status::source_fault);check(f.list.children_4->slots[0]->fields->character_10==123);}
 {Fixture f(d);auto row=in;row[0]=2;row[2]=3;row[3]=INT64_C(0x12345678000000a1);f.setup(row);check(f.run()==Status::complete);for(auto* q:f.list.children_4->slots)check(q->fields->character_10==std::uintptr_t(row[3]));}
 {Fixture f(d);f.setup(in);Definition bad=d;bad.list=nullptr;Runtime r(f.list,f.services());Result out;check(r.assign_pydata(bad,3,&out)==Status::source_fault);check(f.list.children_4&&f.list.count_0==3&&f.list.text_8.empty());}
 {Fixture f(d);auto row=in;row[0]=3;row[1]=0;row[2]=-1;f.setup(row);auto s=f.services();s.text_destroy=nullptr;Runtime r(f.list,s);Result out;check(r.destroy(&out)==Status::service_unavailable);}
}
}
int main(int argc,char** argv){try{check(argc==2);const auto d=definition();std::ifstream in(argv[1]);check(bool(in));std::cout<<"{\"results\":[";bool first=true;Input row;
 while(in>>row[0]){for(unsigned i=1;i<8;++i)check(bool(in>>row[i]));Fixture f(d);f.setup(row);const auto status=f.run();if(!first)std::cout<<',';first=false;print(f,status);}
 const auto before=checks;guards(d);std::cout<<"],\"native_guard_checks\":"<<checks-before<<"}\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
