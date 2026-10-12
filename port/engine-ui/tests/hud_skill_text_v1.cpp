#include "../hud_text_v1.hpp"
#include "../../level-world/character_skill_properties_v1.hpp"
#include "../../level-world/character_skill_info_v1.hpp"
#include "../../level-world/character_hud_skill_text_v1.hpp"
#include "../../level-world/character_property_bindings.hpp"
#include "../../game-data/skill_tables.hpp"
#include "../../game-data/game_design_tables.hpp"
#include "../../script-runtime/script_design_bindings.h"
#include "../../script-runtime/script_constants.hpp"
#include "../../script-runtime/script_return_observer_v1.h"
#include "../../script-runtime/script_scalar_bindings.h"
#include "../../level-world/character_design_services.hpp"
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <vector>
using namespace dh2;
using namespace dh2::ui;
using namespace dh2::character::skills;
static void check(bool b,const char* e){if(!b)throw std::runtime_error(e);}
static std::vector<std::uint8_t> file(const std::string& p){std::ifstream f(p,std::ios::binary);check(bool(f),p.c_str());return {std::istreambuf_iterator<char>(f),{}};}
static data::Bytes bytes(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
static std::uint32_t word(std::istream& f){std::uint32_t v;check(bool(f.read(reinterpret_cast<char*>(&v),4)),"Gold truncated");return v;}
static std::string text(std::istream& f){auto n=word(f);check(n<1048576,"Gold text bounds");std::string s(n,0);check(bool(f.read(s.data(),n)),"Gold text truncated");return s;}
struct Environment {
 std::string asset;
 dh2_script_constants* constants=dh2_script_constants_create();
 character::DebugSwitches* debug=dh2_character_debug_create();
 data::GameDesignTables registry;
 dh2_script_vm* vm=nullptr;
 unsigned opened{},closed{},calls{},first_returns{};
 ~Environment(){dh2_script_vm_destroy(vm);dh2_script_constants_destroy(constants);dh2_character_debug_destroy(debug);}
};
static int design(void* p,std::uint32_t kind,const char* group,const char* name,std::int32_t* v){auto& e=*static_cast<Environment*>(p);if(!kind)return dh2_script_constants_get(e.constants,group,name,v);auto r=e.registry.view();return dh2_game_design_tables_lookup(&r,kind,group,name,v);}
static bool constant(void* p,const char* g,const char* k,std::uint32_t& v,std::string&){std::int32_t i;if(design(p,0,g,k,&i))return false;std::memcpy(&v,&i,4);return true;}
static bool open(void* p,const char* uri,bool& found,std::vector<std::uint8_t>& out,std::uintptr_t& lease,std::string&){auto& e=*static_cast<Environment*>(p);auto* f=std::fopen((e.asset+"/original-cache/data/"+uri).c_str(),"rb");found=f!=nullptr;if(!f)return true;lease=reinterpret_cast<std::uintptr_t>(f);++e.opened;check(!std::fseek(f,0,SEEK_END),"Read seek");auto n=std::ftell(f);check(n>=0&&!std::fseek(f,0,SEEK_SET),"Read size");out.resize(n);check(std::fread(out.data(),1,out.size(),f)==out.size(),"Read data");return true;}
static bool close(void* p,std::uintptr_t lease,std::string&){++static_cast<Environment*>(p)->closed;return !std::fclose(reinterpret_cast<FILE*>(lease));}
static int debug_open(void*,const char*,std::uintptr_t*){return 0;} // Missing private file snapshot.
static int debug_close(void*,std::uintptr_t){return -1;}
static bool debug(void* p,const char* key,std::string&){auto& e=*static_cast<Environment*>(p);character::DebugFileServices24 s{nullptr,debug_open,debug_close};unsigned v;return dh2_character_debug_load(e.debug,&s)==1&&dh2_character_debug_get(&v,e.debug,key,&s)==1;}
static int observe(void* p,const dh2_script_first_return_v1* v,char*,std::size_t){auto& r=*static_cast<SkillInfoResponseV1*>(p);r.return_count=v->count;r.first_type=v->type;r.first_number=v->number;return 0;}
static int info(void* p,const SkillInfoRequestV1* q,SkillInfoResponseV1* r){auto& e=*static_cast<Environment*>(p);*r={};++e.calls;if(q->operation==skill_info_active_v1){r->active=1;return 0;}if(q->operation==skill_info_timer_v1)return -1;
 dh2_script_value a[2]{};std::uint32_t n;
 if(q->operation==skill_info_set_v1){a[0].type=DH2_SCRIPT_STRING;a[0].text=q->instance->script;a[0].text_bytes=std::strlen(a[0].text);a[1].type=DH2_SCRIPT_NUMBER;a[1].number=q->instance->argument_index;n=2;}
 else {a[0].type=DH2_SCRIPT_NUMBER;a[0].number=static_cast<float>(q->level);n=1;}
 int status=dh2_script_vm_call_first_source_v1(e.vm,q->name,a,n,observe,r);if(status<0)return -1;r->source_error=status!=0;++e.first_returns;return 0;
}
static dh2_script_value string(const std::string& s){dh2_script_value v{};v.type=DH2_SCRIPT_STRING;v.text=s.c_str();v.text_bytes=s.size();return v;}
struct Actor {State40* state;const SkillInfoServicesV1* services;data::PropertySheet* temporary;};
static bool resolve_actor(void* p,std::uintptr_t id,State40*& s,const SkillInfoServicesV1*& services,character::PropertySheet16& temporary){auto& a=*static_cast<Actor*>(p);if(id!=1)return false;s=a.state;services=a.services;temporary={a.temporary->data(),224,0};return true;}
static int external_sheet(void* p,std::uintptr_t id,std::int32_t** out){if(id!=7)return -1;*out=static_cast<data::PropertySheet*>(p)->data();return 0;}
int main(int argc,char** argv){try{
 check(argc==4,"Usage: hud_skill_text gold assets extracted-scripts");Environment e;e.asset=argv[2];check(e.constants&&e.debug,"Owners");
 std::string err;auto data_dir=e.asset+"/data/";data::CharacterTable characters;auto c=file(data_dir+"character_properties_pyarray.bin"),cn=file(data_dir+"character_properties_pyarraynames.bin"),cs=file(data_dir+"character_properties_pystructnames.bin");check(data::load_characters(bytes(c),bytes(cn),bytes(cs),characters,err),err.c_str());data::PropertyRules rules;check(data::load_property_rules(characters,rules,err),err.c_str());data::ClassTables classes;auto r=file(data_dir+"character_classes_pyarray.bin"),rn=file(data_dir+"character_classes_pyarraynames.bin"),rs=file(data_dir+"character_classes_pystructnames.bin");check(data::load_classes(bytes(r),bytes(rn),bytes(rs),classes,err),err.c_str());
 data::SkillTables skill_owner;auto sk=file(data_dir+"skills_pyarray.bin"),sn=file(data_dir+"skills_pyarraynames.bin"),ss=file(data_dir+"skills_pystructnames.bin");check(skill_owner.load(bytes(sk),bytes(sn),bytes(ss),err),err.c_str());auto skills=skill_owner.borrow();
 std::vector<const char*> fields,names;for(auto& s:characters.fields)fields.push_back(s.c_str());for(auto& s:classes.names)names.push_back(s.c_str());data::DesignNames16 props{fields.data(),static_cast<std::uint32_t>(fields.size()),0},rows{names.data(),static_cast<std::uint32_t>(names.size()),0};check(e.registry.register_table("CharacterProperties",&props,err)&&e.registry.register_table("ClassTable",&rows,err),err.c_str());
 for(auto path:{e.asset+"/original-cache/data/pydata/common_text_pycst.bin",data_dir+"fonts_pycst.bin"}){auto b=file(path);dh2_script_constants_reload result{};check(!dh2_script_constants_load(e.constants,b.data(),b.size(),&result)&&result.consumed==b.size(),"Real constants load");}
 HudTextV1 text_owner;auto t=file(e.asset+"/original-cache/data/pydata/common_text_pyarray.bin"),tn=file(e.asset+"/original-cache/data/pydata/common_text_pyarraynames.bin"),ts=file(e.asset+"/original-cache/data/pydata/common_text_pystructnames.bin");check(text_owner.load({t.data(),t.size()},{tn.data(),tn.size()},{ts.data(),ts.size()},err)&&text_owner.switch_pack(0,false,err),err.c_str());LocalizationServices loc{&e,open,close,debug,constant,nullptr,nullptr};HudTextEnvironmentV1 text_env{loc};
 e.vm=dh2_script_vm_create(16u*1024u*1024u);check(e.vm,"VM");dh2_script_design_bindings db{&e,design,0};check(!dh2_script_design_bind(e.vm,&db)&&!dh2_script_scalar_bind(e.vm,nullptr),"Real design/scalar bindings");
 data::PropertyState state;data::reset_properties(rules,state);auto view=data::property_view(rules,state);data::PropertySheet temporary;SkillPropertyBindingsV1 pb{&view,&temporary,&classes};check(!skill_property_bind_v1(e.vm,&pb),"Property bindings");character::PropertyBindings48 getters{{state.resolved.data(),224,0},{temporary.data(),224,0},nullptr,nullptr};check(!dh2_character_property_bind(e.vm,&getters),"GetProp binding");
 auto common=file(std::string(argv[3])+"/_commons.luac");check(!dh2_script_vm_load(e.vm,common.data(),common.size(),"data/scripts/skills/_commons.luac"),dh2_script_vm_error(e.vm));
 for(const auto& script:{std::string("prince_mage_staffmaster"),std::string("prince_warrior_hardiness")}){dh2_script_value args[2]{string(script),{}};args[1].type=DH2_SCRIPT_NUMBER;args[1].number=1;std::uint32_t count;check(!dh2_script_vm_call(e.vm,"DeclareSkill",args,2,nullptr,0,&count),"DeclareSkill");auto b=file(std::string(argv[3])+"/"+script+".luac");check(!dh2_script_vm_load(e.vm,b.data(),b.size(),script.c_str()),dh2_script_vm_error(e.vm));}
 std::ifstream gold(argv[1],std::ios::binary);check(word(gold)==0x31435348,"Gold magic");auto cases=word(gold);std::size_t compared=0,formatted=0;for(unsigned i=0;i<cases;++i){auto size=word(gold);auto begin=gold.tellg();auto script=text(gold),actor=text(gold),class_name=text(gold);auto actorid=word(gold),classid=word(gold),level=word(gold);check(actorid<characters.rows.size()&&characters.names[actorid]==actor&&classid<classes.names.size()&&classes.names[classid]==class_name,"Real cache identities");data::PropertySheet expected;check(bool(gold.read(reinterpret_cast<char*>(expected.data()),896)),"Gold sheet");auto expected_current=text(gold),expected_next=text(gold);check(gold.tellg()-begin==static_cast<std::streamoff>(size),"Gold record size");state.resolved=characters.rows[actorid];temporary.fill(0x55555555);Instance32 instance{1,script.c_str(),0,0,0,0};Instance32* items[]{&instance};State40 owner{1,{items,1,0},{}};SkillInfoServicesV1 svc{&e,info};Actor owned{&owner,&svc,&temporary};character::CharacterHudSkillTextV1 frame(text_owner,text_env,&owned,resolve_actor);HudInitRequest64 info_request{};info_request.operation=HudInitOperation::skill_info;info_request.subject=1;info_request.value=level;HudInitResponse32 info_response{};check(frame.query(info_request,info_response,err)==1,err.c_str());check(info_response.fraction==0&&temporary==expected&&state.resolved[172]==static_cast<int>(level*256),"Original skill script whole sheet");compared+=224;
 auto found=std::find_if(skills.skills().begin(),skills.skills().end(),[&](auto& r){return r.script==script;});check(found!=skills.skills().end(),"Actual Skill row");for(unsigned p=0;p<2;++p){HudInitRequest64 q{};HudInitResponse32 r{};q.operation=HudInitOperation::arguments_create;check(frame.query(q,r,err)==1,err.c_str());auto arguments=r.identity;
for(auto id:found->display_props){q={};r={};q.operation=HudInitOperation::property;q.subject=1;q.index=id;check(frame.query(q,r,err)==1,err.c_str());auto value=r.value;q={};q.operation=HudInitOperation::arguments_append;q.subject=arguments;q.value=value>>8;q.number=static_cast<double>(static_cast<float>(value)*(1.f/256.f));check(frame.query(q,r,err)==1,err.c_str());}
q={};q.operation=HudInitOperation::string_symbol;auto w=found->scalar.words[p?17:12];std::memcpy(&q.value,&w,4);check(frame.query(q,r,err)==1,err.c_str());auto* raw=r.text;q={};q.operation=HudInitOperation::parse_text;q.subject=arguments;q.text=raw;check(frame.query(q,r,err)==1,err.c_str());check(r.text&&std::string(r.text)==(p?expected_next:expected_current),"Original localized skill detail string");++formatted;}}
 check(gold.peek()==EOF&&e.opened==e.closed,"Gold/lease lifetime");unsigned guards=0;{character::CharacterHudSkillTextV1 first_frame(text_owner,text_env,nullptr,nullptr),second_frame(text_owner,text_env,nullptr,nullptr);HudInitRequest64 q{};HudInitResponse32 r{};q.operation=HudInitOperation::arguments_create;check(first_frame.query(q,r,err)==1,"Owned VarArgs");q.subject=r.identity;q.operation=HudInitOperation::arguments_append;check(second_frame.query(q,r,err)==0,"Foreign VarArgs rejection");++guards;q.subject=0x1234;check(first_frame.query(q,r,err)==0,"Unknown VarArgs rejection");++guards;q.operation=HudInitOperation::property;q.subject=1;check(first_frame.query(q,r,err)==0,"Missing live actor rejection");++guards;}
{dh2_script_value args[2]{};args[0].type=DH2_SCRIPT_NUMBER;args[0].number=0;std::uint32_t n=99;char diagnostic[256]{};check(skill_apply_class_v1(&pb,args,1,nullptr,0,&n,diagnostic,sizeof diagnostic)==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&n==0,"Uncached class requires genuine provider");++guards;pb.context=&temporary;pb.external=external_sheet;args[0].type=DH2_SCRIPT_IDENTITY;args[0].identity=7;temporary.fill(19);check(skill_clear_properties_v1(&pb,args,1,nullptr,0,&n,diagnostic,sizeof diagnostic)==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&temporary==rules.defaults,"External reset prefix before required recalculation");++guards;args[0].identity=0;temporary.fill(19);check(!skill_clear_properties_v1(&pb,args,1,nullptr,0,&n,diagnostic,sizeof diagnostic)&&temporary[0]==19,"Null sheet source no-op");++guards;}
dh2_script_vm_destroy(e.vm);e.vm=nullptr;std::printf("{\"validation\":\"PASS\",\"real_skill_script_cases\":%u,\"original_sheet_words\":%zu,\"original_formatted_strings\":%zu,\"actual_vm_calls\":%u,\"opened\":%u,\"closed\":%u,\"ownership_and_required_guards\":%u,\"mismatches\":0}\n",cases,compared,formatted,e.first_returns,e.opened,e.closed,guards);return 0;
 }catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}}
