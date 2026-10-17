#include "../character_init_hp_mp.hpp"
#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
namespace im=dh2::character_init_hp_mp;
struct Fixture {
    im::Owner owner{101};im::Services services{this,hp,mp};im::Result report{};
    std::uint32_t mutation=0,fail=0,throwing=0,hp_effects=0,mp_effects=0,alternate_calls=0;
    std::vector<std::array<std::uint64_t,3>> trace;
    bool nested=false,nested_ok=false;
    static std::int32_t hp(void* context,const im::Request* q){return call(context,q,0);}
    static std::int32_t mp(void* context,const im::Request* q){return call(context,q,1);}
    static std::int32_t alternate(void* context,const im::Request*){++static_cast<Fixture*>(context)->alternate_calls;return 7;}
    static std::int32_t call(void* context,const im::Request* q,unsigned op){
        auto& f=*static_cast<Fixture*>(context);f.trace.push_back({op,q->character,q->raw_amount});assert(q->raw_amount==0xffffffffu);
        if(op==0){++f.hp_effects;if(f.mutation&1)f.owner.character=202;if(f.mutation&2)f.owner.character=0;if(f.mutation&4)f.services.regen_mp=alternate;if(f.mutation&8)f.services.context=nullptr;
            if(f.nested){Fixture inner;f.nested_ok=im::execute(&inner.owner,&inner.services,&inner.report)==im::Status::complete;}}
        else ++f.mp_effects;
        if(f.throwing==op+1)throw std::runtime_error("real provider failed after partial effect");
        return f.fail==op+1?7:0;
    }
    im::Status run(){return im::execute(&owner,&services,&report);}
};
void print(const Fixture& f,im::Status status){
    std::cout<<"{\"status\":"<<int(status)<<",\"captured\":"<<f.report.captured_character<<",\"calls\":"<<f.report.calls<<",\"last\":"<<f.report.last_operation<<",\"hp\":"<<f.report.hp_completed<<",\"mp\":"<<f.report.mp_completed<<",\"owner\":"<<f.owner.character<<",\"effects\":["<<f.hp_effects<<','<<f.mp_effects<<"],\"trace\":[";
    for(unsigned i=0;i<f.trace.size();++i){if(i)std::cout<<',';std::cout<<'['<<f.trace[i][0]<<','<<f.trace[i][1]<<','<<f.trace[i][2]<<']';}std::cout<<"]}\n";
}
int main(int argc,char** argv){
    if(argc>1){assert(argc==3);Fixture f;f.owner.character=std::stoull(argv[1]);f.mutation=std::uint32_t(std::stoull(argv[2]));const auto s=f.run();print(f,s);return 0;}
    unsigned cases=0,guards=0,failures=0;
    for(unsigned mutation=0;mutation<16;++mutation){Fixture f;f.mutation=mutation;assert(f.run()==im::Status::complete);assert(f.report.calls==2&&f.report.hp_completed==1&&f.report.mp_completed==1);assert(f.trace[0][1]==101&&f.trace[1][1]==101);assert(f.alternate_calls==0);++cases;}
    {Fixture f;f.nested=true;assert(f.run()==im::Status::complete&&f.nested_ok);++cases;}
    for(unsigned phase=1;phase<=2;++phase)for(unsigned throwing=0;throwing<2;++throwing){Fixture f;if(throwing)f.throwing=phase;else f.fail=phase;assert(f.run()==im::Status::service_failed);assert(f.trace.size()==phase&&f.report.calls==phase);assert(f.hp_effects==1&&f.mp_effects==(phase==2));assert(f.report.hp_completed==(phase==2)&&f.report.mp_completed==0);++failures;}
    {Fixture f;f.services.regen_hp=nullptr;assert(f.run()==im::Status::service_unavailable&&f.report.calls==0&&f.hp_effects==0&&f.mp_effects==0);++failures;}
    {Fixture f;f.services.regen_mp=nullptr;assert(f.run()==im::Status::service_unavailable&&f.report.calls==1&&f.report.hp_completed==1&&f.report.last_operation==1&&f.hp_effects==1&&f.mp_effects==0);++failures;}
    Fixture f;
    auto invalid=[&](const im::Owner* o,const im::Services* s,im::Result* r){assert(im::execute(o,s,r)==im::Status::invalid_argument);assert(f.trace.empty());++guards;};
    invalid(nullptr,&f.services,&f.report);invalid(&f.owner,nullptr,&f.report);invalid(&f.owner,&f.services,nullptr);
    alignas(16)std::array<unsigned char,128> raw{};auto* bad=raw.data()+1;
    invalid(reinterpret_cast<const im::Owner*>(bad),&f.services,&f.report);invalid(&f.owner,reinterpret_cast<const im::Services*>(bad),&f.report);invalid(&f.owner,&f.services,reinterpret_cast<im::Result*>(bad));
    invalid(&f.owner,reinterpret_cast<const im::Services*>(&f.owner),&f.report);invalid(&f.owner,&f.services,reinterpret_cast<im::Result*>(&f.owner));invalid(&f.owner,&f.services,reinterpret_cast<im::Result*>(&f.services));
    {im::Owner zero{};invalid(&zero,&f.services,&f.report);}
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"guard_cases\":"<<guards<<",\"failure_cases\":"<<failures<<",\"mismatches\":0}\n";
}
