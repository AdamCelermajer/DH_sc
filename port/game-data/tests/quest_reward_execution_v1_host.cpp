#include "../quest_reward_execution_v1.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
using namespace dh2::data;using namespace dh2::data::quest_reward_execution_v1;
namespace {
constexpr std::uintptr_t ID=0x10000100,CHAR0=0x10004000,CHAR1=0x10007000,STUB=0x10002000;
using Input=std::array<std::int64_t,10>;using Event=std::array<std::int64_t,4>;
unsigned checks=0,guard_checks=0;void check(bool v){if(!v)throw std::runtime_error("reward execution guard "+std::to_string(checks+1));++checks;}
void guarded(bool v){check(v);++guard_checks;}
void word(std::vector<std::uint8_t>& v,std::uint32_t n){for(unsigned i=0;i<4;++i)v.push_back(std::uint8_t(n>>(8*i)));}
std::vector<std::uint8_t> file(const std::string& path){std::ifstream f(path,std::ios::binary);check(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
quest_reward_list_v1::Definition definition(const Input& input){
 auto raw=std::make_shared<std::vector<std::uint8_t>>();word(*raw,1);for(unsigned i=0;i<4;++i)word(*raw,0);word(*raw,0);word(*raw,0);
 for(unsigned kind=0;kind<3;++kind){word(*raw,2);for(unsigned i=0;i<2;++i){word(*raw,std::uint32_t(input[1]));word(*raw,std::uint32_t(input[3+i]));word(*raw,std::uint32_t(input[5]));}}
 for(unsigned i=0;i<2;++i)for(unsigned j=0;j<8;++j)word(*raw,0);
 word(*raw,0);raw->push_back(0);word(*raw,0);for(unsigned i=0;i<14;++i)word(*raw,0);word(*raw,0);word(*raw,0);
 auto names=std::make_shared<std::vector<std::uint8_t>>();word(*names,1);word(*names,1);names->push_back('R');dh2_quest_table table{};check(!dh2_quests_open(&table,raw->data(),std::uint32_t(raw->size())));quest_table_bindings_v1::Owner owner;std::string error;check(owner.load({table,raw,names->data(),names->size(),names},error));auto view=owner.borrow();return {view,view.list(*view.row(0),2),0};
}
struct Character {PropertyState properties;std::unique_ptr<FreshInventoryOwnedV4> inventory;std::int32_t gold_receipt=-50,xp_receipt=-70;CharacterRef ref;};
struct Fixture {
 Record q{ID};Input input;quest_reward_list_v1::Definition data;std::array<Character,2> characters;std::vector<Event> trace;Runtime* runtime=nullptr;Status nested=Status::complete;
 Fixture(Input in,LootTablesV2::Borrow tables):input(in),data(definition(in)){
  q.dispatch_0=quest_reward_factory_v1::Dispatch(std::uint32_t(input[1])+2);q.compiled_8=std::uint8_t(input[2]);q.fields.py_data_c=data;q.compiled_py_data_14={data.view,data.list,input[0]==0?1u:0u};q.fields.character_10=CHAR0;
  OwnedInventoryServicesV4 initial;initial.invoke=[](void*,FreshInventoryOwnedV4&,const OwnedInventoryRequestV4&,OwnedInventoryResponseV4&,std::string&)->bool{return true;};
  for(unsigned i=0;i<2;++i){auto& c=characters[i];const auto id=i?CHAR1:CHAR0;c.inventory=std::make_unique<FreshInventoryOwnedV4>(id,tables,InventoryRandomServiceV4{},10,c.properties);c.inventory->project_gold_limit(5000);std::string error;check(c.inventory->set_gold(i?2000:1000,initial,error));c.ref={id,c.inventory.get(),&c.gold_receipt,&c.xp_receipt};}
 }
 void mutate(){if(input[6]==1){q.compiled_py_data_14={data.view,data.list,1};q.fields.character_10=CHAR1;}if(input[6]==2)q.compiled_8=0;}
 bool event(int op,std::int64_t a=0,std::int64_t b=0,std::int64_t c=0){trace.push_back({op,a,b,c});mutate();if(input[8]>0&&trace.size()==std::size_t(input[8])){if(input[9])throw std::runtime_error("declared effect exception");return false;}return true;}
 Services services(){return {this,
 [](void* raw,std::uintptr_t id)->CharacterRef*{auto& f=*static_cast<Fixture*>(raw);for(auto& c:f.characters)if(c.ref.identity==id)return &c.ref;return nullptr;},
 [](void* raw,Record& q,CharacterRef& c,std::int32_t amount)->std::int32_t{auto& f=*static_cast<Fixture*>(raw);OwnedInventoryServicesV4 effects;effects.context=&f;effects.invoke=[](void* raw,FreshInventoryOwnedV4& inventory,const OwnedInventoryRequestV4& request,OwnedInventoryResponseV4&,std::string&)->bool{auto& f=*static_cast<Fixture*>(raw);check(request.operation==OwnedInventoryOperationV4::gold_notifications);return f.event(1,inventory.character(),inventory.gold());};InventoryEffects binding{&effects};return inventory_add_gold(&binding,q,c,amount);},
 [](void* raw,Record&,CharacterRef& c,std::int32_t fixed,std::uint8_t flag,bool* granted)->std::int32_t{auto& f=*static_cast<Fixture*>(raw);if(!f.event(2,c.identity,fixed,flag))return 1;*granted=f.input[7]!=0;return 0;}};}
};
template<class T>void values(const T& v){std::cout<<'[';bool comma=false;for(auto n:v){if(comma)std::cout<<',';comma=true;std::cout<<n;}std::cout<<']';}
void projection(const Fixture& f,Status status,const Result& result){std::cout<<"{\"status\":"<<unsigned(status)<<",\"value\":"<<result.value<<",\"fields\":";values(std::array<std::int64_t,3>{f.q.compiled_8,std::int64_t(STUB+f.q.compiled_py_data_14.index*16),std::int64_t(f.q.fields.character_10)});std::cout<<",\"characters\":[";for(unsigned i=0;i<2;++i){if(i)std::cout<<',';const auto& c=f.characters[i];values(std::array<std::int64_t,3>{c.inventory->gold(),c.gold_receipt,c.xp_receipt});}std::cout<<"],\"trace\":[";bool comma=false;for(const auto& event:f.trace){if(comma)std::cout<<',';comma=true;values(event);}std::cout<<"]}";}
void guards(LootTablesV2::Borrow tables){
 Input input{1,0,1,5,11,1,0,1,0,0};
 {Fixture f(input,tables);Runtime r(f.q,{});Result out;guarded(r.give_gold(&out)==Status::service_unavailable);f.q.compiled_8=0;guarded(r.give_gold(&out)==Status::complete&&out.value==0);guarded(r.compile(nullptr)==Status::invalid_argument);}
 {Fixture f(input,tables);Runtime r(f.q,f.services());Result out;f.q.ref.fields=nullptr;guarded(r.compile(&out)==Status::projection_changed);}
 {Fixture f(input,tables);Runtime r(f.q,f.services());guarded(r.compile(reinterpret_cast<Result*>(&f.q))==Status::invalid_argument);}
 {Fixture f(input,tables);f.q.dispatch_0=quest_reward_factory_v1::Dispatch::consume_loot;f.q.fields.py_data_c={};Runtime r(f.q,f.services());Result out;guarded(r.compile(&out)==Status::source_fault&&f.q.compiled_8==1);}
 {Fixture f(input,tables);f.q.compiled_py_data_14={};Runtime r(f.q,f.services());Result out;guarded(r.give_gold(&out)==Status::source_fault&&f.trace.empty());}
 {Fixture f(input,tables);f.characters[0].ref.reward_gold_1500=nullptr;Runtime r(f.q,f.services());Result out;guarded(r.give_gold(&out)==Status::source_fault&&f.characters[0].inventory->gold()==1005);}
 {Fixture f(input,tables);auto s=f.services();s.resolve_character=[](void*,std::uintptr_t)->CharacterRef*{return nullptr;};Runtime r(f.q,s);Result out;guarded(r.give_gold(&out)==Status::source_fault&&f.trace.empty());}
 {Fixture f(input,tables);auto s=f.services();s.add_gold=[](void* raw,Record&,CharacterRef&,std::int32_t)->std::int32_t{auto& x=*static_cast<Fixture*>(raw);Result nested;x.nested=x.runtime->give_gold(&nested);return 0;};Runtime r(f.q,s);f.runtime=&r;Result out;guarded(r.give_gold(&out)==Status::complete&&f.nested==Status::reentrant);}
 {Fixture f(input,tables);Runtime r(f.q,f.services());guarded(r.give_gold(reinterpret_cast<Result*>(&f.characters[0].gold_receipt))==Status::invalid_argument&&f.trace.empty());}
 {Fixture f(input,tables);auto s=f.services();s.add_gold=nullptr;Runtime r(f.q,s);Result out;guarded(r.give_gold(&out)==Status::service_unavailable&&f.trace.empty());}
 {Fixture f(input,tables);f.q.dispatch_0=quest_reward_factory_v1::Dispatch::xp;auto s=f.services();s.give_xp=nullptr;Runtime r(f.q,s);Result out;guarded(r.give_xp(&out)==Status::service_unavailable&&f.trace.empty());}
 {Fixture f(input,tables);f.q.dispatch_0=quest_reward_factory_v1::Dispatch::base;Runtime r(f.q,f.services());Result out;guarded(r.compile(&out)==Status::source_fault);}
}
}
int main(int argc,char** argv){try{check(argc==3);const auto path=std::string(argv[2]);const auto records=file(path+"/loot_table_pyarray.bin"),names=file(path+"/loot_table_pyarraynames.bin"),schema=file(path+"/loot_table_pystructnames.bin");LootTablesV2 tables;std::string error;check(tables.load({records.data(),records.size()},{names.data(),names.size()},{schema.data(),schema.size()},error));std::ifstream f(argv[1]);check(bool(f));Input input{};std::cout<<"{\"results\":[";bool comma=false;while(f>>input[0]>>input[1]>>input[2]>>input[3]>>input[4]>>input[5]>>input[6]>>input[7]>>input[8]>>input[9]){Fixture fixture(input,tables.borrow());Runtime runtime(fixture.q,fixture.services());Result result;const auto status=input[0]==0?runtime.compile(&result):input[0]==1?runtime.give_gold(&result):runtime.give_xp(&result);if(comma)std::cout<<',';comma=true;projection(fixture,status,result);}guards(tables.borrow());std::cout<<"],\"native_guard_checks\":"<<guard_checks<<",\"native_checks\":"<<checks<<"}";}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}return 0;}
