#include "../object_enable_condition_v2.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::world;
struct Fixture {bool character=true,profile=false,level=true,truth=true,fail=false;std::uint8_t byte14=1;std::int32_t difficulty=0;int levels=0,conditions=0,events=0;};
static bool profile(void*p,bool& c,bool& r,std::uint8_t& v,std::string&){auto& f=*static_cast<Fixture*>(p);c=f.character;r=f.profile;v=f.byte14;return true;}
static bool level(void*p,bool& present,std::int32_t& d,std::string&){auto& f=*static_cast<Fixture*>(p);++f.levels;present=f.level;d=f.difficulty;return true;}
static bool condition(void*p,std::uintptr_t id,bool& v,std::string&){assert(id==17);auto& f=*static_cast<Fixture*>(p);++f.conditions;v=f.truth;return true;}
static bool event(void*p,bool,std::string& e){auto& f=*static_cast<Fixture*>(p);++f.events;if(f.fail){e="fixture selected virtual failure";return false;}return true;}
int main(){Fixture f;std::uint8_t enabled=1,disabled=0,tested=0;std::int32_t minimum=-1;std::uintptr_t compiled=0;bool out=false;std::string error;ObjectEnableConditionBorrowV2 b{&enabled,&minimum,&disabled,&compiled,&tested};ObjectEnableConditionServicesV2 s{&f,profile,level,condition,event};
 assert(object_test_enable_condition_v2(b,s,true,out,error)&&out&&!tested&&!f.levels);
 f.profile=true;assert(object_test_enable_condition_v2(b,s,true,out,error)&&out&&tested&&!f.conditions&&!f.events);
 tested=0;compiled=17;f.truth=false;assert(object_test_enable_condition_v2(b,s,true,out,error)&&!out&&!tested&&f.conditions==1&&f.events==1);
 f.truth=true;f.fail=true;assert(!object_test_enable_condition_v2(b,s,true,out,error)&&out&&enabled==1&&!tested&&f.events==2);
 f.fail=false;assert(object_test_enable_condition_v2(b,s,true,out,error)&&tested&&f.events==2);
 disabled=1;assert(object_test_enable_condition_v2(b,s,true,out,error)&&!out);
 disabled=0;minimum=1;assert(object_test_enable_condition_v2(b,s,true,out,error)&&!out);
 minimum=-1;f.level=false;assert(object_test_enable_condition_v2(b,s,false,out,error)&&out);
 std::cout<<"ObjectEnableConditionV2 source branches PASS (explicit profile/Level/virtual fixtures)\n";
}
