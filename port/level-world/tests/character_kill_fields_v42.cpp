#include "../character_kill_fields_v21.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::character;
int main(){std::string error;CharacterKillFieldsV21 fresh;
 assert(fresh.construct_fresh(error)&&fresh.produced&&fresh.killer144c==0&&fresh.master14d4==0&&fresh.template13ca==-1&&fresh.suppress_quest14e4==0);
 fresh.killer144c=91;fresh.master14d4=82;fresh.template13ca=73;fresh.suppress_quest14e4=1;
 assert(!fresh.construct_fresh(error));assert(!fresh.adopt_observed(0,0,-1,0,error));
 assert(fresh.killer144c==91&&fresh.master14d4==82&&fresh.template13ca==73&&fresh.suppress_quest14e4==1);
 CharacterKillFieldsV21 observed;assert(!observed.produced);assert(observed.adopt_observed(11,22,33,1,error));assert(!observed.construct_fresh(error));assert(!observed.adopt_observed(0,0,-1,0,error));assert(observed.killer144c==11&&observed.master14d4==22&&observed.template13ca==33&&observed.suppress_quest14e4==1);
 std::cout<<"Character kill fresh/observed/replay preservation PASS 10 checks; retained actor wiring strict compiled separately\n";
}
