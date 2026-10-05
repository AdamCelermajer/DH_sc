// Versioned particle resource extension. Frozen V1 source remains unchanged.
#include "character_mesh_fx_owner_v2.hpp"
#include "fx_texture_animation_v1.hpp"
#include "visual_timeline.hpp"
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
std::array<float,16> matrix(const float* p,const float* rotation,const float* scale){math::Quaternion q{};dh2_quat_from_euler(&q,rotation[0],rotation[1],rotation[2]);std::array<float,16> m{};const float v[4]{q.x,q.y,q.z,q.w};dh2_node_matrix(m.data(),p,v,scale);return m;}
skinning::VisualGeometryV6 mesh(const resources::BresView& v,unsigned i){
 assets::Mesh m{};if(dh2_mesh_open(&m,&v,i)!=assets::Error::ok||m.vertices>1000000||m.attributes>256||m.primitives>4096)throw std::runtime_error("FX mesh decode/capacity");
 skinning::VisualGeometryV6 g;g.id=m.id;std::copy_n(m.minimum,3,g.minimum);std::copy_n(m.maximum,3,g.maximum);
 for(unsigned a=0;a<m.attributes;++a){assets::Attribute v{};if(dh2_mesh_attribute(&m,a,&v)!=assets::Error::ok||!v.components||v.components>4)throw std::runtime_error("FX vertex attribute");skinning::VisualAttributeV6 t;t.type=v.type;t.components=v.components;t.values.resize(std::size_t(m.vertices)*v.components);for(unsigned j=0;j<m.vertices;++j)if(!dh2_attribute_read(&v,j,t.values.data()+std::size_t(j)*v.components))throw std::runtime_error("FX vertex read");if(!finite(t.values.data(),t.values.size()))throw std::runtime_error("FX nonfinite geometry");g.attributes.push_back(std::move(t));}
 std::int32_t positions=-1;
 for(unsigned p=0;p<m.primitives;++p){assets::Primitive v{};if(dh2_mesh_primitive(&m,p,&v)!=assets::Error::ok)throw std::runtime_error("FX primitive");skinning::VisualPrimitiveV6 t;t.material_symbol=v.material?v.material:"";t.collada_type=v.collada_type;t.engine_type=v.engine_type;std::copy_n(v.attributes,18,t.attributes.begin());if(positions==-1)positions=v.attributes[0];else if(positions!=v.attributes[0])throw std::runtime_error("FX primitive position streams");t.indices.resize(v.index_count);for(unsigned j=0;j<v.index_count;++j)if(!dh2_index_read(&v,j,&t.indices[j])||t.indices[j]>=m.vertices)throw std::runtime_error("FX index");g.primitives.push_back(std::move(t));}
 if(positions<0||std::size_t(positions)>=g.attributes.size()||g.attributes[positions].components<3)throw std::runtime_error("FX no position stream");const auto& a=g.attributes[positions];g.positions.resize(m.vertices);for(unsigned j=0;j<m.vertices;++j)std::copy_n(a.values.data()+std::size_t(j)*a.components,3,g.positions[j].begin());return g;
}
struct Resource {
 struct Texture {assets::Animation accessor;std::uint32_t material;};
 std::vector<std::uint8_t> bytes;scene::Scene scene;animation::Player player;resources::BresView image{};
 std::vector<skinning::VisualGeometryV6> geometry;std::vector<Texture> textures;
 std::shared_ptr<CharacterParticleFxResourceV2> particle;
 bool load(MeshFxAssetsV1 assets,const std::string& uri,CharacterParticleFxFactoryV2 factory,const scene::Scene& live,std::string& error){
  if(!assets.read||!assets.read(assets.context,uri.c_str(),bytes,error))return false;
  try{
   if(dh2_bres_open(&image,bytes.data(),bytes.size())!=resources::BresError::ok)throw std::runtime_error("FX invalid BRES");
   if(word(bytes.data()+image.root_offset+0x78)){if(!factory.create)throw std::runtime_error("Required actual particle FX resource factory");if(!factory.create(factory.context,std::make_shared<const std::vector<std::uint8_t>>(bytes),live,particle,error)||!particle)throw std::runtime_error(error.empty()?"Particle FX constructor failed":error);return true;}
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
   if(dh2_animation_segments(&image)!=1)throw std::runtime_error("Required FX multi-segment controller unavailable");
   for(unsigned i=0;i<dh2_bres_library_count(&image,resources::Library::animation);++i){assets::Animation a{};if(dh2_animation_open(&a,&image,i,0)!=assets::Error::ok)throw std::runtime_error("FX animation accessor");auto type=dh2_animation_type(&a,0);
    if(type==1||type==5||type==10)continue;
    if(type==26)continue; // actual shipping getAnimationTrackEx611ae0 null branch
    if(type<87||type>91)throw std::runtime_error("Required FX animation interpreter unavailable");
    auto* target=dh2_animation_target(&a);auto* channel=dh2_animation_channel(&a,0);auto p=word(channel+12);auto* name=reinterpret_cast<const char*>(span(p,1));auto* end=std::memchr(name,0,bytes.size()-p);if(!end||std::strcmp(name,"diffuse-sampler-matrix"))throw std::runtime_error("Required FX material parameter continuation unavailable");
    auto m=std::find_if(scene.materials.begin(),scene.materials.end(),[&](const scene::Material& m){return target&&m.id==target;});if(m==scene.materials.end())throw std::runtime_error("FX material animation target");textures.push_back({a,unsigned(m-scene.materials.begin())});
   }
   if(!player.load(bytes.data(),bytes.size(),scene,error,animation::MissingTargets::reject))throw std::runtime_error(error);
   if(player.unbound)throw std::runtime_error("FX unbound node animation");return true;
  }catch(const std::exception& e){error=e.what();return false;}
 }
 std::int32_t start_ms()const{return particle?particle->start_ms():player.start;}
 std::int32_t end_ms()const{return particle?particle->end_ms():player.end;}
 bool sample(std::int32_t ms,std::string& error){if(particle)return particle->sample_animation(ms,error);if(!player.sample(scene,ms,error))return false;for(const auto& t:textures){TextureTransform20V1 v;if(!texture_sample_v1(v,t.accessor,ms,true,error))return false;math::Matrix4f m{};if(dh2_fx_texture_matrix_v1(&m,&v)){error="FX material matrix";return false;}std::copy_n(m.m,16,scene.materials[t.material].texture_matrix);}return true;}
};
}
struct CharacterMeshFxOwnerV2::Impl {
 struct Set {std::int32_t id{-1};bool finished{};};
 struct Runtime {FxState96V1 state;std::string uri;std::shared_ptr<Resource> resource;timeline::State timeline{};float visual_position[3]{},visual_rotation[3]{},visual_scale[3]{1,1,1};bool pooled{},pending{};};
 data::EffectsTables::Borrow tables;const scene::Scene* live;MeshFxAssetsV1 assets;MeshFxServicesV1 services;CharacterParticleFxFactoryV2 factory;
 std::map<std::uintptr_t,std::shared_ptr<Runtime>> records;std::map<std::int32_t,std::vector<std::uintptr_t>> active,free;std::vector<std::uintptr_t> pending;std::vector<std::unique_ptr<Set>> sets;std::size_t cold{},warm{};std::int32_t dt{},scene_dt{};bool precache{};std::string failure;
 Impl(data::EffectsTables::Borrow t,const scene::Scene& l,MeshFxAssetsV1 a,MeshFxServicesV1 s,CharacterParticleFxFactoryV2 f):tables(std::move(t)),live(&l),assets(a),services(s),factory(f){}
 bool query(MeshFxOperationV1 op,std::uintptr_t id,const char* text,std::uint32_t& result,float* point=nullptr){if(!services.invoke){failure="Required FX source debug/anchor/floor service unavailable";return false;}MeshFxRequestV1 q{op,id,text,0,{0,0,0},live};if(point)std::copy_n(point,3,q.point);if(!services.invoke(services.context,q,failure))return false;result=q.result;if(point)std::copy_n(q.point,3,point);return true;}
 bool module(){std::uint32_t enabled;if(!query(MeshFxOperationV1::debug_load,0,nullptr,enabled)||!query(MeshFxOperationV1::module_enabled,0,"AnimatedFX",enabled))return false;return enabled!=0;}
 bool sync(Runtime& r,bool reset){auto& s=r.state;std::copy_n(s.position,3,r.visual_position);bool orient=s.orient_once||(reset&&s.orient_with_anchor);std::uint32_t result;
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
  case FxOperationV1::end_callback:if(q->argument){auto at=std::find_if(s.sets.begin(),s.sets.end(),[&](const auto& x){return reinterpret_cast<std::uintptr_t>(x.get())==q->identity;});if(at==s.sets.end()){s.failure="FX set-data identity expired";return -1;}(*at)->finished=true;}else{if(q->payload){auto at=std::find_if(s.sets.begin(),s.sets.end(),[&](const auto& x){return reinterpret_cast<std::uintptr_t>(x.get())==q->payload;});if(at==s.sets.end()){s.failure="FX callback set-data expired";return -1;}}r->pending=true;s.pending.push_back(reinterpret_cast<std::uintptr_t>(r.get()));}return 0;
  case FxOperationV1::app_dt:q->argument=std::uint32_t(s.dt);return 0;
  case FxOperationV1::completed:{if(r->resource->particle){bool complete=false;if(!r->resource->particle->completed(complete,s.failure))return -1;q->argument=complete;}else q->argument=1;return 0;}
  case FxOperationV1::debug_load:return s.query(MeshFxOperationV1::debug_load,0,nullptr,result)?0:-1;
  case FxOperationV1::debug_set:return s.query(MeshFxOperationV1::set_switch,state->set_identity,"isTracingAnim_FX",result)?0:-1;
  case FxOperationV1::debug_instance:return s.query(MeshFxOperationV1::instance_switch,reinterpret_cast<std::uintptr_t>(r.get()),"isTracingAnim_FX",result)?0:-1;
  case FxOperationV1::anchor_dead:case FxOperationV1::anchor_disabled:case FxOperationV1::anchor_stationary:{auto op=q->operation==FxOperationV1::anchor_dead?MeshFxOperationV1::anchor_dead:q->operation==FxOperationV1::anchor_disabled?MeshFxOperationV1::anchor_disabled:MeshFxOperationV1::anchor_stationary;if(!s.query(op,q->identity,nullptr,result))return -1;q->argument=result;return 0;}
  default:s.failure="Required FX state continuation unavailable";return -1;
 }}
 FxServices16V1 state_services(){return {this,invoke};}
 bool sample(Runtime& r,std::int32_t absolute){auto v=state_services();bool good=true;struct C{Impl* self;Runtime* r;FxServices16V1* v;bool* good;};C c{this,&r,&v,&good};timeline::Services callbacks{&c,[](void* p,timeline::State*){auto& c=*static_cast<C*>(p);if(dh2_fx_handle_loop_end_v1(&c.r->state,c.v))*c.good=false;}};if(dh2_timeline_update(&r.timeline,absolute,&callbacks)||!good)return false;return r.resource->sample(r.timeline.current_ms,failure)&&(!r.resource->particle||r.resource->particle->scene_frame(absolute,scene_dt,matrix(r.visual_position,r.visual_rotation,r.visual_scale),failure));}
};
CharacterMeshFxOwnerV2::CharacterMeshFxOwnerV2(data::EffectsTables::Borrow t,const scene::Scene& l,MeshFxAssetsV1 a,MeshFxServicesV1 s,CharacterParticleFxFactoryV2 f):impl_(std::make_unique<Impl>(std::move(t),l,a,s,f)){}
CharacterMeshFxOwnerV2::~CharacterMeshFxOwnerV2()=default;
bool CharacterMeshFxOwnerV2::precache_libraries(std::string& error){error.clear();auto& s=*impl_;s.failure.clear();if(!s.module()){error=s.failure;return error.empty();}s.precache=true;return true;}
bool CharacterMeshFxOwnerV2::precached()const noexcept{return impl_->precache;}
bool CharacterMeshFxOwnerV2::play_set(std::int32_t set,const float p[3],const float* rotation,std::uintptr_t anchor,std::uintptr_t* out,std::string& error){
 error.clear();if(out)*out=0;auto& s=*impl_;s.failure.clear();if(!s.tables||!p||!finite(p,3)||(rotation&&!finite(rotation,3))){error="FX native input contract";return false;}if(set<0||std::size_t(set)>=s.tables.sets().size())return true;const auto& row=s.tables.sets()[set];if(row.type!=0||row.steps.size()!=1||row.steps[0].redir||!row.steps[0].subobject.empty()){error="Required FX set/anchor family continuation unavailable";return false;}const auto& step=row.steps[0];if(step.file<0||std::size_t(step.file)>=s.tables.dictionary().values.size()){error="FX file declaration invalid";return false;}
 auto data=std::make_unique<Impl::Set>();data->id=set;auto id=reinterpret_cast<std::uintptr_t>(data.get());s.sets.push_back(std::move(data)); // source set insertion precedes Play/debug/factory
 auto disabled=s.module();if(!disabled){error=s.failure;return error.empty();}
 std::shared_ptr<Impl::Runtime> r;auto& pool=s.free[step.file];auto& active=s.active[step.file];
 if(!pool.empty()){auto identity=pool.back();r=s.records.at(identity);for(auto& m:r->resource->scene.materials)m.color[3]=1.f;pool.pop_back();r->pooled=false;++s.warm;}
 else{
  if(active.size()>5)return true;r=std::make_shared<Impl::Runtime>();r->state.file=step.file;r->uri=s.tables.dictionary().values[step.file];r->resource=std::make_shared<Resource>();if(!r->resource->load(s.assets,r->uri,s.factory,*s.live,error))return false;r->state.visual=reinterpret_cast<std::uintptr_t>(r->resource.get());r->state.loop=-1;r->timeline.scale=1;r->timeline.library_present=1;
  if(dh2_timeline_clip(&r->timeline,0,r->resource->start_ms(),r->resource->end_ms())){error="FX timeline construction";return false;}s.records.emplace(reinterpret_cast<std::uintptr_t>(r.get()),r);++s.cold;
 }
 auto identity=reinterpret_cast<std::uintptr_t>(r.get());active.push_back(identity);FxStep24V1 native{step.orient_once,step.orient_with_anchor,step.scale_with_anchor,0,step.loop,step.play_time};std::memcpy(&native.speed,&step.speed_bits,4);FxData32V1 d;if(dh2_fx_data_v1(&d,&native,row.type,row.loop,id)){error="FX set-data projection";return false;}auto v=s.state_services();if(dh2_fx_play_v1(&r->state,p,rotation,anchor,&d,1,&v)){error=s.failure.empty()?"FX play source service failure":s.failure;return false;}if(out)*out=identity;return true;
}
bool CharacterMeshFxOwnerV2::animation_event(const char* event,const float p[3],std::string& error){error.clear();if(!event||!p){error="FX event input";return false;}if(std::strncmp(event,"fx_",3)){error="Event is outside source fx_ branch";return false;}auto& s=*impl_;if(!s.tables){error="FX tables unavailable";return false;}const auto& names=s.tables.set_names();for(unsigned i=0;i<names.size();++i)if(names[i]==event+3)return play_set(i,p,nullptr,0,nullptr,error);return true;}
bool CharacterMeshFxOwnerV2::drop(std::uintptr_t& identity,std::string& error){error.clear();if(!identity)return true;auto& s=*impl_;s.failure.clear();if(!s.precache){identity=0;return true;}auto at=s.records.find(identity);if(at==s.records.end()||at->second->pooled){error="FX invalid active identity";return false;}auto r=at->second;auto& active=s.active[r->state.file];active.erase(std::remove(active.begin(),active.end(),identity),active.end());auto& free=s.free[r->state.file];if(std::find(free.begin(),free.end(),identity)==free.end())free.push_back(identity);r->pooled=true;auto v=s.state_services();if(dh2_fx_drop_reset_v1(&r->state,&v)){error=s.failure.empty()?"FX drop source service failure":s.failure;return false;}identity=0;return true;}
bool CharacterMeshFxOwnerV2::scene_frame(std::int32_t ms,std::int32_t app_dt,std::string& error){error.clear();auto& s=*impl_;s.failure.clear();s.scene_dt=app_dt;for(auto& pool:s.active){std::size_t i=0,budget=0;while(i<pool.second.size()){if(++budget>65536){error="FX scene reentry limit";return false;}auto identity=pool.second[i];auto r=s.records.at(identity);if(!s.sample(*r,ms)){error=s.failure.empty()?"FX scene timeline/sample rejected":s.failure;return false;}if(i<pool.second.size()&&pool.second[i]==identity)++i;}}return true;}
bool CharacterMeshFxOwnerV2::manager_frame(std::int32_t dt,std::string& error){error.clear();auto& s=*impl_;s.failure.clear();if(!s.module()){error=s.failure;return error.empty();}s.dt=dt;auto v=s.state_services();for(auto& pool:s.active){std::size_t i=0,budget=0;while(i<pool.second.size()){if(++budget>65536){error="FX manager reentry limit";return false;}auto identity=pool.second[i];auto r=s.records.at(identity);if(dh2_fx_update_v1(&r->state,&v)){error=s.failure.empty()?"FX update service failure":s.failure;return false;}if(i<pool.second.size()&&pool.second[i]==identity)++i;}}
 for(std::size_t i=0;i<s.pending.size();){auto id=s.pending[i];auto r=s.records.at(id);FxRequest32V1 q{FxOperationV1::end_sample};if(Impl::invoke(&s,&r->state,&q)){error=s.failure;return false;}if(r->state.visible){++i;continue;}s.pending.erase(s.pending.begin()+i);r->pending=false;if(!r->pooled&&!drop(id,error))return false;}
 return true;}
bool CharacterMeshFxOwnerV2::draw_parts(std::vector<skinning::VisualDrawPartV6>& out,std::string& error)const{error.clear();auto& s=*impl_;struct Snapshot{std::shared_ptr<Resource> resource;std::vector<scene::Material> materials;std::vector<std::vector<std::uint32_t>> bindings;};std::vector<skinning::VisualDrawPartV6> parts;for(const auto& pool:s.active)for(auto id:pool.second){const auto& r=*s.records.at(id);if(!r.state.visible)continue;if(r.resource->particle){std::vector<skinning::VisualDrawPartV6> particle_parts;if(!r.resource->particle->draw_parts(particle_parts,error))return false;for(auto& part:particle_parts)parts.push_back(std::move(part));continue;}auto retained=std::make_shared<Snapshot>();retained->resource=r.resource;retained->materials=r.resource->scene.materials;for(const auto& instance:r.resource->scene.instances)retained->bindings.push_back(instance.materials);auto outer=matrix(r.visual_position,r.visual_rotation,r.visual_scale);if(!finite(outer.data(),16)){error="FX outer matrix nonfinite";return false;}for(unsigned i=0;i<r.resource->scene.instances.size();++i){const auto& instance=r.resource->scene.instances[i];skinning::VisualDrawPartV6 d;d.retention=retained;d.geometry=&r.resource->geometry[i];d.material_table=&retained->materials;d.materials=&retained->bindings[i];d.positions=d.geometry->positions;d.world=scene::multiply(outer,instance.world);parts.push_back(std::move(d));}}out=std::move(parts);return true;}
std::vector<MeshFxViewV1> CharacterMeshFxOwnerV2::views()const{std::vector<MeshFxViewV1> out;for(const auto& x:impl_->records){const auto& r=*x.second;std::int32_t set=-1;bool finished=false;if(r.state.set_identity)for(const auto& v:impl_->sets)if(reinterpret_cast<std::uintptr_t>(v.get())==r.state.set_identity){set=v->id;finished=v->finished;break;}out.push_back({x.first,r.state,set,r.timeline.current_ms,r.timeline.start_ms,r.timeline.end_ms,r.pooled,r.pending,finished,r.uri});}return out;}
std::size_t CharacterMeshFxOwnerV2::cold_creations()const noexcept{return impl_->cold;}
std::size_t CharacterMeshFxOwnerV2::warm_reuses()const noexcept{return impl_->warm;}
}
