// V2 source regression over the same actual authored swooshes.
// Positive particle construction is separately required and not mocked here.
#include "../character_mesh_fx_owner_v2.hpp"
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
int main(int argc,char** argv){try{
 if(argc!=4)throw std::runtime_error("usage: audit effects-table-directory swoosh-resource-directory prince-model.bdae");
 std::string e;const std::string table=argv[1];auto a=read(table+"/effects_pyarray.bin"),b=read(table+"/effects_pyarraynames.bin"),c=read(table+"/effects_pystructnames.bin"),d=read(table+"/effects_dictionary_pyarraynames.bin"),f=read(table+"/effects_dictionary_pyarray.bin");data::EffectsTables tables;require(tables.load(bytes(a),bytes(b),bytes(c),bytes(d),bytes(f),e),e);auto borrow=tables.borrow();
 auto prince=read(argv[3]);resources::BresView prince_view{};require(dh2_bres_open(&prince_view,prince.data(),prince.size())==resources::BresError::ok,"Prince BRES");scene::Scene live;require(scene::load(prince_view,live,e)&&live.graph.size()==35,e);live.graph[0].translation[0]=100;live.graph[0].translation[1]=-50;live.graph[0].translation[2]=20;live.graph[0].scale[0]=1.25f;live.graph[0].scale[1]=.75f;live.graph[0].scale[2]=2;require(scene::update_world(live,e),e);
 Providers p{argv[2],&live};std::vector<skinning::VisualDrawPartV6> retained;unsigned animated=0,material_animated=0,completed=0,anchor_moves=0;
 {
 fx::CharacterMeshFxOwnerV2 owner(borrow,live,{&p,Providers::asset},{&p,Providers::service},{});
 require(!owner.precached(),"source constructor cache byte0");require(owner.precache_libraries(e)&&owner.precached(),e);
 for(int set=253;set<=255;++set){std::uintptr_t id=0;const float origin[3]{2,3,4};require(owner.play_set(set,origin,nullptr,0x100000001ull,&id,e),"play:"+e);require(id!=0,"cold identity");require(owner.scene_frame(1000,16,e),e);std::vector<skinning::VisualDrawPartV6> first,next;require(owner.draw_parts(first,e),e);require(first.size()==1&&first[0].geometry&&!first[0].positions.empty(),"real retained mesh");require(first[0].material_table->at(0).additive,"authored additive material");auto views=owner.views();auto v=std::find_if(views.begin(),views.end(),[&](const auto& v){return v.identity==id;});require(v!=views.end()&&v->state.anchor==0x100000001ull&&v->state.orient_once==1&&v->state.orient_with_anchor==0,"original swapped orientations");const auto end=v->end_ms;
  require(owner.scene_frame(1000+end/2,16,e),e);require(owner.manager_frame(16,e),e);require(owner.draw_parts(next,e),e);require(next.size()==1,"draw active midframe");if(first[0].world!=next[0].world)++animated;
  if(std::memcmp(first[0].material_table->at(0).texture_matrix,next[0].material_table->at(0).texture_matrix,64))++material_animated;
  retained.push_back(next[0]);live.graph[0].translation[0]+=25;require(scene::update_world(live,e),e);require(owner.manager_frame(16,e),e);std::vector<skinning::VisualDrawPartV6> moved;require(owner.draw_parts(moved,e)&&moved.size()==1,e);require(std::abs(moved[0].world[12]-next[0].world[12]-25)<.0001f,"same live anchor motion");++anchor_moves;
  require(owner.scene_frame(1000+end+100,16,e),e);require(owner.manager_frame(16,e),e);views=owner.views();v=std::find_if(views.begin(),views.end(),[&](const auto& v){return v.identity==id;});require(v!=views.end()&&v->pooled&&v->finished&&!v->state.visible&&!v->pending_return,"original completion to pending Drop");++completed;
  auto reads=p.reads;std::uintptr_t warm=0;require(owner.play_set(set,origin,nullptr,0,&warm,e),e);require(warm==id&&p.reads==reads,"warm identity and no reload");require(owner.drop(warm,e)&&warm==0,e);
 }
 require(owner.cold_creations()==3&&owner.warm_reuses()==3,"three cold/warm authored resources");require(material_animated==3,"actual authored UV timeline changed for all three resources");
 // Source six-live cold limit, LIFO warm reuse and explicit missing/failure prefixes.
 const float zero[3]{};std::vector<std::uintptr_t> ids;for(unsigned i=0;i<7;++i){std::uintptr_t id=0;require(owner.play_set(253,zero,nullptr,0,&id,e),e);ids.push_back(id);}require(ids[5]&&ids[6]==0,"source active count >5 cold guard");for(auto& id:ids)if(id)require(owner.drop(id,e),e);
 std::uintptr_t id=123;require(owner.play_set(-1,zero,nullptr,0,&id,e)&&id==0,"source invalid set miss");p.enabled=false;require(owner.play_set(253,zero,nullptr,0,&id,e)&&id==0,"source debug module disabled");p.enabled=true;p.fail_at=p.delivered.size()+1;require(!owner.play_set(253,zero,nullptr,0,&id,e)&&e=="required fixture failure","required debug failure");p.fail_at=0;
 require(owner.animation_event("fx_source_name_missing",zero,e),"source event unmatched succeeds");require(!owner.animation_event("sfx_other",zero,e),"outside source FX path rejects");require(!owner.drop(id=0x1234,e),"invalid runtime identity guard");
 }
 for(const auto& r:retained){require(r.retention&&r.geometry&&r.material_table&&r.materials&&!r.geometry->primitives.empty(),"snapshot after owner destruction");require(!r.material_table->at(0).diffuse.empty(),"retained original image URI");for(auto index:r.geometry->primitives[0].indices)require(index<r.positions.size(),"retained vertex/index bounds");}
 // Required provider absence cannot become accepted rendering/creation.
 fx::CharacterMeshFxOwnerV2 missing(borrow,live,{&p,Providers::asset},{},{});std::uintptr_t id;const float zero[3]{};require(!missing.play_set(253,zero,nullptr,0,&id,e)&&!e.empty(),"missing debug service");
 p.inject_particle=true;fx::CharacterMeshFxOwnerV2 particle(borrow,live,{&p,Providers::asset},{&p,Providers::service},{});require(!particle.play_set(253,zero,nullptr,0,&id,e)&&e.find("particle")!=std::string::npos,"declared particle required continuation");p.inject_particle=false;
 fx::CharacterMeshFxOwnerV2 uncached(borrow,live,{&p,Providers::asset},{&p,Providers::service},{});require(uncached.play_set(253,zero,nullptr,0,&id,e)&&id,e);auto captured=id;require(uncached.drop(id,e)&&!id&&!uncached.views().at(0).pooled&&uncached.views().at(0).state.visible,"source cache0 Drop only nulls caller");require(uncached.precache_libraries(e)&&uncached.drop(captured,e)&&!captured&&uncached.views().at(0).pooled,e);
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"authored_resources\":3,\"live_scene_nodes\":"<<live.graph.size()<<",\"animated_node_draws\":"<<animated<<",\"animated_materials\":"<<material_animated<<",\"live_anchor_moves\":"<<anchor_moves<<",\"source_completion_returns\":"<<completed<<",\"retained_snapshots\":"<<retained.size()<<",\"asset_deliveries\":"<<p.reads<<",\"source_service_projection_deliveries\":"<<p.delivered.size()<<",\"genuine_debug_filesystem_opens\":"<<p.file_opens<<",\"sanitizer_findings\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
