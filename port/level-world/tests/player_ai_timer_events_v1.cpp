#define main player_skill_session_existing_main
#include "player_skill_session_v1.cpp"
#undef main
#include "../player_ai_timer_events_v1.hpp"
#include "../character_ai_association.hpp"
#include "../character_player_buffs_v1.hpp"
#include <array>

namespace pt=dh2::player_ai_timer_events_v1;
namespace rt=dh2::character_regen_tick_v1;
namespace c=dh2::character;
namespace bf=dh2::character_player_buffs_v1;
namespace {
using Trace=std::array<std::uint32_t,3>;
struct Gold {std::array<std::uint32_t,15> words{};std::vector<Trace> trace;};
template<class T>T read_value(std::istream& f){T value{};f.read(reinterpret_cast<char*>(&value),sizeof(value));check(bool(f),"truncated original tick fixture");return value;}
struct Core {
 const Gold& gold;std::map<unsigned,std::uint32_t> properties;
 std::vector<Trace> trace;unsigned call=0,fail_at=0;
 explicit Core(const Gold& g):gold(g),properties{{39,g.words[7]},{40,g.words[8]},{44,g.words[9]},{45,g.words[10]}}{}
 static int invoke(void* raw,const rt::Request* q,rt::Reply* r){
  auto& s=*static_cast<Core*>(raw);Trace t{std::uint32_t(q->operation),0,0};
  if(q->operation==rt::Operation::read_property){t[1]=q->property;t[2]=s.properties[q->property];r->word=t[2];}
  else if(q->operation==rt::Operation::regen_hp||q->operation==rt::Operation::regen_mp)t[2]=q->amount;
  else if(q->operation==rt::Operation::string_construct){check(std::string(q->text)=="isTracingChar_Stats","actual tracing key differs");r->identity=0x900000001ull;}
  else if(q->operation==rt::Operation::debug_query)r->word=123;
  s.trace.push_back(t);++s.call;
  if(s.call==s.fail_at)return 1;
  if(q->operation==rt::Operation::regen_hp&&s.gold.words[11]){s.properties[44]=777;s.properties[45]=778;}
  return 0;
 }
};
struct Native {
 Fixture f;dh2::character_ai_initialization::State ai{};c::Coordinator coordinator{CHAR};
 dh2::object_update_culling::Object object{CHAR,UINT32_MAX,0,0,{0,0}};
 std::uint32_t dead=0;std::uint8_t dead_byte=0;dh2::character_dot_attack::Runtime attack;
 dh2::character_dot_attack::Actor actor{CHAR,nullptr};dh2::character_dot_attack::CombatContext combat{};
 d::CombatRandom random{};dh2::character_dot_attack::Storage attack_storage{};
 std::unique_ptr<pt::Runtime> runtime;pt::Result output{};std::string error;
 std::unique_ptr<bf::Owner> buffs;std::uintptr_t buff_instance=0;
 std::vector<unsigned> order;unsigned facts=0,machine_services=0,applications=0;
 bool fail_route=false,throw_route=false,mutate_event=false,fail_apply=false,reenter=false;
 Native(const std::filesystem::path& cache,const std::filesystem::path& folder,Catalogue& cat,const char* name,bool attack_provider=true,bool apply_provider=true,bool byte_dead=false)
  :f(cache,folder,cat,name,false){
  ai.identity=0x300000001ull;const dh2::character_ai_initialization::Services queue{this,[](void*,auto*,auto){return 0;}};
  dh2::character_ai_initialization::Result result{};
  check(dh2::character_ai_initialization::construct(&ai,0x410000001ull,&queue,&result)==dh2::character_ai_initialization::Status::complete,"actual CharAI constructor failed");
  check(dh2::character_ai_association::associate(&ai,CHAR)==dh2::character_ai_association::Status::complete,"actual Character association failed");
  coordinator.state.current=3;
  coordinator.bind({this,[](void* p){++static_cast<Native*>(p)->facts;return c::Facts{};},{this,[](void* p,c::State*,const c::Request*){++static_cast<Native*>(p)->machine_services;}},
    before,after,nullptr,route});
  actor.resolved=f.view.resolved;attack_storage={&actor,1,&combat,&random,&f.debug.globals(),&f.debug.services()};
  pt::Bindings b{&ai,&coordinator,&object,0x420000001ull,reinterpret_cast<std::uintptr_t>(&f.properties),&f.view,byte_dead?nullptr:&dead,&f.debug.globals(),&f.debug.services(),attack_provider?&attack:nullptr,attack_provider?&attack_storage:nullptr,{this,apply_provider?apply:nullptr}};
  if(byte_dead)b.dead_byte=&dead_byte;
  runtime=std::make_unique<pt::Runtime>(b);
 }
 ~Native(){
  coordinator.stop_timers();runtime.reset();f.session.reset();
  if(buffs){bf::Result out{};check(buffs->retire(&out)==bf::Status::complete,"buff source retirement failed");buffs.reset();}
 }
 static int buff_service(void* raw,d::PropertyView* view,const bf::Request* q,bf::Response* out){
  auto& s=*static_cast<Native*>(raw);check(view==&s.f.view&&q->character==CHAR,"buff borrowed property/Character differs");
  switch(q->operation){
  case bf::Operation::timer_start:out->word=s.coordinator.start_timer(q->duration,q->repeat,q->event,q->subject);return 0;
  case bf::Operation::timer_stop:return q->id==-1?0:s.coordinator.stop_timer(std::uint32_t(q->id));
  case bf::Operation::timer_time_left:{
   if(q->id<0||std::uint32_t(q->id)>=s.coordinator.timers().count)return 1;
   const auto& t=s.coordinator.timers().slots[q->id];out->elapsed=t.elapsed_ms;out->duration=t.duration_ms;return 0;
  }
  case bf::Operation::fx_release:return q->subject?1:0;
  default:return 1; // No reached class/nonnull FX provider is fabricated.
  }
 }
 static void before(void* raw,c::Coordinator&,std::int32_t,c::Timer32& timer,std::uint32_t){auto& s=*static_cast<Native*>(raw);s.order.push_back(1);if(s.mutate_event)timer.event=0x2a;}
 static void after(void* raw,c::Coordinator&,std::int32_t,c::Timer32&,std::uint32_t){static_cast<Native*>(raw)->order.push_back(3);}
 static c::TimerRouting route(void* raw,c::Coordinator& coord,std::int32_t event,c::Timer32& timer,std::uint32_t){
  auto& s=*static_cast<Native*>(raw);s.order.push_back(2);
  if(s.throw_route)throw std::runtime_error("injected source route throw");
  if(s.fail_route)return c::TimerRouting::failed;
  if(event!=0x33&&event!=0x34&&event!=0x36)return c::TimerRouting::machine;
  if(event==0x36){++s.applications;return c::TimerRouting::delivered;} // Explicit direct BuffExpired provider fixture.
  check(&coord==&s.coordinator,"sole Coordinator identity differs");
  return s.runtime->deliver(event,&timer,&s.output,s.error)==pt::Status::complete?c::TimerRouting::delivered:c::TimerRouting::failed;
 }
 static int apply(void* raw,const dh2::character_dot_tick::ApplyRequest* q,std::string& error){
  auto& s=*static_cast<Native*>(raw);++s.applications;
  check(q->attacker==CHAR&&q->defender==CHAR&&!q->mode&&q->result->mask==0x20080000u,"real DoT attack/application source args differ");
  if(s.reenter){s.reenter=false;pt::Result out{};out.event=91;std::string why="sentinel";check(s.runtime->deliver(0x34,s.coordinator.timers().slots,&out,why)==pt::Status::busy&&out.event==91&&why=="sentinel","same tick reentry changed output");}
  if(s.fail_apply){error="explicit Player application failure";return 1;}
  check(!dh2_property_add(&s.f.view,36,-q->result->amount),"explicit application fixture failed");return 0;
 }
 void set(unsigned property,std::int32_t word){
  if((f.view.types[property]&4)!=0){
   if(!buffs){
    buffs=bf::Owner::create({CHAR,&f.view,{this,buff_service},std::uint32_t(f.catalogue.classes.rows.size()),UINT32_MAX});check(bool(buffs),"sole buff owner creation failed");
    bf::Result out{};check(buffs->add(1,0,1,1,-1,"timer fixture",&out)==bf::Status::complete,"actual buff instance creation failed");buff_instance=out.instance;
   }
   std::int32_t* sheet=nullptr;check(buffs->owned_sheet(buff_instance,&sheet)&&!dh2_property_set_to_sheet(&f.view,property,word,sheet),"canonical owned buff property set failed");
  }else check(!dh2_property_set(&f.view,property,word),"canonical property set failed");
 }
 void tick(unsigned event,unsigned dt=10){coordinator.start_timer(10,0,event,0);check(coordinator.update_timers(dt,0)==1,"source timer scan failed");}
};
void cases(const std::filesystem::path& cache,const std::filesystem::path& folder,const std::filesystem::path& original){
 std::ifstream input(original,std::ios::binary);check(read_value<unsigned>(input)==0x31544950,"original tick fixture magic differs");const auto n=read_value<unsigned>(input);
 std::vector<Gold> gold;for(unsigned i=0;i<n;++i){Gold g;g.words=read_value<std::array<std::uint32_t,15>>(input);for(unsigned j=0;j<g.words[14];++j)g.trace.push_back(read_value<Trace>(input));gold.push_back(std::move(g));}
 Catalogue cat(cache);unsigned original_cases=0,native_cases=0,prefixes=0,guards=0;
 for(const auto& g:gold)if(g.words[0]==0x33&&!g.words[12]){
  Core core(g);rt::State state{CHAR,0x500000001ull,0x600000001ull};rt::Globals globals{0x700000001ull};rt::Services services{&core,Core::invoke};rt::Result result{};
  check(rt::tick(&state,&globals,g.words[13],&services,&result)==rt::Status::complete&&core.trace==g.trace&&result.hp_completed&&result.mp_completed,"actual RegenTick ARM trace differs");++original_cases;
  for(unsigned fail=1;fail<=8;++fail){Core f(g);f.fail_at=fail;services.context=&f;check(rt::tick(&state,&globals,g.words[13],&services,&result)==rt::Status::service_failed&&f.trace.size()==fail&&std::equal(f.trace.begin(),f.trace.end(),g.trace.begin()),"failed RegenTick erased prefix or added cleanup");++prefixes;}
 }
 for(const auto* name:{"KnightPlayerBase","MagePlayerBase","RoguePlayerBase"}){
  Native s(cache,folder/name,cat,name);auto* vm=s.f.session->vm();
  s.set(36,0);s.set(41,0);s.set(39,256);s.set(40,512);s.set(44,128);s.set(45,384);
  const auto hp=s.f.view.resolved[39],mp=s.f.view.resolved[44];s.tick(0x33);
  check(s.order==std::vector<unsigned>({1,2,3})&&!s.facts&&!s.machine_services&&!s.output.in_combat&&s.output.regen.calls==8&&
    s.output.hp.added&&s.output.mp.added&&s.f.view.resolved[36]==hp&&s.f.view.resolved[41]==mp&&s.runtime->retained_strings()==0,"native real Debug/property/regen route differs");++native_cases;
  s.order.clear();s.object.remote_word_110=0;s.tick(0x33);check(s.output.regen_skipped&&!s.output.regen.calls&&!s.facts&&s.f.session->vm()==vm,"remote source gate ran regen/frame/VM");++native_cases;
  s.object.remote_word_110=UINT32_MAX;
  for(int state:{5,6,7}){s.coordinator.state.current=state;s.tick(0x33);check(s.output.in_combat&&s.output.regen.hp_property==40&&s.output.regen.mp_property==45,"actual Coordinator combat-state rate differs");++native_cases;}
  s.coordinator.state.current=3;s.ai.tree_7c.count=1;s.tick(0x33);check(s.output.in_combat,"actual aggro count leaf ignored");++native_cases;s.ai.tree_7c.count=0;s.ai.tree_94.count=1;s.tick(0x33);check(s.output.in_combat,"actual aggroed count leaf ignored");++native_cases;
  for(unsigned p=126;p<132;++p)s.set(p,0);
  s.ai.paused_18=255;s.dead=7;s.tick(0x34);check(s.output.dots.property_reads==6&&!s.output.dots.attacks&&!s.facts&&s.f.session->vm()==vm,"current DoT zero loop called combat/frame or ordinary paused gate");++native_cases;
  s.set(126,256);s.tick(0x34);
  check(s.output.dots.dead_skips==1&&!s.output.dots.attacks&&!s.applications,"dead DoT positive source gate differs");++native_cases;
  s.dead=0;s.reenter=true;s.tick(0x34);check(s.output.dots.attacks==1&&s.output.dots.applications==1&&s.output.attack.calculated==1&&s.applications==1&&!s.facts,"same real DotAttack/runtime application route differs");++native_cases;++guards;
 }
 {Native s(cache,folder/"byte-dead",cat,"KnightPlayerBase",true,true,true);for(unsigned p=126;p<132;++p)s.set(p,0);s.set(126,256);
  s.dead_byte=1;s.tick(0x34);check(s.output.dots.dead_skips==1&&!s.output.dots.attacks&&!s.applications,"byte-backed Character IsDead source gate differs");++native_cases;}
 {Native s(cache,folder/"prefix",cat,"KnightPlayerBase");for(unsigned p=126;p<132;++p)s.set(p,0);s.set(126,256);s.fail_apply=true;
  const auto id=s.coordinator.start_timer(10,2,0x34,0);bool failed=false;try{s.coordinator.update_timers(25,0);}catch(...){failed=true;}
 const auto& timer=s.coordinator.timers().slots[id];check(failed&&s.output.dots.attacks==1&&!s.output.dots.applications&&s.output.attack.calculated&&
   timer.elapsed_ms==15&&timer.repeat==1&&timer.active&&!s.coordinator.timers().update_depth&&s.order==std::vector<unsigned>({1,2})&&!s.facts,"source timer/provider failure prefix lost");++prefixes;}
 for(unsigned missing=0;missing<2;++missing){Native s(cache,folder/("missing-provider"+std::to_string(missing)),cat,"KnightPlayerBase",missing!=0,false);
  for(unsigned p=126;p<132;++p)s.set(p,0);
  s.set(126,256);s.coordinator.start_timer(10,0,0x34,0);
  bool failed=false;try{s.coordinator.update_timers(10,0);}catch(...){failed=true;}
  check(failed&&s.output.dots.positive_properties==1&&!s.output.dots.applications&&!s.applications&&
    s.output.attack.calculated==missing&&s.error==(missing?"Player F_ApplyResult provider unavailable":"Player F_DotAttack provider unavailable")&&
    s.order==std::vector<unsigned>({1,2})&&!s.facts&&!s.coordinator.timers().slots[0].active&&!s.coordinator.timers().update_depth,"missing reached provider became successful empty DoT");++prefixes;
 }
 for(unsigned failure=0;failure<2;++failure){Native s(cache,folder/("route-failure"+std::to_string(failure)),cat,"KnightPlayerBase");s.fail_route=failure==0;s.throw_route=failure==1;
  s.coordinator.start_timer(10,0,0x33,0);bool failed=false;try{s.coordinator.update_timers(10,0);}catch(...){failed=true;}
  check(failed&&!s.coordinator.timers().slots[0].active&&!s.coordinator.timers().update_depth&&s.order==std::vector<unsigned>({1,2})&&!s.facts,"typed route failure forwarded or after-called");++prefixes;}
 {Native s(cache,folder/"captured",cat,"KnightPlayerBase");s.mutate_event=true;s.tick(0x33);check(s.output.event==0x33&&!s.facts,"observer mutation changed captured event route");++native_cases;
  s.mutate_event=false;s.order.clear();s.tick(0x36);check(s.applications==1&&!s.facts&&s.order==std::vector<unsigned>({1,2,3}),"direct buff source route forwarded machine");++native_cases;
  s.coordinator.state.current=5;s.coordinator.state.attack_gate=7;s.order.clear();s.tick(0x2a);check(s.facts==1&&s.coordinator.state.attack_gate==6&&s.order==std::vector<unsigned>({1,2,3}),"fallback source machine route changed");++native_cases;}
 {Native s(cache,folder/"guards",cat,"KnightPlayerBase");s.coordinator.start_timer(10,0,0x33,0);pt::Result result{};result.event=91;std::string error="sentinel";c::Timer32 foreign{};
  check(s.runtime->deliver(0x33,&foreign,&result,error)==pt::Status::invalid_argument&&result.event==91&&error=="sentinel","foreign timer guard mutated output");++guards;
  check(s.runtime->deliver(0x35,s.coordinator.timers().slots,&result,error)==pt::Status::invalid_argument&&result.event==91,"unsupported tick invented handler");++guards;}
 std::cout<<"{\"validation\":\"PASS\",\"original_regen_traces\":"<<original_cases<<",\"native_cases\":"<<native_cases<<",\"failure_prefixes\":"<<prefixes<<",\"guards\":"<<guards<<",\"single_vm_unchanged\":true,\"no_added_fsm_for_delivered\":true,\"selected_native_build\":false,\"live_gameplay\":false}\n";
}
}
int main(int argc,char** argv){try{check(argc==4,"cache/folder/original cases required");cases(argv[1],argv[2],argv[3]);return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
