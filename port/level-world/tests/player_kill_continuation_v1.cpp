#define main existing_session_main
#include "player_skill_session_v1.cpp"
#undef main
#include "../player_kill_continuation_v1.hpp"
#include "../player_skill_cleanup_session_v1.hpp"
#include "../player_ai_death_v1.hpp"
#include "../character_ai_association.hpp"
#include "../character_ai_events.hpp"
#include "../../game-data/animation_tables.hpp"
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
 std::array<std::int32_t,224> defaults{},types{},base{},saved{},gear{},resolved{};
 d::PropertyView death_properties{};d::AnimationTables* tables=nullptr;
 std::uintptr_t group=0;
 std::unique_ptr<death::Runtime> runtime;std::vector<Trace> trace;
 dh2::character_aggro_cleanup::PeerRef peer{PEER,0};
 unsigned call=0,fail=0,held=0,state_calls=0,state_fail=0;
 bool throw_provider=false,reenter=false,mirror=false,missing_link=false;
 std::vector<death::Operation> operations;
 Fixture* fixture=nullptr;c::Runtime* cleanup=nullptr;
 std::vector<std::uint8_t> constants;dh2_pycst_view constants_view{};c::Result skills{},spells{};
 Core(){
  ai.identity=AI;dh2::character_ai_initialization::Result result{};
  const dh2::character_ai_initialization::Services queue{this,[](void*,auto*,auto){return 0;}};
  check(dh2::character_ai_initialization::construct(&ai,0x600000001ull,&queue,&result)==dh2::character_ai_initialization::Status::complete,"source AI constructor failed");
  check(dh2::character_ai_association::associate(&ai,CHAR)==dh2::character_ai_association::Status::complete,"source association failed");
  coordinator.bind({this,[](void*){ch::Facts f{};f.is_player=1;return f;},{this,state}});
  reset();
 }
 void bind_runtime(){
  death::Bindings bindings{};bindings.ai=&ai;bindings.coordinator=&coordinator;
  bindings.target_owner=&owner;bindings.target_services=&target_services;
  bindings.dead_state={&coordinator,{CHAR,&death_properties},tables,
      &constants_view,{}};
  bindings.group_identity=&group;bindings.player_ais_identity=AIS;
  bindings.backend={this,invoke};runtime=std::make_unique<death::Runtime>(bindings);
 }
 void reset(const Gold& value=Gold{}){
  gold=value;trace.clear();operations.clear();call=state_calls=held=0;coordinator.stop_timers();
  coordinator.state={};coordinator.state.current=std::int32_t(gold.in[9]);coordinator.state.dead_alternate=gold.in[4];
  ai.active_ais_1c=gold.in[1]?AIS:0;ai.requested_target_3c=0;ai.target_40=gold.in[2]?PEER:0;ai.last_target_44=PEER;
  ai.alive_48=3;ai.sight_49=4;ai.sticky_4c=1;owner.target_change_marker_14d0=0x1234;
  coordinator.set_source_despawn_animation_38(-1);
  coordinator.set_source_dead_byte_3e(123);
  group=gold.in[0]?GROUP:0;
  resolved.fill(0);resolved[2]=std::int32_t(gold.in[5]);
  death_properties={defaults.data(),types.data(),base.data(),saved.data(),gear.data(),resolved.data(),nullptr,0};
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
  case death::Operation::anim_stance:check(!s.fixture,"real absent inventory reached stance");r->word=s.gold.in[8];s.trace.push_back({5,0,std::uint32_t(r->word)});if(s.gold.in[10])s.coordinator.state.dead_alternate=0;break;
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
namespace k=dh2::player_kill_continuation_v1;
constexpr std::uintptr_t APP=0x600000001ull,MANAGER=0x700000001ull,
 FRESH_MANAGER=0x700000002ull,TROPHY=0x800000001ull,FRESH_TROPHY=0x800000002ull;
using KTrace=std::array<std::int32_t,5>;
std::int32_t signed_int(std::int32_t raw){auto bits=std::uint32_t(raw);bits=(bits>>8)|((bits&0x80000000u)?0xff000000u:0u);std::int32_t value;std::memcpy(&value,&bits,4);return value;}
struct KillCore {
 Fixture& fixture;std::uint32_t dead=1;bool local=true,online=false,player=true;
 unsigned calls=0,fail=0,get_calls=0,application_calls=0,dead_calls=0;
 std::int32_t trophy_id=7,outer_dead=0,inner_dead=0;
 bool throw_service=false,after_add_failure=false,reenter=false,missing_locality=false,
  null_application=false,null_trophy=false,missing_general=false,kill_failure=false,
  event_failure=false,mutate_trophy=false,mutate_manager=false,stale_after_add=false;
 std::uintptr_t current_trophy=TROPHY;
 std::vector<std::int32_t> fresh_counts;
 std::vector<KTrace> trace;std::vector<std::array<std::int32_t,4>> event_trace;
 k::Runtime runtime; k::CtrlCaller caller; k::Result resumed{};
 Core* actual_death=nullptr;bool boundary_events=false;
 ch::AIEventOwner48 event_owner{};ch::AIEventState64 event_state{};
 std::array<std::uintptr_t,51> virtuals{};
 ch::AIEventResult16 event_result{};std::string event_error;
 KillCore(Fixture& f):fixture(f),runtime({CHAR,&dead,&f.view,{this,invoke}}),caller(CHAR,{this,invoke}){
  check(!dh2_property_set(&f.view,36,0),"source HP zero failed");
 }
 static std::int32_t ident(std::uintptr_t id){return id==CHAR?1:id==MANAGER?2:id==FRESH_MANAGER?3:id==TROPHY?4:id==FRESH_TROPHY?5:0;}
 void probe(){if(!reenter)return;reenter=false;k::Result r{};r.calls=99;auto saved=r;std::string error="sentinel";
  check(runtime.continue_after_hp0(PEER,0,&r,error)==k::Status::busy&&error=="sentinel"&&!std::memcmp(&r,&saved,sizeof(r)),"continuation reentry changed outputs");
 }
 static std::int32_t event(void* raw,ch::AIEventState64* state,const ch::AIEventRequest40* q,std::uint32_t*){
  auto& s=*static_cast<KillCore*>(raw);
  check(state==&s.event_state&&q->event==2&&q->payload==PEER,"event2 identity/payload changed");
  s.event_trace.push_back({std::int32_t(q->service),std::int32_t(q->operation),2,1});
  if(q->service==ch::ai_event_virtual){
   check(q->operation==0x24&&q->subject==AI&&q->callee==s.virtuals[0x24/4],"event2 did not use actual virtual24 table");
   if(s.event_failure)return 1;
   if(!s.actual_death)return 0;
   death::Result r{};
   return s.actual_death->runtime->died(PEER,&r,s.event_error)==death::Status::complete?0:1;
  }
  check(q->service==ch::ai_event_state_event&&q->subject==s.event_owner.state_machine,"event2 machine receiver changed");
  if(s.actual_death){check(s.actual_death->coordinator.state.current==12&&s.actual_death->skills.completed+s.actual_death->spells.completed==13,"outer FSM event ran before complete OnDied");
   return s.actual_death->coordinator.event(2,PEER)<0?1:0;
  }return 0;
 }
 int raise_event(){
  virtuals[0x24/4]=reinterpret_cast<std::uintptr_t>(&event);
  event_owner={CHAR,0x900000001ull,(actual_death?reinterpret_cast<std::uintptr_t>(&actual_death->coordinator):reinterpret_cast<std::uintptr_t>(this)),reinterpret_cast<std::uintptr_t>(&fixture.view),0,255,0,0};
  event_state={AI,&event_owner,virtuals.data(),AIS,nullptr,255,0,255,0,0,0};
  const ch::AIEventPayload24 payload{PEER,0,0,0};const ch::AIEventServices24 services{this,event,3,0};
  return dh2_character_ai_event(&event_result,&event_state,2,&payload,&services);
 }
 static int invoke(void* raw,const k::Request* q,k::Reply* r,std::string& error){
  auto& s=*static_cast<KillCore*>(raw);check(q->character==CHAR&&q->killer==PEER,"Kill receiver/killer truncated");
  const auto op=q->operation;std::int32_t name=0;
  if(q->name){const std::string text=q->name;name=text=="quest_died_10_times"?10:text=="quest_died_50_times"?50:text=="quest_died_100_times"?100:0;check(name,"wrong actual trophy string");}
  s.trace.push_back({std::int32_t(op),ident(q->subject),q->argument,q->index,name});++s.calls;s.probe();
  if(s.calls==s.fail){if(s.throw_service)throw std::runtime_error("explicit Kill provider throw");error="explicit Kill provider failure";return 1;}
  switch(op){
  case k::Operation::is_player:check(q->properties==&s.fixture.view&&q->subject==CHAR,"IsPlayer borrowed owner differs");r->word=s.player;break;
  case k::Operation::property_add_int:
   check(q->properties==&s.fixture.view&&q->index==25&&q->argument==1,"AddInt25 arguments changed");
   check(!dh2_property_add(q->properties,25,std::int32_t(std::uint32_t(q->argument)<<8)),"same live property AddInt failed");
   if(s.stale_after_add){s.fixture.properties.base[25]=0x33300;s.fixture.properties.saved[25]=0x44400;}
   if(s.after_add_failure){error="actual AddInt effect then failure";return 1;}break;
  case k::Operation::application_player_manager:r->identity=s.null_application?0:APP;r->player_manager=s.mutate_manager&&s.application_calls?FRESH_MANAGER:MANAGER;++s.application_calls;break;
  case k::Operation::is_local_player:check(q->subject==MANAGER,"locality receiver was not captured manager");if(s.missing_locality){error="actual CMatching locality owner unbound";return 1;}r->word=s.local;break;
  case k::Operation::trophy_manager:r->identity=s.null_trophy?0:s.current_trophy;break;
  case k::Operation::property_get_int:
   check(q->properties==&s.fixture.view&&q->index==25&&!q->argument,"GetInt25(false) arguments changed");
   if(s.get_calls<s.fresh_counts.size())s.fixture.properties.resolved[25]=std::int32_t(std::uint32_t(s.fresh_counts[s.get_calls])<<8);
   ++s.get_calls;r->word=signed_int(s.fixture.properties.resolved[25]);if(s.mutate_trophy)s.current_trophy=FRESH_TROPHY;break;
  case k::Operation::trophy_id:check(name,"lookup omitted exact trophy name");r->word=s.trophy_id;break;
  case k::Operation::trophy_unlock:check(q->subject==TROPHY,"captured trophy receiver reloaded after GetInt");check(q->argument==s.trophy_id,"negative/full-width trophy ID lost");break;
  case k::Operation::online:r->word=s.online;break;
  case k::Operation::get_local_player:check(q->subject==(s.mutate_manager?FRESH_MANAGER:MANAGER)&&!q->index&&q->argument==1,"fresh GetLocalPlayer(0,true) arguments changed");r->identity=0;break;
  case k::Operation::general_continuation:if(s.missing_general){error="actual general Kill branch unbound";return 1;}break;
  case k::Operation::is_dead:r->word=s.dead_calls++?s.inner_dead:s.outer_dead;break;
  case k::Operation::kill:{
   if(s.kill_failure){error="actual full Kill provider failed";return 1;}
   k::Request gate{ k::Operation::is_dead,CHAR,PEER,CHAR,nullptr,0,0,q->force,nullptr};k::Reply value{};
   if(invoke(raw,&gate,&value,error))return 1;
   if(value.word)break;
   s.dead=1;check(!dh2_property_set(&s.fixture.view,36,0),"full Kill HP prefix failed");
   return s.runtime.continue_after_hp0(PEER,q->force,&s.resumed,error)==k::Status::complete?0:1;
  }
  case k::Operation::event2:check(q->argument==2&&q->subject==CHAR,"Ctrl_Kill source event tail changed");if(s.raise_event()){error=s.event_error.empty()?"actual event2 failed":s.event_error;return 1;}break;
  }
  return 0;
 }
 void print()const{
  std::cout<<"{\"dead\":"<<dead<<",\"saved\":"<<fixture.properties.saved[25]<<",\"cached\":"<<fixture.properties.resolved[25]<<",\"hp\":"<<fixture.properties.resolved[36]<<",\"trace\":[";
  for(std::size_t i=0;i<trace.size();++i){if(i)std::cout<<',';std::cout<<'[';for(unsigned j=0;j<5;++j){if(j)std::cout<<',';std::cout<<trace[i][j];}std::cout<<']';}
  std::cout<<"],\"events\":[";for(std::size_t i=0;i<event_trace.size();++i){if(i)std::cout<<',';auto t=event_trace[i];std::cout<<'['<<t[0]<<','<<t[1]<<','<<t[2]<<','<<t[3]<<']';}std::cout<<"]}\n";
 }
};
int main(int argc,char** argv){try{
 if(argc>1&&std::string(argv[1])=="--oracle"){
  check(argc==16,"Kill oracle argument count changed");Catalogue cat(argv[2]);Fixture f(argv[2],argv[3],cat,"KnightPlayerBase");KillCore x(f);
  const auto mode=std::stoi(argv[4]);const auto count=std::stoi(argv[5]);x.local=std::stoi(argv[6])!=0;x.online=std::stoi(argv[7])!=0;x.trophy_id=std::stoi(argv[8]);
  x.fresh_counts={std::stoi(argv[9]),std::stoi(argv[10]),std::stoi(argv[11])};if(x.fresh_counts[0]==INT32_MIN)x.fresh_counts.clear();
  x.outer_dead=std::stoi(argv[12]);x.inner_dead=std::stoi(argv[13]);x.mutate_manager=std::stoi(argv[14])!=0;x.mutate_trophy=std::stoi(argv[15])!=0;
  check(!dh2_property_set_int(&f.view,25,count),"oracle count set failed");k::Result r{};std::string error;
  if(mode){x.dead=0;check(!dh2_property_set_int(&f.view,36,123),"oracle initial HP set failed");check(x.caller.kill(PEER,0,&r,error)==k::Status::complete,error.c_str());}
  else check(x.runtime.continue_after_hp0(PEER,0,&r,error)==k::Status::complete,error.c_str());
  x.print();return 0;
 }
 check(argc==3,"Kill gate needs cache and Debug output");Catalogue cat(argv[1]);unsigned cases=0,failures=0,guards=0,composition=0;
 auto fixture=[&](const std::string& name){return std::make_unique<Fixture>(argv[1],std::filesystem::path(argv[2])/name,cat,"KnightPlayerBase");};
 for(bool local:{false,true})for(bool online:{false,true})for(int count:{-2,8,9,10,49,50,99,100,101}){
  auto f=fixture("ordinary");KillCore x(*f);x.local=local;x.online=online;check(!dh2_property_set_int(&f->view,25,count),"real death count set failed");k::Result r{};std::string error;
  check(x.runtime.continue_after_hp0(PEER,0,&r,error)==k::Status::complete,"ordinary continuation failed");
  check(signed_int(f->properties.resolved[25])==count+1&&r.death_count_added&&r.local_player==local&&r.local_player_queried==online,"ordinary Player prefix/online branch differs");
  const bool trophy=local&&(count==9||count==49||count==99);check(r.trophy_calls==trophy&&r.property_reads==(local?(count==9?1:count==49?2:3):0),"exact threshold/fresh read count differs");
  auto before=r;error="sentinel";check(x.runtime.continue_after_hp0(PEER,0,&r,error)==k::Status::consumed&&error=="sentinel"&&!std::memcmp(&r,&before,sizeof(r)),"completed continuation replayed count");++cases;
 }
 {auto f=fixture("stale-sheets");KillCore x(*f);check(!dh2_property_set_int(&f->view,25,9),"stale count set failed");x.stale_after_add=true;k::Result r{};std::string error;
  check(x.runtime.continue_after_hp0(PEER,0,&r,error)==k::Status::complete&&r.property_reads==1&&r.last_death_count==10&&r.trophy_calls==1&&f->properties.resolved[25]==10*256&&f->properties.base[25]==0x33300&&f->properties.saved[25]==0x44400,"GetInt25(false) recalculated stale sheets");++cases;
 }
 {auto f=fixture("fresh-cached");KillCore x(*f);check(!dh2_property_set_int(&f->view,25,7),"count set failed");x.fresh_counts={49,50,100};x.mutate_trophy=true;x.mutate_manager=true;x.online=true;x.trophy_id=-1;
  f->properties.base[25]=0x33300;f->properties.saved[25]=0x44400;f->properties.resolved[25]=7*256;
  k::Result r{};std::string error;check(x.runtime.continue_after_hp0(PEER,0,&r,error)==k::Status::complete&&r.property_reads==2&&r.last_death_count==50&&r.trophy_index==-1&&r.trophy_calls==1,"cached/fresh threshold or captured trophy changed");
  check(x.trace.back()[1]==3&&f->properties.resolved[25]==50*256,"online Application manager was not freshly read");++cases;
 }
 auto f=fixture("baseline");KillCore baseline(*f);baseline.online=true;check(!dh2_property_set_int(&f->view,25,9),"baseline count set failed");k::Result all{};std::string error;check(baseline.runtime.continue_after_hp0(PEER,0,&all,error)==k::Status::complete,"baseline failed");
 for(bool thrown:{false,true})for(unsigned fail=1;fail<=baseline.calls;++fail){auto p=fixture("failed");KillCore x(*p);x.online=true;x.fail=fail;x.throw_service=thrown;check(!dh2_property_set_int(&p->view,25,9),"failed count set failed");k::Result r{};
  check(x.runtime.continue_after_hp0(PEER,0,&r,error)==k::Status::failed&&!error.empty()&&x.trace.size()==fail,"provider failure crossed prefix");
  check(x.dead==1&&p->properties.resolved[36]==0&&signed_int(p->properties.resolved[25])==(fail>2?10:9),"failure lost reached dead/HP/count effects");
  const auto calls=x.calls;auto before=r;error="sentinel";check(x.runtime.continue_after_hp0(PEER,0,&r,error)==k::Status::consumed&&x.calls==calls&&!std::memcmp(&r,&before,sizeof(r))&&error=="sentinel","failed continuation retried source");++failures;
 }
 for(unsigned kind=0;kind<5;++kind){auto p=fixture("required");KillCore x(*p);x.missing_locality=kind==0;x.null_application=kind==1;x.null_trophy=kind==2;x.after_add_failure=kind==3;x.player=kind!=4;x.missing_general=kind==4;k::Result r{};
  check(x.runtime.continue_after_hp0(PEER,0,&r,error)==k::Status::failed&&!error.empty(),"missing source provider silently succeeded");
  check(!r.online&&!r.local_player_queried&&x.dead==1&&p->properties.resolved[36]==0,"missing provider crossed online tail");++failures;
 }
 {auto p=fixture("guard");KillCore x(*p);x.reenter=true;k::Result r{};check(x.runtime.continue_after_hp0(PEER,0,&r,error)==k::Status::complete,"guard outer failed");++guards;}
 for(unsigned kind=0;kind<6;++kind){auto p=fixture("atomic");KillCore x(*p);k::Result r{};r.calls=99;auto before=r;error="sentinel";k::Status status;
  if(kind==0){x.dead=0;status=x.runtime.continue_after_hp0(PEER,0,&r,error);}
  else if(kind==1){p->properties.resolved[36]=256;status=x.runtime.continue_after_hp0(PEER,0,&r,error);}
  else if(kind==2)status=x.runtime.continue_after_hp0(PEER,0,reinterpret_cast<k::Result*>(&x.runtime),error);
  else if(kind==3)status=x.runtime.continue_after_hp0(PEER,0,reinterpret_cast<k::Result*>(p->properties.resolved.data()),error);
  else if(kind==4)status=x.runtime.continue_after_hp0(PEER,0,&r,*reinterpret_cast<std::string*>(&p->view));
  else status=x.runtime.continue_after_hp0(PEER,2,&r,error);
  check(status==k::Status::invalid_argument&&x.calls==0&&error=="sentinel"&&!std::memcmp(&r,&before,sizeof(r)),"invalid prefix/alias call changed outputs");++guards;
 }
 for(unsigned kind=0;kind<5;++kind){auto p=fixture("ctrl");KillCore x(*p);x.outer_dead=kind==0;x.inner_dead=kind==1;x.kill_failure=kind==2;x.event_failure=kind==3;x.missing_locality=kind==4;x.dead=0;check(!dh2_property_set_int(&p->view,36,123),"Ctrl prefix set failed");k::Result r{};
  const auto status=x.caller.kill(PEER,0,&r,error);
  if(kind<2)check(status==k::Status::complete&&(kind?r.kill_completed&&r.event2_completed:r.skipped&&!r.kill_completed)&&x.dead==0&&p->properties.resolved[36]==123*256,"fresh outer/inner IsDead source gate differs");
  else {check(status==k::Status::failed&&!r.event2_completed&&(kind!=3||r.kill_completed),"Ctrl failure crossed event tail");auto saved=r;auto calls=x.calls;error="sentinel";check(x.caller.kill(PEER,0,&r,error)==k::Status::consumed&&x.calls==calls&&!std::memcmp(&r,&saved,sizeof(r)),"Ctrl failed call retried");}
  if(kind==1)check(x.event_trace==std::vector<std::array<std::int32_t,4>>{{1,0x24,2,1},{0,0,2,1}},"outer event lost source inner-skip tail");
  ++cases;
 }
 auto load=[&](const char* name){return read(std::filesystem::path(argv[1])/name);};d::Dictionary clips;d::AnimationTables animations;
 auto dn=load("data/pydata/animations_dictionary_pyarraynames.bin"),dv=load("data/pydata/animations_dictionary_pyarray.bin"),ar=load("data/pydata/animations_pyarray.bin"),an=load("data/pydata/animations_pyarraynames.bin"),af=load("data/pydata/animations_pystructnames.bin");
 check(d::load_dictionary(bytes(dn),bytes(dv),clips,error)&&d::load_animation_tables(bytes(ar),bytes(an),bytes(af),clips,animations,error),error.c_str());
 for(const char* name:{"KnightPlayerBase","MagePlayerBase","RoguePlayerBase"}){
  Fixture player(argv[1],std::filesystem::path(argv[2])/name,cat,name);cleanup_prepare(player);auto* slot=player.session.get();c::Runtime cleanup(&slot,*player.owner,CHAR);
  Core core;Gold gold{};gold.in={0,1,1,0,0,2,20,0,5,3,0,0,0};core.reset(gold);core.fixture=&player;core.cleanup=&cleanup;core.tables=&animations;core.death_properties=player.view;core.constants=load("data/pydata/animations_pycst.bin");
  check(core.constants.size()<=UINT32_MAX&&
      !dh2_pycst_open(&core.constants_view,core.constants.data(),
                      std::uint32_t(core.constants.size())),
      "Player death constants rejected");core.bind_runtime();
  KillCore x(player);x.actual_death=&core;x.dead=0;check(!dh2_property_set_int(&player.view,36,123),"composition HP set failed");const auto vm=slot->vm();const auto loads=slot->statistics().load_calls;k::Result r{};
  check(x.caller.kill(PEER,0,&r,error)==k::Status::complete,error.c_str());
  check(r.kill_completed&&r.event2_completed&&x.resumed.death_count_added&&core.coordinator.state.current==12&&core.skills.completed+core.spells.completed==13,"actual event2 composition omitted source owners");
  check(x.event_trace==std::vector<std::array<std::int32_t,4>>{{1,0x24,2,1},{0,0,2,1}}&&x.event_result.service_calls==2,"actual dispatcher event2 order differs");
  check(slot==player.session.get()&&slot->vm()==vm&&slot->statistics().load_calls==loads&&core.ai.word_10==UINT32_MAX&&core.ai.word_14==UINT32_MAX&&core.coordinator.timers().slots[2].active,"event2 replaced VM/timer owners or blanket-stopped35");++composition;
 }
 std::cout<<"{\"validation\":\"PASS\",\"continuation_cases\":"<<cases<<",\"failure_prefix_cases\":"<<failures<<",\"guard_cases\":"<<guards<<",\"actual_event2_classes\":"<<composition<<",\"native_locality_bound\":false}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
