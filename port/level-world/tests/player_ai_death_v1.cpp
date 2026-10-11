#define main existing_session_main
#include "player_skill_session_v1.cpp"
#undef main
#include "../player_skill_cleanup_session_v1.hpp"
#include "../player_ai_death_v1.hpp"
#include "../character_ai_association.hpp"
#include "../../game-data/animation_tables.hpp"
#include "../../game-data/properties.hpp"
#include <algorithm>
#include <array>
namespace death=dh2::player_ai_death_v1;
namespace ch=dh2::character;
namespace target=dh2::character::set_target;
namespace c=dh2::player_skill_cleanup_session_v1;
void cleanup_prepare(Fixture& f){f.common();p::source::Result out{};check(f.owner->prepare(&out)==p::source::Status::complete,"cleanup source preparation failed");}
using Trace=std::array<std::uint32_t,3>;
constexpr std::uintptr_t AI=0x300000001ull,PEER=0x400000002ull,GROUP=0x500000001ull;
struct Gold {std::array<std::uint32_t,13> in{};std::array<std::uint32_t,19> out{};std::vector<Trace> trace;};
template<class T>T read_gold(std::istream& file){T x{};file.read(reinterpret_cast<char*>(&x),sizeof(x));check(bool(file),"truncated death capture");return x;}
struct Core {
 Gold gold{};dh2::character_ai_initialization::State ai{};ch::Coordinator coordinator{CHAR};
 target::OwnerFacts owner{CHAR,0,0x1234,0};target::Services target_services{this,20,debug};
 std::uintptr_t group=0;
 std::array<std::int32_t,224> defaults{},types{},base{},saved{},gear{},resolved{};
 d::PropertyView death_properties{};d::AnimationTables death_tables{};
 std::vector<std::uint8_t> constants;dh2_pycst_view constants_view{};
 std::unique_ptr<death::Runtime> runtime;std::vector<Trace> trace;
 dh2::character_aggro_cleanup::PeerRef peer{PEER,0};
 unsigned call=0,fail=0,held=0,mask_calls=0,state_calls=0,state_fail=0;
 bool throw_provider=false,reenter=false,mirror=false,missing_link=false;
 std::vector<death::Operation> operations;
 Fixture* fixture=nullptr;c::Runtime* cleanup=nullptr;
 c::Result skills{},spells{};
 Core(){
  ai.identity=AI;dh2::character_ai_initialization::Result result{};
  const dh2::character_ai_initialization::Services queue{this,[](void*,auto*,auto){return 0;}};
  check(dh2::character_ai_initialization::construct(&ai,0x600000001ull,&queue,&result)==dh2::character_ai_initialization::Status::complete,"source AI constructor failed");
  check(dh2::character_ai_association::associate(&ai,CHAR)==dh2::character_ai_association::Status::complete,"source association failed");
  ch::CoordinatorBindings cb{};cb.context=this;
  cb.facts=[](void*){ch::Facts f{};f.is_player=1;return f;};
  cb.services={this,state};cb.spawn_facts=[](void*){ch::SpawnFacts f{};f.spawn_animation=0;return f;};
  coordinator.bind(cb);reset();
  death::Bindings b{};b.ai=&ai;b.coordinator=&coordinator;b.target_owner=&owner;
  b.target_services=&target_services;
  b.dead_state={&coordinator,{CHAR,&death_properties},&death_tables,&constants_view,{}};
  b.group_identity=&group;b.player_ais_identity=AIS;b.backend={this,invoke};
  runtime=std::make_unique<death::Runtime>(b);
 }
 void reset(const Gold& value=Gold{}){
  gold=value;trace.clear();operations.clear();call=mask_calls=state_calls=held=0;coordinator.stop_timers();
  coordinator.state={};coordinator.state.current=std::int32_t(gold.in[9]);coordinator.state.dead_alternate=gold.in[4];
  ai.active_ais_1c=gold.in[1]?AIS:0;ai.requested_target_3c=0;ai.target_40=gold.in[2]?PEER:0;ai.last_target_44=PEER;
  ai.alive_48=3;ai.sight_49=4;ai.sticky_4c=1;owner.target_change_marker_14d0=0x1234;
  group=gold.in[0]?GROUP:0;coordinator.set_source_despawn_animation_38(-1);
  coordinator.set_source_dead_byte_3e(123);
  defaults.fill(0);types.fill(0);base.fill(0);saved.fill(0);gear.fill(0);resolved.fill(0);
  resolved[2]=std::int32_t(gold.in[5]);
  death_properties={defaults.data(),types.data(),base.data(),saved.data(),gear.data(),resolved.data(),nullptr,0};
  death_tables={};death_tables.characters.resize(gold.in[6]);
  for(auto& row:death_tables.characters){row.fields[4]={20};row.fields[5]={30};row.fields[6]={40};row.fields[7]={10};}
  auto put32=[&](std::uint32_t n){for(unsigned i=0;i<4;++i)constants.push_back(std::uint8_t(n>>(i*8)));};
  auto put_name=[&](const char* name){const auto size=std::uint32_t(std::strlen(name));put32(size);constants.insert(constants.end(),name,name+size);};
  constants.clear();put32(2);put_name("AnimStancedAnim");put32(1);put_name("SL__LIST_IPHONE");put32(gold.in[7]);
  put_name("AnimStances");put32(1);put_name("COUNT_IPHONE");put32(8);
  check(!dh2_pycst_open(&constants_view,constants.data(),std::uint32_t(constants.size())),"synthetic death constants invalid");
  ai.tree_7c.count=gold.in[11];ai.tree_94.count=gold.in[12];
  ai.word_10=coordinator.start_timer(500,-1,0x33,0);ai.word_14=coordinator.start_timer(500,-1,0x34,0);
  check(coordinator.start_timer(1000,-1,0x35,PEER)==2,"source test timer allocation changed");
 }
 bool reject(){++call;if(call!=fail)return false;if(throw_provider)throw std::runtime_error("explicit death provider exception");return true;}
 void probe(){if(!reenter)return;reenter=false;death::Result r{};r.calls=99;auto before=r;std::string error="sentinel";
  check(runtime->died(PEER,&r,error)==death::Status::busy&&!std::memcmp(&r,&before,sizeof(r))&&error=="sentinel","death reentry changed outputs");}
 static int debug(void* raw,const target::Request* q,target::Response* r){
  auto& s=*static_cast<Core*>(raw);check(q->ai_identity==AI&&!q->owner_identity,"target Debug receiver differs");s.probe();
  if(q->operation==target::debug_switches_load)s.trace.push_back({101,0,0});
  else if(q->operation==target::debug_switch_lookup)s.trace.push_back({102,q->key,0});
  else throw std::runtime_error("null target reached unexpected service");
  if(s.reject())return 1;
  if(!s.fixture){r->word=s.gold.in[3];return 0;}
  auto& b=s.fixture->debug;auto* runtime=b.globals().singleton;
  if(q->operation==target::debug_switches_load)return runtime->load(b.globals(),b.services())==dh2::debug_switches::Status::complete?0:1;
  std::uint8_t value=0;const char* key=q->key==1?"IsTracingCharAITarget":"isTracingCharAITarget";
  if(runtime->get_switch(key,b.globals(),b.services(),value)!=dh2::debug_switches::Status::complete)return 1;
  r->word=value;return 0;
 }
 static void state(void* raw,ch::State* s,const ch::Request* q){
  auto& x=*static_cast<Core*>(raw);++x.state_calls;
  // The state kernel and all reached services run. Their external bodies are
  // declared callee fixtures here, as _SetState is in the ARM caller replay.
  if(q->service==ch::dead_focus_prelude)x.trace.push_back({16,std::uint32_t(s->animation_override),std::uint32_t(x.coordinator.source_despawn_animation_38())});
  if(x.state_calls==x.state_fail)throw std::runtime_error("explicit state service failure");
  if(q->service==ch::set_animation)s->current_animation=q->argument[0];
  if(q->service==ch::look_at&&q->argument[0]==1)check(!q->identity,"direct dead state carried killer payload");
  x.probe();
 }
 static int invoke(void* raw,const death::Request* q,death::Reply* r,std::string& error){
  auto& s=*static_cast<Core*>(raw);check(q->character==CHAR&&q->ai==AI,"death provider identity truncated");s.operations.push_back(q->operation);s.probe();
  const auto op=q->operation;const auto dir=std::uint32_t(q->direction);
  // Port holds are released even when a reached provider reports failure.
  if(op==death::Operation::release_peer){check(s.held&&q->lifetime_handle==&s.peer,"unbalanced peer release");--s.held;if(s.reject()){error="explicit release failure";return 1;}return 0;}
  if(s.reject()){error="explicit death provider failure";return 1;}
  switch(op){
  case death::Operation::group_died:check(q->subject==GROUP&&q->killer==PEER,"GroupInfo/killer truncated");s.trace.push_back({0,0,0});break;
  case death::Operation::ais_died:check(q->subject&&q->killer==PEER,"active AIS/killer lost");s.trace.push_back({1,0,0});break;
  case death::Operation::anim_stance:check(!s.fixture&&q->subject==CHAR,"real absent inventory reached stance");r->word=s.gold.in[8];s.trace.push_back({5,0,std::uint32_t(r->word)});if(s.gold.in[10])s.coordinator.state.dead_alternate=0;break;
  case death::Operation::relations:r->count=dir?s.ai.tree_94.count:s.ai.tree_7c.count;r->peers=r->count?&s.peer:nullptr;s.trace.push_back({6,dir,r->count});if(s.missing_link&&r->count){error="actual linked peer provider absent";return 1;}break;
  case death::Operation::retain_peer:check(q->subject==PEER,"peer identity truncated");++s.held;r->lifetime_handle=&s.peer;break;
  case death::Operation::contains_mirror:check(q->subject==PEER&&q->lifetime_handle==&s.peer,"mirror lifetime lost");r->word=s.mirror;break;
  case death::Operation::erase_mirror:check(q->subject==PEER&&s.mirror,"mirror erase without relation");s.mirror=false;break;
  case death::Operation::clear_relations:if(dir)s.ai.tree_94.count=0;else s.ai.tree_7c.count=0;s.trace.push_back({10,dir,0});break;
  case death::Operation::notify_deaggro:check(q->subject==PEER&&q->lifetime_handle==&s.peer&&s.held,"notification after lifetime release");s.trace.push_back({11,dir,2});break;
  case death::Operation::release_peer:check(s.held&&q->lifetime_handle==&s.peer,"unbalanced peer release");--s.held;break;
  case death::Operation::skill_cleanup:s.trace.push_back({13,0,0});if(s.cleanup)return s.cleanup->cleanup(c::List::skill,s.skills,error);break;
  case death::Operation::spell_cleanup:s.trace.push_back({14,0,0});if(s.cleanup)return s.cleanup->cleanup(c::List::faery,s.spells,error);break;
  }
  return 0;
 }
 std::array<std::uint32_t,19> output(const death::Result& r)const{
  const auto& ts=coordinator.timers();return {std::uint32_t(coordinator.state.current),std::uint32_t(r.death_animation),std::uint32_t(coordinator.source_despawn_animation_38()),coordinator.source_dead_byte_3e(),coordinator.state.dead_alternate,
   std::uint32_t(ai.requested_target_3c),std::uint32_t(ai.target_40),std::uint32_t(ai.last_target_44),ai.alive_48,ai.sight_49,ai.sticky_4c,owner.target_change_marker_14d0,ai.word_10,ai.word_14,ts.slots[0].active,ts.slots[1].active,ts.slots[2].active,ai.tree_7c.count,ai.tree_94.count};
 }
};
int main(int argc,char** argv){try{
 check(argc==4,"death gate needs cache Debug output and original binary");std::ifstream file(argv[3],std::ios::binary);check(read_gold<std::uint32_t>(file)==0x31444950,"bad death capture magic");auto count=read_gold<std::uint32_t>(file);
 std::vector<Gold> gold;for(unsigned i=0;i<count;++i){Gold g;g.in=read_gold<decltype(g.in)>(file);g.out=read_gold<decltype(g.out)>(file);auto n=read_gold<unsigned>(file);while(n--)g.trace.push_back(read_gold<Trace>(file));gold.push_back(g);}
 unsigned original=0,prefix=0,guards=0,actual=0;
 for(const auto& g:gold){Core core;core.reset(g);death::Result r{};std::string error;check(core.runtime->died(PEER,&r,error)==death::Status::complete,"original death case failed");
  auto normalized=core.trace;normalized.erase(std::remove_if(normalized.begin(),normalized.end(),[](const Trace& t){return t[0]>=2&&t[0]<=5;}),normalized.end());
  auto expected=g.trace;expected.erase(std::remove_if(expected.begin(),expected.end(),[](const Trace& t){return t[0]>=2&&t[0]<=5;}),expected.end());
  const auto target_call=std::find_if(normalized.begin(),normalized.end(),[](const Trace& t){return t[0]==101;});
  if(g.in[1])normalized.insert(target_call,{17,0,0});
  // Source TMR_Stop bodies are separate selected calls after _SetState.
  auto relations=std::find_if(normalized.begin(),normalized.end(),[](const Trace& t){return t[0]==6;});relations=normalized.insert(relations,{18,0,0});normalized.insert(relations+1,{18,1,0});
  if(core.output(r)!=g.out||normalized!=expected){
   std::ostringstream detail;detail<<"original death output/trace mismatch case"<<original<<" output=";
   for(auto value:core.output(r))detail<<value<<',';
   detail<<" expected=";for(auto value:g.out)detail<<value<<',';
   detail<<" trace=";for(const auto& value:normalized)detail<<'['<<value[0]<<','<<value[1]<<','<<value[2]<<']';
   detail<<" expected_trace=";for(const auto& value:expected)detail<<'['<<value[0]<<','<<value[1]<<','<<value[2]<<']';
   throw std::runtime_error(detail.str());
  }
  check(r.timer_ids_cleared&&r.outgoing_completed&&r.incoming_completed&&r.skill_completed&&r.spell_completed&&!core.held,"death did not complete source tail");++original;
 }
 Gold linked=gold.back();Core done;done.reset(linked);death::Result complete{};std::string why;check(done.runtime->died(PEER,&complete,why)==death::Status::complete,"linked baseline failed");const auto calls=done.call;
 for(bool thrown:{false,true})for(unsigned fail=1;fail<=calls;++fail){Core core;core.reset(linked);core.fail=fail;core.throw_provider=thrown;death::Result r{};std::string error;
  const auto status=core.runtime->died(PEER,&r,error);check(status==death::Status::failed&&!error.empty(),"reached provider failure returned success");
  check(!core.held,"failure leaked peer hold");check(core.coordinator.timers().slots[2].active,"death stopped unrelated skill timer");
  if(r.timer_ids_cleared)check(core.ai.word_10==UINT32_MAX&&core.ai.word_14==UINT32_MAX&&!core.coordinator.timers().slots[0].active&&!core.coordinator.timers().slots[1].active,"failure lost timer prefix");
  if(core.operations.size()&&core.operations.back()==death::Operation::spell_cleanup)check(r.skill_completed&&!r.spell_completed,"spell failure replayed skill cleanup");
  ++prefix;
 }
 Core state;state.reset(gold[1]);death::Result baseline{};check(state.runtime->died(PEER,&baseline,why)==death::Status::complete,"state baseline failed");
 for(unsigned i=1;i<=state.state_calls;++i){Core core;core.reset(gold[1]);core.state_fail=i;death::Result r{};std::string error;check(core.runtime->died(PEER,&r,error)==death::Status::failed&&!r.timer_stops&&core.ai.word_10==0&&core.ai.word_14==1&&core.coordinator.timers().slots[0].active,"state failure crossed timer prefix");++prefix;}
 Core missing;missing.reset(linked);missing.missing_link=true;death::Result r{};check(missing.runtime->died(PEER,&r,why)==death::Status::failed&&r.timer_ids_cleared&&missing.ai.tree_7c.count==1&&missing.ai.tree_94.count==1&&!r.skill_completed,"missing linked service invented map cleanup");++prefix;
 Core busy;busy.reset(gold[1]);busy.reenter=true;check(busy.runtime->died(PEER,&r,why)==death::Status::complete,"reentry outer delivery failed");++guards;
 r.calls=99;const auto sentinel=r;why="sentinel";
 for(auto* control:{static_cast<void*>(busy.runtime.get()),static_cast<void*>(&busy.ai),static_cast<void*>(&busy.coordinator),static_cast<void*>(&busy.owner),static_cast<void*>(&busy.death_properties),static_cast<void*>(&busy.death_tables),static_cast<void*>(&busy.constants_view),static_cast<void*>(&busy.group),static_cast<void*>(busy.coordinator.timers().slots)}){
  check(busy.runtime->died(PEER,reinterpret_cast<death::Result*>(control),why)==death::Status::invalid_argument&&why=="sentinel","death output aliases owner");++guards;
 }
 check(busy.runtime->died(PEER,&r,*reinterpret_cast<std::string*>(&busy.ai))==death::Status::invalid_argument&&!std::memcmp(&r,&sentinel,sizeof(r)),"death error aliases owner");++guards;
 Catalogue catalogue(argv[1]);auto load=[&](const char* p){return read(std::filesystem::path(argv[1])/p);};
 d::Dictionary clips;d::AnimationTables animations;auto dn=load("data/pydata/animations_dictionary_pyarraynames.bin"),dv=load("data/pydata/animations_dictionary_pyarray.bin"),ar=load("data/pydata/animations_pyarray.bin"),an=load("data/pydata/animations_pyarraynames.bin"),af=load("data/pydata/animations_pystructnames.bin");
 check(d::load_dictionary(bytes(dn),bytes(dv),clips,why)&&d::load_animation_tables(bytes(ar),bytes(an),bytes(af),clips,animations,why),why.c_str());
 for(const char* name:{"KnightPlayerBase","MagePlayerBase","RoguePlayerBase"}){
  Fixture f(argv[1],std::filesystem::path(argv[2])/name,catalogue,name);cleanup_prepare(f);auto* script=f.session.get();c::Runtime cleanup(&script,*f.owner,CHAR);
  const auto vm=f.session->vm();const auto loads=f.session->statistics().load_calls;const auto properties=f.properties;const auto skill_slots=f.owner->slots(c::List::skill),spell_slots=f.owner->slots(c::List::faery);
  Core core;core.reset(gold[1]);core.fixture=&f;core.cleanup=&cleanup;core.death_tables=animations;core.death_properties=f.view;core.constants=load("data/pydata/animations_pycst.bin");
  check(!dh2_pycst_open(&core.constants_view,core.constants.data(),std::uint32_t(core.constants.size())),"actual death constants invalid");
  check(core.ai.active_ais_1c==f.session->ais_identity()&&f.owner->state().ai==core.ai.active_ais_1c,"active/preparation/Session AIS differ");
  if(core.runtime->died(PEER,&r,why)!=death::Status::complete)throw std::runtime_error(std::string(name)+" actual death prefix "+why);
  check(core.skills.completed+core.spells.completed==13&&core.skills.examined+core.spells.examined==21&&core.skills.cleanup_calls+core.spells.cleanup_calls==13,"death skipped source same-VM cleanup");
  check(r.default_ais_died&&r.state_completed&&r.timer_ids_cleared&&core.coordinator.state.current==12&&!core.ai.target_40&&!core.ai.last_target_44,"actual death state/AI mismatch");
  check(f.session->vm()==vm&&f.session->statistics().load_calls==loads&&f.owner->slots(c::List::skill)==skill_slots&&f.owner->slots(c::List::faery)==spell_slots&&!std::memcmp(&properties,&f.properties,sizeof(properties)),"death replayed/replaced source owners");
  check(core.coordinator.timers().slots[2].active,"death blanket-stopped timer35");++actual;
 }
 std::cout<<"{\"validation\":\"PASS\",\"original_route_cases\":"<<original<<",\"failure_prefix_cases\":"<<prefix<<",\"guard_cases\":"<<guards<<",\"actual_player_classes\":"<<actual<<",\"same_session\":true,\"native_death\":false}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
