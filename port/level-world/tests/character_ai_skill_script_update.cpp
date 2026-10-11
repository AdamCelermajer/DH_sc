#include "../character_ai_skill_script_update.hpp"
#include <array>
#include <cassert>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>
namespace su=dh2::character_ai_skill_script_update;
struct Fixture {
    su::Character a{101,113},b{202,224};su::State state{0x1000,&a};
    su::Services services{this,call};su::Result result{};su::ValueVector range{};
    std::vector<std::uint64_t> storage;
    std::vector<std::array<std::uint64_t,5>> trace;
    unsigned error=0,count=0,mutation=0,fail=99,throwing=99,bad=0;
    bool live=false,nested=false,nested_ok=false;
    unsigned effects=0,alternate_calls=0;
    static std::int32_t alternate(void* p,su::State*,const su::Request*,su::ReturnValues*){++static_cast<Fixture*>(p)->alternate_calls;return 7;}
    void refresh(){range.begin=reinterpret_cast<std::uintptr_t>(storage.data());range.end=range.begin+storage.size()*sizeof(storage[0]);}
    static std::int32_t call(void* context,su::State* state,const su::Request* request,su::ReturnValues* values){
        auto& f=*static_cast<Fixture*>(context);const auto op=unsigned(request->operation);assert(state==&f.state&&request->skill==0x1000);
        std::uint64_t n=0;if(op==2)n=(request->last-request->first)/sizeof(f.storage[0]);
        f.trace.push_back({op,request->script,request->arguments?request->arguments-request->skill:0,n,op==1?1u:op==3?2u:0u});++f.effects;
        if(op==0){
            assert(!request->resource&&!request->function&&!f.live);f.live=true;f.storage.clear();f.refresh();
            *values={reinterpret_cast<std::uintptr_t>(&f.storage),0,&f.range};
            if(f.mutation&1)state->owner=&f.b;
            if(f.bad==1)values->resource=0;
            if(f.bad==2)state->owner=nullptr;
            if(f.bad==3)state->owner=reinterpret_cast<su::Character*>(&f.result);
            if(f.nested){Fixture inner;f.nested_ok=inner.run()==su::Status::complete;}
        }else{
            assert(request->resource==reinterpret_cast<std::uintptr_t>(&f.storage)&&f.live);
            if(op==1){
                assert(request->script==state->owner->lua_script&&request->arguments==0x100c&&std::string(request->function)=="SetSkill");
                values->error=f.error;f.storage.assign(f.count,0x1122334455667788ull);f.refresh();
                if(f.mutation&2)state->owner=&f.b;
                if(f.mutation&4)state->owner->lua_script=state->owner->lua_script==113?224:113;
                if(f.mutation&32)state->identity=0x9000;
                if(f.mutation&64)f.services.invoke=alternate;
                if(f.mutation&128)f.services.context=nullptr;
                if(f.bad==4)values->values=nullptr;
                if(f.bad==5)values->values=reinterpret_cast<su::ValueVector*>(&f.result);
                if(f.bad==6)values->resource=7;
                if(f.bad==7)state->owner=nullptr;
                if(f.bad==8)state->owner->lua_script=0;
                if(f.bad==9)f.range={7,0};
                if(f.bad==10)f.range={9,7};
                if(f.bad==11)values->values=reinterpret_cast<su::ValueVector*>(reinterpret_cast<unsigned char*>(&f.range)+1);
                if(f.bad==16)values->values=reinterpret_cast<su::ValueVector*>(values);
            }else if(op==2){
                assert(request->first==f.range.begin&&request->last==f.range.end&&n==f.storage.size()&&n!=0&&!request->function);
                f.storage.clear();f.refresh();
                if(f.mutation&8)state->owner=&f.b;
                if(f.mutation&16)state->owner->lua_script=state->owner->lua_script==113?224:113;
                if(f.bad==12)state->owner=nullptr;
                if(f.bad==13)state->owner->lua_script=0;
                if(f.bad==14)values->resource=7;
            }else if(op==3||op==5){
                assert(request->script==state->owner->lua_script&&!request->arguments&&
                    std::string(request->function)==(op==3?"OnSkillUpdate":"OnSkillCleanUp")&&
                    f.storage.empty());
                // Source doesn't check this second Call's script error code.
                values->error=0x80000007u;f.storage.push_back(123);f.refresh();
                if(f.bad==15)values->resource=7;
            }else{
                assert(op==4&&!request->function);f.storage.clear();f.refresh();f.live=false;values->resource=0;
            }
        }
        if(f.throwing==op)throw std::runtime_error("provider failure after real fixture effects");
        return f.fail==op?7:0;
    }
    su::Status run(bool cleanup=false){return cleanup?su::cleanup(&state,&services,&result):su::update(&state,&services,&result);}
};
void print(const Fixture& f,su::Status status){
    std::cout<<"{\"status\":"<<int(status)<<",\"phase\":"<<unsigned(f.result.phase)<<",\"decision\":"<<unsigned(f.result.decision)<<",\"calls\":"<<f.result.service_calls<<",\"set_error\":"<<f.result.set_skill_error<<",\"erased\":"<<f.result.erased<<",\"updated\":"<<f.result.updated<<",\"constructed\":"<<f.result.constructed<<",\"destroyed\":"<<f.result.destroyed<<",\"trace\":[";
    for(unsigned i=0;i<f.trace.size();++i){if(i)std::cout<<',';std::cout<<'[';for(unsigned j=0;j<5;++j){if(j)std::cout<<',';std::cout<<f.trace[i][j];}std::cout<<']';}std::cout<<"]}\n";
}
int main(int argc,char** argv){
    if(argc>1){assert(argc==5);Fixture f;const auto script=std::stoul(argv[1]);f.a.lua_script=script==0?0:script==1?113:224;f.error=unsigned(std::stoull(argv[2]));f.count=unsigned(std::stoul(argv[3]));f.mutation=unsigned(std::stoul(argv[4]));const auto status=f.run();print(f,status);return 0;}
    unsigned cases=0,failures=0,guards=0,boundaries=0;
    for(unsigned script=0;script<3;++script)for(auto error:std::array<unsigned,3>{0,7,0xffffffffu})for(unsigned count:std::array<unsigned,3>{0,1,3})for(unsigned mutation:std::array<unsigned,8>{0,1,2,4,8,16,31,255}){
        Fixture f;f.a.lua_script=script==0?0:script==1?113:224;f.error=error;f.count=count;f.mutation=mutation;
        assert(f.run()==su::Status::complete&&f.result.constructed==1&&f.result.destroyed==1&&!f.live&&f.storage.empty());
        const bool has_script=script!=0||(mutation&1),updates=has_script&&!error;
        assert(f.result.updated==unsigned(updates)&&f.result.erased==unsigned(updates&&count));
        assert(f.result.decision==(has_script?(error?su::Decision::set_skill_error:su::Decision::updated):su::Decision::no_script));
        assert(f.result.service_calls==f.trace.size()&&f.alternate_calls==0);++cases;
    }
    for(unsigned has_script=0;has_script<2;++has_script)for(unsigned error:std::array<unsigned,2>{0,7})
      for(unsigned count:std::array<unsigned,2>{0,2}){
        Fixture f;f.a.lua_script=has_script?113:0;f.error=error;f.count=count;
        assert(f.run(true)==su::Status::complete&&f.result.constructed==1&&
            f.result.destroyed==1&&!f.live&&f.storage.empty());
        const bool cleaned=has_script&&!error;
        assert(f.result.cleaned==unsigned(cleaned)&&f.result.updated==0&&
            f.result.erased==unsigned(cleaned&&count)&&
            f.result.decision==(has_script?(error?su::Decision::set_skill_error:
                                                    su::Decision::cleaned):su::Decision::no_script));
        if(cleaned)assert(f.trace[f.trace.size()-2][0]==5);
        ++cases;
      }
    {Fixture f;f.nested=true;assert(f.run()==su::Status::complete&&f.nested_ok);++cases;}
    for(unsigned op=0;op<6;++op)for(unsigned throwing=0;throwing<2;++throwing){
        Fixture f;f.count=3;if(throwing)f.throwing=op;else f.fail=op;
        assert((op==5?f.run(true):f.run())==su::Status::service_failed&&f.trace.back()[0]==op&&f.effects==f.trace.size());
        assert(f.result.service_calls==f.trace.size()&&f.result.destroyed==0&&
               f.live==(op!=4));
        // No added destructor after failure or exception. Earlier erase/update
        // effects and provider-created resources remain observable.
        assert(f.trace.size()==(op==5?4u:op+1));++failures;
    }
    {Fixture f;f.services.invoke=nullptr;assert(f.run()==su::Status::service_unavailable&&f.result.service_calls==0&&f.result.phase==su::Phase::construct);++failures;}
    for(unsigned bad=1;bad<=16;++bad){Fixture f;f.count=3;f.bad=bad;assert(f.run()==su::Status::invalid_source_fact&&f.result.destroyed==0&&f.live);++boundaries;}
    // Error return skips owner/vector reads after SetSkill, so even replaced
    // null owners/invalid vector projections don't invent a cleanup gate.
    for(unsigned bad:std::array<unsigned,4>{4,5,7,8}){Fixture f;f.count=3;f.error=7;f.bad=bad;assert(f.run()==su::Status::complete&&f.result.decision==su::Decision::set_skill_error&&!f.live);++cases;}
    Fixture f;f.result.service_calls=0xa5a5a5a5u;
    auto invalid=[&](su::State* state,const su::Services* services,su::Result* result){assert(su::update(state,services,result)==su::Status::invalid_argument);assert(f.trace.empty()&&f.result.service_calls==0xa5a5a5a5u);++guards;};
    invalid(nullptr,&f.services,&f.result);invalid(&f.state,nullptr,&f.result);invalid(&f.state,&f.services,nullptr);
    alignas(16)std::array<unsigned char,128> bytes{};auto* bad=bytes.data()+1;
    invalid(reinterpret_cast<su::State*>(bad),&f.services,&f.result);invalid(&f.state,reinterpret_cast<const su::Services*>(bad),&f.result);invalid(&f.state,&f.services,reinterpret_cast<su::Result*>(bad));
    invalid(&f.state,reinterpret_cast<const su::Services*>(&f.state),&f.result);invalid(&f.state,&f.services,reinterpret_cast<su::Result*>(&f.state));invalid(&f.state,&f.services,reinterpret_cast<su::Result*>(&f.services));
    {su::State zero{0,&f.a};invalid(&zero,&f.services,&f.result);}
    {su::State overflow{std::numeric_limits<std::uintptr_t>::max()-11,&f.a};invalid(&overflow,&f.services,&f.result);}
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"failure_cases\":"<<failures<<",\"source_boundary_cases\":"<<boundaries<<",\"guard_cases\":"<<guards<<",\"mismatches\":0}\n";
}
