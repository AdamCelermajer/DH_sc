#include "../monster_external_script_session.hpp"
#include "../lua_script_level_queries.hpp"
#include "../character_script_set_level.hpp"
#include "../character_regeneration.hpp"
#include "../../game-data/properties.hpp"
#include "../../game-data/vitals.hpp"
#include "../../game-data/level_tables.hpp"
#include "../../gameplay-object-callbacks/gameplay_object_callbacks.hpp"
extern "C" {
#include "../../pydata-constants/constants.h"
}
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace ms=dh2::monster_external_script;
namespace lq=dh2::lua_script_level_queries;
namespace sl=dh2::character_script_set_level;
namespace rg=dh2::character_regeneration;
using namespace dh2::data;
static void require(bool value,const char* why){if(!value)throw std::runtime_error(why);}
static std::vector<unsigned char> read(const std::string& path){
 std::ifstream f(path,std::ios::binary);require(bool(f),"fixture missing");
 return {std::istreambuf_iterator<char>(f),{}};
}
static Bytes bytes(const std::vector<unsigned char>& v){return {v.data(),v.size()};}
static std::uint32_t bits(float f){std::uint32_t w;std::memcpy(&w,&f,4);return w;}
static float floating(std::uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
static std::int32_t signed_word(std::uint32_t w){std::int32_t s;std::memcpy(&s,&w,4);return s;}

// Explicit host fixtures for currently unresolved Application/PlayerInfo/Level
// producers. Character/class/constants and property effects use real cache data.
struct Fixture {
 CharacterTable* characters; ClassTables* classes; PropertyRules* rules;
 dh2_pycst_view* design; PropertyState properties;
 ms::Session vm; std::vector<ClassRow> rows; std::vector<std::string> trace;
 std::uintptr_t owner=0x1234567800000011ull,target=0;
 std::int32_t host_word=1,difficulty=2,level_oid=-1;
 float xyz[3]{412.5f,-111.25f,32.0f};
 bool fail_recalc=false,reenter=false; ms::Status reentry_status{};
 unsigned set_level_calls=0,positive_debug_attempts=0;
 sl::Result last_level{};
 LevelTables* levels=nullptr;float range_number=0;
 sl::Application application{0x1234567800000021ull,0x1234567800000031ull};
 Fixture(CharacterTable& c,ClassTables& cl,PropertyRules& r,dh2_pycst_view& d,
         const char* name):characters(&c),classes(&cl),rules(&r),design(&d){
  auto found=std::find(c.names.begin(),c.names.end(),name);require(found!=c.names.end(),"Ghost row absent");
  reset_properties(r,properties,&c.rows.at(found-c.names.begin()));
  for(auto& row:cl.rows)rows.push_back({row.data(),std::uint32_t(row.size())});
  auto view=property_view(r,properties);require(!dh2_class_recalc_base(rows.data(),rows.size(),properties.base.data(),&view),"initial class recalc failed");
  SpawnVitals v;require(!dh2_vitals_spawn_init(&view,&v),"initial vitals failed");
 }
 void subject(std::uintptr_t id){require(id==owner,"wrong actor identity");}
 static Fixture& f(void* p){return *static_cast<Fixture*>(p);}
 static int structure(void* p,const char* category,const char* field,std::int32_t* out){
  auto& s=f(p);require(std::string(category)=="CharacterProperties","wrong struct table");
  auto it=std::find(s.characters->fields.begin(),s.characters->fields.end(),field);
  *out=it==s.characters->fields.end()?-1:std::int32_t(it-s.characters->fields.begin());
  s.trace.emplace_back(std::string("Struct:")+field);return 0;
 }
 static int prop(void* p,std::uintptr_t id,std::int32_t property,float* out){
  auto& s=f(p);s.subject(id);auto v=property_view(*s.rules,s.properties);std::int32_t value;
  if(dh2_property_resolve(&v,property,&value))return 1;
  *out=float(value);
  s.trace.emplace_back("Prop:"+std::to_string(property));return 0;
 }
 static int oid(void* p,const char* category,const char* name,std::int32_t* out){
  auto& s=f(p);require(std::string(category)=="ClassTable","wrong OID table");
  auto it=std::find(s.classes->names.begin(),s.classes->names.end(),name);
  *out=it==s.classes->names.end()?-1:std::int32_t(it-s.classes->names.begin());
  s.trace.emplace_back("ClassOID");return 0;
 }
 static int position(void* p,std::uintptr_t id,float out[3]){
  auto& s=f(p);s.subject(id);s.trace.emplace_back("Position");
  if(s.reenter){std::string e;s.reentry_status=s.vm.dispatch(ms::Event::init,0,e);}
  dh2::gameplay::callbacks::GameObjectView v{"Ghost",s.xyz};
  return dh2::gameplay::callbacks::get_position(&v,out)?0:1;
 }
 static int application_id(void*,std::uintptr_t* out){*out=0x100000001ull;return 0;}
 static int manager(void*,std::uintptr_t app,std::uintptr_t* out){require(app==0x100000001ull,"wrong app");*out=0x200000001ull;return 0;}
 static int hosting(void*,std::uintptr_t mgr,std::uintptr_t* out){require(mgr==0x200000001ull,"wrong manager");*out=0x300000001ull;return 0;}
 static int player_word(void* p,std::uintptr_t player,unsigned at,std::int32_t* out){
  require(player==0x300000001ull&&at==0x330,"wrong PlayerInfo field");*out=f(p).host_word;return 0;
 }
 static int level(void*,std::uintptr_t* out){*out=0x400000001ull;return 0;}
 static int level_word(void* p,std::uintptr_t id,unsigned at,std::int32_t* out){
  require(id==0x400000001ull,"wrong level");auto& s=f(p);
  if(at==0x118)*out=s.difficulty;else if(at==0x3c)*out=s.level_oid;else return 1;return 0;
 }
 static int push(void*,std::int32_t){return 0;}
 static int range_value(void* p,std::uintptr_t id,float* out){auto& s=f(p);
  require(id==reinterpret_cast<std::uintptr_t>(&s.range_number),"wrong range argument");*out=s.range_number;return 0;}
 static int range_convert(void*,float number,std::int32_t* out){
  if(!std::isfinite(number)||number< -2147483648.0f||number>=2147483648.0f)return 1;
  *out=std::int32_t(number);return 0;}
 static int range_table(void* p,std::uintptr_t* out){*out=reinterpret_cast<std::uintptr_t>(f(p).levels);return 0;}
 static int range_word(void* p,std::uintptr_t id,unsigned row,unsigned at,std::int32_t* out){auto& s=f(p);
  require(s.levels&&id==reinterpret_cast<std::uintptr_t>(s.levels),"wrong owned LevelTable");
  return read_level_range_word(*s.levels,row,at,*out)?0:1;}
 lq::Services queries(){return {this,application_id,manager,hosting,player_word,level,level_word,
                              range_value,range_convert,range_table,range_word,push};}
 static int host_level(void* p,std::int32_t* out){auto& s=f(p);s.trace.emplace_back("HostWord");auto q=s.queries();lq::Result r{};
  if(lq::get_host_player_level(&q,&r)!=lq::Status::complete)return 1;
  *out=r.values[0];return 0;}
 static int host_difficulty(void* p,std::int32_t* out){auto& s=f(p);s.trace.emplace_back("Difficulty");auto q=s.queries();lq::Result r{};
  if(lq::get_host_player_difficulty(&q,&r)!=lq::Status::complete)return 1;
  *out=r.values[0];return 0;}
 static int range(void* p,const float* number,std::int32_t out[2],std::uint32_t* count){
  auto& s=f(p);s.trace.emplace_back("Range");auto q=s.queries();lq::Arguments a{};lq::Result r{};
  s.range_number=number?*number:0;
  lq::Argument value{reinterpret_cast<std::uintptr_t>(&s.range_number),3,0};
  if(number){a.values=&value;a.count=1;}
  if(lq::get_current_level_range(&a,&q,&r)!=lq::Status::complete)return 1;
  *count=r.values_pushed;std::memcpy(out,r.values,sizeof(r.values));return 0;
 }
 struct LevelRun{Fixture* fixture;float argument;};
 static int regen(void* p,const rg::Request* request,rg::Reply* out){
  auto& s=f(p);auto view=property_view(*s.rules,s.properties);
  if(request->operation==rg::Operation::read_property){
   require(request->property<224&&request->sheet==reinterpret_cast<std::uintptr_t>(s.properties.resolved.data()),"wrong resolved sheet");
   out->word=std::uint32_t(s.properties.resolved[request->property]);return 0;
  }
  if(request->operation==rg::Operation::add_property)return dh2_property_add(&view,request->property,signed_word(request->amount));
  // A positive-delta caller reaches real debug ownership; it must not be
  // declared complete while that dependency remains unresolved in this fixture.
  ++s.positive_debug_attempts;return 1;
 }
 static int set_operation(void* p,sl::Character* character,const sl::Request* request,std::uint32_t* out){
  auto& run=*static_cast<LevelRun*>(p);auto& s=*run.fixture;
  switch(request->operation){
   case sl::Operation::get_number:*out=bits(run.argument);return 0;
   case sl::Operation::float_to_signed:{auto value=floating(request->number_bits);
    if(!std::isfinite(value)||value< -2147483648.0f||value>=2147483648.0f)return 1;
    *out=std::uint32_t(std::int32_t(value));return 0;}
   case sl::Operation::design_max_level:{require(request->subject==s.application.design_manager,"wrong design owner");
    dh2_pycst_result r{};auto rc=dh2_pycst_get(s.design,request->category,std::strlen(request->category),request->key,std::strlen(request->key),&r);
    if(rc||!r.found)return 1;
    *out=std::uint32_t(r.value);return 0;}
   case sl::Operation::recalc_properties:{
    // The source caller already stored base Level. Mirror that exact write
    // before the class provider reads its live sheet, including failure cases.
    s.properties.base[19]=signed_word(character->base_level_5b8);s.trace.emplace_back("Recalc");
    if(s.fail_recalc)return 1;
    auto view=property_view(*s.rules,s.properties);
    return dh2_class_recalc_base(s.rows.data(),s.rows.size(),s.properties.base.data(),&view);}
   case sl::Operation::regen_hp:case sl::Operation::regen_mp:{
    s.trace.emplace_back(request->operation==sl::Operation::regen_hp?"RegenHP":"RegenMP");
    rg::State state{s.owner,reinterpret_cast<std::uintptr_t>(&s.properties),reinterpret_cast<std::uintptr_t>(s.properties.resolved.data())};
    rg::Globals globals{0x500000001ull};rg::Services services{&s,regen};rg::Result r{};
    auto rc=request->operation==sl::Operation::regen_hp?rg::regen_hp(&state,&globals,request->argument,&services,&r):rg::regen_mp(&state,&globals,request->argument,&services,&r);
    return rc==rg::Status::complete?0:1;}
  }return 1;
 }
 static int set_level(void* p,std::uintptr_t id,float raw){auto& s=f(p);s.subject(id);s.trace.emplace_back("SetLevel");++s.set_level_calls;
  LevelRun run{&s,raw};sl::Character c{s.owner,reinterpret_cast<std::uintptr_t>(&s.properties),std::uint32_t(s.properties.base[19])};
  sl::Arguments args{reinterpret_cast<std::uintptr_t>(&run.argument),1,3};sl::Globals g{&s.application};sl::Services services{&run,set_operation};
  return sl::set_level(&c,&args,&g,&services,&s.last_level)==sl::Status::complete?0:1;
 }
 static int has_target(void* p,std::uintptr_t id,std::uint32_t* out){auto& s=f(p);s.subject(id);*out=s.target!=0;return 0;}
 static int set_target(void* p,std::uintptr_t id,std::uintptr_t target){auto& s=f(p);s.subject(id);s.target=target;return 0;}
 static int head_to(void* p,std::uintptr_t id,std::uintptr_t){f(p).subject(id);return 0;}
 ms::Services services(){ms::Services s{};s.context=this;s.owner=owner;s.get_py_struct=structure;s.get_prop=prop;
  s.has_target=has_target;s.set_target=set_target;s.head_to=head_to;s.get_py_oid=oid;s.get_position=position;
  s.get_host_player_level=host_level;s.get_host_player_difficulty=host_difficulty;s.get_current_level_range=range;s.set_level=set_level;return s;}
 void load(ms::Source common,ms::Source monster){std::string e;auto s=services();
  require(vm.create(s,e)==ms::Status::complete&&vm.bind_functions(e)==ms::Status::complete&&
          vm.load_common(common,e)==ms::Status::complete&&vm.load_external(monster,e)==ms::Status::complete,e.c_str());trace.clear();}
};

int main(int argc,char** argv){try{
 require(argc==4,"pass cache/common/monster");std::string root=std::string(argv[1])+"/data/pydata/",error;
 auto cp=read(root+"character_properties_pyarray.bin"),cn=read(root+"character_properties_pyarraynames.bin"),cs=read(root+"character_properties_pystructnames.bin");
 CharacterTable chars;PropertyRules rules;require(load_characters(bytes(cp),bytes(cn),bytes(cs),chars,error)&&load_property_rules(chars,rules,error),error.c_str());
 auto ca=read(root+"character_classes_pyarray.bin"),can=read(root+"character_classes_pyarraynames.bin"),cas=read(root+"character_classes_pystructnames.bin"),design=read(root+"design_pycst.bin");
 ClassTables classes;require(load_classes(bytes(ca),bytes(can),bytes(cas),classes,error),error.c_str());dh2_pycst_view constants{};require(!dh2_pycst_open(&constants,design.data(),design.size()),"design constants rejected");
 auto common_bytes=read(argv[2]),monster_bytes=read(argv[3]);ms::Source common{common_bytes.data(),common_bytes.size()},monster{monster_bytes.data(),monster_bytes.size()};unsigned cases=0;
 auto levels_bytes=read(root+"levels_pyarray.bin"),levels_names=read(root+"levels_pyarraynames.bin"),levels_schema=read(root+"levels_pystructnames.bin");
 LevelTables levels;require(load_levels(bytes(levels_bytes),bytes(levels_names),bytes(levels_schema),levels,error),error.c_str());
 for(auto name:{"Crypt_Ghost","Crypt_Ghost_RE"})for(auto host:{-9,1,1000}){
  Fixture f(chars,classes,rules,constants,name);auto before=f.properties;f.host_word=host;f.reenter=true;f.load(common,monster);
  require(f.vm.dispatch(ms::Event::init,0,error)==ms::Status::complete,error.c_str());
  require(f.reentry_status==ms::Status::busy&&f.properties.base[19]==256&&f.properties.saved==before.saved&&
          f.properties.resolved==before.resolved&&f.set_level_calls==1&&f.positive_debug_attempts==0,"source OnInit ownership/property effects differ");
  require(f.trace.size()>=13&&f.trace.front()=="ClassOID"&&f.trace[1]=="Position"&&
          f.trace[f.trace.size()-4]=="SetLevel"&&f.trace[f.trace.size()-3]=="Recalc"&&
          f.trace[f.trace.size()-2]=="RegenHP"&&f.trace.back()=="RegenMP","source init order differs");
  require(f.vm.dispatch(ms::Event::enemy_spotted,0x1234567800000091ull,error)==ms::Status::complete&&
          f.target==0x1234567800000091ull&&f.vm.statistics().completed_callbacks==2,"same initialized VM did not dispatch original acquisition callback");++cases;
 }
 Fixture skipped(chars,classes,rules,constants,"Crypt_Ghost");auto max=std::find(chars.fields.begin(),chars.fields.end(),"LevelMax")-chars.fields.begin();
 skipped.properties.base.at(max)=-256;skipped.load(common,monster);require(skipped.vm.dispatch(ms::Event::init,0,error)==ms::Status::complete&&skipped.set_level_calls==0&&std::find(skipped.trace.begin(),skipped.trace.end(),"Range")!=skipped.trace.end(),"negative bound source queries were skipped");++cases;
 Fixture failing(chars,classes,rules,constants,"Crypt_Ghost");failing.fail_recalc=true;failing.properties.base[19]=512;failing.load(common,monster);
 require(failing.vm.dispatch(ms::Event::init,0,error)==ms::Status::script_error&&failing.properties.base[19]==256&&!failing.vm.ready()&&failing.trace.back()=="Recalc","failed recalc did not retain source Level write");++cases;
 Fixture missing(chars,classes,rules,constants,"Crypt_Ghost");auto service=missing.services();service.get_position=nullptr;
 require(missing.vm.initialize(common,monster,service,error)==ms::Status::complete&&missing.vm.dispatch(ms::Event::init,0,error)==ms::Status::script_error&&missing.set_level_calls==0,"missing position fabricated init");++cases;
 Fixture positive(chars,classes,rules,constants,"Crypt_Ghost");positive.properties.saved[36]-=256;positive.load(common,monster);
 require(positive.vm.dispatch(ms::Event::init,0,error)==ms::Status::script_error&&positive.positive_debug_attempts==1&&positive.trace.back()=="RegenHP"&&!positive.vm.ready(),"unresolved positive debug block silently completed");++cases;
 for(auto name:{"Crypt_Ghost","Crypt_Ghost_RE"})for(auto level_name:{"GOTHICUS_CRYPT_01","SWAMP","INFECTED_VILLAGE_01"})for(int mode=0;mode<3;++mode){
  Fixture actual(chars,classes,rules,constants,name);actual.levels=&levels;actual.level_oid=find_level(levels,level_name);actual.difficulty=mode;
  std::int32_t minimum=0;require(read_level_range_word(levels,actual.level_oid*72,0x3c+mode*4,minimum),"actual range absent");
  actual.load(common,monster);const auto result=actual.vm.dispatch(ms::Event::init,0,error);
  require(actual.properties.base[19]==minimum*256&&actual.set_level_calls==1,"unchanged Lua ignored actual level minimum");
  if(minimum==1)require(result==ms::Status::complete&&actual.positive_debug_attempts==0,"zero-delta real-range initialization failed");
  else require(result==ms::Status::script_error&&actual.positive_debug_attempts==1&&actual.trace.back()=="RegenHP", "positive real-range initialization skipped debug owner");
  ++cases;
 }
 std::printf("{\"validation\":\"PASS\",\"host_cases\":%u,\"actual_original_monster_on_init_executed\":true,\"actual_character_class_property_data_used\":true,\"source_query_set_level_class_regen_composed\":true,\"same_vm_following_acquisition\":true,\"missing_providers_fail_closed\":true,\"host_player_level_producers_are_fixtures\":true,\"current_level_oid_is_sentinel_fixture\":true,\"positive_debug_provider_unresolved\":true,\"native_wired\":false}\n",cases);return 0;
 }catch(const std::exception& e){std::fprintf(stderr,"monster init: %s\n",e.what());return 1;}}
