#include "../player_target_out_range_v2.hpp"
#include <cstdio>
#include <vector>
using namespace dh2::character;
struct Fixture{bool seeking=true,path=false,created=false;int state=3,fail=0;std::vector<int> calls;};
int main(){int checks=0;auto run=[&](Fixture& f){
 PlayerTargetOutRangeServicesV2 s{};s.context=&f;
 s.seeking=[](void* p,bool& out){auto& x=*static_cast<Fixture*>(p);x.calls.push_back(1);out=x.seeking;return 0;};
 s.state=[](void* p,std::int32_t& out){auto& x=*static_cast<Fixture*>(p);x.calls.push_back(2);out=x.state;return 0;};
 s.has_path=[](void* p,bool& out){auto& x=*static_cast<Fixture*>(p);x.calls.push_back(3);out=x.path;return 0;};
 s.interaction_spot=[](void* p,float* out){auto& x=*static_cast<Fixture*>(p);x.calls.push_back(4);out[0]=9;out[1]=11;out[2]=13;return x.fail==4?-1:0;};
 s.move_point=[](void* p,const float* in){auto& x=*static_cast<Fixture*>(p);x.calls.push_back(5);if(in[0]!=9||in[1]!=11||in[2]!=13)return -1;x.path=x.created;return x.fail==5?-1:0;};
 s.clear_target=[](void* p){auto& x=*static_cast<Fixture*>(p);x.calls.push_back(6);return x.fail==6?-1:0;};
 s.sync_last=[](void* p){auto& x=*static_cast<Fixture*>(p);x.calls.push_back(7);return 0;};
 s.stop_seeking=[](void* p){auto& x=*static_cast<Fixture*>(p);x.calls.push_back(8);x.seeking=false;return 0;};
 std::string error;return player_target_out_range_v2(s,error);
 };
 auto check=[&](bool ok){++checks;if(!ok){std::printf("FAIL %d\n",checks);return false;}return true;};
 Fixture off;off.seeking=false;if(!check(run(off)==0&&off.calls==std::vector<int>{1}))return 1;
 for(int state:{3,5,13,18}){Fixture f;f.state=state;if(!check(run(f)==0&&f.calls==std::vector<int>{1,2,4,5,3,6,7,8}&&!f.seeking))return 1;}
 Fixture follow;follow.state=4;follow.path=true;if(!check(run(follow)==0&&follow.calls==std::vector<int>{1,2,3}))return 1;
 Fixture create;create.created=true;if(!check(run(create)==0&&create.calls==std::vector<int>{1,2,4,5,3}&&create.seeking))return 1;
 for(int fail:{4,5,6}){Fixture f;f.fail=fail;if(!check(run(f)<0&&f.calls.back()==fail&&f.seeking))return 1;}
 std::printf("PASS %d source pursuit ordering/gates/reentry/failure-prefix checks\n",checks);return 0;
}
