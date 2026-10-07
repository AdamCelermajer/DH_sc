#include "module_fog_v67.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
namespace dh2::world {namespace {
float distance(const float* a,const float* b){
 const float x=a[0]-b[0],y=a[1]-b[1],z=a[2]-b[2];
 const float xx=x*x,yy=y*y,zz=z*z;const float xy=xx+yy;return std::sqrt(xy+zz);
}
bool valid(const std::vector<ModuleFogBorrowV67>& modules,std::string& e){
 for(const auto& module:modules)if(!module.identity||!module.position160||!module.color3f0){e="Required SAME source Module position160/fog3f0";return false;}return true;
}
bool unset(const std::array<float,3>& color){return color[0]==-1&&color[1]==-1&&color[2]==-1;}
}
bool source_init_module_fog_v67(const std::vector<ModuleFogBorrowV67>& modules,
 const std::vector<std::array<float,3>>& input,std::int32_t range,
 const std::function<bool(std::int32_t&,std::string&)>& random,std::string& e){
 if(input.empty()){e.clear();return true;} // whole347138 empty-vector branch
 if(!valid(modules,e))return false;
 if(modules.empty()){e="Source InitModulesFogColor reached empty Module68-list dereference";return false;}
 if(input.size()>4096||modules.size()>65536){e="Module fog request outside native admission domain";return false;}
 // Original copies/shuffles colors before walking any Module, including when
 // all module colors are already authored overrides. Preserve lrand48 call order.
 auto colors=input;
 for(std::size_t i=1;i<colors.size();++i){
  std::int32_t value{};if(!random||!random(value,e)){if(e.empty())e="Required actual libc lrand48 source";return false;}
  if(value<0){e="Original libc lrand48 returned outside nonnegative source domain";return false;}
  std::swap(colors[i],colors[static_cast<std::uint32_t>(value)%(i+1)]);
 }
 auto ordered=modules;const auto* origin=modules.front().position160;
 std::sort(ordered.begin(),ordered.end(),[origin](const auto& a,const auto& b){return distance(origin,a.position160)<distance(origin,b.position160);});
 const auto bits=std::uint32_t(range)*100u;std::int32_t divisor;std::memcpy(&divisor,&bits,4);
 for(const auto& module:ordered){
  const float actual_distance=distance(origin,module.position160);
  if(!unset(*module.color3f0))continue;
  if(!std::isfinite(actual_distance)||actual_distance>=2147483648.f||actual_distance<-2147483648.f||!divisor){e="Source fog color band division outside checked native domain";return false;}
  const auto band=static_cast<std::int32_t>(actual_distance)/divisor;
  const auto index=band%static_cast<std::int32_t>(colors.size());
  if(index<0){e="Original fog shuffle selected negative source vector index";return false;}
  *module.color3f0=colors[static_cast<std::size_t>(index)];
 }
 e.clear();return true;
}
bool source_dynamic_module_fog_v67(const std::vector<ModuleFogBorrowV67>& modules,
 const float* position,float range,std::array<float,3>& output,std::string& e){
 if(!position||!valid(modules,e)){if(e.empty())e="Required source fog position";return false;}
 const float radius=range*100.f;std::array<float,3> color{0,0,0};
 for(const auto& module:modules){
  const float d=distance(module.position160,position);
  if(!(radius>d)||unset(*module.color3f0))continue;
  const float difference=radius-d;const float factor=difference/radius;
  const float y=(*module.color3f0)[1]*factor,z=(*module.color3f0)[2]*factor,x=(*module.color3f0)[0]*factor;
  color[0]=color[0]+x;color[1]=color[1]+y;color[2]=color[2]+z;
 }
 for(auto& channel:color)if(channel>255.f)channel=255.f;
 output=color;e.clear();return true;
}
}
