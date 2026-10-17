#include "../ais_external_initialization.hpp"
#include <array>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
namespace k=dh2::ais_external_initialization;
constexpr std::uintptr_t AIS=0x10014000,C=0x10020000;
using Snapshot=std::array<std::uint64_t,12>;
Snapshot snapshot(const k::State& s) {
    const auto& t=s.state_registry_9c;const auto self=reinterpret_cast<std::uintptr_t>(&t);
    return {s.owner_98,s.dispatch_table,s.current_state_b4,s.flags_b8,s.counter_bc,s.word_c0,
        t.parent,t.color,std::uint64_t(t.left==self),std::uint64_t(t.right==self),t.count,0};
}
struct Call {unsigned operation;std::uintptr_t subject,binder;unsigned argument;Snapshot before;std::string text;};
struct Fixture {
    k::State state{AIS,0x10101010,AIS+0x10,AIS+0x68,0x20202020,
        {7,0x30303030,0x40404040,0x50505050,11},0x60606060,0x70707070,0x80808080,0x90909090};
    k::Tables tables{0x11110000,0x22220000};k::Result result{};k::Services services{this,invoke};
    unsigned mode=0,skip=0,mutation=0,fail=0,throws=0;bool nested=false;std::vector<Call> calls;std::string path;
    void mutate(unsigned op) {
        if(mutation==1 && op==0)state.owner_98=0xf101;
        if(mutation==2 && op==0)state.dispatch_table=0xf102;
        if(mutation==3 && op==0){state.current_state_b4=0xf103;state.state_registry_9c={3,0xf104,0xf105,0xf106,17};}
        if(mutation==4 && op==0){state.flags_b8=0xf107;state.counter_bc=0xf108;state.word_c0=0xf109;}
        if(mutation==5 && op==1){state.owner_98=0xf110;state.current_state_b4=0xf111;state.state_registry_9c.parent=0xf112;state.state_registry_9c.count=19;}
        if(mutation==6 && op==1){state.dispatch_table=0xf113;state.flags_b8=0xf114;state.counter_bc=0xf115;state.word_c0=0xf116;}
        if(mutation==7 && op==2)state.owner_98=0xf117;
        if(mutation==8 && op==2){state.flags_b8=0xf118;state.current_state_b4=0xf119;}
        if(mutation==9 && op==3){state.owner_98=0xf120;state.word_c0=0xf121;}
        if(mutation==10 && calls.size()==1)services.invoke=nullptr;
        if(nested && calls.size()==1){Fixture child;child.mode=1;child.skip=1;
            child.state.identity=AIS+0x1000;child.state.binder_identity=child.state.identity+0x10;
            child.state.path_storage_identity=child.state.identity+0x68;assert(child.run()==k::Status::complete);}
    }
    static int invoke(void* raw,k::State* state,const k::Request* request) {
        auto& f=*static_cast<Fixture*>(raw);assert(state==&f.state);const unsigned op=unsigned(request->operation);
        assert(request->subject==(op<2?state->identity:(op==2?C:state->path_storage_identity)));
        assert(request->binder==(op<3?state->binder_identity:0));assert(request->argument==(op==0?f.skip:0));
        std::string text;if(request->text)text.assign(request->text,request->text_bytes);
        if(op==3)assert(text=="data/scripts/ai/" && request->text_bytes==16);else assert(text.empty() && !request->text_bytes);
        f.calls.push_back({op,request->subject,request->binder,request->argument,snapshot(*state),text});
        f.mutate(op);if(op==3)f.path=text;
        if(f.fail==f.calls.size())return -1;
        if(f.throws==f.calls.size())throw std::runtime_error("resource backend");
        return 0;
    }
    k::Status run() {
        if(mode==0)return k::construct_char_ai_script(&state,skip,&tables,&services,&result);
        if(mode==1)return k::construct_external(&state,skip,&tables,&services,&result);
        return k::set_character(&state,C,&services,&result);
    }
};
void array(const Snapshot& s){std::cout<<'[';for(unsigned i=0;i<s.size();++i){if(i)std::cout<<',';std::cout<<s[i];}std::cout<<']';}
void dump(const Fixture& f,k::Status status) {
    std::cout<<"{\"status\":"<<unsigned(status)<<",\"returned\":"<<f.result.returned_identity<<",\"state\":";array(snapshot(f.state));
    std::cout<<",\"path\":\""<<f.path<<"\",\"calls\":[";bool first=true;
    for(const auto& c:f.calls){if(!first)std::cout<<',';first=false;
        std::cout<<'['<<c.operation<<','<<c.subject<<','<<c.binder<<','<<c.argument<<',';array(c.before);std::cout<<",\""<<c.text<<"\"]";}
    std::cout<<"]}\n";
}
int main(int argc,char** argv) {
    if(argc>1){assert(argc==6);Fixture f;f.mode=std::strtoul(argv[1],nullptr,0);f.skip=std::strtoul(argv[2],nullptr,0);
        f.mutation=std::strtoul(argv[3],nullptr,0);f.tables.char_ai_script=std::strtoull(argv[4],nullptr,0);
        f.tables.ais_external=std::strtoull(argv[5],nullptr,0);const auto status=f.run();dump(f,status);return 0;}
    unsigned cases=0;
    for(unsigned mode=0;mode<3;++mode)for(unsigned skip=0;skip<2;++skip)for(unsigned mutation=0;mutation<=10;++mutation) {
        Fixture f;f.mode=mode;f.skip=skip;f.mutation=mutation;assert(f.run()==k::Status::complete);
        assert(f.result.service_calls==f.calls.size() && f.result.last_operation==f.calls.back().operation);
        if(mode<2){assert(f.result.returned_identity==AIS && f.calls.front().operation==0 && f.calls.size()==(skip?1u:2u));
            if(mutation!=5 || skip){assert(!f.state.owner_98 && !f.state.current_state_b4 && !f.state.state_registry_9c.count);}
            if(mode==1)assert(f.state.dispatch_table==f.tables.ais_external && !f.state.flags_b8 && !f.state.counter_bc && !f.state.word_c0);
        }else{assert(!f.result.returned_identity && f.calls.size()==2 && f.calls[0].operation==2 && f.calls[1].operation==3);
            assert(f.calls[0].before[0]==C && f.path=="data/scripts/ai/");}
        ++cases;
    }
    for(unsigned mode=0;mode<3;++mode)for(unsigned fail=1;fail<=2;++fail)for(bool throwing:{false,true}) {
        Fixture f;f.mode=mode;f.mutation=mode==2?7:1;f.fail=throwing?0:fail;f.throws=throwing?fail:0;
        assert(f.run()==k::Status::service_failed && f.calls.size()==fail && !f.result.returned_identity);
        if(fail==1)assert(f.state.owner_98==(mode==2?0xf117u:0xf101u));
        if(mode<2 && fail==2)assert(f.state.dispatch_table==f.tables.char_ai_script && f.state.flags_b8==0x70707070);
        ++cases;
    }
    for(unsigned mode=0;mode<3;++mode){Fixture f;f.mode=mode;f.services.invoke=nullptr;
        assert(f.run()==k::Status::service_unavailable && f.calls.empty());
        assert(f.state.owner_98==(mode==2?C:0x20202020u));++cases;}
    {Fixture f;f.mode=1;f.nested=true;assert(f.run()==k::Status::complete && f.calls.size()==2);++cases;}
    {Fixture f;assert(k::set_character(&f.state,0,&f.services,&f.result)==k::Status::invalid_argument && f.calls.empty());++cases;}
    for(unsigned item=0;item<3;++item){Fixture f;if(item==0)f.state.identity=0;if(item==1)f.state.binder_identity=0;if(item==2)f.state.path_storage_identity=0;
        assert(f.run()==k::Status::invalid_argument && f.calls.empty());++cases;}
    {Fixture f;f.tables.char_ai_script=0;assert(f.run()==k::Status::invalid_argument);++cases;}
    {Fixture f;f.mode=1;f.tables.ais_external=0;assert(f.run()==k::Status::invalid_argument);++cases;}
    {Fixture f;f.tables.ais_external=0;assert(f.run()==k::Status::complete);++cases;}
    {Fixture f;assert(k::construct_external(&f.state,true,&f.tables,&f.services,reinterpret_cast<k::Result*>(&f.state))==k::Status::invalid_argument);++cases;}
    {Fixture f;assert(k::set_character(&f.state,C,&f.services,reinterpret_cast<k::Result*>(&f.services))==k::Status::invalid_argument);++cases;}
    {Fixture f;assert(k::construct_external(&f.state,true,reinterpret_cast<k::Tables*>(&f.state),&f.services,&f.result)==k::Status::invalid_argument);++cases;}
    {Fixture f;assert(k::construct_external(&f.state,true,reinterpret_cast<k::Tables*>(&f.services),&f.services,&f.result)==k::Status::invalid_argument);++cases;}
    {Fixture f;assert(k::construct_external(&f.state,true,reinterpret_cast<k::Tables*>(&f.result),&f.services,&f.result)==k::Status::invalid_argument);++cases;}
    {Fixture f;assert(k::set_character(&f.state,C,reinterpret_cast<k::Services*>(&f.state),&f.result)==k::Status::invalid_argument);++cases;}
    for(unsigned item=0;item<4;++item){Fixture f;alignas(k::State) unsigned char bytes[sizeof(k::State)+1]{};
        auto state=&f.state;auto tables=&f.tables;auto services=&f.services;auto result=&f.result;
        if(item==0)state=reinterpret_cast<k::State*>(bytes+1);
        if(item==1)tables=reinterpret_cast<k::Tables*>(bytes+1);
        if(item==2)services=reinterpret_cast<k::Services*>(bytes+1);
        if(item==3)result=reinterpret_cast<k::Result*>(bytes+1);
        assert(k::construct_external(state,true,tables,services,result)==k::Status::invalid_argument);++cases;}
    {Fixture f;assert(k::construct_external(nullptr,true,&f.tables,&f.services,&f.result)==k::Status::invalid_argument);++cases;}
    {Fixture f;assert(k::set_character(&f.state,C,nullptr,&f.result)==k::Status::invalid_argument);++cases;}
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"native_wired\":false}\n";
}
