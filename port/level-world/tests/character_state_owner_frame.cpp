#include "../character_state_owner_frame.hpp"
#include "../character_state_owner_behavior.hpp"
#include <cmath>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
static void check(bool x){if(!x)throw std::runtime_error("state owner frame mismatch");}
template<class T>T read(std::ifstream& f){T v{};f.read(reinterpret_cast<char*>(&v),sizeof v);check(bool(f));return v;}
static bool same(State a,State b){if(std::isnan(a.cached_speed)&&std::isnan(b.cached_speed))a.cached_speed=b.cached_speed=0;return !std::memcmp(&a,&b,sizeof a);}
static bool same(Request a,Request b){if(std::isnan(a.scalar)&&std::isnan(b.scalar))a.scalar=b.scalar=0;return !std::memcmp(&a,&b,sizeof a);}
// Explicit corpus fixture staging, not a production current-state API.
static void stage(CharacterStateOwner& o,State s){o.state()=s;o.machine().current_index=s.current;o.native_fsm().current_present=s.current!=-1;o.machine().physical=s.body_present?0xb123456789abcdefull:0;}
struct Fixture {
 CharacterStateOwner* owner;Facts* facts;std::uint32_t dt=0;int stun=-1,scare=-1,fail=-1;
 std::vector<Request> body_calls;std::vector<std::vector<std::uint32_t>> outer_calls;
 std::vector<StateOwnerUpdateRequest24> other_calls;
 StateOwnerServices16* behavior=nullptr;bool route_move=false,mutate_dt=false,real_effects=false,clear_during_begin=false;
};
static void body(void* p,State* s,const Request* r){auto& f=*static_cast<Fixture*>(p);f.body_calls.push_back(*r);
 if(r->service==set_animation)s->current_animation=r->argument[0];
 if(r->service==stop)s->heading_active=0;
 if(r->service==raise_event&&r->argument[0]==0x3f&&f.route_move)check(f.owner->event(0x3f,r->identity,*f.behavior)==1);
}
static int remain(void* p,StateOwnerMachine40*,const StateOwnerRequest48* r,StateOwnerResponse8*){auto& f=*static_cast<Fixture*>(p);
 if(r->operation==state_owner_character_event||r->operation==state_owner_profile_begin||r->operation==state_owner_profile_end)return 0;
 (void)f;return 1;
}
static int other(void* p,StateOwnerMachine40* m,const StateOwnerUpdateRequest24* r){auto& f=*static_cast<Fixture*>(p);check(!r->reserved&&r->character==m->fsm->character&&r->state==m->fsm->state->current&&r->elapsed_ms==m->fsm->state->elapsed_ms);check(f.owner->info(r->state)->update==r->source_function);f.other_calls.push_back(*r);return f.fail==6?1:0;}
static int outer(void* p,NativeFsm24* n,const NativeFsmRequest32* r,std::uint32_t* out){auto& f=*static_cast<Fixture*>(p);check(!r->force&&!r->payload);
 const auto elapsed=n->state->elapsed_ms;const auto current=n->current_present?std::uint32_t(n->state->current):0xffffffffu;
 if(r->service==fsm_profile_begin||r->service==fsm_profile_end){check(!r->argument0&&!r->argument1&&!r->subject);f.outer_calls.push_back({r->service,0,0,0,0,0});}
 else if(r->service==fsm_engine_dt){check(!r->argument0&&!r->argument1&&!r->subject);f.outer_calls.push_back({r->service,0,0,elapsed,0,f.dt});*out=f.dt;if(f.mutate_dt)n->state->elapsed_ms=0x12345678;}
 else{check(r->service==fsm_set_stun||r->service==fsm_set_scare);check(r->subject==n->character);f.outer_calls.push_back({r->service,r->argument0,r->argument1,elapsed,current,0});
  const int next=r->service==fsm_set_stun?f.stun:f.scare;
  if(next>=0){if(f.real_effects)check(f.owner->transition(next,-1,0,*f.behavior)==1);else{auto value=f.owner->state();value.current=next;stage(*f.owner,value);}}
 }
 if(r->service==fsm_profile_begin&&f.clear_during_begin)check(f.owner->transition(99,-1,0,*f.behavior)==1);
 return f.fail==int(r->service)?1:0;
}
static StateOwnerFrameContext56 frame(CharacterStateOwner& o,Facts& facts,Services& services,Fixture& f){return {&o.machine(),&facts,&services,{&f,outer},{&f,other}};}
struct Case {std::uint32_t operation,a,b,count,result,mode;std::uint64_t payload;};
int main(int argc,char** argv){try{
 check(argc==3);std::uint32_t updates=0,body_requests=0,outer_cases=0,outer_requests=0,other_updates=0,guards=0;
 {std::ifstream file(argv[1],std::ios::binary);check(read<std::uint32_t>(file)==0x31545343);const auto count=read<std::uint32_t>(file);
 for(unsigned i=0;i<count;++i){const auto item=read<Case>(file);const auto initial=read<State>(file);auto facts=read<Facts>(file);const auto expected=read<State>(file);std::vector<Request> gold(item.count);for(auto& r:gold)r=read<Request>(file);if(item.operation!=2)continue;
  CharacterStateOwner owner(0xa123456789abcdefull);stage(owner,initial);Fixture fixture{};fixture.owner=&owner;fixture.facts=&facts;fixture.dt=item.a;Services services{&fixture,body};auto context=frame(owner,facts,services,fixture);
  check(dh2_character_state_owner_frame(&context)==int(item.result));check(same(owner.state(),expected)&&fixture.body_calls.size()==gold.size());for(unsigned j=0;j<gold.size();++j)check(same(fixture.body_calls[j],gold[j]));++updates;body_requests+=item.count;
 }check(file.peek()==std::ifstream::traits_type::eof());}
 {std::ifstream file(argv[2],std::ios::binary);check(read<std::uint32_t>(file)==0x314d464e);const auto count=read<std::uint32_t>(file);
 for(unsigned i=0;i<count;++i){const auto initial=read<std::int32_t>(file);const auto mask=read<std::uint32_t>(file),policy=read<std::uint32_t>(file),elapsed=read<std::uint32_t>(file),dt=read<std::uint32_t>(file);check(read<std::uint32_t>(file)==0);const auto stun=read<std::int32_t>(file),scare=read<std::int32_t>(file);const auto final=read<std::uint32_t>(file),final_elapsed=read<std::uint32_t>(file),calls=read<std::uint32_t>(file);std::vector<std::vector<std::uint32_t>> gold;
  for(unsigned j=0;j<calls;++j){std::vector<std::uint32_t> row;for(unsigned k=0;k<6;++k)row.push_back(read<std::uint32_t>(file));if(row[0]!=fsm_current_update)gold.push_back(row);}
  CharacterStateOwner owner(0xa123456789abcdefull);State state;state.current=initial;state.attack_gate=mask;state.flags=policy;state.elapsed_ms=elapsed;stage(owner,state);Facts facts;Fixture fixture{};fixture.owner=&owner;fixture.facts=&facts;fixture.dt=dt;fixture.stun=stun;fixture.scare=scare;Services services{&fixture,body};auto context=frame(owner,facts,services,fixture);
  check(dh2_character_state_owner_frame(&context)==1);check(std::uint32_t(owner.state().current)==final&&owner.state().elapsed_ms==final_elapsed);check(fixture.outer_calls==gold);++outer_cases;outer_requests+=gold.size();other_updates+=fixture.other_calls.size();
 }check(file.peek()==std::ifstream::traits_type::eof());}
 CharacterStateOwner owner(0xa123456789abcdefull);Facts facts;facts.idle=215;facts.walk=223;facts.attack_moving=248;facts.attack_static=243;Fixture fixture{};fixture.owner=&owner;fixture.facts=&facts;fixture.dt=16;Services services{&fixture,body};StateOwnerBehaviorPredicate8 predicates{};StateOwnerBehaviorContext40 behavior_context{&facts,&services,&predicates,{&fixture,remain}};StateOwnerServices16 behavior{};check(dh2_character_state_owner_behavior_bind(&behavior,&behavior_context)==1);fixture.behavior=&behavior;auto context=frame(owner,facts,services,fixture);
 auto reset=[&](int id){fixture.body_calls.clear();fixture.outer_calls.clear();fixture.other_calls.clear();fixture.fail=-1;fixture.real_effects=false;fixture.clear_during_begin=false;fixture.mutate_dt=false;State state;state.current=id;state.elapsed_ms=0xfffffff8u;stage(owner,state);};
 auto reject=[&](StateOwnerFrameContext56 bad){const auto before=owner.state();const auto count=fixture.outer_calls.size();check(dh2_character_state_owner_frame(&bad)==-1&&same(owner.state(),before)&&fixture.outer_calls.size()==count);++guards;};
 reset(3);auto bad=context;bad.machine=nullptr;reject(bad);bad=context;bad.facts=nullptr;reject(bad);bad=context;bad.bodies=nullptr;reject(bad);bad=context;bad.outer.invoke=nullptr;reject(bad);bad=context;bad.other_updates.invoke=nullptr;reject(bad);facts.reserved=1;reject(context);facts.reserved=0;owner.machine().current_index=4;reject(context);owner.machine().current_index=3;owner.native_fsm().reserved=1;reject(context);owner.native_fsm().reserved=0;
 const auto genuine=owner.machine().states;std::vector<StateOwnerInfo40> altered(genuine,genuine+20);owner.machine().states=altered.data();altered[3].update=0xdead;reject(context);altered[3]=genuine[3];altered[17].update=0;reject(context);owner.machine().states=genuine;
 // Live frame service reentry uses the owner transition/event, not assignment.
 reset(4);fixture.route_move=true;check(dh2_character_state_owner_frame(&context)==1&&owner.state().current==3&&owner.state().elapsed_ms==0);check(fixture.body_calls.size()==3&&fixture.body_calls[0].service==raise_event&&fixture.body_calls[1].service==stop&&fixture.body_calls[2].service==set_animation);fixture.route_move=false;
 reset(3);fixture.mutate_dt=true;check(dh2_character_state_owner_frame(&context)==1&&owner.state().elapsed_ms==8);
 reset(3);fixture.clear_during_begin=true;check(dh2_character_state_owner_frame(&context)==1&&!owner.native_fsm().current_present&&owner.state().elapsed_ms==8&&fixture.body_calls.empty());
 reset(3);fixture.real_effects=true;fixture.stun=5;fixture.scare=4;owner.state().attack_gate=6;check(dh2_character_state_owner_frame(&context)==1&&owner.state().current==4&&owner.state().elapsed_ms==0);check(fixture.outer_calls.size()==5&&fixture.outer_calls[2][0]==fsm_set_stun&&fixture.outer_calls[3][0]==fsm_set_scare&&fixture.outer_calls.back()[0]==fsm_profile_end);fixture.stun=fixture.scare=-1;
 // Prefix failures retain exactly the already delivered elapsed/effects.
 for(int fail:{0,1,2,3,5}){reset(3);owner.state().attack_gate=6;fixture.fail=fail;check(dh2_character_state_owner_frame(&context)==-2);check(owner.state().elapsed_ms==(fail<=1?0xfffffff8u:8u));check(fixture.outer_calls.back()[0]==std::uint32_t(fail));++guards;}
 reset(17);fixture.fail=6;check(dh2_character_state_owner_frame(&context)==-2&&owner.state().elapsed_ms==8&&fixture.other_calls.size()==1&&fixture.other_calls[0].source_function==owner.info(17)->update);++guards;
 std::printf("{\"validation\":\"PASS\",\"original_bounded_frame_cases\":%u,\"original_bounded_body_requests\":%u,\"original_outer_frame_cases\":%u,\"original_outer_service_requests\":%u,\"other_update_metadata_deliveries\":%u,\"nested_source_owner_frame_checks\":3,\"captured_elapsed_reload_checks\":1,\"guard_and_prefix_checks\":%u,\"mismatches\":0}\n",updates,body_requests,outer_cases,outer_requests,other_updates,guards);return 0;
 }catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}}
