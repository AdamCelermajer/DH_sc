#include "../character_animation_swoosh_v4.hpp"
#include <vector>
#include <iostream>
#include <stdexcept>
using namespace dh2::character;
struct Fixture{int sound[2]{-1,-1},fx[2]{-1,-1};std::vector<int> calls;bool fail{};
 static int equipped(void* p,int kind,std::uintptr_t& v){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(kind);v=kind;return 0;}
 static int effects(void* p,std::uintptr_t item,int& sound,int& fx){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(10+item);sound=f.sound[item-1];fx=f.fx[item-1];return 0;}
 static int ps(void* p,int id){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(100+id);return f.fail?-1:0;}
 static int pf(void* p,int id,bool anchor){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(200+id+(anchor?1000:0));return f.fail?-1:0;}
};
int main(){try{unsigned n=0;auto check=[&](bool b){++n;if(!b)throw std::runtime_error("Swoosh source ordered branch");};
 for(int main=0;main<4;++main)for(int off=0;off<4;++off)for(bool anchor:{false,true}){Fixture f;f.sound[0]=main&1?3:-1;f.fx[0]=main&2?4:-1;f.sound[1]=off&1?5:-1;f.fx[1]=off&2?6:-1;
  AnimationSwooshServicesV4 s{&f,Fixture::equipped,Fixture::effects,Fixture::ps,Fixture::pf};bool sound=false,fx=false;std::string e;check(character_animation_swoosh_v4(anchor,s,sound,fx,e)==0);check(sound==(!(main&1)&&!(off&1)));check(fx==!(main&2));check(f.calls[0]==1&&f.calls[1]==2);
  std::vector<int> expected{1,2,11};if(main&1)expected.push_back(103);expected.push_back(11);if(main&2)expected.push_back(204+(anchor?1000:0));if(!(main&1)){expected.push_back(12);if(off&1)expected.push_back(105);}if(!(main&2)){expected.push_back(12);if(off&2)expected.push_back(206+(anchor?1000:0));}check(f.calls==expected);
 }
 Fixture f;f.sound[0]=3;f.fail=true;AnimationSwooshServicesV4 s{&f,Fixture::equipped,Fixture::effects,Fixture::ps,Fixture::pf};bool sound=true,fx=true;std::string e;check(character_animation_swoosh_v4(false,s,sound,fx,e)<0);check(f.calls==std::vector<int>({1,2,11,103}));check(sound&&fx);
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<n<<",\"original_arm_oracle\":false}"<<std::endl;return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}}
