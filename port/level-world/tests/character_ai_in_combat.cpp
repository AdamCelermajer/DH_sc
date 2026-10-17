#include "../character_ai_in_combat.hpp"
#include <array>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

using namespace dh2::character_ai_in_combat;
constexpr std::uintptr_t ai_id = 0x10010000, owner_id = 0x10014000;
struct Fixture {
    std::array<std::uint32_t, 5> words{};
    bool mutate = false, throw_call = false;
    int fail = -1;
    std::vector<std::pair<std::uint32_t, std::uintptr_t>> calls;
    static std::int32_t invoke(void* raw, State* state, Query query,
                               std::uintptr_t subject, std::uint32_t* value) {
        auto& self = *static_cast<Fixture*>(raw);
        const auto index = static_cast<std::uint32_t>(query);
        self.calls.emplace_back(index, subject);
        if (self.mutate) state->owner = owner_id + (index + 1) * 0x1000;
        if (self.throw_call) throw std::runtime_error("service fixture");
        if (self.fail == int(index)) return 7;
        *value = self.words[index]; return 0;
    }
    Services services() { return {this, invoke}; }
};

void guards() {
    Fixture f; State state{ai_id, owner_id}; auto services = f.services();
    Result result{9,9,9};
    assert(evaluate(nullptr,&services,&result)==Status::invalid_argument && result.value==9);
    assert(evaluate(&state,&services,reinterpret_cast<Result*>(&state))==Status::invalid_argument && state.ai==ai_id);
    alignas(State) unsigned char bytes[sizeof(State)+8]{};
    assert(evaluate(reinterpret_cast<State*>(bytes+1),&services,&result)==Status::invalid_argument && result.value==9);
    auto missing=services;missing.invoke=nullptr;
    assert(evaluate(&state,&missing,&result)==Status::service_unavailable && result.service_calls==0);
    state.owner=0;f.words[0]=0xffffffff;
    assert(evaluate(&state,&services,&result)==Status::complete && result.value==1 && result.service_calls==1);
    f.words={};
    assert(evaluate(&state,&services,&result)==Status::invalid_argument && result.service_calls==2);
    state.owner=owner_id;f.mutate=true;f.fail=2;
    assert(evaluate(&state,&services,&result)==Status::service_failed && result.service_calls==3 && state.owner==owner_id+0x3000);
    f.fail=-1;f.throw_call=true;
    assert(evaluate(&state,&services,&result)==Status::service_failed && result.service_calls==1 && state.owner==owner_id+0x1000);
    std::cout << "{\"validation\":\"PASS\",\"guard_cases\":8}\n";
}

int main(int argc,char** argv) {
    if(argc==1){guards();return 0;}
    if(argc!=7)return 2;
    Fixture f;
    for(unsigned i=0;i<5;++i)f.words[i]=std::uint32_t(std::strtoull(argv[i+1],nullptr,0));
    f.mutate=std::strtoull(argv[6],nullptr,0)!=0;
    State state{ai_id,owner_id};auto services=f.services();Result result{};
    const auto status=evaluate(&state,&services,&result);
    std::cout << "{\"status\":" << int(status) << ",\"value\":" << result.value
              << ",\"owner\":" << state.owner << ",\"calls\":[";
    for(std::size_t i=0;i<f.calls.size();++i){
        if(i)std::cout << ',';
        std::cout << '[' << f.calls[i].first << ',' << f.calls[i].second << ']';
    }
    std::cout << "]}\n";
}
