#include "../ais_external_init_vcb.hpp"
#include "../../adam-script-runtime/script_function_alias.h"
#include "../../lua-numeric/numeric.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
namespace k=dh2::ais_external_init_vcb;
constexpr std::uintptr_t AIS=0x10014000;
constexpr const char* names[]={"OnTargetHit","OnTargetMissed","OnUpdate","OnFriendSpotted",
    "OnTargetOutOfRange","OnTargetInRangedRange","OnTargetInCloseRange","OnTargetInMeleeRange",
    "OnMasterOutOfRange","OnMasterInRangedRange","OnMasterInCloseRange","OnMasterInMeleeRange"};
struct Call {unsigned index,flags,member;};
struct Fixture {
    k::State state{AIS,0xfefefefe};k::Result result{};k::Services services{this,contains};
    unsigned membership=0,mutation=0,fail=0,throws=0;
    dh2_script_aliases* aliases=nullptr;std::vector<dh2_script_aliases*> retired;
    std::vector<Call> calls;
    explicit Fixture(unsigned mask=0):membership(mask){replace(mask);}
    ~Fixture(){dh2_script_alias_destroy(aliases);for(auto* old:retired)dh2_script_alias_destroy(old);}
    void replace(unsigned mask) {
        auto* next=dh2_script_alias_create();assert(next);
        for(unsigned i=0;i<12;++i)if(mask&(1u<<i))assert(dh2_script_alias_add(next,names[i],"fixture_alias")==0);
        if(aliases)retired.push_back(aliases);
        aliases=next;membership=mask;
    }
    void mutate(unsigned index) {
        if(mutation==1)state.flags_b8=0xbeef0000u+index;
        if(mutation==2 && index==0)replace(membership^2);
        if(mutation==3 && index==1)replace(membership^4);
        if(mutation==4 && index==3)replace(0);
        if(mutation==5 && index==4)replace(0xfff);
        if(mutation==6 && index==5)replace(membership^(1u<<11));
        if(mutation==7)replace(membership^(1u<<((index+1)%12)));
        if(mutation==8 && index==1)services.contains=nullptr;
    }
    static int contains(void* raw,k::State* state,const char* name,bool* member) {
        auto& f=*static_cast<Fixture*>(raw);assert(state==&f.state && state->ais==AIS);
        unsigned index=0;while(index<12 && std::strcmp(name,names[index]))++index;assert(index<12);
        const auto present=dh2_script_alias_contains(f.aliases,name);assert(present==0 || present==1);
        f.calls.push_back({index,f.state.flags_b8,unsigned(present)});f.mutate(index);
        if(f.fail==f.calls.size())return 9;
        if(f.throws==f.calls.size())throw std::runtime_error("provider");
        *member=present!=0;return 0;
    }
    k::Status run(bool external=true){return external?k::initialize_external(&state,&services,&result):k::initialize_default(&state,&services,&result);}
};
std::string read(const char* path){std::ifstream file(path,std::ios::binary);assert(file);return {std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>()};}
int py_struct(void*,const dh2_script_value* args,unsigned count,dh2_script_value* out,unsigned cap,unsigned* n,char*,std::size_t) {
    assert(count==2 && args[0].type==DH2_SCRIPT_STRING && args[1].type==DH2_SCRIPT_STRING && cap>=1);
    assert(!std::strcmp(args[0].text,"CharacterProperties") && !std::strcmp(args[1].text,"SkillTree"));
    out[0]={};out[0].type=DH2_SCRIPT_NUMBER;out[0].number=28;*n=1;return 0;
}
int prop(void*,const dh2_script_value* args,unsigned count,dh2_script_value* out,unsigned cap,unsigned* n,char*,std::size_t) {
    assert(count==1 && args[0].type==DH2_SCRIPT_NUMBER && args[0].number==28 && cap>=1);
    out[0]={};out[0].type=DH2_SCRIPT_NUMBER;out[0].number=-256;*n=1;return 0;
}
int from_fixed(void*,const dh2_script_value* args,unsigned count,dh2_script_value* out,unsigned cap,unsigned* n,char*,std::size_t) {
    assert(count==1 && args[0].type==DH2_SCRIPT_NUMBER && cap>=2);
    dh2_lua_numeric_result result{};assert(dh2_lua_numeric(DH2_FROM_FIXED,&args[0].number,1,&result)==0);
    out[0]={};out[0].type=DH2_SCRIPT_NUMBER;out[0].number=float(result.integer);
    out[1]={};out[1].type=DH2_SCRIPT_NUMBER;out[1].number=result.number;*n=result.count;return 0;
}
unsigned expected_mask(unsigned members,bool external) {
    unsigned result=0;constexpr unsigned bits[]={0x800,0x1000,1,2,4,8,0x10,0x20,0x40,0x80,0x100,0x200};
    for(unsigned i=0;i<(external?12u:2u);++i)if(members&(1u<<i))result|=bits[i];
    return result;
}
int main(int argc,char** argv) {
    if(argc>1 && !std::strcmp(argv[1],"oracle")) {
        assert(argc==6);Fixture f(std::strtoul(argv[3],nullptr,0));f.mutation=std::strtoul(argv[4],nullptr,0);f.state.flags_b8=std::strtoul(argv[5],nullptr,0);
        const auto status=f.run(std::strtoul(argv[2],nullptr,0)!=0);
        std::cout<<"{\"status\":"<<unsigned(status)<<",\"flags\":"<<f.state.flags_b8<<",\"writes\":"<<f.result.writes<<",\"calls\":[";
        bool first=true;for(auto c:f.calls){if(!first)std::cout<<',';first=false;std::cout<<'['<<c.index<<','<<c.flags<<','<<c.member<<']';}
        std::cout<<"]}\n";return 0;
    }
    assert(argc==3);unsigned cases=0;
    for(bool external:{false,true}) {
        std::vector<unsigned> masks={0,0xfff,0x555,0xaaa,0xc3,0x0fc};
        for(unsigned i=0;i<12;++i){masks.push_back(1u<<i);masks.push_back(0xfff^(1u<<i));}
        for(unsigned mask:masks){Fixture f(mask);assert(f.run(external)==k::Status::complete && f.state.flags_b8==expected_mask(mask,external) && f.result.writes==unsigned(external?13:3));++cases;}
        for(unsigned mutation=1;mutation<=8;++mutation)for(unsigned mask:{0u,0xfffu,0x0fcu}) {
            Fixture f(mask);f.mutation=mutation;assert(f.run(external)==k::Status::complete && f.calls.size()==unsigned(external?12:2));
            if(mutation==1)assert(f.state.flags_b8==expected_mask(mask,external));
            ++cases;
        }
        const auto count=external?12u:2u;
        for(unsigned at=1;at<=count;++at){Fixture f(0xfff);f.mutation=1;f.fail=at;
            assert(f.run(external)==k::Status::service_failed && f.result.service_calls==at && f.result.writes==at && f.state.flags_b8==0xbeef0000u+at-1);++cases;
            Fixture g(0xfff);g.mutation=1;g.throws=at;assert(g.run(external)==k::Status::service_failed && g.result.writes==at && g.state.flags_b8==0xbeef0000u+at-1);++cases;
        }
        {Fixture f;f.services.contains=nullptr;assert(f.run(external)==k::Status::service_unavailable && f.state.flags_b8==0 && f.result.writes==1 && f.calls.empty());++cases;}
    }
    {Fixture f;f.state.ais=0;assert(f.run()==k::Status::invalid_argument && f.state.flags_b8==0xfefefefe && f.calls.empty());++cases;}
    {Fixture f;assert(k::initialize_external(&f.state,&f.services,reinterpret_cast<k::Result*>(&f.state))==k::Status::invalid_argument && f.calls.empty());++cases;}
    {Fixture f;assert(k::initialize_external(&f.state,&f.services,reinterpret_cast<k::Result*>(&f.services))==k::Status::invalid_argument && f.calls.empty());++cases;}
    {Fixture f;assert(k::initialize_external(&f.state,reinterpret_cast<k::Services*>(&f.state),&f.result)==k::Status::invalid_argument && f.calls.empty());++cases;}
    {Fixture f;alignas(k::State) unsigned char bytes[sizeof(k::State)+1]{};assert(k::initialize_external(reinterpret_cast<k::State*>(bytes+1),&f.services,&f.result)==k::Status::invalid_argument);++cases;}
    {Fixture f;alignas(k::Services) unsigned char bytes[sizeof(k::Services)+1]{};assert(k::initialize_external(&f.state,reinterpret_cast<k::Services*>(bytes+1),&f.result)==k::Status::invalid_argument);++cases;}
    {Fixture f;alignas(k::Result) unsigned char bytes[sizeof(k::Result)+1]{};assert(k::initialize_external(&f.state,&f.services,reinterpret_cast<k::Result*>(bytes+1))==k::Status::invalid_argument);++cases;}
    {Fixture f(0xfff);f.services.contains=[](void* raw,k::State* state,const char* name,bool* member)->int {
        auto& outer=*static_cast<Fixture*>(raw);if(outer.calls.empty()){Fixture nested(4);assert(nested.run()==k::Status::complete && nested.state.flags_b8==1);}
        return Fixture::contains(raw,state,name,member);
      };assert(f.run()==k::Status::complete && f.state.flags_b8==0x1bff);++cases;
    }
    // Execute unchanged scripts through the maintained real VM and alias map.
    // Only three load-time property/numeric providers are supplied fixtures;
    // no OnInit/combat/timer callback body is invoked in this check.
    {Fixture f;auto* vm=dh2_script_vm_create(2*1024*1024);assert(vm);
        assert(dh2_script_alias_bind(vm,f.aliases)==0);
        assert(dh2_script_vm_bind_source_values(vm,"GetPyStruct",py_struct,nullptr)==0);
        assert(dh2_script_vm_bind_source_values(vm,"GetProp",prop,nullptr)==0);
        assert(dh2_script_vm_bind_source_values(vm,"FromFixed",from_fixed,nullptr)==0);
        const auto commons=read(argv[1]),monster=read(argv[2]);
        assert(dh2_script_vm_load(vm,commons.data(),commons.size(),"@original/_commons.luac")==0);
        dh2_script_value global{};assert(dh2_script_vm_get_global(vm,"OnUpdate",&global)==0 && global.type==DH2_SCRIPT_FUNCTION);
        assert(dh2_script_alias_contains(f.aliases,"OnUpdate")==0);
        assert(f.run()==k::Status::complete && f.state.flags_b8==0);++cases;
        assert(dh2_script_vm_load(vm,monster.data(),monster.size(),"@original/monster.luac")==0);
        f.calls.clear();assert(f.run()==k::Status::complete && f.state.flags_b8==0x183c);
        assert(!std::strcmp(dh2_script_alias_resolve(f.aliases,"OnTargetMissed"),"monster_OnTargetHit"));
        assert(dh2_script_alias_contains(f.aliases,"OnUpdate")==0 && !(f.state.flags_b8&1));++cases;
        assert(dh2_script_alias_add(f.aliases,"OnUpdate","OnUpdate")==0);
        f.calls.clear();assert(f.run()==k::Status::complete && f.state.flags_b8==0x183d);++cases;
        dh2_script_vm_destroy(vm); // closures borrow aliases; destroy VM first
    }
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"unchanged_scripts_executed\":true,\"plain_monster_mask\":6204,\"global_function_is_not_registration\":true,\"fresh_membership_cached_flags\":true,\"errors_preserve_effects\":true,\"same_state_reentry_forbidden\":true,\"native_wired\":false}\n";
}
