#include "../ais_native_bindings.hpp"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace a=dh2::ais_native_bindings;
struct Fixture {
    std::uintptr_t vm=0x10020000;
    int mutation=0, fail=-1, calls=0;
    bool throws=false;
    std::vector<std::string> events;
};
static int finish(Fixture& f) {
    const int call=f.calls++;
    if (f.mutation) f.vm+=16;
    if (f.fail==call) {
        if (f.throws) throw std::runtime_error("explicit test provider exception");
        return 7;
    }
    return 0;
}
static int open(void* context,std::uintptr_t wrapper,a::Library library) {
    auto& f=*static_cast<Fixture*>(context);
    f.events.push_back("{\"kind\":\"open_library\",\"library\":"+
        std::to_string(static_cast<unsigned>(library))+",\"wrapper\":"+
        std::to_string(wrapper)+",\"raw_vm\":"+std::to_string(f.vm)+"}");
    return finish(f);
}
static int bind(void* context,std::uintptr_t binder,const a::Binding* b,std::uintptr_t user) {
    auto& f=*static_cast<Fixture*>(context);
    f.events.push_back("{\"kind\":\"binding\",\"binder\":"+std::to_string(binder)+
        ",\"name\":\""+b->name+"\",\"function_id\":"+
        std::to_string(static_cast<unsigned>(b->function))+",\"userdata\":"+
        std::to_string(user)+"}");
    return finish(f);
}
static a::Status run(int entry,a::State* state,a::Services* services,a::Result* result) {
    if (entry==0) return a::bind_base(state,services,result);
    if (entry==1) return a::bind_character(state,services,result);
    return a::bind_all(state,services,result);
}
int main(int argc,char** argv) {
    if (argc>1) {
        const int entry=std::atoi(argv[1]);
        const auto identity=static_cast<std::uintptr_t>(std::strtoull(argv[2],nullptr,0));
        Fixture f;f.mutation=std::atoi(argv[3]);
        a::State state{identity,identity+4,identity+16};
        a::Services services{&f,open,bind};a::Result result{};
        const auto status=run(entry,&state,&services,&result);
        std::cout<<"{\"status\":"<<static_cast<int>(status)<<",\"calls\":"<<result.calls
            <<",\"libraries_opened\":"<<result.libraries_opened
            <<",\"functions_bound\":"<<result.functions_bound<<",\"events\":[";
        for (std::size_t i=0;i<f.events.size();++i) {if(i)std::cout<<',';std::cout<<f.events[i];}
        std::cout<<"]}\n";return 0;
    }
    unsigned cases=0;
    std::size_t count=0;const auto* base=a::base_bindings(&count);assert(base&&count==33);
    const auto* character=a::character_bindings(&count);assert(character&&count==2);++cases;
    for(int entry=0;entry<3;++entry) {
        const unsigned calls=entry==0?37:entry==1?2:39;
        for(int failure=-1;failure<static_cast<int>(calls);++failure) {
            Fixture f;f.fail=failure;
            a::State state{0x100000001ULL,0x200000004ULL,0x300000010ULL};
            a::Services services{&f,open,bind};a::Result result{};
            const auto status=run(entry,&state,&services,&result);
            assert(status==(failure<0?a::Status::complete:a::Status::service_failed));
            assert(result.calls==(failure<0?calls:static_cast<unsigned>(failure+1)));
            assert(f.events.size()==result.calls);++cases;
        }
    }
    Fixture f;a::State s{1,2,3};a::Services c{&f,open,bind};a::Result out{71,72,73};
    assert(a::bind_all(nullptr,&c,&out)==a::Status::invalid_argument);
    assert(out.calls==71&&f.calls==0);++cases;
    c.open_library=nullptr;assert(a::bind_all(&s,&c,&out)==a::Status::service_unavailable);
    assert(out.calls==0&&f.calls==0);++cases;
    c.open_library=open;c.bind_function=nullptr;
    assert(a::bind_base(&s,&c,&out)==a::Status::service_unavailable);
    assert(out.calls==4&&out.libraries_opened==4&&out.functions_bound==0);++cases;
    c.open_library=nullptr;c.bind_function=bind;s.vm_wrapper=0;f={};
    assert(a::bind_character(&s,&c,&out)==a::Status::complete&&out.functions_bound==2);++cases;
    f={};f.fail=0;f.throws=true;s.vm_wrapper=2;c.open_library=open;
    assert(a::bind_all(&s,&c,&out)==a::Status::service_failed&&out.calls==1);++cases;
    s.script=0;out={71,72,73};
    assert(a::bind_all(&s,&c,&out)==a::Status::invalid_argument&&out.calls==71);++cases;
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<"}\n";
}
