#include "../ais_player_init_vcb.hpp"
#include "../../adam-script-runtime/script_function_alias.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
namespace k=dh2::ais_player_init_vcb;
constexpr const char* names[]={"OnTargetHit","OnTargetMissed","OnKill"};
struct Call {unsigned index,flags,member;};
struct Fixture {
    k::State state{sizeof(std::uintptr_t)==8?std::uintptr_t(0x100014000ull):0x10014000u,0xfefefefe};
    k::Result result{};k::Services services{this,contains};
    unsigned membership=0,mutation=0,fail=0,throws=0;
    dh2_script_aliases* aliases=nullptr;std::vector<dh2_script_aliases*> retired;std::vector<Call> calls;
    explicit Fixture(unsigned mask=0){replace(mask);}
    ~Fixture(){dh2_script_alias_destroy(aliases);for(auto* old:retired)dh2_script_alias_destroy(old);}
    void replace(unsigned mask) {
        auto* next=dh2_script_alias_create();assert(next);
        for(unsigned i=0;i<3;++i)if(mask&(1u<<i))assert(dh2_script_alias_add(next,names[i],"fixture_alias")==0);
        if(aliases)retired.push_back(aliases);
        aliases=next;membership=mask;
    }
    void mutate(unsigned index) {
        if(mutation==1)state.flags_b8=0xbeef0000u+index;
        if(mutation==2 && index==0)replace(membership^2);
        if(mutation==3 && index==1)replace(membership^4);
        if(mutation==4 && index==2)replace(0);
        if(mutation==5 && index==0)replace(7);
        if(mutation==6)replace(membership^(1u<<((index+1)%3)));
    }
    static int contains(void* raw,k::State* state,const char* name,bool* member) {
        auto& f=*static_cast<Fixture*>(raw);assert(state==&f.state);
        assert(state->ais==(sizeof(std::uintptr_t)==8?std::uintptr_t(0x100014000ull):0x10014000u));
        unsigned index=0;while(index<3 && std::strcmp(name,names[index]))++index;assert(index<3);
        const auto present=dh2_script_alias_contains(f.aliases,name);assert(present==0 || present==1);
        f.calls.push_back({index,state->flags_b8,unsigned(present)});f.mutate(index);
        if(f.fail==f.calls.size())return 9;
        if(f.throws==f.calls.size())throw std::runtime_error("provider");
        *member=present!=0;return 0;
    }
    k::Status run(){return k::initialize(&state,&services,&result);}
};
std::string read(const char* path){std::ifstream f(path,std::ios::binary);assert(f);return {std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>()};}
unsigned expected(unsigned mask){return (mask&1?0x800:0)|(mask&2?0x1000:0)|(mask&4?0x400:0);}
int main(int argc,char** argv) {
    if(argc>1 && !std::strcmp(argv[1],"oracle")) {
        assert(argc==5);Fixture f(std::strtoul(argv[2],nullptr,0));f.mutation=std::strtoul(argv[3],nullptr,0);f.state.flags_b8=std::strtoul(argv[4],nullptr,0);
        const auto status=f.run();std::cout<<"{\"status\":"<<unsigned(status)<<",\"flags\":"<<f.state.flags_b8<<",\"writes\":"<<f.result.writes<<",\"calls\":[";
        bool first=true;for(auto c:f.calls){if(!first)std::cout<<',';first=false;std::cout<<'['<<c.index<<','<<c.flags<<','<<c.member<<']';}std::cout<<"]}\n";return 0;
    }
    assert(argc==2);unsigned cases=0,guards=0,failures=0,lua=0;
    for(unsigned mask=0;mask<8;++mask){Fixture f(mask);assert(f.run()==k::Status::complete && f.state.flags_b8==expected(mask) && f.result.service_calls==3 && f.result.writes==4 && f.result.last_bit==0x400);++cases;}
    for(unsigned mutation=1;mutation<=6;++mutation)for(unsigned mask=0;mask<8;++mask){Fixture f(mask);f.mutation=mutation;assert(f.run()==k::Status::complete && f.calls.size()==3 && f.result.writes==4);if(mutation==1)assert(f.state.flags_b8==expected(mask));++cases;}
    for(unsigned at=1;at<=3;++at)for(bool throwing:{false,true}){Fixture f(7);f.mutation=1;if(throwing)f.throws=at;else f.fail=at;assert(f.run()==k::Status::service_failed && f.result.service_calls==at && f.result.writes==at && f.state.flags_b8==0xbeef0000u+at-1);++failures;}
    {Fixture f;f.services.contains=nullptr;assert(f.run()==k::Status::service_unavailable && f.state.flags_b8==0 && f.result.writes==1 && f.calls.empty());++failures;}
    {Fixture f;f.state.ais=0;assert(f.run()==k::Status::invalid_argument && f.state.flags_b8==0xfefefefe && f.calls.empty());++guards;}
    {Fixture f;assert(k::initialize(nullptr,&f.services,&f.result)==k::Status::invalid_argument);++guards;assert(k::initialize(&f.state,nullptr,&f.result)==k::Status::invalid_argument);++guards;assert(k::initialize(&f.state,&f.services,nullptr)==k::Status::invalid_argument);++guards;}
    {Fixture f;assert(k::initialize(&f.state,&f.services,reinterpret_cast<k::Result*>(&f.state))==k::Status::invalid_argument && f.calls.empty());++guards;assert(k::initialize(&f.state,&f.services,reinterpret_cast<k::Result*>(&f.services))==k::Status::invalid_argument && f.calls.empty());++guards;assert(k::initialize(&f.state,reinterpret_cast<k::Services*>(&f.state),&f.result)==k::Status::invalid_argument && f.calls.empty());++guards;}
    {Fixture f;alignas(k::State) unsigned char bytes[sizeof(k::State)+1]{};assert(k::initialize(reinterpret_cast<k::State*>(bytes+1),&f.services,&f.result)==k::Status::invalid_argument);++guards;}
    {Fixture f;alignas(k::Services) unsigned char bytes[sizeof(k::Services)+1]{};assert(k::initialize(&f.state,reinterpret_cast<k::Services*>(bytes+1),&f.result)==k::Status::invalid_argument);++guards;}
    {Fixture f;alignas(k::Result) unsigned char bytes[sizeof(k::Result)+1]{};assert(k::initialize(&f.state,&f.services,reinterpret_cast<k::Result*>(bytes+1))==k::Status::invalid_argument);++guards;}
    {Fixture f(7);f.services.contains=[](void* raw,k::State* state,const char* name,bool* member)->int {auto& outer=*static_cast<Fixture*>(raw);if(outer.calls.empty()){Fixture nested(4);assert(nested.run()==k::Status::complete && nested.state.flags_b8==0x400);}return Fixture::contains(raw,state,name,member);};assert(f.run()==k::Status::complete && f.state.flags_b8==0x1c00);++cases;}
    // Real selected VM and alias map. Commons unchanged; registration fixtures
    // exercise actual Add/Push/Pop. No player OnInit/combat callback is executed.
    {Fixture f;auto* vm=dh2_script_vm_create(2*1024*1024);assert(vm);assert(dh2_script_alias_bind(vm,f.aliases)==0);
        const auto commons=read(argv[1]);assert(dh2_script_vm_load(vm,commons.data(),commons.size(),"@original/_commons.luac")==0);
        const char* globals="function OnKill() end; function OnTargetHit() end; function OnTargetMissed() end";
        assert(dh2_script_vm_load(vm,globals,std::strlen(globals),"@unregistered-fixture")==0);
        assert(f.run()==k::Status::complete && f.state.flags_b8==0);++lua;
        const char* registration="AddToVFTable('OnKill','OnKill'); AddToVFTable('OnTargetHit','OnTargetHit'); AddToVFTable('OnTargetMissed','OnTargetMissed')";
        assert(dh2_script_vm_load(vm,registration,std::strlen(registration),"@registered-fixture")==0);
        f.calls.clear();assert(f.run()==k::Status::complete && f.state.flags_b8==0x1c00);++lua;
        const char* push="PushVFTable(); AddToVFTable('OnKill','temporary'); AddToVFTable('OnUpdate','OnUpdate')";
        assert(dh2_script_vm_load(vm,push,std::strlen(push),"@push-fixture")==0);
        f.calls.clear();assert(f.run()==k::Status::complete && f.state.flags_b8==0x1c00 && !std::strcmp(dh2_script_alias_resolve(f.aliases,"OnKill"),"temporary"));++lua;
        assert(dh2_script_vm_call_discard_source(vm,"PopVFTable",nullptr,0)==0);
        f.calls.clear();assert(f.run()==k::Status::complete && f.state.flags_b8==0x1c00 && !std::strcmp(dh2_script_alias_resolve(f.aliases,"OnKill"),"OnKill") && dh2_script_alias_contains(f.aliases,"OnUpdate")==0);++lua;
        dh2_script_vm_destroy(vm); // VM closes before borrowed alias maps retire.
    }
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"guards\":"<<guards<<",\"failure_cases\":"<<failures<<",\"real_lua_cases\":"<<lua<<",\"full_width_identity\":true,\"native_player_wired\":false}\n";
}
