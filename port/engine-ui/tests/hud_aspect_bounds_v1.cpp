#include "viewport.hpp"
#include <array>
#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace dh2::ui;
namespace {
unsigned checks{};
void require(bool value,const char* message){++checks;if(!value)throw std::runtime_error(message);}
int service(void*,ViewportState64*,const ViewportRequest40* request,ViewportResponse16* response){
 if(request->operation==ViewportOperation::orientation){response->values[0]=0;return 1;}
 return request->operation==ViewportOperation::publish_viewport;
}
}
int main(){try{
 ViewportServices16 services{nullptr,service};
 for(const auto size:std::array<std::array<int,2>,6>{{{480,320},{800,480},{854,480},{1920,1080},{2400,1080},{2560,1600}}}){
  ViewportState64 state{{0,9600,0,6400},{0,0,size[0],size[1]},{0,0,480,320},1,0,1};
  const std::int32_t bounds[]{0,0,size[0],size[1]};
  require(dh2_ui_set_bounds(&state,bounds,2,&services)==0,"actual aspect-fit bounds rejected");
  require(state.bounds[0]>=0&&state.bounds[1]>=0&&state.bounds[0]+state.bounds[2]<=size[0]&&state.bounds[1]+state.bounds[3]<=size[1],"authored stage exceeds physical viewport");
  float rectangle[4]{};
  require(dh2_ui_display_rectangle(&state,rectangle,&services)==0,"actual display projection rejected");
  const float sx=size[0]/(rectangle[1]-rectangle[0]),sy=size[1]/(rectangle[3]-rectangle[2]);
  require(std::abs(120.f*20.f*(sx-sy))<.3f,"circular HUD artwork has unequal physical scale");
  for(const auto logical:std::array<std::array<float,2>,4>{{{73.2f,246.f},{425.f,260.f},{240.f,160.f},{40.f,24.f}}}){
   float physical[]{(logical[0]*20.f-rectangle[0])*sx,(logical[1]*20.f-rectangle[2])*sy};
   require(dh2_ui_screen_to_logical(&state,physical,&services)==0,"actual pointer projection rejected");
   require(std::abs(physical[0]-logical[0])<.0002f&&std::abs(physical[1]-logical[1])<.0002f,"drawn HUD position and touch position disagree");
  }
 }
 std::cout<<"PASS "<<checks<<" checks: actual aspect-fit viewport, circular scale and shared draw/touch projection across six display sizes\n";
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
