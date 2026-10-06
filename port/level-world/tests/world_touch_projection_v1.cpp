#include "../world_touch_projection_v1.hpp"
#include "../world_click_fields_owner_v1.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::world;
int main(){
 WorldClickFieldsOwnerV1 owned;assert(owned.pending14c8==0&&owned.skill14ca==-1&&owned.click_target413==1&&owned.destination14b0[2]==0&&owned.origin14bc[1]==0);
 std::uint32_t normal=0,excluded=0x01000000;std::vector<std::uintptr_t> called;WorldTouchProjectionServicesV1 s;
 s.screen_ray=[](std::int32_t x,std::int32_t y,std::array<float,3>& start,std::array<float,3>& end,std::string&){assert(x==10&&y==-20);start={1,2,3};end={4,5,6};return true;};
 s.room_at=[](std::uint32_t index,bool& present,std::uintptr_t& room,std::string&){present=index<2;room=100+index;return true;};
 s.floor_at=[&](std::uintptr_t room,std::uint32_t index,bool& present,WorldTouchFloorBorrowV1& out,std::string&){present=index<2;out={room*10+index,index==0?&excluded:&normal};return true;};
 s.floor_collision=[&](std::uintptr_t id,const std::array<float,3>& start,const std::array<float,3>& end,bool& hit,std::array<float,3>& out,std::string&){assert(start[0]==1&&end[2]==6);called.push_back(id);hit=true;out={7,8,9};return true;};
 bool hit=false;std::array<float,3> point{99,99,99};std::string e;
 assert(world_touch_projection_v1(10.9f,-20.9f,s,hit,point,e)&&hit&&point[2]==9&&called==std::vector<std::uintptr_t>({1001}));
 called.clear();s.floor_collision=[&](std::uintptr_t id,const std::array<float,3>&,const std::array<float,3>&,bool& found,std::array<float,3>&,std::string&){called.push_back(id);found=false;return true;};
 assert(world_touch_projection_v1(10.9f,-20.9f,s,hit,point,e)&&!hit&&point[2]==9&&called==std::vector<std::uintptr_t>({1001,1011}));
 s.floor_collision={};assert(!world_touch_projection_v1(10.9f,-20.9f,s,hit,point,e)&&e.find("triangle-selector")!=std::string::npos);
 s.screen_ray={};assert(!world_touch_projection_v1(10,-20,s,hit,point,e)&&e.find("camera")!=std::string::npos);
 std::cout<<"world touch projection PASS real ctor field defaults + declared camera/PF vectors/triangle selector services, first-hit/filter/truncation/missing\n";
}
