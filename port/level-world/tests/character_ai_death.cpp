#include "character_ai_death.hpp"
#include "character_ai_events.hpp"
#include "character_design_services.hpp"
#include <array>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace dh2::character;
namespace {
unsigned checks=0,frame=0,failures=0,guards=0,real_debug_calls=0,real_ai_calls=0;
void verify(bool v,unsigned line){++checks;if(!v)throw std::runtime_error("AI death frame "+std::to_string(frame)+" line "+std::to_string(line));}
#define check(v) verify(v,__LINE__)
unsigned word(std::istream&f){unsigned v;f.read(reinterpret_cast<char*>(&v),4);check(bool(f));return v;}
int sint(unsigned v){int n;std::memcpy(&n,&v,4);return n;}
template<class T>void read(std::istream&f,T&v){f.read(reinterpret_cast<char*>(&v),sizeof(v));check(bool(f));}
std::string text(std::istream&f){std::string s(word(f),'\0');check(s.size()<4096);f.read(s.data(),s.size());check(bool(f));return s;}
struct Event {std::array<unsigned,7> fields;std::string text;std::array<unsigned,19> state;bool operator==(const Event&b)const{return fields==b.fields&&text==b.text&&state==b.state;}};
using Trace=std::vector<Event>;
Trace trace(std::istream&f){Trace v(word(f));for(auto&e:v){read(f,e.fields);e.text=text(f);read(f,e.state);}return v;}
struct Config {unsigned owner,group,active,timer0,timer1;std::array<unsigned,2>word;std::array<unsigned,6>timers;std::array<unsigned,3>targets,bytes;unsigned debug;int mutate_op;unsigned next_owner,next_active,next_timer0,next_timer1;int reentry_op;};
struct Snapshot {AIDeathState64 state;TargetState48 target;std::array<TargetOwner16,2>target_owners;std::array<std::array<Timer32,3>,2>timers;};
struct Fixture;
Fixture* current=nullptr;
using Stop=int(*)(TimerStore32*,unsigned);
Stop actual_stop(){static auto p=reinterpret_cast<Stop>(dlsym(RTLD_NEXT,"dh2_character_timer_stop"));check(p);return p;}
struct Fixture {
 std::array<TargetOwner16,2>target_owners{{{2,0,0,0},{3,0,0,0}}};TargetState48 target{1,&target_owners[0],2,3,2,41,42,43,0,0};
 std::array<std::array<Timer32,3>,2> timers{};std::array<TimerStore32,2> stores{};std::array<AIDeathOwner24,2>owners{};std::array<std::uintptr_t,51>virtuals{};AIDeathState64 state{};
 TargetServices16 targets{this,target_service};AIDeathServices16 services{this,service};Config config{};Trace events,nested;std::vector<Snapshot>snapshots;AIDeathResult24 nested_output{};bool reentered=false,genuine=false;int fail=-1;
 DebugSwitches* debug=nullptr;DebugFileServices24 files{this,open,close};std::string missing_file;
 std::array<int,224> property_words{};std::array<std::uintptr_t,51> ai_virtuals{};AIEventOwner48 event_owner{2,7,8,reinterpret_cast<std::uintptr_t>(property_words.data()),0,0,0,0};AIEventState64 event_state{};AIEventServices24 event_services{this,ai_event,63,0};AIDeathResult24 routed{};
 Fixture(){for(unsigned i=0;i<2;i++){stores[i]={timers[i].data(),3,3,i+2,0,0};owners[i]={i+2,i+8,&stores[i]};}virtuals[9]=0x12345678;ai_virtuals[9]=0x3d1000;}
 ~Fixture(){dh2_character_debug_destroy(debug);if(current==this)current=nullptr;}
 void reset(const Config&x){config=x;state={1,&owners[x.owner],&target,x.group?4u:0u,x.active?x.active+5u:0u,virtuals.data(),x.timer0,x.timer1,0};target={1,&target_owners[x.owner],x.targets[0],x.targets[1],x.targets[2],std::uint8_t(x.bytes[0]),std::uint8_t(x.bytes[1]),std::uint8_t(x.bytes[2]),0,0};for(unsigned i=0;i<2;i++){target_owners[i].word14d0=x.word[i];for(unsigned j=0;j<3;j++)timers[i][j]={j,-1,100,99,std::uint8_t(x.timers[i*3+j]),0,0,0x33,0};}events.clear();nested.clear();snapshots.clear();reentered=false;fail=-1;}
 std::array<unsigned,19> snapshot()const{unsigned owner=state.owner==&owners[1];return {owner,unsigned(state.group),unsigned(state.active),state.timer0,state.timer1,unsigned(target.candidate),unsigned(target.target),unsigned(target.last_target),target.alive,target.sight,target.changed,target_owners[0].word14d0,target_owners[1].word14d0,timers[0][0].active,timers[0][1].active,timers[0][2].active,timers[1][0].active,timers[1][1].active,timers[1][2].active};}
 void change(unsigned op){if(int(op)==config.mutate_op){state.owner=&owners[config.next_owner];target.owner=&target_owners[config.next_owner];state.active=config.next_active?config.next_active+5:0;state.timer0=config.next_timer0;state.timer1=config.next_timer1;}if(int(op)==config.reentry_op&&!reentered){reentered=true;auto outer=std::move(events);events.clear();check(dh2_character_ai_on_died(&nested_output,&state,3,&targets,&services)==1);nested=std::move(events);events=std::move(outer);}}
 bool stamp(unsigned op,unsigned subject=0,unsigned owner=0,unsigned payload=0,unsigned callee=0,unsigned operation=0,unsigned argument=0,const char*txt=""){auto n=events.size();events.push_back({{op,subject,owner,payload,callee,operation,argument},txt,snapshot()});snapshots.push_back({state,target,target_owners,timers});if(int(n)==fail)return false;change(op);return true;}
 static int service(void*p,AIDeathState64*s,const AIDeathRequest48*q){auto&f=*static_cast<Fixture*>(p);check(s==&f.state&&!q->reserved);if(!f.stamp(q->service,q->subject,q->owner,q->payload,q->callee,q->operation,q->argument))return -1;
  if(f.genuine){check(q->service==ai_death_state&&q->callee==0x3c58c8&&q->subject==f.state.owner->state_machine&&q->argument==1&&!q->payload);return -1;}// Missing actual SM_SetDeadState must not be accepted.
  return 0;// Explicit original deeper-body fixtures only.
 }
 static int target_service(void*p,TargetState48*s,const TargetRequest24*q,unsigned*out){auto&f=*static_cast<Fixture*>(p);check(s==&f.target&&!q->reserved&&q->service<=1);if(!f.stamp(20+q->service,0,0,0,0,0,0,q->text?q->text:""))return -1;
  if(f.genuine){++real_debug_calls;int r=q->service?dh2_character_debug_get(out,f.debug,q->text,&f.files):dh2_character_debug_load(f.debug,&f.files);return r==1?0:-1;}*out=q->service?f.config.debug:0;return 0;
 }
 static int open(void*p,const char*name,std::uintptr_t*out){auto&f=*static_cast<Fixture*>(p);check(std::string(name)=="DebugSwitches.savegame");errno=0;FILE*file=std::fopen(f.missing_file.c_str(),"rb");if(!file){check(errno==ENOENT);*out=0;return 0;}*out=reinterpret_cast<std::uintptr_t>(file);return 0;}
 static int close(void*,std::uintptr_t p){check(p);return std::fclose(reinterpret_cast<FILE*>(p));}
 static int ai_event(void*p,AIEventState64*,const AIEventRequest40*q,unsigned*){auto&f=*static_cast<Fixture*>(p);check(q->service==ai_event_virtual&&q->operation==0x24&&q->callee==0x3d1000&&q->event==2);++real_ai_calls;check(dh2_character_ai_on_died(&f.routed,&f.state,q->payload,&f.targets,&f.services)==-2&&f.routed.phase==5);return -1;}
};
std::string origin(void*p){Dl_info info{};check(dladdr(p,&info));return info.dli_fname;}
}
extern "C" int dh2_character_timer_stop(TimerStore32*s,unsigned id){if(current){auto&f=*current;unsigned index=s==&f.stores[1];check(s==&f.stores[index]);check(f.stamp(30,10+index,0,0,0,0,id));}return actual_stop()(s,id);}
int main(int argc,char**argv){try{
 check(argc==3);std::ifstream input(argv[1],std::ios::binary);check(word(input)==0x31444941);auto count=word(input);Fixture f;current=&f;unsigned reentries=0;
 for(frame=0;frame<count;frame++){bool wrapper=word(input);Config x;unsigned*fields[]={&x.owner,&x.group,&x.active,&x.timer0,&x.timer1};for(auto p:fields)*p=word(input);read(input,x.word);read(input,x.timers);read(input,x.targets);read(input,x.bytes);x.debug=word(input);x.mutate_op=sint(word(input));x.next_owner=word(input);x.next_active=word(input);x.next_timer0=word(input);x.next_timer1=word(input);x.reentry_op=sint(word(input));auto expected_trace=trace(input);std::array<unsigned,19> expected_state;read(input,expected_state);AIDeathResult24 expected{},out{};read(input,expected);std::array<std::array<Timer32,3>,2> expected_timers;read(input,expected_timers);auto nested_count=word(input);Trace nested;AIDeathResult24 nested_result{};if(nested_count){check(nested_count==1);nested=trace(input);read(input,nested_result);++reentries;}f.reset(x);
  auto run=[&](){return wrapper?dh2_character_ai_on_died(&out,&f.state,3,&f.targets,&f.services):dh2_character_ai_set_dead(&out,&f.state,&f.targets,&f.services);};check(run()==1);check(f.events==expected_trace&&f.snapshot()==expected_state&&std::memcmp(&out,&expected,24)==0&&std::memcmp(f.timers.data(),expected_timers.data(),sizeof(expected_timers))==0);if(nested_count)check(f.reentered&&f.nested==nested&&std::memcmp(&f.nested_output,&nested_result,24)==0);auto snapshots=f.snapshots;
  if(frame<100&&!nested_count)for(unsigned j=0;j<expected_trace.size();j++)if(expected_trace[j].fields[0]!=30){f.reset(x);f.fail=int(j);check(run()==-2);check(f.events==Trace(expected_trace.begin(),expected_trace.begin()+j+1));check(f.snapshot()==expected_trace[j].state&&std::memcmp(f.timers.data(),snapshots[j].timers.data(),sizeof(f.timers))==0);++failures;}
 }
 check(input.peek()==std::char_traits<char>::eof());Config x{};x.word={99,101};x.targets={2,3,2};x.bytes={41,42,43};x.timers={1,1,1,1,1,1};x.timer0=0;x.timer1=1;x.mutate_op=x.reentry_op=-1;f.reset(x);AIDeathResult24 out{1,2,3,4,5,6},before=out;auto snapshot=f.snapshot();auto timers=f.timers;auto reject=[&](int s){check(s==-1&&f.events.empty()&&std::memcmp(&out,&before,24)==0&&f.snapshot()==snapshot&&std::memcmp(timers.data(),f.timers.data(),sizeof(timers))==0);++guards;};
 reject(dh2_character_ai_on_died(&out,nullptr,3,&f.targets,&f.services));reject(dh2_character_ai_on_died(reinterpret_cast<AIDeathResult24*>(&f.state),&f.state,3,&f.targets,&f.services));reject(dh2_character_ai_on_died(&out,reinterpret_cast<AIDeathState64*>(reinterpret_cast<char*>(&f.state)+1),3,&f.targets,&f.services));reject(dh2_character_ai_on_died(&out,&f.state,3,&f.targets,nullptr));f.state.reserved=1;reject(dh2_character_ai_on_died(&out,&f.state,3,&f.targets,&f.services));f.state.reserved=0;reject(dh2_character_ai_on_died(&out,&f.state,3,nullptr,&f.services));f.target.reserved=1;reject(dh2_character_ai_on_died(&out,&f.state,3,&f.targets,&f.services));f.target.reserved=0;reject(dh2_character_ai_on_died(reinterpret_cast<AIDeathResult24*>(f.timers[0].data()),&f.state,3,&f.targets,&f.services));
 f.genuine=true;f.debug=dh2_character_debug_create();check(f.debug);f.missing_file=argv[2];f.reset(x);
 f.event_state={1,&f.event_owner,f.ai_virtuals.data(),0,nullptr,0,0,0,0,0,0};AIEventResult16 routed{};AIEventPayload24 payload{3,0,0,0};
 check(dh2_character_ai_event(&routed,&f.event_state,2,&payload,&f.event_services)==3&&routed.service_calls==1&&real_ai_calls==1);
 check(f.target.target==0&&f.target.candidate==0&&f.target.last_target==0&&f.target_owners[0].word14d0==0&&f.target.alive==41&&f.target.sight==42&&f.target.changed==43&&f.state.timer0==0&&f.state.timer1==1&&f.timers[0][0].active==1&&f.timers[0][1].active==1&&real_debug_calls==2);
 unsigned loaded,entries;check(dh2_character_debug_snapshot(f.debug,&loaded,&entries)==1&&loaded==1&&entries==7);dh2_character_debug_destroy(f.debug);f.debug=nullptr;current=nullptr;
 std::cout<<"{\"validation\":\"PASS\",\"original_gold_cases\":"<<count<<",\"synchronous_reentry_cases\":"<<reentries<<",\"checks\":"<<checks<<",\"service_failure_prefixes\":"<<failures<<",\"atomic_guards\":"<<guards<<",\"genuine_AI_event2_calls\":"<<real_ai_calls<<",\"genuine_debug_target_services\":"<<real_debug_calls<<",\"genuine_SM_SetDeadState_failure_prefixes\":1,\"module_library\":\""<<origin(reinterpret_cast<void*>(&dh2_character_ai_on_died))<<"\",\"world_library\":\""<<origin(reinterpret_cast<void*>(&dh2_character_ai_event))<<"\",\"timer_stop_library\":\""<<origin(reinterpret_cast<void*>(actual_stop()))<<"\",\"full_group_state_aggro_skill_spell_bodies\":false}\n";
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
