#include "../character_ai_melee_range.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>
namespace k=dh2::character_ai_melee_range;
constexpr std::uintptr_t A=0x10014000,B=0x10018000,P=0x10020000,R=0x10028000,AI=A+0x3c8;
unsigned word(float x){unsigned r;std::memcpy(&r,&x,4);return r;}
struct Call {unsigned op,kind;std::uintptr_t subject,other;unsigned key;};
struct Fixture {
    k::State state{AI,A,P};k::ResolvedObject resolved{R,0};
    k::Point owner{{0,0,0}},target{{0x40400000,0,0}};
    unsigned resolving=1,type=8,radA=word(2),radB=word(2),debug=0,debug2=1,mutation=0,generic=7,fail=0,throws=0;
    k::Result result{};std::vector<Call> calls;
    k::Services services{this,invoke};
    static int invoke(void* c,k::State* s,const k::Request* q,k::Response* out) {
        auto& f=*static_cast<Fixture*>(c);f.calls.push_back({unsigned(q->operation),unsigned(q->kind),q->subject,q->other,q->key});
        if(f.fail==f.calls.size())return 9;
        if(f.throws==f.calls.size())throw std::runtime_error("provider");
        switch(q->operation) {
        case k::Operation::resolve_object:out->view=f.resolving?&f.resolved:nullptr;break;
        case k::Operation::interaction_type:out->word=f.type;break;
        case k::Operation::target_position:
            out->view=q->subject==P?&f.target:&f.owner;
            if(q->subject==P && f.mutation==3)f.owner.words[0]=word(1);
            if(q->subject==P && f.mutation==8)out->view=&f.result;
            break;
        case k::Operation::melee_radius:
            out->word=q->kind==k::Subject::original_ai?f.radA:f.radB;
            if(q->kind==k::Subject::original_ai && f.mutation==4){f.owner.words[0]=word(100);f.target.words[0]=word(100);}
            break;
        case k::Operation::diagnostic_switch:
            out->word=q->key==1?f.debug:f.debug2;
            if(f.mutation==7){f.radA=word(0);f.radB=word(0);f.target.words[0]=word(100);}
            break;
        case k::Operation::interaction_range:out->word=f.generic;break;
        }
        if((f.mutation==1 && q->operation==k::Operation::resolve_object) ||
           (f.mutation==2 && q->operation==k::Operation::interaction_type) ||
           (f.mutation==5 && q->operation==k::Operation::melee_radius))s->owner=B;
        if(f.mutation==6 && q->operation==k::Operation::target_position)s->target_40=0;
        return 0;
    }
    k::Status run(std::uintptr_t candidate=P){
        if(mutation==9){target.words[1]=word(2);target.words[2]=word(3);}
        return k::evaluate_object(&state,candidate,&services,&result);
    }
};
struct RadiusFixture {
    k::State state{AI,A,P};k::AiRow rows[9]{},next[9]{};k::AiTable table{rows,9};
    unsigned equipment=1,radius=word(2),id=8,mutation=0,fail=0;
    k::RadiusResult result{};std::vector<Call> calls;
    k::RadiusServices services{this,invoke};
    RadiusFixture(){rows[8].words[8]=radius;next[8].words[8]=word(100);}
    static int invoke(void* c,k::State* state,const k::RadiusRequest* q,k::RadiusResponse* out) {
        auto& f=*static_cast<RadiusFixture*>(c);f.calls.push_back({unsigned(q->operation),0,q->owner,0,0});
        if(f.fail==f.calls.size())return 1;
        switch(q->operation) {
        case k::RadiusOperation::inventory_can_melee_attack:out->word=f.equipment;if(f.mutation==1)state->owner=B;break;
        case k::RadiusOperation::ai_table:out->table=&f.table;if(f.mutation==2)state->owner=B;break;
        case k::RadiusOperation::char_ai_id:
            out->word=f.id;
            if(f.mutation==3)f.table.rows=f.next;
            if(f.mutation==4)f.rows[f.id].words[8]=word(7);
            if(f.mutation==5)state->owner=B;
            break;
        }
        return 0;
    }
    k::Status run(){return k::get_radius(&state,&services,&result);}
};
void dumpCalls(const std::vector<Call>& calls){std::cout<<'[';bool first=true;for(auto c:calls){if(!first)std::cout<<',';first=false;
    std::cout<<'['<<c.op<<','<<c.kind<<','<<c.subject<<','<<c.other<<','<<c.key<<']';}std::cout<<']';}
int main(int argc,char** argv) {
    if(argc>1) {
        if(std::string(argv[1])=="melee") {
            assert(argc==14);Fixture f;
            f.resolving=std::strtoul(argv[2],nullptr,0);f.resolved.word_f4=std::strtoul(argv[3],nullptr,0);f.type=std::strtoul(argv[4],nullptr,0);
            const auto explicit_target=std::strtoul(argv[5],nullptr,0);f.state.target_40=std::strtoul(argv[6],nullptr,0)?P:0;
            f.target.words[0]=std::strtoul(argv[7],nullptr,0);f.radA=std::strtoul(argv[8],nullptr,0);f.radB=std::strtoul(argv[9],nullptr,0);
            f.debug=std::strtoul(argv[10],nullptr,0);f.debug2=std::strtoul(argv[11],nullptr,0);f.mutation=std::strtoul(argv[12],nullptr,0);f.generic=std::strtoul(argv[13],nullptr,0);
            const auto status=f.run(explicit_target?P:0);auto& r=f.result;
            std::cout<<"{\"status\":"<<int(status)<<",\"value\":"<<r.value<<",\"distance\":"<<r.distance_word<<",\"sum\":"<<r.sum_word
                <<",\"square\":"<<r.square_word<<",\"owner\":"<<f.state.owner<<",\"target40\":"<<f.state.target_40<<",\"calls\":";dumpCalls(f.calls);std::cout<<"}\n";return 0;
        }
        if(std::string(argv[1])=="radius") {
            assert(argc==6);RadiusFixture f;f.equipment=std::strtoul(argv[2],nullptr,0);f.rows[8].words[8]=std::strtoul(argv[3],nullptr,0);
            f.id=std::strtoul(argv[4],nullptr,0);f.mutation=std::strtoul(argv[5],nullptr,0);
            const auto status=f.run();std::cout<<"{\"status\":"<<int(status)<<",\"value\":"<<f.result.value_word<<",\"owner\":"<<f.state.owner<<",\"calls\":";
            dumpCalls(f.calls);std::cout<<"}\n";return 0;
        }return 2;
    }
    unsigned cases=0;
    {Fixture f;assert(f.run()==k::Status::complete && f.result.value==1);++cases;}
    {Fixture f;f.radA=f.radB=word(1.5);assert(f.run()==k::Status::complete && !f.result.value);++cases;}
    {Fixture f;f.state.target_40=0;assert(f.run(0)==k::Status::complete && f.calls.empty());++cases;}
    {Fixture f;assert(f.run(0)==k::Status::complete && f.result.value);++cases;}
    for(unsigned branch=0;branch<3;++branch){Fixture f;if(branch==0)f.resolving=0;if(branch==1)f.resolved.word_f4=1;if(branch==2)f.type=7;
        assert(f.run()==k::Status::complete && f.result.used_interaction_range && f.result.value==7);++cases;}
    for(unsigned i=1;i<=7;++i){Fixture f;f.mutation=i;f.debug=1;assert(f.run()==k::Status::complete);++cases;}
    {Fixture f;f.mutation=8;assert(f.run()==k::Status::invalid_source_fact);++cases;}
    for(unsigned i=1;i<=7;++i){Fixture f;f.fail=i;assert(f.run()==k::Status::service_failed && f.calls.size()==i);++cases;}
    {Fixture f;f.throws=7;assert(f.run()==k::Status::service_failed);++cases;}
    {Fixture f;f.debug=1;f.fail=8;assert(f.run()==k::Status::service_failed && f.result.sum_word==word(4) && f.calls.size()==8);++cases;}
    {Fixture f;f.debug=1;f.throws=8;assert(f.run()==k::Status::service_failed && f.result.sum_word==word(4));++cases;}
    {Fixture f;f.mutation=9;f.target.words[0]=word(1);assert(f.run()==k::Status::complete && f.result.distance_word==word(14));++cases;}
    {Fixture f;f.services.invoke=nullptr;assert(f.run()==k::Status::service_unavailable);++cases;}
    {Fixture f;assert(k::evaluate_object(&f.state,P,&f.services,reinterpret_cast<k::Result*>(&f.state))==k::Status::invalid_argument && f.calls.empty());++cases;}
    for(unsigned i=0;i<=5;++i){RadiusFixture f;f.mutation=i;assert(f.run()==k::Status::complete);++cases;}
    for(unsigned i=1;i<=3;++i){RadiusFixture f;f.fail=i;assert(f.run()==k::Status::service_failed && f.calls.size()==i);++cases;}
    {RadiusFixture f;f.id=9;assert(f.run()==k::Status::invalid_source_fact);++cases;}
    {RadiusFixture f;f.table.rows=reinterpret_cast<k::AiRow*>(&f.result);assert(f.run()==k::Status::invalid_source_fact);++cases;}
    // Actual composition uses the new original radius orchestration for BOTH
    // the original AI and resolved Character AI, not independent numeric copies.
    {Fixture f;RadiusFixture a,b;a.equipment=1;b.equipment=2;
        struct Context{Fixture* f;RadiusFixture* a;RadiusFixture* b;};Context context{&f,&a,&b};
        f.services.context=&context;f.services.invoke=[](void* p,k::State* s,const k::Request* q,k::Response* r)->int {
            auto& c=*static_cast<Context*>(p);const auto status=Fixture::invoke(c.f,s,q,r);
            if(status || q->operation!=k::Operation::melee_radius)return status;
            auto* radius=q->kind==k::Subject::original_ai?c.a:c.b;
            if(radius->run()!=k::Status::complete)return 1;
            r->word=radius->result.value_word;return 0;
        };
        assert(f.run()==k::Status::complete && f.result.owner_radius_word==word(3) && f.result.target_radius_word==word(4));++cases;
    }
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"live_points_and_table\":true,\"fixed_candidate_resolved_radius\":true,\"radius_composition\":true,\"failure_alias_contracts\":true}\n";
}
