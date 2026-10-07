#include "retained_gameobject_visual_v1.hpp"
#include "authored_scene_subtree_v2.hpp"
#include "visual_mesh_box_fallback_v2.hpp"
#include "visual_aabb_dispatch_scope_v3.hpp"
#include "native_scene_lights_v113.hpp"
#include "../engine-math/math.hpp"
#include <algorithm>
#include <cstring>
#include <cmath>
#include <stdexcept>
namespace dh2::world {
bool RetainedGameObjectVisualV1::source_light_by_name_v113(const std::string& name,std::shared_ptr<NativeLightV113>& out,std::string& e)const{
 out.reset();if(!ready_||!root_present_)return missing("live Scene light NAME lookup",e);for(const auto& parent:node_handles_)if(parent&&!parent->native_destroyed_v106)for(const auto& node:parent->lights_v113)if(node&&node->live()&&node->source_name_v113()==name){out=node->light();e.clear();return true;}e.clear();return true;
}
bool RetainedGameObjectVisualV1::source_apply_light_set_v113(NativeLightSetV113& lights,std::string& e){
 //471214 NULLRoot skips the original Scene.UpdateLightSet entirely.
 if(!root_present_){e.clear();return true;}
 for(auto& mesh:native_meshes_v93_){if(!mesh||mesh->native_destroyed_v106||!mesh->fields)return missing("actual Scene light/material mesh collection",e);
  assets::Mesh data;if(dh2_mesh_open(&data,&mesh->source_image_v93,mesh->fields->geometry)!=assets::Error::ok)return missing("actual Scene light material buffers",e);
  for(unsigned p=0;p<data.primitives;++p){std::array<std::shared_ptr<NativeLightV113>,4> selected;if(!lights.select(light_set40_,light_filter44_v113_,selected,e))return false;auto found=mesh->material_lights_v113.find(p);auto selection=found==mesh->material_lights_v113.end()?std::make_shared<NativeMaterialLightsV113>():found->second;if(!selection)return missing("actual retained material light fields",e);for(unsigned i=0;i<4;++i)if(selected[i]&&!selection->assign(i,selected[i],e))return false;mesh->material_lights_v113[p]=std::move(selection);}
 }
 e.clear();return true;
}
bool RetainedGameObjectVisualV1::source_apply_material_tail_v113(std::string& e){
 //ApplyMaterial470cd4 is literal BX LR. ApplyMaterialBaseParam474318's
 //source7e/a9 C1 zero skips its positive per-material rim/extra effects.
 if(material_rim7e_v113_||material_special_a9_v113_)return missing("reached actual rim/extra material provider",e);
 //ApplyShadowMaterial4710f0 and ApplyXrayMaterial473ea0 have actual ctor-
 //empty vectors. Their uninitialized selector bytes cannot alter that empty
 //return; no selector byte is fabricated or read by this native transport.
 if(!shadow_nodes8c_v113_.empty())return missing("actual shadow material collection/render-pass transport",e);
 if(!xray_nodes9c_v113_.empty())return missing("actual Xray material collection/render-pass transport",e);
 e.clear();return true;
}
namespace {struct BatchAnimationReferenceV112 {
 std::shared_ptr<RetainedGameObjectVisualV1> visual;
 std::vector<std::shared_ptr<RetainedVisualNodeV91>> nodes;
 ~BatchAnimationReferenceV112(){if(visual)visual->drop_batch_animation_v112();}
};}
bool RetainedGameObjectVisualV1::retain_batch_animation_v112(const std::shared_ptr<RetainedGameObjectVisualV1>& self,BatchAnimationBorrowV112& out,std::string& e){
 out={};if(self.get()!=this||!ready_||!root_present_||batch_animation_references_v112_==UINT32_MAX)return missing("actual pre-D1 animation/mesh ownership transfer",e);
 auto reference=std::make_shared<BatchAnimationReferenceV112>();
 //These are the SAME native node references, protecting bone/mesh cells that
 //the existing animator binds. Source parent removal still executes normally.
 for(const auto& node:node_handles_){std::shared_ptr<RetainedVisualNodeV91> held;if(!node||!node->grab_native_v106(held,node,e))return false;reference->nodes.push_back(std::move(held));}
 ++batch_animation_references_v112_;reference->visual=self;
 out.owner=reference;out.identity=reinterpret_cast<std::uintptr_t>(this);
 const std::weak_ptr<BatchAnimationReferenceV112> weak=reference;
 out.phase=[weak](std::uint32_t stamp,std::string& e){auto held=weak.lock();if(!held||!held->visual){e="Retired compiled animation reference";return false;}return held->visual->batch_animation_phase_v112(stamp,e);};
 e.clear();return true;
}
bool RetainedGameObjectVisualV1::batch_animation_phase_v112(std::uint32_t stamp,std::string& e){
 if(!batch_animation_references_v112_)return missing("actual compiled animation reference",e);
 //An original root still retained for nobatch children owns the ONE scene
 //tick. Only after its real removal does the compiled root take that delivery.
 if(root_present_){e.clear();return true;}
 if(!batch_visual_released_v112_)return missing("completed source VisualD1 before compiled animation",e);
 if(!sync_batch_parent_pose_v113(e))return false;
 const auto flags=binding_.root.flags;
 if(((flags&0x400u)&&!(flags&1u))||!(flags&0x200u)){timestamp_=stamp;e.clear();return true;}
 if(character_scene_phase_v69_){auto lease=character_scene_lifetime_v69_.lock();if(!lease)return missing("SAME retained Character animator",e);return character_scene_phase_v69_(stamp,e);}
 return update_callbacks_v21(stamp,generic_callbacks_v69_,e);
}
bool RetainedGameObjectVisualV1::sync_batch_parent_pose_v113(std::string& e){
 const auto* position=base_?base_->vector3(0x160):fields_.position160;
 const auto* rotation=base_?base_->vector3(0x16c):fields_.rotation16c;
 const auto* scale=base_?base_->vector3(0x120):fields_.scale120;
 if(!batch_animation_references_v112_||(!base_&&!fields_.receiver_lease)||!position||!rotation||!scale)return missing("SAME acquired parent pose160/16c/120",e);
 if(static_optimized_v76_)return missing("moving compiled animator over a source static-baked graph",e);
 std::copy_n(position,3,binding_.root.position);std::copy_n(scale,3,binding_.root.scale);
 if(!binding_.set_rotation(rotation)||!update_root_cache_v76(e))return false;
 //This updates the existing pose graph's parent transform, not another actor
 //or physics step. Its retained animator then samples the current bone pose.
 return binding_.update_world(scene_,e);
}
void RetainedGameObjectVisualV1::drop_batch_animation_v112()noexcept{
 if(!batch_animation_references_v112_)return;
 --batch_animation_references_v112_;
 if(!batch_animation_references_v112_&&batch_visual_released_v112_){
  selected_modular2c_v114_={};own_modular_v114_.reset();modular_resources_v114_.reset();
  character_scene_phase_v69_={};character_scene_lifetime_v69_.reset();generic_callbacks_v69_.set({});
  animation_=animation::Player{};scene_=scene::Scene{};skinned_.clear();mesh_resources_v91_.clear();native_meshes_v93_.clear();node_handles_.clear();bytes_.reset();bres_={};
 }
}
bool RetainedGameObjectVisualV1::borrow_batch_root_matrix_v113(const std::array<float,16>*& out,std::string& e)const{
 out=nullptr;if(!batch_animation_references_v112_||!batch_visual_released_v112_||root_present_)return missing("SAME live compiled animator/root matrix",e);
 out=&root_cached_v76_;e.clear();return true;
}
bool RetainedGameObjectVisualV1::borrow_mesh_source_v111(unsigned index,std::shared_ptr<RetainedMeshNodeV91>& out,std::string& e)const{
 out.reset();
 if(!ready_||!root_present_||index>=native_meshes_v93_.size()||!native_meshes_v93_[index]||native_meshes_v93_[index]->native_destroyed_v106||!native_meshes_v93_[index]->fields){e="Required SAME live Visual mesh source receiver";return false;}
 out=native_meshes_v93_[index];e.clear();return true;
}
namespace {
std::uint32_t word(const std::uint8_t* p){return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
std::int32_t signed_word(const std::uint8_t* p){auto u=word(p);std::int32_t result;std::memcpy(&result,&u,4);return result;}
void nothing(const animation::TriggeredEvent*,void*){}
}
RetainedGameObjectVisualV1::RetainedGameObjectVisualV1(CanonicalGameObjectBaseOwnerV1& b,RetainedGameObjectVisualServicesV1 s):base_(&b),services_(std::move(s)){
 fields_.identity=b.identity();fields_.static84=&b.lifecycle().static84;fields_.position160=b.vector3(0x160);fields_.rotation16c=b.vector3(0x16c);fields_.scale120=b.vector3(0x120);
 // CTimelineController666e40: loop1, scale1, remaining clock flags zero.
 timeline_.loop=1;timeline_.scale=1.f;binding_.root.flags=0x60fu; // ISceneNodeC2 599220
}
RetainedGameObjectVisualV1::RetainedGameObjectVisualV1(GameObjectVisualFieldBorrowV5 fields,RetainedGameObjectVisualServicesV1 services):fields_(std::move(fields)),services_(std::move(services)){
 timeline_.loop=1;timeline_.scale=1.f;binding_.root.flags=0x60fu;
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
  SkinnedMesh m(mesh_resources_v91_.at(i));m.instance=static_cast<std::uint32_t>(i);
  if(!skinning::load(bres_,static_cast<unsigned>(instance.controller),scene_,m.skin,e))return false;
  if(m.skin.geometry!=instance.geometry)return missing("SAME authored skin geometry",e);
  assets::Mesh geometry{};if(dh2_mesh_open(&geometry,&bres_,static_cast<std::int32_t>(instance.geometry))!=assets::Error::ok)return missing("actual skinned mesh geometry",e);
  assets::Attribute position{},normal{};std::int32_t position_index=-1,normal_index=-1;
  for(std::uint32_t p=0;p<geometry.primitives;++p){assets::Primitive primitive{};
   if(dh2_mesh_primitive(&geometry,static_cast<std::int32_t>(p),&primitive)!=assets::Error::ok)return missing("actual skinned primitive layout",e);
   auto index=primitive.attributes[0];if(index<0)return missing("authored position semantic",e);
   if(position_index>=0&&index!=position_index)return missing("shared skinned position stream",e);position_index=index;
   const auto normals=primitive.attributes[1];if(normals>=0){if(normal_index>=0&&normal_index!=normals)return missing("shared software-skin normal stream",e);normal_index=normals;}
  }
  if(position_index<0||dh2_mesh_attribute(&geometry,position_index,&position)!=assets::Error::ok||position.components!=3||position.vertices!=m.skin.influences.size())return missing("skinned position/influence domain",e);
  m.source_positions.resize(position.vertices);
  for(std::uint32_t v=0;v<position.vertices;++v)if(!dh2_attribute_read(&position,v,m.source_positions[v].data()))return missing("source skinned position data",e);
  if(normal_index>=0){if(dh2_mesh_attribute(&geometry,normal_index,&normal)!=assets::Error::ok||normal.components!=3||normal.vertices!=m.skin.influences.size())return missing("software-skin normal/influence domain",e);
   auto& values=m.mesh_owner->source_normals_v113;values.resize(normal.vertices);for(std::uint32_t v=0;v<normal.vertices;++v)if(!dh2_attribute_read(&normal,v,values[v].data()))return missing("actual source software-skin normal data",e);
  }
  skinned_.push_back(std::move(m));
 }
 return update_skinned_meshes(e);
}
bool RetainedGameObjectVisualV1::update_skinned_meshes(std::string& e){
 for(auto& m:skinned_){if(!skinning::palette(m.skin,scene_,m.palette,e)||!skinning::positions(m.skin,m.palette,m.source_positions,m.positions,e))return false;
  if(!m.mesh_owner->source_normals_v113.empty()&&!skinning::directions_v113(m.skin,m.palette,m.mesh_owner->source_normals_v113,m.mesh_owner->normals_v113,e))return false;
 }
 return true;
}
bool RetainedGameObjectVisualV1::initialize(const char* model,const char* xref,std::string& e){
 if(ready_||root_present_||bytes_)return missing("fresh constructor candidate",e);
 if(!model||!xref)return missing("source model/subscene strings",e);
 if(!services_.owner||!services_.read_asset)return missing("asset resource owner",e);
 bytes_=std::make_shared<std::vector<std::uint8_t>>();
 bool found=false;if(!services_.read_asset(model,*bytes_,found,e))return false;
 if(!found){bytes_.reset();ready_=true;return true;} // original root==NULL branch
 if(dh2_bres_open(&bres_,bytes_->data(),bytes_->size())!=resources::BresError::ok)return missing("valid source BRES",e);
 if(!scene::load_authored_v76(bres_,scene_,authored_visibility_v76_,e))return false;
 if(*xref){
  // Original getNode searches visualScene0 only. This complete-scene bridge
  // currently proves that domain when the BRES has exactly one visualScene.
  if(bres_.root_offset>bres_.size||bres_.size-bres_.root_offset<192||word(bres_.bytes+bres_.root_offset+152)!=1)return missing("multi-visual-scene source getNode(visualScene0)",e);
  scene::Scene selected;scene::AuthoredVisibilityV76 visibility;bool selected_found=false;if(!authored_scene_subtree_v2(scene_,xref,selected,selected_found,e,&authored_visibility_v76_,&visibility))return false;
  if(!selected_found){scene_={};authored_visibility_v76_={};ready_=true;return true;}
  scene_=std::move(selected);authored_visibility_v76_=std::move(visibility);
 }
 if(!binding_.bind(scene_,e))return false;
 node_flags_.bind(scene_.graph);node_flags_.assign(scene_.graph.size(),0x60fu);
 node_visibility_.bind(scene_.graph);node_visibility_.assign(scene_.graph.size(),Visibility{});
 detached_nodes_v76_.bind(scene_.graph);detached_meshes_v76_.bind(scene_.instances);
 detached_nodes_v76_.assign(scene_.graph.size(),0);detached_meshes_v76_.assign(scene_.instances.size(),0);
 mesh_fields_v76_.bind(scene_.instances);mesh_fields_v76_.assign(scene_.instances.size(),MeshFieldsV76{});
 for(std::size_t i=0;i<node_flags_.size();++i){
  const auto parent=scene_.graph[i].parent;auto& v=node_visibility_[i];
  v.local120=authored_visibility_v76_.node_local[i];v.parent121=parent<0?1u:authored_visibility_v76_.node_effective[parent];
  if(!authored_visibility_v76_.node_effective[i])node_flags_[i]&=~1u;
 }
 for(std::size_t i=0;i<mesh_fields_v76_.size();++i){if(detached_meshes_v76_[i])continue;mesh_fields_v76_[i].visibility.local120=authored_visibility_v76_.mesh_local[i];update_mesh_visibility_v76(i,(node_flags_[scene_.instances[i].node_index]&1u)!=0);}
 if(!initialize_child_owners_v91(e))return false;
 root_present_=true;
 root_force_position208_v96_=1; //New source RootC1, including owner reuse after release.
 // SetParent47295c: dynamic receiver (or animated static receiver) recursively
 // sets a source node bool. Optimized static transform baking is separate.
 if(!fields_.static84)return missing("SAME parent source static84",e);
 bool dynamic=*fields_.static84==0;
 if(!dynamic){if(!services_.parent_is_animated)return missing("parent IsAnimated virtual",e);if(!services_.parent_is_animated(dynamic,e))return false;}
 // Source IsAnimated precedes Sync in the static branch.
 if(!sync(e))return false;
 if(!dynamic){if(!optimize_static_v76(e))return false;}
 // Exact recursive setOnAnimateEnabled50e46c: OR scene bit200 on root and
 // every descendant; these are the retained node fields, not display flags.
 if(dynamic){binding_.root.flags|=0x200u;for(auto& flags:node_flags_)flags|=0x200u;for(auto& mesh:mesh_fields_v76_)mesh.flags|=0x200u;}
 // Constructor explicitly registers root and forces SceneManager registration.
 if(!services_.register_root||!services_.force_register)return missing("SceneManager registration services",e);
 if(!services_.register_root(root_identity(),e))return false;
 if(!load_skinned_meshes(e))return false;
 if(!physical::decor_scene_marker(bres_,scene_,marker_,e))return false;
 if(!calc_mesh_box(e)||!apply_mesh_box(e))return false;
 bool has_modular{};if(!skinning::VisualSkinResourcesV6::source_has_modular_v114(bres_,has_modular,e))return false;
 if(has_modular){
  modular_resources_v114_.emplace();if(!modular_resources_v114_->load(*bytes_,e))return false;
  skinning::VisualAssetServicesV6 assets;assets.context=this;assets.read=[](void* raw,const char* uri,std::vector<std::uint8_t>& bytes,std::string& e){auto& self=*static_cast<RetainedGameObjectVisualV1*>(raw);bool found{};
   if(!uri||!self.services_.read_asset||!self.services_.read_asset(uri,bytes,found,e))return skinning::VisualAssetResultV6::failed;
   return found?skinning::VisualAssetResultV6::found:skinning::VisualAssetResultV6::missing;};
  own_modular_v114_=std::make_shared<skinning::VisualSkinOwnerV6>(modular_resources_v114_->borrow(),scene_,assets);
  if(!own_modular_v114_->initialize(e))return false;
 }
 //472b38..74: refresh bounds, SAME global SearchByType/last modular loan,
 //then ForceRegister. Never infer2c from this object's resource alone.
 if(!services_.find_modular_v114||!services_.find_modular_v114(selected_modular2c_v114_,e))return missing("SceneManager source modular-node search",e);
 if(!services_.force_register(root_identity(),e))return false;
 animated_=dh2_bres_library_count(&bres_,resources::Library::animation)>0;
 // Original applyAnimationValues65d9c4..65da08 skips NULL target bindings;
 // shared ItemDrops clips legitimately contain channels for other subtrees.
 if(animated_&&!animation_.load(bytes_->data(),bytes_->size(),scene_,e,*xref?animation::MissingTargets::ignore:animation::MissingTargets::reject))return false;
 root_animator_present_=animated_;
 if(!clips(e))return false;
 // AnimController(root,false) installs literal DoNothing endpoints; its
 // SetCallbacks virtual is bx lr. No gameplay event callback is substituted.
 base_named_animation_set_callbacks_v1();generic_callbacks_v69_.set(GenericAnimatorCallbackFieldsV21::source_do_nothing());ready_=true;return true;
}
bool RetainedGameObjectVisualV1::sync_rotation_v86(std::string& e){
   if(!root_present_||source_parent_detached_v112_){e.clear();return true;}
 auto* rotation=base_?base_->vector3(0x16c):fields_.rotation16c;
 if(!rotation)return missing("SAME parent Rotation16c",e);
 if(!binding_.set_rotation(rotation)||!update_root_cache_v76(e))return false;
 if(!static_optimized_v76_&&!binding_.update_world(scene_,e))return false;
 return update_skinned_meshes(e);
}
 bool RetainedGameObjectVisualV1::sync_scaling_v111(std::string& e){
  if(!root_present_||source_parent_detached_v112_){e.clear();return true;}
  const auto* scale=base_?base_->vector3(0x120):fields_.scale120;
  if(!scale)return missing("SAME parent Scaling120",e);
  std::copy_n(scale,3,binding_.root.scale);
  if(!update_root_cache_v76(e))return false;
  if(!static_optimized_v76_&&!binding_.update_world(scene_,e))return false;
  return update_skinned_meshes(e);
 }
 bool RetainedGameObjectVisualV1::sync(std::string& e){
 if(!root_present_||source_parent_detached_v112_)return true;
 auto* p=fields_.position160;auto* r=fields_.rotation16c;auto* scale=fields_.scale120;
 if(!p||!r||!scale)return missing("SAME parent pose/scale fields",e);
 std::copy_n(p,3,binding_.root.position);std::copy_n(scale,3,binding_.root.scale);
 if(!binding_.set_rotation(r)||!update_root_cache_v76(e))return false;
 // Optimized children retain their absolute caches: recomposition would apply
 // the root again to already world-baked local position/quaternion fields.
 if(!static_optimized_v76_){if(!binding_.update_world(scene_,e))return false;for(auto& flags:node_flags_)flags=(flags&~0xeu)|0x120u;for(auto& flags:node_flags_)flags&=~0x50u;}
 return update_skinned_meshes(e);
}
bool RetainedGameObjectVisualV1::update_root_cache_v76(std::string& e){
 dh2_node_matrix(root_cached_v76_.data(),binding_.root.position,binding_.root.quaternion,binding_.root.scale);
 for(float value:root_cached_v76_)if(!std::isfinite(value))return missing("finite actual root TRS",e);
 binding_.root.flags=(binding_.root.flags&~0xeu)|0x10u;
 binding_.root.flags=(binding_.root.flags|0x120u)&~0x50u;return true;
}
void RetainedGameObjectVisualV1::optimize_node_v76(unsigned index){
 auto& node=scene_.graph[index];auto& flags=node_flags_[index];
 const auto parent_flags=node.parent<0?binding_.root.flags:node_flags_[node.parent];
 // Exact ModuleStaticSceneV2/ISceneNode source updateAbsolutePosition control.
 if((parent_flags&0x20u)||(flags&0x5eu)){
  std::array<float,16> relative;dh2_node_matrix(relative.data(),node.translation,node.quaternion,node.scale);
  flags=(flags&~0xeu)|0x10u;
  node.world=scene::multiply(node.parent<0?root_cached_v76_:scene_.graph[node.parent].world,relative);
  flags=(flags|0x120u)&~0x50u;
 }
 std::copy_n(node.world.data()+12,3,node.translation);flags|=8u;
 math::Matrix4f matrix{};std::copy(node.world.begin(),node.world.end(),matrix.m);matrix.identity_hint=0;
 math::Quaternion q{};dh2_quat_from_matrix(&q,&matrix);
 node.quaternion[0]=q.x;node.quaternion[1]=q.y;node.quaternion[2]=q.z;node.quaternion[3]=q.w;
 flags=(flags|4u)&~0x200u;
 // Real geometry children precede child SNodes. Mesh C1 local scale stays1.
 for(unsigned i=0;i<scene_.instances.size();++i){auto& instance=scene_.instances[i];if(instance.node_index!=index||detached_meshes_v76_[i])continue;
  instance.world=node.world;auto& fields=mesh_fields_v76_[i];
  std::copy_n(instance.world.data()+12,3,fields.position.begin());
  std::copy(instance.world.begin(),instance.world.end(),matrix.m);matrix.identity_hint=0;dh2_quat_from_matrix(&q,&matrix);
  fields.quaternion={q.x,q.y,q.z,q.w};fields.flags=(fields.flags|0xcu)&~0x200u;
 }
 for(unsigned child=index+1;child<scene_.graph.size();++child)if(scene_.graph[child].parent==static_cast<std::int32_t>(index))optimize_node_v76(child);
}
bool RetainedGameObjectVisualV1::optimize_static_v76(std::string& e){
 if(!root_present_||static_optimized_v76_)return missing("fresh SAME static OptimizeStatic receiver",e);
 // SetParent47295c repeats root.setPosition(root.getPosition), retaining Sync's
 // cache. OptimizeStatic50f220 changes local TRS/flags, never extracts scale.
 binding_.root.flags=(binding_.root.flags|0xcu)&~0x200u;
 math::Matrix4f matrix{};std::copy(root_cached_v76_.begin(),root_cached_v76_.end(),matrix.m);matrix.identity_hint=0;
 math::Quaternion q{};dh2_quat_from_matrix(&q,&matrix);
 binding_.root.quaternion[0]=q.x;binding_.root.quaternion[1]=q.y;binding_.root.quaternion[2]=q.z;binding_.root.quaternion[3]=q.w;
 for(unsigned i=0;i<scene_.graph.size();++i)if(scene_.graph[i].parent<0)optimize_node_v76(i);
 static_optimized_v76_=true;return true;
}
bool RetainedGameObjectVisualV1::node_attached_v76(unsigned i)const noexcept{return root_present_&&i<detached_nodes_v76_.size()&&!detached_nodes_v76_[i];}
bool RetainedGameObjectVisualV1::mesh_attached_v76(unsigned i)const noexcept{return root_present_&&i<detached_meshes_v76_.size()&&!detached_meshes_v76_[i];}
bool RetainedGameObjectVisualV1::mesh_visible_v76(unsigned i)const noexcept{return mesh_attached_v76(i)&&(binding_.root.flags&1u)&&(mesh_fields_v76_[i].flags&1u);}
bool RetainedGameObjectVisualV1::source_mesh_pose_v76(unsigned i,std::array<float,4>& quaternion,std::array<float,3>& scale,std::string& e)const{
 if(!root_present_||i>=mesh_fields_v76_.size())return missing("SAME actual mesh local TRS receiver",e);
 quaternion=mesh_fields_v76_[i].quaternion;scale=mesh_fields_v76_[i].scale;e.clear();return true;
}
void RetainedGameObjectVisualV1::update_mesh_visibility_v76(std::size_t i,bool parent){
 if(detached_meshes_v76_[i])return;auto& mesh=mesh_fields_v76_[i];mesh.visibility.parent121=parent?1:0;
 const bool visible=mesh.visibility.local120&&parent&&!detached_meshes_v76_[i];
 if(visible)mesh.flags|=1u;else mesh.flags&=~1u;
 authored_visibility_v76_.mesh_effective[i]=visible?1:0;
}
bool RetainedGameObjectVisualV1::remove_floor_mesh_v76(unsigned i,std::string& e){
 if(!static_optimized_v76_||!mesh_attached_v76(i))return missing("attached actual static floor mesh",e);
 // virtual48(false), then virtual68 detach: stable caches remain borrowed by
 // the source search snapshot and clone; registry/draw membership is removed.
 auto& mesh=mesh_fields_v76_[i];mesh.visibility.local120=0;mesh.flags&=~1u;
 authored_visibility_v76_.mesh_effective[i]=0;detached_meshes_v76_[i]=1;
 if(i<native_meshes_v93_.size()){auto native=native_meshes_v93_[i];if(auto parent=native->parent.lock()){auto found=std::find(parent->meshes.begin(),parent->meshes.end(),native);if(found!=parent->meshes.end())parent->meshes.erase(found);}native->parent.reset();native->fields->parentec=0;}e.clear();return true;
}
bool RetainedGameObjectVisualV1::remove_node_v76(unsigned i,std::string& e){
 if(!static_optimized_v76_||!node_attached_v76(i))return missing("attached actual static exit node",e);
 for(unsigned node=i;node<scene_.graph.size();++node){auto parent=static_cast<std::int32_t>(node);
  while(parent>=0&&static_cast<unsigned>(parent)!=i)parent=scene_.graph[parent].parent;
  if(parent<0)continue;detached_nodes_v76_[node]=1;node_flags_[node]&=~1u;authored_visibility_v76_.node_effective[node]=0;
  for(unsigned mesh=0;mesh<scene_.instances.size();++mesh)if(scene_.instances[mesh].node_index==node){detached_meshes_v76_[mesh]=1;update_mesh_visibility_v76(mesh,false);}
 }
 e.clear();return true;
}
bool RetainedGameObjectVisualV1::calc_mesh_box(std::string& e){
 if(!root_present_)return true;
 if(!marker_.found)return visual_mesh_box_fallback_v2(bres_,scene_,binding_.root.position,binding_.root.quaternion,binding_.root.scale,mesh_box_.data(),e);
 physical::DecorMeshBoxInput input{};std::copy_n(marker_.bounds,6,input.bounds);
 std::copy_n(marker_.parent_scale,3,input.parent_scale);
 dh2_node_matrix(input.node_matrix,binding_.root.position,binding_.root.quaternion,binding_.root.scale);
 if(dh2_decor_marker_mesh_box(mesh_box_.data(),&input))return missing("actual mesh-box kernel",e);
 return true;
}
bool RetainedGameObjectVisualV1::apply_mesh_box(std::string& e){
 if(!root_present_)return true;
 if(!base_){if(!fields_.receiver_lease)return missing("SAME derived SetRelativeAABB/PF owner",e);if(fields_.apply_mesh_box_v6)return fields_.apply_mesh_box_v6(mesh_box_.data(),marker_.found!=0,e);if(!fields_.apply_mesh_box)return missing("SAME derived SetRelativeAABB/PF owner",e);return fields_.apply_mesh_box(mesh_box_.data(),e);}
 auto* p=base_->vector3(0x160);auto* flat=base_->byte(0x15c);
 bool handled=false;if(!visual_aabb_dispatch_v3(*base_,mesh_box_.data(),marker_.found!=0,handled,e))return false;if(handled)return true;
 if(!p||!flat)return missing("SAME parent bounds/flat fields",e);
 physical::DecorBodyInput input{};input.owner=reinterpret_cast<void*>(base_->identity());input.visual_present=1;
 input.previous_flat=*flat;std::copy_n(mesh_box_.data(),6,input.mesh_box);std::copy_n(p,3,input.position);
 physical::DecorBodyConfig out{};if(dh2_decor_body_config(&out,&input))return missing("source mesh-box application",e);
 *flat=static_cast<std::uint8_t>(out.flat);std::copy_n(out.relative_box,6,base_->relative_aabb144());
 std::copy_n(out.absolute_box,6,base_->absolute_aabb12c());
 if(!services_.update_pf)return missing("SAME parent UpdatePFObject",e);
 return services_.update_pf(e);
}
bool RetainedGameObjectVisualV1::sample(bool reset,std::string& e){
 if(!animated_)return true;
 if(reset)return missing("displacement-enabled reset on generic scene",e);
 if(static_optimized_v76_)return missing("animation on static-baked graph requires actual source reparent/update producer",e);
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
 if(!ready_)return missing("completed constructor",e);
 if(!commit_root_visibility_v3({},e))return false;
 if(!root_present_||!animated_||!root_animator_present_)return true;
 timestamp_=ms;auto previous=timeline_.current_ms;std::int32_t signed_ms;std::memcpy(&signed_ms,&ms,4);
 timeline::Services callbacks{&completion_,[](void* p,timeline::State* t){if(dh2_timeline_notify(static_cast<timeline::Completion*>(p),t))throw std::runtime_error("source animation completion rejected");}};
 if(dh2_timeline_update(&timeline_,signed_ms,&callbacks))return missing("source timeline update",e);
 if(!sample(false,e))return false;
 if(completion_.pending)completion_.pending=0; // CheckCallback invokes installed DoNothing then clears
 // Original constructor callback table routes authored triggers to DoNothing.
 return dh2_events_update_interval(&animation_.events.view(),previous,timeline_.current_ms,nothing,this);
}
bool RetainedGameObjectVisualV1::generic_animator_identity_v21(std::uintptr_t& id,bool& present,std::string& e)const{
 id=0;present=false;if(!ready_&&!batch_animation_references_v112_)return missing("completed generic animator constructor",e);
 if(character_animator_remove_v6_)return missing("generic callbacks on replaced Character animator",e);
 present=(root_present_||batch_animation_references_v112_)&&animated_&&(root_animator_present_||batch_animation_references_v112_);if(present)id=reinterpret_cast<std::uintptr_t>(&animation_);return true;
}
bool RetainedGameObjectVisualV1::bind_character_scene_phase_v69(std::weak_ptr<void> lifetime,
 std::function<bool(std::uint32_t,std::string&)> body,std::string& e){
 if(!ready_||!root_present_||!root_animator_present_||!character_animator_remove_v6_||
    lifetime.expired()||!body||character_scene_phase_v69_)return missing("SAME attached Character scene-phase owner",e);
 character_scene_lifetime_v69_=std::move(lifetime);character_scene_phase_v69_=std::move(body);e.clear();return true;
}
bool RetainedGameObjectVisualV1::source_scene_phase_v69(std::uint32_t ms,
 const std::function<bool(std::string&)>& notify_visibility,std::string& e){
 if(!ready_||!root_present_)return missing("actual root scene phase",e);
 if(!commit_root_visibility_v3(notify_visibility,e))return false;
 const auto flags=binding_.root.flags;
 // Captured ISNode/Root enabled/hidden gates, not GPU visibility culling.
 // All admitted visible animators advance on the same actual Timer; bone FX
 // cannot lose their pose because a native GPU primitive was culled.
 if(((flags&0x400u)&&!(flags&1u))||!(flags&0x200u)){timestamp_=ms;e.clear();return true;}
 if(character_animator_remove_v6_){
  auto lifetime=character_scene_lifetime_v69_.lock();
  if(!lifetime||!character_scene_phase_v69_)return missing("live SAME replacement Character animator phase",e);
  if(!character_scene_phase_v69_(ms,e))return false;
 }else if(!update_callbacks_v21(ms,generic_callbacks_v69_,e))return false;
 timestamp_=ms;e.clear();return true;
}
bool RetainedGameObjectVisualV1::update_callbacks_v21(std::uint32_t ms,GenericAnimatorCallbackFieldsV21& fields,std::string& e){
 std::uintptr_t identity{};bool present{};if(!generic_animator_identity_v21(identity,present,e))return false;
 if(root_present_&&!commit_root_visibility_v3({},e))return false;if(!present)return true;
 timestamp_=ms;
 // Source applyAnimationValues65d91c skips updateTime when both transform
 // bindings and events are absent. All mutable clock/completion storage is
 // the same retained animator's existing fields.
 if(animation_.track_count()||animation_.events.view().count){
  const auto previous=timeline_.current_ms;std::int32_t signed_ms;std::memcpy(&signed_ms,&ms,4);
  timeline::Services ending{&completion_,[](void* p,timeline::State* t){if(dh2_timeline_notify(static_cast<timeline::Completion*>(p),t))throw std::runtime_error("source animation completion rejected");}};
  if(dh2_timeline_update(&timeline_,signed_ms,&ending))return missing("source callback timeline update",e);
  // Original updateTime667c48 dispatches events BEFORE transform sampling;
  // source Animator.animateNode3662c8 checks completion AFTER both.
  const auto& callback=fields.fields();auto event=callback.event;auto lease=callback.event_owner;bool delivered=true;
  struct Context {decltype(event)* fn;bool* ok;std::string* error;RetainedGameObjectVisualV1* visual;};Context context{&event,&delivered,&e,this};
  struct DeliveryStopped{};
  auto dispatch=[](const animation::TriggeredEvent* value,void* p){auto& c=*static_cast<Context*>(p);if(!(*c.fn)(*value,*c.error)){*c.ok=false;throw DeliveryStopped{};}if((!c.visual->ready()||!c.visual->root_animator_present())&&!c.visual->batch_animation_retained_v112()){*c.error="Required generic animator removed during event delivery";*c.ok=false;throw DeliveryStopped{};}};
  try{if(animation_.events.view().count&&!dh2_events_update(&animation_.events.view(),&fields.event_cursor(),previous,timeline_.current_ms,timeline_.start_ms,timeline_.end_ms,event?+dispatch:nullptr,&context))return missing("source actual event track update",e);}catch(const DeliveryStopped&){return false;}
  if(!delivered)return false;if((!ready_||!root_present_||!root_animator_present_)&&!batch_animation_references_v112_)return missing("generic animator removed during required event callback",e);
  if(!sample(false,e))return false;
 }
 return fields.check(completion_,timeline_,e);
}
bool RetainedGameObjectVisualV1::release(std::string& e){
 // VisualObjectD1 473884 releases its controller before the retained root,
 // then invokes SceneManager::ForceRegister. Preserve required delivery.
 if(!services_.force_register)return missing("source destructor ForceRegister",e);
 auto identity=root_identity();if(root_present_){
  if(!services_.release_root)return missing("source root release",e);
  if(!services_.release_root(identity,e))return false;root_present_=false;
 }if(!services_.force_register(identity,e))return false;
 for(const auto& node:node_handles_)if(node&&node->native_parent_owned_v106&&node->parent.expired()){if(!node->drop_parent_native_v106(e))return false;}
 for(const auto& mesh:native_meshes_v93_)if(mesh&&mesh->fields&&!mesh->fields->parentec&&!mesh->native_destroyed_v106){if(!retire_retained_map_mesh_v106(mesh,e))return false;}
 if(batch_animation_references_v112_){ready_=false;batch_visual_released_v112_=true;root_animator_present_=false;e.clear();return true;}
 selected_modular2c_v114_={};own_modular_v114_.reset();modular_resources_v114_.reset();
 ready_=false;animated_=false;animation_=animation::Player{};scene_=scene::Scene{};
 bytes_.reset();bres_={};clips_.clear();node_flags_.clear();node_visibility_.clear();node_handles_.clear();native_meshes_v93_.clear();skinned_.clear();mesh_resources_v91_.clear();root_animator_present_=false;
 authored_visibility_v76_={};mesh_fields_v76_.clear();detached_nodes_v76_.clear();detached_meshes_v76_.clear();static_optimized_v76_=false;return true;
}
bool RetainedGameObjectVisualV1::set_root_game_object(std::uintptr_t identity,std::string& e){
 if(!root_present_||identity!=fields_.identity)return missing("SAME root+204 parent identity",e);
 root_game_object204_=identity;return true;
}
bool RetainedGameObjectVisualV1::node_from_name(const char* name,std::uintptr_t& result,std::string& e)const{
 result=0;if(!root_present_||!name)return missing("retained root/name lookup",e);
 // Source search order is depth-first child order. Scene::load emits that order.
 for(std::size_t i=0;i<scene_.graph.size();++i)if(node_attached_v76(static_cast<unsigned>(i))&&scene_.graph[i].name==name){result=reinterpret_cast<std::uintptr_t>(node_handles_[i].get());break;}return true;
}
bool RetainedGameObjectVisualV1::node_position(std::uintptr_t handle,float* out,std::string& e)const{
 if(!out)return missing("node position output",e);
 for(auto& node:node_handles_)if(reinterpret_cast<std::uintptr_t>(node.get())==handle){auto& matrix=scene_.graph[node->index].world;for(unsigned i=0;i<3;++i)out[i]=matrix[12+i];return true;}
 return missing("SAME retained scene node identity",e);
}
bool RetainedGameObjectVisualV1::borrow_node_position_v80(std::uintptr_t handle,const float*& out,std::string& e)const{
 out=nullptr;
 if(!root_present_||!ready_)return missing("live SAME retained target scene",e);
 for(const auto& node:node_handles_)if(reinterpret_cast<std::uintptr_t>(node.get())==handle){
  if(node->index>=scene_.graph.size())return missing("SAME retained target node index",e);
  out=scene_.graph[node->index].world.data()+12;e.clear();return true;
 }return missing("SAME live target node handle",e);
}
bool RetainedGameObjectVisualV1::source_modular_receivers_v114(std::weak_ptr<void> root,std::vector<skinning::SourceModularSkinBorrowV114>& out,std::string& e)const{
 if(!root_present_||root.expired()){e="Retired actual modular root search";return false;}
 if(own_modular_v114_)out.push_back({std::move(root),own_modular_v114_});e.clear();return true;
}
std::optional<std::uint32_t> RetainedGameObjectVisualV1::source_modular_node_v114()const{
 if(!own_modular_v114_||!modular_resources_v114_)return {};return modular_resources_v114_->borrow().modular_node();
}
bool RetainedGameObjectVisualV1::source_set_modular_skin_v114(std::int32_t category,std::int32_t module,std::string& e){
 //Whole Visual.SetModularSkin470e18: NULL2c/category-1 genuine return;
 //actual selected mesh mutation followed by Scene visibility notification.
 if(!selected_modular2c_v114_.mesh||category==-1){e.clear();return true;}
 auto root=selected_modular2c_v114_.root.lock();if(!root)return missing("live selected source modular2c root",e);
 if(!selected_modular2c_v114_.mesh->set_modular(category,module,e))return false;
 if(!services_.notify_visibility_v114)return missing("SceneManager modular visibility notification",e);
 return services_.notify_visibility_v114(e);
}
bool RetainedGameObjectVisualV1::modular_draw_views_v114(const std::vector<skinning::VisualDrawViewV32>*& out,std::string& e)const{
 out=nullptr;if(!own_modular_v114_){e.clear();return true;}return own_modular_v114_->draw_views(out,e);
}
bool RetainedGameObjectVisualV1::source_node_set_position_v112(std::uintptr_t handle,const float* xyz,std::string& e){
 if(!xyz||!ready_)return missing("live node SetPosition receiver",e);
 for(const auto& node:node_handles_)if(reinterpret_cast<std::uintptr_t>(node.get())==handle){
  if(node->index>=scene_.graph.size()||!node_attached_v76(node->index))return missing("attached same source node",e);
  auto& actual=scene_.graph[node->index];std::memcpy(actual.translation,xyz,12);node_flags_[node->index]|=8u;
  e.clear();return true;
 }
 return missing("selected source node SetPosition",e);
}
bool RetainedGameObjectVisualV1::source_root_set_position_v112(const float* xyz,std::string& e){
 if(!xyz||!ready_)return missing("source Visual.SetPosition receiver",e);
 if(root_present_){std::memcpy(binding_.root.position,xyz,12);binding_.root.flags|=8u;}
 e.clear();return true;
}
bool RetainedGameObjectVisualV1::source_current_clip_duration_v112(std::uint32_t group,std::int32_t& out,std::string& e)const{
 if(!ready_)return missing("source AnimController.GetClipDuration receiver",e);
 //This retained generic controller has the SAME one root animator, group0.
 //GetAnim(other)/absent animator is the source NULL -> return-1 branch.
 if(group||!root_present_||!root_animator_present_||!animated_){out=-1;e.clear();return true;}
 const auto index=timeline_.clip_index;
 if(index<0||static_cast<std::size_t>(index)>=clips_.size())return missing("actual selected clip library",e);
 const auto& clip=clips_[index];const std::uint32_t duration=std::uint32_t(clip.end)-std::uint32_t(clip.start);
 std::memcpy(&out,&duration,4);e.clear();return true;
}
void RetainedGameObjectVisualV1::propagate_node_visibility(std::size_t parent){
 for(std::size_t m=0;m<scene_.instances.size();++m)if(scene_.instances[m].node_index==parent)update_mesh_visibility_v76(m,(node_flags_[parent]&1u)!=0);
 for(std::size_t i=0;i<scene_.graph.size();++i)if(scene_.graph[i].parent==static_cast<std::int32_t>(parent))notify_node_visibility(i,(node_flags_[parent]&1u)!=0);
}
void RetainedGameObjectVisualV1::notify_node_visibility(std::size_t i,bool parent){
 auto& v=node_visibility_[i];auto previous=node_flags_[i]&1u;v.parent121=parent?1:0;
 if(v.local120&&parent&&!detached_nodes_v76_[i])node_flags_[i]|=1u;else node_flags_[i]&=~1u;
 if(previous!=(node_flags_[i]&1u))propagate_node_visibility(i);
}
bool RetainedGameObjectVisualV1::notify_root_visibility(bool parent,std::string& e){
 if(!root_present_||node_visibility_.size()!=scene_.graph.size())return missing("SAME root visibility graph",e);
 auto previous=binding_.root.flags&1u;root_visibility_.parent121=parent?1:0;
 if(root_visibility_.local120&&parent)binding_.root.flags|=1u;else binding_.root.flags&=~1u;
 if(previous!=(binding_.root.flags&1u))for(std::size_t i=0;i<scene_.graph.size();++i)if(scene_.graph[i].parent<0)notify_node_visibility(i,(binding_.root.flags&1u)!=0);
 return true;
}
bool RetainedGameObjectVisualV1::set_root_local_visibility_v3(bool local,std::string& e){
 if(!root_present_)return true;
 auto previous=binding_.root.flags&1u;
 if(previous!=std::uint32_t(local)){if(!services_.force_register)return missing("SetVisible source ForceRegister",e);if(!services_.force_register(root_identity(),e))return false;}
 root_visible_request209_=local?1:0;
 if(local)return true; // RootSceneNode35c26c defers positive visibility.
 if(root_visibility_.local120==std::uint8_t(local))return true;
 root_visibility_.local120=local?1:0;
 if(local&&root_visibility_.parent121)binding_.root.flags|=1u;else binding_.root.flags&=~1u;
 if(previous!=(binding_.root.flags&1u))for(std::size_t i=0;i<scene_.graph.size();++i)if(scene_.graph[i].parent<0)notify_node_visibility(i,(binding_.root.flags&1u)!=0);
 return true;
}
bool RetainedGameObjectVisualV1::commit_root_visibility_v3(const std::function<bool(std::string&)>& notify,std::string& e){
 if(root_visibility_failed_v3_)return missing("retained failed visibility notification prefix",e);
 if(!root_present_||!root_visible_request209_||(binding_.root.flags&1u))return true;
 // First reached onAnimate35d310 prefix: qualified ISceneNode.setVisible(true),
 // actual SceneManager notification, then byte209 clear. Keep failure prefix.
 auto previous=binding_.root.flags&1u;root_visibility_.local120=1;
 if(root_visibility_.parent121)binding_.root.flags|=1u;else binding_.root.flags&=~1u;
 if(previous!=(binding_.root.flags&1u))for(std::size_t i=0;i<scene_.graph.size();++i)if(scene_.graph[i].parent<0)notify_node_visibility(i,true);
 if(!notify){root_visibility_failed_v3_=true;return missing("SceneManager notifyVisibilityChanged5890a8",e);}
 if(!notify(e)){root_visibility_failed_v3_=true;return false;}root_visible_request209_=0;return true;
}
bool RetainedGameObjectVisualV1::set_node_local_visibility(std::uintptr_t handle,bool local,std::string& e){
 for(auto& h:node_handles_)if(reinterpret_cast<std::uintptr_t>(h.get())==handle){auto i=h->index;auto& v=node_visibility_[i];if(v.local120==std::uint8_t(local))return true;
  auto previous=node_flags_[i]&1u;v.local120=local?1:0;if(local&&v.parent121&&!detached_nodes_v76_[i])node_flags_[i]|=1u;else node_flags_[i]&=~1u;
  if(previous!=(node_flags_[i]&1u))propagate_node_visibility(i);return true;
 }
 return missing("SAME node local visibility receiver",e);
}
bool RetainedGameObjectVisualV1::sync_position_v7(std::string& error){
 error.clear();if(!root_present_||source_parent_detached_v112_)return true;
 const float* position=base_?base_->vector3(0x160):fields_.position160;
 if(!position)return missing("SAME parent Position160",error);
 // Whole470cb8 ->470c24 position-only/root update domain. Snapshot before
 // writing the actual root, preserving source position argument semantics.
 const std::array<float,3> snapshot{position[0],position[1],position[2]};
 std::copy(snapshot.begin(),snapshot.end(),binding_.root.position);
 if(!update_root_cache_v76(error))return false;
 if(!static_optimized_v76_&&!binding_.update_world(scene_,error))return false;
 return update_skinned_meshes(error);
}
bool RetainedGameObjectVisualV1::remove_root_animators(std::string& e){
 if(!root_present_)return missing("SAME root removeAnimators receiver",e);
 if(character_animator_remove_v6_){if(!character_animator_remove_v6_(e))return false;character_animator_remove_v6_={};}
 if(batch_animation_references_v112_){root_animator_present_=false;e.clear();return true;}
 // Source removeAnimators598658 detaches and drops each owned animator;
 // CSceneNodeAnimatorD1 removes tracks, drops its AnimationBlock and database.
 // Player is this native successor's sole owned track/block/database graph.
 // Move assignment destroys that actual graph while leaving sampled pose.
 character_scene_phase_v69_={};character_scene_lifetime_v69_.reset();generic_callbacks_v69_.set({});
 animation_=animation::Player{};root_animator_present_=false;animated_=false;
 clips_.clear();timeline_={};completion_={};return true;
}
bool RetainedGameObjectVisualV1::attach_character_animator_v6(std::function<bool(std::string&)> remove,std::string& error){
 if(!root_present_||!ready_||!remove)return missing("SAME Character animator root/endpoint",error);
 if(!remove_root_animators(error))return false;
 character_animator_remove_v6_=std::move(remove);root_animator_present_=true;return true;
}
bool RetainedGameObjectVisualV1::character_pose_changed_v6(std::string& error){
 if((!root_present_||!ready_||!character_animator_remove_v6_)&&!batch_animation_references_v112_)return missing("attached SAME Character animator pose",error);
 return update_skinned_meshes(error);
}
}

namespace dh2::world {
bool RetainedGameObjectVisualV1::initialize_child_owners_v91(std::string& e){
 node_handles_.reserve(scene_.graph.size());
 for(std::size_t i=0;i<scene_.graph.size();++i){
  auto node=std::make_shared<RetainedVisualNodeV91>();node->index=static_cast<std::uint32_t>(i);node->fields=scene_.graph[i].source_owner_v91();node->native_parent_owned_v106=true;node_handles_.push_back(std::move(node));
 }
 for(std::size_t i=0;i<scene_.graph.size();++i){const auto parent=scene_.graph[i].parent;
  if(parent>=0){if(static_cast<std::size_t>(parent)>=i)return missing("actual child parent order",e);node_handles_[i]->parent=node_handles_[parent];node_handles_[parent]->children.push_back(node_handles_[i]);}
 }
 mesh_resources_v91_.resize(scene_.instances.size());
 for(std::size_t i=0;i<scene_.instances.size();++i){const auto& instance=scene_.instances[i];
  if(instance.node_index>=node_handles_.size())return missing("actual child mesh parent",e);
  if(!RetainedMeshDataV91::create(bres_,instance.geometry,mesh_resources_v91_[i],e))return false;
  auto mesh=std::make_shared<RetainedMeshNodeV91>();mesh->fields=instance.source_owner_v91();mesh->mesh=mesh_resources_v91_[i];mesh->parent=node_handles_[instance.node_index];mesh->fields->parentec=reinterpret_cast<std::uintptr_t>(node_handles_[instance.node_index].get());mesh->source_resource_v93=bytes_;mesh->source_image_v93=bres_;
  native_meshes_v93_.push_back(mesh);node_handles_[instance.node_index]->meshes.push_back(std::move(mesh));
 }
 if(!construct_retained_scene_lights_v113(bres_,scene_,node_handles_,native_meshes_v93_,e))return false;
 e.clear();return true;
}
bool RetainedGameObjectVisualV1::grab_node_v91(std::uintptr_t id,std::shared_ptr<RetainedVisualNodeV91>& out,std::string& e)const{
 if(!root_present_||!ready_)return missing("live actual child grab",e);
 for(const auto& node:node_handles_)if(reinterpret_cast<std::uintptr_t>(node.get())==id)return node->grab_native_v106(out,node,e);
 return missing("SAME original named child identity",e);
}
}

namespace dh2::world {
bool RetainedGameObjectVisualV1::source_set_visible_recur_v91(bool visible,std::string& e){
 if(!root_present_)return missing("actual recursive visibility root",e);
 if(!set_root_local_visibility_v3(visible,e))return false;
 for(const auto& node:node_handles_)if(!set_node_local_visibility(reinterpret_cast<std::uintptr_t>(node.get()),visible,e))return false;
 for(std::size_t i=0;i<mesh_fields_v76_.size();++i){mesh_fields_v76_[i].visibility.local120=visible?1:0;
  update_mesh_visibility_v76(i,(node_flags_[scene_.instances[i].node_index]&1u)!=0);}
 e.clear();return true;
}
}

namespace dh2::world {
bool RetainedGameObjectVisualV1::lend_map_mesh_v93(unsigned i,const std::shared_ptr<SceneManagerMapOwnerV2>& map,SceneMapNodeBorrowV2& out,std::string& e)const{
 if(!root_present_||!ready_||i>=native_meshes_v93_.size())return missing("SAME source-created generic mesh receiver",e);return lend_retained_map_mesh_v93(native_meshes_v93_[i],map,out,e);
}
}

namespace dh2::world {
bool RetainedGameObjectVisualV1::source_mesh_name_v93(unsigned i,const std::shared_ptr<SceneManagerMapOwnerV2>& map,std::string& out,unsigned& parent,std::string& e)const{
 if(!(root_present_&&ready_)||i>=native_meshes_v93_.size()){e="Required SAME current mesh/name receiver";return false;}return read_retained_mesh_name_v93(native_meshes_v93_[i],map,out,parent,e);
}
}
#include "retained_scene_node_borrow_v109.inc"
