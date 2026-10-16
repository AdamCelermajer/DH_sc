#include "character_state.hpp"
#include "../character_sm_set_dead_state_v1.hpp"
#include "../game-data/properties.hpp"
#include <array>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace {
void require(bool condition,const char* message){if(!condition)throw std::runtime_error(message);}
namespace dead_state=dh2::character_sm_set_dead_state_v1;
namespace animation_data=dh2::data;

void append_word(std::vector<std::uint8_t>& bytes,std::uint32_t value){
 for(unsigned i=0;i<4;++i)bytes.push_back(std::uint8_t(value>>(i*8)));
}
void append_name(std::vector<std::uint8_t>& bytes,const char* value){
 const auto size=std::strlen(value);append_word(bytes,std::uint32_t(size));
 bytes.insert(bytes.end(),value,value+size);
}
void append_group(std::vector<std::uint8_t>& bytes,const char* group,
                  const char* key,std::int32_t value){
 append_name(bytes,group);append_word(bytes,1);append_name(bytes,key);
 append_word(bytes,std::uint32_t(value));
}
struct DeadStateFixture {
 Coordinator coordinator{0x4d0000001ull,1};
 Facts facts{};CombatFacts combat{};std::vector<std::uint32_t> services;
 std::uintptr_t look_payload=0;std::uint32_t stance_calls=0;
 bool mutate_great_after_death_stance=false;
 static Facts read_facts(void* raw){return static_cast<DeadStateFixture*>(raw)->facts;}
 static CombatFacts read_combat(void* raw){return static_cast<DeadStateFixture*>(raw)->combat;}
 static void state_service(void* raw,State* state,const Request* request){
  auto& self=*static_cast<DeadStateFixture*>(raw);self.services.push_back(request->service);
  if(request->service==set_animation)state->current_animation=request->argument[0];
  if(request->service==look_at)self.look_payload=std::uintptr_t(request->identity);
 }
 static int anim_stance(void* raw,std::uintptr_t character,std::int32_t* stance){
  auto& self=*static_cast<DeadStateFixture*>(raw);
  if(character!=self.coordinator.owner()||!stance)return 1;
  ++self.stance_calls;*stance=2;
  if(self.mutate_great_after_death_stance&&self.stance_calls==1)
   self.coordinator.state.dead_alternate=0;
  return 0;
 }
 DeadStateFixture(){
  CoordinatorBindings bindings{};bindings.context=this;bindings.facts=read_facts;
  bindings.services={this,state_service};bindings.combat_facts=read_combat;
  coordinator.bind(bindings);
 }
};

void dead_state_helper_audit(){
 std::array<std::int32_t,224> defaults{},types{},base{},saved{},gear{},resolved{};
 types.fill(-1);resolved[2]=0;
 animation_data::PropertyView properties{defaults.data(),types.data(),base.data(),
  saved.data(),gear.data(),resolved.data(),nullptr,0};
 animation_data::AnimationTables tables;tables.characters.resize(1);
 auto& row=tables.characters[0];row.fields[3]={300};row.fields[4]={400};
 row.fields[5]={500};row.fields[6]={200};
 std::vector<std::uint8_t> constants_bytes;append_word(constants_bytes,2);
 append_group(constants_bytes,"AnimStancedAnim","SL__LIST_IPHONE",0x78000);
 append_group(constants_bytes,"AnimStances","COUNT_IPHONE",6);
 dh2_pycst_view constants{};require(!dh2_pycst_open(&constants,constants_bytes.data(),
  std::uint32_t(constants_bytes.size())),"SM_SetDeadState constants fixture");

 DeadStateFixture direct;direct.coordinator.state.current=10;
 direct.coordinator.state.dead_alternate=1;
 direct.mutate_great_after_death_stance=true;
 dead_state::Bindings bindings{};bindings.coordinator=&direct.coordinator;
 bindings.character={direct.coordinator.owner(),&properties};bindings.tables=&tables;
 bindings.constants=&constants;bindings.anim_stance={&direct,DeadStateFixture::anim_stance};
 dead_state::Request request{1,0x4d1000001ull,1};dead_state::Result result{};
 std::string error;
 require(dead_state::execute(bindings,request,&result,error)==dead_state::Status::complete,
  "SM_SetDeadState direct transition failed");
 require(result.table_valid&&result.death_animation==302&&
  result.despawn_animation==402&&direct.stance_calls==2&&
  direct.coordinator.state.dead_alternate==0&&
  direct.coordinator.source_dead_byte_3e()==1&&
  direct.coordinator.source_despawn_animation_38()==402&&
  direct.coordinator.state.current==12&&
  direct.coordinator.state.current_animation==302&&
  direct.coordinator.state.animation_override==-1&&
  direct.look_payload==request.killer,
  "SM_SetDeadState lost exact stance reread/field/transition order");

 DeadStateFixture raised;raised.coordinator.state.current=10;
 raised.coordinator.state.dead_alternate=0;
 bindings.coordinator=&raised.coordinator;
 bindings.character.identity=raised.coordinator.owner();
 bindings.anim_stance.context=&raised;
 request={0,0x4d2000001ull,0};result={};
 require(dead_state::execute(bindings,request,&result,error)==dead_state::Status::complete&&
  result.state_transitioned&&raised.coordinator.state.current==12&&
  raised.coordinator.source_dead_byte_3e()==0&&
  raised.coordinator.state.current_animation==202&&
  raised.look_payload==request.killer,
  "SM_SetDeadState RaiseStateEvent branch failed");

 DeadStateFixture invalid;invalid.coordinator.state.current=10;
 invalid.coordinator.state.dead_alternate=1;resolved[2]=7;
 bindings.coordinator=&invalid.coordinator;
 bindings.character.identity=invalid.coordinator.owner();
 bindings.anim_stance.context=&invalid;
 request={1,0,1};result={};
 require(dead_state::execute(bindings,request,&result,error)==dead_state::Status::complete&&
  !result.table_valid&&invalid.coordinator.state.current==10&&
  invalid.coordinator.state.dead_alternate==1&&
  invalid.coordinator.source_dead_byte_3e()==0&&
  invalid.coordinator.source_despawn_animation_38()==0,
  "SM_SetDeadState invalid table must return before mutation");
}
struct Case {std::uint32_t operation,a,b,count,result,callback_mode;std::uint64_t payload;};
static_assert(sizeof(Case)==32);
template<class T>T read(std::ifstream& input){T value{};input.read(reinterpret_cast<char*>(&value),sizeof value);require(bool(input),"truncated reference");return value;}
bool state_equal(const State& expected,const State& actual){
 State left=expected,right=actual;
 if(std::isnan(left.cached_speed)&&std::isnan(right.cached_speed))left.cached_speed=right.cached_speed=0;
 return std::memcmp(&left,&right,sizeof left)==0;
}
bool request_equal(const Request& expected,const Request& actual){
 Request left=expected,right=actual;
 if(std::isnan(left.scalar)&&std::isnan(right.scalar))left.scalar=right.scalar=0;
 return std::memcmp(&left,&right,sizeof left)==0;
}
struct Fixture {
 std::vector<Request> calls;
 std::uint32_t mode=0;
 Facts* facts=nullptr;
 bool complete_on_notification=false;
 std::uint32_t original_flags=0,debug_preludes=0;
 bool fail_debug_prelude=false;
};
void callback(void* context,State* state,const Request* request){
 require(reinterpret_cast<std::uintptr_t>(context)>0xffffffffu,"64-bit callback context");
 auto& fixture=*static_cast<Fixture*>(context);require(request->reserved==0,"request reserved");
 if(request->service==dead_focus_prelude){
  require(state->current==12&&state->flags==fixture.original_flags,"dead Debug prelude before flags");
  ++fixture.debug_preludes;
  if(fixture.fail_debug_prelude)throw std::runtime_error("injected mandatory Debug failure");
  // The frozen historical oracle explicitly fixtures Debug callees, so this
  // added mandatory integration request is counted separately from its trace.
  // The dead-focus adapter audit executes the actual selected Debug owner.
  return;
 }
 fixture.calls.push_back(*request);
 if(request->service==stop){state->heading_active=0;if(fixture.mode==4)for(float& heading:fixture.facts->heading)heading=0;}
 if(request->service==set_animation&&request->argument[0]>=0){state->current_animation=request->argument[0];if(fixture.mode==2)state->body_present=0;}
 if(request->service==swap_animation&&fixture.mode==1)state->heading_active=0;
 if(request->service==remove_body)state->body_present=0;
 if(request->service==raise_event&&request->argument[0]==0x1d&&fixture.complete_on_notification){
  fixture.complete_on_notification=false;Services services{&fixture,callback};
  require(dh2_character_state_event(state,fixture.facts,0x22,0,&services)==1,"synchronous state reentry");
 }
}
}
int main(int argc,char** argv){
 try{
  dead_state_helper_audit();
  require(argc==2,"usage: character_state REFERENCE");std::ifstream input(argv[1],std::ios::binary);require(bool(input),"open reference");
  auto magic=read<std::uint32_t>(input),count=read<std::uint32_t>(input);require(magic==0x31545343,"reference format");
  Fixture fixture;Services services{&fixture,callback};std::uint64_t request_count=0;
  for(std::uint32_t index=0;index<count;++index){
   Case item=read<Case>(input);State state=read<State>(input);Facts facts=read<Facts>(input);State expected=read<State>(input);
   fixture.calls.clear();fixture.mode=item.callback_mode;fixture.facts=&facts;fixture.original_flags=state.flags;int result=0;
   if(item.operation==0)result=dh2_character_state_transition(&state,&facts,static_cast<std::int32_t>(item.a),static_cast<std::int32_t>(item.b),item.payload,&services);
   else if(item.operation==1)result=dh2_character_state_event(&state,&facts,item.a,item.payload,&services);
   else if(item.operation==2)result=dh2_character_state_update(&state,&facts,item.a,&services);
   else if(item.operation==3){std::array<std::int32_t,224> properties{};properties[48]=state.current;result=dh2_character_attack_speed(&state.cached_speed,properties.data());}
   else if(item.operation==4)result=dh2_character_state_is_idle(state.current,item.a);
   else throw std::runtime_error("reference operation");
   if(result!=static_cast<int>(item.result)||!state_equal(expected,state)||fixture.calls.size()!=item.count){std::fprintf(stderr,"case %u operation %u\n",index,item.operation);throw std::runtime_error("original state/return mismatch");}
   for(std::uint32_t n=0;n<item.count;++n)require(request_equal(read<Request>(input),fixture.calls[n]),"original ordered service mismatch");
   request_count+=item.count;
  }
  require(input.peek()==std::ifstream::traits_type::eof(),"trailing reference bytes");
  // Character::Interact reaches the active StateMachine through event 50003.
  // Exercise the same Coordinator kernel path used by NPC owners and keep the
  // source CSInteract focus prefix observable through its existing services.
  State interacting;interacting.current=3;interacting.body_present=1;
  Facts interaction_facts;interaction_facts.is_player=0;
  fixture.calls.clear();fixture.facts=&interaction_facts;
  require(dh2_character_state_event(&interacting,&interaction_facts,50003,
      0x12345678,&services)==1,"Character::Interact registered FSM event");
  require(interacting.current==13&&interacting.flags==25537&&
      interacting.current_animation==-1,"CSInteract source focus state");
  require(fixture.calls.size()==3&&fixture.calls[0].service==set_animation&&
      fixture.calls[0].argument[0]==-1&&fixture.calls[1].service==cancel_sneaking&&
      fixture.calls[2].service==raise_event&&fixture.calls[2].argument[0]==0x1d,
      "CSInteract focus source service order");
  unsigned malformed=0;
  State base;base.current=3;Facts facts;fixture.mode=0;fixture.facts=&facts;
  auto reject=[&](State state,Facts value,const Services* callbacks){fixture.calls.clear();State before=state;require(dh2_character_state_event(&state,&value,0xc351,0,callbacks)==-1,"malformed input accepted");require(state_equal(state,before)&&fixture.calls.empty(),"malformed input mutated state");++malformed;};
  Services missing{&fixture,nullptr};reject(base,facts,nullptr);reject(base,facts,&missing);
  for(int field=0;field<5;++field){Facts value=facts;if(field==0)value.reserved=1;if(field==1)value.is_player=2;if(field==2)value.is_at_destination=2;if(field==3)value.following_path=2;if(field==4)value.has_ranged_weapon=2;reject(base,value,&services);}
  for(int field=0;field<8;++field){State state=base;if(field==0)state.current=7;if(field==1)state.body_present=2;if(field==2)state.move_type=3;if(field==3)state.idle_suppressed=256;if(field==4)state.stop_attack_allowed=256;if(field==5)state.heading_active=256;if(field==6)state.controller_locked=256;if(field==7)state.dead_alternate=256;reject(state,facts,&services);}
  require(dh2_character_state_event(nullptr,&facts,0x22,0,&services)==-1,"null state");++malformed;
  require(dh2_character_state_update(&base,nullptr,16,&services)==-1,"null facts");++malformed;
  State before=base;require(dh2_character_state_transition(&base,&facts,18,0,0,&services)==-1&&state_equal(base,before),"unsupported transition");++malformed;
  float speed=17;std::array<std::int32_t,224> properties{};require(dh2_character_attack_speed(&speed,nullptr)==-1&&speed==17,"null properties");++malformed;
  require(dh2_character_attack_speed(nullptr,properties.data())==-1,"null output");++malformed;
  require(dh2_character_state_is_idle(3,2)==-1,"malformed idle mode");++malformed;
  // The borrowed callback can synchronously dispatch completion during outgoing
  // transition notification, as source Character->AI->FSM dispatch permits.
  State reentrant;reentrant.current=4;reentrant.body_present=1;reentrant.heading_active=1;
  facts.is_player=1;facts.attack_moving=248;facts.attack_static=243;facts.idle=215;facts.stance_mask=210;facts.stance=5;facts.attack_delay=400;
  fixture.calls.clear();fixture.complete_on_notification=true;
  require(dh2_character_state_transition(&reentrant,&facts,5,0xc354,0,&services)==1,"move attack transition");
  require(reentrant.current==3&&reentrant.flags==0x2380&&reentrant.current_animation==220,"synchronous completion idle");
  const std::array<unsigned,12> order{stop,pin,set_animation,unpin,set_speed,cancel_sneaking,raise_event,start_timer,pin,set_animation,raise_event,0};
  require(fixture.calls.size()==11,"reentry service count");for(unsigned n=0;n<11;++n)require(fixture.calls[n].service==order[n],"reentry service order");
  require(fixture.calls[2].argument[0]==253,"AttackMoving source Attack+stance selection");
  require(fixture.calls[7].argument[0]==400&&fixture.calls[7].argument[2]==0x2a,"attack blur delay event");
  State despawn;despawn.current=12;despawn.flags=0x40;despawn.current_animation=214;
  Facts monster_facts;monster_facts.is_player=0;monster_facts.despawn_delay=300;
  fixture.calls.clear();fixture.facts=&monster_facts;
  require(dh2_character_state_event(&despawn,&monster_facts,0x2e,0,&services)==1,
          "Dead despawn-timer transition");
  require(despawn.current==2&&despawn.flags==512&&despawn.current_animation==214&&
          despawn.controller_locked==0,"CSDespawn entry state");
  require(fixture.calls.size()==4&&fixture.calls[0].service==store_previous_flags&&
          fixture.calls[0].argument[0]==0x40&&fixture.calls[1].service==set_animation&&
          fixture.calls[1].argument[0]==-1&&fixture.calls[2].service==cancel_sneaking&&
          fixture.calls[3].service==raise_event&&fixture.calls[3].argument[0]==0x1d&&
          fixture.calls[3].argument[1]==12,"CSDespawn source entry order");
  // Combat handlers share this State/service owner. The added CombatFacts are
  // transient projections; transitions still use the canonical state and
  // preserve the source ordered Focus/Blur services.
  State combat_state;combat_state.current=3;combat_state.body_present=1;
  Facts combat_character_facts;combat_character_facts.idle=12;
  CombatFacts combat_facts;combat_facts.scared_animation=107;
  combat_facts.stunned_animation=205;combat_facts.controller_present=1;
  combat_facts.machine_flags_44=0;
  fixture.calls.clear();fixture.facts=&combat_character_facts;
  require(dh2_character_combat_state_transition(&combat_state,
      &combat_character_facts,&combat_facts,8,50013,0x12345678,&services)==1&&
      combat_state.current==8&&combat_state.flags==8768&&
      combat_state.current_animation==107,"CSScared Focus entry");
  require(fixture.calls.size()==5&&fixture.calls[0].service==set_animation&&
      fixture.calls[0].argument[0]==107&&fixture.calls[1].service==scare_focus&&
      fixture.calls[1].identity==0x12345678&&
      fixture.calls[2].service==cancel_sneaking&&fixture.calls[3].service==unpin&&
      fixture.calls[4].service==raise_event&&fixture.calls[4].argument[0]==0x1d,
      "CSScared Focus service order");
  fixture.calls.clear();
  require(dh2_character_combat_state_event(&combat_state,
      &combat_character_facts,&combat_facts,50012,0x23456789,&services)==1&&
      combat_state.current==9&&combat_state.flags==8706&&
      combat_state.current_animation==205&&combat_state.controller_locked==1,
      "CSScared to CSStunned registered transition");
  require(fixture.calls.size()==7&&fixture.calls[0].service==scare_blur&&
      fixture.calls[1].service==pin&&fixture.calls[2].service==set_animation&&
      fixture.calls[2].argument[0]==205&&fixture.calls[3].service==controller_lock&&
      fixture.calls[3].argument[0]==1&&fixture.calls[4].service==cancel_sneaking&&
      fixture.calls[5].service==unpin&&fixture.calls[6].service==raise_event&&
      fixture.calls[6].argument[0]==0x1d,"CSStunned transition service order");
  fixture.calls.clear();
  require(dh2_character_combat_state_update(&combat_state,
      &combat_character_facts,&combat_facts,16,&services)==1&&
      combat_state.current==3&&combat_state.flags==0x2380,
      "CSStunned Update returns to same Coordinator Idle state");
  require(fixture.calls.size()==4&&fixture.calls[0].service==controller_lock&&
      fixture.calls[0].argument[0]==0&&fixture.calls[1].service==pin&&
      fixture.calls[2].service==set_animation&&fixture.calls[2].argument[0]==12&&
      fixture.calls[3].service==raise_event&&fixture.calls[3].argument[0]==0x1d,
      "CSStunned Update transition service order");
  fixture.calls.clear();
  require(dh2_character_combat_state_event(&combat_state,
      &combat_character_facts,&combat_facts,50011,0x3456789a,&services)==1&&
      combat_state.current==10&&combat_state.flags==9025,
      "Idle registered transition to CSKnockedBack");
  require(fixture.calls.size()==7&&fixture.calls[0].service==set_animation&&
      fixture.calls[0].argument[0]==-1&&fixture.calls[1].service==combat_machine_flags&&
      fixture.calls[1].argument[0]==0x20&&fixture.calls[2].service==controller_lock&&
      fixture.calls[2].argument[0]==1&&fixture.calls[3].service==look_at&&
      fixture.calls[3].identity==0x3456789a&&fixture.calls[4].service==cancel_sneaking&&
      fixture.calls[5].service==unpin&&fixture.calls[6].service==raise_event,
      "CSKnockedBack Focus service order");
  State injured;injured.current=11;
  CombatFacts unknown_injured_facts=combat_facts;
  unknown_injured_facts.last_target_available=0;
  fixture.calls.clear();
  require(dh2_character_combat_state_transition(&injured,
      &combat_character_facts,&unknown_injured_facts,3,34,0,&services)==-1&&
      injured.current==11&&fixture.calls.empty(),
      "CSInjured Blur must fail closed without Character+0x408 projection");
  unknown_injured_facts.last_target_available=1;
  unknown_injured_facts.last_target=0; // observed null remains a real source value
  require(dh2_character_combat_state_transition(&injured,
      &combat_character_facts,&unknown_injured_facts,3,34,0,&services)==1&&
      injured.current==3&&fixture.calls.size()==3&&
      fixture.calls[0].service==look_at&&fixture.calls[0].argument[0]==1&&
      fixture.calls[0].identity==0&&fixture.calls[1].service==set_animation&&
      fixture.calls[2].service==raise_event,
      "CSInjured Blur must preserve a known null Character+0x408 focus");
  // IDA CSKnockedBack::OnFocus 0x3c4a48: the great-knock bit (0x10) replaces the whole
  // Character+0x528 machine word with 0x20; a plain knock clears only bit 0x20.
  {State great;great.current=3;great.body_present=1;CombatFacts great_facts=combat_facts;great_facts.machine_flags_44=0x18;
   fixture.calls.clear();
   require(dh2_character_combat_state_transition(&great,&combat_character_facts,&great_facts,10,50011,0x11,&services)==1&&great.current==10,"great knock entry");
   require(fixture.calls.size()==8&&fixture.calls[0].service==set_animation&&fixture.calls[1].service==set_death_filter&&
           fixture.calls[2].service==combat_machine_flags&&fixture.calls[2].argument[0]==-1&&fixture.calls[2].argument[1]==0x20&&
           fixture.calls[3].service==controller_lock,"great knock replaces the machine word");}
  // IDA CSScared::OnUpdate 0x3c4788 tests machine bit 4 and CSStunned::OnUpdate 0x3c549c bit 2.
  {State scared;scared.current=8;CombatFacts timed=combat_facts;
   timed.machine_flags_44=0x4;fixture.calls.clear();
   require(dh2_character_combat_state_update(&scared,&combat_character_facts,&timed,16,&services)==1&&fixture.calls.empty(),"CSScared keeps its loop while bit 4 is set");
   timed.machine_flags_44=0x12;fixture.calls.clear();
   require(dh2_character_combat_state_update(&scared,&combat_character_facts,&timed,16,&services)==1&&fixture.calls.size()==1&&
           fixture.calls[0].service==stop_loop,"CSScared stops looping once bit 4 clears");
   State stunned;stunned.current=9;timed.machine_flags_44=0x2;fixture.calls.clear();
   require(dh2_character_combat_state_update(&stunned,&combat_character_facts,&timed,16,&services)==1&&stunned.current==9&&fixture.calls.empty(),"CSStunned stays while bit 2 is set");
   timed.machine_flags_44=0x4;
   require(dh2_character_combat_state_update(&stunned,&combat_character_facts,&timed,16,&services)==1&&stunned.current==3,"CSStunned returns to Idle once bit 2 clears");}
  // IDA: CharAI::RaiseAIEvent(event2) calls CharAI::OnDied/AI_SetDead before
  // forwarding to the machine. The owner is already Dead(12); CSDead has no
  // event2 registration or OnEvent2 action, so this is an exact no-op.
  State dead_event2;dead_event2.current=12;dead_event2.flags=0x241;
  dead_event2.body_present=1;dead_event2.current_animation=0x1234;
  fixture.calls.clear();fixture.facts=&monster_facts;
  require(dh2_character_state_event(&dead_event2,&monster_facts,2,
      0x123456789abcdef0ull,&services)==0,"Monster dead event2 must remain unregistered");
  require(dead_event2.current==12&&dead_event2.flags==0x241&&
      dead_event2.body_present==1&&dead_event2.current_animation==0x1234&&
      fixture.calls.empty(),"Monster dead event2 unexpectedly changed FSM state");
  State illegal;illegal.current=12;before=illegal;
  require(dh2_character_state_transition(&illegal,&monster_facts,2,0,0,&services)==-1&&
          state_equal(illegal,before),"CSDespawn cannot be entered outside event 46");
  State dead;dead.current=3;dead.flags=0x2380;dead.elapsed_ms=41;
  fixture.original_flags=dead.flags;fixture.fail_debug_prelude=true;fixture.calls.clear();
  bool threw=false;try{dh2_character_state_transition(&dead,&facts,12,0xc358,0,&services);}catch(const std::runtime_error&){threw=true;}
  require(threw&&dead.current==12&&dead.elapsed_ms==0&&dead.flags==0x2380&&!dead.controller_locked&&fixture.calls.empty(),"Debug failure preserves reached transition prefix");
  std::printf("{\"original_reference_cases\":%u,\"ordered_service_requests\":%llu,\"mandatory_dead_debug_preludes\":%u,\"debug_failure_prefix_passed\":true,\"despawn_entry_passed\":true,\"malformed_no_mutation_cases\":%u,\"synchronous_reentry_passed\":true,\"mismatches\":0}\n",count,static_cast<unsigned long long>(request_count),fixture.debug_preludes,malformed);
  return 0;
 }catch(const std::exception& failure){std::fprintf(stderr,"character_state audit: %s\n",failure.what());return 1;}
}
