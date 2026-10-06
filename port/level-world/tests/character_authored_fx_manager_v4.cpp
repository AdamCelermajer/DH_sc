#include "../character_mesh_fx_owner_v4.hpp"
#include "../character_authored_fx_forces_v4.hpp"
#include "../visual_fx_preload.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
using namespace dh2;
using Raw=std::vector<std::uint8_t>;
static unsigned checks;
static void check(bool b,const std::string& e){++checks;if(!b)throw std::runtime_error(e);}
static Raw read(const std::string& p){std::ifstream f(p,std::ios::binary);if(!f)throw std::runtime_error(p);return {std::istreambuf_iterator<char>(f),{}};}
static bool camera(void*,float* m,float* p,std::string&){const float id[16]{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};std::copy_n(id,16,m);p[0]=17;p[1]=29;p[2]=500;return true;}
static bool driver(void*,std::uint32_t& v,std::string&){v=8;return true;}
struct Services{std::string directory;character::DebugSwitches* debug{};fx::DebugModules* modules{};character::DebugFileServices24 files{};float pos[3]{113,227,331};bool dead{};unsigned reads{};
 explicit Services(std::string p):directory(std::move(p)){files={this,[](void*,const char*,std::uintptr_t* h){*h=0;return 0;},[](void*,std::uintptr_t){return 0;}};debug=dh2_character_debug_create();modules=dh2_fx_debug_modules_create(debug,&files);check(debug&&modules,"genuine native debug/module owners");}
 ~Services(){dh2_fx_debug_modules_destroy(modules);dh2_character_debug_destroy(debug);}
 static bool asset(void* p,const char* uri,Raw& out,std::string& e){auto& s=*static_cast<Services*>(p);++s.reads;try{const std::string path=uri;out=read(s.directory+"/"+path.substr(path.find_last_of("/\\")+1));return true;}catch(const std::exception& x){e=x.what();return false;}}
 static bool invoke(void* p,fx::MeshFxRequestV1& q,std::string& e){auto& s=*static_cast<Services*>(p);using O=fx::MeshFxOperationV1;
  if(q.operation==O::debug_load)return dh2_character_debug_load(s.debug,&s.files)==1;
  if(q.operation==O::module_enabled)return dh2_fx_debug_module_get(&q.result,s.modules,q.text)==1;
  if(q.operation==O::set_switch||q.operation==O::instance_switch)return dh2_character_debug_get(&q.result,s.debug,q.text,&s.files)==1;
  if(q.operation==O::floor_normal){std::fill_n(q.point,3,0.f);return true;} // declared no-hit floor fixture
  if(q.identity!=0x100000001ull){e="fixture required exact anchor identity";return false;}
  if(q.operation==O::anchor_dead)q.result=s.dead;
  else if(q.operation==O::anchor_disabled||q.operation==O::anchor_stationary)q.result=0;
  else if(q.operation==O::anchor_position)std::copy_n(s.pos,3,q.point);
  else if(q.operation==O::anchor_rotation){q.point[0]=.1f;q.point[1]=.2f;q.point[2]=.3f;}
  else if(q.operation==O::anchor_scale){q.point[0]=1.2f;q.point[1]=.9f;q.point[2]=1.1f;}
  else {e="unsupported declared fixture FX service";return false;}return true;
 }
};
int main(int argc,char** argv){try{check(argc==2,"actual cache directory required");Services s(argv[1]);data::EffectsTables tables;std::string e;
 const auto a=read(s.directory+"/effects_pyarray.bin"),b=read(s.directory+"/effects_pyarraynames.bin"),c=read(s.directory+"/effects_pystructnames.bin"),d=read(s.directory+"/effects_dictionary_pyarraynames.bin"),f=read(s.directory+"/effects_dictionary_pyarray.bin");
 auto view=[](const Raw& r){return data::Bytes{r.data(),r.size()};};check(tables.load(view(a),view(b),view(c),view(d),view(f),e),e);
 fx::CharacterAuthoredFxForceFactoryOwnerV4 forces;fx::CharacterAuthoredParticleFxFactoryV4 resources({nullptr,camera,driver},forces.factory());scene::Scene live;
 fx::CharacterMeshFxOwnerV4 owner(tables.borrow(),live,{&s,Services::asset},{&s,Services::invoke},resources.factory());check(owner.precache_libraries(e),e);
 const float zero[3]{};std::uintptr_t id=0;check(owner.play_set(164,zero,nullptr,0x100000001ull,&id,e)&&id,e);check(s.reads==1,"exact authored resource loaded once");bool particles=false;unsigned mesh_frames=0;
 for(int ms=0;ms<2500;ms+=16){s.pos[0]+=0.25f;check(owner.scene_frame(ms,16,e),e);check(owner.manager_frame(16,e),e);std::vector<fx::CharacterFxMeshDrawSourceV4> meshes;check(owner.mesh_draw_sources_v4(meshes,e),e);if(!meshes.empty()){check(meshes.size()==3,"all authored mesh receivers");++mesh_frames;for(auto& m:meshes)check(m.fx_identity==id&&m.source_texture_matrix68,"same manager FX identity and material owner");}std::vector<fx::CharacterParticleDrawSourceV3> cloud;check(owner.particle_draw_sources_v3(cloud,e),e);particles|=!cloud.empty();}
 check(particles&&mesh_frames>0,"whole manager produces both authored domains");auto states=owner.views();check(states.size()==1&&states[0].pooled&&states[0].finished,"source timeline/completion/pool");auto reads=s.reads;std::uintptr_t warm=0;check(owner.play_set(164,zero,nullptr,0x100000001ull,&warm,e)&&warm==id&&s.reads==reads,"same warm source resource reused");
 check(owner.scene_frame(2512,16,e)&&owner.manager_frame(16,e),e);std::vector<fx::CharacterFxMeshDrawSourceV4> meshes;check(owner.mesh_draw_sources_v4(meshes,e)&&meshes.size()==3,e);
 s.dead=true;check(owner.manager_frame(16,e),e);check(owner.views()[0].state.anchor==0,"source dead anchor releases without replacing FX");
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"mesh_frames\":"<<mesh_frames<<",\"anchor_fixture\":true,\"live_binding\":false}"<<std::endl;return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}}
