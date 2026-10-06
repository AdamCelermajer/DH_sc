#include "retained_gameobject_visual_v1.hpp"
#include "../engine-math/math.hpp"
#include <algorithm>
#include <cstring>
namespace dh2::world {
namespace {
std::uint32_t word(const std::uint8_t* p){return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
std::int32_t signed_word(const std::uint8_t* p){auto u=word(p);std::int32_t result;std::memcpy(&result,&u,4);return result;}
void nothing(const animation::TriggeredEvent*,void*){}
}
RetainedGameObjectVisualV1::RetainedGameObjectVisualV1(CanonicalGameObjectBaseOwnerV1& b,RetainedGameObjectVisualServicesV1 s):base_(b),services_(std::move(s)){
 // CTimelineController666e40: loop1, scale1, remaining clock flags zero.
 timeline_.loop=1;timeline_.scale=1.f;binding_.root.flags=0x60fu; // ISceneNodeC2 599220
}
bool RetainedGameObjectVisualV1::missing(const char* endpoint,std::string& e)const{e=std::string("Required actual retained VisualObject ")+endpoint;return false;}
bool RetainedGameObjectVisualV1::clips(std::string& e){
 auto n=dh2_bres_library_count(&bres_,resources::Library::animation_clip);
 for(std::uint32_t i=0;i<n;++i){auto* r=dh2_bres_library_item(&bres_,resources::Library::animation_clip,static_cast<std::int32_t>(i));
  if(!r)return missing("valid clip library record",e);auto offset=word(r);
  if(offset>=bres_.size)return missing("valid authored clip name",e);
  auto* begin=reinterpret_cast<const char*>(bres_.bytes+offset);
  auto* end=static_cast<const char*>(std::memchr(begin,0,bres_.size-offset));
  if(!end)return missing("terminated authored clip name",e);
  Clip c{std::string(begin,end),signed_word(r+4),signed_word(r+8)};
  if(c.end<c.start)return missing("ordered authored clip bounds",e);
  clips_.push_back(std::move(c));
 }
 timeline_.library_present=n?1u:0u;
 // Actual CSceneNodeAnimator65dbd8 selects library clip0 where present.
 if(n){auto& c=clips_.front();if(dh2_timeline_clip(&timeline_,0,c.start,c.end))return missing("clip0 constructor",e);}
 else if(animated_){if(dh2_timeline_range(&timeline_,animation_.start,animation_.end,1))return missing("source animation range",e);}
 return true;
}
bool RetainedGameObjectVisualV1::load_skinned_meshes(std::string& e){
 for(std::size_t i=0;i<scene_.instances.size();++i){auto& instance=scene_.instances[i];if(instance.controller<0)continue;
  SkinnedMesh m;m.instance=static_cast<std::uint32_t>(i);
  if(!skinning::load(bres_,static_cast<unsigned>(instance.controller),scene_,m.skin,e))return false;
  if(m.skin.geometry!=instance.geometry)return missing("SAME authored skin geometry",e);
  assets::Mesh geometry{};if(dh2_mesh_open(&geometry,&bres_,static_cast<std::int32_t>(instance.geometry))!=assets::Error::ok)return missing("actual skinned mesh geometry",e);
  assets::Attribute position{};std::int32_t position_index=-1;
  for(std::uint32_t p=0;p<geometry.primitives;++p){assets::Primitive primitive{};
   if(dh2_mesh_primitive(&geometry,static_cast<std::int32_t>(p),&primitive)!=assets::Error::ok)return missing("actual skinned primitive layout",e);
   auto index=primitive.attributes[0];if(index<0)return missing("authored position semantic",e);
   if(position_index>=0&&index!=position_index)return missing("shared skinned position stream",e);position_index=index;
  }
  if(position_index<0||dh2_mesh_attribute(&geometry,position_index,&position)!=assets::Error::ok||position.components!=3||position.vertices!=m.skin.influences.size())return missing("skinned position/influence domain",e);
  m.source_positions.resize(position.vertices);
  for(std::uint32_t v=0;v<position.vertices;++v)if(!dh2_attribute_read(&position,v,m.source_positions[v].data()))return missing("source skinned position data",e);
  skinned_.push_back(std::move(m));
 }
 return update_skinned_meshes(e);
}
bool RetainedGameObjectVisualV1::update_skinned_meshes(std::string& e){
 for(auto& m:skinned_)if(!skinning::palette(m.skin,scene_,m.palette,e)||!skinning::positions(m.skin,m.palette,m.source_positions,m.positions,e))return false;
 return true;
}
bool RetainedGameObjectVisualV1::initialize(const char* model,const char* xref,std::string& e){
 if(ready_||root_present_||!bytes_.empty())return missing("fresh constructor candidate",e);
 if(!model||!xref)return missing("source model/subscene strings",e);
 if(!services_.owner||!services_.read_asset)return missing("asset resource owner",e);
 // Selected cache chests pass empty subscene. Do not silently load full scene
 // when the source requested an authored subtree that has not been ported.
 if(*xref)return missing("LoadNode authored subscene selection",e);
 bool found=false;if(!services_.read_asset(model,bytes_,found,e))return false;
 if(!found){bytes_.clear();ready_=true;return true;} // original root==NULL branch
 if(dh2_bres_open(&bres_,bytes_.data(),bytes_.size())!=resources::BresError::ok)return missing("valid source BRES",e);
 if(!scene::load(bres_,scene_,e)||!binding_.bind(scene_,e))return false;
 node_flags_.assign(scene_.graph.size(),0x60fu);
 node_visibility_.assign(scene_.graph.size(),Visibility{});
 for(std::size_t i=0;i<scene_.graph.size();++i)node_handles_.push_back(std::make_unique<NodeHandle>(NodeHandle{static_cast<std::uint32_t>(i)}));
 root_present_=true;
 if(!sync(e))return false;
 // SetParent47295c: dynamic receiver (or animated static receiver) recursively
 // sets a source node bool. Optimized static transform baking is separate.
 bool dynamic=base_.lifecycle().static84==0;
 if(!dynamic){if(!services_.parent_is_animated)return missing("parent IsAnimated virtual",e);if(!services_.parent_is_animated(dynamic,e))return false;}
 if(!dynamic)return missing("OptimizeStatic scene producer",e);
 // Exact recursive setOnAnimateEnabled50e46c: OR scene bit200 on root and
 // every descendant; these are the retained node fields, not display flags.
 binding_.root.flags|=0x200u;for(auto& flags:node_flags_)flags|=0x200u;
 // Constructor explicitly registers root and forces SceneManager registration.
 if(!services_.register_root||!services_.force_register)return missing("SceneManager registration services",e);
 if(!services_.register_root(root_identity(),e)||!services_.force_register(root_identity(),e))return false;
 if(!load_skinned_meshes(e))return false;
 if(!physical::decor_scene_marker(bres_,scene_,marker_,e))return false;
 if(!marker_.found)return missing("CalcMeshBox no-colbox node-type traversal",e);
 if(!calc_mesh_box(e)||!apply_mesh_box(e))return false;
 animated_=dh2_bres_library_count(&bres_,resources::Library::animation)>0;
 if(animated_&&!animation_.load(bytes_.data(),bytes_.size(),scene_,e))return false;
 root_animator_present_=animated_;
 if(!clips(e))return false;
 // AnimController(root,false) installs literal DoNothing endpoints; its
 // SetCallbacks virtual is bx lr. No gameplay event callback is substituted.
 base_named_animation_set_callbacks_v1();ready_=true;return true;
}
bool RetainedGameObjectVisualV1::sync(std::string& e){
 if(!root_present_)return true;
 auto* p=base_.vector3(0x160);auto* r=base_.vector3(0x16c);auto* scale=base_.vector3(0x120);
 if(!p||!r||!scale)return missing("SAME parent pose/scale fields",e);
 std::copy_n(p,3,binding_.root.position);std::copy_n(scale,3,binding_.root.scale);
 if(!binding_.set_rotation(r)||!binding_.update_world(scene_,e))return false;
 return update_skinned_meshes(e);
}
bool RetainedGameObjectVisualV1::calc_mesh_box(std::string& e){
 if(!root_present_)return true;if(!marker_.found)return missing("CalcMeshBox no-colbox node traversal",e);
 physical::DecorMeshBoxInput input{};std::copy_n(marker_.bounds,6,input.bounds);
 std::copy_n(marker_.parent_scale,3,input.parent_scale);
 dh2_node_matrix(input.node_matrix,binding_.root.position,binding_.root.quaternion,binding_.root.scale);
 if(dh2_decor_marker_mesh_box(mesh_box_.data(),&input))return missing("actual mesh-box kernel",e);
 return true;
}
bool RetainedGameObjectVisualV1::apply_mesh_box(std::string& e){
 if(!root_present_)return true;auto* p=base_.vector3(0x160);auto* flat=base_.byte(0x15c);
 if(!p||!flat)return missing("SAME parent bounds/flat fields",e);
 physical::DecorBodyInput input{};input.owner=reinterpret_cast<void*>(base_.identity());input.visual_present=1;
 input.previous_flat=*flat;std::copy_n(mesh_box_.data(),6,input.mesh_box);std::copy_n(p,3,input.position);
 physical::DecorBodyConfig out{};if(dh2_decor_body_config(&out,&input))return missing("source mesh-box application",e);
 *flat=static_cast<std::uint8_t>(out.flat);std::copy_n(out.relative_box,6,base_.relative_aabb144());
 std::copy_n(out.absolute_box,6,base_.absolute_aabb12c());
 if(!services_.update_pf)return missing("SAME parent UpdatePFObject",e);
 return services_.update_pf(e);
}
bool RetainedGameObjectVisualV1::sample(bool reset,std::string& e){
 if(!animated_)return true;
 if(reset)return missing("displacement-enabled reset on generic scene",e);
 if(!binding_.sample(scene_,animation_,timeline_.current_ms,e))return false;
 return update_skinned_meshes(e);
}
BaseNamedAnimationBorrowV1 RetainedGameObjectVisualV1::named_animation(std::shared_ptr<void> lease){
 BaseNamedAnimationBorrowV1 b;b.owner=std::move(lease);if(!root_present_||!animated_)return b;
 b.timeline=&timeline_;b.scene_flags11c=&binding_.root.flags;b.timeline_library_count=static_cast<std::int32_t>(clips_.size());
 b.applicator_extra_ms=&completion_.extra_ms;
 auto find=[this](const char* name,std::int32_t& v,std::string& e){if(!name)return missing("clip name",e);v=-1;for(std::size_t i=0;i<clips_.size();++i)if(clips_[i].name==name){v=static_cast<std::int32_t>(i);break;}return true;};
 b.timeline_find_name=find;b.animator_find_name=find;
 b.timeline_get_loop=[this](bool& v,std::string&){v=timeline_.loop!=0;return true;}; // source virtual44 getLoop
 b.animator_current=[this](std::int32_t& v,std::string&){v=timeline_.clip_index;return true;};
 b.animator_select=[this](std::int32_t v,std::string& e){if(v<0||std::size_t(v)>=clips_.size())return missing("resolved animator clip",e);auto& c=clips_[v];return dh2_timeline_clip(&timeline_,v,c.start,c.end)==0;};
 b.root_new_anim=[this](bool displacement,std::string& e){if(displacement)return missing("source displacement-enabled NewAnim",e);displacement_enabled_=false;return true;};
 return b;
}
bool RetainedGameObjectVisualV1::play(const char* name,bool loop,bool& accepted,std::string& e){
 accepted=false;if(!ready_)return missing("completed constructor",e);if(!root_present_||!animated_)return true;
 auto b=named_animation(services_.owner);return base_named_animation_play_v1(b,name,loop,accepted,e);
}
bool RetainedGameObjectVisualV1::update(std::uint32_t ms,std::string& e){
 if(!ready_)return missing("completed constructor",e);if(!root_present_||!animated_||!root_animator_present_)return true;
 timestamp_=ms;auto previous=timeline_.current_ms;std::int32_t signed_ms;std::memcpy(&signed_ms,&ms,4);
 timeline::Services callbacks{&completion_,[](void* p,timeline::State* t){if(dh2_timeline_notify(static_cast<timeline::Completion*>(p),t))throw std::runtime_error("source animation completion rejected");}};
 if(dh2_timeline_update(&timeline_,signed_ms,&callbacks))return missing("source timeline update",e);
 if(!sample(false,e))return false;
 if(completion_.pending)completion_.pending=0; // CheckCallback invokes installed DoNothing then clears
 // Original constructor callback table routes authored triggers to DoNothing.
 return dh2_events_update_interval(&animation_.events.view(),previous,timeline_.current_ms,nothing,this);
}
bool RetainedGameObjectVisualV1::release(std::string& e){
 // VisualObjectD1 473884 releases its controller before the retained root,
 // then invokes SceneManager::ForceRegister. Preserve required delivery.
 if(!services_.force_register)return missing("source destructor ForceRegister",e);
 auto identity=root_identity();if(root_present_){
  if(!services_.release_root)return missing("source root release",e);
  if(!services_.release_root(identity,e))return false;root_present_=false;
 }if(!services_.force_register(identity,e))return false;
 ready_=false;animated_=false;animation_=animation::Player{};scene_=scene::Scene{};
 bytes_.clear();bres_={};clips_.clear();node_flags_.clear();node_visibility_.clear();node_handles_.clear();skinned_.clear();root_animator_present_=false;return true;
}
bool RetainedGameObjectVisualV1::set_root_game_object(std::uintptr_t identity,std::string& e){
 if(!root_present_||identity!=base_.identity())return missing("SAME root+204 parent identity",e);
 root_game_object204_=identity;return true;
}
bool RetainedGameObjectVisualV1::node_from_name(const char* name,std::uintptr_t& result,std::string& e)const{
 result=0;if(!root_present_||!name)return missing("retained root/name lookup",e);
 // Source search order is depth-first child order. Scene::load emits that order.
 for(std::size_t i=0;i<scene_.graph.size();++i)if(scene_.graph[i].name==name){result=reinterpret_cast<std::uintptr_t>(node_handles_[i].get());break;}return true;
}
bool RetainedGameObjectVisualV1::node_position(std::uintptr_t handle,float* out,std::string& e)const{
 if(!out)return missing("node position output",e);
 for(auto& node:node_handles_)if(reinterpret_cast<std::uintptr_t>(node.get())==handle){auto& matrix=scene_.graph[node->index].world;for(unsigned i=0;i<3;++i)out[i]=matrix[12+i];return true;}
 return missing("SAME retained scene node identity",e);
}
void RetainedGameObjectVisualV1::propagate_node_visibility(std::size_t parent){
 for(std::size_t i=0;i<scene_.graph.size();++i)if(scene_.graph[i].parent==static_cast<std::int32_t>(parent))notify_node_visibility(i,(node_flags_[parent]&1u)!=0);
}
void RetainedGameObjectVisualV1::notify_node_visibility(std::size_t i,bool parent){
 auto& v=node_visibility_[i];auto previous=node_flags_[i]&1u;v.parent121=parent?1:0;
 if(v.local120&&parent)node_flags_[i]|=1u;else node_flags_[i]&=~1u;
 if(previous!=(node_flags_[i]&1u))propagate_node_visibility(i);
}
bool RetainedGameObjectVisualV1::notify_root_visibility(bool parent,std::string& e){
 if(!root_present_||node_visibility_.size()!=scene_.graph.size())return missing("SAME root visibility graph",e);
 auto previous=binding_.root.flags&1u;root_visibility_.parent121=parent?1:0;
 if(root_visibility_.local120&&parent)binding_.root.flags|=1u;else binding_.root.flags&=~1u;
 if(previous!=(binding_.root.flags&1u))for(std::size_t i=0;i<scene_.graph.size();++i)if(scene_.graph[i].parent<0)notify_node_visibility(i,(binding_.root.flags&1u)!=0);
 return true;
}
bool RetainedGameObjectVisualV1::set_node_local_visibility(std::uintptr_t handle,bool local,std::string& e){
 for(auto& h:node_handles_)if(reinterpret_cast<std::uintptr_t>(h.get())==handle){auto i=h->index;auto& v=node_visibility_[i];if(v.local120==std::uint8_t(local))return true;
  auto previous=node_flags_[i]&1u;v.local120=local?1:0;if(local&&v.parent121)node_flags_[i]|=1u;else node_flags_[i]&=~1u;
  if(previous!=(node_flags_[i]&1u))propagate_node_visibility(i);return true;
 }
 return missing("SAME node local visibility receiver",e);
}
bool RetainedGameObjectVisualV1::remove_root_animators(std::string& e){
 if(!root_present_)return missing("SAME root removeAnimators receiver",e);
 // Source removeAnimators598658 detaches and drops each owned animator;
 // CSceneNodeAnimatorD1 removes tracks, drops its AnimationBlock and database.
 // Player is this native successor's sole owned track/block/database graph.
 // Move assignment destroys that actual graph while leaving sampled pose.
 animation_=animation::Player{};root_animator_present_=false;animated_=false;
 clips_.clear();timeline_={};completion_={};return true;
}
}
