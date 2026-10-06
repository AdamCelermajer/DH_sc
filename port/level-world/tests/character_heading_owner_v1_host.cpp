#include "../character_heading_owner_v1.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
unsigned checks{};void check(bool b){++checks;if(!b)throw std::runtime_error("heading check failed "+std::to_string(checks));}
struct Probe {std::vector<unsigned> calls;bool remote{},physics{},fail{};float self[3]{},other[3]{-100,0,0};TargetState48* target{};};
bool position(void* p,std::uintptr_t id,const float*& out,std::string&){auto& q=*static_cast<Probe*>(p);q.calls.push_back(id==1?11:12);out=id==1?q.self:q.other;return true;}
bool remote(void* p,bool& out,std::string&){auto& q=*static_cast<Probe*>(p);q.calls.push_back(20);out=q.remote;return true;}
bool physics(void* p,bool& out,std::string&){auto& q=*static_cast<Probe*>(p);q.calls.push_back(21);out=q.physics;return true;}
bool event(void* p,unsigned id,std::string&){auto& q=*static_cast<Probe*>(p);q.calls.push_back(100+id);return !q.fail;}
int target(void* p,TargetState48*,const TargetRequest24* request,unsigned* out){auto& q=*static_cast<Probe*>(p);q.calls.push_back(30+request->service);*out=0;return 0;}
int main(){try{ControllerCommandState32 command{99,1,0,0,0,0};State state;state.current=3;TargetState48 targets{};targets.identity=101;Probe probe;probe.target=&targets;TargetServices16 ts{&probe,target};dh2::navigation::PathController object{};dh2::navigation::PathObject path{};dh2::physical::NativeBody* body{};
 TargetOwner16 target_owner{1,0,0,0};targets.owner=&target_owner;
 CharacterHeadingOwnerV1 owner(command,state,targets,ts,object,path,object.position,object.destination,body,{&probe,position,remote,physics,event});std::string error;const float heading[]{.25f,.5f,0};const float zero[]{0,0,0};
 for(unsigned blocked=0;blocked<2;++blocked)for(unsigned locked=0;locked<2;++locked)for(unsigned forced=0;forced<2;++forced){command.global_blocked=blocked;command.locked=locked;command.forced=forced;probe.calls.clear();check(owner.command_head_towards(heading,error));check(probe.calls.empty()==(!forced&&(blocked||locked)));}
 command.global_blocked=command.locked=command.forced=0;
 for(int s:{6,7}){state.current=s;probe.calls.clear();check(owner.command_head_towards(heading,error));check(probe.calls.empty());}state.current=3;
 probe.calls.clear();object.heading.active=0;check(owner.command_head_towards(zero,error));check(probe.calls.empty());
 object.position[0]=123;object.position[1]=456;object.heading.active=1;object.path_requested=1;probe.calls.clear();check(owner.command_head_towards(zero,error));check(probe.calls==std::vector<unsigned>({20,163}));check(object.destination[0]==123&&object.destination[1]==456&&!object.heading.active&&!object.path_requested);
 object.heading.active=1;probe.remote=true;probe.calls.clear();check(owner.command_stop(error));check(probe.calls==std::vector<unsigned>({20})&&object.heading.active);probe.remote=false;
 targets.target=targets.last_target=2;const float away[]{1,0,0};probe.calls.clear();check(owner.command_head_towards(away,error));check(!targets.target&&!targets.last_target);check(probe.calls[0]==12&&probe.calls[1]==11&&probe.calls.back()==100);check(object.heading.active&&object.heading.direction[0]==1);
 targets.target=2;probe.other[0]=100;probe.calls.clear();check(owner.command_head_towards(away,error));check(targets.target==2&&probe.calls==std::vector<unsigned>({12,11,100}));
 probe.fail=true;probe.calls.clear();error.clear();check(!owner.command_stop(error));check(!object.heading.active&&object.destination[0]==123&&probe.calls.back()==163&&!error.empty());
 command.owner=targets.identity;object.heading.active=1;probe.calls.clear();error.clear();
 check(!owner.command_head_towards(away,error));check(probe.calls.empty()&&object.heading.active);
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<"}\n";
 }catch(const std::exception& e){std::cerr<<e.what();return 1;}}
