#include "../character_monster_retarget.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace retarget=dh2::character_monster_retarget;
namespace {
constexpr std::uintptr_t owner=0x10014000,owner_b=0x10018000,ai=owner+0x3c8;
constexpr std::uintptr_t peer=0x10020000,current=0x10024000;
std::uint32_t bits(float value) { std::uint32_t word; std::memcpy(&word,&value,4); return word; }
struct Call { unsigned op,kind; std::uintptr_t subject,peer; std::uint32_t word; };
struct Fixture {
    retarget::State state{ai,owner};
    std::uintptr_t highest=peer,resolved=current,target_a=current,target_b=current,last_a=0x10028000;
    std::uint32_t current_threat=bits(10),highest_threat=bits(20),factor=bits(1.5f),enemy=0,debug=0;
    unsigned mutation=0,aggro_calls=0;
    int fail=-1,throwing=-1;
    std::vector<Call> calls;
    retarget::Services services{this,invoke}; retarget::Result result{};
    static std::int32_t invoke(void* context,retarget::State* state,const retarget::Request* request,retarget::Response* response) {
        auto& f=*static_cast<Fixture*>(context); const auto op=static_cast<unsigned>(request->operation);
        f.calls.push_back({op,static_cast<unsigned>(request->kind),request->subject,request->peer,request->word});
        switch (request->operation) {
            case retarget::Operation::highest_aggro: response->identity=f.highest; break;
            case retarget::Operation::resolve_target_408: response->identity=f.resolved; break;
            case retarget::Operation::get_aggro: response->word=f.aggro_calls++ ? f.highest_threat : f.current_threat; break;
            case retarget::Operation::design_factor_8: response->word=f.factor; break;
            case retarget::Operation::diagnostic_switch: response->word=f.debug; break;
            case retarget::Operation::set_target:
                assert(!request->word);
                if (request->kind==retarget::Subject::original_ai || request->subject==owner) f.target_a=request->peer;
                else f.target_b=request->peer;
                if (f.mutation==9) f.target_a=peer;
                break;
            case retarget::Operation::is_enemy: response->word=f.enemy; break;
            case retarget::Operation::clear_aggro: break;
            case retarget::Operation::sync_last_target: f.last_a=f.target_a; break;
        }
        if ((f.mutation==1 && op==0) || (f.mutation==2 && op==1) ||
            (f.mutation==3 && op==2 && f.aggro_calls==1) || (f.mutation==4 && op==2 && f.aggro_calls==2) ||
            (f.mutation==5 && op==3) || (f.mutation==6 && op==4) ||
            (f.mutation==7 && op==6) || (f.mutation==8 && op==7)) state->owner=owner_b;
        if (f.throwing==static_cast<int>(op)) throw std::runtime_error("provider");
        return f.fail==static_cast<int>(op);
    }
    retarget::Status run() { return retarget::update(&state,owner,&services,&result); }
};
void emit(const Fixture& f,retarget::Status status) {
    std::cout << "{\"status\":" << static_cast<int>(status) << ",\"decision\":" << static_cast<unsigned>(f.result.decision)
              << ",\"threshold\":" << f.result.threshold_word << ",\"owner\":" << f.state.owner
              << ",\"target_a\":" << f.target_a << ",\"target_b\":" << f.target_b << ",\"last_a\":" << f.last_a << ",\"calls\":[";
    for (std::size_t i=0;i<f.calls.size();++i) { if (i) std::cout<<','; const auto& c=f.calls[i];
        std::cout<<'['<<c.op<<','<<c.kind<<','<<c.subject<<','<<c.peer<<','<<c.word<<']'; }
    std::cout << "]}\n";
}
}
int main(int argc,char** argv) {
    if (argc==9) {
        Fixture f; f.highest=std::strtoull(argv[1],nullptr,0); f.resolved=std::strtoull(argv[2],nullptr,0);
        f.current_threat=std::uint32_t(std::strtoull(argv[3],nullptr,0)); f.highest_threat=std::uint32_t(std::strtoull(argv[4],nullptr,0));
        f.factor=std::uint32_t(std::strtoull(argv[5],nullptr,0)); f.enemy=std::uint32_t(std::strtoull(argv[6],nullptr,0));
        f.mutation=unsigned(std::strtoull(argv[7],nullptr,0)); f.debug=std::uint32_t(std::strtoull(argv[8],nullptr,0));
        const auto status=f.run(); emit(f,status); return 0;
    }
    assert(argc==1); unsigned cases=0;
    auto complete=[&](Fixture& f,retarget::Decision decision) { assert(f.run()==retarget::Status::complete && f.result.decision==decision); ++cases; };
    { Fixture f; complete(f,retarget::Decision::switched_to_highest); assert(f.target_a==peer && f.last_a==0x10028000); }
    { Fixture f; f.highest_threat=bits(15); complete(f,retarget::Decision::keep_current); assert(f.calls.size()==5); }
    { Fixture f; f.highest_threat=bits(14); complete(f,retarget::Decision::keep_current); }
    for (auto value:{0u,0x80000000u,0x7fc00000u,0x7f800000u}) {
        Fixture f; f.current_threat=value; complete(f,value==0 || value==0x80000000 ? retarget::Decision::switched_to_highest : retarget::Decision::keep_current);
    }
    { Fixture f; f.factor=0x7fc00000; complete(f,retarget::Decision::keep_current); }
    { Fixture f; f.factor=bits(-1); complete(f,retarget::Decision::switched_to_highest); }
    { Fixture f; f.highest_threat=0x7fc00000; complete(f,retarget::Decision::keep_current); }
    { Fixture f; f.debug=0xffffffff; complete(f,retarget::Decision::switched_to_highest); }
    for (unsigned mutation=1;mutation<=6;++mutation) {
        Fixture f; f.mutation=mutation; complete(f,retarget::Decision::switched_to_highest);
        assert(f.target_b==peer && f.calls.back().subject==owner_b);
        if (mutation==1) assert(f.calls.front().subject==owner && f.calls[1].subject==owner_b);
    }
    for (unsigned mode=0;mode<3;++mode) {
        Fixture f; if (!mode) f.highest=0; else if (mode==1) f.resolved=0; else f.highest=f.resolved;
        complete(f,retarget::Decision::cleared_non_enemy);
        assert(!f.target_a && !f.last_a && f.calls[2].op==6 && f.calls[3].kind==1);
    }
    for (unsigned mutation=7;mutation<=9;++mutation) {
        Fixture f; f.highest=0; f.mutation=mutation; complete(f,retarget::Decision::cleared_non_enemy);
        assert(f.last_a==(mutation==9 ? peer : 0));
        for (unsigned i=3;i<f.calls.size();++i) assert(f.calls[i].kind==1 && f.calls[i].subject==ai);
    }
    { Fixture f; f.highest=0; f.enemy=7;
      assert(f.run()==retarget::Status::unsupported_branch && f.result.decision==retarget::Decision::enemy_retention_search_boundary && f.calls.size()==3); ++cases; }
    { Fixture f; f.fail=5; f.mutation=6; assert(f.run()==retarget::Status::service_failed && f.target_b==peer); ++cases; }
    { Fixture f; f.highest=0; f.throwing=7; f.mutation=8;
      assert(f.run()==retarget::Status::service_failed && f.state.owner==owner_b && f.target_a==current && f.calls.size()==4); ++cases; }
    { Fixture f; f.services.invoke=nullptr; assert(f.run()==retarget::Status::service_unavailable && !f.result.service_calls); ++cases; }
    { Fixture f; f.result.threshold_word=123;
      assert(retarget::update(&f.state,owner,&f.services,reinterpret_cast<retarget::Result*>(&f.state))==retarget::Status::invalid_argument);
      assert(retarget::update(reinterpret_cast<retarget::State*>(reinterpret_cast<char*>(&f.state)+1),owner,&f.services,&f.result)==retarget::Status::invalid_argument);
      assert(f.calls.empty() && f.result.threshold_word==123); ++cases; }
    std::cout << "{\"validation\":\"PASS\",\"monster_retarget_cases\":"<<cases
              <<",\"fresh_owner_and_original_ai_preserved\":true,\"strict_float_threshold\":true,\"sync_last_target_after_clear\":true,\"unsupported_retention_search_explicit\":true,\"native_wired\":false}\n";
}
