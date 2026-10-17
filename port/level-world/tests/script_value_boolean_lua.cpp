#include "../script_value_boolean_lua.hpp"
extern "C" {
#include "lua.h"
}
#include <cassert>
#include <iostream>
#include <string>
namespace vb=dh2::script_value_boolean;namespace vl=dh2::script_value_boolean_lua;
struct Wrapper {
    vl::TemporaryLua lua;vb::Services actual=lua.services();vb::Value value{4,0,0,"0"};vb::Result result{};
    unsigned fail=99,mutation=0;
    static std::int32_t call(void* context,const vb::Value* value,const vb::Request* q,vb::Response* reply){
        auto& w=*static_cast<Wrapper*>(context);const auto status=w.actual.invoke(w.actual.context,value,q,reply);if(status)return status;
        if(q->operation==vb::Operation::new_state&&w.mutation==1)w.value.string=nullptr;
        if(q->operation==vb::Operation::new_state&&w.mutation==2)w.value.string="";
        return unsigned(q->operation)==w.fail?7:0;
    }
    vb::Status run(){const vb::Services wrapped{this,call};return vb::execute(&value,&wrapped,&result);}
};
int main(){
    unsigned cases=0,failures=0;
    vl::TemporaryLua adapter;const auto services=adapter.services();
    const char embedded[]={0,'h','i',0};
    for(const char* text:{static_cast<const char*>(nullptr),"","0","false","false\0tail",embedded}){
        vb::Value value{4,0,0,text};vb::Result r{};
        assert(vb::execute(&value,&services,&r)==vb::Status::complete&&r.value==unsigned(text!=nullptr)&&r.closed==1&&r.service_calls==4);++cases;
    }
    std::string long_text(262144,'x');vb::Value long_value{4,0,0,long_text.c_str()};vb::Result long_result{};
    assert(vb::execute(&long_value,&services,&long_result)==vb::Status::complete&&long_result.value==1&&long_result.closed);++cases;
    for(unsigned i=0;i<32;++i){vb::Value value{4,0,0,i%2?nullptr:""};vb::Result r{};assert(vb::execute(&value,&services,&r)==vb::Status::complete&&r.value==(i%2==0));++cases;}
    const auto stats=adapter.statistics();assert(stats.created==cases&&stats.pushed==cases&&stats.queried==cases&&stats.closed==cases&&stats.live==0);
    for(unsigned mutation=1;mutation<=2;++mutation){Wrapper w;w.mutation=mutation;assert(w.run()==vb::Status::complete&&w.result.value==(mutation==2)&&w.lua.statistics().live==0);++cases;}
    for(unsigned phase=0;phase<4;++phase){
        Wrapper w;w.fail=phase;assert(w.run()==vb::Status::service_failed&&w.result.closed==0);
        const auto s=w.lua.statistics();assert(s.created==1&&s.live==(phase!=3)&&s.closed==(phase==3));
        if(phase!=3){
            // Failed source call did not add close; the retained real state can
            // be explicitly recovered by the outer owner using the same handle.
            const auto handle=w.result.lua;
            if(phase>0){vb::Request close{vb::Operation::close_state,handle,nullptr,0};vb::Response reply{};assert(w.actual.invoke(w.actual.context,&w.value,&close,&reply)==0&&w.lua.statistics().live==0);}
            // phase0 handle is retained only by adapter ownership registry; its
            // destructor performs outer teardown, not source per-call cleanup.
        }
        ++failures;
    }
    vb::Value value{4,0,0,""};vb::Response reply{};
    vb::Request unknown{static_cast<vb::Operation>(99),17,nullptr,0};assert(services.invoke(services.context,&value,&unknown,&reply)!=0);++failures;
    // Real temporary VM has no libraries opened. This also checks ownership
    // before direct source C calls, not a stand-in global getter.
    vb::Request create{vb::Operation::new_state,0,nullptr,0};assert(services.invoke(services.context,&value,&create,&reply)==0);
    auto* L=reinterpret_cast<lua_State*>(reply.lua);lua_getglobal(L,"print");assert(lua_isnil(L,-1));lua_pop(L,1);
    vb::Request bad_index{vb::Operation::to_boolean,reinterpret_cast<std::uintptr_t>(L),nullptr,0};assert(services.invoke(services.context,&value,&bad_index,&reply)!=0);++failures;
    vb::Request close{vb::Operation::close_state,reinterpret_cast<std::uintptr_t>(L),nullptr,0};assert(services.invoke(services.context,&value,&close,&reply)==0&&adapter.statistics().live==0);++cases;
    std::cout<<"{\"validation\":\"PASS\",\"real_lua_cases\":"<<cases<<",\"failure_cases\":"<<failures<<",\"temporary_resources_retained_on_error\":true,\"no_libraries_opened\":true,\"mismatches\":0}\n";
}
