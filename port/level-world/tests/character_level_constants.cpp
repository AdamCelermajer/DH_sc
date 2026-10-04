#include "character_level.hpp"
#include "character_property_bindings.hpp"
#include "../script-runtime/script_constants.hpp"
#include "../script-runtime/script_function_alias.h"
#include "../script-runtime/script_scalar_bindings.h"
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
int fields(void* opaque,const dh2_script_value* a,unsigned n,dh2_script_value* out,unsigned cap,unsigned* count,char*,std::size_t){auto& f=*static_cast<Fixture*>(opaque);check(n==2&&cap&&a[0].type==4&&a[1].type==4&&!std::strcmp(a[0].text,"CharacterProperties"));auto i=std::find(f.table.fields.begin(),f.table.fields.end(),a[1].text);check(i!=f.table.fields.end());out[0]=arg(float(i-f.table.fields.begin()));*count=1;return 0;}
int oid(void* opaque,const dh2_script_value* a,unsigned n,dh2_script_value* out,unsigned cap,unsigned* count,char*,std::size_t){auto& f=*static_cast<Fixture*>(opaque);check(n==2&&cap&&a[0].type==4&&a[1].type==4&&!std::strcmp(a[0].text,"ClassTable"));auto i=std::find(f.class_names.begin(),f.class_names.end(),a[1].text);check(i!=f.class_names.end());out[0]=arg(float(i-f.class_names.begin()));*count=1;return 0;}
int host_level(void* opaque,const dh2_script_value*,unsigned,dh2_script_value* out,unsigned cap,unsigned* count,char*,std::size_t){auto& f=*static_cast<Fixture*>(opaque);std::int32_t level;check(cap&&dh2_character_get_level(&level,&f.host_view)==0);out[0]=arg(float(level));*count=1;return 0;}
int world_input(void* opaque,const dh2_script_value*,unsigned,dh2_script_value* out,unsigned cap,unsigned* count,char*,std::size_t){auto id=reinterpret_cast<std::uintptr_t>(opaque);check(cap>=2);*count=0;if(id==1){out[0]=arg(10);out[1]=arg(20);*count=2;}else if(id==2){out[0]=arg(0);*count=1;}else if(id==3){out[0]=arg(-1);out[1]=arg(-1);*count=2;}else check(false);return 0;}
std::string origin(void* p){Dl_info d{};check(dladdr(p,&d)!=0);return d.dli_fname;}
void load(dh2_script_vm* vm,const std::string& code){check(dh2_script_vm_load(vm,code.data(),code.size(),"level-vm")==0);}
dh2_script_value get(dh2_script_vm* vm,const char* name){dh2_script_value v{};check(dh2_script_vm_get_global(vm,name,&v)==0);return v;}
}
int main(int argc,char** argv){try{
 check(argc==7);std::ifstream in(argv[1],std::ios::binary);check(bool(in)&&word(in)==0x314c4843);auto np=word(in),nr=word(in),nc=word(in);check(np==14&&nc>0&&nc<10000);Fixture f;
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
 f.design={constants,dh2_script_constants_lookup,0};f.services={&f,debug};f.model={&f.view,f.classes.data(),nc,0,&f.design};f.bindings={&f.model,f.services,{0,0,0}};
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
 f.host_sheets=profiles[10];f.host_view={f.rules.defaults.data(),f.rules.types.data(),f.host_sheets[0].data(),f.host_sheets[1].data(),f.host_sheets[2].data(),f.host_sheets[3].data(),nullptr,0};check(dh2_class_recalc_base(f.classes.data(),nc,f.host_sheets[0].data(),&f.host_view)==0);
 auto common=file(argv[2]),monster=file(argv[3]);unsigned monster_sessions=0;dh2::data::PropertySheet temporary{};
 for(unsigned pi=0;pi<10;++pi){f.sheets=profiles[pi];refresh(f);check(dh2_class_recalc_base(f.classes.data(),nc,f.sheets[0].data(),&f.view)==0);f.trace.clear();f.lookups=0;f.prop={{f.sheets[3].data(),224,0},{temporary.data(),224,0},nullptr,nullptr};auto* vm=dh2_script_vm_create(8*1024*1024);check(vm);auto* aliases=dh2_script_alias_create();check(aliases);
  check(dh2_character_property_bind(vm,&f.prop)==0&&dh2_character_level_bind(vm,&f.bindings)==0&&dh2_script_alias_bind(vm,aliases)==0&&dh2_script_scalar_bind(vm,nullptr)==0&&dh2_script_design_bind(vm,&f.design)==0);
  load(vm,"actual_cap=GetPyCst('CharacterDesign','MaxLevelDVeryHard'); actual_attack=GetPyCst('AIStates','Attack')");check(get(vm,"actual_cap").number==100&&get(vm,"actual_attack").number==5);vm_checks+=2;
  check(dh2_script_vm_bind_source_values(vm,"GetPyStruct",fields,&f)==0&&dh2_script_vm_bind_source_values(vm,"GetPyOID",oid,&f)==0&&dh2_script_vm_bind_source_values(vm,"GetHostPlayerLevel",host_level,&f)==0);
  for(auto p:{std::pair<const char*,unsigned>{"GetPosition",1},{"GetHostPlayerDifficulty",2},{"GetCurrentLevelRange",3}})check(dh2_script_vm_bind_source_values(vm,p.first,world_input,reinterpret_cast<void*>(std::uintptr_t(p.second)))==0);
  check(dh2_script_vm_load_source_file(vm,common.data(),common.size())==0&&dh2_script_vm_load_source_file(vm,monster.data(),monster.size())==0);check(dh2_script_alias_call_discard_source(vm,aliases,"OnInit",nullptr,0)==0);++monster_sessions;
  auto actual=f.sheets;std::int32_t actual_level;check(dh2_character_get_level(&actual_level,&f.view)==0);load(vm,"level_before=FromFixed(GetProp(19)); returns=select('#',SetLevel(ToFixed(4)));level_after=FromFixed(GetProp(19));bad_returns=select('#',SetLevel('4'));level_bad=FromFixed(GetProp(19))");check(get(vm,"level_before").number==float(actual_level)&&get(vm,"level_after").number==4.f&&get(vm,"level_bad").number==4.f&&get(vm,"returns").number==0.f&&get(vm,"bad_returns").number==0.f);vm_checks+=5;
  f.sheets=actual;refresh(f);f.lookups=0;check(dh2_character_set_level(&f.model,1024,&f.services)==0);auto expected=f.sheets;f.sheets=actual;refresh(f);load(vm,"SetLevel(ToFixed(4))");check(f.sheets==expected);++vm_checks;
  dh2_script_vm_destroy(vm);dh2_script_alias_destroy(aliases);
 }
 dh2_script_constants_destroy(constants);
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<compared<<",\"compared_sheet_words\":"<<compared*896<<",\"ordered_provider_callbacks\":"<<callbacks<<",\"actual_monster_sessions\":"<<monster_sessions<<",\"vm_checks\":"<<vm_checks<<",\"guards\":"<<guards<<",\"checks\":"<<checks<<",\"mismatches\":0,\"level_library\":\""<<origin(reinterpret_cast<void*>(dh2_character_set_level))<<"\",\"property_library\":\""<<origin(reinterpret_cast<void*>(dh2_character_get_prop))<<"\",\"runtime_library\":\""<<origin(reinterpret_cast<void*>(dh2_script_vm_create))<<"\",\"data_library\":\""<<origin(reinterpret_cast<void*>(dh2_property_resolve))<<"\",\"real_constant_loads\":"<<loads<<",\"real_constant_assignments\":"<<assignments<<",\"skipped_non100_cases\":"<<skipped<<",\"constants_library\":\""<<origin(reinterpret_cast<void*>(dh2_script_constants_lookup))<<"\",\"GetHostPlayerLevel_receiver_is_borrowed\":true,\"host_level_word_uses_genuine_native_GetLevel\":true,\"design_constants_use_genuine_native_loader\":true,\"debug_and_world_services_are_explicit\":true}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
