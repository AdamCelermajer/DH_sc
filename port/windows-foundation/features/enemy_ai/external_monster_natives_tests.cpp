#include "external_monster_natives.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation::enemy_ai;
void check(bool v,const char* e){if(!v)throw std::runtime_error(e);}
int main(){try{
 ExternalMonsterDoSkill b;b.character=7;b.receiver_lease=std::make_shared<int>(0);
 std::vector<std::string> trace;std::uint32_t limit=1;bool failure=false;
 b.get_unsigned=[&](const dh2_script_value& a,std::uint32_t& n,std::string&){trace.push_back("unsigned");check(a.number==0,"argument changed");n=0;return true;};
 b.skill_count=[&](std::uint32_t& n,std::string&){trace.push_back("count");n=limit;return true;};
 b.get_number_integer=[&](const dh2_script_value&,std::int32_t& n,std::string&){trace.push_back("number");n=0;return true;};
 b.use_skill=[&](std::int32_t n,std::string& e){trace.push_back("use");check(n==0,"NPC instance index remapped");if(failure)e="required original Check callback";return !failure;};
 dh2_script_value arg{};arg.type=DH2_SCRIPT_NUMBER;std::uint32_t returned=99;char error[200]{};
 check(external_monster_do_skill(&b,&arg,1,nullptr,0,&returned,error,sizeof(error))==0&&returned==0&&trace==std::vector<std::string>{"unsigned","count","number","use"},"native wrapper order");
 trace.clear();limit=0;check(external_monster_do_skill(&b,&arg,1,nullptr,0,&returned,error,sizeof(error))==0&&trace==std::vector<std::string>{"unsigned","count"},"invalid native index admitted");
 trace.clear();arg.type=DH2_SCRIPT_BOOLEAN;check(external_monster_do_skill(nullptr,&arg,1,nullptr,0,&returned,error,sizeof(error))==0&&trace.empty(),"source guard touched unavailable owner");
 arg.type=DH2_SCRIPT_NUMBER;limit=1;failure=true;
 check(external_monster_do_skill(&b,&arg,1,nullptr,0,&returned,error,sizeof(error))==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&std::string(error)=="required original Check callback","Check failure substituted");
 dh2_script_function fn{};void* context{};check(select_external_monster_do_skill(&b,0x3b9fbc,&fn,&context)==0,"combat roll mistaken for DoSkill");check(select_external_monster_do_skill(&b,0x3b8bd8,&fn,&context)==1&&context==&b&&fn==external_monster_do_skill,"source address binding wrong");
 std::cout<<"PASS original DoSkill wrapper ordered conversions/native vector guard, instance0 use, missing Check failure and exact callback address\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
