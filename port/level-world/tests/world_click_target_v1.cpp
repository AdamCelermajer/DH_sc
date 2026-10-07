#include "../world_click_target_v1.hpp"
#include <cassert>
#include <limits>
#include <iostream>
using namespace dh2::world;
struct Fixture {
 std::uint8_t pending=165,clicked{};std::int16_t skill=123;
 float destination[3]{},origin[3]{},radius=1;
 dh2::character::TargetOwner16 target_owner{1,0,0,0};
 dh2::character::TargetState48 target{11,&target_owner,0,0,0,0,0,0,0,0};
 bool allowed=true,casting{},using_skill{},mode{},option{};unsigned target_failure=99;
 std::vector<std::uintptr_t> chars{2},objects;
 std::vector<std::string> calls;
 static int target_call(void* p,dh2::character::TargetState48*,const dh2::character::TargetRequest24* q,std::uint32_t* out){auto& f=*static_cast<Fixture*>(p);f.calls.push_back("targetservice:"+std::to_string(q->service));*out=q->service==dh2::character::target_in_sight?1:0;return q->service==f.target_failure?1:0;}
 WorldClickFieldsV1 fields(){return {1,&pending,&skill,destination,origin,&clicked,&target,{this,target_call}};}
 WorldClickServicesV1 services(){
  WorldClickServicesV1 s;s.enemy_radius2c=s.friend_radius50=s.object_radius54=&radius;
  s.ctrl_allowed=[this](bool& v,std::string&){calls.push_back("allowed");v=allowed;return true;};
  s.application_mode320e74=[this](bool& v,std::string&){calls.push_back("mode");v=mode;return true;};
  s.is_casting=[this](bool& v,std::string&){calls.push_back("casting");v=casting;return true;};
  s.is_using_skill=[this](bool& v,std::string&){calls.push_back("using");v=using_skill;return true;};
  s.characters=[this](bool first,std::uintptr_t& cursor,std::uintptr_t& id,std::string&){cursor=first?0:cursor+1;id=cursor<chars.size()?chars[cursor]:0;return true;};
  s.objects=[this](bool first,std::uintptr_t& cursor,std::uintptr_t& id,std::string&){cursor=first?0:cursor+1;id=cursor<objects.size()?objects[cursor]:0;return true;};
  s.target_position=[this](std::uintptr_t id,std::array<float,3>& out,std::string&){calls.push_back("position:"+std::to_string(id));out={10,20,30};return true;};
  s.interactive=[this](std::uintptr_t,std::uintptr_t owner,bool& out,std::string&){assert(owner==1);calls.push_back("interactive");out=true;return true;};
  s.neutral=[this](std::uintptr_t,std::uintptr_t,bool& out,std::string&){calls.push_back("neutral");out=false;return true;};
  s.enemy=[this](std::uintptr_t,std::uintptr_t,bool& out,std::string&){calls.push_back("enemy");out=true;return true;};
  s.nearby=[this](std::uintptr_t,const std::array<float,3>& p,float value,bool& out,std::string&){assert(value==radius);const float box[6]{9,19,29,11,21,31};out=world_click_nearby_v1(box,p.data(),value);calls.push_back("nearby");return true;};
  s.object_type_name5c=[](std::uintptr_t id,const char*& name,std::string&){name=id==3?"Block":"Chest";return true;};
  s.debug=[this](std::uint32_t site,bool& out,std::string&){calls.push_back("debug:"+std::to_string(site));out=option;return true;};
  s.move=[this](const std::array<float,3>&,bool released,std::string&){calls.push_back(released?"release_move":"move");return true;};return s;
 }
};
int main(){const std::array<float,3> point{10,20,30};std::string e;
 {Fixture f;f.allowed=false;auto fields=f.fields();assert(world_click_target_v1(fields,point,false,f.services(),e)&&f.pending==165&&f.skill==123&&f.calls==std::vector<std::string>({"allowed"}));}
 for(bool released:{false,true}){Fixture f;f.casting=true;auto fields=f.fields();assert(world_click_target_v1(fields,point,released,f.services(),e)&&f.pending==!released&&f.skill==-1&&f.target.target==0);if(!released)assert(f.destination[2]==30&&f.origin[0]==10);}
 {Fixture f;auto fields=f.fields();assert(world_click_target_v1(fields,point,false,f.services(),e)&&f.target.target==2&&f.target.last_target==2&&f.clicked==1&&f.pending==0&&f.skill==-1&&f.target.alive==1);}
 {Fixture f;f.chars={2,4};auto fields=f.fields();assert(world_click_target_v1(fields,point,false,f.services(),e)&&f.target.target==2);}// equal-distance later actor never replaces first
 {Fixture f;f.target_failure=dh2::character::target_owner_ai_id;auto fields=f.fields();assert(!world_click_target_v1(fields,point,false,f.services(),e)&&f.clicked==1&&f.target.candidate==2&&f.target.target==2);}
 {Fixture f;f.chars.clear();f.mode=true;auto fields=f.fields();assert(world_click_target_v1(fields,point,false,f.services(),e)&&f.target.target==0&&f.calls.back()=="debug:"+std::to_string(0x3ae208));}
 {Fixture f;f.chars.clear();auto fields=f.fields();assert(world_click_target_v1(fields,point,false,f.services(),e)&&f.calls.back()=="move"&&f.target.target==0&&f.target.last_target==0);}
 {Fixture f;f.chars.clear();f.objects={3,4};auto fields=f.fields();assert(world_click_target_v1(fields,point,true,f.services(),e)&&f.target.target==4&&f.clicked==0);}// Block excluded; actual generic receiver chosen
 {Fixture f;auto fields=f.fields();auto services=f.services();services.neutral={};assert(!world_click_target_v1(fields,point,false,services,e)&&f.target.target==0&&f.pending==0&&e.find("AI_IsNeutral")!=std::string::npos);}
 const float box[6]{-1,-1,-1,1,1,1},edge[3]{51,51,51};assert(world_click_nearby_v1(box,edge,1));float invalid[3]{0,0,std::numeric_limits<float>::quiet_NaN()};assert(!world_click_nearby_v1(box,invalid,1));
 std::cout<<"world Ctrl_Click PASS same fields/whole target setter + declared world predicates; no live input binding claim\n";
}
