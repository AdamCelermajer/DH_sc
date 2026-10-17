#include "../character_skill_state_queries.hpp"
#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <limits>
#include <string>
namespace sq=dh2::character_skill_state_queries;
std::int32_t signed_word(std::uint32_t word){std::int32_t value;std::memcpy(&value,&word,4);return value;}
void print(sq::Status status,const sq::Result& r){
    std::cout<<"{\"status\":"<<int(status)<<",\"state_word\":"<<r.state_word<<",\"value\":"<<r.value<<"}\n";
}
int main(int argc,char** argv){
    if(argc>1){
        assert(argc==4);const auto kind=static_cast<sq::Query>(std::stoul(argv[1]));
        const auto present=std::stoul(argv[2]);assert(present<=1);
        std::int32_t word=signed_word(std::uint32_t(std::stoull(argv[3])));
        sq::Machine machine{present?&word:nullptr};sq::Result r{};
        const auto status=sq::query(kind,&machine,&r);print(status,r);return 0;
    }
    unsigned cases=0,guards=0;
    auto check=[&](sq::Machine& machine,std::uint32_t expected){
        const auto original=machine.current_state_id;
        const auto old=original?*original:0;
        sq::Result r{123,456};assert(sq::is_using_skill(&machine,&r)==sq::Status::complete);
        assert(r.state_word==expected&&r.value==(expected==6));++cases;
        assert(sq::is_casting(&machine,&r)==sq::Status::complete);
        assert(r.state_word==expected&&r.value==(expected==7));++cases;
        assert(machine.current_state_id==original&&(!original||*original==old));
    };
    for(std::uint32_t word=0;word<32;++word){std::int32_t id=signed_word(word);sq::Machine machine{&id};check(machine,word);}
    for(auto word:std::array<std::uint32_t,15>{0xffffffffu,0x80000000u,0x7fffffffu,0x80000006u,0x80000007u,0x10006u,0x10007u,0xfffffffeu,0xdeadbeefu,42,65535,0xffff0006u,0xffff0007u,0x3f800000u,0x7fc00000u}){
        std::int32_t id=signed_word(word);sq::Machine machine{&id};check(machine,word);
    }
    // Replaced pointer, changed pointee and absent current state are read fresh
    // on each invocation; no stale skill/casting predicate is retained.
    std::int32_t first=6,second=7;sq::Machine machine{&first};
    check(machine,6);machine.current_state_id=&second;check(machine,7);
    machine.current_state_id=nullptr;check(machine,0xffffffffu);
    second=3;machine.current_state_id=&second;check(machine,3);
    second=-1;check(machine,0xffffffffu);first=6;machine.current_state_id=&first;check(machine,6);
    sq::Result output{0xa5a5a5a5u,0x5a5a5a5au};
    auto invalid=[&](sq::Query kind,const sq::Machine* m,sq::Result* r){
        const auto before=output;assert(sq::query(kind,m,r)==sq::Status::invalid_argument);
        assert(output.state_word==before.state_word&&output.value==before.value);++guards;
    };
    invalid(sq::Query::using_skill,nullptr,&output);invalid(sq::Query::casting,&machine,nullptr);
    invalid(static_cast<sq::Query>(2),&machine,&output);invalid(static_cast<sq::Query>(0xffffffffu),&machine,&output);
    alignas(16)std::array<unsigned char,128> bytes{};const auto bad=bytes.data()+1;
    invalid(sq::Query::using_skill,reinterpret_cast<const sq::Machine*>(bad),&output);
    invalid(sq::Query::using_skill,&machine,reinterpret_cast<sq::Result*>(const_cast<unsigned char*>(bad)));
    sq::Machine bad_state{reinterpret_cast<const std::int32_t*>(bad)};
    invalid(sq::Query::using_skill,&bad_state,&output);
    invalid(sq::Query::using_skill,&machine,reinterpret_cast<sq::Result*>(&machine));
    sq::Machine state_in_result{reinterpret_cast<const std::int32_t*>(&output)};
    invalid(sq::Query::using_skill,&state_in_result,&output);
    sq::Machine state_in_machine{nullptr};state_in_machine.current_state_id=reinterpret_cast<const std::int32_t*>(&state_in_machine);
    invalid(sq::Query::using_skill,&state_in_machine,&output);
    const auto maximum=std::numeric_limits<std::uintptr_t>::max();
    invalid(sq::Query::using_skill,reinterpret_cast<const sq::Machine*>(maximum-(alignof(sq::Machine)-1)),&output);
    invalid(sq::Query::using_skill,&machine,reinterpret_cast<sq::Result*>(maximum-(alignof(sq::Result)-1)));
    sq::Machine overflowing_state{reinterpret_cast<const std::int32_t*>(maximum-(alignof(std::int32_t)-1))};
    invalid(sq::Query::using_skill,&overflowing_state,&output);
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"guard_cases\":"<<guards<<",\"mismatches\":0}\n";
}
