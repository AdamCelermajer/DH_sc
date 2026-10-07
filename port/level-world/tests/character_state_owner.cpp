#include "../character_state_owner.hpp"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
using Row=std::array<std::uint32_t,8>;
static void check(bool x){if(!x)throw std::runtime_error("state owner audit mismatch");}
constexpr std::uintptr_t owners[2]={0x123456789abcdef0ull,0xfedcba9876543210ull};
constexpr std::uintptr_t body=0xabcdef1234567890ull,payload=0xa123456789abcdefull;
struct Context {std::vector<Row> calls;int mutation=0,initial=0,accepted=1,chosen=-1,fail=0,nested=0;CharacterStateOwner* owner=nullptr;};
static void select(StateOwnerMachine40* m,int id){m->current_index=id;m->fsm->current_present=id>=0;m->fsm->state->current=id;}
static int invoke(void* context,StateOwnerMachine40* m,const StateOwnerRequest48* r,StateOwnerResponse8* out){
 auto& c=*static_cast<Context*>(context);check(!r->reserved[0]&&!r->reserved[1]);
 const auto owner=r->character==body?2u:r->character==owners[0]?0u:r->character==owners[1]?1u:99u;check(owner!=99);
 auto value=static_cast<std::uint32_t>(r->payload);if((r->operation==2||r->operation==3||r->operation==4)&&r->payload==payload)value=0x89abcdef;
 c.calls.push_back({r->operation,r->source_function,static_cast<std::uint32_t>(r->state),static_cast<std::uint32_t>(r->other),r->event,r->adjustment,owner,value});
 if(c.fail==int(r->operation))return 1;
 if(c.mutation==1&&r->operation==1)m->fsm->character=owners[1];
 if((c.mutation==2&&r->operation==3)||(c.mutation==3&&r->operation==4))select(m,c.initial!=5?5:3);
 if(r->operation==4){out->next=c.chosen!=-1?c.chosen:r->other;out->accepted=c.accepted;}
 if(c.nested&&r->operation==1){c.nested=0;StateOwnerServices16 service{&c,invoke};check(c.owner->transition(5,-1,0,service)==1);}
 return 0;
}
static std::uint32_t read(std::istream& s){std::uint32_t x;s.read(reinterpret_cast<char*>(&x),4);check(bool(s));return x;}
static int si(std::uint32_t x){return static_cast<std::int32_t>(x);}
int main(int argc,char** argv){try{
 check(argc==3);std::ifstream file(argv[1],std::ios::binary);char magic[4];file.read(magic,4);check(std::string(magic,4)=="SBO1");auto count=read(file);std::uint32_t callbacks=0,guards=0;
 for(std::uint32_t i=0;i<count;++i){std::array<std::uint32_t,9> input;for(auto& x:input)x=read(file);std::array<std::uint32_t,5> final;for(auto& x:final)x=read(file);auto result=read(file),calls=read(file);std::vector<Row> gold(calls);for(auto& r:gold)for(auto& x:r)x=read(file);
  CharacterStateOwner owner(owners[0]);auto& m=owner.machine();auto& s=owner.state();select(&m,si(input[1]));s.elapsed_ms=0xfffffff0;s.attack_gate=7;s.idle_suppressed=1;s.body_present=input[8];m.physical=input[8]?body:0;
  Context ctx;ctx.mutation=si(input[5]);ctx.initial=si(input[0]==2?input[4]:input[1]);ctx.accepted=si(input[6]);ctx.chosen=si(input[7]);StateOwnerServices16 service{&ctx,invoke};int actual;
  if(!input[0])actual=owner.transition(si(input[2]),si(input[3]),payload,service);else if(input[0]==1)actual=owner.event(si(input[3]),payload,service);else actual=owner.initialize_level(si(input[4]),service);
  check(actual==si(result));check(ctx.calls==gold);check((std::array<std::uint32_t,5>{static_cast<std::uint32_t>(s.current),s.elapsed_ms,s.attack_gate,s.idle_suppressed,m.fsm->character==owners[0]?0u:1u})==final);callbacks+=calls;
 }
 // Independent owners retain distinct event maps; shared behavior identities
 // remain equal across sessions, exactly the original singleton distinction.
 CharacterStateOwner a(owners[0]),b(owners[1]);for(int id=0;id<20;++id){check(a.info(id)&&b.info(id));check(a.info(id)!=b.info(id));check(a.info(id)->events!=b.info(id)->events);check(a.info(id)->singleton==b.info(id)->singleton);++guards;}
 Context c;StateOwnerServices16 service{&c,invoke};auto& m=a.machine();check(a.info(-1)==nullptr&&a.info(20)==nullptr);++guards;
 check(dh2_character_state_owner_transition(nullptr,3,-1,0,&service)==-1);++guards;
 check(dh2_character_state_owner_transition(&m,3,-1,0,nullptr)==-1);++guards;
 auto before=a.state();m.reserved=1;check(a.initialize_level(3,service)==-1&&c.calls.empty());check(a.state().current==before.current);m.reserved=0;++guards;
 m.fsm->reserved=1;check(a.event(0x2a,0,service)==-1&&c.calls.empty());m.fsm->reserved=0;++guards;
 m.current_index=20;check(a.transition(3,-1,0,service)==-1);m.current_index=-1;++guards;
 check(a.transition(3,-1,0,service)==1);c.calls.clear();c.fail=1;auto prior=a.state().current;check(a.transition(4,-1,0,service)==-2&&a.state().current==prior&&c.calls.size()==1);c.fail=0;++guards;
 // A callback invalidating current is a source null-dereference boundary;
 // native API refuses rather than fabricating an accepted terminal state.
 StateOwnerServices16 clear{&c,[](void* p,StateOwnerMachine40* n,const StateOwnerRequest48* r,StateOwnerResponse8* out){auto result=invoke(p,n,r,out);if(r->operation==3)select(n,-1);return result;}};
 check(a.event(0x22,0,clear)==-3);++guards;
 // Actual synchronous nested transition: outer blur captures previous/next,
 // inner changes current, outer focus still receives its captured previous3.
 select(&m,3);c.calls.clear();c.owner=&a;c.nested=1;check(a.transition(4,-1,0,service)==1);check(a.state().current==4&&c.calls.size()==6);check(c.calls[4][0]==2&&c.calls[4][2]==4&&c.calls[4][3]==3);++guards;
 std::ifstream predicates(argv[2],std::ios::binary);predicates.read(magic,4);check(std::string(magic,4)=="SBP1");const auto predicate_count=read(predicates);
 for(std::uint32_t i=0;i<predicate_count;++i){const auto fn=read(predicates);const auto id=si(read(predicates));StateOwnerPredicateFacts16 facts{};predicates.read(reinterpret_cast<char*>(&facts),sizeof(facts));StateOwnerResponse8 gold{};predicates.read(reinterpret_cast<char*>(&gold),sizeof(gold));check(bool(predicates));StateOwnerResponse8 actual{0x12345678,0xabcdef01};check(dh2_character_state_owner_predicate(&actual,fn,id,&facts)==1);check(actual.next==gold.next&&actual.accepted==gold.accepted);}
 StateOwnerPredicateFacts16 facts{};StateOwnerResponse8 response{123,456};check(dh2_character_state_owner_predicate(&response,0x3ad2e4,0,&facts)==-2&&response.next==123&&response.accepted==456);++guards;
 facts.reserved[0]=1;check(dh2_character_state_owner_predicate(&response,0x3ad22c,3,&facts)==-1&&response.next==123&&response.accepted==456);++guards;
 std::printf("{\"validation\":\"PASS\",\"gold_cases\":%u,\"ordered_callbacks\":%u,\"predicate_gold_cases\":%u,\"owned_registry_states\":20,\"independent_owner_and_guard_checks\":%u,\"nested_transition_checks\":1,\"mismatches\":0}\n",count,callbacks,predicate_count,guards);return 0;
 }catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}}
