#include "../character_skill_combat_v6.hpp"
static unsigned debug_calls=0;
static int debug_delivered(void*){++debug_calls;return 0;}
static unsigned ordered_result(dh2::data::CombatResult* o,const dh2::data::CombatResultRequest* r){return dh2::character::skills::dh2_combat_result_ordered_v6(o,r,nullptr,debug_delivered);}
#define dh2_combat_result ordered_result
#define main original_result_fixture_main
#include "../../game-data/tests/combat_result.cpp"
#undef main
#undef dh2_combat_result
static int debug_missing(void*){return -1;}
int main(int argc,char** argv){
 const int corpus=original_result_fixture_main(argc,argv);if(corpus)return corpus;
 std::array<std::int32_t,224> props{};dh2::data::CombatantView actor{props.data()};
 dh2::data::CombatRandom random{123,456};dh2::data::CombatResult result;
 dh2::data::CombatResultRequest request{&actor,&actor,&random,0x8040000u,-1,2,0};
 check(dh2::character::skills::dh2_combat_result_ordered_v6(&result,&request,nullptr,debug_missing)==2,"Required Debug failure accepted");
 check(result.amount==-1&&result.mask==request.mask&&result.element==2,"Reached result prefix lost");
 check(random.seed==123&&random.calls==456,"Damage RNG advanced beyond required Debug failure");
 check(debug_calls>0,"Debug continuation never reached");
 std::cout<<"{\"validation\":\"PASS\",\"ordered_debug_calls\":"<<debug_calls<<",\"required_debug_prefix\":true}\n";
}
