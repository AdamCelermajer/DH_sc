#include "character_dead_select.hpp"
#include "character_stance.hpp"
#include "../game-data/animation_tables.hpp"
#include "../script-runtime/script_constants.hpp"
#include <array>
#include <cstring>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace {
unsigned checks=0,cases=0,prefixes=0,reentries=0,guards=0,compositions=0;
void verify(bool v,unsigned line){++checks;if(!v)throw std::runtime_error("DeadSelect case"+std::to_string(cases)+" line"+std::to_string(line));}
#define check(v) verify(v,__LINE__)
using Words=std::array<unsigned,8>;using Event=std::array<unsigned,11>;using Trace=std::vector<Event>;
unsigned word(std::istream& f){unsigned v;f.read(reinterpret_cast<char*>(&v),4);check(bool(f));return v;}
int integer(unsigned v){int r;std::memcpy(&r,&v,4);return r;}
template<std::size_t N>std::array<unsigned,N> read(std::istream&f){std::array<unsigned,N> r;for(auto&v:r)v=word(f);return r;}
Trace read_trace(std::istream&f,unsigned n){Trace t(n);for(auto&r:t)r=read<11>(f);return t;}
struct Nested {int status;Trace trace;Words state;};
struct Fixture {
 std::array<unsigned,26>x{};State state{};NativeFsm24 fsm{};std::array<StateOwnerInfo40,20> infos{};StateOwnerMachine40 machine{};DeadSelect32 view{};
 std::array<std::array<DeadAnimationRow16,20>,2> rows{};DeadSelectServices16 services{this,invoke};StateOwnerServices16 owner_services{this,unavailable};Trace trace;std::vector<Words> checkpoints;std::vector<Nested> nested;bool reentered=false;int fail=-1;
 Words snapshot()const{return {unsigned(fsm.character-1),unsigned(fsm.current_present?state.current:-1),fsm.current_present,unsigned(state.animation_override),unsigned(view.secondary_animation),state.dead_alternate,view.pending_alternate,state.elapsed_ms};}
 void reset(const std::array<unsigned,26>& config){x=config;state={};state.current=integer(x[2]);state.elapsed_ms=x[13];state.dead_alternate=x[12];state.animation_override=integer(x[10]);fsm={&state,1,x[3],0};machine={&fsm,infos.data(),20,x[3]?integer(x[2]):-1,0,0};view={&machine,rows[0].data(),x[0],x[4],integer(x[11]),0};for(unsigned j=0;j<2;++j)for(unsigned i=0;i<20;++i){auto*p=reinterpret_cast<int*>(&rows[j][i]);for(unsigned k=0;k<4;++k)p[k]=integer(x[22+k]+i*9+j*77);}for(unsigned i=0;i<20;++i)infos[i]={int(i),0,0,0,0,0,nullptr,0,0};trace.clear();checkpoints.clear();nested.clear();reentered=false;fail=-1;}
 void mutate(unsigned n){if(n!=x[14])return;fsm.character=x[15]+1;state.current=integer(x[16]);fsm.current_present=x[17];machine.current_index=x[17]?integer(x[16]):-1;view.pending_alternate=x[18];view.count=x[19];view.rows=rows[1].data();for(auto& array:rows)for(auto& row:array){row.despawn=integer(x[20]);row.despawn_great_kb=integer(x[20]+1);}}
 int event(unsigned op,unsigned mask,unsigned payload){auto s=snapshot();Event e{op,mask,payload};std::copy(s.begin(),s.end(),e.begin()+3);const auto n=trace.size();trace.push_back(e);mutate(n);
  if(n==x[21]&&!reentered){reentered=true;auto saved=std::move(trace);auto cp=std::move(checkpoints);trace.clear();checkpoints.clear();int status=dh2_character_dead_select(&view,0,0,1,&services,&owner_services);nested.push_back({status,trace,snapshot()});trace=std::move(saved);checkpoints=std::move(cp);}
  checkpoints.push_back(snapshot());return int(n)==fail?-2:0;
 }
 static int unavailable(void*,StateOwnerMachine40*,const StateOwnerRequest48*,StateOwnerResponse8*){return -1;}
 static int invoke(void*p,DeadSelect32*d,const DeadSelectRequest24*q,unsigned*out){auto&f=*static_cast<Fixture*>(p);check(d==&f.view&&q->character==f.fsm.character&&!q->reserved[0]&&!q->reserved[1]);const auto r=f.event(q->operation,q->mask,0);*out=q->operation==0?f.x[1]:q->operation==1?f.x[8]:f.x[9];return r;}
 int run(){return dh2_character_dead_select(&view,x[5],x[7]?9:0,x[6],&services,&owner_services);}
};
Fixture* active=nullptr;bool genuine=false;
using Transition=int(*)(StateOwnerMachine40*,int,int,std::uintptr_t,const StateOwnerServices16*);
using StateEvent=int(*)(StateOwnerMachine40*,int,std::uintptr_t,const StateOwnerServices16*);
Transition real_transition=nullptr;StateEvent real_event=nullptr;
std::vector<unsigned char> bytes(const std::string& path){std::ifstream f(path,std::ios::binary);check(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
dh2::data::Bytes view_bytes(const std::vector<unsigned char>&v){return {v.data(),v.size()};}
struct Real {
 CharacterStateOwner owner{0xa123456789abcdefull};std::vector<DeadAnimationRow16> rows;DeadSelect32 selection{};dh2_script_constants* constants=nullptr;unsigned index=62;StanceFacts16 stance{0,8,{0,0}};std::vector<StateOwnerRequest48> calls;
 DeadSelectServices16 services{this,invoke};StateOwnerServices16 state_services{this,state_invoke};
 ~Real(){dh2_script_constants_destroy(constants);}
 static int invoke(void*p,DeadSelect32*,const DeadSelectRequest24*q,unsigned*out){auto&r=*static_cast<Real*>(p);check(q->character==r.owner.native_fsm().character);int value=0;switch(q->operation){case dead_animation_index:*out=r.index;return 0;case dead_stance_bits:check(dh2_script_constants_get(r.constants,"AnimStancedAnim","SL__LIST_IPHONE",&value)==0);break;case dead_anim_stance:check(dh2_character_anim_stance(&value,&r.stance)==1);break;default:return -1;}*out=unsigned(value);return 0;}
 static int state_invoke(void*p,StateOwnerMachine40*m,const StateOwnerRequest48*q,StateOwnerResponse8*){auto&r=*static_cast<Real*>(p);check(m==&r.owner.machine());r.calls.push_back(*q);if(q->operation==state_owner_blur&&q->state==3){Facts f{};Services s{nullptr,[](void*,State*,const Request*){throw std::runtime_error("Idle blur unexpectedly requested service");}};return dh2_character_state_blur_body(&r.owner.state(),&f,&s)==1?0:1;}
  // Genuine Dead OnFocus, previous-state methods and AI notification backends
  // are still required. This composition never accepts a missing callback.
  return -1;
 }
};
void real_composition(const std::string& root){
 using namespace dh2::data;auto records=bytes(root+"/animations_pyarray.bin"),names=bytes(root+"/animations_pyarraynames.bin"),fields=bytes(root+"/animations_pystructnames.bin"),keys=bytes(root+"/animations_dictionary_pyarraynames.bin"),paths=bytes(root+"/animations_dictionary_pyarray.bin"),constants=bytes(root+"/animations_pycst.bin");Dictionary dictionary;AnimationTables table;std::string error;check(load_dictionary(view_bytes(keys),view_bytes(paths),dictionary,error));check(load_animation_tables(view_bytes(records),view_bytes(names),view_bytes(fields),dictionary,table,error));check(table.characters.size()==80);Real r;r.constants=dh2_script_constants_create();check(r.constants);dh2_script_constants_reload reload{};check(dh2_script_constants_load(r.constants,constants.data(),constants.size(),&reload)==0);
 for(const auto&row:table.characters)r.rows.push_back({row.fields[3][0],row.fields[4][0],row.fields[5][0],row.fields[6][0]});r.selection={&r.owner.machine(),r.rows.data(),unsigned(r.rows.size()),0,-1,0};
 for(int prior:{0,16,17,3,4,12,-1})for(unsigned force:{0u,1u}){
  auto&state=r.owner.state();auto&fsm=r.owner.native_fsm();auto&m=r.owner.machine();state={};state.current=prior;state.elapsed_ms=0xdeadbeef;fsm.current_present=prior>=0;m.current_index=prior;r.calls.clear();r.selection.pending_alternate=0;r.selection.secondary_animation=-1;
  auto result=dh2_character_dead_select(&r.selection,0,0,force,&r.services,&r.state_services);bool waiting=prior==0||prior==16||prior==17;
  if(force){check(result==-2);check(!r.calls.empty());if(waiting||prior==-1){check(r.calls[0].operation==state_owner_focus&&r.calls[0].state==12&&r.calls[0].other==-1);check(state.current==12&&fsm.current_present&&state.elapsed_ms==0);}else{check(r.calls[0].operation==state_owner_blur&&r.calls[0].state==prior);if(prior==3){check(r.calls.size()==2&&r.calls[1].operation==state_owner_focus&&r.calls[1].other==3);check(state.current==12&&state.elapsed_ms==0);}else check(state.current==prior&&state.elapsed_ms==0xdeadbeef);}}
  else if(waiting||prior==-1){check(result==1&&r.calls.empty());check(!fsm.current_present&&m.current_index==-1&&state.elapsed_ms==0xdeadbeef);}else{check(result==-2&&r.calls.size()==1&&r.calls[0].operation==state_owner_event&&r.calls[0].state==prior);check(state.current==prior);}
  check(state.animation_override==r.rows[62].died&&r.selection.secondary_animation==r.rows[62].despawn);++compositions;
 }
}
}
extern "C" int dh2_character_state_owner_transition(StateOwnerMachine40*m,int next,int event,std::uintptr_t payload,const StateOwnerServices16*s){if(genuine)return real_transition(m,next,event,payload,s);check(active&&m==&active->machine&&next==12&&event==0xc358&&(payload==0||payload==9));return active->event(3,event,bool(payload));}
extern "C" int dh2_character_state_owner_event(StateOwnerMachine40*m,int event,std::uintptr_t payload,const StateOwnerServices16*s){if(genuine)return real_event(m,event,payload,s);check(active&&m==&active->machine&&event==0xc358&&(payload==0||payload==9));return active->event(4,event,bool(payload));}
int main(int argc,char**argv){try{
 if(argc!=3)return 2;real_transition=reinterpret_cast<Transition>(dlsym(RTLD_NEXT,"dh2_character_state_owner_transition"));real_event=reinterpret_cast<StateEvent>(dlsym(RTLD_NEXT,"dh2_character_state_owner_event"));check(real_transition&&real_event);Dl_info module{},world{},data{},runtime{};check(dladdr(reinterpret_cast<void*>(dh2_character_dead_select),&module)&&dladdr(reinterpret_cast<void*>(real_transition),&world)&&dladdr(reinterpret_cast<void*>(dh2::data::load_animation_tables),&data)&&dladdr(reinterpret_cast<void*>(dh2_script_constants_get),&runtime));
 std::ifstream f(argv[1],std::ios::binary);check(bool(f));char magic[4];f.read(magic,4);check(std::string(magic,4)=="DDS1");const auto n=word(f);Fixture fixture;active=&fixture;
 for(cases=0;cases<n;++cases){auto x=read<26>(f);int status=integer(word(f));auto count=word(f);auto expected=read<8>(f);auto nested_count=word(f);auto trace=read_trace(f,count);std::vector<Nested> nested;for(unsigned i=0;i<nested_count;++i){int s=integer(word(f));unsigned size=word(f);auto state=read<8>(f);nested.push_back({s,read_trace(f,size),state});}
  fixture.reset(x);check(fixture.run()==status&&fixture.trace==trace&&fixture.snapshot()==expected&&fixture.nested.size()==nested.size());for(unsigned i=0;i<nested.size();++i){check(fixture.nested[i].status==nested[i].status&&fixture.nested[i].trace==nested[i].trace&&fixture.nested[i].state==nested[i].state);++reentries;}
  // Prefix checkpoints are recorded while replaying source-derived gold, just
  // after each delivered service, before subsequent kernel effects.
  auto checkpoints=fixture.checkpoints;if(cases<500&&nested.empty())for(unsigned i=0;i<count;++i){fixture.reset(x);fixture.fail=i;check(fixture.run()==-2);check(fixture.trace==Trace(trace.begin(),trace.begin()+i+1)&&fixture.snapshot()==checkpoints[i]);++prefixes;}
 }
 check(f.peek()==EOF);auto config=fixture.x;config[0]=20;
 for(unsigned i=0;i<8;++i){fixture.reset(config);auto* d=&fixture.view;auto* c=&fixture.services;auto* s=&fixture.owner_services;
  if(i==0)d=nullptr;else if(i==1)c=nullptr;else if(i==2)s=nullptr;else if(i==3)fixture.view.reserved=1;else if(i==4)fixture.view.pending_alternate=256;else if(i==5)fixture.view.rows=nullptr;else if(i==6)fixture.fsm.reserved=1;else fixture.machine.current_index=99;
  const auto before=fixture.snapshot();const auto raw=fixture.view;check(dh2_character_dead_select(d,0,9,1,c,s)==-1&&fixture.trace.empty()&&fixture.snapshot()==before&&std::memcmp(&raw,&fixture.view,sizeof(raw))==0);++guards;
 }
 genuine=true;real_composition(argv[2]);std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"original_cases\":"<<cases<<",\"failure_prefixes\":"<<prefixes<<",\"synchronous_reentries\":"<<reentries<<",\"atomic_guards\":"<<guards<<",\"genuine_state_owner_compositions\":"<<compositions<<",\"module_library\":\""<<module.dli_fname<<"\",\"world_library\":\""<<world.dli_fname<<"\",\"data_library\":\""<<data.dli_fname<<"\",\"runtime_library\":\""<<runtime.dli_fname<<"\",\"missing_dead_focus_backends_rejected\":true}\n";return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 3;}}
