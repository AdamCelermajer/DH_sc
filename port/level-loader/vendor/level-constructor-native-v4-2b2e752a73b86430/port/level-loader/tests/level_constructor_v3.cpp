#include "../level_constructor_v3.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace dh2::loader;
struct Fixture {
 LevelConstructorFieldsV3 f;
 std::uintptr_t config=0xa5a5a5a5;
 std::int32_t music=123,safe=123,ambient=123,module=123;
 std::uint32_t gate=123,debug=0,global=123;
 float offset[3]{1,2,3};
 dh2::data::LevelTables levels;
 std::shared_ptr<int> lifetime=std::make_shared<int>(1),script=std::make_shared<int>(2),save=std::make_shared<int>(3);
 std::vector<std::string> trace;
 std::string script_path="poison";
 std::vector<std::uint32_t> save_args;
 int online{},hosting{},pm{},state{},mutation{};
 Fixture(){
  f.phase30=f.party_dc=f.word_e0=f.phase_e4=f.seed114=0xa5a5a5a5;
  f.row3c=f.difficulty40=f.level110=f.mode118=f.field148=-123;
  for(auto p:{&f.field128,&f.field12c,&f.field130,&f.field134,&f.field138,&f.field13c,&f.field140,&f.field14c,&f.field154,&f.field158,&f.field15c,&f.field194,&f.field19c,&f.field1a0,&f.field1a4})*p=0xa5a5a5a5;
  for(auto p:{&f.byte_e8,&f.byte_f0,&f.byte_f1,&f.byte_f2,&f.byte_f3,&f.byte_f4,&f.byte_f5,&f.byte144,&f.byte145,&f.byte16c,&f.byte198,&f.byte1a8})*p=0xa5;
  f.save_ec=save;f.script44=save;f.name_f8="poison";
 }
 LevelConstructorBorrowV3 borrow(){return {lifetime,0x1234,&f,&config,&music,&safe,&ambient,&gate,offset,&module};}
 LevelConstructorServicesV3 services(){
  LevelConstructorServicesV3 s;s.application=lifetime;s.debug_level_load_count=&debug;s.module_id_global=&global;s.levels=&levels;
  s.construct_script=[this](auto id,bool deferred,auto& out,auto&){
   assert(id==0x1234&&!deferred&&f.events&&f.events->identity()==id&&f.events->pending_count()==0&&f.events->receiver_records_v12().empty());
   trace.push_back("event");trace.push_back("script:0");out=script;
   if(mutation&1){f.phase30=17;f.difficulty40=91;}return true;
  };
  s.script_assign_path=[this](auto& receiver,const char* name,std::size_t n,auto&){assert(receiver==script&&n==13);script_path.assign(name,n);return true;};
  s.script_load=[this](auto& receiver,const char* name,auto&){
   assert(receiver==script&&script_path=="data/scripts/");trace.push_back(std::string("load:")+name);
   if((mutation&2)&&std::string(name)=="level/combat_formulas"){f.seed114=123;f.phase30=18;f.name_f8="worlds/crypt01.dwld";}return true;
  };
  s.online_byte5=[this](auto& value,auto&){trace.push_back("online");value=online;if(mutation&8)f.row3c=-1;return true;};
  s.local_player_hosting=[this](auto& value,auto&){trace.push_back("hosting");value=hosting==1;return true;};
  s.player_manager_byte719=[this](auto& value,auto&){value=pm;return true;};
  s.online_state34=[this](auto& value,auto&){trace.push_back("state");value=state;return true;};
  s.matching_is_host=[this](auto& value,auto&){trace.push_back("matching_get");trace.push_back("matching_host");value=hosting==2;return true;};
  s.allocate_save=[this](auto& out,auto&){trace.push_back("allocate:60:0");out=save;if(mutation&4){f.seed114=234;f.difficulty40=8;f.row3c=9;f.mode118=5;}return true;};
  s.construct_save=[this](auto& receiver,const auto& a,auto&){assert(receiver==save&&a.level_identity==0x1234&&!a.source_flag);
   trace.push_back("save");save_args={a.seed,std::uint32_t(a.difficulty),std::uint32_t(a.row),std::uint32_t(a.mode),std::uint32_t(a.source_flag)};return true;};
  return s;
 }
 void table(int which){
  if(which==0)return;
  if(which==2){dh2::data::LevelRecord r;r.file="";r.scalar.words[4]=0xffffffff;r.scalar.words[5]=0x1234;levels.levels.push_back(r);}
  for(auto p:std::vector<std::pair<std::string,int>>{{"SWAMP",0},{"crypt",3},{"swamp",2}}){dh2::data::LevelRecord r;r.file=p.first;r.scalar.words[4]=p.second;r.scalar.words[5]=0x1234+levels.levels.size();levels.levels.push_back(r);}
 }
 std::vector<std::uint32_t> projection(){
  auto bits=[](auto x){return std::uint32_t(x);};
  return {f.phase30,bits(config),bits(f.row3c),bits(f.difficulty40),f.party_dc,f.word_e0,f.phase_e4,
   f.byte_e8,bits(bool(f.save_ec)),f.byte_f0,f.byte_f1,f.byte_f2,f.byte_f3,f.byte_f4,f.byte_f5,
   bits(f.level110),f.seed114,bits(f.mode118),bits(music),bits(safe),bits(ambient),
   bits(f.field128),bits(f.field12c),bits(f.field130),bits(f.field134),bits(f.field138),bits(f.field13c),bits(f.field140),
   f.byte144,f.byte145,bits(f.field148),bits(f.field14c),gate,bits(f.field154),bits(f.field158),bits(f.field15c),
   bits(offset[0]),bits(offset[1]),bits(offset[2]),f.byte16c,bits(module),bits(f.field194),f.byte198,bits(f.field19c),bits(f.field1a0),bits(f.field1a4),f.byte1a8,debug,global};
 }
};
template<class T> void array(const std::vector<T>& v){std::cout<<'[';bool first=true;for(auto& x:v){if(!first)std::cout<<',';first=false;std::cout<<x;}std::cout<<']';}
void strings(const std::vector<std::string>& v){std::cout<<'[';bool first=true;for(auto& x:v){if(!first)std::cout<<',';first=false;std::cout<<'"'<<x<<'"';}std::cout<<']';}
int failures(){
 unsigned checks=0;
 for(int missing=0;missing<13;++missing){
  Fixture f;f.table(1);f.online=1;f.hosting=2;f.state=3;
  auto s=f.services();
  switch(missing){case 0:s.construct_script={};break;case 1:s.script_load={};break;case 2:s.application.reset();break;case 3:s.debug_level_load_count=nullptr;break;case 4:s.module_id_global=nullptr;break;case 5:s.levels=nullptr;break;case 6:s.online_byte5={};break;case 7:s.local_player_hosting={};break;case 8:s.online_state34={};break;case 9:s.matching_is_host={};break;case 10:s.allocate_save={};break;case 11:s.construct_save={};break;case 12:s.script_assign_path={};break;}
  LevelConstructorV3 owner(f.borrow(),std::move(s));assert(!owner.construct({"worlds/swamp01.dwld",2,3,4,5,1,1,-1,0}));++checks;
  auto trace=f.trace;auto projection=f.projection();assert(!owner.construct({"worlds/swamp01.dwld",2,3,4,5,1,1,-1,0}));assert(f.trace==trace&&f.projection()==projection);++checks;
  assert(missing==0||!f.f.save_ec);++checks;
 }
 {Fixture f;f.table(1);auto s=f.services();LevelConstructorV3* entered=nullptr;
  s.construct_script=[&](auto,bool,auto& receiver,auto&){assert(!entered->construct({"nested",1,2,3,4,1,1,0,1}));receiver=f.script;return true;};
  LevelConstructorV3 owner(f.borrow(),std::move(s));entered=&owner;assert(owner.construct({"worlds/swamp01.dwld",2,3,4,5,1,1,-1,0}));assert(owner.error().empty());checks+=2;}
 {Fixture f;auto s=f.services();f.f.events=std::make_unique<dh2::events::EventManagerOwnerV12>(0x1234);LevelConstructorV3 owner(f.borrow(),std::move(s));assert(!owner.construct({"x",0,0,0,0,0,0,-1,0}));assert(f.trace.empty());checks+=2;}
 {Fixture f;f.table(1);auto s=f.services();s.allocate_save=[](auto&,auto&){return true;};LevelConstructorV3 owner(f.borrow(),std::move(s));assert(!owner.construct({"worlds/swamp01.dwld",2,3,4,5,1,1,-1,0}));assert(!f.f.save_ec);checks+=2;}
 {Fixture f;f.table(1);f.levels.levels[0].file=std::string(1024,'x');LevelConstructorV3 owner(f.borrow(),f.services());assert(!owner.construct({"x",0,0,0,0,0,0,-1,0}));assert(f.debug==1&&f.global==0);checks+=2;}
 {Fixture f;f.table(1);f.levels.levels[0].file=std::string("a\0b",3);LevelConstructorV3 owner(f.borrow(),f.services());assert(!owner.construct({"x",0,0,0,0,0,0,-1,0}));++checks;}
 std::cout<<"PASS failures "<<checks<<'\n';return 0;
}
int main(int argc,char** argv){
 if(argc==2&&std::string(argv[1])=="failures")return failures();
 assert(argc==2);std::ifstream input(argv[1]);assert(input);std::string line;
 const char* names[]={"worlds/swamp01.dwld","worlds/crypt01.dwld","SWAMP","absent","worlds/swamp01_crypt.dwld"};
 while(std::getline(input,line)){Fixture f;int table,name,requested,f1,f2;std::uint32_t debug;std::istringstream row(line);
  assert(bool(row>>table>>name>>requested>>f.online>>f.hosting>>f.pm>>f.state>>f1>>f2>>f.mutation>>debug));
  f.debug=debug;f.table(table);LevelConstructorV3 owner(f.borrow(),f.services());
  assert(owner.construct({names[name],-7,0xfedcba98,0x12345678,0x87654321,std::uint8_t(f1),std::uint8_t(f2),requested,6}));
  std::cout<<"{\"fields\":";array(f.projection());std::cout<<",\"events\":";strings(f.trace);std::cout<<",\"save_args\":";array(f.save_args);
  std::cout<<",\"name\":\""<<f.f.name_f8<<"\",\"path\":\""<<f.script_path<<"\"}\n";
 }
}
