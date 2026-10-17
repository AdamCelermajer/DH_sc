#include "../character_native_bindings.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>
namespace k=dh2::character_native_bindings;
constexpr std::uintptr_t C=0x10014000,B=0x10020000;
struct Installed {const k::Binding* binding;std::uintptr_t userdata;};
struct Fixture {
    k::State state{C,B};k::Services services{this,bind};k::Result result{};
    std::vector<Installed> calls;std::map<std::string,Installed> functions,methods;
    unsigned fail=0,throws=0;bool alter_input=false,nested=false;std::uintptr_t expected_character=C,expected_binder=B;
    static int bind(void* raw,std::uintptr_t binder,const k::Binding* row,std::uintptr_t userdata) {
        auto& f=*static_cast<Fixture*>(raw);assert(binder==f.expected_binder && row && row->name && *row->name);
        assert(k::provider_name(row->function));assert(userdata==(row->context==k::Context::character?f.expected_character:0));
        if(row->kind==k::Kind::method)assert(row->context==k::Context::none && !userdata);
        f.calls.push_back({row,userdata});
        if(f.alter_input && f.calls.size()==1){f.state.character=0;f.state.binder=0;f.services.bind=nullptr;}
        if(f.nested && f.calls.size()==1){Fixture child;child.expected_character=child.state.character=C+0x1000;
            child.expected_binder=child.state.binder=B+0x1000;assert(child.run(false)==k::Status::complete);}
        if(f.fail==f.calls.size())return 17;
        if(f.throws==f.calls.size())throw std::runtime_error("native Binder installation");
        auto& registry=row->kind==k::Kind::function?f.functions:f.methods;registry[row->name]={row,userdata};return 0;
    }
    k::Status run(bool character=true){return character?k::bind_character(&state,&services,&result):k::bind_game_object(&state,&services,&result);}
};
void dump(const Fixture& f,k::Status status) {
    std::cout<<"{\"status\":"<<unsigned(status)<<",\"functions\":"<<f.result.functions_bound<<",\"methods\":"<<f.result.methods_bound<<",\"calls\":[";
    bool first=true;for(const auto& call:f.calls){if(!first)std::cout<<',';first=false;
        const auto& row=*call.binding;std::cout<<'['<<unsigned(row.kind)<<",\""<<row.name<<"\",\""<<k::provider_name(row.function)<<"\","<<call.userdata<<']';}
    std::cout<<"]}\n";
}
int main(int argc,char** argv) {
    if(argc>1){assert(argc==4);Fixture f;f.expected_character=f.state.character=std::strtoull(argv[2],nullptr,0);
        f.expected_binder=f.state.binder=std::strtoull(argv[3],nullptr,0);const auto status=f.run(std::strtoul(argv[1],nullptr,0));dump(f,status);return 0;}
    unsigned cases=0;
    for(bool character:{false,true}) {
        Fixture f;assert(f.run(character)==k::Status::complete);const unsigned count=character?265:86;
        assert(f.calls.size()==count && f.result.calls==count && f.result.functions_bound+f.result.methods_bound==count);
        if(character)assert(f.result.functions_bound==135 && f.result.methods_bound==130);
        ++cases;
        for(unsigned failure=1;failure<=count;++failure)for(bool throwing:{false,true}) {
            Fixture failed;if(throwing)failed.throws=failure;else failed.fail=failure;
            assert(failed.run(character)==k::Status::service_failed && failed.calls.size()==failure && failed.result.calls==failure);
            assert(failed.result.functions_bound+failed.result.methods_bound==failure-1);
            for(unsigned i=0;i<failure;++i){assert(failed.calls[i].binding==f.calls[i].binding && failed.calls[i].userdata==f.calls[i].userdata);}
            ++cases;
        }
        Fixture altered;altered.alter_input=true;assert(altered.run(character)==k::Status::complete && altered.calls.size()==count);++cases;
        Fixture nested;nested.nested=true;assert(nested.run(character)==k::Status::complete && nested.calls.size()==count);++cases;
    }
    {Fixture f;assert(f.run()==k::Status::complete);
        for(const char* name:{"GetDistanceBetween","SetFXEndPoint","RegisterSummon","SetProjectileTarget"}) {
            assert(f.functions.at(name).userdata==0);++cases;
        }
        const auto position=f.functions.at("GetPosition");assert(position.userdata==C && position.binding->function==k::Function::game_object_get_position);++cases;
        assert(f.methods.at("GetPosition").userdata==0 && f.methods.at("GetPosition").binding->context==k::Context::none);++cases;
        const auto move_to=f.functions.at("MoveTo");assert(move_to.binding->function==k::Function::character_move_to);++cases;
        assert(f.calls.front().binding->kind==k::Kind::method && std::strcmp(f.calls.front().binding->name,"LOCK")==0);++cases;
        assert(std::strcmp(f.calls[85].binding->name,"SetMaxPath")==0);++cases;
        assert(std::strcmp(f.calls[86].binding->name,"SetActorPosition")==0 && f.calls[86].binding->function==k::Function::game_object_set_position);++cases;
        assert(std::strcmp(f.calls.back().binding->name,"SetSpellCooldownTimerId__")==0 && f.calls.back().binding->kind==k::Kind::method);++cases;
    }
    {std::size_t base_count=0,own_count=0;const auto base=k::game_object_bindings(&base_count),own=k::character_own_bindings(&own_count);
        assert(base_count==86 && own_count==179 && base && own && k::game_object_bindings(nullptr)==base && k::character_own_bindings(nullptr)==own);++cases;}
    {assert(!k::provider_name(k::Function::count) && !k::provider_name(static_cast<k::Function>(65535)));++cases;}
    {Fixture f;f.services.bind=nullptr;assert(f.run()==k::Status::service_unavailable && f.calls.empty() && !f.result.calls);++cases;}
    {Fixture f;f.state.character=0;assert(f.run()==k::Status::invalid_argument && f.calls.empty());++cases;}
    {Fixture f;f.state.binder=0;assert(f.run()==k::Status::invalid_argument && f.calls.empty());++cases;}
    {Fixture f;assert(k::bind_character(nullptr,&f.services,&f.result)==k::Status::invalid_argument);++cases;}
    {Fixture f;assert(k::bind_character(&f.state,nullptr,&f.result)==k::Status::invalid_argument);++cases;}
    {Fixture f;assert(k::bind_character(&f.state,&f.services,nullptr)==k::Status::invalid_argument);++cases;}
    {Fixture f;assert(k::bind_character(&f.state,&f.services,reinterpret_cast<k::Result*>(&f.state))==k::Status::invalid_argument);++cases;}
    {Fixture f;assert(k::bind_character(&f.state,&f.services,reinterpret_cast<k::Result*>(&f.services))==k::Status::invalid_argument);++cases;}
    {Fixture f;assert(k::bind_character(&f.state,reinterpret_cast<k::Services*>(&f.state),&f.result)==k::Status::invalid_argument);++cases;}
    for(unsigned index=0;index<3;++index){Fixture f;alignas(k::Services) unsigned char bytes[sizeof(k::Services)+1]{};
        auto state=&f.state;auto services=&f.services;auto result=&f.result;
        if(index==0)state=reinterpret_cast<k::State*>(bytes+1);
        if(index==1)services=reinterpret_cast<k::Services*>(bytes+1);
        if(index==2)result=reinterpret_cast<k::Result*>(bytes+1);
        assert(k::bind_character(state,services,result)==k::Status::invalid_argument);++cases;
    }
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"ordered_registrations\":265,\"callback_bodies\":0,\"native_wired\":false}\n";
}
