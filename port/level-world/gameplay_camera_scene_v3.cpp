#include "gameplay_camera_scene_v3.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <stdexcept>
namespace dh2::camera {
namespace {
struct Reader {
 const resources::BresView& b;scene::Scene& graph;std::vector<AuthoredCameraV3>& cameras;std::vector<std::uint32_t> path;
 const std::uint8_t* span(std::uint64_t p,std::uint64_t n)const{if(p>b.size||n>b.size-p)throw std::runtime_error("Camera field outside BRES");return b.bytes+p;}
 std::uint32_t w(std::uint64_t p)const{const auto* x=span(p,4);return x[0]|std::uint32_t(x[1])<<8|std::uint32_t(x[2])<<16|std::uint32_t(x[3])<<24;}
 float f(std::uint64_t p)const{auto raw=w(p);float value;std::memcpy(&value,&raw,4);if(!std::isfinite(value))throw std::runtime_error("Nonfinite camera field");return value;}
 std::string text(std::uint32_t p)const{if(!p)return {};const auto* start=span(p,1);const auto* end=static_cast<const std::uint8_t*>(std::memchr(start,0,std::min<std::size_t>(4096,b.size-p)));if(!end)throw std::runtime_error("Unterminated camera name");return {reinterpret_cast<const char*>(start),reinterpret_cast<const char*>(end)};}
 std::string field(std::uint64_t p)const{return text(w(p));}
 void array(std::uint32_t p,std::uint32_t n,unsigned stride)const{if(n>10000)throw std::runtime_error("Excessive camera graph array");span(p,std::uint64_t(n)*stride);}
 void node(std::uint32_t p,std::int32_t parent){
  span(p,80);if(path.size()>=64||graph.graph.size()>=10000||std::find(path.begin(),path.end(),p)!=path.end())throw std::runtime_error("Cyclic/excessive camera graph");path.push_back(p);
  scene::Node n;n.id=field(p);n.name=field(p+4);n.sid=field(p+8);n.parent=parent;
  for(unsigned i=0;i<3;++i){n.translation[i]=f(p+12+i*4);n.scale[i]=f(p+40+i*4);}for(unsigned i=0;i<4;++i)n.quaternion[i]=f(p+24+i*4);
  if(w(p+72))n.user_properties=field(w(p+72));const auto index=std::uint32_t(graph.graph.size());graph.graph.push_back(std::move(n));++graph.nodes;
  const auto count=w(p+64),instances=w(p+68);array(instances,count,8);
  for(unsigned i=0;i<count;++i){const auto q=instances+i*8;if(w(q)!=1)throw std::runtime_error("Required source non-camera instance constructor");const auto instance=w(q+4);span(instance,8);if(w(instance))throw std::runtime_error("Required external camera database");auto uri=field(instance+4);if(uri.empty()||uri.front()!='#')throw std::runtime_error("Invalid camera URI");uri.erase(0,1);bool found=false;
   for(unsigned j=0;j<dh2_bres_library_count(&b,resources::Library::camera);++j){const auto* item=dh2_bres_library_item(&b,resources::Library::camera,j);if(!item)throw std::runtime_error("Invalid camera library");const auto c=std::uint32_t(item-b.bytes);if(field(c)!=uri)continue;AuthoredCameraV3 camera;camera.id=uri;camera.node=index;camera.kind=w(c+4);camera.horizontal_fov_or_mag=f(c+8);camera.aspect=f(c+12);camera.znear=f(c+16);camera.zfar=f(c+20);camera.target_uri=field(c+24);cameras.push_back(std::move(camera));found=true;break;}
   if(!found)throw std::runtime_error("Unresolved authored camera URI");
  }
  const auto children=w(p+56),base=w(p+60);array(base,children,80);for(unsigned i=0;i<children;++i)node(base+i*80,index);path.pop_back();
 }
 void run(){const auto r=b.root_offset;span(r,192);const auto scenes=w(r+152),scene_base=w(r+156),count=w(r+184),instances=w(r+188);array(scene_base,scenes,16);array(instances,count,8);
  for(unsigned i=0;i<count;++i){const auto q=instances+i*8;if(w(q)!=6)throw std::runtime_error("Required source alternate camera scene root");const auto a=w(q+4);span(a,8);if(w(a))throw std::runtime_error("Required external visual scene");auto uri=field(a+4);if(uri.empty()||uri.front()!='#')throw std::runtime_error("Invalid camera visual scene URI");uri.erase(0,1);bool found=false;for(unsigned j=0;j<scenes;++j){const auto vs=scene_base+j*16;if(field(vs)!=uri)continue;const auto n=w(vs+8),base=w(vs+12);array(base,n,80);for(unsigned k=0;k<n;++k)node(base+k*80,-1);found=true;break;}if(!found)throw std::runtime_error("Unresolved camera visual scene");}
  for(auto& c:cameras){if(c.target_uri.empty()||c.target_uri.front()!='#')throw std::runtime_error("Required authored camera target URI");const auto target=c.target_uri.substr(1);const auto n=std::find_if(graph.graph.begin(),graph.graph.end(),[&](const auto& node){return node.id==target;});if(n==graph.graph.end())throw std::runtime_error("Unresolved authored camera target node");c.target_node=std::uint32_t(n-graph.graph.begin());}
 }
};
}
bool GameplayCameraSceneV3::load(std::vector<std::uint8_t> bytes,std::string& e){
 if(!bytes_.empty()){e="Camera scene already retained; source resource replacement requires release";return false;}bytes_=std::move(bytes);if(dh2_bres_open(&bres_,bytes_.data(),bytes_.size())!=resources::BresError::ok){e="Invalid actual camera BRES";return false;}
 try{Reader{bres_,scene_,cameras_,{}}.run();return scene::update_world(scene_,e);}catch(const std::exception& failure){e=failure.what();return false;}
}
bool GameplayCameraSceneV3::select(const std::string& name,std::uint32_t& selected,std::string& e)const{
 const auto start=std::find_if(scene_.graph.begin(),scene_.graph.end(),[&](const auto& n){return n.name==name;});if(start==scene_.graph.end()){e="Original selected camera node name absent";return false;}const auto index=std::int32_t(start-scene_.graph.begin());
 for(unsigned i=0;i<cameras_.size();++i){auto node=std::int32_t(cameras_[i].node);while(node>=0){if(node==index){selected=i;return true;}node=scene_.graph[node].parent;}}
 e="Selected authored node has no camera scene instance";return false;
}
bool GameplayCameraSceneV3::root_position(float p[3],std::string& e)const{if(!p||scene_.graph.empty()){e="Required actual camera root";return false;}std::copy_n(root_position_.data(),3,p);return true;}
bool GameplayCameraSceneV3::set_root_position(const float p[3],std::string& e){if(!p||scene_.graph.empty()){e="Required actual camera root SetPosition";return false;}std::copy_n(p,3,root_position_.data());if(!scene::update_world(scene_,e))return false;for(auto& n:scene_.graph)for(unsigned axis=0;axis<3;++axis)n.world[12+axis]+=root_position_[axis];return true;}
bool GameplayCameraSceneV3::set_camera_instance_position(std::uint32_t index,const float p[3],std::string& e){if(index>=cameras_.size()||!p){e="Required same selected camera instance";return false;}std::copy_n(p,3,cameras_[index].instance_position.data());return true;}
bool GameplayCameraSceneV3::eye_and_target(std::uint32_t index,float eye[3],float target[3],std::string& e)const{if(index>=cameras_.size()||!eye||!target){e="Required same selected camera eye/target";return false;}const auto& c=cameras_[index];if(c.node>=scene_.graph.size()||c.target_node>=scene_.graph.size()){e="Released camera node binding";return false;}const auto& parent=scene_.graph[c.node].world;const auto& destination=scene_.graph[c.target_node].world;for(unsigned row=0;row<3;++row){float value=parent[12+row];for(unsigned column=0;column<3;++column)value+=parent[column*4+row]*c.instance_position[column];eye[row]=value;target[row]=destination[12+row];}return true;}
bool GameplayCameraSceneV3::update_selected_absolute_v67(std::uint32_t index,std::string& e){
 if(index>=cameras_.size()||cameras_[index].node>=scene_.graph.size()){e="Required actual selected camera node before updateAbsolutePosition(false)";return false;}
 // Native source scene keeps serialized nodes in the one owned graph. Update
 // their absolute matrix cache from current local TRS, then apply the distinct
 // actual CRootSceneNode wrapper translation once. No animator/time advances.
 if(!scene::update_world(scene_,e))return false;
 for(auto& node:scene_.graph)for(unsigned axis=0;axis<3;++axis)node.world[12+axis]+=root_position_[axis];
 e.clear();return true;
}
bool GameplayCameraSceneV3::camera_parent_position_v67(std::uint32_t index,float parent[3],std::string& e)const{
 if(!parent||index>=cameras_.size()||cameras_[index].node>=scene_.graph.size()){e="Required SAME camera instance parent absolute cache";return false;}
 const auto& matrix=scene_.graph[cameras_[index].node].world;
 for(unsigned axis=0;axis<3;++axis)parent[axis]=matrix[12+axis];e.clear();return true;
}
}
