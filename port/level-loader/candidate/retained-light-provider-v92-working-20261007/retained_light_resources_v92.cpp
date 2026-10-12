#include "retained_light_resources_v92.hpp"
#include <algorithm>
#include <cstring>
#include <limits>
#include <cmath>
#include <stdexcept>
#include "../engine-math/math.hpp"
namespace dh2::world {
namespace {
float bits(std::uint32_t v){float f;std::memcpy(&f,&v,4);return f;}
std::array<float,16> identity(){return {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};}
struct Reader {
 const resources::BresView& v;
 const std::uint8_t* at(std::uint64_t p,std::uint64_t n)const{if(!v.bytes||p>v.size||n>v.size-p)throw std::runtime_error("Light scene field outside actual BRES");return v.bytes+p;}
 std::uint32_t word(std::uint64_t p)const{auto q=at(p,4);return q[0]|(std::uint32_t(q[1])<<8)|(std::uint32_t(q[2])<<16)|(std::uint32_t(q[3])<<24);}
 float scalar(std::uint64_t p)const{float f=bits(word(p));if(!std::isfinite(f))throw std::runtime_error("Unsupported nonfinite authored light scalar");return f;}
 std::string text(std::uint32_t p)const{if(!p)return {};auto q=at(p,1);auto end=static_cast<const std::uint8_t*>(std::memchr(q,0,v.size-p));if(!end)throw std::runtime_error("Unterminated authored light URI");return {reinterpret_cast<const char*>(q),std::size_t(end-q)};}
 std::uint32_t find_light(const std::string& uri)const{
  if(uri.empty()||uri[0]!='#')throw std::runtime_error("Unsupported external authored light reference");
  for(std::uint32_t i=0;i<dh2_bres_library_count(&v,resources::Library::light);++i){auto p=dh2_bres_library_item(&v,resources::Library::light,i);if(!p)throw std::runtime_error("Invalid original light library item");auto o=std::uint32_t(p-v.bytes);if(text(word(o))==uri.substr(1))return o;}
  throw std::runtime_error("Unresolved SAME BRES light URI");
 }
};
void source_authored_light(const Reader& r,std::uint32_t p,RetainedLightSceneNodeV92& node){
 r.at(p,24);const auto type=r.word(p+8);if(type>3)throw std::runtime_error("Unsupported actual Collada light type");
 const float scale=r.scalar(p+16)/255.f;std::array<float,4> color;
 for(unsigned i=0;i<4;++i)color[i]=float(*r.at(p+12+i,1))*scale;
 auto& light=*node.light;light.specular24=color;
 if(type==0){light.type58=3;light.ambient4=color;light.diffuse14={0,0,0,0};light.specular24={0,0,0,0};}
 else{light.diffuse14=color;light.type58=type==1?0:type==2?1:2;
  if(type==1||type==2){auto q=r.word(p+20);r.at(q,type==2?20:12);for(unsigned i=0;i<3;++i)light.attenuation34[i]=r.scalar(q+4*i);
   if(type==2){light.outer48=r.scalar(q+12);light.inner4c=r.scalar(q+16);}}
 }
 node.recalculate_source_bounds();
}
}
RetainedLightFieldsV92::RetainedLightFieldsV92():radius40(bits(0x7f7fffff)){}
const float* RetainedLightFieldsV92::absolute_transform()const noexcept{return transformation?transformation->world.data():retired_transformation?retired_transformation->data():nullptr;}
RetainedLightSceneNodeV92::RetainedLightSceneNodeV92(){
 fields=std::make_shared<scene::NodeStorageV91>();fields->world=identity();
 light=std::make_shared<RetainedLightFieldsV92>();light->transformation=fields;
 recalculate_source_bounds();
}
RetainedLightSceneNodeV92::~RetainedLightSceneNodeV92(){
 //5848a4 copies transformation ownership only when another CLight reference
 //survives the node; never retain this deleting node through the LightSet.
 if(light&&light.use_count()>1&&light->transformation){
  // Its already owned node storage becomes the surviving transformation
  // source. No allocation/throwing destructor or pointer into deleted node.
  light->transformation=fields;
 }
}
void RetainedLightSceneNodeV92::recalculate_source_bounds()noexcept{
 const auto type=light->type58;
 if(type<=1){if(light->radius40==bits(0x7f7fffff))culling118=0;
  else{const float half=(light->radius40*light->radius40)*.5f;bounds13c={-half,-half,-half,half,half,half};culling118=1;}}
 else if(type==2){bounds13c={0,0,0,0,0,0};culling118=0;}
 cached_type138=type;
}
bool RetainedLightSceneNodeV92::update_static_node(std::uint32_t,std::string& e){
 if(!fields||!light||!light->transformation){e="Retired SAME light transformation";return false;}
 // No animator was constructed for this retained static light. This is the
 // reached ISceneNode transform-cache primitive; Scene alone owns its clock.
 std::array<float,16> relative;dh2_node_matrix(relative.data(),fields->translation,fields->quaternion,fields->scale);
 if(auto p=parent.lock())fields->world=scene::multiply(p->fields->world,relative);else fields->world=relative;
 fields->flags&=~0x40u;e.clear();return true;
}
bool RetainedLightSceneV92::load(const resources::BresView& image,std::shared_ptr<RetainedLightSceneV92>& out,std::string& e){
 try{
  if(dh2_bres_library_count(&image,resources::Library::animation)){e="Authored light-scene animation owner is not admitted by the static light domain";return false;}
  auto result=std::make_shared<RetainedLightSceneV92>();scene::AuthoredVisibilityV76 visible;
  if(!scene::load_authored_v76(image,result->scene,visible,e))return false;
  for(std::size_t i=0;i<result->scene.graph.size();++i){auto node=std::make_shared<RetainedVisualNodeV91>();node->index=std::uint32_t(i);node->fields=result->scene.graph[i].source_owner_v91();
   node->fields->visibility={visible.node_local[i],std::uint8_t(result->scene.graph[i].parent<0?1:visible.node_effective[std::size_t(result->scene.graph[i].parent)])};
   if(!visible.node_effective[i])node->fields->flags&=~1u;result->nodes.push_back(std::move(node));}
  for(std::size_t i=0;i<result->nodes.size();++i){auto p=result->scene.graph[i].parent;if(p>=0){result->nodes[i]->parent=result->nodes[std::size_t(p)];result->nodes[std::size_t(p)]->children.push_back(result->nodes[i]);}}
  for(const auto& instance:result->scene.instances){if(instance.controller>=0){e="Actual skinned light-scene asset needs its admitted skin resource owner";return false;}
   auto mesh=std::make_shared<RetainedMeshNodeV91>();mesh->fields=instance.source_owner_v91();if(!RetainedMeshDataV91::create(image,instance.geometry,mesh->mesh,e))return false;result->nodes.at(instance.node_index)->meshes.push_back(mesh);}
  Reader reader{image};std::size_t index{};std::vector<std::uint32_t> active;
  // Rewalk only the SAME serialized source node/instance structure to create
  // its genuine light resources. Runtime attachment never rescans actor XML.
  const auto visit=[&](auto&& visit,std::uint32_t p)->void{
   reader.at(p,80);if(index>=result->nodes.size()||active.size()>=64||std::find(active.begin(),active.end(),p)!=active.end())throw std::runtime_error("Invalid light-scene source hierarchy");
   active.push_back(p);auto node=result->nodes[index++];auto count=reader.word(p+64),instances=reader.word(p+68);reader.at(instances,std::uint64_t(count)*8);
   for(std::uint32_t i=0;i<count;++i){auto a=instances+8*i,tag=reader.word(a);if(tag==2||tag==3)continue;
    if(tag!=4)throw std::runtime_error("Unsupported actual light-scene instance kind");auto ref=reader.word(a+4);reader.at(ref,8);if(reader.word(ref))throw std::runtime_error("External light-scene resource not admitted");
    auto light=std::make_shared<RetainedLightSceneNodeV92>();source_authored_light(reader,reader.find_light(reader.text(reader.word(ref+4))),*light);
    light->parent=node;node->children.push_back(light);result->lights188.push_back(std::move(light));}
   count=reader.word(p+56);auto children=reader.word(p+60);reader.at(children,std::uint64_t(count)*80);for(std::uint32_t i=0;i<count;++i)visit(visit,children+80*i);active.pop_back();
  };
  auto root=image.root_offset;reader.at(root,192);auto scenes=reader.word(root+152),base=reader.word(root+156);reader.at(base,std::uint64_t(scenes)*16);
  auto count=reader.word(root+184),instances=reader.word(root+188);reader.at(instances,std::uint64_t(count)*8);
  for(std::uint32_t i=0;i<count;++i){auto p=instances+8*i;if(reader.word(p)!=6)throw std::runtime_error("Unsupported light-scene root instance");auto ref=reader.word(p+4);reader.at(ref,8);if(reader.word(ref))throw std::runtime_error("External light visual scene");auto uri=reader.text(reader.word(ref+4));bool found=false;
   for(std::uint32_t j=0;j<scenes;++j){auto s=base+16*j;if(uri!="#"+reader.text(reader.word(s)))continue;found=true;auto n=reader.word(s+8),nodes=reader.word(s+12);reader.at(nodes,std::uint64_t(n)*80);for(std::uint32_t k=0;k<n;++k)visit(visit,nodes+80*k);}
   if(!found)throw std::runtime_error("Unresolved light visual scene");}
  if(index!=result->nodes.size())throw std::runtime_error("Light-scene hierarchy differs from SAME scene parser");out=std::move(result);e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return false;}
}
SceneLightResourcesV92::SceneLightResourcesV92(std::shared_ptr<LightSetNameOwnerV3> names):names_(std::move(names)),owning_thread_(std::this_thread::get_id()){}
bool SceneLightResourcesV92::set_light(std::int32_t set,std::int32_t slot,std::shared_ptr<RetainedLightFieldsV92> light,std::string& e){
 if(!owning_thread()||set<0||set>=4||slot<0||slot>=5){e="Original LightSet SetLight domain/owning-thread requirement";return false;}
 auto& actual=lights78_[std::size_t(set)][std::size_t(slot)];if(actual!=light)dirty64_[std::size_t(set)][std::size_t(slot)]=1;
 actual=std::move(light);e.clear();return true;
}
bool SceneLightResourcesV92::add_automatic(std::uintptr_t actor,std::shared_ptr<RetainedLightSceneNodeV92> node,std::string& e){
 if(!owning_thread()||!actor||!node){e="Required actual automatic LightBase/node source entry";return false;}
 automatic424_.push_back({actor,node});e.clear();return true;
}
bool SceneLightResourcesV92::add_active(target_providers::Handle16 target,std::shared_ptr<RetainedLightSceneNodeV92> node,std::string& e){
 if(!owning_thread()||!node||active11c_.size()>=4){e="Original LightSet active-light storage/assertion boundary";return false;}
 active11c_.push_back({target,node});e.clear();return true;
}
void SceneLightResourcesV92::source_reset_lightset_at_scene_clear()noexcept{
 for(auto& row:lights78_)for(auto& light:row)light.reset();for(auto& row:dirty64_)row.fill(0);active11c_.clear();staticdc_.clear();++generation_;
 // Names fields remain the SAME existing owner; original C1-assignment names
 // are unchanged. Automatic424 remains a distinct SceneManager collection.
}
RetainedLightActorReferenceV92::RetainedLightActorReferenceV92(std::shared_ptr<RetainedLightSceneNodeV92> node,std::shared_ptr<SceneLightResourcesV92> scene,std::shared_ptr<void> runtime,LightRuntimeStampV67 current,std::uintptr_t actor):actor_pin_(std::move(node)),scene_(scene){
 runtime_owner=std::move(runtime);stamp=current;actor_identity=actor;node_identity=reinterpret_cast<std::uintptr_t>(actor_pin_.get());light_identity=reinterpret_cast<std::uintptr_t>(actor_pin_->light.get());actor_pin_->actor_reference_live=true;
}
bool RetainedLightActorReferenceV92::validate_exact_actor_reference(const LightQuiescenceLeaseV67& lease)const noexcept{
 auto scene=scene_.lock();return !consumed_&&actor_pin_&&actor_pin_->actor_reference_live&&scene&&scene->owning_thread()&&scene->generation()==stamp.generation&&lease.stamp==stamp&&lease.validate_current_owning_thread()&&runtime_owner&&lease.runtime_owner&&runtime_owner.get()==lease.runtime_owner.get()&&!runtime_owner.owner_before(lease.runtime_owner)&&!lease.runtime_owner.owner_before(runtime_owner);
}
void RetainedLightActorReferenceV92::drop_actual_actor_node_reference()noexcept{
 if(consumed_)return;consumed_=true;if(actor_pin_)actor_pin_->actor_reference_live=false;actor_pin_.reset();
}
bool borrow_retained_light_parameters_v92(const LightNodeBorrowV53& source,LightParameterFieldsV53& out,std::string& e){
 auto node=source.source_node_v92;if(!node||!node->light||source.identity!=reinterpret_cast<std::uintptr_t>(node.get())||source.owner.get()!=node.get()||source.actual_light_identity!=reinterpret_cast<std::uintptr_t>(node->light.get())){e="Foreign retained CLight/actual LightNode source owner";return false;}
 auto light=node->light;LightParameterFieldsV53 fields;fields.owner=light;fields.actual_light_identity=reinterpret_cast<std::uintptr_t>(light.get());fields.ambient4=&light->ambient4;fields.diffuse14=&light->diffuse14;fields.specular24=&light->specular24;fields.attenuation34=&light->attenuation34;fields.radius40=&light->radius40;
 fields.validate_current=[weak=std::weak_ptr<RetainedLightSceneNodeV92>(node),expected=std::weak_ptr<RetainedLightFieldsV92>(light)](std::string& e){auto n=weak.lock();auto l=expected.lock();if(!n||!l||n->light!=l||!n->actor_reference_live){e="Actual CLight actor reference retired";return false;}e.clear();return true;};out=std::move(fields);e.clear();return true;
}
bool attach_retained_light_root_v92(GameObjectSceneRootRegistryV1& roots,const LightNodeBorrowV53& source,std::string& e){
 auto node=source.source_node_v92;if(!node||source.identity!=reinterpret_cast<std::uintptr_t>(node.get())||source.owner.get()!=node.get()||!node->actor_reference_live){e="Required SAME live light root child";return false;}
 if(auto parent=node->parent.lock()){auto i=std::find(parent->children.begin(),parent->children.end(),node);if(i==parent->children.end()){e="Light child lost actual previous parent membership";return false;}parent->children.erase(i);node->parent.reset();}
 GameObjectSceneRootBorrowV1 root;root.owner=node;root.identity=source.identity;root.flags11c=&node->fields->flags;root.parentec=&node->parentec;
 const std::weak_ptr<RetainedLightSceneNodeV92> weak=node;
 root.notify_visibility=[weak](bool parent,std::string& e){auto n=weak.lock();if(!n){e="Actual light child retired";return false;}n->fields->visibility.parent121=parent?1:0;if(parent&&n->fields->visibility.local120)n->fields->flags|=1u;else n->fields->flags&=~1u;e.clear();return true;};
 root.remove_animators=[weak](std::string& e){if(!weak.lock()){e="Actual static light child retired";return false;}e.clear();return true;}; // C1's actual empty animator list.
 root.scene_phase_v69=[weak](std::uint32_t time,std::string& e){auto n=weak.lock();return n&&n->update_static_node(time,e);};
 if(!roots.add_child(std::move(root),e))return false;node->root_reference_live=true;if(source.load_prefix_v92)source.load_prefix_v92->root.reset();e.clear();return true;
}
const std::shared_ptr<SceneLightResourcesV92>& GameObjectSceneRootRegistryV1::source_light_resources_v92(){
 if(!source_light_resources_v92_)source_light_resources_v92_=std::make_shared<SceneLightResourcesV92>(source_light_names_v89());return source_light_resources_v92_;
}
}
