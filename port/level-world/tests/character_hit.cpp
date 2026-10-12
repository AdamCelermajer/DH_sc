#include "character_hit.hpp"
#include "character_design_services.hpp"
#include "character_dot_attack.hpp"
#include "../game-data/vitals.hpp"
#include "../game-data/aggro.hpp"
#include <array>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace tp=dh2::target_providers;
namespace {
unsigned checks=0,frame=0,guards=0,failures=0,real_hits=0,real_lethal_prefixes=0,real_dot_prefixes=0;
void verify(bool v,unsigned line){++checks;if(!v)throw std::runtime_error("HitFor frame "+std::to_string(frame)+" line "+std::to_string(line));}
#define check(v) verify(v,__LINE__)
unsigned word(std::istream& f){unsigned v;f.read(reinterpret_cast<char*>(&v),4);check(bool(f));return v;}
int sint(unsigned v){int x;std::memcpy(&x,&v,4);return x;}
template<class T>void read(std::istream& f,T& v){f.read(reinterpret_cast<char*>(&v),sizeof(v));check(bool(f));}
std::string text(std::istream& f){auto n=word(f);check(n<4096);std::string s(n,'\0');f.read(s.data(),n);check(bool(f));return s;}
struct Event {unsigned op,subject,target;std::string name;bool operator==(const Event& b)const{return op==b.op&&subject==b.subject&&target==b.target&&name==b.name;}};
using Trace=std::vector<Event>;
Trace trace(std::istream&f){Trace t(word(f));for(auto&x:t){x.op=word(f);x.subject=word(f);x.target=word(f);x.name=text(f);}return t;}
struct Config {
 int dead,main_present,main_dead,god,monster,online,oneshot,app_oneshot,attacker_player,character,remote,attacker,cached,key,oldframe,frame,mutate_op,mutate_hp;
 std::vector<int> players;
};
struct Snapshot {std::array<dh2::data::PropertySheet,4>sheets;HitActor32 actor;tp::Handle16 shared;tp::Registry24 registry;std::array<tp::Record16,8> records;};
struct Fixture {
 dh2::data::PropertyRules rules;std::array<dh2::data::PropertySheet,4>sheets;dh2::data::PropertyView view;HitActor32 actor;tp::Handle16 shared{};std::array<tp::Record16,8>records{};tp::Registry24 registry{records.data(),2,8,0,0};HitAttacker24 attacker{1,&shared,&registry};HitServices16 services{this,invoke};Config config{};Trace events;unsigned players=0;bool mutated=false;int fail=-1;std::vector<Snapshot> snapshots;
 bool genuine=false,reentered=false;unsigned reentry_damage=0;HitResult40 nested{};std::vector<int> ai_types;tp::Types16 types{};std::array<tp::Character32,3> characters{};dh2::data::PropertySheet main_properties{},enemy_properties{};DebugSwitches* debug=nullptr;DebugFileServices24 files{this,open,close};std::string missing_file;
 explicit Fixture(const dh2::data::PropertyRules& r):rules(r),view{rules.defaults.data(),rules.types.data(),sheets[0].data(),sheets[1].data(),sheets[2].data(),sheets[3].data(),nullptr,0},actor{1,&view,4,123,0}{}
 ~Fixture(){dh2_character_debug_destroy(debug);}
 void reset(const std::array<dh2::data::PropertySheet,4>& initial,const Config& x){config=x;sheets=initial;actor.controller=4;actor.lifecycle=123;actor.reserved=0;shared={x.key,unsigned(x.oldframe),std::uintptr_t(x.cached)};registry={records.data(),2,8,unsigned(x.frame),0};records={};records[0]={-7,0,1};records[1]={5,0,3};attacker={std::uintptr_t(x.attacker),x.attacker?&shared:nullptr,x.attacker?&registry:nullptr};events.clear();players=0;mutated=false;fail=-1;snapshots.clear();reentered=false;reentry_damage=0;}
 static int open(void* p,const char* name,std::uintptr_t* out){auto& f=*static_cast<Fixture*>(p);check(std::string(name)=="DebugSwitches.savegame");auto* file=std::fopen(f.missing_file.c_str(),"rb");if(!file){check(errno==ENOENT);*out=0;}else *out=reinterpret_cast<std::uintptr_t>(file);return 0;}
 static int close(void*,std::uintptr_t p){return std::fclose(reinterpret_cast<FILE*>(p));}
 int query(unsigned op,std::uintptr_t subject,std::uintptr_t* out){check(subject>=1&&subject<=3);int v=99;check(tp::dh2_character_target_query(&v,op,&characters[subject-1],nullptr,&types,nullptr)==0);*out=unsigned(v);return 0;}
 static int invoke(void* p,HitActor32* actor,const HitRequest32* q,std::uintptr_t* out){
  auto& f=*static_cast<Fixture*>(p);check(actor==&f.actor&&q->force==0);check(q->target==(q->service==hit_controller_kill?f.attacker.identity:0));if(q->service==hit_controller_kill)check(q->subject==f.actor.controller);
  auto count=f.events.size();f.events.push_back({q->service,unsigned(q->subject),unsigned(q->target),q->name?q->name:""});f.snapshots.push_back({f.sheets,f.actor,f.shared,f.registry,f.records});if(int(count)==f.fail)return -1;
  if(int(q->service)==f.config.mutate_op&&!f.mutated){f.mutated=true;f.sheets[3][36]=f.sheets[1][36]=f.config.mutate_hp;f.actor.controller=5;f.actor.lifecycle=-19;}
  if(q->service==hit_application_switch&&f.reentry_damage&&!f.reentered){f.reentered=true;check(dh2_character_hit_for(&f.nested,&f.actor,f.reentry_damage,&f.attacker,&f.services)==1);}
  switch(q->service){
   case hit_is_dead:if(f.genuine)return f.query(tp::is_dead,q->subject,out);*out=q->subject==1?f.config.dead:f.config.main_dead;break;
   case hit_main_player:*out=f.config.main_present?2:0;break; // Explicit Game/session fixture.
   case hit_debug_load:if(f.genuine)return dh2_character_debug_load(f.debug,&f.files)==1?0:-1;break;
   case hit_debug_query:if(f.genuine){unsigned v=99;auto status=dh2_character_debug_get(&v,f.debug,q->name,&f.files);*out=v;return status==1?0:-1;}*out=std::string(q->name)=="GOD_Monster"?f.config.god:f.config.oneshot;break;
   case hit_is_monster:if(f.genuine)return f.query(tp::is_monster,q->subject,out);*out=f.config.monster;break;
   case hit_online:*out=f.config.online;break; // Explicit online singleton fixture.
   case hit_application_switch:check(std::string(q->name)=="OneShotKill");*out=f.config.app_oneshot;break; // Explicit Application fixture.
   case hit_is_player:if(f.genuine)return f.query(tp::is_player,q->subject,out);*out=q->subject==1?f.config.players.at(std::min<std::size_t>(f.players++,f.config.players.size()-1)):f.config.attacker_player;break;
   case hit_controller_kill:
    if(f.genuine)return -1; // Missing full Kill never accepted.
    break; // Original service oracle fixture only.
   case hit_is_remotely_updated:*out=f.config.remote;break;
   case hit_is_character:if(f.genuine)return f.query(tp::is_character,q->subject,out);*out=f.config.character;break;
   default:check(false);
  }return 0;
 }
};
struct DotComposition {
 Fixture* fixture;DotActor32 actor;DotServices16 services{this,invoke};HitResult40 hit{};
 std::array<dh2::data::AggroEntry,1> outgoing_entries{{{1,0x3f800000u,0}}},incoming_entries{{{1,0x3f800000u,0}}};
 dh2::data::AggroTable outgoing{outgoing_entries.data(),1,1},incoming{incoming_entries.data(),1,1};
 static int invoke(void* p,DotActor32*,const DotRequest40* q,DotResponse8* out,dh2::data::CombatResult*){
  auto& c=*static_cast<DotComposition*>(p);auto& f=*c.fixture;
  switch(q->service){
   case dot_debug_load:return dh2_character_debug_load(f.debug,&f.files)==1?0:-1;
   case dot_debug_query:{unsigned v=99;auto status=dh2_character_debug_get(&v,f.debug,q->name,&f.files);out->word=sint(v);return status==1?0:-1;}
   case dot_online:out->word=f.config.online;return 0;
   case dot_party_count:out->word=1;return 0; // Explicit one-character party fixture.
   case dot_application_switch:check(std::string(q->name)=="GOD");out->word=0;return 0; // Explicit Application fixture.
   case dot_add_aggro:{unsigned threat;std::memcpy(&threat,&q->threat,4);dh2::data::AggroChange change{};dh2::data::AggroRequest request{&c.outgoing,&c.incoming,1,1,threat,0};check(dh2_aggro_apply(&change,&request,dh2::data::aggro_add)==0&&change.requests==0);std::memcpy(&out->number,&change.returned_bits,4);return 0;}
   case dot_hit_for:return dh2_character_hit_for(&c.hit,&f.actor,unsigned(q->amount),&f.attacker,&f.services)==1?0:-1;
   case dot_is_dead:{std::uintptr_t v;auto status=f.query(tp::is_dead,1,&v);out->word=int(v);return status;}
   case dot_is_player:{std::uintptr_t v;auto status=f.query(tp::is_player,1,&v);out->word=int(v);return status;}
   case dot_regen_hp:case dot_regen_mp:{dh2::data::VitalsChange change{};return dh2_vitals_regen(&f.view,q->service==dot_regen_mp,q->amount,&change)?-1:0;}
   case dot_cancel_sneaking:case dot_combat_text:case dot_combat_sound:return -1;
   case dot_profile_begin:case dot_profile_end:return 0; // Ordered profiler observer.
   default:return -1;
  }
 }
};
std::string origin(void* p){Dl_info info{};check(dladdr(p,&info));return info.dli_fname;}
}
int main(int argc,char** argv){try{
 check(argc==3);std::ifstream f(argv[1],std::ios::binary);check(word(f)==0x31544948);auto count=word(f);dh2::data::PropertyRules rules;read(f,rules.defaults);read(f,rules.types);std::vector<int> ai_types(word(f));for(auto&v:ai_types)v=sint(word(f));Fixture fixture(rules);
 for(frame=0;frame<count;frame++){
  std::array<dh2::data::PropertySheet,4> sheets;read(f,sheets);auto damage=word(f);Config x;int* fields[]={&x.dead,&x.main_present,&x.main_dead,&x.god,&x.monster,&x.online,&x.oneshot,&x.app_oneshot,&x.attacker_player,&x.character,&x.remote,&x.attacker,&x.cached,&x.key,&x.oldframe,&x.frame,&x.mutate_op,&x.mutate_hp};for(auto*p:fields)*p=sint(word(f));x.players.resize(word(f));for(auto&v:x.players)v=sint(word(f));auto expected_trace=trace(f);std::array<int,4> state;read(f,state);std::vector<tp::Record16> entries(word(f));for(auto&r:entries){r.key=sint(word(f));r.reserved=0;r.object=word(f);}std::array<dh2::data::PropertySheet,4> final;read(f,final);HitResult40 expected{},out{};read(f,expected);fixture.reset(sheets,x);
  check(dh2_character_hit_for(&out,&fixture.actor,damage,&fixture.attacker,&fixture.services)==expected.status);check(std::memcmp(&out,&expected,40)==0&&fixture.events==expected_trace&&fixture.sheets==final);check(fixture.actor.lifecycle==state[0]&&fixture.shared.key==state[1]&&fixture.shared.frame==unsigned(state[2])&&fixture.shared.cached==unsigned(state[3]));check(fixture.registry.count==entries.size());for(unsigned i=0;i<entries.size();i++)check(std::memcmp(&fixture.records[i],&entries[i],16)==0);
  auto snapshots=fixture.snapshots;
  if(frame<65)for(unsigned j=0;j<expected_trace.size();j++){
   fixture.reset(sheets,x);fixture.fail=int(j);check(dh2_character_hit_for(&out,&fixture.actor,damage,&fixture.attacker,&fixture.services)==-2);check(fixture.events==Trace(expected_trace.begin(),expected_trace.begin()+j+1));check(out.calls==j+1&&out.phase==expected_trace[j].op+1&&out.status==-2);check(fixture.sheets==snapshots[j].sheets&&std::memcmp(&fixture.actor,&snapshots[j].actor,32)==0&&std::memcmp(&fixture.shared,&snapshots[j].shared,16)==0&&fixture.registry.count==snapshots[j].registry.count&&std::memcmp(fixture.records.data(),snapshots[j].records.data(),sizeof(fixture.records))==0);++failures;
  }
 }
 check(f.peek()==std::char_traits<char>::eof());Config x{};x.main_present=1;x.character=1;x.attacker=1;x.cached=1;x.key=-7;x.players={0};x.mutate_op=-1;std::array<dh2::data::PropertySheet,4> sheets;for(auto&s:sheets)s=rules.defaults;fixture.reset(sheets,x);HitResult40 out{1,2,3,4,5,6,7,8,9,10},original=out;auto actor=fixture.actor;auto before=fixture.sheets;
 auto reject=[&](int status){check(status==-1&&std::memcmp(&out,&original,40)==0&&std::memcmp(&fixture.actor,&actor,32)==0&&fixture.sheets==before&&fixture.events.empty());++guards;};
 reject(dh2_character_hit_for(&out,nullptr,1,&fixture.attacker,&fixture.services));reject(dh2_character_hit_for(reinterpret_cast<HitResult40*>(fixture.sheets[3].data()),&fixture.actor,1,&fixture.attacker,&fixture.services));reject(dh2_character_hit_for(&out,reinterpret_cast<HitActor32*>(reinterpret_cast<char*>(&fixture.actor)+1),1,&fixture.attacker,&fixture.services));HitServices16 missing{};reject(dh2_character_hit_for(&out,&fixture.actor,1,&fixture.attacker,&missing));fixture.actor.reserved=1;actor=fixture.actor;reject(dh2_character_hit_for(&out,&fixture.actor,1,&fixture.attacker,&fixture.services));fixture.actor.reserved=0;actor=fixture.actor;fixture.registry.capacity=1;reject(dh2_character_hit_for(&out,&fixture.actor,1,&fixture.attacker,&fixture.services));fixture.registry.capacity=8;fixture.records[0].reserved=1;reject(dh2_character_hit_for(&out,&fixture.actor,1,&fixture.attacker,&fixture.services));fixture.records[0].reserved=0;reject(dh2_character_hit_for(&out,&fixture.actor,1,reinterpret_cast<HitAttacker24*>(&fixture.actor),&fixture.services));auto saved=fixture.view.resolved;fixture.view.resolved=reinterpret_cast<int*>(reinterpret_cast<char*>(saved)+1);reject(dh2_character_hit_for(&out,&fixture.actor,1,&fixture.attacker,&fixture.services));fixture.view.resolved=saved;auto shared=fixture.attacker.shared_handle;fixture.attacker.shared_handle=reinterpret_cast<tp::Handle16*>(&fixture.registry);reject(dh2_character_hit_for(&out,&fixture.actor,1,&fixture.attacker,&fixture.services));fixture.attacker.shared_handle=shared;
 fixture.genuine=true;fixture.ai_types=ai_types;fixture.types={fixture.ai_types.data(),unsigned(ai_types.size()),0};fixture.missing_file=argv[2];fixture.debug=dh2_character_debug_create();check(fixture.debug);
 fixture.main_properties=rules.defaults;fixture.main_properties[1]=44;fixture.enemy_properties=rules.defaults;fixture.enemy_properties[1]=1;
 fixture.characters={tp::Character32{1,reinterpret_cast<CombatProperties896*>(fixture.sheets[3].data()),"Crypt_Skeleton",0,0,0,1,1},tp::Character32{2,reinterpret_cast<CombatProperties896*>(fixture.main_properties.data()),"PlayerCharacterPrince",0,0,0,1,1},tp::Character32{3,reinterpret_cast<CombatProperties896*>(fixture.enemy_properties.data()),"CryptSlime",0,0,0,1,1}};
 for(auto hp:{3000,10000}){
  fixture.reset(sheets,x);fixture.sheets[3][1]=1;check(dh2_property_set(&fixture.view,36,hp)==0);check(fixture.sheets[3][36]==hp);check(dh2_character_hit_for(&out,&fixture.actor,256,&fixture.attacker,&fixture.services)==1);check(out.raw_add==-256&&out.before_hp==hp&&out.after_hp==hp-256&&fixture.sheets[3][36]==hp-256&&!out.kill_called);check(fixture.events.back().op==hit_is_player&&fixture.events.back().subject==1);++real_hits;
 }
 fixture.reset(sheets,x);fixture.sheets[3][1]=1;check(dh2_property_set(&fixture.view,36,256)==0);check(dh2_character_hit_for(&out,&fixture.actor,256,&fixture.attacker,&fixture.services)==-2);check(out.phase==hit_controller_kill+1&&out.kill_called==0&&fixture.actor.lifecycle==123&&fixture.sheets[3][36]==0&&!fixture.characters[0].dead1449);++real_lethal_prefixes;
 for(auto amount:{1u,256u}){fixture.reset(sheets,x);fixture.sheets[3][1]=1;check(dh2_property_set(&fixture.view,36,3000)==0);fixture.reentry_damage=amount;check(dh2_character_hit_for(&out,&fixture.actor,256,&fixture.attacker,&fixture.services)==1);check(fixture.reentered&&fixture.nested.before_hp==2744&&fixture.nested.after_hp==2744-int(amount)&&out.before_hp==3000&&out.after_hp==2744-int(amount)&&fixture.sheets[3][36]==out.after_hp);}
 fixture.reset(sheets,x);fixture.sheets[3][1]=1;check(dh2_property_set(&fixture.view,36,3000)==0);fixture.registry.capacity=2;fixture.shared.key=17;fixture.shared.cached=0;fixture.registry.frame=21;check(dh2_character_hit_for(&out,&fixture.actor,256,&fixture.attacker,&fixture.services)==-2&&out.phase==hit_is_player+1&&fixture.shared.frame==21&&fixture.registry.count==2&&fixture.sheets[3][36]==2744);++failures;
 // No fake HitFor acceptance: actual DoT callback reaches our complete method,
 // then deliberately stops at a required scene FX provider. Other enclosing
 // Application/party/aggro facts remain explicitly supplied oracle services.
 fixture.reset(sheets,x);fixture.sheets[3][1]=1;fixture.sheets[3][204]=256;check(dh2_property_set(&fixture.view,36,3000)==0);DotComposition composition{&fixture,DotActor32{1,&fixture.view,-1,0,0,0,0}};dh2::data::CombatResult combat;DotCombatContext32 context{};DotResult24 dot{};check(dh2_character_dot_calculate(&dot,&combat,&context,&composition.actor,256,0,&composition.services)==1);check(dh2_character_dot_apply(&dot,&combat,&composition.actor,&composition.services)==-2&&dot.phase==dot_cancel_sneaking+1&&dot.hit_called==1);check(composition.hit.status==1&&fixture.sheets[3][36]==2744);++real_dot_prefixes;
 std::cout<<"{\"validation\":\"PASS\",\"original_gold_cases\":"<<count<<",\"checks\":"<<checks<<",\"atomic_guards\":"<<guards<<",\"service_failure_prefixes\":"<<failures<<",\"genuine_nonlethal_HitFor\":"<<real_hits<<",\"genuine_lethal_failure_prefixes\":"<<real_lethal_prefixes<<",\"genuine_synchronous_reentry\":2,\"genuine_DoT_HitFor_bridge_calls\":"<<real_dot_prefixes<<",\"module_library\":\""<<origin(reinterpret_cast<void*>(&dh2_character_hit_for))<<"\",\"world_library\":\""<<origin(reinterpret_cast<void*>(tp::dh2_target_handle_character))<<"\",\"data_library\":\""<<origin(reinterpret_cast<void*>(&dh2_property_add))<<"\",\"full_Kill_backend\":false}\n";
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
