#include "character_dot_attack.hpp"
#include "character_timer_effects.hpp"
#include "character_design_services.hpp"
#include "../game-data/aggro.hpp"
#include "../game-data/health.hpp"
#include "../game-data/vitals.hpp"
#include <array>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace {
unsigned checks=0,frame=0,guards=0,failures=0,health_prefixes=0,timer_prefixes=0;
void verify(bool ok,unsigned line){++checks;if(!ok)throw std::runtime_error("DoT check "+std::to_string(checks)+" frame "+std::to_string(frame)+" line "+std::to_string(line));}
#define check(ok) verify(ok,__LINE__)
unsigned word(std::istream&f){unsigned x=0;f.read(reinterpret_cast<char*>(&x),4);check(bool(f));return x;}
int sint(unsigned x){int n;std::memcpy(&n,&x,4);return n;}
unsigned bits(float x){unsigned n;std::memcpy(&n,&x,4);return n;}
float number(unsigned x){float n;std::memcpy(&n,&x,4);return n;}
template<class T>void read(std::istream&f,T&x){f.read(reinterpret_cast<char*>(&x),sizeof(x));check(bool(f));}
std::string text(std::istream&f){auto n=word(f);check(n<4096);std::string s(n,'\0');f.read(s.data(),n);check(bool(f));return s;}
struct Event {unsigned op,amount;std::string name;unsigned threat;bool operator==(const Event&b)const{return op==b.op&&amount==b.amount&&name==b.name&&threat==b.threat;}};
using Trace=std::vector<Event>;
Trace trace(std::istream&f){Trace t(word(f));for(auto&r:t){r.op=word(f);r.amount=word(f);r.name=text(f);r.threat=word(f);}return t;}
struct Config {
 int network=0,combo=0,push=0,god=0,online=0,party=0,app_god=0,mutate=0;
 unsigned aggro=0;std::array<int,4> debug{};std::vector<int> players,dead;
};
struct Fixture {
 dh2::data::PropertyRules rules;std::array<dh2::data::PropertySheet,4>sheets;dh2::data::PropertyView view;DotActor32 actor;DotCombatContext32 combat{};DotServices16 services{this,invoke};Config config;Trace events;unsigned players=0,dead=0;int fail=-1;bool actual_prefix=false;DebugSwitches* debug_owner=nullptr;DebugFileServices24 files{this,open,close};std::string missing_file;
 std::array<dh2::data::AggroEntry,1> outgoing_entries{},incoming_entries{};dh2::data::AggroTable outgoing{outgoing_entries.data(),1,1},incoming{incoming_entries.data(),1,1};dh2::data::HealthChange health{};
 explicit Fixture(const dh2::data::PropertyRules&r):rules(r),view{rules.defaults.data(),rules.types.data(),sheets[0].data(),sheets[1].data(),sheets[2].data(),sheets[3].data(),nullptr,0},actor{0x123456789abcdef0u,&view,-1,0,0,0,0}{}
 ~Fixture(){dh2_character_debug_destroy(debug_owner);}
 void reset(const dh2::data::PropertySheet&sheet,const Config&x){config=x;for(auto&s:sheets)s=sheet;actor.network_id=x.network;actor.combo_hits=x.combo;actor.push_death=x.push;actor.god=x.god;events.clear();players=dead=0;fail=-1;}
 static int open(void*ctx,const char*name,std::uintptr_t*out){auto&f=*static_cast<Fixture*>(ctx);check(std::string(name)=="DebugSwitches.savegame");auto* file=std::fopen(f.missing_file.c_str(),"rb");if(!file){check(errno==ENOENT);*out=0;}else *out=reinterpret_cast<std::uintptr_t>(file);return 0;}
 static int close(void*,std::uintptr_t p){return std::fclose(reinterpret_cast<FILE*>(p));}
 static int invoke(void*context,DotActor32*a,const DotRequest40*r,DotResponse8*out,dh2::data::CombatResult*){
  auto&f=*static_cast<Fixture*>(context);check(a==&f.actor&&r->subject==a->identity&&r->reserved==0);check(r->target==(r->service==dot_add_aggro||r->service==dot_hit_for||r->service==dot_combat_text||r->service==dot_combat_sound?a->identity:0));
  auto call=f.events.size();f.events.push_back({r->service,unsigned(r->amount),r->name?r->name:"",bits(r->threat)});if(int(call)==f.fail)return -1;
  switch(r->service){
  case dot_debug_load:if(f.actual_prefix)return dh2_character_debug_load(f.debug_owner,&f.files)==1?0:-1;break;
  case dot_debug_query:
   if(f.actual_prefix){unsigned value=99;auto status=dh2_character_debug_get(&value,f.debug_owner,r->name,&f.files);out->word=sint(value);return status==1?0:-1;}
   {static const char*keys[]={"NoDamages","GOD","isTracingThreatChange","isTracingChar_Attack"};bool found=false;for(unsigned i=0;i<4;i++)if(std::string(r->name)==keys[i]){out->word=f.config.debug[i];found=true;}check(found);}break;
  case dot_online:out->word=f.config.online;break;
  case dot_application_switch:check(std::string(r->name)=="GOD");out->word=f.config.app_god;break;
  case dot_party_count:out->word=f.config.party;break;
  case dot_is_player:out->word=f.config.players.at(std::min<std::size_t>(f.players++,f.config.players.size()-1));break;
  case dot_is_dead:out->word=f.config.dead.at(std::min<std::size_t>(f.dead++,f.config.dead.size()-1));break;
  case dot_add_aggro:
   if(f.actual_prefix){dh2::data::AggroChange c{};dh2::data::AggroRequest request{&f.outgoing,&f.incoming,a->identity,a->identity,bits(r->threat),0};check(dh2_aggro_apply(&c,&request,dh2::data::aggro_add)==0&&c.requests==0);out->number=number(c.returned_bits);}
   else{out->number=number(f.config.aggro);if(f.config.mutate){f.sheets[3][204]=-17;a->network_id=f.config.mutate;}}break;
  case dot_hit_for:
   if(f.actual_prefix){dh2::data::HealthRequest request{a->properties,unsigned(r->amount),dh2::data::health_monster|dh2::data::health_main_player_present,0,0};check(dh2_health_hit(&f.health,&request)==0);++health_prefixes;return -1;}
   break; // Explicit oracle service fixture; not a real HitFor backend.
  case dot_regen_hp:case dot_regen_mp:
   if(f.actual_prefix){dh2::data::VitalsChange change{};return dh2_vitals_regen(a->properties,r->service==dot_regen_mp,r->amount,&change)?-1:0;}break;
  case dot_cancel_sneaking:case dot_combat_text:case dot_combat_sound:
   if(f.actual_prefix)return -1;
   break; // Missing real providers never accepted.
  case dot_profile_begin:case dot_profile_end:break; // Ordered profile observer.
  default:check(false);
  }return 0;
 }
 static int timer(void*context,TimerEffectState32*state,const TimerEffectRequest40*r,int*out,dh2::data::CombatResult*attack){
  auto&f=*static_cast<Fixture*>(context);check(state->properties==&f.view&&r->subject==f.actor.identity);DotResult24 result{};DotResponse8 response{};DotRequest40 request{};request.subject=f.actor.identity;
  switch(r->service){
  case effect_is_dead:*out=0;return 0;
  case effect_debug_load:request.service=dot_debug_load;return invoke(&f,&f.actor,&request,&response,attack);
  case effect_debug_query:request.service=dot_debug_query;request.name=r->name;return invoke(&f,&f.actor,&request,&response,attack);
  case effect_dot_calculate:return dh2_character_dot_calculate_result(&result,attack,&f.combat,&f.actor,r->amount,r->element,&f.services)==1?0:-1;
  case effect_dot_apply:return dh2_character_dot_apply(&result,attack,&f.actor,&f.services)==1?0:-1;
  default:check(false);return -1;
  }
 }
};
std::string origin(void*p){Dl_info d{};check(dladdr(p,&d));return d.dli_fname;}
}
int main(int argc,char**argv){try{
 check(argc==3);std::ifstream f(argv[1],std::ios::binary);check(word(f)==0x31544f44);auto count=word(f);dh2::data::PropertyRules rules;read(f,rules.defaults);read(f,rules.types);Fixture fixture(rules);
 for(frame=0;frame<count;frame++){
  auto wrapper=word(f);dh2::data::PropertySheet sheet;read(f,sheet);auto amount=sint(word(f)),element=sint(word(f));Config x;x.network=sint(word(f));x.combo=word(f);x.push=word(f);x.god=word(f);x.online=word(f);x.party=sint(word(f));x.app_god=sint(word(f));x.aggro=word(f);x.mutate=sint(word(f));for(auto&v:x.debug)v=sint(word(f));x.players.resize(word(f));for(auto&v:x.players)v=sint(word(f));x.dead.resize(word(f));for(auto&v:x.dead)v=sint(word(f));
  dh2::data::CombatResult expected{},result{};read(f,expected);std::array<int,7> context;read(f,context);auto ct=trace(f),at=trace(f);std::array<int,4> state;read(f,state);DotResult24 expected_output{},out{};read(f,expected_output);std::array<dh2::data::PropertySheet,4>final;read(f,final);fixture.reset(sheet,x);
  check((wrapper?dh2_character_dot_calculate:dh2_character_dot_calculate_result)(&out,&result,&fixture.combat,&fixture.actor,amount,element,&fixture.services)==1);check(std::memcmp(&result,&expected,40)==0&&fixture.events==ct);check(fixture.combat.attacker==fixture.actor.identity&&fixture.combat.defender==fixture.actor.identity);check(fixture.combat.level_delta==context[0]&&fixture.combat.reverse_level_delta==context[1]&&fixture.combat.element==context[2]&&!fixture.combat.offhand&&!fixture.combat.magic&&!fixture.combat.blocked&&!fixture.combat.critical);
  fixture.events.clear();check(dh2_character_dot_apply(&out,&result,&fixture.actor,&fixture.services)==expected_output.status);check(fixture.events==at&&std::memcmp(&out,&expected_output,24)==0);check(fixture.actor.network_id==state[0]&&fixture.actor.combo_hits==state[1]&&fixture.actor.push_death==state[2]&&fixture.actor.god==state[3]);check(fixture.sheets==final&&std::memcmp(&result,&expected,40)==0);
  if(frame<50)for(unsigned j=0;j<at.size();j++){fixture.reset(sheet,x);fixture.fail=int(j);result=expected;check(dh2_character_dot_apply(&out,&result,&fixture.actor,&fixture.services)==-2);check(fixture.events==Trace(at.begin(),at.begin()+j+1));check(out.calls==j+1&&out.status==-2);++failures;}
 }
 check(f.peek()==std::char_traits<char>::eof());Config x;x.network=-1;x.party=1;x.players={0};x.dead={0,0};auto sheet=rules.defaults;sheet[204]=256;sheet[38]=3000;sheet[43]=1000;fixture.reset(sheet,x);dh2::data::CombatResult result;result.amount=256;result.mask=0x20080000;DotResult24 out{1,2,3,4,5,6};auto original=out;auto actor=fixture.actor;auto sheets=fixture.sheets;
 auto reject=[&](int status){check(status==-1&&std::memcmp(&out,&original,24)==0&&std::memcmp(&actor,&fixture.actor,32)==0&&fixture.sheets==sheets&&fixture.events.empty());++guards;};
 reject(dh2_character_dot_apply(&out,nullptr,&fixture.actor,&fixture.services));reject(dh2_character_dot_apply(reinterpret_cast<DotResult24*>(&result),&result,&fixture.actor,&fixture.services));DotServices16 missing{};reject(dh2_character_dot_apply(&out,&result,&fixture.actor,&missing));fixture.actor.reserved=1;actor=fixture.actor;reject(dh2_character_dot_apply(&out,&result,&fixture.actor,&fixture.services));fixture.actor.reserved=0;actor=fixture.actor;auto bad=result;bad.outcomes=1;reject(dh2_character_dot_apply(&out,&bad,&fixture.actor,&fixture.services));reject(dh2_character_dot_calculate(&out,&result,reinterpret_cast<DotCombatContext32*>(&fixture.actor),&fixture.actor,256,0,&fixture.services));reject(dh2_character_dot_apply(reinterpret_cast<DotResult24*>(fixture.sheets[3].data()),&result,&fixture.actor,&fixture.services));reject(dh2_character_dot_apply(&out,&result,reinterpret_cast<DotActor32*>(reinterpret_cast<char*>(&fixture.actor)+1),&fixture.services));
 fixture.actual_prefix=true;fixture.missing_file=argv[2];fixture.debug_owner=dh2_character_debug_create();check(fixture.debug_owner);fixture.outgoing_entries[0]={fixture.actor.identity,bits(1),0};fixture.incoming_entries[0]={fixture.actor.identity,bits(1),0};
 for(auto hp:{3000,256}){
  fixture.reset(sheet,x);check(dh2_property_set(&fixture.view,36,hp)==0);
  check(fixture.sheets[3][36]==hp);
  check(dh2_character_dot_calculate(&out,&result,&fixture.combat,&fixture.actor,256,0,&fixture.services)==1);
  check(dh2_character_dot_apply(&out,&result,&fixture.actor,&fixture.services)==-2&&out.phase==dot_hit_for+1);
  if(fixture.sheets[3][36]!=hp-256||fixture.health.after!=hp-256||fixture.health.kill_requested!=unsigned(hp==256))throw std::runtime_error("health prefix HP="+std::to_string(hp)+" current="+std::to_string(fixture.sheets[3][36])+" before="+std::to_string(fixture.health.before)+" after="+std::to_string(fixture.health.after)+" kill="+std::to_string(fixture.health.kill_requested));
  check(true);
  check(fixture.events.back().op==dot_hit_for);
 }
 fixture.reset(sheet,x);for(unsigned p=126;p<132;p++)fixture.sheets[3][p]=0;fixture.sheets[3][126]=256;check(dh2_property_set(&fixture.view,36,3000)==0);TimerEffectState32 timer_state{fixture.actor.identity,&fixture.view,0,0};TimerEffectServices16 timer_services{&fixture,Fixture::timer};TimerEffectResult24 timer_result{};check(dh2_character_timer_effect(&timer_result,&timer_state,0x34,&timer_services)==-2&&timer_result.phase==7&&timer_result.calls==5&&timer_result.dot_attacks==0);check(fixture.sheets[3][36]==2744&&fixture.events.back().op==dot_hit_for);unsigned profile_begin=0,queries=0;for(const auto&e:fixture.events){profile_begin+=e.op==dot_profile_begin;queries+=e.op==dot_debug_query&&e.name=="isTracingChar_Attack";}check(profile_begin==1&&queries==3);++timer_prefixes;
 dh2_character_debug_destroy(fixture.debug_owner);fixture.debug_owner=nullptr;
 std::cout<<"{\"validation\":\"PASS\",\"original_gold_cases\":"<<count<<",\"checks\":"<<checks<<",\"atomic_guards\":"<<guards<<",\"service_failure_prefixes\":"<<failures<<",\"genuine_health_prefixes\":"<<health_prefixes<<",\"genuine_timer_compositions\":"<<timer_prefixes<<",\"module_library\":\""<<origin(reinterpret_cast<void*>(&dh2_character_dot_apply))<<"\",\"world_library\":\""<<origin(reinterpret_cast<void*>(&dh2_character_timer_effect))<<"\",\"data_library\":\""<<origin(reinterpret_cast<void*>(&dh2_health_hit))<<"\"}\n";
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
