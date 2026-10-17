#include "../character_ai_master_update.hpp"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace dh2::character_ai_master_update;
constexpr std::uintptr_t AI=0x10010000, OWNER=0x10014000,
    MASTER=0x10018000, OTHER=0x10019000;
struct Recorded { Request request; std::uint8_t alive, sight; };
struct Fixture {
    std::uint32_t words[9]{};
    std::uint32_t mutate_op=99, mutation=0, fail_op=99;
    std::vector<Recorded> calls;
};
std::int32_t invoke(void* context, State* state, const Request* request, Response* response) {
    auto& f=*static_cast<Fixture*>(context);
    f.calls.push_back({*request,state->alive_54,state->sight_55});
    const auto op=static_cast<std::uint32_t>(request->operation);
    if (f.mutate_op==op || f.mutate_op==98) {
        if (f.mutation==1) state->owner+=0x1000;
        if (f.mutation==2) state->master=state->master==MASTER?OTHER:MASTER;
        if (f.mutation==3) state->master=0;
        if (f.mutation==4) {state->alive_54=0;state->sight_55=0;}
        if (f.mutation==5) {state->alive_54=7;state->sight_55=9;}
    }
    if (op==f.fail_op) return 1;
    response->word=f.words[op];
    return 0;
}
int main(int argc,char** argv) {
    if (argc!=14) {
        State s{AI,OWNER,0,1,1,0};Fixture f{};Services services{&f,invoke};Result out{};
        assert(update(&s,&services,&out)==Status::complete&&out.calls==0);
        assert(update(nullptr,&services,&out)==Status::invalid_argument);
        assert(update(&s,nullptr,&out)==Status::invalid_argument);
        assert(update(&s,&services,nullptr)==Status::invalid_argument);
        assert(update(&s,&services,reinterpret_cast<Result*>(&s))==Status::invalid_argument);
        auto bad=s;bad.reserved=1;
        assert(update(&bad,&services,&out)==Status::invalid_argument);
        auto missing=services;missing.invoke=nullptr;
        assert(update(&s,&missing,&out)==Status::invalid_argument);
        Services throwing{nullptr,[](void*,State*,const Request*,Response*)->std::int32_t{throw std::runtime_error("provider");}};
        s.master=MASTER;
        assert(update(&s,&throwing,&out)==Status::service_failed&&out.calls==1);
        std::cout<<"{\"validation\":\"PASS\",\"guard_cases\":8}\n";
        return 0;
    }
    std::uint32_t v[13]{};
    for (int i=0;i<13;++i) v[i]=static_cast<std::uint32_t>(std::strtoull(argv[i+1],nullptr,0));
    State s{AI,OWNER,v[0]?MASTER:0,static_cast<std::uint8_t>(v[1]),static_cast<std::uint8_t>(v[2]),0};
    Fixture f{};f.words[0]=777;f.words[1]=v[3];f.words[2]=v[4];f.words[3]=v[5];
    f.words[4]=v[6];f.words[5]=v[7];f.words[6]=v[8];f.words[7]=v[9];
    f.mutate_op=v[10];f.mutation=v[11];f.fail_op=v[12];
    Services services{&f,invoke};Result out{};
    const auto status=update(&s,&services,&out);
    std::cout<<"{\"status\":"<<static_cast<int>(status)<<",\"owner\":"<<s.owner
        <<",\"master\":"<<s.master<<",\"alive\":"<<unsigned(s.alive_54)
        <<",\"sight\":"<<unsigned(s.sight_55)<<",\"calls\":[";
    for (std::size_t i=0;i<f.calls.size();++i) {
        if(i)std::cout<<',';
        const auto& row=f.calls[i];const auto& r=row.request;
        std::cout<<'['<<static_cast<unsigned>(r.operation)<<','<<r.subject<<','<<r.peer<<','<<r.event
            <<','<<unsigned(row.alive)<<','<<unsigned(row.sight)<<']';
    }
    std::cout<<"]}\n";
}
