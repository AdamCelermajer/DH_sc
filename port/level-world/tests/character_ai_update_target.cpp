#include "../character_ai_update_target.hpp"
#include "../character_ai_sight.hpp"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>
namespace k=dh2::character_ai_update_target;
constexpr std::uintptr_t A=0x10014000,B=0x10018000,AI=A+0x3c8,P=0x10020000,Q=0x10024000;
struct Call {unsigned op,kind;std::uintptr_t subject,other;unsigned event,alive,sight;};
struct Fixture {
    k::Owner a{A,8,0,0},b{B,8,0,0};
    k::State state{AI,&a,0,P,Q,0,0,1,0};
    k::Result result{};
    unsigned awaiting=0,limbus=0,interactive1=1,dead=0,sight=1,interactive2=1,
        ranged=0,close=0,range=0,melee=1,mutation=0,fail=0,throws=0;
    unsigned interactiveCalls=0;
    std::vector<Call> calls;
    k::Services services{this,invoke};
    static int invoke(void* ctx,k::State* state,const k::Request* q,k::Response* r) {
        auto& f=*static_cast<Fixture*>(ctx);
        f.calls.push_back({unsigned(q->operation),unsigned(q->kind),q->subject,q->other,q->event,
            state->alive_snapshot,state->sight_snapshot});
        if(f.fail==f.calls.size())return 7;
        if(f.throws==f.calls.size())throw std::runtime_error("provider");
        switch(q->operation) {
        case k::Operation::is_awaiting_spawn:r->word=f.awaiting;break;
        case k::Operation::is_in_limbus:r->word=f.limbus;break;
        case k::Operation::is_interactive:
            r->word=++f.interactiveCalls==1?f.interactive1:f.interactive2;break;
        case k::Operation::char_ai_id:r->word=0xffffffff;break;
        case k::Operation::is_dead:r->word=f.dead;break;
        case k::Operation::is_in_sight:r->word=f.sight;break;
        case k::Operation::can_range_attack:r->word=f.ranged;break;
        case k::Operation::is_in_close_range:r->word=f.close;break;
        case k::Operation::is_in_range:r->word=f.range;break;
        case k::Operation::is_in_melee_range:r->word=f.melee;break;
        case k::Operation::raise_event:r->word=0xffffffff;break;
        }
        const auto op=q->operation;
        if((f.mutation==1 && op==k::Operation::is_awaiting_spawn) ||
           (f.mutation==2 && op==k::Operation::is_interactive && f.interactiveCalls==1) ||
           (f.mutation==3 && op==k::Operation::char_ai_id) ||
           (f.mutation==4 && op==k::Operation::is_dead) ||
           (f.mutation==5 && op==k::Operation::is_in_sight) ||
           (f.mutation==6 && op==k::Operation::raise_event && q->event==11) ||
           (f.mutation==7 && op==k::Operation::raise_event && q->event==13) ||
           (f.mutation==8 && op==k::Operation::can_range_attack) ||
           (f.mutation==9 && op==k::Operation::is_in_close_range))state->owner=&f.b;
        if((f.mutation==10 && op==k::Operation::is_interactive && f.interactiveCalls==1) ||
           (f.mutation==11 && op==k::Operation::raise_event && q->event==10) ||
           (f.mutation==12 && op==k::Operation::raise_event && q->event==12))state->target=state->last_target=0;
        if((f.mutation==13 && op==k::Operation::char_ai_id) ||
           (f.mutation==14 && op==k::Operation::is_dead) ||
           (f.mutation==15 && op==k::Operation::raise_event && q->event==11) ||
           (f.mutation==16 && op==k::Operation::raise_event && q->event==13) ||
           (f.mutation==17 && op==k::Operation::is_interactive && f.interactiveCalls==2) ||
           (f.mutation==18 && op==k::Operation::can_range_attack) ||
           (f.mutation==19 && op==k::Operation::is_in_close_range))state->target=Q;
        if(f.mutation==20 && op==k::Operation::raise_event && q->event==11)state->alive_snapshot=7;
        if(f.mutation==21 && op==k::Operation::raise_event && q->event==13)state->sight_snapshot=7;
        if(f.mutation==22 && op==k::Operation::is_dead)state->alive_snapshot=1;
        if(f.mutation==23 && op==k::Operation::is_in_sight)state->sight_snapshot=1;
        if(f.mutation==24 && op==k::Operation::char_ai_id)state->target=0;
        if(f.mutation==25 && op==k::Operation::is_in_close_range)state->target=0;
        if(f.mutation==26 && op==k::Operation::is_in_sight)state->owner=reinterpret_cast<k::Owner*>(&f.result);
        return 0;
    }
    k::Status run(){return k::update(&state,&services,&result);}
};
void dump(Fixture& f,k::Status s) {
    std::cout<<"{\"status\":"<<int(s)<<",\"owner\":"<<f.state.owner->identity
      <<",\"target\":"<<f.state.target<<",\"last\":"<<f.state.last_target
      <<",\"alive\":"<<unsigned(f.state.alive_snapshot)<<",\"sight\":"<<unsigned(f.state.sight_snapshot)<<",\"calls\":[";
    bool first=true;for(auto c:f.calls){if(!first)std::cout<<',';first=false;std::cout<<'['<<c.op<<','<<c.kind<<','<<c.subject<<','<<c.other<<','<<c.event<<','<<c.alive<<','<<c.sight<<']';}
    std::cout<<"]}\n";
}
int main(int argc,char** argv) {
    if(argc>1) {
        assert(argc==16);Fixture f;
        unsigned* values[]={&f.awaiting,&f.limbus,&f.interactive1,&f.dead,&f.sight,&f.interactive2,
            &f.ranged,&f.close,&f.range,&f.melee,&f.mutation};
        for(unsigned i=0;i<11;++i)*values[i]=std::strtoul(argv[i+1],nullptr,0);
        f.state.alive_snapshot=std::strtoul(argv[12],nullptr,0);f.state.sight_snapshot=std::strtoul(argv[13],nullptr,0);
        f.state.target=std::strtoul(argv[14],nullptr,0)?P:0;f.state.last_target=std::strtoul(argv[15],nullptr,0)?Q:0;
        dump(f,f.run());return 0;
    }
    unsigned count=0;
    {Fixture f;assert(f.run()==k::Status::complete && f.result.range_event==17 && f.result.event_calls==3);++count;}
    {Fixture f;f.awaiting=1;assert(f.run()==k::Status::complete && f.calls.size()==1);++count;}
    {Fixture f;f.limbus=1;assert(f.run()==k::Status::complete && f.calls.size()==2);++count;}
    {Fixture f;f.state.target=0;assert(f.run()==k::Status::complete && f.state.last_target==Q && f.calls.size()==2);++count;}
    {Fixture f;f.interactive1=0;assert(f.run()==k::Status::complete && !f.state.target && !f.state.last_target && f.result.last_event==12);++count;}
    {Fixture f;f.dead=1;f.state.alive_snapshot=1;f.mutation=11;assert(f.run()==k::Status::complete && !f.state.target && !f.state.alive_snapshot && f.calls.size()==6);++count;}
    {Fixture f;f.sight=0;f.state.sight_snapshot=1;f.mutation=12;assert(f.run()==k::Status::complete && !f.state.target && !f.state.sight_snapshot);++count;}
    {Fixture f;f.interactive2=0;assert(f.run()==k::Status::complete && f.result.decision==k::Decision::no_longer_interactive);++count;}
    for(unsigned event=14;event<=17;++event){Fixture f;f.ranged=event!=17;f.close=event==16;f.range=event==15;f.melee=event==17;
        assert(f.run()==k::Status::complete && f.result.range_event==event);++count;}
    {Fixture f;f.dead=2;assert(f.run()==k::Status::complete && f.state.alive_snapshot==3);++count;}
    {Fixture f;f.sight=256;assert(f.run()==k::Status::complete && !f.state.sight_snapshot && f.result.range_event==17);++count;}
    for(unsigned mutation=1;mutation<=23;++mutation){Fixture f;f.mutation=mutation;f.ranged=1;f.close=0;f.range=1;
        if(mutation==11){f.dead=1;f.state.alive_snapshot=1;}
        if(mutation==12){f.sight=0;f.state.sight_snapshot=1;}
        assert(f.run()==k::Status::complete);++count;}
    {Fixture f;f.mutation=24;assert(f.run()==k::Status::invalid_source_fact && f.calls.size()==4);++count;}
    {Fixture f;f.mutation=25;f.ranged=1;assert(f.run()==k::Status::complete && f.calls.back().other==0);++count;}
    {Fixture f;f.mutation=26;assert(f.run()==k::Status::invalid_source_fact);++count;}
    for(unsigned at=1;at<=12;++at){Fixture f;f.fail=at;assert(f.run()==k::Status::service_failed && f.calls.size()==at);++count;}
    {Fixture f;f.throws=8;assert(f.run()==k::Status::service_failed && f.state.alive_snapshot==1 && !f.state.sight_snapshot);++count;}
    {Fixture f;f.services.invoke=nullptr;assert(f.run()==k::Status::service_unavailable && f.calls.empty());++count;}
    {Fixture f;auto* alias=reinterpret_cast<k::Result*>(&f.state);assert(k::update(&f.state,&f.services,alias)==k::Status::invalid_argument && f.calls.empty());++count;}
    {Fixture f;auto* alias=reinterpret_cast<k::Result*>(&f.a);assert(k::update(&f.state,&f.services,alias)==k::Status::invalid_argument && f.calls.empty());++count;}
    {Fixture f;auto* bad=reinterpret_cast<k::State*>(reinterpret_cast<char*>(&f.state)+1);assert(k::update(bad,&f.services,&f.result)==k::Status::invalid_argument);++count;}
    // Bind the existing sight kernel with a live owner propagation bridge.
    // Its second position query changes the actor owner before the tail radius
    // getter, and the new caller then uses that owner for all later events.
    {
        namespace sight=dh2::character_ai_sight;
        struct Bridge {
            Fixture* fixture;
            sight::Point owner_point{{0,0,0}},target_point{{0x40400000,0,0}};
            sight::ViewRadius radius{0x40800000};
            unsigned positions=0;std::uintptr_t radius_owner=0;
        };
        Fixture f;Bridge bridge{&f};
        f.services.context=&bridge;
        f.services.invoke=[](void* p,k::State* state,const k::Request* q,k::Response* result)->int {
            auto& bridge=*static_cast<Bridge*>(p);
            const auto status=Fixture::invoke(bridge.fixture,state,q,result);
            if(status || q->operation!=k::Operation::is_in_sight)return status;
            sight::State live{state->identity,state->owner->identity,state->target};
            sight::Services services{
                &bridge,
                [](void* c,sight::State* inner,std::uintptr_t object,const sight::Point** point)->std::int32_t {
                    auto& b=*static_cast<Bridge*>(c);++b.positions;
                    *point=object==P?&b.target_point:&b.owner_point;
                    if(object==P){b.fixture->state.owner=&b.fixture->b;inner->owner=B;}
                    return 0;
                },
                [](void* c,sight::State*,std::uintptr_t owner,const sight::ViewRadius** row)->std::int32_t {
                    auto& b=*static_cast<Bridge*>(c);b.radius_owner=owner;*row=&b.radius;return 0;
                }
            };
            sight::Result output{};
            if(sight::evaluate_object(&live,q->other,&services,&output)!=sight::Status::complete)return 1;
            result->word=output.value;return 0;
        };
        assert(f.run()==k::Status::complete && bridge.positions==2 && bridge.radius_owner==B && f.result.range_event==17);
        assert(f.calls.back().subject==B && f.state.sight_snapshot==1);++count;
    }
    {Fixture outer;
        outer.services.invoke=[](void* p,k::State* state,const k::Request* q,k::Response* response)->int {
            auto* fixture=static_cast<Fixture*>(p);
            const auto status=Fixture::invoke(fixture,state,q,response);
            if(q->operation==k::Operation::raise_event && q->event==13){Fixture inner;inner.awaiting=1;
                assert(inner.run()==k::Status::complete && inner.calls.size()==1);}
            return status;
        };
        assert(outer.run()==k::Status::complete && outer.result.range_event==17 && outer.calls.size()==12);++count;
    }
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<count<<",\"live_owner_target_queries\":true,\"event_before_snapshot_writes\":true,\"raw_predicate_conversions\":true,\"failure_alias_contracts\":true}\n";
}
