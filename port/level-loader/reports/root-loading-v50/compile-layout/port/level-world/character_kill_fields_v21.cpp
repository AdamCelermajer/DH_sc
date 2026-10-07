#include "character_kill_fields_v21.hpp"
namespace dh2::character {
bool CharacterKillFieldsV21::construct_fresh(std::string& e){
 if(produced){e="Character Kill C1 fields cannot replay";return false;}
 // CharacterC1 r8=0/r6=-1: 3aa438 killer0,3aa354 template-1,
 // 3aa57c master0,3aa58c suppress0. C2 corresponding stores are identical.
 killer144c=master14d4=0;template13ca=-1;suppress_quest14e4=0;produced=true;return true;
}
bool CharacterKillFieldsV21::adopt_observed(std::uintptr_t killer,std::uintptr_t master,
 std::int16_t templ,std::uint8_t suppress,std::string& e){
 if(produced){e="Character Kill metadata adoption cannot replay";return false;}
 killer144c=killer;master14d4=master;template13ca=templ;suppress_quest14e4=suppress;produced=true;return true;
}
}
