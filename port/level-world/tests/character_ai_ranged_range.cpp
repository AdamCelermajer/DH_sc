#include "../character_ai_ranged_range.hpp"
#include "../character_ai_interaction_range.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>
namespace k=dh2::character_ai_ranged_range;
constexpr std::uintptr_t A=0x10014000,B=0x10018000,P=0x10020000,R=0x10028000,AI=A+0x3c8,D=0x10038000,N=0x10039000;
unsigned word(float x){unsigned r;std::memcpy(&r,&x,4);return r;}
unsigned asr8(unsigned x){return (x>>8)|((x&0x80000000)?0xff000000:0);}
struct Call {unsigned op;std::uintptr_t subject,other;unsigned key,alias;};
struct Fixture {
    k::State state{AI,A,P};k::ResolvedObject resolved{R,0};
    k::Point owner{{0,0,0}},target{{0x40400000,0,0}};
    unsigned resolving=1,type=8,capability=1,minimum=512,maximum=1280,projectile=4,
        debug=0,debug2=1,mutation=0,inventory=0,generic=7,fail=0,throws=0;
    std::uintptr_t global_debug=D;
    unsigned *min_output=nullptr,*max_output=nullptr;
    k::Result result{};std::vector<Call> calls;k::Services services{this,invoke};
    static int invoke(void* p,k::State* s,const k::Request* q,k::Response* out) {
        auto& f=*static_cast<Fixture*>(p);
        const auto alias=q->operation==k::Operation::can_range_attack?unsigned(q->maximum==q->projectile):0;
        f.calls.push_back({unsigned(q->operation),q->subject,q->other,q->key,alias});
        if(f.fail==f.calls.size())return 9;
        if(f.throws==f.calls.size())throw std::runtime_error("provider");
        switch(q->operation) {
        case k::Operation::resolve_object:
            out->view=f.resolving?&f.resolved:nullptr;
            if(f.mutation==1)s->owner=B;
            if(f.mutation==15)out->view=&f.result;
            break;
        case k::Operation::interaction_type:
            out->word=f.type;if(f.mutation==2)s->owner=B;break;
        case k::Operation::can_range_attack:
            assert(q->minimum && q->maximum && q->projectile && q->minimum!=q->maximum);
            f.min_output=q->minimum;f.max_output=q->maximum;
            out->word=f.capability;
            if(f.capability) {
                *q->minimum=f.inventory?f.minimum:asr8(f.minimum);
                *q->maximum=f.inventory?f.maximum:asr8(f.maximum);
                *q->projectile=f.projectile;
            }
            if(f.mutation==3)s->owner=B;
            if(f.mutation==16)s->owner=0;
            break;
        case k::Operation::target_position:
            out->view=q->subject==P?&f.target:&f.owner;
            if(q->subject!=P && f.mutation==4)s->owner=B;
            if(q->subject==P && f.mutation==5)f.owner.words[0]=word(1);
            if(f.mutation==6)s->target_40=0;
            if(q->subject==P && f.mutation==14)out->view=&f.result;
            if(q->subject==P && f.mutation==17)out->view=&f.owner;
            break;
        case k::Operation::diagnostic_switch:
            out->identity=q->key==1?f.global_debug:q->subject;
            out->word=q->key==1?f.debug:f.debug2;
            if(f.mutation==7){f.target.words[0]=word(100);f.owner.words[0]=word(100);}
            if(f.mutation==8)s->owner=B;
            if(f.mutation==9){f.minimum=0;f.maximum=0;}
            if(f.mutation==10)f.global_debug=N;
            if(f.mutation==11)*f.min_output=4;
            if(f.mutation==12)*f.max_output=2;
            if(f.mutation==18)out->identity=0;
            break;
        case k::Operation::interaction_range:out->word=f.generic;break;
        }
        return 0;
    }
    k::Status run(bool close,std::uintptr_t candidate=P) {
        if(mutation==13){target.words[1]=word(2);target.words[2]=word(3);}
        return close?k::evaluate_close(&state,candidate,&services,&result):k::evaluate_ranged(&state,candidate,&services,&result);
    }
};
void dumpCalls(const std::vector<Call>& calls) {
    std::cout<<'[';bool first=true;
    for(auto c:calls){if(!first)std::cout<<',';first=false;
        std::cout<<'['<<c.op<<','<<c.subject<<','<<c.other<<','<<c.key<<','<<c.alias<<']';}
    std::cout<<']';
}
int main(int argc,char** argv) {
    if(argc>1) {
        assert(argc==16);Fixture f;unsigned x[14];for(unsigned i=0;i<14;++i)x[i]=std::strtoul(argv[i+2],nullptr,0);
        f.resolving=x[0];f.resolved.word_f4=x[1];f.type=x[2];f.state.target_40=x[4]?P:0;
        f.capability=x[5];f.minimum=x[6];f.maximum=x[7];f.projectile=x[8];f.target.words[0]=x[9];
        f.debug=x[10];f.debug2=x[11];f.mutation=x[12];f.inventory=x[13];
        const auto status=f.run(std::string(argv[1])=="close",x[3]?P:0);const auto& r=f.result;
        std::cout<<"{\"status\":"<<int(status)<<",\"value\":"<<r.value<<",\"distance\":"<<r.distance_word
            <<",\"minimum\":"<<r.minimum_word<<",\"maximum\":"<<r.maximum_word<<",\"projectile\":"<<r.projectile_word
            <<",\"minimum_square\":"<<r.minimum_square_word<<",\"maximum_square\":"<<r.maximum_square_word
            <<",\"minimum_limit\":"<<r.minimum_limit_word<<",\"maximum_limit\":"<<r.maximum_limit_word
            <<",\"owner\":"<<f.state.owner<<",\"target40\":"<<f.state.target_40<<",\"debug_global\":"<<f.global_debug
            <<",\"calls\":";dumpCalls(f.calls);std::cout<<"}\n";return 0;
    }
    unsigned cases=0;
    for(bool close:{false,true}) {
        {Fixture f;assert(f.run(close)==k::Status::complete && f.result.value==unsigned(!close));++cases;}
        {Fixture f;f.minimum=768;assert(f.run(close)==k::Status::complete && f.result.value==unsigned(!close));++cases;}
        {Fixture f;f.maximum=768;assert(f.run(close)==k::Status::complete && f.result.value==unsigned(!close));++cases;}
        {Fixture f;f.minimum=1024;assert(f.run(close)==k::Status::complete && f.result.value==unsigned(close));++cases;}
        {Fixture f;f.state.target_40=0;assert(f.run(close,0)==k::Status::complete && f.calls.empty());++cases;}
        {Fixture f;assert(f.run(close,0)==k::Status::complete);++cases;}
        {Fixture f;f.capability=0;assert(f.run(close)==k::Status::complete && !f.result.value && f.result.distance_word==0);
            assert(f.calls.size()==unsigned(close?3:1));++cases;}
        for(unsigned mutation=1;mutation<=13;++mutation){Fixture f;f.mutation=mutation;f.debug=1;assert(f.run(close)==k::Status::complete);++cases;}
        {Fixture f;f.mutation=14;assert(f.run(close)==k::Status::invalid_source_fact);++cases;}
        {Fixture f;f.mutation=16;assert(f.run(close)==k::Status::invalid_source_fact);++cases;}
        {Fixture f;f.mutation=17;assert(f.run(close)==k::Status::complete && f.result.distance_word==0);++cases;}
        {Fixture f;f.mutation=18;assert(f.run(close)==k::Status::invalid_source_fact);++cases;}
        const auto count=close?7u:5u;
        for(unsigned i=1;i<=count;++i){Fixture f;f.debug=1;f.fail=i;assert(f.run(close)==k::Status::service_failed && f.calls.size()==i);++cases;}
        {Fixture f;f.debug=1;f.throws=count;assert(f.run(close)==k::Status::service_failed && f.result.distance_word==word(9));++cases;}
        {Fixture f;f.services.invoke=nullptr;assert(f.run(close)==k::Status::service_unavailable);++cases;}
        {Fixture f;assert(k::evaluate_close(&f.state,P,&f.services,reinterpret_cast<k::Result*>(&f.state))==k::Status::invalid_argument && f.calls.empty());++cases;}
        {Fixture f;alignas(k::State) unsigned char invalid[sizeof(k::State)+1]{};
            assert(k::evaluate_ranged(reinterpret_cast<k::State*>(invalid+1),P,&f.services,&f.result)==k::Status::invalid_argument);++cases;}
    }
    for(unsigned reason=0;reason<3;++reason){Fixture f;if(!reason)f.resolving=0;if(reason==1)f.resolved.word_f4=1;if(reason==2)f.type=7;
        assert(f.run(true)==k::Status::complete && f.result.used_interaction_range && f.result.value==7);++cases;}
    {Fixture f;f.mutation=15;assert(f.run(true)==k::Status::invalid_source_fact);++cases;}
    // Actual source interaction fallback is composed; it keeps original AI and
    // unconverted candidate, never the handle-resolved identity.
    {Fixture f;f.resolving=0;namespace g=dh2::character_ai_interaction_range;
        g::ObjectFacts facts{P,1};g::Point owner{{0,0,0}},spot{{word(3),0,0}};
        struct Context{Fixture* f;g::ObjectFacts* facts;g::Point* owner;g::Point* spot;};Context c{&f,&facts,&owner,&spot};
        f.services.context=&c;f.services.invoke=[](void* p,k::State* state,const k::Request* q,k::Response* r)->int {
            auto& context=*static_cast<Context*>(p);
            if(q->operation!=k::Operation::interaction_range)return Fixture::invoke(context.f,state,q,r);
            assert(q->subject==AI && q->other==P);
            g::Services services{&context,[](void* v,g::State*,const g::Request* request,g::Response* out)->int {
                auto& x=*static_cast<Context*>(v);
                if(request->operation==g::Operation::target_position)out->view=x.owner;
                else if(request->operation==g::Operation::interaction_spot){out->view=x.facts;out->point=*x.spot;}
                else if(request->operation==g::Operation::interaction_type)out->word=7;
                else return 1;
                return 0;
            }};g::Result result{};
            if(g::evaluate_object(state,P,&services,&result)!=g::Status::complete)return 1;
            r->word=result.value;return 0;
        };
        assert(f.run(true)==k::Status::complete && f.result.value==1 && f.result.used_interaction_range);++cases;
    }
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"output_alias_order\":true,\"inclusive_ranged_lower_and_upper\":true,\"strict_close_upper\":true,\"same_debug_identity\":true,\"interaction_composition\":true,\"failure_lifetime_guards\":true}\n";
}
