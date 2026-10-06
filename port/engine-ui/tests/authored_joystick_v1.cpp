#include "../authored_joystick_v1.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <vector>
using namespace dh2::ui;
unsigned checks{};void require(bool b,const std::string& e){++checks;if(!b)throw std::runtime_error(e);}
struct Observer{std::int32_t x{},y{};bool allowed{},reject_position{};std::vector<int> calls;float heading[3]{};
 static bool position(void* p,int x,int y,std::string& e){auto& o=*static_cast<Observer*>(p);o.calls.push_back(1);o.x=x;o.y=y;if(o.reject_position){e="required position failure";return false;}return true;}
 static bool control(void* p,bool& out,std::string&){auto& o=*static_cast<Observer*>(p);o.calls.push_back(2);out=o.allowed;return true;}
 static bool rotate(void* p,const float* base,float,float* out,std::string&){auto& o=*static_cast<Observer*>(p);o.calls.push_back(3);require(base[0]==1&&base[1]==-1&&base[2]==0,"source base vector");out[0]=.6f;out[1]=.8f;out[2]=0;return true;}
 static bool head(void* p,const float* v,std::string&){auto& o=*static_cast<Observer*>(p);o.calls.push_back(4);std::memcpy(o.heading,v,12);return true;}
 static bool stop(void* p,std::string&){static_cast<Observer*>(p)->calls.push_back(5);return true;}
 AuthoredJoystickServicesV1 services(){return {this,position,control,rotate,head,stop};}
};
int main(){try{std::ifstream f("port/engine-ui/reference/authored-joystick-v1/fixtures.bin",std::ios::binary);require(bool(f),"gold missing");unsigned count{};f.read(reinterpret_cast<char*>(&count),4);std::string e;
 for(unsigned n=0;n<count;++n){std::int32_t state[4];float values[4];unsigned expected[3];f.read(reinterpret_cast<char*>(state),16);f.read(reinterpret_cast<char*>(values),16);f.read(reinterpret_cast<char*>(expected),12);require(bool(f),"gold truncated");
  AuthoredJoystickStateV1 s;s.radius_x=state[0];s.radius_y=state[1];s.center_x=state[2];s.center_y=state[3];Observer o;auto services=o.services();require(authored_joystick_drag_v1(s,values[0],values[1],values[2],values[3],services,e),e);unsigned magnitude;std::memcpy(&magnitude,&s.magnitude,4);
  require(magnitude==expected[0]&&static_cast<unsigned>(o.x)==expected[1]&&static_cast<unsigned>(o.y)==expected[2],"original joystick numeric mismatch case"+std::to_string(n));require(o.calls==std::vector<int>{1,2}&&!s.active,"original ordered callbacks/CTRL false");
 }
 AuthoredJoystickStateV1 s;Observer o;auto services=o.services();require(!authored_joystick_drag_v1(s,0,0,0,0,services,e)&&o.calls.empty(),"zero radius prefix");
 require(authored_joystick_initialize_v1(s,2000,e)&&s.radius_x==50&&s.radius_y==50,"source bg radius");require(authored_joystick_press_v1(s,21,-21,e)&&s.center_x==1&&s.center_y==-1,"source press truncation");
 o.allowed=true;require(authored_joystick_drag_v1(s,21,-21,0,0,services,e)&&s.active&&s.magnitude==0,"zero drag source active/magnitude");o.calls.clear();require(authored_joystick_update_v1(s,services,e)&&o.calls==std::vector<int>{2,4}&&o.heading[0]==0&&o.heading[1]==0,"source no deadzone zero vector");
 s.magnitude=.5f;require(authored_joystick_update_v1(s,services,e)&&o.heading[0]==.3f&&o.heading[1]==.4f,"source scaled heading");o.calls.clear();auto before=s.magnitude;require(authored_joystick_release_v1(s,true,services,e)&&!s.active&&!s.center_x&&!s.center_y&&s.magnitude==before&&o.calls==std::vector<int>{1,5},"source release order/magnitude retained");
 s.active=1;s.center_x=2;s.center_y=3;o.reject_position=true;o.calls.clear();require(!authored_joystick_release_v1(s,true,services,e)&&s.active&&!s.center_x&&!s.center_y&&o.calls==std::vector<int>{1},"required position failure prefix");
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"original_cases\":"<<count<<",\"scope\":\"original full event5 numeric/position branch and source ordered services; direction producer is explicit host observer, outer event/level/player/attack gates external\"}\n";
 }catch(const std::exception& e){std::cerr<<e.what();return 1;}}
