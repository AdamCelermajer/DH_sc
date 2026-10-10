// Versioned particle resource extension. Frozen V1 source remains unchanged.
#include "character_mesh_fx_owner_v4.hpp"
#include "visual_fx_manager_libraries_v63.hpp"
#include "fx_texture_animation_v1.hpp"
#include "visual_timeline.hpp"
#include "level_up_placeholder_column_v1.hpp" // P16 LEVELUP4 placeholder (see header)
#include "source_fx_node_matrix_v4.hpp"
#include "source_fx_segment_v87.hpp"
#include "visual_motion.hpp"
#include "../engine-animation/animation.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <map>
#include <stdexcept>
namespace dh2::fx {
namespace {
std::uint32_t word(const std::uint8_t* p){std::uint32_t x;std::memcpy(&x,p,4);return x;}
float add(float a,float b){volatile float x=a+b;return x;}
bool finite(const float* v,std::size_t n){for(std::size_t i=0;i<n;++i)if(!std::isfinite(v[i]))return false;return true;}
std::array<float,16> matrix(const float* p,const float* rotation,const float* scale){std::array<float,16> m{};float q[4]{};dh2_visual_rotation(q,rotation);dh2_node_matrix(m.data(),p,q,scale);return m;}
skinning::VisualGeometryV6 mesh(const resources::BresView& v,unsigned i){
 assets::Mesh m{};if(dh2_mesh_open(&m,&v,i)!=assets::Error::ok||m.vertices>1000000||m.attributes>256||m.primitives>4096)throw std::runtime_error("FX mesh decode/capacity");
 skinning::VisualGeometryV6 g;g.id=m.id;std::copy_n(m.minimum,3,g.minimum);std::copy_n(m.maximum,3,g.maximum);
 for(unsigned a=0;a<m.attributes;++a){assets::Attribute v{};if(dh2_mesh_attribute(&m,a,&v)!=assets::Error::ok||!v.components||v.components>4)throw std::runtime_error("FX vertex attribute");skinning::VisualAttributeV6 t;t.type=v.type;t.components=v.components;t.values.resize(std::size_t(m.vertices)*v.components);for(unsigned j=0;j<m.vertices;++j)if(!dh2_attribute_read(&v,j,t.values.data()+std::size_t(j)*v.components))throw std::runtime_error("FX vertex read");if(!finite(t.values.data(),t.values.size()))throw std::runtime_error("FX nonfinite geometry");g.attributes.push_back(std::move(t));}
 std::int32_t positions=-1;
 for(unsigned p=0;p<m.primitives;++p){assets::Primitive v{};if(dh2_mesh_primitive(&m,p,&v)!=assets::Error::ok)throw std::runtime_error("FX primitive");skinning::VisualPrimitiveV6 t;t.material_symbol=v.material?v.material:"";t.collada_type=v.collada_type;t.engine_type=v.engine_type;std::copy_n(v.attributes,18,t.attributes.begin());if(positions==-1)positions=v.attributes[0];else if(positions!=v.attributes[0])throw std::runtime_error("FX primitive position streams");t.indices.resize(v.index_count);for(unsigned j=0;j<v.index_count;++j)if(!dh2_index_read(&v,j,&t.indices[j])||t.indices[j]>=m.vertices)throw std::runtime_error("FX index");g.primitives.push_back(std::move(t));}
 if(positions<0||std::size_t(positions)>=g.attributes.size()||g.attributes[positions].components<3)throw std::runtime_error("FX no position stream");const auto& a=g.attributes[positions];g.positions.resize(m.vertices);for(unsigned j=0;j<m.vertices;++j)std::copy_n(a.values.data()+std::size_t(j)*a.components,3,g.positions[j].begin());return g;
}
struct Resource {
 //Actual quest marker source reference204. C1 provenance is intentionally
 //unknown until the original Install/Remove producer stores this field.
 std::optional<std::uintptr_t> source_owner204;
 struct Texture {assets::Animation accessor;std::uint32_t material,segment;};
 std::vector<std::uint8_t> bytes;scene::Scene scene;animation::Player player;resources::BresView image{};
 std::vector<skinning::VisualGeometryV6> geometry;std::vector<Texture> textures;
 std::shared_ptr<CharacterParticleFxResourceV2> particle;
 bool clip(std::uint32_t index,std::int32_t& start,std::int32_t& end,std::string& e)const{
  const auto* row=dh2_bres_library_item(&image,resources::Library::animation_clip,static_cast<std::int32_t>(index));
  if(!row){e="Required authored FX animation clip record";return false;}
  std::memcpy(&start,row+4,4);std::memcpy(&end,row+8,4);
  if(end<start){e="Invalid authored FX clip bounds";return false;}return true;
 }
 bool load(MeshFxAssetsV1 assets,const std::string& uri,CharacterParticleFxFactoryV2 factory,const scene::Scene& live,std::string& error){
  if(!assets.read||!assets.read(assets.context,uri.c_str(),bytes,error))return false;
  try{
   if(dh2_bres_open(&image,bytes.data(),bytes.size())!=resources::BresError::ok)throw std::runtime_error("FX invalid BRES");
   // The configured composite factory owns both mesh-only and mixed resources.
   // Zero emitters does not imply absent source scene/material/node metadata.
   if(factory.create){if(!factory.create(factory.context,std::make_shared<const std::vector<std::uint8_t>>(bytes),live,particle,error)||!particle)throw std::runtime_error("FX resource "+uri+": "+(error.empty()?"Authored FX constructor failed":error));return true;}
   if(word(bytes.data()+image.root_offset+0x78))throw std::runtime_error("Required actual particle FX resource factory");
   for(auto offset:{0x80u,0x88u,0x90u})if(word(bytes.data()+image.root_offset+offset))throw std::runtime_error("Required FX resource family continuation unavailable");
   for(unsigned i=0;i<unsigned(resources::Library::count);++i){auto l=resources::Library(i);if(word(bytes.data()+image.root_offset+resources::libraries[i].count_offset)!=dh2_bres_library_count(&image,l))throw std::runtime_error("FX invalid declared library range");}
   for(auto l:{resources::Library::controller,resources::Library::camera,resources::Library::light})if(dh2_bres_library_count(&image,l))throw std::runtime_error("Required FX controller/light/camera constructor unavailable");
   auto span=[&](std::uint64_t at,std::uint64_t n){if(at>bytes.size()||n>bytes.size()-at)throw std::runtime_error("FX node range");return bytes.data()+at;};
   auto w=[&](std::uint32_t at){return word(span(at,4));};
   // Validate every declared instance before reduced Scene decoding: tag3 mesh,
   // tag4 animation library. No other ignored instance may silently disappear.
   std::vector<unsigned> path;
   std::function<void(unsigned)> visit=[&](unsigned p){span(p,80);if(path.size()>64||std::find(path.begin(),path.end(),p)!=path.end())throw std::runtime_error("FX node cycle");path.push_back(p);auto n=w(p+64),a=w(p+68);if(n>4096)throw std::runtime_error("FX instance count");span(a,std::uint64_t(n)*8);for(unsigned i=0;i<n;++i){auto tag=w(a+8*i);if(tag!=3&&tag!=4)throw std::runtime_error("Required FX nonmesh instance constructor unavailable");}n=w(p+56);a=w(p+60);if(n>4096)throw std::runtime_error("FX child count");span(a,std::uint64_t(n)*80);for(unsigned i=0;i<n;++i)visit(a+80*i);path.pop_back();};
   auto n=w(image.root_offset+152),a=w(image.root_offset+156);if(n!=1)throw std::runtime_error("FX requires one authored visual scene");span(a,16);n=w(a+8);a=w(a+12);span(a,std::uint64_t(n)*80);for(unsigned i=0;i<n;++i)visit(a+80*i);
   if(!scene::load(image,scene,error))throw std::runtime_error(error);
   if(scene.instances.empty())throw std::runtime_error("FX has no mesh draw resource");
   for(const auto& i:scene.instances){if(i.controller!=-1)throw std::runtime_error("Required FX skin continuation unavailable");geometry.push_back(mesh(image,i.geometry));}

   const auto segments=dh2_animation_segments(&image);if(dh2_bres_library_count(&image,resources::Library::animation)&&(!segments||segments>256))throw std::runtime_error("FX authored segment domain");
   for(unsigned segment=0;segment<segments;++segment)for(unsigned i=0;i<dh2_bres_library_count(&image,resources::Library::animation);++i){assets::Animation a{};if(dh2_animation_open(&a,&image,i,segment)!=assets::Error::ok)throw std::runtime_error("FX animation accessor");auto type=dh2_animation_type(&a,0);
    if(type==1||type==5||type==10)continue;
    if(type==26)continue; // actual shipping getAnimationTrackEx611ae0 null branch
    if(type<87||type>91)throw std::runtime_error("Required FX animation interpreter unavailable");
    auto* target=dh2_animation_target(&a);auto* channel=dh2_animation_channel(&a,0);auto p=word(channel+12);auto* name=reinterpret_cast<const char*>(span(p,1));auto* end=std::memchr(name,0,bytes.size()-p);if(!end||std::strcmp(name,"diffuse-sampler-matrix"))throw std::runtime_error("Required FX material parameter continuation unavailable");
    auto m=std::find_if(scene.materials.begin(),scene.materials.end(),[&](const scene::Material& m){return target&&m.id==target;});if(m==scene.materials.end())throw std::runtime_error("FX material animation target");textures.push_back({a,unsigned(m-scene.materials.begin()),segment});
   }
   if(!player.load(bytes.data(),bytes.size(),scene,error,animation::MissingTargets::reject))throw std::runtime_error(error);
   if(player.unbound)throw std::runtime_error("FX unbound node animation");return true;
  }catch(const std::exception& e){error=e.what();return false;}
 }
 std::int32_t start_ms()const{const auto* clip0=dh2_bres_library_item(&image,resources::Library::animation_clip,0);if(clip0){std::int32_t value;std::memcpy(&value,clip0+4,4);return value;}return particle?particle->start_ms():player.start;}
 std::int32_t end_ms()const{const auto* clip0=dh2_bres_library_item(&image,resources::Library::animation_clip,0);if(clip0){std::int32_t value;std::memcpy(&value,clip0+8,4);return value;}return particle?particle->end_ms():player.end;}
 bool sample(std::int32_t ms,std::string& error){if(particle)return particle->sample_animation(ms,error);if(!player.sample(scene,ms,error))return false;std::uint32_t segment{};if(!textures.empty()&&!source_fx_segment_v87(image,ms,segment,error))return false;for(const auto& t:textures){if(t.segment!=segment)continue;TextureTransform20V1 v;if(!texture_sample_v1(v,t.accessor,ms,true,error))return false;math::Matrix4f m{};if(dh2_fx_texture_matrix_v1(&m,&v)){error="FX material matrix";return false;}std::copy_n(m.m,16,scene.materials[t.material].texture_matrix);}return true;}
};
}
struct CharacterMeshFxOwnerV4::Impl {
 struct Set {std::int32_t id{-1};bool finished{};std::uint32_t step{};std::int32_t loops{};
  float position[3]{},rotation[3]{};bool explicit_rotation{};std::uintptr_t anchor{};std::weak_ptr<Set> parent;};
 struct Runtime {FxState96V1 state;std::string uri;std::shared_ptr<Resource> resource;timeline::State timeline{};float visual_position[3]{},visual_rotation[3]{},visual_scale[3]{1,1,1};bool pooled{},pending{};};
 data::EffectsTables::Borrow tables;const scene::Scene* live;MeshFxAssetsV1 assets;MeshFxServicesV1 services;CharacterParticleFxFactoryV2 factory;
 std::shared_ptr<VisualFxManagerLibrariesV63> source_libraries;
 std::function<bool(std::uint32_t,std::uint32_t&,std::string&)> set_random_v118;
 std::size_t set_dispatch_depth_v118{},set_dispatch_count_v118{};
 std::shared_ptr<Set> set_data_v118(std::uintptr_t);
 bool play_set_v118(std::int32_t,const float*,const float*,std::uintptr_t,const std::shared_ptr<Set>&,std::uintptr_t*);
 bool play_step_v118(const std::shared_ptr<Set>&,std::uintptr_t*,bool initial=false);
 bool play_file_v118(std::int32_t,const std::shared_ptr<Set>&,std::uintptr_t*,bool initial=false);
 bool sequence_v118(const std::shared_ptr<Set>&,bool);
 bool loop_callback_v118(const std::shared_ptr<Set>&);
 std::map<std::uintptr_t,std::shared_ptr<Runtime>> records;std::map<std::int32_t,std::vector<std::uintptr_t>> active,free;std::vector<std::uintptr_t> pending;std::vector<std::shared_ptr<Set>> sets;std::size_t cold{},warm{};std::int32_t dt{},scene_dt{};bool precache{};std::string failure;
 Impl(data::EffectsTables::Borrow t,const scene::Scene& l,MeshFxAssetsV1 a,MeshFxServicesV1 s,CharacterParticleFxFactoryV2 f):tables(std::move(t)),live(&l),assets(a),services(s),factory(f){}
 bool query(MeshFxOperationV1 op,std::uintptr_t id,const char* text,std::uint32_t& result,float* point=nullptr){if(!services.invoke){failure="Required FX source debug/anchor/floor service unavailable";return false;}MeshFxRequestV1 q{op,id,text,0,{0,0,0},live};if(point)std::copy_n(point,3,q.point);if(!services.invoke(services.context,q,failure))return false;result=q.result;if(point)std::copy_n(q.point,3,point);return true;}
 bool publish_active_v88(const std::shared_ptr<Runtime>& r){
  if(!source_libraries)return true;auto* info=source_libraries->animated_info_v88(r->state.file);
  if(!info){failure="Required SAME actual AnimatedFxInfo before instance publication";return false;}
  const auto old=std::find_if(info->free4.begin(),info->free4.end(),[&](const auto& p){return p.get()==r.get();});
  if(old!=info->free4.end())info->free4.erase(old);
  info->active10.push_back(r);return true;
 }
 bool publish_free_v88(const std::shared_ptr<Runtime>& r){
  if(!source_libraries)return true;auto* info=source_libraries->animated_info_v88(r->state.file);
  if(!info){failure="Required SAME actual AnimatedFxInfo at instance Drop";return false;}
  const auto old=std::find_if(info->active10.begin(),info->active10.end(),[&](const auto& p){return p.get()==r.get();});
  if(old==info->active10.end()){failure="FX Drop lost its actual active source membership";return false;}
  info->active10.erase(old);info->free4.push_back(r);return true;
 }
 bool publish_set_v88(const std::shared_ptr<Set>& data){
  if(!source_libraries)return true;auto* info=source_libraries->set_info_v88(data->id);
  if(!info){if(!source_libraries->precache_byte4())return true;failure="Required SAME actual AnimFxSetInfo for SetData publication";return false;} //unpublished native C1 prefix before the source module guard
  info->active10.push_back(data);return true;
 }
 bool module(){std::uint32_t enabled;if(!query(MeshFxOperationV1::debug_load,0,nullptr,enabled)||!query(MeshFxOperationV1::module_enabled,0,"AnimatedFX",enabled))return false;return enabled!=0;}
 bool sync(Runtime& r,bool reset){auto& s=r.state;std::copy_n(s.position,3,r.visual_position);bool orient=s.orient_once||(reset&&s.orient_with_anchor);std::uint32_t result;
  if(s.set_identity&&reset&&s.orient_with_anchor){auto data=set_data_v118(s.set_identity);if(!data){failure="Expired SAME FX SetData during Sync";return false;}std::size_t depth=0;while(auto parent=data->parent.lock()){if(++depth>4096){failure="Cyclic source FX parent chain";return false;}data=std::move(parent);}if(data->step>0)orient=false;}
  if(s.anchor){float p[3]{};if(!query(MeshFxOperationV1::anchor_position,s.anchor,nullptr,result,p))return false;for(unsigned i=0;i<3;++i)r.visual_position[i]=add(s.position[i],p[i]);if(orient){float rotation[3]{};if(!query(MeshFxOperationV1::anchor_rotation,s.anchor,nullptr,result,rotation))return false;std::copy_n(rotation,3,s.rotation);std::copy_n(rotation,3,r.visual_rotation);}if(s.scale_with_anchor){float scale[3]{};if(!query(MeshFxOperationV1::anchor_scale,s.anchor,nullptr,result,scale))return false;std::copy_n(scale,3,r.visual_scale);}}
  if(s.fixed_rotation)std::copy_n(s.rotation,3,r.visual_rotation);
  if(!s.fixed_rotation&&!(s.anchor&&(orient||s.orient_with_anchor))){float point[3];std::copy_n(r.visual_position,3,point);if(!query(MeshFxOperationV1::floor_normal,s.anchor,nullptr,result,point))return false;}
  return true;
 }
 std::shared_ptr<Runtime> find(FxState96V1* state){for(const auto& x:records)if(&x.second->state==state)return x.second;return {};}
 static int invoke(void* context,FxState96V1* state,FxRequest32V1* q){auto& s=*static_cast<Impl*>(context);auto r=s.find(state);if(!r){s.failure="FX state identity expired";return -1;}std::uint32_t result=0;switch(q->operation){
  case FxOperationV1::sync:return s.sync(*r,q->argument!=0)?0:-1;
  case FxOperationV1::speed:return dh2_timeline_scale(&r->timeline,q->scalar);
  case FxOperationV1::loop:return dh2_timeline_loop(&r->timeline,q->argument);
  case FxOperationV1::start:return dh2_timeline_jump(&r->timeline,r->timeline.start_ms);
  case FxOperationV1::visible:return 0; // state written first; root visibility is draw gate
  case FxOperationV1::end_sample:if(r->timeline.current_ms!=r->timeline.end_ms&&dh2_timeline_jump(&r->timeline,r->timeline.end_ms))return -1;return r->resource->sample(r->timeline.end_ms,s.failure)?0:-1;
  case FxOperationV1::end_callback:if(q->argument){auto data=s.set_data_v118(q->identity);if(!data){s.failure="FX set-data identity expired";return -1;}data->finished=true;}else{if(q->payload){auto data=s.set_data_v118(q->payload);if(!data){s.failure="FX callback set-data expired";return -1;}if(!s.loop_callback_v118(data))return -1;}r->pending=true;s.pending.push_back(reinterpret_cast<std::uintptr_t>(r.get()));if(s.source_libraries)s.source_libraries->finished8_v88().push_back(r);}return 0;
  case FxOperationV1::app_dt:q->argument=std::uint32_t(s.dt);return 0;
  case FxOperationV1::completed:{if(r->resource->particle){bool complete=false;if(!r->resource->particle->completed(complete,s.failure))return -1;q->argument=complete;}else q->argument=1;return 0;}
  case FxOperationV1::debug_load:return s.query(MeshFxOperationV1::debug_load,0,nullptr,result)?0:-1;
  case FxOperationV1::debug_set:return s.query(MeshFxOperationV1::set_switch,state->set_identity,"isTracingAnim_FX",result)?0:-1;
  case FxOperationV1::debug_instance:return s.query(MeshFxOperationV1::instance_switch,reinterpret_cast<std::uintptr_t>(r.get()),"isTracingAnim_FX",result)?0:-1;
  case FxOperationV1::anchor_dead:case FxOperationV1::anchor_disabled:case FxOperationV1::anchor_stationary:{auto op=q->operation==FxOperationV1::anchor_dead?MeshFxOperationV1::anchor_dead:q->operation==FxOperationV1::anchor_disabled?MeshFxOperationV1::anchor_disabled:MeshFxOperationV1::anchor_stationary;if(!s.query(op,q->identity,nullptr,result))return -1;q->argument=result;return 0;}
  default:s.failure="Required FX state continuation unavailable";return -1;
 }}
 FxServices16V1 state_services(){return {this,invoke};}
 bool sample(Runtime& r,std::int32_t absolute){auto v=state_services();bool good=true;struct C{Impl* self;Runtime* r;FxServices16V1* v;bool* good;};C c{this,&r,&v,&good};timeline::Services callbacks{&c,[](void* p,timeline::State*){auto& c=*static_cast<C*>(p);if(dh2_fx_handle_loop_end_v1(&c.r->state,c.v))*c.good=false;}};if(dh2_timeline_update(&r.timeline,absolute,&callbacks)||!good)return false;if(!r.resource->sample(r.timeline.current_ms,failure))return false;if(!r.resource->particle)return true;if(auto composite=std::dynamic_pointer_cast<CharacterAuthoredCompositeFxResourceV4>(r.resource->particle)){math::Matrix4f outer{};source_fx_trs_matrix_v4(outer,r.visual_position,r.visual_rotation,r.visual_scale);return composite->source_scene_frame_v4(absolute,scene_dt,outer,failure);}return r.resource->particle->scene_frame(absolute,scene_dt,matrix(r.visual_position,r.visual_rotation,r.visual_scale),failure);}
};
#include "character_mesh_fx_sets_v118.inc"
CharacterMeshFxOwnerV4::CharacterMeshFxOwnerV4(data::EffectsTables::Borrow t,const scene::Scene& l,MeshFxAssetsV1 a,MeshFxServicesV1 s,CharacterParticleFxFactoryV2 f):impl_(std::make_unique<Impl>(std::move(t),l,a,s,f)){}
CharacterMeshFxOwnerV4::CharacterMeshFxOwnerV4(std::shared_ptr<VisualFxManagerLibrariesV63> owner,const scene::Scene& l,MeshFxAssetsV1 a,MeshFxServicesV1 s,CharacterParticleFxFactoryV2 f):
 impl_(std::make_unique<Impl>(owner?owner->source_tables():data::EffectsTables::Borrow{},l,a,s,f)){
 if(!owner)throw std::invalid_argument("Required same App source FX libraries");
 impl_->source_libraries=std::move(owner);
}
CharacterMeshFxOwnerV4::~CharacterMeshFxOwnerV4()=default;
bool CharacterMeshFxOwnerV4::precache_libraries(std::string& error){error.clear();auto& s=*impl_;s.failure.clear();if(s.source_libraries)return s.source_libraries->precache_libraries(error);if(!s.module()){error=s.failure;return error.empty();}s.precache=true;return true;}
bool CharacterMeshFxOwnerV4::precached()const noexcept{return impl_->source_libraries?impl_->source_libraries->precache_byte4()!=0:impl_->precache;}
const std::shared_ptr<VisualFxManagerLibrariesV63>& CharacterMeshFxOwnerV4::source_libraries_v63()const noexcept{return impl_->source_libraries;}
bool CharacterMeshFxOwnerV4::play_set(std::int32_t set,const float p[3],const float* rotation,std::uintptr_t anchor,std::uintptr_t* out,std::string& error){
 error.clear();if(out)*out=0;auto& s=*impl_;s.failure.clear();
 if(!s.tables||!p||!finite(p,3)||(rotation&&!finite(rotation,3))){error="FX native input contract";return false;}
 const bool result=s.play_set_v118(set,p,rotation,anchor,{},out);
 if(!result)error=s.failure.empty()?"FX authored set source delivery failed":s.failure;
 return result;
}
bool CharacterMeshFxOwnerV4::flush_libraries_v88(std::string& e){
 auto& s=*impl_;if(!s.source_libraries){e="Campaign FX Flush requires SAME App library owner";return false;}
 if(s.set_dispatch_depth_v118){e="Cannot delete source FX libraries during authored set/callback delivery";return false;}
 VisualFxLibraryReleaseV88 release;release.owner=s.source_libraries;
 release.drop_finished=[this,&s](std::shared_ptr<void>& value,std::string& e){
  const auto id=reinterpret_cast<std::uintptr_t>(value.get());auto at=s.records.find(id);
  if(at==s.records.end()){e="Finished source FX has no SAME native instance";return false;}
  auto r=at->second;auto identity=id;if(!r->pooled&&!drop(identity,e))return false;value.reset();return true;
 };
 release.animated_d0=[&s](std::shared_ptr<void>& value,std::string& e){
  const auto id=reinterpret_cast<std::uintptr_t>(value.get());auto at=s.records.find(id);
  if(at==s.records.end()){e="Source FX D0 addressed an unknown native pool instance";return false;}
  auto r=at->second;r->resource.reset();r->state.visual=0;
  for(auto* pools:{&s.active,&s.free})for(auto& pool:*pools)pool.second.erase(std::remove(pool.second.begin(),pool.second.end(),id),pool.second.end());
  s.pending.erase(std::remove(s.pending.begin(),s.pending.end(),id),s.pending.end());s.records.erase(at);value.reset();e.clear();return true;
 };
 release.set_data_d1=[&s](std::shared_ptr<void>& value,std::string& e){
  const auto found=std::find_if(s.sets.begin(),s.sets.end(),[&](const auto& data){return data.get()==value.get();});
  if(found==s.sets.end()){e="Source SetData D1 addressed a different retained owner";return false;}
  s.sets.erase(found);value.reset();e.clear();return true;
 };
 if(!s.source_libraries->flush_libraries_v88(release,e))return false;
 //Private port construction failures can retain allocated-but-unpublished
 //prefixes. Native resources are released only after the source pool walk.
 s.records.clear();s.active.clear();s.free.clear();s.pending.clear();s.sets.clear();s.failure.clear();e.clear();return true;
}
bool CharacterMeshFxOwnerV4::animation_event(const char* event,const float p[3],std::string& error){error.clear();if(!event||!p){error="FX event input";return false;}if(std::strncmp(event,"fx_",3)){error="Event is outside source fx_ branch";return false;}auto& s=*impl_;if(!s.tables){error="FX tables unavailable";return false;}const auto& names=s.tables.set_names();for(unsigned i=0;i<names.size();++i)if(names[i]==event+3)return play_set(i,p,nullptr,0,nullptr,error);return true;}
bool CharacterMeshFxOwnerV4::drop(std::uintptr_t& identity,std::string& error){error.clear();if(!identity)return true;auto& s=*impl_;s.failure.clear();if(!precached()){identity=0;return true;}auto at=s.records.find(identity);if(at==s.records.end()||at->second->pooled){error="FX invalid active identity";return false;}auto r=at->second;auto& active=s.active[r->state.file];active.erase(std::remove(active.begin(),active.end(),identity),active.end());auto& free=s.free[r->state.file];if(std::find(free.begin(),free.end(),identity)==free.end())free.push_back(identity);r->pooled=true;if(!s.publish_free_v88(r)){error=s.failure;return false;}auto v=s.state_services();if(dh2_fx_drop_reset_v1(&r->state,&v)){error=s.failure.empty()?"FX drop source service failure":s.failure;return false;}identity=0;return true;}
void CharacterMeshFxOwnerV4::detach_anchor_v117(std::uintptr_t anchor)noexcept{
 if(!anchor)return;
 for(auto& entry:impl_->records)if(entry.second->state.anchor==anchor)
  entry.second->state.anchor=0;
 //Native lifetime barrier also retires the original SetData40 callback loan.
 //Capture the last actual submitted transform from a same-set/descendant
 //instance; never look up the actor again after its source D1 begins.
 for(auto& data:impl_->sets)if(data->anchor==anchor){
  for(const auto& entry:impl_->records){auto source=impl_->set_data_v118(entry.second->state.set_identity);std::size_t depth=0;
   while(source&&source!=data&&depth++<4096)source=source->parent.lock();
   if(source==data){std::copy_n(entry.second->visual_position,3,data->position);break;}
  }
  data->anchor=0;
 }
}
bool CharacterMeshFxOwnerV4::drop_set_by_id_v117(std::int32_t set,std::string& error){
 error.clear();auto& s=*impl_;s.failure.clear();
 if(!s.tables){error="StopEffect requires actual FX tables";return false;}
 if(set<0||std::size_t(set)>=s.tables.sets().size())return true;
 const auto& row=s.tables.sets()[set];
 if(row.steps.empty()){error="Original StopEffect set has no first authored step";return false;}
 const auto file=row.steps[0].file;
 if(file<0||std::size_t(file)>=s.tables.dictionary().values.size()){
  error="StopEffect file declaration invalid";return false;
 }
 auto& active=s.active[file];if(active.empty())return true;
 const auto identity=active.front();auto found=s.records.find(identity);
 if(found==s.records.end()){error="StopEffect lost actual first active AnimatedFX";return false;}
 auto instance=found->second;
 // Shipping494660 pushes the first active receiver into free4, clears this
 // file's entire active10 list, then resets that first receiver. It neither
 // filters by a script-owned handle nor invokes the animation end callback.
 auto& free=s.free[file];
 if(std::find(free.begin(),free.end(),identity)==free.end())free.push_back(identity);
 if(s.source_libraries){
  auto* pool=s.source_libraries->animated_info_v88(file);
  if(!pool){error="StopEffect requires SAME AnimatedFxInfo";return false;}
  if(std::none_of(pool->free4.begin(),pool->free4.end(),[&](const auto& value){return value.get()==instance.get();}))
   pool->free4.push_back(instance);
  pool->active10.clear();
 }
 active.clear();instance->pooled=true;
 std::fill_n(instance->state.position,3,0.f);
 if(!s.sync(*instance,false)){error=s.failure;return false;}
 instance->state.anchor=0;
 if(!s.sync(*instance,true)){error=s.failure;return false;}
 if(instance->state.visible){
  instance->state.visible=0;
  if(!s.sync(*instance,false)){error=s.failure;return false;}
 }
 return true;
}
bool CharacterMeshFxOwnerV4::scene_frame(std::int32_t ms,std::int32_t app_dt,std::string& error){error.clear();auto& s=*impl_;s.failure.clear();s.scene_dt=app_dt;for(auto& pool:s.active){std::size_t i=0,budget=0;while(i<pool.second.size()){if(++budget>65536){error="FX scene reentry limit";return false;}auto identity=pool.second[i];auto r=s.records.at(identity);if(!s.sample(*r,ms)){error=s.failure.empty()?"FX scene timeline/sample rejected":s.failure;return false;}if(i<pool.second.size()&&pool.second[i]==identity)++i;}}return true;}
bool CharacterMeshFxOwnerV4::manager_frame(std::int32_t dt,std::string& error){error.clear();auto& s=*impl_;s.failure.clear();if(!s.module()){error=s.failure;return error.empty();}s.dt=dt;auto v=s.state_services();for(auto& pool:s.active){std::size_t i=0,budget=0;while(i<pool.second.size()){if(++budget>65536){error="FX manager reentry limit";return false;}auto identity=pool.second[i];auto r=s.records.at(identity);if(dh2_fx_update_v1(&r->state,&v)){error=s.failure.empty()?"FX update service failure":s.failure;return false;}if(i<pool.second.size()&&pool.second[i]==identity)++i;}}
 for(std::size_t i=0;i<s.pending.size();){auto id=s.pending[i];auto r=s.records.at(id);FxRequest32V1 q{FxOperationV1::end_sample};if(Impl::invoke(&s,&r->state,&q)){error=s.failure;return false;}if(r->state.visible){++i;continue;}s.pending.erase(s.pending.begin()+i);if(s.source_libraries){auto& finished=s.source_libraries->finished8_v88();auto at=std::find_if(finished.begin(),finished.end(),[&](const auto& p){return p.get()==r.get();});if(at!=finished.end())finished.erase(at);}r->pending=false;if(!r->pooled&&!drop(id,error))return false;}
 return true;}
bool CharacterMeshFxOwnerV4::particle_draw_sources_v3(std::vector<CharacterParticleDrawSourceV3>& out,std::string& error)const{
 error.clear();std::vector<CharacterParticleDrawSourceV3> result;const auto& s=*impl_;
 for(const auto& pool:s.active){for(auto id:pool.second){const auto& r=*s.records.at(id);if(!r.state.visible)continue;
  if(!r.resource->particle){error="Required non-particle source render metadata";return false;}
  std::vector<CharacterParticleDrawSourceV3> parts;if(!r.resource->particle->draw_sources_v3(parts,error))return false;
  for(auto& part:parts){part.fx_identity=id;part.source_owner204=r.resource->source_owner204;result.push_back(std::move(part));}
 }}
 out=std::move(result);return true;
}
bool CharacterMeshFxOwnerV4::draw_parts(std::vector<skinning::VisualDrawPartV6>& out,std::string& error)const{error.clear();auto& s=*impl_;struct Snapshot{std::shared_ptr<Resource> resource;std::vector<scene::Material> materials;std::vector<std::vector<std::uint32_t>> bindings;};std::vector<skinning::VisualDrawPartV6> parts;for(const auto& pool:s.active)for(auto id:pool.second){const auto& r=*s.records.at(id);if(!r.state.visible)continue;if(r.resource->particle){std::vector<skinning::VisualDrawPartV6> particle_parts;if(!r.resource->particle->draw_parts(particle_parts,error))return false;/*P16 LEVELUP4 PLACEHOLDER: level_up white column window + gold sheet suppression*/apply_level_up_placeholder_v1(r.uri,r.timeline.current_ms-r.timeline.start_ms,particle_parts);for(auto& part:particle_parts)parts.push_back(std::move(part));continue;}auto retained=std::make_shared<Snapshot>();retained->resource=r.resource;retained->materials=r.resource->scene.materials;for(const auto& instance:r.resource->scene.instances)retained->bindings.push_back(instance.materials);auto outer=matrix(r.visual_position,r.visual_rotation,r.visual_scale);if(!finite(outer.data(),16)){error="FX outer matrix nonfinite";return false;}for(unsigned i=0;i<r.resource->scene.instances.size();++i){const auto& instance=r.resource->scene.instances[i];skinning::VisualDrawPartV6 d;d.retention=retained;d.geometry=&r.resource->geometry[i];d.material_table=&retained->materials;d.materials=&retained->bindings[i];d.positions=d.geometry->positions;d.world=scene::multiply(outer,instance.world);parts.push_back(std::move(d));}}out=std::move(parts);return true;}
bool CharacterMeshFxOwnerV4::mesh_draw_sources_v4(std::vector<CharacterFxMeshDrawSourceV4>& out,std::string& error)const{
 error.clear();std::vector<CharacterFxMeshDrawSourceV4> result;
 for(const auto& pool:impl_->active)for(auto id:pool.second){const auto& r=*impl_->records.at(id);if(!r.state.visible)continue;
  if(!r.resource->particle){error="Required pure-mesh source receiver metadata continuation";return false;}
  if(auto composite=std::dynamic_pointer_cast<CharacterAuthoredCompositeFxResourceV4>(r.resource->particle)){
   math::Matrix4f outer{};source_fx_trs_matrix_v4(outer,r.visual_position,r.visual_rotation,r.visual_scale);
   std::vector<CharacterFxMeshDrawSourceV4> parts;if(!composite->mesh_draw_sources_at_outer_v49(outer,parts,error))return false;
   for(auto& part:parts){/*P16 LEVELUP4 PLACEHOLDER: same window/suppression as draw_parts*/if(!level_up_placeholder_keep_part_v1(r.uri,r.timeline.current_ms-r.timeline.start_ms,part.part))continue;part.fx_identity=id;part.source_owner204=r.resource->source_owner204;result.push_back(std::move(part));}
  }
 }
 out=std::move(result);return true;
}
std::vector<MeshFxViewV1> CharacterMeshFxOwnerV4::views()const{std::vector<MeshFxViewV1> out;for(const auto& x:impl_->records){const auto& r=*x.second;std::int32_t set=-1;bool finished=false;if(r.state.set_identity)for(const auto& v:impl_->sets)if(reinterpret_cast<std::uintptr_t>(v.get())==r.state.set_identity){set=v->id;finished=v->finished;break;}out.push_back({x.first,r.state,set,r.timeline.current_ms,r.timeline.start_ms,r.timeline.end_ms,r.pooled,r.pending,finished,r.uri});}return out;}
std::size_t CharacterMeshFxOwnerV4::cold_creations()const noexcept{return impl_->cold;}
std::size_t CharacterMeshFxOwnerV4::warm_reuses()const noexcept{return impl_->warm;}
#include "character_mesh_fx_marker_v28.inc"
#include "character_mesh_fx_quest_v76.inc"
#include "character_mesh_fx_buff_v87.inc"
}
