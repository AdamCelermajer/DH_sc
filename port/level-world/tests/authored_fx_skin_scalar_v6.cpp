#include "../character_authored_resource_v6.hpp"
#include "../character_authored_fx_forces_v4.hpp"
#include "../source_fx_node_matrix_v4.hpp"
#include "../../engine-skinning/skinning.hpp"
#include "../../engine-animation/particle_scalar_animation_v6.hpp"
#include "../../engine-animation/particle_cloud_runtime_v1.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <map>
using namespace dh2;
static unsigned checks;
static void check(bool condition,const char* what){++checks;if(!condition)throw std::runtime_error(what);}
static bool camera(void*,float* m,float* p,std::string&){const float id[]{1.f,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};std::copy_n(id,16,m);p[0]=17;p[1]=29;p[2]=500;return true;}
static bool driver(void*,std::uint32_t& out,std::string&){out=8;return true;}
static std::shared_ptr<const std::vector<std::uint8_t>> read(const std::string& name){std::ifstream f(name,std::ios::binary);check(bool(f),"Actual cache resource");return std::make_shared<const std::vector<std::uint8_t>>(std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>());}
int main(int argc,char** argv){try{if(argc!=2)return 1;const std::string dir=argv[1];unsigned moved=0;
 for(unsigned index:{16u,17u,18u,20u,27u}){const auto name=dir+"/asset_"+(index<10?"0":"")+std::to_string(index)+".bdae";auto raw=read(name);fx::CharacterAuthoredFxForceFactoryOwnerV4 forces;fx::CharacterAuthoredResourceFactoryV6 factory({nullptr,camera,driver},forces.factory());auto api=factory.factory();std::shared_ptr<fx::CharacterParticleFxResourceV2> base;scene::Scene unused;std::string e;check(api.create(api.context,raw,unused,base,e),e.c_str());auto resource=std::dynamic_pointer_cast<fx::CharacterAuthoredCompositeFxResourceV4>(base);check(bool(resource),"Same composite");math::Matrix4f outer{};float p[]{113,227,331},r[]{.1f,.2f,.3f},s[]{1.2f,.9f,1.1f};fx::source_fx_trs_matrix_v4(outer,p,r,s);resources::BresView image{};check(dh2_bres_open(&image,raw->data(),raw->size())==resources::BresError::ok,"BRES");std::map<std::uint64_t,std::vector<std::array<float,3>>> first;bool changed=false;
  for(int ms:{0,64,256,512}){check(resource->sample_animation(std::min(ms,resource->end_ms()),e)&&resource->source_scene_frame_v4(ms,32,outer,e),e.c_str());std::vector<fx::CharacterFxMeshDrawSourceV4> draws;check(resource->mesh_draw_sources_v4(draws,e),e.c_str());
   for(const auto& draw:draws){const auto& live=*draw.scene;auto instance=std::find_if(live.instances.begin(),live.instances.end(),[&](const auto& x){return x.node_index==draw.node;});check(instance!=live.instances.end(),"Same source instance");if(instance->controller<0)continue;skinning::Skin skin;check(skinning::load(image,unsigned(instance->controller),live,skin,e),e.c_str());assets::Mesh mesh{};check(dh2_mesh_open(&mesh,&image,skin.geometry)==assets::Error::ok,"Same controller geometry");assets::Primitive primitive{};check(dh2_mesh_primitive(&mesh,draw.primitive,&primitive)==assets::Error::ok,"Primitive");assets::Attribute attr{};check(dh2_mesh_attribute(&mesh,primitive.attributes[0],&attr)==assets::Error::ok&&attr.components==3,"Actual rest position stream");std::vector<std::array<float,3>> rest(mesh.vertices);for(unsigned i=0;i<mesh.vertices;++i)check(dh2_attribute_read(&attr,i,rest[i].data()),"Rest vertex");std::vector<skinning::Matrix> palette;std::vector<std::array<float,3>> expected;check(skinning::palette(skin,live,palette,e)&&skinning::positions(skin,palette,rest,expected,e),e.c_str());check(expected.size()==draw.part.positions.size(),"Source skin count");for(unsigned i=0;i<expected.size();++i)check(!std::memcmp(expected[i].data(),draw.part.positions[i].data(),12),"Source palette position identity");check(!std::memcmp(outer.m,draw.part.world.data(),64),"Skin outer applied once");auto& initial=first[(std::uint64_t(draw.node)<<32)|draw.primitive];if(initial.empty())initial=expected;else if(initial.size()==expected.size()&&std::memcmp(initial.data(),expected.data(),expected.size()*12))changed=true;
   }
  }if(changed)++moved;
 }
 check(moved>0,"Authored animated skin positions changed");
 auto raw=read(dir+"/asset_27.bdae");std::string e;auto scalar=animation::ParticleScalarAnimationResourceV6::create(raw->data(),raw->size(),e);check(bool(scalar),e.c_str());animation::ParticleContextSeed92 context{};auto generation=animation::ParticleGenerationOwner::create(context);animation::ParticleCloudModelsV1 models{};check(animation::register_particle_cloud_models_v1(*generation,models),"Actual model parameter registry");unsigned spin_tracks=0;
 for(unsigned i=0;i<scalar->track_count();++i){const auto* name=scalar->parameter_name(i);if(std::strcmp(name,"SpinPhase")&&std::strcmp(name,"SpinPhaseVariation"))continue;++spin_tracks;auto lease=generation->parameter(name);float* expected=std::strcmp(name,"SpinPhase")?&models.spin.phase_variation:&models.spin.phase;check(lease.storage==expected,"Same model storage identity");animation::ParticleScalarBindingV6 binding{scalar,lease,i,0};for(int ms:{0,64,256,512,0}){float value{};int cursor=binding.cursor;check(!scalar->sample(i,ms,cursor,value),"Actual scalar sampling");check(!binding.sample_apply(ms),"Exact parameter application");check(!std::memcmp(&value,expected,4),"Source raw cell write");check(binding.cursor==cursor,"Same source cursor");}auto bad=binding;bad.parameter.storage=&models.life.life;float before=models.life.life;check(bad.sample_apply(64)<0&&!std::memcmp(&before,&models.life.life,4),"Foreign storage rejected before mutation");}
 check(spin_tracks==2,"Actual two DeathClaw spin tracks");std::cout<<"PASS "<<checks<<" checks; moving_skin_resources "<<moved<<"; same_model_spin_tracks "<<spin_tracks<<"; GPU=false live=false\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
