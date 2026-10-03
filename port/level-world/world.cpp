#include "world.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <stdexcept>
namespace dh2::world {
namespace {
unsigned word(const std::uint8_t* p){return p[0]|(unsigned(p[1])<<8)|(unsigned(p[2])<<16)|(unsigned(p[3])<<24);}
float value(const std::uint8_t* p){auto bits=word(p);float v;std::memcpy(&v,&bits,4);if(!std::isfinite(v)||std::abs(v)>10000000)throw std::runtime_error("World component exceeds limit");return v;}
void triangles(const resources::BresView& view,const scene::Instance& instance,unsigned room,Level& level){
 std::string error;if(!floors::append(view,level.scene,instance,room,*level.native_floor,error))throw std::runtime_error(error);
 const auto& record=*level.native_floor->records.back();if(level.floor.size()+record.triangles.size()>100000)throw std::runtime_error("Navigation triangle budget exceeded");
 for(const auto& raw:record.triangles){Triangle triangle{};triangle.room=room;std::copy(raw.points[0],raw.points[0]+3,triangle.a.begin());std::copy(raw.points[1],raw.points[1]+3,triangle.b.begin());std::copy(raw.points[2],raw.points[2]+3,triangle.c.begin());level.floor.push_back(triangle);}
}
}
bool load(const resources::BresView& view,const std::uint8_t* descriptor,std::size_t size,Level& out,std::string& error){
 out={};error.clear();try{
  if(!descriptor||size<24||std::memcmp(descriptor,"DWLD",4)||word(descriptor+4)!=1)throw std::runtime_error("World descriptor rejected");
  const auto count=word(descriptor+8);if(!count||count>512||size!=24+std::uint64_t(count)*128)throw std::runtime_error("World room table rejected");
  Level level;level.rooms=count;level.native_floor=std::make_unique<floors::World>();for(unsigned i=0;i<3;++i)level.spawn[i]=value(descriptor+12+i*4);
  scene::Scene source;if(!scene::load(view,source,error))throw std::runtime_error(error);level.scene.materials=source.materials;
  for(unsigned room=0;room<count;++room){const auto* record=descriptor+24+room*128;auto* end=static_cast<const std::uint8_t*>(std::memchr(record,0,112));
   if(!end||end==record||word(record+124))throw std::runtime_error("World room identifier rejected");
   std::string id(reinterpret_cast<const char*>(record),end-record);auto root=std::find_if(source.graph.begin(),source.graph.end(),[&](const scene::Node& n){return n.id==id&&n.parent==-1;});
   if(root==source.graph.end())throw std::runtime_error("Missing room template: "+id);
   const unsigned root_index=root-source.graph.begin();
   std::vector<int> mapping(source.graph.size(),-1);const unsigned first=level.scene.graph.size();
   for(unsigned i=root_index;i<source.graph.size();++i){const auto& n=source.graph[i];if(i!=root_index&&(n.parent<0||mapping[n.parent]<0))continue;
    auto copied=n;copied.id="room"+std::to_string(room)+"/"+copied.id;copied.parent=i==root_index?-1:mapping[n.parent];
    if(i==root_index)for(unsigned j=0;j<3;++j)copied.translation[j]=value(record+112+j*4);
    mapping[i]=level.scene.graph.size();level.scene.graph.push_back(std::move(copied));}
   level.scene.nodes=level.scene.graph.size();
   for(const auto& instance:source.instances){if(mapping[instance.node_index]<0)continue;auto copied=instance;copied.node_index=mapping[instance.node_index];copied.node=level.scene.graph[copied.node_index].id;level.scene.instances.push_back(std::move(copied));}
   if(!scene::update_world(level.scene,error))throw std::runtime_error(error);
   unsigned floors=0;
   for(auto i=level.scene.instances.begin();i!=level.scene.instances.end();){
    if(i->node_index>=first&&i->node.find("/_floor_")!=std::string::npos){triangles(view,*i,room,level);++floors;i=level.scene.instances.erase(i);}
    else if(i->node_index==first||(i->node_index>=first&&i->node.find("/_exit_")!=std::string::npos))i=level.scene.instances.erase(i);
    else ++i;
   }
   if(!floors)throw std::runtime_error("Room has no navigation root: "+id);
  }
  if(level.floor.empty()||level.scene.instances.empty())throw std::runtime_error("World has no floor or visual geometry");
  if(!floors::build_graph(*level.native_floor,error))throw std::runtime_error(error);
  if(!floors::post_load(*level.native_floor,error))throw std::runtime_error(error);
  float ground=0;const bool found=height(level,level.spawn,ground);
  if(!found||std::abs(ground-level.spawn[2])>100){Point low{INFINITY,INFINITY,INFINITY},high{-INFINITY,-INFINITY,-INFINITY};
   for(const auto& t:level.floor)if(t.room==0)for(const auto& p:{t.a,t.b,t.c})for(unsigned i=0;i<3;++i){low[i]=std::min(low[i],p[i]);high[i]=std::max(high[i],p[i]);}
   throw std::runtime_error("Spawn floor mismatch: found="+std::to_string(found)+" height="+std::to_string(ground)+" first room bounds="+std::to_string(low[0])+","+std::to_string(low[1])+","+std::to_string(low[2])+".."+std::to_string(high[0])+","+std::to_string(high[1])+","+std::to_string(high[2]));}
  level.spawn[2]=ground;
  out=std::move(level);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
bool height(const Level& level,const Point& point,float& result){
 if(level.native_floor)return floors::height(*level.native_floor,point.data(),result);
 for(float x:point)if(!std::isfinite(x))return false;
 double closest=1e30;bool found=false;
 for(const auto& t:level.floor){
  const double bx=double(t.b[0])-t.a[0],by=double(t.b[1])-t.a[1],cx=double(t.c[0])-t.a[0],cy=double(t.c[1])-t.a[1];
  const double dx=double(point[0])-t.a[0],dy=double(point[1])-t.a[1],denom=bx*cy-by*cx;
  if(std::abs(denom)<1e-4)continue;
  const double u=(dx*cy-dy*cx)/denom,v=(bx*dy-by*dx)/denom;
  if(u< -1e-6||v< -1e-6||u+v>1.000001)continue;
  const double h=t.a[2]+u*(double(t.b[2])-t.a[2])+v*(double(t.c[2])-t.a[2]);const double delta=std::abs(h-point[2]);
  if(delta<closest){closest=delta;result=h;found=true;}
 }return found;
}
bool supported(const Level& level,const Point& point,float radius,float& ground){
 if(!std::isfinite(radius)||radius<0||radius>1000||!height(level,point,ground))return false;
 if(std::abs(ground-point[2])>72)return false;
 constexpr float directions[8][2]{{1,0},{0,1},{-1,0},{0,-1},{.70710678f,.70710678f},{-.70710678f,.70710678f},{.70710678f,-.70710678f},{-.70710678f,-.70710678f}};
 for(const auto& d:directions){Point edge{point[0]+radius*d[0],point[1]+radius*d[1],ground};float h;
  if(!height(level,edge,h)||std::abs(h-ground)>72)return false;}return true;
}
bool move(const Level& level,Point& point,float dx,float dy,float radius){
 if(!std::isfinite(dx)||!std::isfinite(dy)||std::abs(dx)>10000||std::abs(dy)>10000)return false;
 const unsigned steps=std::max(1u,unsigned(std::ceil(std::max(std::abs(dx),std::abs(dy))/8)));dx/=steps;dy/=steps;bool changed=false;
 for(unsigned i=0;i<steps;++i){Point next{point[0]+dx,point[1]+dy,point[2]};float h;
  if(!supported(level,next,radius,h)){next={point[0]+dx,point[1],point[2]};if(!supported(level,next,radius,h)){next={point[0],point[1]+dy,point[2]};if(!supported(level,next,radius,h))break;}}
  next[2]=h;changed=changed||next!=point;point=next;
 }return changed;
}
}
