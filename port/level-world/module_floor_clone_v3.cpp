#include "module_floor_clone_v3.hpp"
#include "../engine-math/math.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
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
 if(native_d1_attempted_v1_)throw std::logic_error("Cannot republish native-destroyed floor clone");
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
 b.scene_phase_v69=[self](std::uint32_t,std::string& e){
  auto map=self->map_.lock();if(!map||self->parent_!=map->identity()){e="Required SAME actual floor-clone parent phase";return false;}
  const auto flags=self->flags_;
  if(((flags&0x400u)&&!(flags&1u))||!(flags&0x200u)){e.clear();return true;}
  // CopyMesh's static/empty-animator domain updates only this cached node.
  // There is no authored pose/clip to synthesize or copied PF Record to mutate.
  if((flags&0x5eu)||(map->flags()&0x20u)){
   std::array<float,16> relative;
   dh2_node_matrix(relative.data(),self->position_.data(),self->rotation_.data(),self->scale_.data());
   self->cached_=scene::multiply(map->cached_matrix(),relative);
   self->flags_=(self->flags_|0x120u)&~0x5eu;self->flags_|=0x10u;
  }
  e.clear();return true;
 };
 return b;
}
const floors::Record& ModuleFloorCloneV3::floor()const{
 if((native_d1_attempted_v1_&&!native_d1_running_v1_)||!record_||!world_)throw std::logic_error("Native floor clone Record is retired");
 return *record_;
}
bool ModuleFloorCloneV3::drop_floor_reference_v1(const FloorCloneDestructionV1& services,std::string& e){
 if(native_d1_complete_v1_){e.clear();return true;}
 if(native_d1_attempted_v1_){e=native_d1_failure_v1_.empty()?"Floor clone D1 cannot replay/reenter a reached prefix":native_d1_failure_v1_;return false;}
 auto map=map_.lock();
 // PFFloorD1 only drops mesh40; it does NOT detach it. Source LevelD1's
 // Scene virtual68 must have dropped parent references/render aliases first.
 if(parent_){e="Require genuine Scene/map parent removal before last PF mesh40 drop";return false;}
 if(map){const auto children=map->children();if(std::find(children.begin(),children.end(),identity())!=children.end()){e="Floor clone still published in SAME Scene map";return false;}}
 if(!record_||!world_||!resource_||!services.owner||!services.require_unpublished||!services.native_mesh_d1){e="Required live SAME floor/bytes and actual copied-mesh D1 services";return false;}
 if(!services.require_unpublished(*this,e))return false;
 native_d1_attempted_v1_=true;
 auto fail=[&]{native_d1_failure_v1_=e.empty()?"Required actual copied IMeshSceneNode D1":e;e=native_d1_failure_v1_;return false;};
 native_d1_running_v1_=true;struct Guard{bool& running;~Guard(){running=false;}}guard{native_d1_running_v1_};
 try{if(!services.native_mesh_d1(*this,e))return fail();}
 catch(const std::exception& ex){e=ex.what();return fail();}catch(...){e="Actual copied-mesh D1 provider threw";return fail();}
 if(parent_){e="Native clone D1 reparented the retired receiver";return fail();}
 // Logical native drop/D1 completed. Diagnostic shared aliases may still pin
 // this port allocation, but cannot re-enter its engine/draw/floor methods.
 record_=nullptr;resource_.reset();world_.reset();map_.reset();native_d1_complete_v1_=true;e.clear();return true;
}
bool ModuleFloorCloneV3::native_mesh_d1_v106(std::string& e){
 if(!native_d1_running_v1_||parent_||!record_||!world_||!resource_){e="Copied native mesh D1 requires the actual last floor-reference scope";return false;}
 // Native clone owns no animator/child/selector allocation. Its independent
 // BRES mesh-resource reference is its actual positive derived D1 domain.
 resource_.reset();e.clear();return true;
}
}

