#include "character_blood_fx_resource_v2.hpp"
#include "../engine-animation/particle_resource_init_v1.hpp"
#include "../engine-animation/particle_cloud_runtime_v1.hpp"
#include "../engine-animation/particle_billboard_v1.hpp"
#include "../engine-animation/particle_force_scene_v1.hpp"
#include "../engine-animation/particle_scene_color_v1.hpp"
#include "../scene-materials/particle_scene_v1.hpp"
#include "../engine-animation/animation.hpp"
#include <algorithm>
#include <cstring>
#include <cmath>
#include <stdexcept>
namespace dh2::fx {
namespace {
using namespace animation;
struct Snapshot {
 skinning::VisualGeometryV6 geometry;
 std::vector<scene::Material> materials;
 std::vector<std::uint32_t> binding;
};
class BloodResource final:public CharacterParticleFxResourceV2 {
 std::shared_ptr<const std::vector<std::uint8_t>> bytes_;
 resources::BresView image_{};scene::Scene scene_;animation::Player animation_;
 CharacterBloodFxSceneServicesV2 services_;ParticleEmitterInput input_;
 std::shared_ptr<ParticleGenerationOwner> generation_;
 std::unique_ptr<ParticleEmissionOwner> emission_;
 ParticleCloudModelsV1 models_{};std::int32_t seed_{123456789};
 std::unique_ptr<ParticleCloudRuntimeV1> cloud_;
 std::shared_ptr<const ParticleAnimationResource> scalar_animation_;
 ParticleBirthRateBinding birth_;std::vector<ParticleGravitySceneV1> forces_;
 std::vector<ParticleGravityV1> force_models_;std::vector<std::uint32_t> bound_forces_;
 std::uint32_t emitter_node_{},emitter_instance_{},material_{};float radius_{};
 float camera_view_[16]{},camera_position_[3]{},uv_[8]{},source_uv_matrix_[16]{};
 std::array<float,16> emitter_world_{};ParticleBillboardBoundsV1 bounds_{};
 std::shared_ptr<Snapshot> draw_;std::string failure_;bool ready_{};std::int32_t source_start_{},source_end_{};
 std::uint32_t word(std::uint64_t p)const{if(p>image_.size||4>image_.size-p)throw std::runtime_error("Blood FX record range");std::uint32_t v;std::memcpy(&v,image_.bytes+p,4);return v;}
 std::string text(std::uint32_t p)const{if(!p||p>=image_.size)throw std::runtime_error("Blood FX string");auto b=reinterpret_cast<const char*>(image_.bytes+p);auto e=static_cast<const char*>(std::memchr(b,0,image_.size-p));if(!e)throw std::runtime_error("Blood FX unterminated string");return {b,e};}
 static int emitter(void*,ParticleGenerationOwner&,std::uint32_t type){return type==1?0:-2;}
 static int shape(void* p,ParticleGenerationOwner& o,const char* n,std::uint32_t value){auto& r=*static_cast<BloodResource*>(p);if(std::strcmp(n,"RadiusLength"))return -2;o.register_parameter(o.hash_name(n),&r.radius_);std::memcpy(&r.radius_,&value,4);return 0;}
 static int vector(void*,ParticleGenerationOwner& o,const char* n,const std::uint32_t v[3]){auto lease=o.parameter(n);if(lease)std::memcpy(lease.storage,v,12);return 0;}
 static int render_initialize(void* p,ParticleGenerationOwner& o,const ParticleEmitterInput&){auto& r=*static_cast<BloodResource*>(p);const auto maximum=o.generation().max_particles;if(maximum!=30){r.failure_="Required blood FX capacity domain";return -2;}
  // Original allocates maxParticles*4 vertices and maxParticles*6 indices.
  // Native retained CPU geometry is populated by the same source baker.
  auto s=std::make_shared<Snapshot>();s->geometry.positions.reserve(maximum*4);s->materials=r.scene_.materials;s->binding={r.material_};r.draw_=std::move(s);return 0;}
 int apply(ParticleSeed100* p,std::uint32_t count){
  int rc=dh2_particle_billboard_apply_v1(p,count,camera_position_,emitter_world_.data(),false,&bounds_);if(rc)return rc;
  ParticleBillboardBasisV1 basis{};rc=dh2_particle_billboard_basis_v1(&basis,camera_view_);if(rc)return rc;
  auto s=std::make_shared<Snapshot>();s->materials=scene_.materials;s->binding={material_};auto& g=s->geometry;g.id=input_.name;
  std::copy_n(bounds_.minimum,3,g.minimum);std::copy_n(bounds_.maximum,3,g.maximum);
  skinning::VisualAttributeV6 colors{1,4,{}},uvs{6,2,{}};
  skinning::VisualPrimitiveV6 primitive;primitive.material_symbol=scene_.materials.at(material_).id;primitive.collada_type=0;primitive.engine_type=3;primitive.attributes.fill(-1);primitive.attributes[2]=0;primitive.attributes[4]=1;
  for(unsigned i=0;i<count;++i){ParticleBillboardVertexV1 v[4]{};rc=dh2_particle_billboard_vertices_v1(v,&basis,p+i,uv_);if(rc)return rc;
   for(const auto& vertex:v){g.positions.push_back({vertex.position[0],vertex.position[1],vertex.position[2]});const auto c=vertex.color;colors.values.insert(colors.values.end(),{float((c>>16)&255),float((c>>8)&255),float(c&255),float(c>>24)});
    // Actual ProfileCOMMON_emul_VS: TextureMatrix0*vec4(TexCoord0,1,0).
    // Bake this source equation because the existing native shader uses w1.
    const float u=source_uv_matrix_[0]*vertex.uv[0]+source_uv_matrix_[4]*vertex.uv[1]+source_uv_matrix_[8];
    const float vcoord=source_uv_matrix_[1]*vertex.uv[0]+source_uv_matrix_[5]*vertex.uv[1]+source_uv_matrix_[9];
    uvs.values.insert(uvs.values.end(),{u,vcoord});}
   // Exact original twelve-byte template at99b85c, init656f2c/6569e8.
   for(unsigned index:{0u,1u,2u,0u,2u,3u})primitive.indices.push_back(i*4+index);
  }g.attributes.push_back(std::move(colors));g.attributes.push_back(std::move(uvs));g.primitives.push_back(std::move(primitive));draw_=std::move(s);return 0;
 }
public:
 BloodResource(std::shared_ptr<const std::vector<std::uint8_t>> b,CharacterBloodFxSceneServicesV2 s):bytes_(std::move(b)),services_(s){}
 bool initialize(std::string& error){try{
  if(dh2_bres_open(&image_,bytes_->data(),bytes_->size())!=resources::BresError::ok)throw std::runtime_error("Blood FX BRES");
  if(word(image_.root_offset+0x78)!=1||word(image_.root_offset+0x80)||word(image_.root_offset+0x90))throw std::runtime_error("Required particle resource family");
  if(!decode_particle_emitter(bytes_,0,input_,error))return false;
  if(input_.name!="IrrPCloud01-emitter")throw std::runtime_error("Required authored particle resource domain: "+input_.name);
  if(!scene::load_particle_scene_v1(image_,scene_,error))return false;
  unsigned found=0;auto vs=word(image_.root_offset+156);auto nodes=word(vs+12);
  // Both source blood resources consist of two actual authored root nodes.
  for(unsigned i=0;i<word(vs+8);++i){auto node=nodes+80*i;if(word(node+56))throw std::runtime_error("Required nested blood emitter resource");auto n=word(node+64),base=word(node+68);for(unsigned j=0;j<n;++j)if(word(base+8*j)==9){++found;emitter_instance_=word(base+8*j+4);auto uri=text(word(emitter_instance_+4));if(uri!="#"+input_.name)throw std::runtime_error("Blood emitter named lookup");auto id=text(word(node));unsigned matches=0;for(unsigned k=0;k<scene_.graph.size();++k)if(scene_.graph[k].id==id){emitter_node_=k;++matches;}if(matches!=1)throw std::runtime_error("Blood emitter same scene node");}}
  if(found!=1)throw std::runtime_error("Blood emitter instance count");
  if(word(emitter_instance_+12)!=1)throw std::runtime_error("Required blood material binding count");auto binding=word(emitter_instance_+16);if(word(binding))throw std::runtime_error("Required external blood material");auto material_uri=text(word(binding+4));if(material_uri.empty()||material_uri[0]!='#')throw std::runtime_error("Blood material URI");unsigned matches=0;for(unsigned i=0;i<scene_.materials.size();++i)if(scene_.materials[i].id==material_uri.substr(1)){material_=i;++matches;}if(matches!=1)throw std::runtime_error("Blood material same source table");
  // Source effect defaults are separate from material override parameters.
  // Both actual blood assets bind ProfileCOMMON_fx_particles_alpha; its
  // diffuse-sampler-matrix selects the authored atlas tile, not the whole image.
  if(scene_.materials.at(material_).id!="fx_particles_alpha")throw std::runtime_error("Required blood shader material domain");
  const auto* material_record=dh2_bres_library_item(&image_,resources::Library::material,material_);if(!material_record)throw std::runtime_error("Blood material source record");const auto mat=std::uint32_t(material_record-image_.bytes);if(word(mat+8))throw std::runtime_error("Required external blood effect");auto effect_uri=text(word(mat+12));if(effect_uri.empty()||effect_uri[0]!='#')throw std::runtime_error("Blood effect source URI");
  unsigned effect_matches=0;bool has_matrix=false,has_color=false;for(unsigned i=0;i<dh2_bres_library_count(&image_,resources::Library::effect);++i){const auto* record=dh2_bres_library_item(&image_,resources::Library::effect,i);const auto at=std::uint32_t(record-image_.bytes);if(text(word(at))!=effect_uri.substr(1))continue;++effect_matches;if(text(word(at+4))!="ProfileCOMMON_fx_particles_alpha")throw std::runtime_error("Required source blood shader profile");const auto n=word(at+16),base=word(at+20);for(unsigned j=0;j<n;++j){const auto p=base+24*j;auto name=text(word(p));if(name=="diffuse-sampler-matrix"){if(word(p+4)!=10||word(p+12)!=1)throw std::runtime_error("Blood effect matrix layout");for(unsigned k=0;k<16;++k){auto raw=word(word(p+20)+4*k);std::memcpy(source_uv_matrix_+k,&raw,4);if(!std::isfinite(source_uv_matrix_[k]))throw std::runtime_error("Blood effect nonfinite matrix");}has_matrix=true;}else if(name=="diffuse-color"){if(word(p+4)!=16||word(p+12)!=1)throw std::runtime_error("Blood effect diffuse color layout");for(unsigned k=0;k<4;++k){auto raw=word(word(p+20)+4*k);std::memcpy(scene_.materials[material_].color+k,&raw,4);}has_color=true;}}}
  if(effect_matches!=1||!has_matrix||!has_color)throw std::runtime_error("Required actual blood effect defaults");
  // Existing native material matrix remains identity: source UV transform is
  // now baked once. Transparent sampler is not an AlphaMap red-channel alias.
  if(!decode_particle_gravity_scene_v1(image_,scene_,forces_,error)||!particle_gravity_bindings_v1(image_,emitter_instance_,scene_,forces_,bound_forces_,error))return false;
  for(const auto& force:forces_)force_models_.push_back({force.strength,force.falloff,force.point_mode});
  // All source model parameter cells used in this authored domain are supplied
  // below; untouched diagnostic context cells never become gameplay defaults.
  ParticleContextSeed92 context{};generation_=ParticleGenerationOwner::create(context);if(!register_particle_cloud_models_v1(*generation_,models_))throw std::runtime_error("Blood parameter owner registration");
  const int initialized=initialize_particle_resource_v1(*generation_,input_,{this,emitter,shape},{this,vector,render_initialize});if(initialized)throw std::runtime_error(failure_.empty()?"Required blood resource initialization":failure_);
  const float center[3]{};if(dh2_particle_sphere_construct_v1(&models_.sphere,center,radius_,0))throw std::runtime_error("Blood source sphere constructor");
  dh2_particle_billboard_uv_v1(uv_);
  scalar_animation_=ParticleAnimationResource::create(bytes_->data(),bytes_->size(),error);if(!scalar_animation_)return false;
  if(scalar_animation_->track_count()!=1||*scalar_animation_->target(0)!=input_.name)throw std::runtime_error("Required blood parameter animation target");birth_={scalar_animation_,generation_->parameter("BirthRate"),0,0};
  if(!animation_.load(bytes_->data(),bytes_->size(),scene_,error,animation::MissingTargets::reject))return false;
  if(animation_.unbound)throw std::runtime_error("Blood unbound node animation");
  for(unsigned i=0;i<dh2_bres_library_count(&image_,resources::Library::animation);++i){assets::Animation a{};if(dh2_animation_open(&a,&image_,i,0)!=assets::Error::ok)throw std::runtime_error("Blood animation accessor");if(!i){source_start_=a.segment_start;source_end_=a.segment_end;}else{source_start_=std::min(source_start_,a.segment_start);source_end_=std::max(source_end_,a.segment_end);}auto type=dh2_animation_type(&a,0);if(type!=1&&type!=5&&type!=10&&type!=28&&type!=26)throw std::runtime_error("Required blood material/other animation continuation");}
  emission_=std::make_unique<ParticleEmissionOwner>(generation_);cloud_=std::make_unique<ParticleCloudRuntimeV1>(*emission_,models_,seed_,123456789,ParticleCloudRenderV1{[this](){return draw_?0:-2;},[this](ParticleSeed100* p,std::uint32_t n){return apply(p,n);}});if(cloud_->initialize())throw std::runtime_error("Blood cloud source initialize");ready_=true;return true;
 }catch(const std::exception& e){error=e.what();return false;}}
 std::int32_t start_ms()const noexcept override{return source_start_;}
 std::int32_t end_ms()const noexcept override{return source_end_;}
 bool sample_animation(std::int32_t ms,std::string& error)override{if(!ready_){error="Blood resource not initialized";return false;}if(!animation_.sample(scene_,ms,error)||birth_.sample_apply(ms)){if(error.empty())error="Blood source animation sample";return false;}return true;}
 bool scene_frame(std::int32_t absolute,std::int32_t,const std::array<float,16>& outer,std::string& error)override{
  if(!ready_||!services_.camera||!services_.driver_type){error="Required actual blood camera/driver owner";return false;}
  if(!services_.camera(services_.context,camera_view_,camera_position_,error))return false;std::uint32_t driver,color;if(!services_.driver_type(services_.context,driver,error))return false;if(particle_scene_color_v1(driver,{},color)){error="Required actual particle scene lighting lookup";return false;}
  emitter_world_=scene::multiply(outer,scene_.graph.at(emitter_node_).world);
  std::vector<std::array<float,16>> matrices;matrices.reserve(bound_forces_.size());for(auto i:bound_forces_)matrices.push_back(scene::multiply(outer,*forces_.at(i).matrix()));
  ParticleCloudFrameV1 frame{emitter_world_.data(),false,&color,{}};for(unsigned i=0;i<bound_forces_.size();++i)frame.forces.push_back({&force_models_.at(bound_forces_[i]),matrices[i].data()});ParticleSeed100 untouched{};int rc=cloud_->update(float(absolute)/1000.f,untouched,frame);if(rc){error="Blood source cloud update "+std::to_string(rc);return false;}return true;
 }
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
   s.particle_identity=reinterpret_cast<std::uintptr_t>(this);s.part=std::move(part);result.push_back(std::move(s));
  }out=std::move(result);return true;
 }
};
}
bool CharacterBloodFxFactoryV2::create(void* p,std::shared_ptr<const std::vector<std::uint8_t>> bytes,const scene::Scene&,std::shared_ptr<CharacterParticleFxResourceV2>& out,std::string& error){out.reset();if(!p||!bytes){error="Blood factory source borrow";return false;}auto r=std::make_shared<BloodResource>(std::move(bytes),static_cast<CharacterBloodFxFactoryV2*>(p)->services_);if(!r->initialize(error))return false;out=std::move(r);return true;}
}
