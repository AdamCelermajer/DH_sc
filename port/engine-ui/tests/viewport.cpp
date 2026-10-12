#include "viewport.hpp"
#include <array>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
using namespace dh2::ui;
namespace {
std::uint32_t u32(const unsigned char* p){std::uint32_t v;std::memcpy(&v,p,4);return v;}
bool equal(const unsigned char* a,const unsigned char* b,std::size_t n){for(std::size_t k=0;k<n;k+=4){auto x=u32(a+k),y=u32(b+k);float f,g;std::memcpy(&f,&x,4);std::memcpy(&g,&y,4);if(x!=y&&!(std::isnan(f)&&std::isnan(g)))return false;}return true;}
struct Context {std::int32_t facts[4];unsigned orientation_count=0;std::vector<ViewportRequest40> calls;int fail=0;bool reenter=false;int nested=-99;ViewportServices16* services=nullptr;};
int invoke(void* p,ViewportState64* s,const ViewportRequest40* r,ViewportResponse16* out){auto& c=*static_cast<Context*>(p);c.calls.push_back(*r);if(c.fail==int(r->operation))return 0;
 if(r->operation==ViewportOperation::orientation)out->values[0]=c.facts[c.orientation_count++?1:0];
 if(r->operation==ViewportOperation::driver_dimensions){out->values[0]=c.facts[2];out->values[1]=c.facts[3];}
 if(r->operation==ViewportOperation::publish_viewport&&c.reenter){c.reenter=false;std::int32_t b[4]={7,9,640,480};c.nested=dh2_ui_set_bounds(s,b,0,c.services);}
 return 1;}
int run(unsigned op,ViewportState64& s,FlashCamera40& camera,const std::int32_t* params,float* point,unsigned mode,ViewportServices16& services){switch(op){case 0:return dh2_ui_set_bounds(&s,params,std::int32_t(mode),&services);case 1:return dh2_ui_set_viewport(&s,params,&services);case 2:return dh2_ui_screen_to_logical(&s,point,&services);case 3:return dh2_ui_logical_to_screen(&s,point,&services);case 4:return dh2_ui_flash_camera_update(&camera,&s,&services);case 5:return dh2_ui_display_rectangle(&s,point,&services);default:return -99;}}
}
int main(int argc,char** argv){if(argc!=2)return 2;std::ifstream f(argv[1],std::ios::binary);std::vector<unsigned char> data((std::istreambuf_iterator<char>(f)),{});if(data.size()<8||u32(data.data())!=0x31505756)return 2;const unsigned count=u32(data.data()+4);std::size_t at=8;unsigned services_count=0;
 for(unsigned i=0;i<count;++i){if(at+12>data.size())return 2;auto input=u32(data.data()+at),output=u32(data.data()+at+4),events=u32(data.data()+at+8);at+=12;if(input!=152||output!=120||at+input+output+events*40>data.size())return 2;auto raw=data.data()+at;at+=input;auto expected=data.data()+at;at+=output;auto event_data=data.data()+at;at+=events*40;
  ViewportState64 s;FlashCamera40 camera;std::int32_t params[4];float point[4]{};Context c{};std::memcpy(&s,raw+4,64);std::memcpy(&camera,raw+68,40);std::memcpy(params,raw+108,16);std::memcpy(point,raw+124,8);std::memcpy(c.facts,raw+132,16);ViewportServices16 svc{&c,invoke};int result=run(u32(raw),s,camera,params,point,u32(raw+148),svc);
  std::array<unsigned char,120> actual{};std::memcpy(actual.data(),&s,64);std::memcpy(actual.data()+64,&camera,40);std::memcpy(actual.data()+104,point,16);
  if(result||!equal(actual.data(),expected,120)||c.calls.size()!=events){std::cerr<<"viewport record "<<i<<" mismatch\n";return 1;}
  for(unsigned k=0;k<events;++k){if(!equal(reinterpret_cast<const unsigned char*>(&c.calls[k]),event_data+k*40,40)){std::cerr<<"viewport service record "<<i<<" mismatch\n";return 1;}}
  services_count+=events;
 }
 if(at!=data.size())return 2;
 ViewportState64 s{{0,9600,0,6400},{0,0,480,320},{0,0,480,320},1,0,1};FlashCamera40 camera{};Context c{};c.facts[2]=1080;c.facts[3]=1920;ViewportServices16 svc{&c,invoke};c.services=&svc;std::int32_t b[4]={-10,-20,1080,1920};float p[2]={11,17};unsigned guards=0;
 auto check=[&](bool ok){if(!ok)throw std::string("guard mismatch");++guards;};
 try{
  auto before=s;c.fail=1;check(dh2_ui_set_bounds(&s,b,0,&svc)==-2&&std::memcmp(&s,&before,64)==0);
  check(dh2_ui_set_viewport(&s,b,&svc)==-2&&std::memcmp(s.viewport,b,16)==0);
  check(dh2_ui_screen_to_logical(&s,p,&svc)==-2&&p[0]==11&&p[1]==17);
  c.fail=2;camera.desired[0]=100;check(dh2_ui_flash_camera_update(&camera,&s,&svc)==-2&&camera.current[0]==11);
  c.fail=3;check(dh2_ui_set_bounds(&s,b,0,&svc)==-2&&std::memcmp(s.bounds,b,16)==0);
  c.fail=0;c.reenter=true;b[0]=3;check(dh2_ui_set_bounds(&s,b,0,&svc)==0&&c.nested==0&&s.bounds[0]==7&&s.bounds[1]==9&&s.bounds[2]==640);
  check(dh2_ui_set_bounds(nullptr,b,0,&svc)==-1);
  check(dh2_ui_set_bounds(&s,reinterpret_cast<std::int32_t*>(&s),0,&svc)==-1);
  check(dh2_ui_screen_to_logical(&s,reinterpret_cast<float*>(&s),&svc)==-1);
  alignas(8) unsigned char unaligned[80]{};check(dh2_ui_set_bounds(reinterpret_cast<ViewportState64*>(unaligned+1),b,0,&svc)==-1);
  check(dh2_ui_flash_camera_update(reinterpret_cast<FlashCamera40*>(&s),&s,&svc)==-1);
  ViewportServices16 missing{};check(dh2_ui_set_bounds(&s,b,0,&missing)==-2);
 }catch(const std::string& error){std::cerr<<error<<'\n';return 1;}
 std::cout<<"{\"comparisons\":"<<count<<",\"ordered_services\":"<<services_count<<",\"failure_reentry_guards\":"<<guards<<",\"mismatches\":0}\n";
}
