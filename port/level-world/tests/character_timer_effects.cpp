#include "character_timer_effects.hpp"
#include "character_design_services.hpp"
#include "character_timers.hpp"
#include "character_ai_events.hpp"
#include "character_native_fsm.hpp"
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include <dlfcn.h>
using namespace dh2::character;
namespace {
unsigned checks=0;void check(bool v){++checks;if(!v)throw std::runtime_error("timer effects check "+std::to_string(checks));}
unsigned word(std::istream& f){unsigned v=0;f.read(reinterpret_cast<char*>(&v),4);check(bool(f));return v;}
std::int32_t integer(unsigned v){std::int32_t i;std::memcpy(&i,&v,4);return i;}
std::string text(std::istream& f,unsigned n){std::string s(n,'\0');f.read(s.data(),n);check(bool(f));return s;}
using Sheets=std::array<dh2::data::PropertySheet,4>;
using Trace=std::tuple<unsigned,unsigned,std::int32_t,std::int32_t,std::string>;
struct Fixture {
 dh2::data::PropertyRules rules;Sheets sheets;dh2::data::PropertyView view{};TimerEffectState32 state{};TimerEffectResult24 result{};TimerEffectServices16 service{};
 std::vector<std::int32_t> states,deaths;unsigned state_index=0,death_index=0,inhibited=0,mutation=0,dot_mutation=0;int failure=-1;
 std::vector<Trace> trace;std::vector<dh2::data::CombatResult> results;
 void refresh(){view={rules.defaults.data(),rules.types.data(),sheets[0].data(),sheets[1].data(),sheets[2].data(),sheets[3].data(),nullptr,0};state.properties=&view;}
 static int invoke(void* p,TimerEffectState32* state,const TimerEffectRequest40* r,std::int32_t* out,dh2::data::CombatResult* result){auto& f=*static_cast<Fixture*>(p);check(state==&f.state&&r->subject==state->owner&&r->target==((r->service==effect_dot_calculate||r->service==effect_dot_apply)?state->owner:0));f.trace.emplace_back(r->service,r->property,r->amount,r->element,r->name?r->name:"");if(int(r->service)==f.failure)return 1;
  switch(r->service){case effect_remote_update:*out=f.inhibited;break;case effect_current_state:*out=f.states[std::min(f.state_index++,unsigned(f.states.size()-1))];break;case effect_is_dead:*out=f.deaths[std::min(f.death_index++,unsigned(f.deaths.size()-1))];break;
   case effect_debug_load:break;case effect_debug_query:if(f.mutation){f.sheets[3][36]=-17;f.sheets[3][44]=513;}break;
   case effect_dot_calculate:check(result);*result={r->amount,-1,-1,-1,0,0,0,0x20080000u,-1,r->element};break;
   case effect_dot_apply:check(result);f.results.push_back(*result);if(f.dot_mutation)f.sheets[3][131]=257;break;default:check(false);
  }return 0;
 }
};
std::string origin(void* p){Dl_info d{};check(dladdr(p,&d));return d.dli_fname;}
struct Persistent {
 Fixture f;DebugSwitches* debug=nullptr;DebugFileServices24 files{};std::string directory;unsigned opens=0,regen=0,dot=0,attack=0;TimerOwner8 fields{-1,0,0,0};State current{};NativeFsm24 machine{&current,0xabcdef0123456789ull,1,0};
 AIEventOwner48 owner{0xabcdef0123456789ull,1,2,3,0,0,0,0};AIEventState64 ai{4,&owner,nullptr,0,nullptr,0,0,0,0,0,0};AIEventServices24 event_services{this,event,1u<<ai_event_helper,0};AIEventPayload24 payload{};AIEventResult16 event_result{};
 static int open(void* p,const char* name,std::uintptr_t* h){auto& s=*static_cast<Persistent*>(p);check(!std::strcmp(name,"DebugSwitches.savegame"));++s.opens;auto path=s.directory+"/"+name;*h=reinterpret_cast<std::uintptr_t>(std::fopen(path.c_str(),"rb"));return *h||errno==ENOENT?0:1;}
 static int close(void*,std::uintptr_t h){return std::fclose(reinterpret_cast<FILE*>(h))?1:0;}
 static int effect(void* p,TimerEffectState32*,const TimerEffectRequest40* r,std::int32_t* out,dh2::data::CombatResult*){auto& s=*static_cast<Persistent*>(p);switch(r->service){case effect_remote_update:return dh2_character_timer_owner_query(out,&s.fields,0)==1?0:1;case effect_is_dead:return dh2_character_timer_owner_query(out,&s.fields,1)==1?0:1;case effect_current_state:return dh2_character_native_fsm_get_integer(out,&s.machine,0)==1?0:1;case effect_debug_load:return dh2_character_debug_load(s.debug,&s.files)==1?0:1;case effect_debug_query:{std::uint32_t value=0;return dh2_character_debug_get(&value,s.debug,r->name,&s.files)==1?0:1;}default:++s.attack;return 1;} } // No fabricated damage/application provider for a positive DoT.
 static int event(void* p,AIEventState64*,const AIEventRequest40* r,std::uint32_t*){auto& s=*static_cast<Persistent*>(p);check(r->service==ai_event_helper&&((r->event==0x33&&r->operation==0x3cb77c)||(r->event==0x34&&r->operation==0x3df3f0)));TimerEffectServices16 services{&s,effect};return dh2_character_timer_effect(&s.f.result,&s.f.state,r->event,&services)==1?0:1;}
 static void expire(void* p,std::uintptr_t owner,std::int32_t e,Timer32* timer){auto& s=*static_cast<Persistent*>(p);check(owner==s.owner.owner&&timer&&timer->event==e);check(!dh2_character_ai_event(&s.event_result,&s.ai,unsigned(e),&s.payload,&s.event_services));if(e==0x33)++s.regen;else if(e==0x34)++s.dot;else check(false);}
};
}
int main(int argc,char** argv){try{check(argc==6);std::ifstream in(argv[1],std::ios::binary);check(word(in)==0x31484554);auto count=word(in);Fixture f;in.read(reinterpret_cast<char*>(f.rules.defaults.data()),896);in.read(reinterpret_cast<char*>(f.rules.types.data()),896);check(bool(in));f.state.owner=0xabcdef0123456789ull;f.service={&f,Fixture::invoke};unsigned applications=0,adds=0;
 for(unsigned i=0;i<count;++i){auto e=word(in);f.inhibited=word(in);f.state.has_aggro=word(in);f.state.aggroed=word(in);f.mutation=word(in);f.dot_mutation=word(in);auto ns=word(in),nd=word(in);f.states.clear();f.deaths.clear();for(unsigned j=0;j<ns;++j)f.states.push_back(integer(word(in)));for(unsigned j=0;j<nd;++j)f.deaths.push_back(integer(word(in)));std::vector<Trace> expected;auto nt=word(in);for(unsigned j=0;j<nt;++j){auto op=word(in),p=word(in),a=word(in),element=word(in),n=word(in);expected.emplace_back(op,p,integer(a),integer(element),text(in,n));}auto na=word(in);std::int32_t hp=0,mp=0;for(unsigned j=0;j<na;++j){auto p=word(in),a=word(in);check(p==36||p==41);(p==36?hp:mp)=integer(a);}auto nr=word(in);std::vector<dh2::data::CombatResult> results(nr);in.read(reinterpret_cast<char*>(results.data()),nr*40);in.read(reinterpret_cast<char*>(f.sheets.data()),3584);Sheets after;in.read(reinterpret_cast<char*>(after.data()),3584);check(bool(in));f.refresh();f.state_index=f.death_index=0;f.trace.clear();f.results.clear();check(dh2_character_timer_effect(&f.result,&f.state,e,&f.service)==1&&f.sheets==after&&f.trace==expected&&f.result.phase==8&&f.result.calls==nt&&f.result.regen_adds==na&&f.result.dot_attacks==nr&&f.result.hp_add==hp&&f.result.mp_add==mp&&f.results.size()==nr);check(!nr||!std::memcmp(f.results.data(),results.data(),nr*40));applications+=nr;adds+=na;
 }check(in.peek()==EOF);
 std::ifstream resets(argv[2],std::ios::binary);check(word(resets)==0x31524554);auto reset_count=word(resets);unsigned removals=0;for(unsigned i=0;i<reset_count;++i){resets.read(reinterpret_cast<char*>(f.rules.defaults.data()),896);resets.read(reinterpret_cast<char*>(f.sheets.data()),3584);Sheets after;resets.read(reinterpret_cast<char*>(after.data()),3584);check(bool(resets));f.refresh();const auto before=f.sheets;for(auto element:{INT32_MIN,-1,0,4,5,INT32_MAX}){check(dh2_character_dot_remove(&f.view,element)==1&&f.sheets==before);++removals;}check(dh2_character_cached_reset(&f.view)==1&&f.sheets==after);}check(resets.peek()==EOF);
 std::ifstream queries(argv[3],std::ios::binary);check(word(queries)==0x31514554);const auto nq=word(queries);for(unsigned i=0;i<nq;++i){TimerOwner8 fields{};queries.read(reinterpret_cast<char*>(&fields),8);auto kind=word(queries),value=word(queries);std::int32_t out=0;check(dh2_character_timer_owner_query(&out,&fields,kind)==1&&std::uint32_t(out)==value);}check(queries.peek()==EOF);
 f.state.has_aggro=f.state.aggroed=0;f.inhibited=f.mutation=f.dot_mutation=0;f.states={3};f.deaths={0};for(auto& sheet:f.sheets)sheet.fill(0);f.sheets[3][39]=f.sheets[3][40]=f.sheets[3][44]=f.sheets[3][45]=256;f.sheets[3][38]=f.sheets[3][43]=1024;for(auto& type:f.rules.types)type=8;f.refresh();unsigned guards=0;
 auto original=f.result;const auto backup=f.sheets;for(unsigned mode=0;mode<7;++mode){auto* output=&f.result;auto* state=&f.state;auto* services=&f.service;unsigned event=0x33;if(mode==0)output=nullptr;if(mode==1)state=nullptr;if(mode==2)services=nullptr;if(mode==3)event=0x35;if(mode==4)output=reinterpret_cast<TimerEffectResult24*>(f.sheets[3].data());if(mode==5)output=reinterpret_cast<TimerEffectResult24*>(&f.state);if(mode==6)output=reinterpret_cast<TimerEffectResult24*>(reinterpret_cast<char*>(&f.result)+1);check(dh2_character_timer_effect(output,state,event,services)==-1&&f.sheets==backup&&!std::memcmp(&f.result,&original,24));++guards;}
 unsigned failures=0;for(int failure=0;failure<7;++failure){f.failure=failure;f.sheets=backup;f.state_index=f.death_index=0;f.trace.clear();for(unsigned p=126;p<=131;++p)f.sheets[3][p]=256;const auto event=failure==2||failure>=5?0x34:0x33;check(dh2_character_timer_effect(&f.result,&f.state,event,&f.service)==-2&&!f.trace.empty()&&std::get<0>(f.trace.back())==unsigned(failure));++failures;}f.failure=-1;
 // Persistent TimerStore→complete RaiseAIEvent→genuine effects, with source
 // owner/FSM getters and real DebugSwitch filesystem storage. DoT is genuinely
 // inactive here; there is deliberately no accepted damage callback provider.
 Persistent live;live.directory=argv[5];live.debug=dh2_character_debug_create();check(live.debug);live.files={&live,Persistent::open,Persistent::close};std::ifstream raw(argv[4],std::ios::binary);check(word(raw)==448);raw.read(reinterpret_cast<char*>(live.f.rules.defaults.data()),896);raw.read(reinterpret_cast<char*>(live.f.rules.types.data()),896);check(bool(raw));for(auto& sheet:live.f.sheets)sheet=live.f.rules.defaults;live.f.refresh();live.f.state.owner=live.owner.owner;live.current.current=3;live.f.sheets[0][36]=live.f.sheets[0][41]=-1;live.f.sheets[1][36]=live.f.sheets[1][41]=0;live.f.sheets[3][36]=live.f.sheets[3][41]=0;live.f.sheets[3][38]=live.f.sheets[3][43]=100000;live.f.sheets[3][39]=live.f.sheets[3][44]=256;for(unsigned p=126;p<=131;++p)check(live.f.sheets[3][p]<=0);
 Timer32 slots[2];TimerStore32 store{slots,0,2,live.owner.owner,0,0};TimerServices32 timer_services{&live,Persistent::expire,nullptr,0};check(dh2_character_timer_start(&store,3000,-1,0x33,0,&timer_services)==0&&dh2_character_timer_start(&store,1000,-1,0x34,0,&timer_services)==1);for(unsigned i=0;i<6;++i)check(dh2_character_timers_update(&store,1000,0,&timer_services)==1);check(live.regen==2&&live.dot==6&&live.f.sheets[3][36]==512&&live.f.sheets[3][41]==512&&!live.attack&&live.opens==1);check(dh2_character_timers_update(&store,60000,1,&timer_services)==1&&live.regen==2&&live.dot==6);check(dh2_character_timer_pause(&store,0,1)==1&&dh2_character_timers_update(&store,3000,0,&timer_services)==1&&live.regen==2&&live.dot==9);check(dh2_character_timer_pause(&store,0,0)==1&&dh2_character_timers_update(&store,6000,0,&timer_services)==1&&live.regen==4&&live.dot==15&&live.f.sheets[3][36]==1024&&live.f.sheets[3][41]==1024&&!live.attack&&live.opens==1);
 // Positive active DoT must surface unavailable application, rather than a
 // no-op expiry provider. Its genuine debug/calculation prefix is preserved.
 live.f.sheets[3][126]=256;TimerEffectServices16 effects{&live,Persistent::effect};check(dh2_character_timer_effect(&live.f.result,&live.f.state,0x34,&effects)==-2&&live.attack==1&&live.f.result.dot_attacks==0);dh2_character_debug_destroy(live.debug);
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"gold_expiry_cases\":"<<count<<",\"exact_sheet_words\":"<<count*896<<",\"positive_regen_adds\":"<<adds<<",\"explicit_attack_fixture_applications\":"<<applications<<",\"cached_reset_cases\":"<<reset_count<<",\"actual_RemoveDot_cases\":"<<removals<<",\"owner_query_cases\":"<<nq<<",\"atomic_guards\":"<<guards<<",\"service_failure_prefixes\":"<<failures<<",\"genuine_timer_regen_expiries\":4,\"genuine_timer_inactive_DoT_expiries\":15,\"genuine_debug_file_attempts\":1,\"positive_DoT_provider_failure\":true,\"module_library\":\""<<origin(reinterpret_cast<void*>(dh2_character_timer_effect))<<"\",\"world_library\":\""<<origin(reinterpret_cast<void*>(dh2_character_ai_event))<<"\",\"debug_library\":\""<<origin(reinterpret_cast<void*>(dh2_character_debug_load))<<"\",\"data_library\":\""<<origin(reinterpret_cast<void*>(dh2_property_add))<<"\",\"full_DoT_buff_registry_or_application\":false}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
