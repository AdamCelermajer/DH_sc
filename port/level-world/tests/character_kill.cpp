#include "character_kill.hpp"
#include "character_ai_events.hpp"
#include "character_target_providers.hpp"
#include "../script-runtime/script_constants.hpp"
#include <array>
#include <cstring>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace tp=dh2::target_providers;
namespace {
unsigned checks=0,frame=0,failures=0,guards=0,real_kills=0,real_prefixes=0,ai_calls=0,constant_calls=0,reentry_cases=0;
void verify(bool v,unsigned line){++checks;if(!v)throw std::runtime_error("Kill frame "+std::to_string(frame)+" line "+std::to_string(line));}
#define check(v) verify(v,__LINE__)
unsigned word(std::istream& f){unsigned v;f.read(reinterpret_cast<char*>(&v),4);check(bool(f));return v;}
int sint(unsigned v){int x;std::memcpy(&x,&v,4);return x;}
template<class T>void read(std::istream& f,T& v){f.read(reinterpret_cast<char*>(&v),sizeof(v));check(bool(f));}
std::vector<int> list(std::istream& f){std::vector<int> v(word(f));for(auto&x:v)x=sint(word(f));return v;}
std::string text(std::istream& f){auto n=word(f);check(n<4096);std::string s(n,'\0');f.read(s.data(),n);check(bool(f));return s;}
struct Event {unsigned op,subject,target;int argument;std::string name,key;std::vector<int> payload;int index;bool operator==(const Event& b)const{return op==b.op&&subject==b.subject&&target==b.target&&argument==b.argument&&name==b.name&&key==b.key&&payload==b.payload&&index==b.index;}};
using Trace=std::vector<Event>;
Trace trace(std::istream&f){Trace t(word(f));for(auto&x:t){x.op=word(f);x.subject=word(f);x.target=word(f);x.argument=sint(word(f));x.name=text(f);x.key=text(f);x.payload=list(f);x.index=sint(word(f));}return t;}
using Sheets=std::array<std::array<dh2::data::PropertySheet,4>,4>;
struct Config {std::array<int,4>players,local,owners,targets,constant_values;std::vector<int>dead,levels,counts,entries;int online,loot_gate,killer,oid,property,templ,dead_byte,suppress,is_character,cast,remote,trophy_id,mutate_op,mutate_oid,mutate_property,mutate_template,mutate_suppress,mutate_killer;};
struct Snapshot {Sheets sheets;std::array<KillActor56,4> actors;KillWorld16 world;};
struct Fixture {
 dh2::data::PropertyRules rules;Sheets sheets;std::array<dh2::data::PropertyView,4> views;std::array<KillActor56,4> actors;std::array<KillLevel16,2>levels{{{5,1,0},{6,1,0}}};KillWorld16 world{8,9};KillServices16 services{this,invoke};Config config{};Trace events;std::vector<Snapshot> snapshots;std::vector<KillQuest48> queued;unsigned dead_calls=0,level_calls=0,count_calls=0;bool mutated=false,genuine=false;int fail=-1;
 std::vector<int> ai_types;tp::Types16 types{};tp::Handle16 shared{-1,0,1};std::array<tp::Record16,1>records{{{-1,0,1}}};tp::Registry24 registry{records.data(),1,1,0,0};tp::Services16 handle_services{this,classify};
 std::array<std::uintptr_t,51> virtuals{};AIEventOwner48 ai_owner{};AIEventState64 ai_state{};AIEventServices24 ai_services{this,ai,63,0};AIEventResult16 ai_result{};
 dh2_script_constants* constants=nullptr;
 int reentry_op=-1;unsigned nested_force=1;bool nested_wrapper=false,reentered=false,live_dead=false;KillResult24 nested_result{};Trace nested_trace;
 explicit Fixture(const dh2::data::PropertyRules& r):rules(r){for(unsigned i=0;i<4;i++){views[i]={rules.defaults.data(),rules.types.data(),sheets[i][0].data(),sheets[i][1].data(),sheets[i][2].data(),sheets[i][3].data(),nullptr,0};actors[i]={i+1,&views[i],0,nullptr,0,0,0,0,0,0,{}};}}
 void reset(const Sheets& initial,const Config& x){sheets=initial;config=x;events.clear();snapshots.clear();queued.clear();dead_calls=level_calls=count_calls=0;mutated=false;reentered=false;live_dead=false;reentry_op=-1;nested_trace.clear();fail=-1;world={8,9};for(unsigned i=0;i<4;i++)actors[i]={i+1,&views[i],std::uintptr_t(x.killer),x.owners[i]?&actors[x.owners[i]-1]:nullptr,std::uintptr_t(x.targets[i]),sint(unsigned(x.oid)+i),std::int16_t(x.property),std::int16_t(x.templ),std::uint8_t(x.dead_byte),std::uint8_t(x.suppress),{}};for(auto&l:levels)l.loot_gate150=x.loot_gate;}
 int query(unsigned op,std::uintptr_t identity,int& out){check(identity>=1&&identity<=4);auto& a=actors[identity-1];tp::Character32 c{identity,reinterpret_cast<CombatProperties896*>(a.properties->resolved),identity==2?"PlayerCharacterPrince":"Crypt_Skeleton",0,a.dead,0,1,1};return tp::dh2_character_target_query(&out,op,&c,nullptr,&types,nullptr);}
 static int classify(void* p,const tp::Request24* q,std::uintptr_t* out){auto&f=*static_cast<Fixture*>(p);check(q->service==tp::virtual_character);int v;check(f.query(tp::is_character,q->subject,v)==0);*out=unsigned(v);return 0;}
 static int ai(void*,AIEventState64*,const AIEventRequest40* q,unsigned*){check(q->service==ai_event_virtual&&q->operation==0x24&&q->event==2&&q->callee==0x3d1000);++ai_calls;return -1;}
 static int invoke(void* p,KillActor56* receiver,const KillRequest56* q,KillResponse16* out){
  auto& f=*static_cast<Fixture*>(p);check(receiver==&f.actors[0]&&!q->reserved);std::vector<int> payload;
  if(q->event){const auto&e=*q->event;check(!e.reserved&&!e.reserved2);payload={int(e.kind),sint(e.type),int(e.killer),e.oid,e.network_id,e.subject_id,e.flag0,e.flag1,int(e.level)};}
  auto count=f.events.size();f.events.push_back({q->service,unsigned(q->subject),unsigned(q->target),q->argument,q->name?q->name:"",q->key?q->key:"",payload,q->index});f.snapshots.push_back({f.sheets,f.actors,f.world});if(int(count)==f.fail)return -1;
  auto captured_dead=receiver->dead;
  if(int(q->service)==f.reentry_op&&!f.reentered){f.reentered=true;auto outer=std::move(f.events);f.events.clear();auto nested=f.nested_wrapper?dh2_character_ctrl_kill:dh2_character_kill;check(nested(&f.nested_result,receiver,0,f.nested_force,&f.world,&f.services)==1);f.nested_trace=std::move(f.events);f.events=std::move(outer);}
  if(int(q->service)==f.config.mutate_op&&!f.mutated){f.mutated=true;receiver->oid=f.config.mutate_oid;receiver->property_id=f.config.mutate_property;receiver->template_id=f.config.mutate_template;receiver->suppress_quest=f.config.mutate_suppress;receiver->killer=f.config.mutate_killer;f.world.trophy_manager=99;}
  auto& x=f.config;
  if(f.genuine){
   switch(q->service){
    case kill_is_dead:return f.query(tp::is_dead,q->subject,out->word);
    case kill_is_player:return f.query(tp::is_player,q->subject,out->word);
    case kill_is_character:return f.query(tp::is_character,q->subject,out->word);
    case kill_current_level:out->pointer=reinterpret_cast<std::uintptr_t>(&f.levels[0]);return 0; // Explicit retained Level projection.
    case kill_aggro_count:out->word=0;return 0; // Explicit empty source aggro fixture.
    case kill_handle_character:{tp::Handle16 local{};check(q->subject==1);return tp::dh2_target_handle_character(&out->pointer,&local,&f.shared,&f.registry,&f.handle_services);}
    case kill_is_remotely_updated:out->word=0;return 0; // Explicit local object projection.
    case kill_raise_event:{AIEventPayload24 payload{q->target,0,0,0};check(q->argument==2&&q->subject==1);auto status=dh2_character_ai_event(&f.ai_result,&f.ai_state,2,&payload,&f.ai_services);check(status==3&&f.ai_result.service_calls==1);return -1;}
    case kill_constant:check(f.constants&&q->subject==f.world.constants);++constant_calls;return dh2_script_constants_get(f.constants,q->name,q->key,&out->word);
    case kill_drop_loot:case kill_distribute_xp:case kill_raise_async:return -1; // Actual missing backends never accepted.
    default:return -1;
   }
  }
  switch(q->service){
   case kill_is_dead:out->word=f.live_dead?captured_dead:x.dead.at(std::min<std::size_t>(f.dead_calls++,x.dead.size()-1));break;
   case kill_is_player:out->word=x.players[q->subject-1];break;
   case kill_is_local_player:out->word=x.local[q->subject-1];break;
   case kill_online:out->word=x.online;break;
   case kill_current_level:{int i=x.levels.at(std::min<std::size_t>(f.level_calls++,x.levels.size()-1));out->pointer=i?reinterpret_cast<std::uintptr_t>(&f.levels[i-1]):0;break;}
   case kill_aggro_count:out->word=x.counts.at(std::min<std::size_t>(f.count_calls++,x.counts.size()-1));break;
   case kill_aggro_entry:{int i=unsigned(q->index)<x.entries.size()?x.entries[q->index]:0;out->pointer=i?reinterpret_cast<std::uintptr_t>(&f.actors[i-1]):0;break;}
   case kill_trophy_id:out->word=x.trophy_id;break;
   case kill_is_character:out->word=x.is_character;break;
   case kill_handle_character:out->pointer=x.cast;break;
   case kill_is_remotely_updated:out->word=x.remote;break;
   case kill_constant:{static const char* keys[]={"KillXEnemies","ClearEnemies","KillEnemyTemplate","ClearEnemyTemplate"};check(std::string(q->name)=="v2QuestObjectiveType");bool found=false;for(unsigned i=0;i<4;i++)if(std::string(q->key)==keys[i]){out->word=x.constant_values[i];found=true;}check(found);break;}
   case kill_raise_async:check(q->event&&q->subject==q->event->level);f.queued.push_back(*q->event);break;
   case kill_raise_event:case kill_drop_loot:case kill_unlock_trophy:case kill_get_local_player:case kill_distribute_xp:break; // Explicit original service fixtures only.
   default:check(false);
  }return 0;
 }
};
std::string origin(void* p){Dl_info info{};check(dladdr(p,&info));return info.dli_fname;}
}
int main(int argc,char**argv){try{
 check(argc==3);std::ifstream f(argv[1],std::ios::binary);check(word(f)==0x314c494b);auto count=word(f);dh2::data::PropertyRules rules;read(f,rules.defaults);read(f,rules.types);auto ai_types=list(f);Fixture fixture(rules);
 for(frame=0;frame<count;frame++){
  bool wrapper=word(f);Sheets initial;read(f,initial);auto attacker=word(f),force=word(f);Config x;read(f,x.players);read(f,x.local);read(f,x.owners);read(f,x.targets);read(f,x.constant_values);x.dead=list(f);x.levels=list(f);x.counts=list(f);x.entries=list(f);int* fields[]={&x.online,&x.loot_gate,&x.killer,&x.oid,&x.property,&x.templ,&x.dead_byte,&x.suppress,&x.is_character,&x.cast,&x.remote,&x.trophy_id,&x.mutate_op,&x.mutate_oid,&x.mutate_property,&x.mutate_template,&x.mutate_suppress,&x.mutate_killer};for(auto*p:fields)*p=sint(word(f));auto expected_trace=trace(f);std::array<std::array<int,8>,4> state;read(f,state);Sheets final;read(f,final);KillResult24 expected{},out{};read(f,expected);fixture.reset(initial,x);auto run=wrapper?dh2_character_ctrl_kill:dh2_character_kill;check(run(&out,&fixture.actors[0],attacker,force,&fixture.world,&fixture.services)==expected.status);check(std::memcmp(&out,&expected,24)==0&&fixture.events==expected_trace&&fixture.sheets==final);
  for(unsigned i=0;i<4;i++){auto& a=fixture.actors[i];std::array<int,8> current{int(a.killer),a.owner?int(a.owner->identity):0,int(a.tracked_target),a.oid,a.property_id,a.template_id,a.dead,a.suppress_quest};check(current==state[i]);}
  unsigned queued=0;for(const auto& event:expected_trace)queued+=event.op==kill_raise_async;check(fixture.queued.size()==queued);auto snapshots=fixture.snapshots;
  if(frame<70)for(unsigned j=0;j<expected_trace.size();j++){fixture.reset(initial,x);fixture.fail=int(j);check(run(&out,&fixture.actors[0],attacker,force,&fixture.world,&fixture.services)==-2);check(fixture.events==Trace(expected_trace.begin(),expected_trace.begin()+j+1));check(out.calls==j+1&&out.phase==expected_trace[j].op+1&&out.status==-2);check(fixture.sheets==snapshots[j].sheets&&std::memcmp(fixture.actors.data(),snapshots[j].actors.data(),sizeof(fixture.actors))==0&&std::memcmp(&fixture.world,&snapshots[j].world,16)==0);++failures;}
 }
 reentry_cases=word(f);
 for(unsigned i=0;i<reentry_cases;i++){auto op=word(f);bool nested_wrapper=word(f);auto nested_force=word(f);bool wrapper=word(f);auto force=word(f);Sheets initial;read(f,initial);auto outer=trace(f),nested=trace(f);std::array<std::array<int,8>,4> state;read(f,state);Sheets final;read(f,final);KillResult24 expected{},nested_expected{},out{};read(f,expected);read(f,nested_expected);Config x{};x.dead={0};x.levels={1};x.loot_gate=1;x.counts={0};x.oid=-91;x.property=-32768;x.templ=17;x.is_character=1;x.cast=1;x.trophy_id=3;x.constant_values={11,12,13,14};x.mutate_op=-1;fixture.reset(initial,x);fixture.reentry_op=int(op);fixture.nested_wrapper=nested_wrapper;fixture.nested_force=nested_force;fixture.live_dead=true;auto run=wrapper?dh2_character_ctrl_kill:dh2_character_kill;check(run(&out,&fixture.actors[0],0,force,&fixture.world,&fixture.services)==1&&fixture.reentered);check(fixture.events==outer&&fixture.nested_trace==nested&&std::memcmp(&out,&expected,24)==0&&std::memcmp(&fixture.nested_result,&nested_expected,24)==0&&fixture.sheets==final);for(unsigned j=0;j<4;j++){auto&a=fixture.actors[j];check((std::array<int,8>{int(a.killer),a.owner?int(a.owner->identity):0,int(a.tracked_target),a.oid,a.property_id,a.template_id,a.dead,a.suppress_quest})==state[j]);}}
 check(f.peek()==std::char_traits<char>::eof());Config x{};x.players={0,1,0,0};x.local={};x.owners={};x.targets={};x.dead={0};x.levels={1};x.counts={0};x.loot_gate=1;x.templ=-1;x.mutate_op=-1;Sheets initial;for(auto&parts:initial)for(auto& s:parts)s=rules.defaults;fixture.reset(initial,x);KillResult24 out{1,2,3,4,5,6},before_out=out;auto before_actor=fixture.actors;auto before_sheets=fixture.sheets;
 auto reject=[&](int status){check(status==-1&&std::memcmp(&out,&before_out,24)==0&&fixture.sheets==before_sheets&&std::memcmp(fixture.actors.data(),before_actor.data(),sizeof(before_actor))==0&&fixture.events.empty());++guards;};
 reject(dh2_character_ctrl_kill(&out,nullptr,0,0,&fixture.world,&fixture.services));reject(dh2_character_ctrl_kill(reinterpret_cast<KillResult24*>(fixture.sheets[0][3].data()),&fixture.actors[0],0,0,&fixture.world,&fixture.services));reject(dh2_character_ctrl_kill(&out,reinterpret_cast<KillActor56*>(reinterpret_cast<char*>(&fixture.actors[0])+1),0,0,&fixture.world,&fixture.services));KillServices16 missing{};reject(dh2_character_ctrl_kill(&out,&fixture.actors[0],0,0,&fixture.world,&missing));fixture.actors[0].reserved[0]=1;before_actor=fixture.actors;reject(dh2_character_ctrl_kill(&out,&fixture.actors[0],0,0,&fixture.world,&fixture.services));fixture.actors[0].reserved[0]=0;before_actor=fixture.actors;reject(dh2_character_ctrl_kill(&out,&fixture.actors[0],0,0,reinterpret_cast<KillWorld16*>(&fixture.actors[0]),&fixture.services));auto resolved=fixture.views[0].resolved;fixture.views[0].resolved=reinterpret_cast<int*>(reinterpret_cast<char*>(resolved)+1);reject(dh2_character_ctrl_kill(&out,&fixture.actors[0],0,0,&fixture.world,&fixture.services));fixture.views[0].resolved=resolved;reject(dh2_character_ctrl_kill(reinterpret_cast<KillResult24*>(&fixture.world),&fixture.actors[0],0,0,&fixture.world,&fixture.services));
 fixture.genuine=true;fixture.ai_types=ai_types;fixture.types={fixture.ai_types.data(),unsigned(ai_types.size()),0};fixture.virtuals[9]=0x3d1000;fixture.ai_owner={1,7,11,reinterpret_cast<std::uintptr_t>(&fixture.views[0]),0,0,0,0};fixture.ai_state={12,&fixture.ai_owner,fixture.virtuals.data(),0,nullptr,0,0,0,0,0,0};
 fixture.reset(initial,x);fixture.sheets[0][3][1]=1;check(dh2_property_set(&fixture.views[0],36,3000)==0);check(dh2_character_kill(&out,&fixture.actors[0],1,1,&fixture.world,&fixture.services)==1&&out.dead_written==1&&fixture.actors[0].dead==1&&fixture.sheets[0][3][36]==0&&fixture.actors[0].killer==0);++real_kills;
 fixture.reset(initial,x);fixture.sheets[0][3][1]=1;check(dh2_property_set(&fixture.views[0],36,3000)==0);check(dh2_character_ctrl_kill(&out,&fixture.actors[0],1,1,&fixture.world,&fixture.services)==-2&&out.phase==kill_raise_event+1&&out.events_raised==0&&fixture.actors[0].dead==1&&fixture.sheets[0][3][36]==0&&ai_calls==1);++real_prefixes;
 fixture.reset(initial,x);fixture.sheets[0][3][1]=1;check(dh2_property_set(&fixture.views[0],36,3000)==0);check(dh2_character_ctrl_kill(&out,&fixture.actors[0],1,0,&fixture.world,&fixture.services)==-2&&out.phase==kill_distribute_xp+1&&fixture.actors[0].killer==1&&fixture.actors[0].dead==1&&fixture.sheets[0][3][36]==0&&ai_calls==1);++real_prefixes;
 x.loot_gate=0;fixture.reset(initial,x);fixture.sheets[0][3][1]=1;check(dh2_property_set(&fixture.views[0],36,3000)==0);check(dh2_character_kill(&out,&fixture.actors[0],1,0,&fixture.world,&fixture.services)==-2&&out.phase==kill_drop_loot+1&&fixture.actors[0].dead==1&&fixture.sheets[0][3][36]==0&&fixture.actors[0].killer==0);++real_prefixes;
 std::ifstream constants_file(argv[2],std::ios::binary);std::vector<std::uint8_t> bytes{std::istreambuf_iterator<char>(constants_file),{}};check(!bytes.empty());fixture.constants=dh2_script_constants_create();check(fixture.constants);dh2_script_constants_reload loaded{};check(dh2_script_constants_load(fixture.constants,bytes.data(),unsigned(bytes.size()),&loaded)==0&&loaded.consumed==bytes.size());std::int32_t kill_type;check(dh2_script_constants_get(fixture.constants,"v2QuestObjectiveType","KillXEnemies",&kill_type)==0);
 x.loot_gate=1;x.oid=-192;x.property=-32768;x.templ=17;fixture.reset(initial,x);fixture.sheets[0][3][1]=1;check(dh2_property_set(&fixture.views[0],36,3000)==0);check(dh2_character_kill(&out,&fixture.actors[0],0,0,&fixture.world,&fixture.services)==-2&&out.phase==kill_raise_async+1&&constant_calls==1&&fixture.actors[0].dead==1&&fixture.sheets[0][3][36]==0&&fixture.actors[0].killer==0&&fixture.queued.empty());check(fixture.events.back().payload==std::vector<int>({1,kill_type,0,-192,-1,-32768,0,0,5})&&ai_calls==1);++real_prefixes;dh2_script_constants_destroy(fixture.constants);fixture.constants=nullptr;
 std::cout<<"{\"validation\":\"PASS\",\"original_gold_cases\":"<<count<<",\"synchronous_reentry_gold_cases\":"<<reentry_cases<<",\"checks\":"<<checks<<",\"atomic_guards\":"<<guards<<",\"service_failure_prefixes\":"<<failures<<",\"genuine_forced_Kill_completions\":"<<real_kills<<",\"genuine_required_backend_failure_prefixes\":"<<real_prefixes<<",\"genuine_AI_event2_calls\":"<<ai_calls<<",\"genuine_quest_constant_calls\":"<<constant_calls<<",\"module_library\":\""<<origin(reinterpret_cast<void*>(&dh2_character_kill))<<"\",\"world_library\":\""<<origin(reinterpret_cast<void*>(&dh2_character_ai_event))<<"\",\"data_library\":\""<<origin(reinterpret_cast<void*>(&dh2_property_set))<<"\",\"runtime_library\":\""<<origin(reinterpret_cast<void*>(&dh2_script_constants_get))<<"\",\"full_loot_xp_quest_AI_backends\":false}\n";
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
