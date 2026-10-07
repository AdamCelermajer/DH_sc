#include "../../level-world/character_level.hpp"
#include "../game_design_tables.hpp"
#include "../ai.hpp"
#include "../../level-world/character_property_bindings.hpp"
#include "../../script-runtime/script_constants.hpp"
#include "../../script-runtime/script_function_alias.h"
#include "../../script-runtime/script_scalar_bindings.h"
#include <array>
#include <vector>
#include <fstream>
#include <iterator>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <dlfcn.h>
using namespace dh2::character;
namespace {
unsigned checks=0,guards=0,vm_checks=0;
void check(bool x){++checks;if(!x)throw std::runtime_error("level check "+std::to_string(checks));}
unsigned word(std::istream& s){unsigned n;s.read(reinterpret_cast<char*>(&n),4);check(bool(s));return n;}
std::int32_t signed_word(unsigned u){std::int32_t x;std::memcpy(&x,&u,4);return x;}
float number(unsigned u){float x;std::memcpy(&x,&u,4);return x;}
using Sheets=std::array<dh2::data::PropertySheet,4>;
using Event=std::array<unsigned,9>;
struct Fixture {
 Sheets sheets{},host_sheets{};dh2::data::PropertyRules rules{};dh2::data::PropertyView view{},host_view{};
 std::vector<dh2::data::ClassRow> classes;LevelModel32 model{};LevelServices16 services{};dh2_script_design_bindings design{};LevelBindings48 bindings{};PropertyBindings48 prop{};
 std::int32_t cap[2]{100,100};unsigned lookups=0;std::vector<Event> trace;bool fail=false;
 dh2::data::CharacterTable table;std::vector<std::string> class_names;
};
void refresh(Fixture& f){f.view={f.rules.defaults.data(),f.rules.types.data(),f.sheets[0].data(),f.sheets[1].data(),f.sheets[2].data(),f.sheets[3].data(),nullptr,0};}
void event(Fixture& f,unsigned stage,unsigned property=0,unsigned delta=0){auto& s=f.sheets;f.trace.push_back({stage,property,delta,unsigned(s[0][19]),unsigned(s[3][19]),unsigned(s[3][36]),unsigned(s[3][38]),unsigned(s[3][41]),unsigned(s[3][43])});}
int debug(void* opaque,LevelModel32* model,const LevelRequest24* r){auto& f=*static_cast<Fixture*>(opaque);check(model==&f.model&&(r->property==36||r->property==41));check(!r->reserved&&r->retained_delta>0);if(r->service==level_debug_load)check(!r->name);else check(r->service==level_debug_query&&!std::strcmp(r->name,"isTracingChar_Stats"));event(f,r->service==level_debug_load?3:4,r->property,unsigned(r->retained_delta));return f.fail?1:0;}
std::vector<unsigned char> file(const std::string& name){std::ifstream f(name,std::ios::binary);check(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
dh2_script_value arg(float n){dh2_script_value a{};a.type=3;a.number=n;return a;}
int host_level(void* opaque,const dh2_script_value*,unsigned,dh2_script_value* out,unsigned cap,unsigned* count,char*,std::size_t){auto& f=*static_cast<Fixture*>(opaque);std::int32_t level;check(cap&&dh2_character_get_level(&level,&f.host_view)==0);out[0]=arg(float(level));*count=1;return 0;}
int world_input(void* opaque,const dh2_script_value*,unsigned,dh2_script_value* out,unsigned cap,unsigned* count,char*,std::size_t){auto id=reinterpret_cast<std::uintptr_t>(opaque);check(cap>=2);*count=0;if(id==1){out[0]=arg(10);out[1]=arg(20);*count=2;}else if(id==2){out[0]=arg(0);*count=1;}else if(id==3){out[0]=arg(-1);out[1]=arg(-1);*count=2;}else check(false);return 0;}
std::string origin(void* p){Dl_info d{};check(dladdr(p,&d)!=0);return d.dli_fname;}
void load(dh2_script_vm* vm,const std::string& code){check(dh2_script_vm_load(vm,code.data(),code.size(),"level-vm")==0);}
dh2_script_value get(dh2_script_vm* vm,const char* name){dh2_script_value v{};check(dh2_script_vm_get_global(vm,name,&v)==0);return v;}
std::string text(std::istream& in){auto n=word(in);check(n<4096);std::string s(n,'\0');in.read(s.data(),n);check(bool(in));return s;}
struct DesignFixture {
 std::vector<std::string> groups;
 std::vector<std::vector<std::string>> strings,original;
 std::vector<std::vector<const char*>> pointers;
 std::vector<dh2::data::DesignNames16> names;
 dh2::data::GameDesignTables registry;
 void refresh(unsigned i){pointers[i].resize(strings[i].size());for(unsigned j=0;j<strings[i].size();++j)pointers[i][j]=strings[i][j].c_str();names[i]={pointers[i].data(),unsigned(pointers[i].size()),0};}
 const std::vector<std::string>& values(const char* key){auto it=std::find(groups.begin(),groups.end(),key);check(it!=groups.end());return strings[unsigned(it-groups.begin())];}
};
unsigned replay_design(const char* path,DesignFixture& f){
 std::ifstream in(path,std::ios::binary);check(bool(in)&&word(in)==0x31544447);auto n=word(in),actions=word(in);check(n==6&&actions<10000);f.groups.resize(n);f.strings.resize(n);f.pointers.resize(n);f.names.resize(n);
 for(unsigned i=0;i<n;++i){f.groups[i]=text(in);auto original_getter=word(in),kind=word(in),count=word(in);check(original_getter&&kind<2&&count<65536);f.strings[i].resize(count);for(auto& s:f.strings[i])s=text(in);f.refresh(i);}
 f.original=f.strings;unsigned queries=0;
 for(unsigned i=0;i<actions;++i){auto op=word(in);if(op==0){auto index=word(in);auto group=text(in);std::string error;check(index<n&&f.registry.register_table(group.c_str(),&f.names[index],error));}
  else if(op==1){auto group=text(in),key=text(in);auto expected=word(in);auto view=f.registry.view();std::int32_t value;check(!dh2_game_design_tables_lookup(&view,1,group.c_str(),key.c_str(),&value)&&unsigned(value)==expected);++queries;}
  else if(op==2){auto index=word(in),row=word(in);auto s=text(in);check(index<n&&row<f.strings[index].size());f.strings[index][row]=s;f.refresh(index);}
  else if(op==3){auto index=word(in);check(index<n);f.names[index].count=0;}
  else check(false);
 }
 char extra;check(!in.read(&extra,1));f.strings=f.original;for(unsigned i=0;i<n;++i)f.refresh(i);
 auto good=f.registry.view();std::int32_t output=123;auto saved_names=f.names[0];
 for(unsigned mode=0;mode<12;++mode){auto r=good;auto* p=&r;const char* group="CharacterProperties";const char* key="SkillTree";auto kind=1u;
  if(mode==0)p=nullptr;
  if(mode==1)p=reinterpret_cast<dh2::data::DesignRegistry16*>(reinterpret_cast<char*>(p)+1);
  if(mode==2)r.reserved=1;
  if(mode==3)r.count=65537;
  if(mode==4)r.registrations=nullptr;
  if(mode==5)group=nullptr;
  if(mode==6)key=nullptr;
  if(mode==7)kind=0;
  if(mode==8)f.names[0].reserved=1;
  if(mode==9)f.names[0].names=nullptr;
  if(mode==10)f.names[0].count=65537;
  if(mode==11)f.names[0].names=reinterpret_cast<const char* const*>(reinterpret_cast<const char*>(f.names[0].names)+1);
  check(dh2_game_design_tables_lookup(p,kind,group,key,&output)!=0&&output==123);f.names[0]=saved_names;++guards;
 }
 auto* alias=reinterpret_cast<std::int32_t*>(&good);auto snapshot=good;check(dh2_game_design_tables_lookup(&good,1,"CharacterProperties","SkillTree",alias)!=0&&!std::memcmp(&good,&snapshot,sizeof good));++guards;
 return queries;
}
std::vector<std::string> first_strings(const std::vector<unsigned char>& b){std::size_t at=0;auto read=[&](){check(at+4<=b.size());unsigned value;std::memcpy(&value,b.data()+at,4);at+=4;return value;};auto n=read();check(n<65536);std::vector<std::string> out;for(unsigned i=0;i<n;++i){auto size=read();check(size<=b.size()-at);out.emplace_back(reinterpret_cast<const char*>(b.data()+at),size);at+=size;}return out;}
struct Composite {dh2::data::DesignRegistry16 registry;dh2_script_constants* constants;};
int lookup_design(void* p,unsigned kind,const char* group,const char* name,std::int32_t* out){auto& c=*static_cast<Composite*>(p);return kind?dh2_game_design_tables_lookup(&c.registry,kind,group,name,out):dh2_script_constants_lookup(c.constants,kind,group,name,out);}
}
int main(int argc,char** argv){try{
 check(argc==8);
 DesignFixture design_fixture;auto registry_cases=replay_design(argv[7],design_fixture);std::ifstream in(argv[1],std::ios::binary);check(bool(in)&&word(in)==0x314c4843);auto np=word(in),nr=word(in),nc=word(in);check(np==14&&nc>0&&nc<10000);Fixture f;
 for(auto& v:f.rules.defaults)v=signed_word(word(in));
 for(auto& v:f.rules.types)v=signed_word(word(in));
 std::vector<Sheets> profiles(np);for(auto& p:profiles)for(auto& s:p)for(auto& v:s)v=signed_word(word(in));
 std::vector<std::vector<dh2::data::ClassFormula>> rows(nc);for(auto& row:rows){auto n=word(in);check(n<=10000);row.resize(n);for(auto& item:row){item.destination=signed_word(word(in));item.type=signed_word(word(in));item.p1=signed_word(word(in));item.p2=signed_word(word(in));item.p3=signed_word(word(in));}}
 for(auto& row:rows)f.classes.push_back({row.data(),unsigned(row.size())});
 auto* constants=dh2_script_constants_create();check(constants);unsigned loads=0,assignments=0;
 // Exact original CST1 load records 0 and 5, verified against cache bytes by
 // the runner. This is source loader state, not a supplied cap callback.
 const dh2_script_constants_reload source_loads[]={{1042,58,7,0},{3999,152,13,0}};
 for(unsigned ai=5;ai<7;++ai){auto bytes=file(argv[ai]);dh2_script_constants_reload r{};check(dh2_script_constants_load(constants,bytes.data(),unsigned(bytes.size()),&r)==0&&r.consumed==bytes.size()&&!std::memcmp(&r,&source_loads[ai-5],sizeof r));assignments+=r.assignments;++loads;}
 std::int32_t cap;check(!dh2_script_constants_get(constants,"CharacterDesign","MaxLevelDVeryHard",&cap)&&cap==100);check(!dh2_script_constants_get(constants,"AIStates","Attack",&cap)&&cap==5);
 Composite composite{design_fixture.registry.view(),constants};f.design={&composite,lookup_design,0};f.services={&f,debug};f.model={&f.view,f.classes.data(),nc,0,&f.design};f.bindings={&f.model,f.services,{0,0,0}};
 unsigned callbacks=0,compared=0,skipped=0;for(unsigned ri=0;ri<nr;++ri){auto pi=word(in),kind=word(in),payload=word(in),n=word(in);f.cap[0]=signed_word(word(in));f.cap[1]=signed_word(word(in));auto carry=word(in),nt=word(in),level=word(in);check(pi<np&&n<=3&&nt<32);
  Sheets before{},after{};for(auto& s:before)for(auto& v:s)v=signed_word(word(in));for(auto& s:after)for(auto& v:s)v=signed_word(word(in));std::vector<Event> expected;for(unsigned j=0;j<nt;++j){Event e;for(auto& v:e)v=word(in);if(e[0]!=1&&e[0]!=2&&e[0]!=5)expected.push_back(e);}
  (void)carry;
  if(f.cap[0]!=100||f.cap[1]!=100){++skipped;continue;}
  f.sheets=before;refresh(f);f.trace.clear();f.lookups=0;
  std::array<dh2_script_value,3> a{};a[0].type=kind;a[0].number=number(payload);unsigned returned=999;char error[128]{};check(dh2_character_set_level_lua(&f.bindings,a.data(),n,nullptr,0,&returned,error,sizeof error)==0&&returned==0);check(f.sheets==after&&f.trace==expected);std::int32_t native_level;check(dh2_character_get_level(&native_level,&f.view)==0&&unsigned(native_level)==level);callbacks+=expected.size();++compared;
 }
 char extra;check(!in.read(&extra,1));f.sheets=profiles[0];refresh(f);f.cap[0]=f.cap[1]=100;auto stable=f.sheets;auto backup_model=f.model;auto backup_view=f.view;
 for(unsigned mode=0;mode<7;++mode){f.model=backup_model;f.view=backup_view;LevelModel32* m=&f.model;
  if(mode==0)m=reinterpret_cast<LevelModel32*>(reinterpret_cast<char*>(m)+1);
  if(mode==1)f.model.reserved=1;
  if(mode==2)f.model.class_count=10001;
  if(mode==3)f.view.base=reinterpret_cast<const std::int32_t*>(reinterpret_cast<const char*>(f.view.base)+1);
  if(mode==4)f.view.group_count=10001;
  if(mode==5)f.model.design=nullptr;
  if(mode==6)f.model.properties=nullptr;
  check(dh2_character_set_level(m,256,&f.services)==1&&f.sheets==stable);++guards;
 }f.model=backup_model;f.view=backup_view;
 // Read actual tables for Lua metadata adapters. They are explicitly caller
 // services, while GetProp, SetLevel, fixed math and class/cache are genuine.
 std::string assets=argv[4],error;auto data=file(assets+"/character_properties_pyarray.bin"),names=file(assets+"/character_properties_pyarraynames.bin"),fields_data=file(assets+"/character_properties_pystructnames.bin");check(dh2::data::load_characters({data.data(),data.size()},{names.data(),names.size()},{fields_data.data(),fields_data.size()},f.table,error));auto cd=file(assets+"/character_classes_pyarray.bin"),cn=file(assets+"/character_classes_pyarraynames.bin"),cf=file(assets+"/character_classes_pystructnames.bin");dh2::data::ClassTables classes;check(dh2::data::load_classes({cd.data(),cd.size()},{cn.data(),cn.size()},{cf.data(),cf.size()},classes,error));f.class_names=classes.names;
 check(design_fixture.values("CharacterProperties")==f.table.fields&&design_fixture.values("CharacterTable")==f.table.names&&design_fixture.values("ClassTable")==classes.names);
 auto aid=file(assets+"/ai_pyarray.bin"),ain=file(assets+"/ai_pyarraynames.bin"),ais=file(assets+"/ai_pystructnames.bin"),afd=file(assets+"/ai_factions_pyarray.bin"),afn=file(assets+"/ai_factions_pyarraynames.bin"),afs=file(assets+"/ai_factions_pystructnames.bin");dh2::data::AiTables ai;check(dh2::data::load_ai({aid.data(),aid.size()},{ain.data(),ain.size()},{ais.data(),ais.size()},{afd.data(),afd.size()},{afn.data(),afn.size()},{afs.data(),afs.size()},ai,error));check(design_fixture.values("AITable")==ai.names&&design_fixture.values("AIProps")==first_strings(ais));
 f.host_sheets=profiles[10];f.host_view={f.rules.defaults.data(),f.rules.types.data(),f.host_sheets[0].data(),f.host_sheets[1].data(),f.host_sheets[2].data(),f.host_sheets[3].data(),nullptr,0};check(dh2_class_recalc_base(f.classes.data(),nc,f.host_sheets[0].data(),&f.host_view)==0);
 auto common=file(argv[2]),monster=file(argv[3]);unsigned monster_sessions=0;dh2::data::PropertySheet temporary{};
 for(unsigned pi=0;pi<10;++pi){f.sheets=profiles[pi];refresh(f);check(dh2_class_recalc_base(f.classes.data(),nc,f.sheets[0].data(),&f.view)==0);f.trace.clear();f.lookups=0;f.prop={{f.sheets[3].data(),224,0},{temporary.data(),224,0},nullptr,nullptr};auto* vm=dh2_script_vm_create(8*1024*1024);check(vm);auto* aliases=dh2_script_alias_create();check(aliases);
  check(dh2_character_property_bind(vm,&f.prop)==0&&dh2_character_level_bind(vm,&f.bindings)==0&&dh2_script_alias_bind(vm,aliases)==0&&dh2_script_scalar_bind(vm,nullptr)==0&&dh2_script_design_bind(vm,&f.design)==0);
  load(vm,"actual_cap=GetPyCst('CharacterDesign','MaxLevelDVeryHard'); actual_attack=GetPyCst('AIStates','Attack')");check(get(vm,"actual_cap").number==100&&get(vm,"actual_attack").number==5);vm_checks+=2;
  load(vm,"actual_field=GetPyStruct('CharacterProperties','SkillTree'); actual_player=GetPyOID('CharacterTable','KnightPlayerBase'); actual_ai=GetPyOID('AITable','Player'); actual_buff=GetPyOID('ClassTable','Buff_Speed'); missing_table=GetPyOID('absent_table','absent')");check(get(vm,"actual_field").number==28&&get(vm,"actual_player").number==263&&get(vm,"missing_table").number==-1);std::int32_t ai_value,buff_value;check(!lookup_design(&composite,1,"AITable","Player",&ai_value)&&get(vm,"actual_ai").number==float(ai_value));check(!lookup_design(&composite,1,"ClassTable","Buff_Speed",&buff_value)&&get(vm,"actual_buff").number==float(buff_value));vm_checks+=5;
  load(vm,"cross_field=GetPyOID('CharacterProperties','SkillTree');cross_row=GetPyStruct('CharacterTable','KnightPlayerBase');nul_row=GetPyOID('CharacterTable'..string.char(0)..'tail','KnightPlayerBase'..string.char(0)..'tail')");check(get(vm,"cross_field").number==28&&get(vm,"cross_row").number==263&&get(vm,"nul_row").number==263);vm_checks+=3;
  check(dh2_script_vm_bind_source_values(vm,"GetHostPlayerLevel",host_level,&f)==0);
  for(auto p:{std::pair<const char*,unsigned>{"GetPosition",1},{"GetHostPlayerDifficulty",2},{"GetCurrentLevelRange",3}})check(dh2_script_vm_bind_source_values(vm,p.first,world_input,reinterpret_cast<void*>(std::uintptr_t(p.second)))==0);
  check(dh2_script_vm_load_source_file(vm,common.data(),common.size())==0&&dh2_script_vm_load_source_file(vm,monster.data(),monster.size())==0);check(dh2_script_alias_call_discard_source(vm,aliases,"OnInit",nullptr,0)==0);++monster_sessions;
  auto actual=f.sheets;std::int32_t actual_level;check(dh2_character_get_level(&actual_level,&f.view)==0);load(vm,"level_before=FromFixed(GetProp(19)); returns=select('#',SetLevel(ToFixed(4)));level_after=FromFixed(GetProp(19));bad_returns=select('#',SetLevel('4'));level_bad=FromFixed(GetProp(19))");check(get(vm,"level_before").number==float(actual_level)&&get(vm,"level_after").number==4.f&&get(vm,"level_bad").number==4.f&&get(vm,"returns").number==0.f&&get(vm,"bad_returns").number==0.f);vm_checks+=5;
  f.sheets=actual;refresh(f);f.lookups=0;check(dh2_character_set_level(&f.model,1024,&f.services)==0);auto expected=f.sheets;f.sheets=actual;refresh(f);load(vm,"SetLevel(ToFixed(4))");check(f.sheets==expected);++vm_checks;
  dh2_script_vm_destroy(vm);dh2_script_alias_destroy(aliases);
 }
 dh2_script_constants_destroy(constants);
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<compared<<",\"compared_sheet_words\":"<<compared*896<<",\"ordered_provider_callbacks\":"<<callbacks<<",\"actual_monster_sessions\":"<<monster_sessions<<",\"vm_checks\":"<<vm_checks<<",\"guards\":"<<guards<<",\"checks\":"<<checks<<",\"mismatches\":0,\"level_library\":\""<<origin(reinterpret_cast<void*>(dh2_character_set_level))<<"\",\"property_library\":\""<<origin(reinterpret_cast<void*>(dh2_character_get_prop))<<"\",\"runtime_library\":\""<<origin(reinterpret_cast<void*>(dh2_script_vm_create))<<"\",\"data_library\":\""<<origin(reinterpret_cast<void*>(dh2_property_resolve))<<"\",\"registered_lookup_cases\":"<<registry_cases<<",\"design_table_library\":\""<<origin(reinterpret_cast<void*>(dh2_game_design_tables_lookup))<<"\",\"real_constant_loads\":"<<loads<<",\"real_constant_assignments\":"<<assignments<<",\"skipped_non100_cases\":"<<skipped<<",\"constants_library\":\""<<origin(reinterpret_cast<void*>(dh2_script_constants_lookup))<<"\",\"GetHostPlayerLevel_receiver_is_borrowed\":true,\"host_level_word_uses_genuine_native_GetLevel\":true,\"design_constants_use_genuine_native_loader\":true,\"debug_and_world_services_are_explicit\":true,\"GetPyStruct_GetPyOID_use_native_registry\":true}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
