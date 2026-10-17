#include "../character_ai_interaction_range.hpp"
#include "../character_ai_melee_range.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>
namespace k=dh2::character_ai_interaction_range;
namespace m=dh2::character_ai_melee_range;
constexpr std::uintptr_t A=0x10014000,B=0x10018000,P=0x10020000,AI=A+0x3c8,NODE=0x10028000;
unsigned word(float x){unsigned r;std::memcpy(&r,&x,4);return r;}
struct Call {unsigned op;std::uintptr_t subject,other;};
struct Fixture {
    k::State state{AI,A,P};k::ObjectFacts target{P,0};
    k::Point owner{{0,0,0}},spot{{0x40400000,0,0}};
    k::AiRow row{},rowB{};
    unsigned cache=1,visual=0,lookup=0,type=0xffffffff,radA=word(2),radT=word(2),mutation=0,fail=0,throws=0,alias=0;
    k::Result result{};std::vector<Call> calls;k::Services services{this,invoke};
    Fixture(){row.words[6]=word(1);rowB.words[6]=word(5);}
    static int invoke(void* c,k::State* s,const k::Request* q,k::Response* out) {
        auto& f=*static_cast<Fixture*>(c);f.calls.push_back({unsigned(q->operation),q->subject,q->other});
        if(f.fail==f.calls.size())return 9;
        if(f.throws==f.calls.size())throw std::runtime_error("provider");
        switch(q->operation) {
        case k::Operation::target_position:
            out->view=f.alias==1?static_cast<void*>(&f.result):&f.owner;
            if(f.mutation==1)s->owner=B;
            break;
        case k::Operation::interaction_spot:
            if(!f.cache){f.cache=1;f.target.interaction_node_2e8=f.visual && f.lookup?NODE:0;}
            out->view=f.alias==2?static_cast<void*>(&f.result):&f.target;out->point=f.spot;
            if(f.mutation==2)f.owner.words[0]=word(1);
            if(f.mutation==3)s->target_40=0;
            if(f.mutation==13)s->owner=0;
            if(f.mutation==14)f.target.identity=P+4;
            break;
        case k::Operation::melee_radius:
            out->word=f.radA;
            if(f.mutation==4){f.owner.words[0]=word(100);f.spot.words[0]=word(100);}
            if(f.mutation==5)s->owner=B;
            if(f.mutation==10)f.target.interaction_node_2e8=NODE;
            break;
        case k::Operation::interaction_radius:
            out->word=f.radT;if(f.mutation==6)s->owner=B;
            break;
        case k::Operation::char_ai:
            out->view=f.alias==3?static_cast<void*>(&f.result):(q->subject==B?&f.rowB:&f.row);
            if(f.mutation==7)s->owner=B;
            if(f.mutation==8)f.row.words[6]=word(9);
            break;
        case k::Operation::interaction_type:
            out->word=f.type;
            if(f.mutation==9){f.radA=f.radT=0;f.row.words[6]=0;f.spot.words[0]=word(100);}
            break;
        }
        return 0;
    }
    k::Status run(std::uintptr_t candidate=P){return k::evaluate_object(&state,candidate,&services,&result);}
};
void dump(const Fixture& f,k::Status status) {
    const auto& r=f.result;
    std::cout<<"{\"status\":"<<int(status)<<",\"value\":"<<r.value<<",\"squared\":"<<r.distance_squared_word
       <<",\"distance\":"<<r.distance_word<<",\"remaining\":"<<r.remaining_word<<",\"threshold\":"<<r.threshold_word
       <<",\"node_present\":"<<r.node_present<<",\"type\":"<<r.interaction_type<<",\"owner\":"<<f.state.owner
       <<",\"target40\":"<<f.state.target_40<<",\"ai\":"<<f.state.ai<<",\"cache\":"<<f.cache
       <<",\"node\":"<<f.target.interaction_node_2e8<<",\"calls\":[";
    bool first=true;for(auto c:f.calls){if(!first)std::cout<<',';first=false;std::cout<<'['<<c.op<<','<<c.subject<<','<<c.other<<']';}
    std::cout<<"]}\n";
}
int main(int argc,char** argv) {
    if(argc>1) {
        assert(argc==18);unsigned x[17];for(unsigned i=0;i<17;++i)x[i]=std::strtoul(argv[i+1],nullptr,0);
        Fixture f;f.state.target_40=x[1]?P:0;f.target.interaction_node_2e8=x[2]?NODE:0;f.cache=x[3];f.visual=x[4];f.lookup=x[5];f.type=x[6];
        for(unsigned i=0;i<3;++i){f.owner.words[i]=x[7+i];f.spot.words[i]=x[10+i];}
        f.radA=x[13];f.radT=x[14];f.row.words[6]=x[15];
        float threshold;std::memcpy(&threshold,&x[15],4);f.rowB.words[6]=word(threshold+4.f);f.mutation=x[16];
        const auto status=f.run(x[0]?P:0);dump(f,status);return 0;
    }
    unsigned cases=0;
    {Fixture f;assert(f.run()==k::Status::complete && f.result.value && f.result.remaining_word==word(-1));++cases;}
    {Fixture f;f.spot.words[0]=word(5);assert(f.run()==k::Status::complete && f.result.value);++cases;}
    {Fixture f;f.type=8;assert(f.run()==k::Status::complete && f.result.value);++cases;}
    {Fixture f;f.type=8;f.spot.words[0]=word(4);assert(f.run()==k::Status::complete && f.result.value);++cases;}
    {Fixture f;f.target.interaction_node_2e8=NODE;f.spot.words[0]=word(79);assert(f.run()==k::Status::complete && f.result.value && f.calls.size()==3);++cases;}
    {Fixture f;f.target.interaction_node_2e8=NODE;f.spot.words[0]=word(80);assert(f.run()==k::Status::complete && f.result.value);++cases;}
    {Fixture f;f.target.interaction_node_2e8=NODE;f.type=8;f.spot.words[0]=0;assert(f.run()==k::Status::complete && f.result.value);++cases;}
    {Fixture f;f.state.target_40=0;assert(f.run(0)==k::Status::complete && f.calls.empty());++cases;}
    {Fixture f;assert(f.run(0)==k::Status::complete && f.result.candidate==P);++cases;}
    for(unsigned node=0;node<2;++node){Fixture f;f.cache=0;f.visual=1;f.lookup=node;
        assert(f.run()==k::Status::complete && f.cache==1 && f.result.node_present==node && f.calls.size()==(node?3:6));++cases;}
    {Fixture f;f.cache=0;f.target.interaction_node_2e8=NODE;assert(f.run()==k::Status::complete && !f.result.node_present && !f.target.interaction_node_2e8);++cases;}
    for(unsigned mutation=1;mutation<=10;++mutation){Fixture f;f.mutation=mutation;assert(f.run()==k::Status::complete);
        if(mutation==2)assert(f.result.distance_word==word(2));
        if(mutation==4 || mutation==9)assert(f.result.distance_word==word(3) && f.result.remaining_word==word(-1));
        if(mutation==5 || mutation==6)assert(f.calls[4].subject==B && f.result.threshold_word==word(5));
        if(mutation==7)assert(f.calls[4].subject==A && f.calls[5].other==B && f.result.threshold_word==word(1));
        if(mutation==8)assert(f.result.threshold_word==word(9));
        if(mutation==10)assert(!f.result.node_present && f.calls.size()==6);
        ++cases;}
    {Fixture f;f.spot={{word(1),word(2),word(3)}};assert(f.run()==k::Status::complete && f.result.distance_squared_word==word(14));++cases;}
    for(unsigned i=1;i<=6;++i){Fixture f;f.fail=i;assert(f.run()==k::Status::service_failed && f.calls.size()==i);++cases;}
    {Fixture f;f.cache=0;f.visual=1;f.lookup=1;f.fail=3;
        assert(f.run()==k::Status::service_failed && f.cache==1 && f.target.interaction_node_2e8==NODE);++cases;}
    {Fixture f;f.throws=6;assert(f.run()==k::Status::service_failed && f.result.remaining_word==word(-1));++cases;}
    {Fixture f;f.services.invoke=nullptr;assert(f.run()==k::Status::service_unavailable && f.calls.empty());++cases;}
    for(unsigned alias=1;alias<=3;++alias){Fixture f;f.alias=alias;assert(f.run()==k::Status::invalid_source_fact);++cases;}
    {Fixture f;f.mutation=13;assert(f.run()==k::Status::invalid_source_fact && f.calls.size()==4);++cases;}
    {Fixture f;f.mutation=14;assert(f.run()==k::Status::invalid_source_fact && f.calls.size()==2);++cases;}
    {Fixture f;assert(k::evaluate_object(&f.state,P,&f.services,reinterpret_cast<k::Result*>(&f.state))==k::Status::invalid_argument && f.calls.empty());++cases;}
    {Fixture f;alignas(k::State) unsigned char storage[sizeof(k::State)+1]{};
        assert(k::evaluate_object(reinterpret_cast<k::State*>(storage+1),P,&f.services,&f.result)==k::Status::invalid_argument);++cases;}
    {Fixture f;alignas(k::Services) unsigned char storage[sizeof(k::Services)+1]{};
        assert(k::evaluate_object(&f.state,P,reinterpret_cast<k::Services*>(storage+1),&f.result)==k::Status::invalid_argument);++cases;}
    // Frozen melee source's generic fallback uses this new exact caller, with
    // independent output storage and the same genuine original-AI state view.
    {Fixture f;m::Result result{};m::Services services{&f,[](void* c,m::State* s,const m::Request* q,m::Response* out)->int {
        auto& f=*static_cast<Fixture*>(c);
        if(q->operation==m::Operation::resolve_object){out->view=nullptr;return 0;}
        assert(q->operation==m::Operation::interaction_range && q->subject==AI && q->other==P);
        if(k::evaluate_object(s,q->other,&f.services,&f.result)!=k::Status::complete)return 1;
        out->word=f.result.value;return 0;
    }};
        assert(m::evaluate_object(&f.state,P,&services,&result)==m::Status::complete && result.used_interaction_range && result.value);++cases;}
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"copied_spot_live_owner\":true,\"lazy_node_branch\":true,\"melee_composition\":true,\"failure_alias_contracts\":true}\n";
}
