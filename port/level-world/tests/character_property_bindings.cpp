#include "character_property_bindings.hpp"
#include "../game-data/properties.hpp"
#include "../script-runtime/script_scalar_bindings.h"
#include "../script-runtime/script_function_alias.h"
#include <array>
#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <dlfcn.h>
using namespace dh2::character;
namespace {
unsigned checks=0,guards=0,resolver_calls=0,property_reads=0,lua_checks=0,cache_words=0;
void check(bool x){++checks;if(!x)throw std::runtime_error("property binding check "+std::to_string(checks));}
std::uint32_t word(std::istream& f){std::uint32_t v;f.read(reinterpret_cast<char*>(&v),4);check(bool(f));return v;}
float number(std::uint32_t v){float f;std::memcpy(&f,&v,4);return f;}
std::uint32_t bits(float f){std::uint32_t v;std::memcpy(&v,&f,4);return v;}
std::vector<std::uint8_t> file(const std::string& path){std::ifstream f(path,std::ios::binary);check(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
using Profile=std::array<dh2::data::PropertySheet,7>;
struct Fixture {
 PropertyBindings48 bindings{};const std::int32_t* external=nullptr;
 bool failure=false,malformed=false;unsigned external_calls=0,level_calls=0;float requested_level=0;
 std::vector<unsigned> properties;std::vector<std::int32_t> values;
 dh2::data::CharacterTable* table=nullptr;dh2::data::ClassTables* classes=nullptr;
};
int sheet(void* opaque,std::uintptr_t identity,PropertySheet16* out){
 auto& f=*static_cast<Fixture*>(opaque);++resolver_calls;++f.external_calls;check(identity==UINT64_C(0xfedcba9876543210));
 if(f.failure)return 1;
 *out={f.external,f.malformed?223u:224u,0};return 0;
}
void setup(Fixture& f,const Profile& p){f.bindings={{p[3].data(),224,0},{p[6].data(),224,0},&f,sheet};f.external=p[2].data();f.external_calls=0;f.failure=f.malformed=false;}
dh2_script_value arg(float n){dh2_script_value a{};a.type=3;a.number=n;return a;}
int prop(void* opaque,const dh2_script_value* a,std::uint32_t n,dh2_script_value* out,std::uint32_t cap,std::uint32_t* count,char* error,std::size_t size){
 auto& f=*static_cast<Fixture*>(opaque);auto status=dh2_character_get_prop(&f.bindings,a,n,out,cap,count,error,size);
 if(!status&&*count){++property_reads;f.properties.push_back(a[0].number>=0.f&&a[0].number<224.f?static_cast<unsigned>(a[0].number):0u);f.values.push_back(f.bindings.resolved.words[f.properties.back()]);}return status;
}
int level(void* opaque,const dh2_script_value* a,std::uint32_t n,dh2_script_value*,std::uint32_t,std::uint32_t* count,char*,std::size_t){auto& f=*static_cast<Fixture*>(opaque);check(n==1&&a[0].type==3);++f.level_calls;f.requested_level=a[0].number;*count=0;return 0;}
// Explicit caller services; table indices come from genuine native decoded
// table bytes. This is not a proof of Character.GetPyStruct/GetPyOID bodies.
int fields(void* opaque,const dh2_script_value* a,std::uint32_t n,dh2_script_value* out,std::uint32_t cap,std::uint32_t* count,char*,std::size_t){
 auto& f=*static_cast<Fixture*>(opaque);*count=0;check(n>=2&&a[0].type==4&&a[1].type==4&&cap>=1);
 check(std::string(a[0].text,a[0].text_bytes)=="CharacterProperties");
 auto name=std::string(a[1].text,a[1].text_bytes);auto i=std::find(f.table->fields.begin(),f.table->fields.end(),name);check(i!=f.table->fields.end());out[0]=arg(float(i-f.table->fields.begin()));*count=1;return 0;
}
int oid(void* opaque,const dh2_script_value* a,std::uint32_t n,dh2_script_value* out,std::uint32_t cap,std::uint32_t* count,char*,std::size_t){
 auto& f=*static_cast<Fixture*>(opaque);check(n==2&&a[0].type==4&&a[1].type==4&&cap);check(std::string(a[0].text,a[0].text_bytes)=="ClassTable");
 auto i=std::find(f.classes->names.begin(),f.classes->names.end(),std::string(a[1].text,a[1].text_bytes));check(i!=f.classes->names.end());out[0]=arg(float(i-f.classes->names.begin()));*count=1;return 0;
}
int input(void* opaque,const dh2_script_value*,std::uint32_t,dh2_script_value* out,std::uint32_t cap,std::uint32_t* count,char*,std::size_t){
 auto service=reinterpret_cast<std::uintptr_t>(opaque);check(cap>=2);*count=0;
 if(service==1){out[0]=arg(123.f);out[1]=arg(-456.f);*count=2;}
 else if(service==2){out[0]=arg(4.f);*count=1;}
 else if(service==3){out[0]=arg(0.f);*count=1;}
 else if(service==4){out[0]=arg(-1.f);out[1]=arg(-1.f);*count=2;}
 else check(false);
 return 0;
}
std::string origin(void* fn){Dl_info i{};check(dladdr(fn,&i)!=0);return i.dli_fname;}
void load(dh2_script_vm* vm,const std::string& text){check(dh2_script_vm_load(vm,text.data(),text.size(),"property-host")==0);}
dh2_script_value global(dh2_script_vm* vm,const char* name){dh2_script_value v{};check(dh2_script_vm_get_global(vm,name,&v)==0);return v;}
int identity(void*,const dh2_script_value*,std::uint32_t,dh2_script_value* out,std::uint32_t cap,std::uint32_t* count,char*,std::size_t){check(cap);out[0]={};out[0].type=2;out[0].identity=UINT64_C(0xfedcba9876543210);*count=1;return 0;}
}
int main(int argc,char** argv){try{
 check(argc==5);std::ifstream in(argv[1],std::ios::binary);check(bool(in));check(word(in)==0x31425043);
 auto profiles_count=word(in),records=word(in),direct=word(in);check(profiles_count==12);
 std::vector<Profile> profiles(profiles_count);for(auto& p:profiles)for(auto& s:p)for(auto& value:s){auto u=word(in);std::memcpy(&value,&u,4);}
 for(unsigned pi=0;pi+1<profiles_count;++pi){auto& p=profiles[pi];dh2::data::PropertyRules r{p[4],p[5]};dh2::data::PropertyState s{p[0],p[1],p[2],p[4]};auto view=dh2::data::property_view(r,s);
  for(int i=0;i<224;++i){std::int32_t result;check(dh2_property_resolve(&view,i,&result)==0&&result==p[3][i]);++cache_words;}check(s.resolved==p[3]);}
 Fixture f;char error[256]{};
 for(unsigned i=0;i<records;++i){auto pi=word(in),kind=word(in),payload=word(in),second=word(in),boolean=word(in),id=word(in),n=word(in),expected_count=word(in),expected_value=word(in),calls=word(in);
  check(pi<profiles_count&&n<=3&&calls<=1);std::vector<std::array<unsigned,2>> trace(calls);for(auto& t:trace){t[0]=word(in);t[1]=word(in);}
  setup(f,profiles[pi]);std::array<dh2_script_value,3> a{};for(unsigned j=0;j<n;++j){a[j].type=j?second:kind;a[j].number=number(j?0:payload);a[j].boolean=j?boolean:0;if(j&&id)a[j].identity=UINT64_C(0xfedcba9876543210);}
  dh2_script_value result{};unsigned count=999;check(dh2_character_get_prop(&f.bindings,a.data(),n,&result,1,&count,error,sizeof error)==0);check(count==expected_count);if(count)check(result.type==3&&bits(result.number)==expected_value);
  check(f.external_calls==unsigned(calls&&trace[0][0]==2&&second==2));
 }
 for(unsigned i=0;i<direct;++i){auto pi=word(in),si=word(in),id=word(in),expected=word(in);std::int32_t p;std::memcpy(&p,&id,4);auto& profile=profiles[pi];PropertySheet16 s{profile[si==0?3:si==1?6:2].data(),224,0};std::int32_t out=123;check(dh2_character_property_word(&out,si==3?nullptr:&s,p)==0&&std::uint32_t(out)==expected);}
 char extra;check(!in.read(&extra,1));
 setup(f,profiles[0]);std::int32_t output=0x12345678;PropertySheet16 bad=f.bindings.resolved;
 for(int mode=0;mode<6;++mode){bad=f.bindings.resolved;if(mode==0)bad.count=223;if(mode==1)bad.reserved=1;if(mode==2)bad.words=nullptr;if(mode==3)bad.words=reinterpret_cast<const std::int32_t*>(reinterpret_cast<const char*>(bad.words)+1);if(mode==4)bad.count=65537;
  auto* projected=mode==5?reinterpret_cast<const PropertySheet16*>(reinterpret_cast<const char*>(&bad)+1):&bad;
  check(dh2_character_property_word(&output,projected,0)==1&&output==0x12345678);++guards;}
 check(dh2_character_property_word(&output,nullptr,0)==0&&output==-1);++guards;
 auto a=arg(0);dh2_script_value result{};unsigned count;f.failure=true;
 std::array<dh2_script_value,2> ext{a,{}};ext[1].type=2;ext[1].identity=UINT64_C(0xfedcba9876543210);
 for(bool malformed:{false,true}){f.failure=!malformed;f.malformed=malformed;std::memset(&result,0x5a,sizeof result);auto saved=result;check(dh2_character_get_prop(&f.bindings,ext.data(),2,&result,1,&count,error,sizeof error)==1&&count==0&&!std::memcmp(&saved,&result,sizeof result));++guards;}
 setup(f,profiles[0]);auto before=profiles[0];
 check(dh2_character_property_word(reinterpret_cast<std::int32_t*>(const_cast<std::int32_t*>(f.bindings.resolved.words)),&f.bindings.resolved,0)==1&&profiles[0]==before);++guards;
 for(int mode=0;mode<8;++mode){result={};std::memset(&result,0x5a,sizeof result);auto saved=result;count=123;const auto* args=&a;auto* output_value=&result;auto* returned=&count;void* context=&f.bindings;unsigned capacity=1,n=1;
  if(mode==0)args=reinterpret_cast<const dh2_script_value*>(reinterpret_cast<const char*>(&a)+1);
  if(mode==1)output_value=reinterpret_cast<dh2_script_value*>(reinterpret_cast<char*>(&result)+1);
  if(mode==2)returned=reinterpret_cast<unsigned*>(reinterpret_cast<char*>(&count)+1);
  if(mode==3)capacity=0;
  if(mode==4)context=nullptr;
  if(mode==5)output_value=&a;
  if(mode==6)returned=reinterpret_cast<unsigned*>(&f.bindings);
  if(mode==7)n=1048577;
  auto input_copy=a;auto bindings_copy=f.bindings;check(dh2_character_get_prop(context,args,n,output_value,capacity,returned,error,sizeof error)==1);check(!std::memcmp(&result,&saved,sizeof result)&&!std::memcmp(&a,&input_copy,sizeof a)&&!std::memcmp(&f.bindings,&bindings_copy,sizeof f.bindings)&&profiles[0]==before);++guards;
 }
 std::string assets=argv[4],diagnostic;auto data=file(assets+"/character_properties_pyarray.bin"),names=file(assets+"/character_properties_pyarraynames.bin"),fields_data=file(assets+"/character_properties_pystructnames.bin");dh2::data::CharacterTable table;
 check(dh2::data::load_characters({data.data(),data.size()},{names.data(),names.size()},{fields_data.data(),fields_data.size()},table,diagnostic));auto cd=file(assets+"/character_classes_pyarray.bin"),cn=file(assets+"/character_classes_pyarraynames.bin"),cf=file(assets+"/character_classes_pystructnames.bin");dh2::data::ClassTables classes;
 check(dh2::data::load_classes({cd.data(),cd.size()},{cn.data(),cn.size()},{cf.data(),cf.size()},classes,diagnostic));
 auto common=file(argv[2]),monster=file(argv[3]);unsigned monsters=0;
 for(unsigned pi=0;pi+1<profiles_count;++pi){setup(f,profiles[pi]);f.table=&table;f.classes=&classes;f.properties.clear();f.values.clear();f.level_calls=0;auto* vm=dh2_script_vm_create(8*1024*1024);check(vm);auto* aliases=dh2_script_alias_create();check(aliases);
  check(dh2_script_scalar_bind(vm,nullptr)==0&&dh2_script_alias_bind(vm,aliases)==0);check(dh2_character_property_bind(vm,&f.bindings)==0);
  check(dh2_script_vm_bind_source_values(vm,"GetProp",prop,&f)==0&&dh2_script_vm_bind_source_values(vm,"GetPyStruct",fields,&f)==0&&dh2_script_vm_bind_source_values(vm,"GetPyOID",oid,&f)==0);
  check(dh2_script_vm_bind_source_values(vm,"SetLevel",level,&f)==0);
  for(auto pair:{std::pair<const char*,unsigned>{"GetPosition",1},{"GetHostPlayerLevel",2},{"GetHostPlayerDifficulty",3},{"GetCurrentLevelRange",4}})check(dh2_script_vm_bind_source_values(vm,pair.first,input,reinterpret_cast<void*>(std::uintptr_t(pair.second)))==0);
  check(dh2_script_vm_load_source_file(vm,common.data(),common.size())==0);check(dh2_script_vm_load_source_file(vm,monster.data(),monster.size())==0);check(f.properties.size()==1);
  check(dh2_script_alias_call_discard_source(vm,aliases,"OnInit",nullptr,0)==0);check(f.properties.size()==4);
  for(unsigned j=0;j<4;++j){const char* property[]={"SkillTree","LevelMax","LevelMin","LevelOffset"};auto it=std::find(table.fields.begin(),table.fields.end(),property[j]);check(it!=table.fields.end());check(f.properties[j]==unsigned(it-table.fields.begin())&&f.values[j]==profiles[pi][3][f.properties[j]]);++lua_checks;}
  auto maximum=f.values[1]>>8,minimum=f.values[2]>>8,offset=f.values[3]>>8;check(f.level_calls==unsigned(maximum>-1));if(maximum>-1){auto selected=4;if(selected<=minimum)selected=minimum;else if(selected>=maximum)selected=maximum;check(f.requested_level==float((selected+offset)*256));}++lua_checks;
  check(global(vm,"m_flee_flag").boolean==1&&global(vm,"saved_X").number==123.f&&global(vm,"saved_Y").number==-456.f);++lua_checks;
  check(dh2_script_vm_bind_source_values(vm,"SheetIdentity",identity,nullptr)==0);load(vm,"r0=GetProp(198);r1=GetProp(198,true);r2=GetProp(198,SheetIdentity());r3=GetProp(198,{_this=SheetIdentity()});n0=select('#',GetProp('198'));n1=select('#',GetProp(224));n2=select('#',GetProp(0,nil));rn=GetProp(-1);rz=GetProp(0/0)");
  check(global(vm,"r0").number==float(profiles[pi][3][198])&&global(vm,"r1").number==float(profiles[pi][6][198])&&global(vm,"r2").number==float(profiles[pi][2][198])&&global(vm,"r3").number==float(profiles[pi][3][198]));check(global(vm,"n0").number==0&&global(vm,"n1").number==0&&global(vm,"n2").number==1);check(global(vm,"rn").number==float(profiles[pi][3][0])&&global(vm,"rz").number==float(profiles[pi][3][0]));lua_checks+=9;
  dh2_script_vm_destroy(vm);dh2_script_alias_destroy(aliases);++monsters;
 }
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<records<<",\"direct_sheet_comparisons\":"<<direct<<",\"genuine_cache_words\":"<<cache_words<<",\"actual_monster_sessions\":"<<monsters<<",\"lua_property_checks\":"<<lua_checks<<",\"monster_initialization_property_reads\":"<<monsters*4<<",\"ordered_lua_property_reads\":"<<property_reads<<",\"external_resolutions\":"<<resolver_calls<<",\"guards\":"<<guards<<",\"checks\":"<<checks<<",\"mismatches\":0,\"binding_library\":\""<<origin(reinterpret_cast<void*>(dh2_character_get_prop))<<"\",\"runtime_library\":\""<<origin(reinterpret_cast<void*>(dh2_script_vm_create))<<"\",\"data_library\":\""<<origin(reinterpret_cast<void*>(dh2_property_resolve))<<"\",\"monster_getprop_is_genuine\":true,\"other_gameplay_inputs_are_explicit_fixtures\":true}\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
