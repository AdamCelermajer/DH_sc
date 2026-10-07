#include "../character_target_bindings.hpp"
#include "../character_target_providers.hpp"
#include "../../game-data/ai.hpp"
#include "../../script-runtime/script_function_alias.h"
#include <array>
#include <cstring>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace dh2::character;
static unsigned checks=0,callbacks=0,vm_checks=0,guards=0;
static void check(bool v){++checks;if(!v)throw std::runtime_error("target audit check "+std::to_string(checks));}
static std::vector<std::uint8_t> file(const std::string& p){std::ifstream s(p,std::ios::binary);check(bool(s));return {std::istreambuf_iterator<char>(s),{}};}
static std::uint32_t word(const std::vector<std::uint8_t>& b,std::size_t& at){check(at+4<=b.size());std::uint32_t v;std::memcpy(&v,b.data()+at,4);at+=4;return v;}
static const char* origin(void* p){Dl_info d{};check(dladdr(p,&d));return d.dli_fname;}
struct Fixture {
 std::array<std::uint32_t,47> row{};
 TargetState48 state{};std::array<TargetOwner16,5> owners{};TargetBindings48 bindings{};
 std::array<dh2::character::CombatProperties896,5> sheets{};
 std::array<dh2::target_providers::Character32,5> objects{};
 std::array<float,24> positions{};std::vector<std::int32_t> types;
 dh2::data::AiTables* ai=nullptr;bool nested=false,fail=false,nested_lua=false;
 std::vector<std::array<std::uint32_t,11>> trace;
 std::uintptr_t pointer(unsigned i)const{return i?0x100000000ull+i*0x10000:0;}
 unsigned id(std::uintptr_t p)const{if(!p)return 0;for(unsigned i=1;i<5;++i)if(pointer(i)==p)return i;throw std::runtime_error("unowned target identity");}
 unsigned owner()const{for(unsigned i=1;i<5;++i)if(&owners[i]==state.owner)return i;throw std::runtime_error("unowned owner view");}
 std::array<std::uint32_t,9> snapshot()const{return {owner(),id(state.candidate),id(state.target),id(state.last_target),state.alive,state.sight,state.changed,owners[1].word14d0,owners[4].word14d0};}
 void stamp(unsigned op,unsigned subject=0){auto v=snapshot();std::array<std::uint32_t,11> r{};r[0]=op;r[1]=subject;std::copy(v.begin(),v.end(),r.begin()+2);trace.push_back(r);++callbacks;}
 void change(unsigned op){
  const unsigned mask=row[15];
  if(op==1){if(mask&1)state.target=pointer(3);if(mask&2)state.owner=&owners[4];if(mask&4)state.last_target=pointer(3);}
  if(op==3){if(mask&8)state.target=pointer(3);if(mask&16)state.target=0;}
  if(op==4&&(mask&32))state.owner=&owners[4];
  const unsigned trigger=row[16]==1||row[16]==3?1:row[16]==2?3:row[16]==4?6:99;
  if(trigger==op&&!nested){nested=true;stamp(7);check(!dh2_character_ai_set_target(&state,pointer(3),row[16]==3?0:1,&bindings.services));stamp(8);}
 }
 const float* position(unsigned index)const{const unsigned bits=(row[21]>>(2*(index-1)))&3;return positions.data()+(bits==3?12:0)+(index-1)*3;}
 static int invoke(void* p,TargetState48*,const TargetRequest24* r,std::uint32_t* out){
  auto& f=*static_cast<Fixture*>(p);unsigned op=r->service,subject=f.id(r->subject);if(op==1&&!std::strcmp(r->text,"isTracingCharAITarget"))op=6;
  f.stamp(op,subject);f.change(op);if(f.fail)return 1;
  *out=0;
  if(r->service==target_debug_query){check(!std::strcmp(r->text,op==1?"IsTracingCharAITarget":"isTracingCharAITarget"));*out=f.row[op==1?11:12];
   if(f.nested_lua&&!f.nested){f.nested=true;const auto* scope=f.bindings.scope;check(scope&&dh2_script_callback_scope_valid(scope));check(!dh2_script_callback_call_discard_source(scope,"TargetMutation",nullptr,0));}
  }else if(r->service==target_owner_ai_id){std::int32_t v;check(!dh2_character_target_ai_id(&v,f.sheets[subject].words,std::uint32_t(f.ai->rows.size())));*out=std::uint32_t(v);
  }else if(r->service==target_virtual_dead){std::int32_t value;const dh2::target_providers::Types16 types{f.types.data(),std::uint32_t(f.types.size()),0};check(!dh2::target_providers::dh2_character_target_query(&value,dh2::target_providers::is_dead,&f.objects[subject],nullptr,&types,nullptr));*out=std::uint32_t(value);
  }else if(r->service==target_in_sight){subject=subject?subject:f.id(f.state.target);if(subject){const auto owner=f.owner();f.stamp(2,owner);std::int32_t ai;check(!dh2_character_target_ai_id(&ai,f.sheets[owner].words,std::uint32_t(f.ai->rows.size())));check(!dh2_character_target_sight(out,f.position(owner),f.position(subject),f.ai->rows.at(ai).view_radius));}}
  return 0;
 }
 void configure(dh2::data::AiTables& tables){
  ai=&tables;nested=false;fail=false;nested_lua=false;trace.clear();
  std::memcpy(positions.data(),row.data()+23,sizeof positions);types.clear();for(const auto& r:tables.rows)types.push_back(r.type);
  for(unsigned i=1;i<5;++i){owners[i]={pointer(i),std::uint16_t(row[i==1?9:10]),0,0};sheets[i]={};sheets[i].words[1]=std::int32_t(row[i==1?19:20]);objects[i]={pointer(i),&sheets[i],"Crypt",0,std::uint8_t(i==2?row[13]:i==3?row[14]:0),0,1,0};}
  state={0x200000000ull,&owners[1],pointer(row[3]),pointer(row[4]),pointer(row[5]),std::uint8_t(row[6]),std::uint8_t(row[7]),std::uint8_t(row[8]),0,0};bindings={&state,{this,invoke},nullptr,{0,0}};
 }
 std::vector<std::uint32_t> execute(){
  std::vector<dh2_script_value> args(std::max(1u,row[18]));for(auto& v:args){v={};v.type=row[17];v.identity=pointer(row[1]);}
  switch(row[0]){
   case 0:check(!dh2_character_ai_set_target(&state,pointer(row[1]),row[2],&bindings.services));break;
   case 1:check(!dh2_character_clear_target(&state,&bindings.services));break;
   case 2:check(!dh2_character_target_set_values(&bindings,args.data(),row[18]));break;
   case 3:{dh2_script_value value{};std::uint32_t count=99;char error[128]{};check(!dh2_character_target_has_lua(&bindings,args.data(),row[18],&value,1,&count,error,sizeof error));check(count==1);return {value.type,value.boolean};}
   case 4:{std::uintptr_t target;check(!dh2_character_target_identity(&target,&state));return {7,id(target)};}
  }return {};
 }
};
static void load(dh2_script_vm* vm,const char* text){check(!dh2_script_vm_load_source_file(vm,text,std::strlen(text)));}
static dh2_script_value global(dh2_script_vm* vm,const char* name){dh2_script_value v{};check(!dh2_script_vm_get_global(vm,name,&v));return v;}
int main(int argc,char** argv){try{
 check(argc==3);const auto gold=file(argv[1]);check(gold.size()>=16&&!std::memcmp(gold.data(),"CTB1",4));std::size_t at=4;const auto cases=word(gold,at);check(word(gold,at)==47);const auto nr=word(gold,at);std::vector<std::uint32_t> radii(nr);for(auto& x:radii)x=word(gold,at);
 const std::string assets=argv[2];const char* names[]={"ai_pyarray.bin","ai_pyarraynames.bin","ai_pystructnames.bin","ai_factions_pyarray.bin","ai_factions_pyarraynames.bin","ai_factions_pystructnames.bin"};std::array<std::vector<std::uint8_t>,6> blobs;for(unsigned i=0;i<6;++i)blobs[i]=file(assets+"/"+names[i]);dh2::data::AiTables ai;std::string error;check(dh2::data::load_ai({blobs[0].data(),blobs[0].size()},{blobs[1].data(),blobs[1].size()},{blobs[2].data(),blobs[2].size()},{blobs[3].data(),blobs[3].size()},{blobs[4].data(),blobs[4].size()},{blobs[5].data(),blobs[5].size()},ai,error));check(ai.rows.size()==nr);
 for(unsigned i=0;i<nr;++i){std::uint32_t b;std::memcpy(&b,&ai.rows[i].view_radius,4);check(b==radii[i]);}
 Fixture f;std::uint32_t expected_callbacks=0;
 for(unsigned i=0;i<cases;++i){for(auto& x:f.row)x=word(gold,at);f.configure(ai);const auto rc=word(gold,at);std::vector<std::uint32_t> results(rc);for(auto& x:results)x=word(gold,at);std::array<std::uint32_t,9> state{};for(auto& x:state)x=word(gold,at);const auto tc=word(gold,at);std::vector<std::array<std::uint32_t,11>> trace(tc);for(auto& r:trace)for(auto& x:r)x=word(gold,at);check(f.execute()==results);check(f.snapshot()==state);check(f.trace==trace);expected_callbacks+=tc;}
 check(at==gold.size());check(callbacks==expected_callbacks);
 f.configure(ai);auto original=f.state;check(dh2_character_ai_set_target(reinterpret_cast<TargetState48*>(reinterpret_cast<char*>(&f.state)+1),0,0,&f.bindings.services)==1);check(!std::memcmp(&original,&f.state,sizeof original));++guards;
 f.state.owner=reinterpret_cast<TargetOwner16*>(reinterpret_cast<char*>(&f.owners[1])+1);auto malformed=f.state;check(dh2_character_ai_set_target(&f.state,0,0,&f.bindings.services)==1&&!std::memcmp(&malformed,&f.state,sizeof malformed));++guards;
 f.configure(ai);f.state.reserved=1;malformed=f.state;check(dh2_character_clear_target(&f.state,&f.bindings.services)==1&&!std::memcmp(&malformed,&f.state,sizeof malformed));++guards;
 f.configure(ai);f.fail=true;check(dh2_character_ai_set_target(&f.state,f.pointer(2),0,&f.bindings.services)==2&&f.state.candidate==f.pointer(2)&&f.state.target==original.target);++guards;
 f.configure(ai);check(!dh2_character_ai_set_target(&f.state,f.pointer(2),1,nullptr)&&f.state.target==f.pointer(2));++guards;
 f.configure(ai);dh2_script_value v{};v.type=7;v.identity=f.pointer(2);v.reserved=1;original=f.state;check(dh2_character_target_set_values(&f.bindings,&v,1)==1&&!std::memcmp(&original,&f.state,sizeof original));++guards;
 auto* vm=dh2_script_vm_create_empty(8*1024*1024);check(vm&&dh2_script_vm_open_source_libraries(vm)==0);f.configure(ai);f.row[15]=f.row[16]=0;check(!dh2_character_target_bind(vm,&f.bindings));++vm_checks;
 load(vm,"initial=HasTarget(); rejected=SetTarget(nil); rejected_string=SetTarget('Prince'); rejected_num=SetTarget(3); zero_returns=select('#',ClearTarget()); clear_has=HasTarget(); missing_projection=(GetTarget==nil)");
 check(global(vm,"initial").boolean==bool(f.row[4]));check(global(vm,"zero_returns").number==0);check(!global(vm,"clear_has").boolean);check(global(vm,"missing_projection").boolean);vm_checks+=4;
 // Actual firstNUL _this projection and scoped same-VM nested callback. The
 // userdata itself comes from an explicit caller identity (no fake GetTarget).
 load(vm,"function TargetMutation() ClearTarget() end; function InstallTarget(x) SetTarget(setmetatable({}, {__index=function(_,key) if key=='_this' then return x end end})); final_has=HasTarget() end");
 f.nested=false;f.nested_lua=true;dh2_script_value identity{};identity.type=DH2_SCRIPT_IDENTITY;identity.identity=f.pointer(2);std::uint32_t returns=99;check(!dh2_script_vm_call(vm,"InstallTarget",&identity,1,nullptr,0,&returns));check(!returns&&f.state.target==identity.identity&&f.bindings.scope==nullptr&&global(vm,"final_has").boolean);vm_checks+=3;
 // Generic same-VM capability remains rejected and the copied scoped handle
 // is not exposed as a permanent owner permission.
 dh2_script_vm_destroy(vm);
 std::cout<<"{\"validation\":\"PASS\",\"original_gold_cases\":"<<cases<<",\"ordered_gold_services\":"<<expected_callbacks<<",\"actual_ai_rows\":"<<nr<<",\"vm_checks\":"<<vm_checks<<",\"guards\":"<<guards<<",\"checks\":"<<checks<<",\"mismatches\":0,\"kernel_library\":\""<<origin(reinterpret_cast<void*>(&dh2_character_ai_set_target))<<"\",\"runtime_library\":\""<<origin(reinterpret_cast<void*>(&dh2_script_vm_create_empty))<<"\",\"full_GetTarget_Lua_projection\":false}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
