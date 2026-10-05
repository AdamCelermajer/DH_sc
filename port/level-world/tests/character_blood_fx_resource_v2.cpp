#include "../character_mesh_fx_owner_v1.hpp"
#include "../visual_fx_preload.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <filesystem>
#include <cstdio>
using namespace dh2;
using Raw=std::vector<std::uint8_t>;
static unsigned checks=0;
static void require(bool b,const std::string& m){++checks;if(!b)throw std::runtime_error(m);}
static Raw read(const std::string& p){std::ifstream f(p,std::ios::binary);if(!f)throw std::runtime_error(p);return {std::istreambuf_iterator<char>(f),{}};}
static data::Bytes bytes(const Raw& x){return {x.data(),x.size()};}
struct Providers {
 std::string assets;const scene::Scene* expected{};std::vector<fx::MeshFxOperationV1> delivered;unsigned reads{},fail_at{},file_opens{};bool enabled=true,inject_particle=false;
 character::DebugSwitches* debug{};fx::DebugModules* modules{};character::DebugFileServices24 files{};
 Providers(std::string a,const scene::Scene* s):assets(std::move(a)),expected(s){files={this,open,close};debug=dh2_character_debug_create();modules=dh2_fx_debug_modules_create(debug,&files);if(!debug||!modules)throw std::runtime_error("debug owner allocation");}
 ~Providers(){dh2_fx_debug_modules_destroy(modules);dh2_character_debug_destroy(debug);}
 static int open(void* p,const char* name,std::uintptr_t* out){auto& s=*static_cast<Providers*>(p);++s.file_opens;auto dir=s.assets+"/fx-owner-missing-debug";std::filesystem::create_directories(dir);auto path=dir+"/"+name;auto* f=std::fopen(path.c_str(),"rb");*out=reinterpret_cast<std::uintptr_t>(f);return 0;}
 static int close(void*,std::uintptr_t h){return std::fclose(reinterpret_cast<std::FILE*>(h));}
 static bool asset(void* p,const char* uri,Raw& out,std::string& error){auto& s=*static_cast<Providers*>(p);++s.reads;std::string path=uri?uri:"";auto at=path.find_last_of("/\\");try{out=read(s.assets+"/"+path.substr(at==std::string::npos?0:at+1));if(s.inject_particle){std::uint32_t root,one=1;std::memcpy(&root,out.data()+32,4);std::memcpy(out.data()+root+120,&one,4);}return true;}catch(const std::exception& e){error=e.what();return false;}}
 static bool service(void* p,fx::MeshFxRequestV1& q,std::string& error){auto& s=*static_cast<Providers*>(p);s.delivered.push_back(q.operation);require(q.live_scene==s.expected,"different live Scene projection");if(s.fail_at&&s.delivered.size()==s.fail_at){error="required fixture failure";return false;}
  using O=fx::MeshFxOperationV1;
  if(q.operation==O::debug_load){if(dh2_character_debug_load(s.debug,&s.files)!=1){error="genuine debug load failed";return false;}}
  else if(q.operation==O::module_enabled){if(!s.enabled)q.result=0;else if(dh2_fx_debug_module_get(&q.result,s.modules,q.text)!=1){error="genuine debug module failed";return false;}}
  else if(q.operation==O::set_switch||q.operation==O::instance_switch){require(q.text&&std::strcmp(q.text,"isTracingAnim_FX")==0,"source tracing name");if(dh2_character_debug_get(&q.result,s.debug,q.text,&s.files)!=1){error="genuine debug switch failed";return false;}}
  else if(q.operation==O::anchor_position){require(q.identity==0x100000001ull,"64-bit anchor identity");std::copy_n(s.expected->graph.at(0).translation,3,q.point);}
  else if(q.operation==O::anchor_rotation){math::Matrix4f m{};std::copy_n(s.expected->graph.at(0).world.data(),16,m.m);math::Vector3f v;dh2_matrix_rotation_degrees(&v,&m);float factor;std::uint32_t bits=0x3c8efa35;std::memcpy(&factor,&bits,4);q.point[0]=v.x*factor;q.point[1]=v.y*factor;q.point[2]=v.z*factor;}
  else if(q.operation==O::anchor_scale)std::copy_n(s.expected->graph.at(0).scale,3,q.point);
  else if(q.operation==O::floor_normal){q.point[0]=q.point[1]=0;q.point[2]=1;}
  else q.result=0;
  return true;
 }
};
#include "../character_blood_fx_resource_v2.hpp"
#include "../character_mesh_fx_owner_v2.hpp"
#include "../../engine-animation/particle_emission.hpp"
static bool camera(void*,float* v,float* p,std::string&){const float m[16]={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};std::copy_n(m,16,v);p[0]=17;p[1]=29;p[2]=500;return true;}
static bool driver(void*,std::uint32_t& v,std::string&){v=8;return true;}
int main(int argc,char**argv){try{require(argc==3,"two actual blood resources");std::string e;fx::CharacterBloodFxFactoryV2 factory({nullptr,camera,driver});auto f=factory.factory();scene::Scene live;unsigned frames=0,vertices=0;std::vector<skinning::VisualDrawPartV6> retained;
for(int a=1;a<3;++a){auto raw=std::make_shared<const Raw>(read(argv[a]));std::shared_ptr<fx::CharacterParticleFxResourceV2> resource;require(f.create(f.context,raw,live,resource,e),e);require(resource&&resource->end_ms()>0,"authored source duration");const std::array<float,16> outer={1,0,0,0,0,1,0,0,0,0,1,0,17,29,41,1};bool positive=false;
for(int ms=0;ms<3000;ms+=16){require(resource->sample_animation(std::min(ms,resource->end_ms()),e),e);require(resource->scene_frame(ms,16,outer,e),e);std::vector<skinning::VisualDrawPartV6> parts;require(resource->draw_parts(parts,e),e);for(auto& p:parts){positive=true;++frames;vertices+=p.positions.size();require(p.retention&&p.material_table&&p.materials&&p.geometry,"retained actual resource");require(p.positions.size()%4==0,"source billboard quads");require(p.geometry->primitives[0].indices.size()==p.positions.size()/4*6,"source index count");require(p.geometry->attributes.size()==2,"source color/UV streams");const auto& uv=p.geometry->attributes[1].values;const float left=a==1?.25f:.125f;require(uv[0]==left&&uv[1]==0&&uv[4]==left+.125f&&uv[5]==.125f,"actual distinct source atlas tile and shader z1 equation");require(p.material_table->at(0).diffuse=="atlas_fx_particles_001.tga","actual source diffuse atlas");for(auto x:p.positions)require(std::isfinite(x[0])&&std::isfinite(x[1])&&std::isfinite(x[2]),"finite source world particles");retained.push_back(p);}}
require(positive,"actual blood emits positive particles");bool done=false;require(resource->completed(done,e)&&done,"actual particles expire");}
const std::string table="port/game-data/reference/effects-tables";auto a=read(table+"/effects_pyarray.bin"),b=read(table+"/effects_pyarraynames.bin"),c=read(table+"/effects_pystructnames.bin"),d=read(table+"/effects_dictionary_pyarraynames.bin"),x=read(table+"/effects_dictionary_pyarray.bin");data::EffectsTables tables;require(tables.load(bytes(a),bytes(b),bytes(c),bytes(d),bytes(x),e),e);auto raw=read("port/android-native/app/src/main/assets/models/prince_modular.bdae");resources::BresView view{};require(dh2_bres_open(&view,raw.data(),raw.size())==resources::BresError::ok,"Actual prince");require(scene::load(view,live,e),e);Providers providers{".local-inputs/combat-hit-fx-v1",&live};unsigned manager_draws=0;
for(int set:{79,80}){fx::CharacterMeshFxOwnerV2 manager(tables.borrow(),live,{&providers,Providers::asset},{&providers,Providers::service},factory.factory());require(manager.precache_libraries(e),e);const float pos[3]={17,29,41},rot[3]={.1f,.2f,.3f};std::uintptr_t id=0;require(manager.play_set(set,pos,rot,0,&id,e)&&id,e);for(int ms=0;ms<3000;ms+=16){require(manager.scene_frame(ms,16,e)&&manager.manager_frame(16,e),e);std::vector<skinning::VisualDrawPartV6> parts;require(manager.draw_parts(parts,e),e);manager_draws+=parts.size();}auto states=manager.views();require(states.size()==1&&states[0].pooled&&states[0].finished,"actual blood source completion/pool");auto reads=providers.reads;std::uintptr_t warm=0;require(manager.play_set(set,pos,rot,0,&warm,e)&&warm==id&&providers.reads==reads,"actual blood warm reuse");}
require(manager_draws>0,"actual manager positive blood draw");
for(auto& p:retained)require(p.geometry&&!p.geometry->positions.empty(),"snapshot survives resource destruction");std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"positive_frames\":"<<frames<<",\"vertices\":"<<vertices<<",\"gpu_submission\":false}"<<std::endl;return 0;}catch(const std::exception&e){std::cerr<<e.what()<<std::endl;return 1;}}
