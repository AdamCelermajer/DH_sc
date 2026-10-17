#include "../character_dot_tick.hpp"
#include <array>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace dt=dh2::character_dot_tick;
constexpr std::uintptr_t C=0x10014000, B=0x10018000, P=0x10014560, S=0x10014ff4;
using Trace=std::array<std::int64_t,7>;
void check(bool ok,const char* message) { if(!ok)throw std::runtime_error(message); }
struct Fixture {
    dt::Owner owner{C};dt::State state{P,S,&owner};
    std::array<std::uint32_t,6> values{{1,2,3,4,5,6}},dead{};
    std::vector<Trace> trace;
    unsigned mutation=0,fail_at=0,throw_at=0,index=0;
    unsigned produced=0,applied=0;
    dh2::data::CombatResult* result_address=nullptr;
    dt::Result report{};
    dt::Status status=dt::Status::complete;
    dt::Services services{this,read,query,attack,apply};
    bool mutate(unsigned operation) {
        if(index==0) {
            if(mutation==1&&operation==0){owner.character=B;values[0]=800;values[1]=501;}
            if(mutation==2&&operation==1){owner.character=B;values[0]=902;}
            if(mutation==3&&operation==2){owner.character=B;values[1]=303;}
            if(mutation==4&&operation==3){owner.character=B;values[1]=404;}
            if(mutation==5&&operation==0){values[1]=0x80000000u;values[2]=777;}
            if(mutation==6&&operation==1)dead[1]=7;
            if(mutation==7&&operation==2){owner.character=B;values[5]=1555;}
            if(mutation==8&&operation==3){owner.character=B;values[5]=1666;}
            if(mutation==9&&operation==0)owner.character=0;
            if(mutation==10&&operation==1)owner.character=0;
            if(mutation==11&&operation==2)owner.character=0;
        }
        if(throw_at==trace.size())throw std::runtime_error("provider failure");
        return fail_at==trace.size();
    }
    static std::int32_t read(void* context,const dt::ReadRequest* request,std::uint32_t* value) {
        auto& f=*static_cast<Fixture*>(context);check(request->property>=126&&request->property<132,"property domain");
        check(request->properties==f.state.properties&&request->sheet==f.state.resolved_sheet,"captured sheet");
        f.index=request->property-126;*value=f.values[f.index];
        f.trace.push_back({0,std::int64_t(request->properties),std::int64_t(request->sheet),request->property,0,-1,*value});
        return f.mutate(0)?1:0;
    }
    static std::int32_t query(void* context,std::uintptr_t character,std::uint32_t* value) {
        auto& f=*static_cast<Fixture*>(context);check(character==f.owner.character,"fresh query owner");*value=f.dead[f.index];
        f.trace.push_back({1,std::int64_t(character),0,126+f.index,0,-1,*value});return f.mutate(1)?1:0;
    }
    static std::int32_t attack(void* context,const dt::AttackRequest* request) {
        auto& f=*static_cast<Fixture*>(context);
        check(request->attacker==f.owner.character&&request->defender==request->attacker,"fresh self attack");
        check(request->element==std::int32_t(f.index)-1,"element domain");
        if(f.result_address)check(f.result_address==request->result,"same local result storage");
        f.result_address=request->result;
        // Named combat-provider fixture, not a substitute damage calculation.
        std::int32_t amount;std::memcpy(&amount,&request->amount,4);
        *request->result={amount,request->element,123,456,7,8,9,0x20080000u,-1,request->element};
        ++f.produced;f.trace.push_back({2,std::int64_t(request->attacker),std::int64_t(request->defender),126+f.index,request->amount,request->element,0});
        return f.mutate(2)?1:0;
    }
    static std::int32_t apply(void* context,const dt::ApplyRequest* request) {
        auto& f=*static_cast<Fixture*>(context);
        check(request->attacker==f.owner.character&&request->defender==request->attacker,"fresh self apply");
        check(request->mode==0&&request->result==f.result_address,"captured dead word and result identity");
        check(request->result->dot_duration==123&&request->result->dot_amount==456&&request->result->hp_leech==7&&
              request->result->mp_leech==8&&request->result->outcomes==9&&request->result->mask==0x20080000u&&
              request->result->weapon_category==-1&&request->result->element==std::int32_t(f.index)-1,"complete provider result forwarded");
        ++f.applied;f.trace.push_back({3,std::int64_t(request->attacker),std::int64_t(request->defender),126+f.index,request->mode,request->result->element,std::uint32_t(request->result->amount)});
        request->result->outcomes=0xfeedu;return f.mutate(3)?1:0;
    }
    void run(){status=dt::tick(&state,&services,&report);}
};
void print(const Fixture& f) {
    const auto& r=f.report;
    std::cout<<"{\"status\":"<<std::int32_t(f.status)<<",\"calls\":"<<r.calls<<",\"last_operation\":"<<r.last_operation
             <<",\"property_reads\":"<<r.property_reads<<",\"positive_properties\":"<<r.positive_properties
             <<",\"dead_skips\":"<<r.dead_skips<<",\"attacks\":"<<r.attacks<<",\"applications\":"<<r.applications
             <<",\"completed_properties\":"<<r.completed_properties<<",\"last_property\":"<<r.last_property
             <<",\"captured_amount\":"<<r.captured_amount<<",\"owner\":"<<f.owner.character<<",\"values\":[";
    for(unsigned i=0;i<6;++i)std::cout<<(i?",":"")<<f.values[i];
    std::cout<<"],\"dead\":[";for(unsigned i=0;i<6;++i)std::cout<<(i?",":"")<<f.dead[i];
    std::cout<<"],\"produced\":"<<f.produced<<",\"applied\":"<<f.applied<<",\"trace\":[";
    for(unsigned i=0;i<f.trace.size();++i){std::cout<<(i?",":"")<<'[';for(unsigned j=0;j<7;++j)std::cout<<(j?",":"")<<f.trace[i][j];std::cout<<']';}
    std::cout<<"]}\n";
}
unsigned host_cases=0,guard_cases=0,failure_cases=0;
void host() {
    {Fixture f;f.run();check(f.status==dt::Status::complete&&f.trace.size()==24&&f.report.applications==6&&f.report.completed_properties==6,"all positive");++host_cases;}
    {Fixture f;f.values={0,0xffffffffu,0x80000000u,0,0xffffffffu,0x80000000u};f.owner.character=0;f.services.is_dead=nullptr;f.services.dot_attack=nullptr;f.services.apply_result=nullptr;f.run();check(f.status==dt::Status::complete&&f.trace.size()==6&&f.report.positive_properties==0,"nonpositive needs no owner/combat provider");++host_cases;}
    for(unsigned i=0;i<6;++i)for(auto dead:{0u,1u,0x80000000u,0xffffffffu}) {
        Fixture f;f.values.fill(0);f.values[i]=0x7fffffffu;f.dead[i]=dead;f.run();
        check(f.status==dt::Status::complete&&f.report.positive_properties==1&&f.report.dead_skips==unsigned(dead!=0)&&f.report.applications==unsigned(!dead),"raw dead and element");++host_cases;
    }
    for(unsigned m=1;m<=8;++m){Fixture f;f.mutation=m;f.run();check(f.status==dt::Status::complete&&f.report.property_reads==6,"live mutation");++host_cases;}
    for(unsigned m=9;m<=11;++m){Fixture f;f.mutation=m;f.run();check(f.status==dt::Status::invalid_argument&&f.trace.size()==m-8,"fresh null owner bound");++guard_cases;}
    for(unsigned fail=1;fail<=24;++fail){Fixture f;f.fail_at=fail;f.run();check(f.status==dt::Status::service_failed&&f.trace.size()==fail&&f.report.calls==fail,"no continuation after returned failure");++failure_cases;}
    for(unsigned fail=1;fail<=24;++fail){Fixture f;f.throw_at=fail;f.run();check(f.status==dt::Status::service_failed&&f.trace.size()==fail&&f.report.calls==fail,"no continuation after exception");++failure_cases;}
    for(unsigned missing=0;missing<4;++missing){Fixture f;
        if(missing==0)f.services.read_property=nullptr;
        if(missing==1)f.services.is_dead=nullptr;
        if(missing==2)f.services.dot_attack=nullptr;
        if(missing==3)f.services.apply_result=nullptr;
        f.run();check(f.status==dt::Status::service_unavailable&&f.trace.size()==missing,"reached provider missing");++guard_cases;
    }
    {Fixture f;f.state.properties+=0x100000000ull;f.state.resolved_sheet+=0x100000000ull;f.owner.character+=0x100000000ull;f.run();check(f.status==dt::Status::complete&&f.trace[1][1]>0xffffffffll,"native owner above4GiB");++host_cases;}
    {Fixture outer,inner;auto callback=+[](void* p,const dt::ReadRequest* r,std::uint32_t* out)->std::int32_t{
        auto* pair=static_cast<std::array<Fixture*,2>*>(p);auto& a=*(*pair)[0];auto& b=*(*pair)[1];
        if(r->property==126){b.run();check(b.status==dt::Status::complete,"independent nested owner");}
        return Fixture::read(&a,r,out);
    };std::array<Fixture*,2> pair{&outer,&inner};outer.services={&pair,callback,nullptr,nullptr,nullptr};outer.values.fill(0);outer.run();check(outer.status==dt::Status::complete&&inner.trace.size()==24,"nested independent caller");++host_cases;}
    {Fixture f;const auto before=f.report;check(dt::tick(nullptr,&f.services,&f.report)==dt::Status::invalid_argument&&std::memcmp(&before,&f.report,sizeof before)==0,"null preflight");++guard_cases;}
    {Fixture f;check(dt::tick(&f.state,nullptr,&f.report)==dt::Status::invalid_argument&&f.trace.empty(),"missing services control");++guard_cases;}
    {Fixture f;check(dt::tick(&f.state,&f.services,nullptr)==dt::Status::invalid_argument&&f.trace.empty(),"missing result control");++guard_cases;}
    {Fixture f;f.state.owner=nullptr;f.run();check(f.status==dt::Status::invalid_argument&&f.trace.empty(),"missing owner control");++guard_cases;}
    for(unsigned which=0;which<3;++which){Fixture f;alignas(dt::Result) std::array<unsigned char,256> memory{};
        if(which==0)check(dt::tick(reinterpret_cast<const dt::State*>(memory.data()+1),&f.services,&f.report)==dt::Status::invalid_argument,"state alignment");
        if(which==1)check(dt::tick(&f.state,reinterpret_cast<const dt::Services*>(memory.data()+1),&f.report)==dt::Status::invalid_argument,"services alignment");
        if(which==2)check(dt::tick(&f.state,&f.services,reinterpret_cast<dt::Result*>(memory.data()+1))==dt::Status::invalid_argument,"result alignment");
        check(f.trace.empty(),"alignment no effects");++guard_cases;
    }
    {Fixture f;f.state.owner=reinterpret_cast<dt::Owner*>(&f.state);f.run();check(f.status==dt::Status::invalid_argument&&f.trace.empty(),"owner/state overlap");++guard_cases;}
    {Fixture f;check(dt::tick(&f.state,&f.services,reinterpret_cast<dt::Result*>(&f.services))==dt::Status::invalid_argument&&f.trace.empty(),"services/output overlap");++guard_cases;}
    {Fixture f;f.state.properties=0;f.run();check(f.status==dt::Status::invalid_argument&&f.trace.empty(),"zero properties");++guard_cases;}
    {Fixture f;f.state.resolved_sheet=0;f.run();check(f.status==dt::Status::invalid_argument&&f.trace.empty(),"zero resolved sheet");++guard_cases;}
    {Fixture f;alignas(dt::Owner) unsigned char storage[32]{};f.state.owner=reinterpret_cast<dt::Owner*>(storage+1);f.run();check(f.status==dt::Status::invalid_argument&&f.trace.empty(),"owner alignment");++guard_cases;}
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<host_cases<<",\"guard_cases\":"<<guard_cases<<",\"failure_cases\":"<<failure_cases<<",\"mismatches\":0}\n";
}
int main(int argc,char** argv) {
    try {if(argc==14){Fixture f;for(unsigned i=0;i<6;++i)f.values[i]=std::uint32_t(std::stoull(argv[1+i]));for(unsigned i=0;i<6;++i)f.dead[i]=std::uint32_t(std::stoull(argv[7+i]));f.mutation=unsigned(std::stoul(argv[13]));f.run();print(f);}else {check(argc==1,"argument count");host();}return 0;}
    catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
