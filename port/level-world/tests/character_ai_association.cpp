#include "../character_ai_association.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
namespace a=dh2::character_ai_association;
using State=dh2::character_ai_initialization::State;
int main(int argc,char** argv) {
    if(argc==3) {
        State state;std::memset(&state,std::atoi(argv[1]),sizeof state);state.identity=0x10010000;
        State before;std::memcpy(&before,&state,sizeof state);
        const auto owner=static_cast<std::uintptr_t>(std::strtoull(argv[2],nullptr,0));
        const auto status=a::associate(&state,owner);
        State expected;std::memcpy(&expected,&before,sizeof state);expected.owner_04=owner;
        assert(!std::memcmp(&state,&expected,sizeof state));
        std::cout<<"{\"status\":"<<static_cast<int>(status)<<",\"owner\":"<<state.owner_04
                 <<",\"other_fields_unchanged\":true}\n";return 0;
    }
    State state{};state.identity=1;state.owner_04=0x200000001ULL;
    assert(a::associate(&state,0x300000002ULL)==a::Status::complete);
    assert(state.owner_04==0x300000002ULL&&state.active_ais_1c==0);unsigned cases=1;
    assert(a::associate(&state,0)==a::Status::invalid_argument&&state.owner_04==0x300000002ULL);++cases;
    assert(a::associate(nullptr,1)==a::Status::invalid_argument);++cases;
    state.identity=0;assert(a::associate(&state,1)==a::Status::invalid_argument&&state.owner_04==0x300000002ULL);++cases;
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<"}\n";
}
