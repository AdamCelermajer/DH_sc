#include "../character_state_owner_behavior.hpp"
#include <array>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
static void check(bool x){if(!x)throw std::runtime_error("owner behavior composition mismatch");}
template<class T>T read(std::ifstream& f){T v{};f.read(reinterpret_cast<char*>(&v),sizeof(v));check(bool(f));return v;}
struct Case {std::uint32_t operation,a,b,count,result,mode;std::uint64_t payload;};
static_assert(sizeof(Case)==32);
static bool same(State a,State b){if(std::isnan(a.cached_speed)&&std::isnan(b.cached_speed))a.cached_speed=b.cached_speed=0;return !std::memcmp(&a,&b,sizeof a);}
static bool same(Request a,Request b){if(std::isnan(a.scalar)&&std::isnan(b.scalar))a.scalar=b.scalar=0;return !std::memcmp(&a,&b,sizeof a);}
struct Fixture {
 CharacterStateOwner* owner=nullptr;Facts* facts=nullptr;StateOwnerServices16* service=nullptr;
 std::vector<Request> calls;std::vector<StateOwnerRequest48> outer;
 unsigned mode=0;bool nested=false,route_dead_masks=false;
};
static void body(void* p,State* s,const Request* r){
 auto& f=*static_cast<Fixture*>(p);check(!r->reserved);f.calls.push_back(*r);
 if(r->service==stop){s->heading_active=0;if(f.mode==4)for(float& h:f.facts->heading)h=0;}
 if(r->service==set_animation){s->current_animation=r->argument[0];if(f.mode==2){s->body_present=0;f.owner->machine().physical=0;}}
 if(r->service==swap_animation&&f.mode==1)s->heading_active=0;
 if(r->service==remove_body){s->body_present=0;f.owner->machine().physical=0;}
 if(r->service==raise_event&&r->argument[0]==0x1d&&f.nested){f.nested=false;check(f.owner->event(0x22,0,*f.service)==1);}
 if(r->service==raise_event&&f.route_dead_masks&&r->argument[0]>=0x2a&&r->argument[0]<=0x2c)check(f.owner->event(r->argument[0],0,*f.service)==0);
}
static int remaining(void* p,StateOwnerMachine40* m,const StateOwnerRequest48* r,StateOwnerResponse8*){
 auto& f=*static_cast<Fixture*>(p);f.outer.push_back(*r);
 if(r->operation==state_owner_profile_begin||r->operation==state_owner_profile_end)return 0;
 Request q{};
 if(r->operation==state_owner_character_event){q.service=raise_event;q.argument[0]=static_cast<std::int32_t>(r->event);q.argument[1]=r->state;q.identity=r->payload;}
 else if(r->operation==state_owner_pin)q.service=pin;
 else return 1;
 body(p,m->fsm->state,&q);return 0;
}
static void stage(CharacterStateOwner& o,const State& input){o.state()=input;o.machine().current_index=input.current;o.native_fsm().current_present=input.current!=-1;o.machine().physical=input.body_present?0xb123456789abcdefull:0;}
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream file(argv[1],std::ios::binary);check(read<std::uint32_t>(file)==0x31545343);const auto count=read<std::uint32_t>(file);std::uint32_t transitions=0,events=0,requests=0,outer=0,guards=0;
 for(unsigned i=0;i<count;++i){auto item=read<Case>(file);auto initial=read<State>(file);auto facts=read<Facts>(file);auto expected=read<State>(file);std::vector<Request> gold(item.count);for(auto& r:gold)r=read<Request>(file);
  if(item.operation>1)continue;
  CharacterStateOwner owner(0xa123456789abcdefull);stage(owner,initial);Fixture f;f.owner=&owner;f.facts=&facts;f.mode=item.mode;Services body_service{&f,body};StateOwnerBehaviorPredicate8 predicates{};StateOwnerBehaviorContext40 context{&facts,&body_service,&predicates,{&f,remaining}};StateOwnerServices16 service{};check(dh2_character_state_owner_behavior_bind(&service,&context)==1);f.service=&service;
  auto result=!item.operation?owner.transition(static_cast<std::int32_t>(item.a),static_cast<std::int32_t>(item.b),item.payload,service):owner.event(static_cast<std::int32_t>(item.a),item.payload,service);
  if(result!=int(item.result)||!same(owner.state(),expected)||f.calls.size()!=gold.size()){std::fprintf(stderr,"original case %u\n",i);throw std::runtime_error("gold state/return/count");}
  for(unsigned j=0;j<gold.size();++j){check(same(f.calls[j],gold[j]));}requests+=item.count;outer+=f.outer.size();if(item.operation)++events;else ++transitions;
 }
 check(file.peek()==std::ifstream::traits_type::eof());
 CharacterStateOwner owner(0xa123456789abcdefull);Facts facts;facts.is_player=1;facts.attack_moving=248;facts.attack_static=243;facts.idle=215;facts.stance_mask=210;facts.stance=5;facts.attack_delay=400;
 Fixture f;f.owner=&owner;f.facts=&facts;Services body_service{&f,body};StateOwnerBehaviorPredicate8 predicates{};StateOwnerBehaviorContext40 context{&facts,&body_service,&predicates,{&f,remaining}};StateOwnerServices16 service{};check(dh2_character_state_owner_behavior_bind(&service,&context)==1);f.service=&service;
 auto reject=[&](StateOwnerBehaviorContext40 candidate){StateOwnerServices16 output{reinterpret_cast<void*>(0x123456789abcdef0ull),remaining};check(dh2_character_state_owner_behavior_bind(&output,&candidate)==-1);check(output.context==reinterpret_cast<void*>(0x123456789abcdef0ull)&&output.invoke==remaining);++guards;};
 auto bad=context;bad.facts=nullptr;reject(bad);bad=context;bad.bodies=nullptr;reject(bad);bad=context;bad.predicates=nullptr;reject(bad);bad=context;bad.remaining.invoke=nullptr;reject(bad);
 Facts wrong=facts;wrong.reserved=1;bad=context;bad.facts=&wrong;reject(bad);wrong=facts;wrong.is_player=2;reject(bad);StateOwnerBehaviorPredicate8 invalid=predicates;invalid.reserved[0]=1;bad=context;bad.predicates=&invalid;reject(bad);
 check(dh2_character_state_owner_behavior_bind(nullptr,&context)==-1);++guards;
 const auto original_context=context;check(dh2_character_state_owner_behavior_bind(reinterpret_cast<StateOwnerServices16*>(&context),&context)==-1);check(context.facts==original_context.facts&&context.bodies==original_context.bodies);++guards;
 check(dh2_character_state_owner_behavior_bind(reinterpret_cast<StateOwnerServices16*>(&facts),&context)==-1);check(facts.idle==215);++guards;
 State initial;initial.current=4;initial.body_present=1;initial.heading_active=1;stage(owner,initial);f.nested=true;check(owner.transition(5,0xc354,0,service)==1);check(owner.state().current==3&&owner.state().flags==0x2380&&owner.state().current_animation==220);
 const std::array<unsigned,11> nested{stop,pin,set_animation,unpin,set_speed,cancel_sneaking,raise_event,start_timer,pin,set_animation,raise_event};check(f.calls.size()==nested.size());for(unsigned j=0;j<nested.size();++j)check(f.calls[j].service==nested[j]);
 check(f.calls[2].argument[0]==253&&f.calls[7].argument[2]==0x2a);
 // Dead focus emits mask events; synchronous source routing clears owner masks,
 // while its remaining genuine cleanup services are explicit fixture effects.
 f.calls.clear();f.outer.clear();f.route_dead_masks=true;initial=State{};initial.current=3;initial.attack_gate=7;initial.animation_override=259;initial.body_present=1;stage(owner,initial);check(owner.transition(12,0xc358,0,service)==1&&owner.state().attack_gate==0);f.route_dead_masks=false;
 // Required unsupported family preserves only the already delivered prefix.
 f.calls.clear();f.outer.clear();initial=State{};initial.current=3;stage(owner,initial);check(owner.transition(17,-1,0,service)==-2);check(owner.state().current==17&&f.outer.back().operation==state_owner_focus&&f.outer.back().source_function==0x3c688c);++guards;
 // Spawn predicate stays a required service, never a synthesized true result.
 StateOwnerRequest48 request{state_owner_predicate,0x3ad2e4,0,1,0x2f,0,owner.native_fsm().character,0,{0,0}};StateOwnerResponse8 response{1,99};check(dh2_character_state_owner_behavior_invoke(&context,&owner.machine(),&request,&response)!=0&&response.accepted==99);++guards;
 initial=State{};initial.current=3;stage(owner,initial);f.calls.clear();request={state_owner_focus,0xdead,3,-1,0xffffffff,0,owner.native_fsm().character,0,{0,0}};check(dh2_character_state_owner_behavior_invoke(&context,&owner.machine(),&request,&response)!=0&&f.calls.empty());++guards;
 // Native predicates use live owner's mask, not a stale caller snapshot.
 owner.state().attack_gate=1;request={state_owner_predicate,0x3ad22c,3,5,0xc354,0,owner.native_fsm().character,0,{0,0}};check(dh2_character_state_owner_behavior_invoke(&context,&owner.machine(),&request,&response)==0&&!response.accepted);owner.state().attack_gate=0;check(dh2_character_state_owner_behavior_invoke(&context,&owner.machine(),&request,&response)==0&&response.accepted);++guards;
 request.character=0;const auto previous_response=response;check(dh2_character_state_owner_behavior_invoke(&context,&owner.machine(),&request,&response)!=0&&response.next==previous_response.next&&response.accepted==previous_response.accepted);++guards;
 std::printf("{\"validation\":\"PASS\",\"original_transition_cases\":%u,\"original_event_cases\":%u,\"original_ordered_body_requests\":%u,\"required_remaining_fixture_deliveries\":%u,\"guard_and_boundary_checks\":%u,\"nested_notification_completion_checks\":1,\"nested_dead_gate_routing_checks\":1,\"mismatches\":0}\n",transitions,events,requests,outer,guards);return 0;
 }catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}}

