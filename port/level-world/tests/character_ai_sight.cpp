#include "../character_ai_sight.hpp"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace sight = dh2::character_ai_sight;
namespace {
constexpr std::uintptr_t ai = 0x10010000, owner = 0x10014000, owner_b = 0x10018000, target = 0x1001c000;
std::uint32_t bits(float value) { std::uint32_t out; std::memcpy(&out,&value,4); return out; }
struct Call { unsigned op; std::uintptr_t subject; };
struct Fixture {
    sight::State state{ai,owner,target};
    sight::Point owner_point{{0,0,0}}, target_point{{bits(3),bits(4),0}}, alternate{{bits(99),0,0}};
    const sight::Point* current_owner_point = &owner_point;
    sight::ViewRadius row{bits(6)}, row_b{bits(2)};
    const sight::ViewRadius* current_row = &row;
    sight::Services services{this,position,props};
    sight::Result result{};
    unsigned mutation = 0, positions = 0;
    int fail = -1, throwing = -1;
    bool null_owner_point = false, alias_point = false;
    std::vector<Call> calls;
    static std::int32_t position(void* raw, sight::State* state, std::uintptr_t object, const sight::Point** out) {
        auto& f = *static_cast<Fixture*>(raw); ++f.positions; f.calls.push_back({0,object});
        *out = object == target ? &f.target_point : f.current_owner_point;
        if (f.positions == 1) {
            if (f.null_owner_point) *out = nullptr;
            if (f.alias_point) *out = reinterpret_cast<const sight::Point*>(&f.result);
            if (f.mutation == 1) { state->owner = owner_b; state->target_40 = 0; }
            if (f.mutation == 4) f.current_owner_point = &f.alternate;
        } else {
            if (f.mutation == 2) state->owner = owner_b;
            if (f.mutation == 3 || f.mutation == 4) f.owner_point.words[0] = bits(7);
            if (f.mutation == 7) state->target_40 = owner_b;
        }
        if (f.throwing == static_cast<int>(f.positions)) throw std::runtime_error("point");
        return f.fail == static_cast<int>(f.positions);
    }
    static std::int32_t props(void* raw, sight::State* state, std::uintptr_t object, const sight::ViewRadius** out) {
        auto& f = *static_cast<Fixture*>(raw); f.calls.push_back({1,object});
        *out = object == owner_b ? &f.row_b : f.current_row;
        if (f.mutation == 5) { state->owner = owner_b; f.current_row = &f.row_b; }
        if (f.mutation == 6) const_cast<sight::ViewRadius*>(*out)->word_3c = bits(2);
        if (f.throwing == 3) throw std::runtime_error("props");
        return f.fail == 3;
    }
    sight::Status scalar(std::uint32_t word) { return sight::evaluate_distance(&state,word,&services,&result); }
    sight::Status object(std::uintptr_t candidate = target) { return sight::evaluate_object(&state,candidate,&services,&result); }
};
void emit(const Fixture& f, sight::Status status) {
    std::cout << "{\"status\":" << static_cast<int>(status) << ",\"value\":" << f.result.value
              << ",\"distance\":" << f.result.distance_word << ",\"radius_squared\":" << f.result.squared_radius_word
              << ",\"owner\":" << f.state.owner << ",\"target40\":" << f.state.target_40 << ",\"calls\":[";
    for (std::size_t i=0;i<f.calls.size();++i) {
        if (i) std::cout << ',';
        std::cout << '[' << f.calls[i].op << ',' << f.calls[i].subject << ']';
    }
    std::cout << "]}\n";
}
}  // namespace

int main(int argc,char** argv) {
    if (argc == 14) {
        std::uint32_t words[13];
        for (unsigned i=0;i<13;++i) words[i]=std::uint32_t(std::strtoull(argv[i+1],nullptr,0));
        Fixture f; f.row.word_3c=words[3]; f.row_b.word_3c=words[12];
        for (unsigned i=0;i<3;++i) { f.owner_point.words[i]=words[5+i]; f.target_point.words[i]=words[8+i]; }
        f.mutation=words[11]; if (!words[2]) f.state.target_40=0;
        const auto status=words[0] ? f.object(words[1] ? target : 0) : f.scalar(words[4]);
        emit(f,status); return 0;
    }
    assert(argc==1);
    unsigned cases=0;
    auto scalar=[&](Fixture& f,std::uint32_t distance,bool value) {
        assert(f.scalar(distance)==sight::Status::complete && f.result.value==value);
        assert(f.result.props_queries==1 && f.result.position_queries==0); ++cases;
    };
    auto object=[&](Fixture& f,bool value,std::uint32_t distance=0x41c80000) {
        assert(f.object()==sight::Status::complete && f.result.value==value && f.result.distance_word==distance);
        assert(f.result.position_queries==2 && f.result.props_queries==1); ++cases;
    };
    { Fixture f; f.row.word_3c=bits(5); scalar(f,bits(25),false); }
    { Fixture f; f.row.word_3c=bits(5); scalar(f,bits(24),true); }
    { Fixture f; f.row.word_3c=bits(-5); scalar(f,bits(24),true); }
    { Fixture f; f.row.word_3c=0; scalar(f,bits(-1),true); }
    for (auto radius:{0u,0x80000000u}) { Fixture f; f.row.word_3c=radius; scalar(f,0x80000000,false); }
    { Fixture f; f.row.word_3c=0x7fc00000; scalar(f,0,false); }
    { Fixture f; scalar(f,0x7fc00000,false); }
    { Fixture f; f.row.word_3c=0x7f800000; scalar(f,bits(100),true); }
    { Fixture f; f.row.word_3c=0xff800000; scalar(f,0x7f800000,false); }
    { Fixture f; object(f,true); }
    { Fixture f; f.row.word_3c=bits(5); object(f,false); }
    { Fixture f; f.row.word_3c=bits(-6); object(f,true); }
    { Fixture f; f.target_point=f.owner_point; object(f,true,0); }
    { Fixture f; f.target_point=f.owner_point; f.row.word_3c=0; object(f,false,0); }
    { Fixture f; assert(f.object(owner)==sight::Status::complete && f.result.value && !f.result.distance_word); ++cases; }
    { Fixture f; assert(f.object(0)==sight::Status::complete && f.result.candidate==target); ++cases; }
    { Fixture f; f.state.owner=f.state.target_40=0; f.services={};
      assert(f.object(0)==sight::Status::complete && !f.result.value && f.calls.empty()); ++cases; }
    for (unsigned mutation=1;mutation<=7;++mutation) {
        Fixture f; f.mutation=mutation;
        object(f,mutation!=1 && mutation!=2 && mutation!=6,
               mutation==3 || mutation==4 ? bits(32) : bits(25));
        if (mutation==1 || mutation==2) assert(f.calls.back().subject==owner_b);
        if (mutation==5) assert(f.result.squared_radius_word==bits(36) && f.current_row==&f.row_b);
        if (mutation==4) assert(f.result.owner_point==reinterpret_cast<std::uintptr_t>(&f.owner_point));
    }
    { Fixture f; f.mutation=3; f.row.word_3c=bits(5); object(f,false,bits(32)); }
    { Fixture f; f.fail=2; f.mutation=2;
      assert(f.object()==sight::Status::service_failed && f.state.owner==owner_b && f.result.position_queries==2); ++cases; }
    { Fixture f; f.throwing=3; f.mutation=5;
      assert(f.object()==sight::Status::service_failed && f.state.owner==owner_b && f.result.props_queries==1); ++cases; }
    { Fixture f; f.services.char_ai=nullptr;
      assert(f.object()==sight::Status::service_unavailable && f.result.position_queries==2); ++cases; }
    { Fixture f; f.null_owner_point=true;
      assert(f.object()==sight::Status::invalid_source_fact && f.result.position_queries==2); ++cases; }
    { Fixture f; f.alias_point=true;
      assert(f.object()==sight::Status::invalid_source_fact && f.result.position_queries==2); ++cases; }
    { Fixture f; f.result.value=123;
      assert(sight::evaluate_object(&f.state,target,&f.services,reinterpret_cast<sight::Result*>(&f.state))==sight::Status::invalid_argument);
      assert(sight::evaluate_distance(reinterpret_cast<sight::State*>(reinterpret_cast<char*>(&f.state)+1),0,
                                     &f.services,&f.result)==sight::Status::invalid_argument);
      assert(f.calls.empty() && f.result.value==123); ++cases; }
    std::cout << "{\"validation\":\"PASS\",\"sight_cases\":" << cases
              << ",\"borrowed_points_read_after_target_callback\":true,\"fresh_scalar_owner\":true,"
                 "\"strict_binary32_predicates\":true,\"failure_alias_contracts\":true,\"native_wired\":false}\n";
}
