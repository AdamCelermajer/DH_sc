#ifdef NDEBUG
#undef NDEBUG
#endif
#include "../character_game_design.hpp"
#include "../character_property_bindings.hpp"
#include "../../script-runtime/script_scalar_bindings.h"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
using namespace dh2::character;
using Raw=std::vector<std::uint8_t>;
static unsigned checks=0,VMchecks=0,guards=0,lookupchecks=0;
static void check(bool value){++checks;if(!value)throw std::runtime_error("Game design check "+std::to_string(checks));}
static std::uint32_t word(std::ifstream& f){std::uint32_t v;f.read(reinterpret_cast<char*>(&v),4);check(bool(f));return v;}
static Raw blob(std::ifstream& f){Raw value(word(f));if(!value.empty())f.read(reinterpret_cast<char*>(value.data()),value.size());check(bool(f));return value;}
static std::string text(std::ifstream& f){auto raw=blob(f);return {raw.begin(),raw.end()};}
struct Inputs {
 std::array<std::array<Raw,3>,5> tables;
 std::vector<Raw> constants;
 std::vector<std::string> constant_names,catalog;
 std::vector<dh2::data::Bytes> constant_views;
 GameDesignInputs256 view{};
 void refresh(){GameDesignTableInput48* t[]={&view.characters,&view.classes,&view.ai,&view.factions,&view.levels};for(unsigned i=0;i<5;++i)*t[i]={{tables[i][0].data(),tables[i][0].size()},{tables[i][1].data(),tables[i][1].size()},{tables[i][2].data(),tables[i][2].size()}};
  constant_views.clear();for(const auto& raw:constants)constant_views.push_back({raw.data(),raw.size()});view.constants=constant_views.data();view.constant_count=constant_views.size();view.reserved=0;
 }
};
static Inputs inputs(const char* path){std::ifstream f(path,std::ios::binary);check(bool(f)&&word(f)==0x314f4447);Inputs out;for(auto& table:out.tables)for(auto& raw:table)raw=blob(f);auto count=word(f);for(unsigned i=0;i<count;++i){out.constant_names.push_back(text(f));out.constants.push_back(blob(f));}count=word(f);for(unsigned i=0;i<count;++i)out.catalog.push_back(text(f));char c;check(!f.read(&c,1));out.refresh();return out;}
static std::int32_t query(const CharacterGameDesign::Borrow& b,unsigned kind,const char* group,const char* name){auto* d=b.design();check(d);std::int32_t out=777;check(!d->lookup(d->context,kind,group,name,&out));++lookupchecks;return out;}
static std::string quoted(const std::string& s){std::string out="'";for(char c:s){if(c=='\''||c=='\\')out+='\\';out+=c;}return out+"'";}
static void load(dh2_script_vm* vm,const std::string& code){check(dh2_script_vm_load_source_file(vm,code.data(),code.size())==0);}
static dh2_script_value get(dh2_script_vm* vm,const char* name){dh2_script_value v{};check(!dh2_script_vm_get_global(vm,name,&v));return v;}
static int debug(void* context,LevelModel32* model,const LevelRequest24* r){auto& calls=*static_cast<unsigned*>(context);check(model&&r&&!r->reserved&&r->retained_delta>0);check(r->service==level_debug_load||r->service==level_debug_query);if(r->service==level_debug_query)check(!std::strcmp(r->name,"isTracingChar_Stats"));++calls;return 0;}
static int finalizer(void* context,const dh2_script_value* a,unsigned n,dh2_script_value*,unsigned,unsigned* returned,char*,std::size_t){check(n==2&&a[0].type==3&&a[0].number==263&&a[1].type==3&&a[1].number==100);++*static_cast<unsigned*>(context);*returned=0;return 0;}
static std::string origin(void* p){Dl_info d{};check(dladdr(p,&d));return d.dli_fname;}
int main(int argc,char** argv){try{
 check(argc==3);auto raw=inputs(argv[1]);raw.refresh();CharacterGameDesign owner;std::string error;
 check(!owner.ready()&&!owner.borrow());auto malformed=raw.view;malformed.reserved=1;check(!owner.initialize(malformed,error)&&!error.empty()&&!owner.ready());++guards;
 check(owner.initialize(raw.view,error)&&error.empty()&&owner.ready());auto b=owner.borrow();check(bool(b));
 auto* bindings=b.design();check(b.characters()->rows.size()==448&&b.classes()->rows.size()==260&&b.ai()->names.size()==76&&b.levels()->travel.size()==33&&b.levels()->levels.size()==51);
 check(b.rules()->defaults==b.characters()->rows[0]&&b.rules()->types==b.characters()->rows[1]);
 check(b.class_rows()->size()==260);for(unsigned i=0;i<260;++i){auto& row=b.classes()->rows[i];check(b.class_rows()->at(i).data==row.data()&&b.class_rows()->at(i).count==row.size());}
 // Actual original six-getter corpus header contains each authored name list.
 std::ifstream original(argv[2],std::ios::binary);check(bool(original)&&word(original)==0x31544447);check(word(original)==6);word(original);
 unsigned original_names=0;
 for(unsigned i=0;i<6;++i){auto group=text(original);auto getter=word(original),kind=word(original),n=word(original);check(kind<2);auto& r=b.registrations()->at(i);check(group==r.group&&getter==r.source_getter&&!r.reserved&&n==r.members->count);
  for(unsigned j=0;j<n;++j){auto name=text(original);check(!std::strcmp(r.members->names[j],name.c_str())&&query(b,1,group.c_str(),name.c_str())==std::int32_t(j));++original_names;}
  check(query(b,1,group.c_str(),"not_an_original_member")==-1);check(query(b,1,(group+"\0ignored").c_str(),r.members->names[0])==0);
 }
 unsigned unsupported=0;for(const auto& group:raw.catalog){bool supported=false;for(const auto& r:*b.registrations())supported|=group==r.group;if(!supported){std::int32_t out=777;check(bindings->lookup(bindings->context,1,group.c_str(),"missing",&out)!=0&&out==777);++unsupported;}}
 check(query(b,1,"absent_group","absent")==-1&&query(b,1,"characterproperties","SkillTree")==-1);
 check(query(b,1,"CharacterProperties","SkillTree")==28&&query(b,1,"CharacterTable","KnightPlayerBase")==263&&query(b,0,"CharacterDesign","MaxLevelDVeryHard")==100&&query(b,0,"AIStates","Attack")==5);
 check(b.constant_loads()->size()==raw.constants.size());unsigned assignments=0;for(unsigned i=0;i<raw.constants.size();++i){auto& loaded=b.constant_loads()->at(i);check(loaded.status==0&&loaded.bytes==raw.constants[i].size()&&!loaded.source.source_name_stop);assignments+=loaded.source.assignments;}
 check(assignments==5608);
 // Caller input streams can be overwritten/freed; all native decoded strings
 // and registered pointer arrays still refer to the owned snapshot.
 for(auto& table:raw.tables)for(auto& value:table)std::fill(value.begin(),value.end(),0);for(auto& value:raw.constants)std::fill(value.begin(),value.end(),0);
 check(query(b,1,"CharacterTable","KnightPlayerBase")==263&&query(b,0,"AIStates","Attack")==5);
 check(!owner.initialize(raw.view,error)&&error=="Design snapshot has live borrowers"&&b.design()==bindings);++guards;
 auto* vm=dh2_script_vm_create_empty(8*1024*1024);check(vm&&!dh2_script_vm_open_source_libraries(vm)&&dh2_script_vm_stack_size(vm)==5&&b.bind(vm)==0);
 for(const auto& r:*b.registrations())for(unsigned i=0;i<r.members->count;++i){auto name=quoted(r.members->names[i]);load(vm,"a=GetPyStruct("+quoted(r.group)+","+name+");b=GetPyOID("+quoted(r.group)+","+name+")");check(get(vm,"a").number==float(i)&&get(vm,"b").number==float(i));VMchecks+=2;}
 load(vm,"missing=GetPyOID('not_registered','absent');known_ok,known_error=pcall(GetPyOID,'ItemTable','absent'); cap=GetPyCst('CharacterDesign','MaxLevelDVeryHard');attack=GetPyCst('AIStates','Attack');nul=GetPyOID('CharacterTable'..string.char(0)..'tail','KnightPlayerBase'..string.char(0)..'tail');case=GetPyOID('characterproperties','SkillTree')");
 check(get(vm,"missing").number==-1&&!get(vm,"known_ok").boolean&&get(vm,"known_error").type==4&&get(vm,"cap").number==100&&get(vm,"attack").number==5&&get(vm,"nul").number==263&&get(vm,"case").number==-1);VMchecks+=7;
 dh2::data::PropertyState props;dh2::data::reset_properties(*b.rules(),props,&b.characters()->rows[263]);auto property=dh2::data::property_view(*b.rules(),props);check(dh2_class_recalc_base(b.class_rows()->data(),b.class_rows()->size(),props.base.data(),&property)==0);
 LevelModel32 model{};check(b.level_model(model,property,error)&&model.classes==b.class_rows()->data()&&model.design==bindings);unsigned debug_calls=0;LevelBindings48 level{&model,{&debug_calls,&debug},{0,0,0}};dh2::data::PropertySheet temporary{};PropertyBindings48 p{{props.resolved.data(),224,0},{temporary.data(),224,0},nullptr,nullptr};
 check(!dh2_character_property_bind(vm,&p)&&!dh2_character_level_bind(vm,&level)&&!dh2_script_scalar_bind(vm,nullptr));
 auto expected=props;auto expected_view=dh2::data::property_view(*b.rules(),expected);LevelModel32 expected_model{};check(b.level_model(expected_model,expected_view,error));LevelServices16 services{&debug_calls,&debug};check(dh2_character_set_level(&expected_model,1024,&services)==0);
 load(vm,"skill=GetProp(GetPyStruct('CharacterProperties','SkillTree'));returns=select('#',SetLevel(ToFixed(4)));level=FromFixed(GetProp(19));health=GetProp(36);mana=GetProp(41)");check(props.base==expected.base&&props.saved==expected.saved&&props.gear==expected.gear&&props.resolved==expected.resolved);check(get(vm,"skill").number==float(expected.resolved[28])&&get(vm,"returns").number==0&&get(vm,"level").number==4&&get(vm,"health").number==float(expected.resolved[36])&&get(vm,"mana").number==float(expected.resolved[41]));VMchecks+=5;
 check(dh2_script_vm_stack_size(vm)==5);dh2_script_vm_destroy(vm);
 // Initialization is atomic both without and with a previously valid snapshot.
 b={};check(!owner.initialize(raw.view,error)&&owner.ready());++guards;auto preserved=owner.borrow();check(query(preserved,1,"CharacterTable","KnightPlayerBase")==263);preserved={};
 auto valid=inputs(argv[1]);valid.refresh();for(unsigned mode=0;mode<8;++mode){auto bad=valid.view;if(mode==0)bad.characters.records={nullptr,1};if(mode==1)bad.levels.records.size--;if(mode==2)bad.ai.schema.size--;if(mode==3)bad.constants=nullptr;if(mode==4)bad.constant_count=4097;if(mode==5)bad.classes.records.size=9*1024*1024;if(mode==6)bad.characters.records.size=1;if(mode==7)bad.levels.schema.size=1;check(!owner.initialize(bad,error)&&owner.ready());auto saved=owner.borrow();check(query(saved,1,"CharacterTable","KnightPlayerBase")==263);++guards;}
 auto huge=valid.view;std::vector<dh2::data::Bytes> too_big(9,{valid.constants[0].data(),8*1024*1024});huge.constants=too_big.data();huge.constant_count=too_big.size();check(!owner.initialize(huge,error)&&error=="Design snapshot size outside limit");++guards;
 check(owner.initialize(valid.view,error));auto next=owner.borrow();check(query(next,1,"AITable","Player")==44);next={};
 // Source constant reload order is caller-supplied, not Application order.
 auto override_constants=valid.constants;auto proof=[](int value){Raw r={1,0,0,0,5,0,0,0,'P','r','o','o','f',1,0,0,0,3,0,0,0,'k','e','y'};for(unsigned i=0;i<4;++i)r.push_back(std::uint32_t(value)>>(8*i));return r;};
 override_constants.push_back(proof(17));override_constants.push_back(proof(29));std::vector<dh2::data::Bytes> override_views;for(auto& r:override_constants)override_views.push_back({r.data(),r.size()});auto override_input=valid.view;override_input.constants=override_views.data();override_input.constant_count=override_views.size();check(owner.initialize(override_input,error));auto ordered=owner.borrow();check(query(ordered,0,"Proof","key")==29);ordered={};std::swap(override_views[26],override_views[27]);check(owner.initialize(override_input,error));ordered=owner.borrow();check(query(ordered,0,"Proof","key")==17);ordered={};
 // A genuine positive source name-stop is distinct from malformed delivery:
 // prefix assignments are retained and its projection is exposed unchanged.
 auto stopped=proof(17);stopped[0]=2;stopped.insert(stopped.end(),{0,1,0,0});stopped.insert(stopped.end(),256,'x');
 override_views.resize(26);override_views.push_back({stopped.data(),stopped.size()});override_input.constants=override_views.data();override_input.constant_count=override_views.size();check(owner.initialize(override_input,error));auto prefix=owner.borrow();check(query(prefix,0,"Proof","key")==17);auto& stop=prefix.constant_loads()->back();check(stop.status==1&&stop.source.source_name_stop==1&&stop.source.assignments==1&&stop.source.groups_complete==1&&stop.source.consumed==stopped.size()-1);prefix={};++guards;
 Raw bad_constant={1,0,0};override_views.push_back({bad_constant.data(),bad_constant.size()});override_input.constants=override_views.data();override_input.constant_count=override_views.size();check(!owner.initialize(override_input,error)&&owner.ready());prefix=owner.borrow();check(query(prefix,0,"Proof","key")==17);prefix={};++guards;
 auto no_constants=valid.view;no_constants.constants=nullptr;no_constants.constant_count=0;check(owner.initialize(no_constants,error));auto no_cst=owner.borrow();check(no_cst.constant_loads()->empty()&&query(no_cst,0,"AIStates","Attack")==0);no_cst={};++guards;
 CharacterGameDesign::Borrow retained;{CharacterGameDesign temporary_owner;check(temporary_owner.initialize(valid.view,error));retained=temporary_owner.borrow();}
 auto* close_vm=dh2_script_vm_create(8*1024*1024);unsigned finalizers=0;check(close_vm&&!retained.bind(close_vm)&&!dh2_script_vm_bind_source_values(close_vm,"Observe",&finalizer,&finalizers));load(close_vm,"held=newproxy(true);getmetatable(held).__gc=function() Observe(GetPyOID('CharacterTable','KnightPlayerBase'),GetPyCst('CharacterDesign','MaxLevelDVeryHard')) end");dh2_script_vm_destroy(close_vm);check(finalizers==1);++VMchecks;
 CharacterGameDesign::Borrow empty;LevelModel32 sentinel{};sentinel.reserved=77;auto old=sentinel;check(!empty.level_model(sentinel,property,error)&&!std::memcmp(&sentinel,&old,sizeof old)&&empty.bind(nullptr)==-1);++guards;
 std::cout<<"{\"validation\":\"PASS\",\"original_authored_names_compared\":"<<original_names<<",\"registered_namespaces\":6,\"unsupported_catalog_deliveries\":"<<unsupported<<",\"real_constant_loads\":26,\"real_constant_assignments\":"<<assignments<<",\"native_lookup_checks\":"<<lookupchecks<<",\"actual_VM_checks\":"<<VMchecks<<",\"native_atomic_and_lifetime_guards\":"<<guards<<",\"actual_finalizers\":"<<finalizers<<",\"checks\":"<<checks<<",\"mismatches\":0,\"runtime_library\":\""<<origin(reinterpret_cast<void*>(dh2_script_vm_create))<<"\",\"data_library\":\""<<origin(reinterpret_cast<void*>(dh2_game_design_tables_lookup))<<"\",\"world_library\":\""<<origin(reinterpret_cast<void*>(dh2_character_set_level))<<"\"}\n";
 }catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}}
