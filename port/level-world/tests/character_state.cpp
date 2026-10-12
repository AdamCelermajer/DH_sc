#include "character_state.hpp"
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
};
void callback(void* context,State* state,const Request* request){
 require(reinterpret_cast<std::uintptr_t>(context)>0xffffffffu,"64-bit callback context");
 auto& fixture=*static_cast<Fixture*>(context);require(request->reserved==0,"request reserved");fixture.calls.push_back(*request);
 if(request->service==stop){state->heading_active=0;if(fixture.mode==4)for(float& heading:fixture.facts->heading)heading=0;}
 if(request->service==set_animation){state->current_animation=request->argument[0];if(fixture.mode==2)state->body_present=0;}
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
  require(argc==2,"usage: character_state REFERENCE");std::ifstream input(argv[1],std::ios::binary);require(bool(input),"open reference");
  auto magic=read<std::uint32_t>(input),count=read<std::uint32_t>(input);require(magic==0x31545343,"reference format");
  Fixture fixture;Services services{&fixture,callback};std::uint64_t request_count=0;unsigned isolated_bodies=0,isolated_events=0;
  for(std::uint32_t index=0;index<count;++index){
   Case item=read<Case>(input);State state=read<State>(input);Facts facts=read<Facts>(input);State expected=read<State>(input);
   const State initial=state;const Facts initial_facts=facts;
   fixture.calls.clear();fixture.mode=item.callback_mode;fixture.facts=&facts;int result=0;
   if(item.operation==0)result=dh2_character_state_transition(&state,&facts,static_cast<std::int32_t>(item.a),static_cast<std::int32_t>(item.b),item.payload,&services);
   else if(item.operation==1)result=dh2_character_state_event(&state,&facts,item.a,item.payload,&services);
   else if(item.operation==2)result=dh2_character_state_update(&state,&facts,item.a,&services);
   else if(item.operation==3){std::array<std::int32_t,224> properties{};properties[48]=state.current;result=dh2_character_attack_speed(&state.cached_speed,properties.data());}
   else if(item.operation==4)result=dh2_character_state_is_idle(state.current,item.a);
   else throw std::runtime_error("reference operation");
   if(result!=static_cast<int>(item.result)||!state_equal(expected,state)||fixture.calls.size()!=item.count){std::fprintf(stderr,"case %u operation %u\n",index,item.operation);throw std::runtime_error("original state/return mismatch");}
   for(std::uint32_t n=0;n<item.count;++n)require(request_equal(read<Request>(input),fixture.calls[n]),"original ordered service mismatch");
   if(item.operation==0){
    State composed=initial;Facts composed_facts=initial_facts;Fixture bodies;bodies.mode=item.callback_mode;bodies.facts=&composed_facts;Services body_services{&bodies,callback};const auto previous=composed.current;
    if(previous!=-1)require(dh2_character_state_blur_body(&composed,&composed_facts,&body_services)==1,"isolated Blur body");
    composed.current=static_cast<std::int32_t>(item.a);if(previous!=composed.current)composed.elapsed_ms=0;
    require(dh2_character_state_focus_body(&composed,&composed_facts,previous,item.payload,&body_services)==1,"isolated Focus body");
    const Request notification{raise_event,{0x1d,previous,0},0,0,std::uint64_t(std::int64_t(previous))};callback(&bodies,&composed,&notification);
    require(state_equal(expected,composed)&&bodies.calls.size()==fixture.calls.size(),"isolated bodies original result mismatch");
    for(unsigned n=0;n<bodies.calls.size();++n)require(request_equal(fixture.calls[n],bodies.calls[n]),"isolated bodies original ordered mismatch");++isolated_bodies;
   }
   if(item.operation==1&&item.result==0&&initial.current!=-1){
    State composed=initial;Facts composed_facts=initial_facts;Fixture bodies;bodies.mode=item.callback_mode;bodies.facts=&composed_facts;Services body_services{&bodies,callback};
    if(item.a>=0x2a&&item.a<=0x2c)composed.attack_gate&=~(1u<<(item.a-0x2a));
    require(dh2_character_state_event_body(&composed,&composed_facts,item.a,&body_services)==1,"isolated OnEvent body");
    require(state_equal(expected,composed)&&bodies.calls.size()==fixture.calls.size(),"isolated event original result mismatch");
    for(unsigned n=0;n<bodies.calls.size();++n)require(request_equal(fixture.calls[n],bodies.calls[n]),"isolated event original ordered mismatch");++isolated_events;
   }
   request_count+=item.count;
  }
  require(input.peek()==std::ifstream::traits_type::eof(),"trailing reference bytes");
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
  std::printf("{\"validation\":\"PASS\",\"original_reference_cases\":%u,\"ordered_service_requests\":%llu,\"malformed_no_mutation_cases\":%u,\"isolated_Focus_Blur_original_cases\":%u,\"isolated_OnEvent_original_cases\":%u,\"synchronous_reentry_passed\":true,\"mismatches\":0}\n",count,static_cast<unsigned long long>(request_count),malformed,isolated_bodies,isolated_events);
  return 0;
 }catch(const std::exception& failure){std::fprintf(stderr,"character_state audit: %s\n",failure.what());return 1;}
}
