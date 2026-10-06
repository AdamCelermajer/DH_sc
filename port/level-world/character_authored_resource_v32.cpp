#include "../engine-animation/particle_scalar_animation_v6.hpp"
#include "character_authored_resource_v32.hpp"
#include "../engine-animation/particle_resource_init_v32.hpp"
#include "../engine-animation/particle_cloud_runtime_v3.hpp"
#include "../engine-animation/particle_billboard_v32.hpp"
#include "../engine-animation/particle_force_scene_v1.hpp"
#include "../engine-animation/particle_scene_color_v1.hpp"
#include "../scene-materials/particle_scene_v1.hpp"
#include "../engine-animation/animation.hpp"
#include "../engine-animation/particle_box_v2.hpp"
#include "../engine-animation/material_color.hpp"
#include "../engine-animation/material_color_v3.hpp"
#include "../scene-materials/material_matrix_v4.hpp"
#include "source_fx_node_matrix_v4.hpp"
#include "authored_fx_transform_v5.hpp"
#include "authored_fx_mesh_graph_v32.hpp"
#include "authored_fx_null_tracks_v5.hpp"
#include <algorithm>
#include <cstring>
#include <cmath>
#include <stdexcept>
#include <functional>
namespace dh2::fx {
namespace {
using namespace animation;
struct Snapshot {
 skinning::VisualGeometryV6 geometry;
 std::vector<scene::Material> materials;
 std::vector<std::uint32_t> binding;
};
struct SharedGraphV32 { scene::Scene scene;std::vector<math::Matrix4f> matrices;bool pose_sampled{};std::unique_ptr<AuthoredFxTransformOwnerV5> transforms;std::int32_t start{},end{}; };
class AuthoredEmitterV32 final:public CharacterParticleFxResourceV2 {
 std::shared_ptr<const std::vector<std::uint8_t>> bytes_;
 resources::BresView image_{};std::shared_ptr<SharedGraphV32> shared_;scene::Scene& scene_;animation::Player animation_;std::uint32_t emitter_index_{};CharacterFxForceFactoryV4 force_factory_;CharacterFxBoundForcesV4 bound_;math::Matrix4f outer_{};
 CharacterBloodFxSceneServicesV2 services_;ParticleEmitterInput input_;
 std::shared_ptr<ParticleGenerationOwner> generation_;
 std::unique_ptr<ParticleEmissionOwner> emission_;
 ParticleCloudModelsV1 models_{};std::int32_t seed_{123456789};
 std::unique_ptr<ParticleCloudRuntimeV3> cloud_;
 std::shared_ptr<const ParticleScalarAnimationResourceV6> scalar_animation_;std::vector<ParticleScalarBindingV6> scalar_bindings_;
 ParticleBirthRateBinding birth_;std::vector<ParticleGravitySceneV1> forces_;
 std::vector<ParticleGravityV1> force_models_;std::vector<std::uint32_t> bound_forces_;
 std::uint32_t emitter_node_{},emitter_instance_{},material_{};float radius_{},dimensions_[3]{};ParticleBoxV2 box_{};
 std::uint32_t camera_offset_word_{0},rendering_layer_{0};
 struct ColorTrack {assets::Animation accessor;assets::Vector values;};std::vector<ColorTrack> colors_;
 float camera_view_[16]{},camera_position_[3]{},uv_[8]{};math::Matrix4f* source_uv_matrix68_{};
 std::array<float,16> emitter_world_{};ParticleBillboardBoundsV1 bounds_{};
 std::shared_ptr<Snapshot> draw_;std::string failure_;bool ready_{};std::int32_t source_start_{},source_end_{};
 std::uint32_t word(std::uint64_t p)const{if(p>image_.size||4>image_.size-p)throw std::runtime_error("Blood FX record range");std::uint32_t v;std::memcpy(&v,image_.bytes+p,4);return v;}
 std::string text(std::uint32_t p)const{if(!p||p>=image_.size)throw std::runtime_error("Blood FX string");auto b=reinterpret_cast<const char*>(image_.bytes+p);auto e=static_cast<const char*>(std::memchr(b,0,image_.size-p));if(!e)throw std::runtime_error("Blood FX unterminated string");return {b,e};}
 static int emitter(void*,ParticleGenerationOwner&,std::uint32_t type){return type<=1?0:-2;}
 static int shape(void* p,ParticleGenerationOwner& o,const char* n,std::uint32_t value){auto& r=*static_cast<AuthoredEmitterV32*>(p);float* field=nullptr;if(r.input_.record[2]==1&&std::strcmp(n,"RadiusLength")==0)field=&r.radius_;else if(r.input_.record[2]==0){const char* names[]{"RadiusLength","Width","Height"};for(unsigned i=0;i<3;++i)if(std::strcmp(n,names[i])==0)field=r.dimensions_+i;}if(!field)return -2;o.register_parameter(o.hash_name(n),field);std::memcpy(field,&value,4);return 0;}
 static int initialize_emitter(void* p,ParticleSeed100* particles,std::uint32_t count,const float* world,bool local,std::int32_t* seed){auto& r=*static_cast<AuthoredEmitterV32*>(p);return r.input_.record[2]==0?dh2_particle_box_emitter_init_v2(particles,count,&r.box_,world,local,seed):dh2_particle_emitter_init_v1(particles,count,&r.models_.sphere,world,local,seed);}
 static int vector(void*,ParticleGenerationOwner& o,const char* n,const std::uint32_t v[3]){auto lease=o.parameter(n);if(lease)std::memcpy(lease.storage,v,12);return 0;}
 static int render_initialize(void* p,ParticleGenerationOwner& o,const ParticleEmitterInput&){auto& r=*static_cast<AuthoredEmitterV32*>(p);const auto maximum=o.generation().max_particles;if(!maximum||maximum>16383){r.failure_="Required blood FX capacity domain";return -2;}
  // Original allocates maxParticles*4 vertices and maxParticles*6 indices.
  // Native retained CPU geometry is populated by the same source baker.
  auto s=std::make_shared<Snapshot>();s->geometry.positions.reserve(maximum*4);s->materials=r.scene_.materials;s->binding={r.material_};r.draw_=std::move(s);return 0;}
 int apply(ParticleSeed100* p,std::uint32_t count){
  int rc=dh2_particle_billboard_apply_v32(p,count,camera_position_,emitter_world_.data(),false,&bounds_);if(rc)return rc;
  ParticleBillboardBasisV1 basis{};rc=dh2_particle_billboard_basis_v1(&basis,camera_view_);if(rc)return rc;
  auto s=std::make_shared<Snapshot>();s->materials=scene_.materials;s->binding={material_};auto& g=s->geometry;g.id=input_.name;
  std::copy_n(bounds_.minimum,3,g.minimum);std::copy_n(bounds_.maximum,3,g.maximum);
  skinning::VisualAttributeV6 colors{1,4,{}},uvs{6,2,{}};
  skinning::VisualPrimitiveV6 primitive;primitive.material_symbol=scene_.materials.at(material_).id;primitive.collada_type=0;primitive.engine_type=6;primitive.attributes.fill(-1);primitive.attributes[2]=0;primitive.attributes[4]=1;
  for(unsigned i=0;i<count;++i){ParticleBillboardVertexV1 v[4]{};rc=dh2_particle_billboard_vertices_v1(v,&basis,p+i,uv_);if(rc)return rc;
   for(const auto& vertex:v){g.positions.push_back({vertex.position[0],vertex.position[1],vertex.position[2]});const auto c=vertex.color;colors.values.insert(colors.values.end(),{float((c>>16)&255),float((c>>8)&255),float(c&255),float(c>>24)});
    // Actual ProfileCOMMON_emul_VS: TextureMatrix0*vec4(TexCoord0,1,0).
    // Bake this source equation because the existing native shader uses w1.
    const float u=source_uv_matrix68_->m[0]*vertex.uv[0]+source_uv_matrix68_->m[4]*vertex.uv[1]+source_uv_matrix68_->m[8];
    const float vcoord=source_uv_matrix68_->m[1]*vertex.uv[0]+source_uv_matrix68_->m[5]*vertex.uv[1]+source_uv_matrix68_->m[9];
    uvs.values.insert(uvs.values.end(),{u,vcoord});}
   // Exact original twelve-byte template at99b85c, init656f2c/6569e8.
   for(unsigned index:{0u,1u,2u,0u,2u,3u})primitive.indices.push_back(i*4+index);
  }g.attributes.push_back(std::move(colors));g.attributes.push_back(std::move(uvs));g.primitives.push_back(std::move(primitive));draw_=std::move(s);return 0;
 }
public:
 AuthoredEmitterV32(std::shared_ptr<const std::vector<std::uint8_t>> b,CharacterBloodFxSceneServicesV2 s,std::shared_ptr<SharedGraphV32> shared,std::uint32_t index,CharacterFxForceFactoryV4 force):bytes_(std::move(b)),shared_(std::move(shared)),scene_(shared_->scene),emitter_index_(index),force_factory_(force),services_(s){}
 bool initialize(std::string& error){try{
  if(dh2_bres_open(&image_,bytes_->data(),bytes_->size())!=resources::BresError::ok)throw std::runtime_error("Blood FX BRES");
  if(!word(image_.root_offset+0x78)||word(image_.root_offset+0x80)||word(image_.root_offset+0x90))throw std::runtime_error("Required particle resource family");
  if(!decode_particle_emitter(bytes_,emitter_index_,input_,error))return false;

  if(scene_.graph.empty())throw std::runtime_error("Required same retained composite scene");
  unsigned found=0;auto vs=word(image_.root_offset+156);auto nodes=word(vs+12);
  // Traverse the actual serialized source root forest and children in order.
  // Scene loader retains this complete domain, including FIRE child emitter.
  unsigned visited=0;std::function<void(std::uint32_t,unsigned)> visit=[&](std::uint32_t node,unsigned depth){if(depth>64||++visited>10000)throw std::runtime_error("Authored emitter graph range");word(std::uint64_t(node)+76);auto n=word(node+64),base=word(node+68);for(unsigned j=0;j<n;++j)if(word(std::uint64_t(base)+8*j)==9){auto instance=word(std::uint64_t(base)+8*j+4);auto uri=text(word(instance+4));if(uri!="#"+input_.name)continue;++found;emitter_instance_=instance;auto id=text(word(node));unsigned matches=0;for(unsigned k=0;k<scene_.graph.size();++k)if(scene_.graph[k].id==id){emitter_node_=k;++matches;}if(matches!=1)throw std::runtime_error("Authored emitter same scene node");}auto count=word(node+56),children=word(node+60);if(count>10000)throw std::runtime_error("Authored emitter child count");for(unsigned i=0;i<count;++i)visit(children+80*i,depth+1);};for(unsigned i=0;i<word(vs+8);++i)visit(nodes+80*i,0);
  if(found!=1)throw std::runtime_error("Blood emitter instance count");
  if(word(emitter_instance_+12)!=1)throw std::runtime_error("Required blood material binding count");auto binding=word(emitter_instance_+16);if(word(binding))throw std::runtime_error("Required external blood material");auto material_uri=text(word(binding+4));if(material_uri.empty()||material_uri[0]!='#')throw std::runtime_error("Blood material URI");unsigned matches=0;for(unsigned i=0;i<scene_.materials.size();++i)if(scene_.materials[i].id==material_uri.substr(1)){material_=i;++matches;}if(matches!=1)throw std::runtime_error("Blood material same source table");source_uv_matrix68_=&shared_->matrices.at(material_);
  // Source effect defaults are separate from material override parameters.
  // Both actual blood assets bind ProfileCOMMON_fx_particles_alpha; its
  // diffuse-sampler-matrix selects the authored atlas tile, not the whole image.

  const auto* material_record=dh2_bres_library_item(&image_,resources::Library::material,material_);if(!material_record)throw std::runtime_error("Blood material source record");const auto mat=std::uint32_t(material_record-image_.bytes);if(word(mat+8))throw std::runtime_error("Required external blood effect");auto effect_uri=text(word(mat+12));if(effect_uri.empty()||effect_uri[0]!='#')throw std::runtime_error("Blood effect source URI");
  unsigned effect_matches=0;bool has_matrix=false,has_color=false;for(unsigned i=0;i<dh2_bres_library_count(&image_,resources::Library::effect);++i){const auto* record=dh2_bres_library_item(&image_,resources::Library::effect,i);const auto at=std::uint32_t(record-image_.bytes);if(text(word(at))!=effect_uri.substr(1))continue;++effect_matches;if(text(word(at+4)).rfind("ProfileCOMMON",0)!=0)throw std::runtime_error("Required authored shader profile");const auto n=word(at+16),base=word(at+20);for(unsigned j=0;j<n;++j){const auto p=base+24*j;auto name=text(word(p));if(name=="diffuse-sampler-matrix"){if(word(p+4)!=10||word(p+12)!=1)throw std::runtime_error("Blood effect matrix layout");for(unsigned k=0;k<16;++k){auto raw=word(word(p+20)+4*k);std::memcpy(source_uv_matrix68_->m+k,&raw,4);if(!std::isfinite(source_uv_matrix68_->m[k]))throw std::runtime_error("Blood effect nonfinite matrix");}scene::material_matrix_from_bres_v4(*source_uv_matrix68_,source_uv_matrix68_->m);has_matrix=true;}else if(name=="diffuse-color"){if(word(p+12)!=1)throw std::runtime_error("Authored diffuse color count");if(word(p+4)==16){for(unsigned k=0;k<4;++k){auto raw=word(word(p+20)+4*k);std::memcpy(scene_.materials[material_].color+k,&raw,4);}}else if(word(p+4)==15){ColorParameter32 parameter{8,1,{0,0,0,0},-1,-1};const auto raw=word(word(p+20));std::uint8_t color[4];std::memcpy(color,&raw,4);if(dh2_material_color_set(&parameter,0,color)!=1)throw std::runtime_error("Authored SColor conversion");std::memcpy(scene_.materials[material_].color,parameter.words,16);}else throw std::runtime_error("Required authored diffuse color type");has_color=true;}}}
  if(effect_matches!=1||!has_matrix||!has_color)throw std::runtime_error("Required actual blood effect defaults");
  // Existing native material matrix remains identity: source UV transform is
  // now baked once. Transparent sampler is not an AlphaMap red-channel alias.
  if(word(image_.root_offset+0x88)){
   if(!force_factory_.create||!force_factory_.create(force_factory_.context,image_,scene_,shared_,emitter_instance_,bound_,error)||!bound_.apply)throw std::runtime_error(error.empty()?"Required actual retained bound force models":error);
  }
  // All source model parameter cells used in this authored domain are supplied
  // below; untouched diagnostic context cells never become gameplay defaults.
  ParticleContextSeed92 context{};generation_=ParticleGenerationOwner::create(context);if(!register_particle_cloud_models_v1(*generation_,models_))throw std::runtime_error("Blood parameter owner registration");
  const int initialized=initialize_particle_resource_v32(*generation_,input_,{this,emitter,shape},{this,vector,render_initialize});if(initialized)throw std::runtime_error(failure_.empty()?"Required blood resource initialization":failure_);
  const float center[3]{};if(input_.record[2]==0?dh2_particle_box_construct_v2(&box_,dimensions_):dh2_particle_sphere_construct_v1(&models_.sphere,center,radius_,0))throw std::runtime_error("Blood source sphere constructor");
  dh2_particle_billboard_uv_v1(uv_);
  bool has_scalar=false;for(unsigned i=0;i<dh2_bres_library_count(&image_,resources::Library::animation);++i){assets::Animation a{};if(dh2_animation_open(&a,&image_,i,0)!=assets::Error::ok)throw std::runtime_error("Required source scalar accessor");auto type=dh2_animation_type(&a,0);has_scalar|=type==28||type==37||type==38;}
  if(has_scalar){scalar_animation_=ParticleScalarAnimationResourceV6::create(bytes_->data(),bytes_->size(),error);if(!scalar_animation_)return false;
   for(unsigned i=0;i<scalar_animation_->track_count();++i)if(*scalar_animation_->target(i)==input_.name){auto parameter=generation_->parameter(scalar_animation_->parameter_name(i));if(!parameter.storage)throw std::runtime_error("Required source particle parameter storage");scalar_bindings_.push_back({scalar_animation_,parameter,i,0});}
  }
  if(!animation_.load(bytes_->data(),bytes_->size(),scene_,error,animation::MissingTargets::reject))return false;
  if(animation_.unbound)throw std::runtime_error("Blood unbound node animation");
  for(unsigned i=0;i<dh2_bres_library_count(&image_,resources::Library::animation);++i){assets::Animation a{};if(dh2_animation_open(&a,&image_,i,0)!=assets::Error::ok)throw std::runtime_error("Blood animation accessor");if(!i){source_start_=a.segment_start;source_end_=a.segment_end;}else{source_start_=std::min(source_start_,a.segment_start);source_end_=std::max(source_end_,a.segment_end);}auto type=dh2_animation_type(&a,0);if(type==86){if(scene_.materials[material_].id!=dh2_animation_target(&a))continue;if(dh2_animation_channels(&a)!=1||dh2_animation_samplers(&a)!=1)throw std::runtime_error("Required authored material animation target");auto ch=dh2_animation_channel(&a,0);std::uint32_t prop;std::memcpy(&prop,ch+12,4);if(text(prop)!="diffuse-color")throw std::runtime_error("Required authored material parameter");assets::Vector values{};if(!dh2_animation_vector(&a,0,true,&values)||values.type!=1||(values.components!=4&&values.components!=1)||!values.count||dh2_animation_animator(&a)||dh2_animation_offsets(&a))throw std::runtime_error("Required uchar4 material sampler");for(unsigned k=1;k<values.count;++k)if(dh2_animation_key_time(&a,0,k)<dh2_animation_key_time(&a,0,k-1))throw std::runtime_error("Unordered material time");if(values.components==1&&(!dh2_animation_has_default(&a)||!dh2_animation_default(&a)))throw std::runtime_error("Required source particle alpha default");colors_.push_back({a,values});}else if(!(type>=1&&type<=13)&&type!=28&&type!=37&&type!=38&&!authored_fx_null_track_v5(type)&&(type<87||type>91))throw std::runtime_error("Required authored other animation continuation");}
  emission_=std::make_unique<ParticleEmissionOwner>(generation_);cloud_=std::make_unique<ParticleCloudRuntimeV3>(*emission_,models_,seed_,123456789,ParticleCloudRenderV1{[this](){return draw_?0:-2;},[this](ParticleSeed100* p,std::uint32_t n){return apply(p,n);}},ParticleCloudEmitterV2{this,initialize_emitter},ParticleCloudForcesV3{this,[](void* raw,ParticleSeed100* particles,std::uint32_t count,float dt,std::int32_t* seed){auto& r=*static_cast<AuthoredEmitterV32*>(raw);if(!r.bound_.apply)return 0;return r.bound_.apply(r.bound_.context,particles,count,dt,seed,r.outer_,r.failure_);}});if(cloud_->initialize())throw std::runtime_error("Blood cloud source initialize");ready_=true;return true;
 }catch(const std::exception& e){error=e.what();return false;}}
 std::int32_t start_ms()const noexcept override{return source_start_;}
 std::int32_t end_ms()const noexcept override{return source_end_;}
 bool sample_animation(std::int32_t ms,std::string& error)override{if(!ready_){error="Blood resource not initialized";return false;}if((!shared_->pose_sampled&&!animation_.sample(scene_,ms,error))){if(error.empty())error="Authored source animation sample";return false;}
  if(!shared_->pose_sampled&&!source_fx_rebuild_graph_world_v4(scene_,error))return false;shared_->pose_sampled=true;for(auto& binding:scalar_bindings_)if(binding.sample_apply(ms)){error="Required source scalar particle application";return false;}for(const auto& t:colors_){std::int32_t key=0;float fraction=0;bool between=dh2_animation_find(&t.accessor,0,ms,&key,&fraction);if(key<0||unsigned(key)>=t.values.count||(between&&unsigned(key)+1>=t.values.count)){error="Authored material key";return false;}std::uint8_t value[4];if(t.values.components==1){ColorAccessor24 alpha{t.values.data,t.values.count,1,dh2_animation_default(&t.accessor)};const int status=between?dh2_material_alpha_between(value,&alpha,key,key+1,fraction):dh2_material_alpha_key(value,&alpha,key);if(status){error="Required source particle alpha interpreter";return false;}}else if(between){MaterialColorAccessorV3 accessor{t.values.data,t.values.count};if(dh2_material_color_between_v3(value,&accessor,key,key+1,fraction)){error="Authored color interpolation";return false;}}else std::memcpy(value,t.values.data+4*key,4);ColorParameter32 parameter{8,1,{0,0,0,0},-1,-1};std::memcpy(parameter.words,scene_.materials[material_].color,16);if(dh2_material_color_set(&parameter,0,value)!=1){error="Authored material application";return false;}std::memcpy(scene_.materials[material_].color,parameter.words,16);}
  return true;}
 bool scene_frame(std::int32_t absolute,std::int32_t,const std::array<float,16>& outer,std::string& error)override{
  if(!ready_||!services_.camera||!services_.driver_type){error="Required actual blood camera/driver owner";return false;}
  if(!services_.camera(services_.context,camera_view_,camera_position_,error))return false;std::uint32_t driver,color;if(!services_.driver_type(services_.context,driver,error))return false;if(particle_scene_color_v1(driver,{},color)){error="Required actual particle scene lighting lookup";return false;}
  (void)outer;math::Matrix4f local{},world{};if(!source_fx_node_world_matrix_v4(scene_,emitter_node_,local,error))return false;source_fx_matrix_multiply_v4(world,outer_,local);std::copy_n(world.m,16,emitter_world_.data());
  ParticleCloudFrameV1 frame{emitter_world_.data(),false,&color,{}};ParticleSeed100 untouched{};int rc=cloud_->update(float(absolute)/1000.f,untouched,frame);if(rc){error="Blood source cloud update "+std::to_string(rc);return false;}return true;
 }
 bool typed_scene_frame(std::int32_t absolute,std::int32_t dt,const math::Matrix4f& outer,std::string& error){std::memcpy(&outer_,&outer,65);std::array<float,16> values{};std::copy_n(outer.m,16,values.data());return scene_frame(absolute,dt,values,error);}
 std::uint32_t source_node()const noexcept{return emitter_node_;}
 // AnimatedFX::HasCompleted4924e0 calls the source particle node's count
 // virtualf8 and returns count<=0. Selected SParticle getter64bb90 reads the
 // SAME vector begin/end divided by100; it does not disable generation.
 bool completed(bool& out,std::string&)const override{out=emission_&&emission_->particles().empty();return ready_;}
 bool draw_parts(std::vector<skinning::VisualDrawPartV6>& out,std::string& error)const override{if(!ready_||!draw_){error="Blood source render backing";return false;}if(draw_->geometry.positions.empty()){out.clear();return true;}skinning::VisualDrawPartV6 part;part.retention=draw_;part.geometry=&draw_->geometry;part.material_table=&draw_->materials;part.materials=&draw_->binding;part.positions=draw_->geometry.positions;part.world={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};out={std::move(part)};return true;}
 bool draw_sources_v3(std::vector<CharacterParticleDrawSourceV3>& out,std::string& error)const override{
  std::vector<skinning::VisualDrawPartV6> parts;if(!draw_parts(parts,error))return false;
  std::vector<CharacterParticleDrawSourceV3> result;for(auto& part:parts){
   CharacterParticleDrawSourceV3 s;s.resource_bytes=bytes_;s.image=&image_;s.scene=&scene_;
   s.emitter_node=emitter_node_;s.material=material_;s.emitter_world=emitter_world_;
   s.particle_identity=reinterpret_cast<std::uintptr_t>(this);s.part=std::move(part);s.camera_offset_word=camera_offset_word_;s.rendering_layer=rendering_layer_;s.source_node_render_fields_ready=true;s.source_texture_matrix68=source_uv_matrix68_;result.push_back(std::move(s));
  }out=std::move(result);return true;
 }
};
}
class CompositeResourceV32 final:public CharacterAuthoredCompositeFxResourceV4 {
 std::shared_ptr<const std::vector<std::uint8_t>> bytes_;resources::BresView image_{};
 std::shared_ptr<SharedGraphV32> shared_=std::make_shared<SharedGraphV32>();
 std::vector<std::shared_ptr<AuthoredEmitterV32>> emitters_;
 std::unique_ptr<AuthoredFxMeshGraphV32> meshes_;math::Matrix4f outer_{};bool outer_written_{};
public:
 bool initialize(std::shared_ptr<const std::vector<std::uint8_t>> bytes,CharacterBloodFxSceneServicesV2 services,CharacterFxForceFactoryV4 forces,std::string& error){
  bytes_=std::move(bytes);if(dh2_bres_open(&image_,bytes_->data(),bytes_->size())!=resources::BresError::ok){error="Composite FX BRES";return false;}
  if(!scene::load_particle_scene_v1(image_,shared_->scene,error))return false;
  if(!source_fx_rebuild_graph_world_v4(shared_->scene,error))return false;
  shared_->matrices.resize(shared_->scene.materials.size());
  shared_->transforms=std::make_unique<AuthoredFxTransformOwnerV5>(image_,shared_->scene);if(!shared_->transforms->initialize(error))return false;
  const auto animation_count=dh2_bres_library_count(&image_,resources::Library::animation);
  if(dh2_animation_segments(&image_)!=1){error="Required general FX single segment controller";return false;}
  for(unsigned i=0;i<animation_count;++i){assets::Animation a{};if(dh2_animation_open(&a,&image_,i,0)!=assets::Error::ok){error="General FX source segment range";return false;}
   if(!i){shared_->start=a.segment_start;shared_->end=a.segment_end;}else{shared_->start=std::min(shared_->start,a.segment_start);shared_->end=std::max(shared_->end,a.segment_end);}}

  std::uint32_t count;std::memcpy(&count,image_.bytes+image_.root_offset+0x78,4);
  if(count>4096){error="Composite emitter count";return false;}
  for(unsigned i=0;i<count;++i){auto emitter=std::make_shared<AuthoredEmitterV32>(bytes_,services,shared_,i,forces);if(!emitter->initialize(error))return false;emitters_.push_back(std::move(emitter));}
  std::stable_sort(emitters_.begin(),emitters_.end(),[](const auto& a,const auto& b){return a->source_node()<b->source_node();});
  meshes_=std::make_unique<AuthoredFxMeshGraphV32>(bytes_,shared_,shared_->scene,shared_->matrices);if(!meshes_->initialize(error))return false;
  return true;
 }
 std::int32_t start_ms()const noexcept override{return shared_->start;}
 std::int32_t end_ms()const noexcept override{return shared_->end;}
 bool sample_animation(std::int32_t ms,std::string& error)override{if(!shared_->transforms->sample(ms,error))return false;shared_->pose_sampled=true;for(auto& emitter:emitters_)if(!emitter->sample_animation(ms,error))return false;return meshes_->sample(ms,error);}
 bool scene_frame(std::int32_t,std::int32_t,const std::array<float,16>&,std::string& error)override{error="Required source-produced typed FX outer68";return false;}
 bool source_scene_frame_v4(std::int32_t absolute,std::int32_t dt,const math::Matrix4f& outer,std::string& error)override{std::memcpy(&outer_,&outer,65);outer_written_=true;for(auto& emitter:emitters_)if(!emitter->typed_scene_frame(absolute,dt,outer,error))return false;return true;}
 bool completed(bool& out,std::string& error)const override{if(emitters_.empty()){out=true;return true;}return emitters_.front()->completed(out,error);}
 bool draw_parts(std::vector<skinning::VisualDrawPartV6>& out,std::string& error)const override{std::vector<skinning::VisualDrawPartV6> result;std::vector<CharacterFxMeshDrawSourceV4> meshes;if(!mesh_draw_sources_v4(meshes,error))return false;for(auto& mesh:meshes)result.push_back(std::move(mesh.part));for(auto& emitter:emitters_){std::vector<skinning::VisualDrawPartV6> parts;if(!emitter->draw_parts(parts,error))return false;for(auto& part:parts)result.push_back(std::move(part));}out=std::move(result);return true;}
 bool mesh_draw_sources_v4(std::vector<CharacterFxMeshDrawSourceV4>& out,std::string& error)const override{if(!outer_written_){error="Required actual typed FX outer transform before mesh submission";return false;}return meshes_->draw_sources(outer_,out,error);}
 bool draw_sources_v3(std::vector<CharacterParticleDrawSourceV3>& out,std::string& error)const override{std::vector<CharacterParticleDrawSourceV3> result;for(auto& emitter:emitters_){std::vector<CharacterParticleDrawSourceV3> parts;if(!emitter->draw_sources_v3(parts,error))return false;for(auto& part:parts)result.push_back(std::move(part));}out=std::move(result);return true;}
};
bool CharacterAuthoredResourceFactoryV32::create(void* raw,std::shared_ptr<const std::vector<std::uint8_t>> bytes,const scene::Scene&,std::shared_ptr<CharacterParticleFxResourceV2>& out,std::string& error){out.reset();if(!raw||!bytes){error="Composite FX factory source borrow";return false;}auto& factory=*static_cast<CharacterAuthoredResourceFactoryV32*>(raw);auto resource=std::make_shared<CompositeResourceV32>();if(!resource->initialize(std::move(bytes),factory.services_,factory.forces_,error))return false;out=std::move(resource);return true;}
}


