#include "module_floor_clone_v3.hpp"
#include "../engine-math/math.hpp"
#include <algorithm>
#include <cmath>
namespace dh2::world {
ModuleFloorCloneV3::ModuleFloorCloneV3(std::shared_ptr<void> resource,std::shared_ptr<void> world,
 floors::Record& record,const float* q,const float* scale,std::shared_ptr<SceneManagerMapOwnerV2> map):
 resource_(std::move(resource)),world_(std::move(world)),record_(&record),map_(map){
 std::copy_n(record.clone.values,16,cached_.begin());std::copy_n(cached_.data()+12,3,position_.begin());
 std::copy_n(q,4,rotation_.begin());std::copy_n(scale,3,scale_.begin());
}
bool ModuleFloorCloneV3::create(std::shared_ptr<void> resource,std::shared_ptr<void> world,
 floors::Record& record,const float* q,const float* scale,std::shared_ptr<SceneManagerMapOwnerV2> map,
 std::shared_ptr<ModuleFloorCloneV3>& out,std::string& error){
 error.clear();if(!resource||!world||!q||!scale||!map||record.triangles.empty()){
  error="CopyMeshSceneNode requires SAME resource/PF owner, loaded geometry and actual local TRS";return false;}
 for(unsigned i=0;i<4;++i)if(!std::isfinite(q[i])){error="Invalid actual floor clone quaternion";return false;}
 for(unsigned i=0;i<3;++i)if(!std::isfinite(scale[i])){error="Invalid actual floor clone scale";return false;}
 out=std::shared_ptr<ModuleFloorCloneV3>(new ModuleFloorCloneV3(std::move(resource),std::move(world),record,q,scale,std::move(map)));return true;
}
SceneMapNodeBorrowV2 ModuleFloorCloneV3::map_node(){
 auto self=shared_from_this();SceneMapNodeBorrowV2 b;
 b.owner=self;b.identity=identity();b.parentec=&parent_;b.flags11c=&flags_;
 b.cached_position=[self](std::array<float,3>& p,std::string& e){e.clear();std::copy_n(self->cached_.data()+12,3,p.begin());return true;};
 b.set_position=[self](const std::array<float,3>& p,std::string& e){e.clear();self->position_=p;self->flags_|=8u;return true;};
 b.detach=[self](std::string& e){auto map=self->map_.lock();if(!map){e="Required SAME retained floor map during detach";return false;}return map->remove(self->identity(),e);};
 b.optimize_static=[self](std::string& e){
  e.clear();if(self->parent_){e="Required actual non-map parent for floor clone static optimization";return false;}
  if(self->flags_&0x5eu){dh2_node_matrix(self->cached_.data(),self->position_.data(),self->rotation_.data(),self->scale_.data());self->flags_=(self->flags_|0x120u)&~0x5eu;self->flags_|=0x10u;}
  std::copy_n(self->cached_.data()+12,3,self->position_.begin());
  math::Matrix4f m{};std::copy_n(self->cached_.data(),16,m.m);m.identity_hint=0;math::Quaternion q{};dh2_quat_from_matrix(&q,&m);
  self->rotation_={q.x,q.y,q.z,q.w};self->flags_=(self->flags_|0xcu)&~0x200u;return true;
 };
 b.notify_parent_visibility=[self](bool visible,std::string& e){e.clear();self->parent_visible_=visible;
  if(self->local_visible_&&visible)self->flags_|=1u;else self->flags_&=~1u;return true;};
 return b;
}
}
