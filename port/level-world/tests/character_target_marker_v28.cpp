#include "../character_target_marker_v28.hpp"
#include <cmath>
#include "../character_authored_fx_forces_v4.hpp"
#include "../character_authored_resource_v6.hpp"
#include "../authored_fx_geometry_packet_v7.hpp"
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
struct Services{std::string directory;character::DebugSwitches* debug{};fx::DebugModules* modules{};character::DebugFileServices24 files{};float pos[3]{113,227,331};bool dead{};unsigned reads{};std::int32_t interaction{8};
 explicit Services(std::string p):directory(std::move(p)){files={this,[](void*,const char*,std::uintptr_t* h){*h=0;return 0;},[](void*,std::uintptr_t){return 0;}};debug=dh2_character_debug_create();modules=dh2_fx_debug_modules_create(debug,&files);check(debug&&modules,"genuine native debug/module owners");}
 ~Services(){dh2_fx_debug_modules_destroy(modules);dh2_character_debug_destroy(debug);}
 static bool asset(void* p,const char* uri,Raw& out,std::string& e){auto& s=*static_cast<Services*>(p);++s.reads;try{const std::string path=uri;out=read(s.directory+"/"+path.substr(path.find_last_of("/\\")+1));return true;}catch(const std::exception& x){e=x.what();return false;}}
 static bool invoke(void* p,fx::MeshFxRequestV1& q,std::string& e){auto& s=*static_cast<Services*>(p);using O=fx::MeshFxOperationV1;
  if(q.operation==O::debug_load)return dh2_character_debug_load(s.debug,&s.files)==1;
  if(q.operation==O::module_enabled)return dh2_fx_debug_module_get(&q.result,s.modules,q.text)==1;
  if(q.operation==O::set_switch||q.operation==O::instance_switch)return dh2_character_debug_get(&q.result,s.debug,q.text,&s.files)==1;
  if(q.operation==O::floor_normal){std::fill_n(q.point,3,0.f);return true;} // declared no-hit floor fixture
  if(q.identity!=0x100000001ull&&q.identity!=0x100000002ull){e="fixture required exact anchor identity";return false;}
  if(q.operation==O::anchor_dead)q.result=s.dead;
  else if(q.operation==O::anchor_disabled||q.operation==O::anchor_stationary)q.result=0;
  else if(q.operation==O::anchor_position)std::copy_n(s.pos,3,q.point);
  else if(q.operation==O::anchor_rotation){q.point[0]=.1f;q.point[1]=.2f;q.point[2]=.3f;}
  else if(q.operation==O::anchor_scale){q.point[0]=1.2f;q.point[1]=.9f;q.point[2]=1.1f;}
  else {e="unsupported declared fixture FX service";return false;}return true;
 }
};

static bool interaction(void* p,std::uintptr_t target,std::uintptr_t player,std::int32_t& out,std::string&){
 check(player==0x100000003ull,"Same player identity");check(target==0x100000001ull||target==0x100000002ull,"Same target identity");
 out=static_cast<Services*>(p)->interaction;return true;
}
static bool tooltip(void*,std::uintptr_t,std::uintptr_t,std::string&){return true;}
int main(int argc,char** argv){try{
 check(argc==2,"Actual cache directory");Services s(argv[1]);data::EffectsTables tables;std::string e;
 const auto a=read(s.directory+"/effects_pyarray.bin"),b=read(s.directory+"/effects_pyarraynames.bin"),c=read(s.directory+"/effects_pystructnames.bin"),d=read(s.directory+"/effects_dictionary_pyarraynames.bin"),f=read(s.directory+"/effects_dictionary_pyarray.bin");
 auto view=[](const Raw& r){return data::Bytes{r.data(),r.size()};};check(tables.load(view(a),view(b),view(c),view(d),view(f),e),e);
 fx::CharacterAuthoredFxForceFactoryOwnerV4 forces;fx::CharacterAuthoredResourceFactoryV6 resources({nullptr,camera,driver},forces.factory());scene::Scene live;
 fx::CharacterMeshFxOwnerV4 effects(tables.borrow(),live,{&s,Services::asset},{&s,Services::invoke},resources.factory());check(effects.precache_libraries(e),e);
 character::CharacterTargetMarkerV28 marker(effects,tables.borrow(),0x100000003ull,{&s,interaction,tooltip});
 check(marker.initialize(e),e);check(!marker.initialize(e),"Init prefix cannot replay");
 check(marker.selected()==-1,"Ctor source selected=-1");
 unsigned nonnull=0;for(auto id:marker.circles())nonnull+=id!=0;check(nonnull==4,"Actual nine slots, four declared resources");
 check(s.reads==4,"No fabricated assets for NULL slots");
 for(const auto& r:effects.views()){check(r.state.loop==-1&&r.state.looping==1&&!r.state.visible&&!r.pooled,"Grab override retained/hidden");check(r.state.position[0]==0&&r.state.position[1]==0&&r.state.position[2]==0,"Actual source zero offset");}
 check(marker.update(0x100000001ull,0,e),e);check(marker.selected()==8,"Actual attack indicator slot");
 auto snapshot=[&](int ms){check(effects.scene_frame(ms,33,e),e);check(effects.manager_frame(33,e),e);std::vector<fx::CharacterFxMeshDrawSourceV4> meshes;check(effects.mesh_draw_sources_v4(meshes,e),e);check(!meshes.empty(),"Authored target triangles");for(auto& m:meshes){check(m.fx_identity==marker.circles()[8],"Selected same target effect identity");fx::AuthoredFxGeometryPacketV7 packet;check(fx::authored_fx_geometry_packet_v7(m.part,m.material,packet,e),e);check(!packet.vertices.empty()&&!packet.indices.empty(),"Actual authored mesh packet");}return meshes;};
 auto first=snapshot(1000);check(first.front().part.world[12]!=0,"Live target placement");
 for(int ms=1033;ms<=4000;ms+=33){check(marker.update(0x100000001ull,0,e),e);snapshot(ms);}
 auto before=effects.views();auto old=std::find_if(before.begin(),before.end(),[&](const auto& r){return r.identity==marker.circles()[8];});check(old!=before.end()&&old->state.visible&&old->state.loop==-1&&!old->pooled&&!old->finished,"Persistent ring survives animation periods");
 const auto current=old->current_ms;check(marker.update(0x100000002ull,0,e),e);auto views=effects.views();auto changed=std::find_if(views.begin(),views.end(),[&](const auto& r){return r.identity==marker.circles()[8];});check(changed->state.anchor==0x100000002ull&&changed->current_ms==current,"Same-type retarget does not restart authored clock");
 auto previous=snapshot(4033);s.pos[0]+=37;check(marker.update(0x100000002ull,0,e),e);auto moved=snapshot(4033);check(std::abs(moved.front().part.world[12]-previous.front().part.world[12]-37)<0.0002f,"Target movement applied exactly once");
 check(marker.update(0,0,e),e);check(marker.selected()==-1,"Source target clear removes ring");std::vector<fx::CharacterFxMeshDrawSourceV4> hidden;check(effects.mesh_draw_sources_v4(hidden,e)&&hidden.empty(),"No stale dead/cleared target ring draw");
 check(marker.update(0,0x100000001ull,e),e);check(marker.selected()==8,"Actual OOI fallback");
 s.interaction=0;check(marker.update(0x100000001ull,0,e),e);check(marker.selected()==0,"Source interaction-family switch");views=effects.views();unsigned visible=0;for(const auto& r:views)visible+=r.state.visible!=0;check(visible==1,"Old indicator hidden on type change");
 s.interaction=10;check(!marker.update(0x100000001ull,0,e)&&!e.empty(),"Unavailable source array family remains required");
 check(marker.release(e),e);check(marker.selected()==-1,"Source release clear");for(auto id:marker.circles())check(!id,"Same receivers returned to source pool");for(const auto& r:effects.views())check(r.pooled&&!r.state.visible&&!r.state.anchor,"Actual marker cleanup");
 const auto reads=s.reads;s.interaction=8;
 {character::CharacterTargetMarkerV28 reused(effects,tables.borrow(),0x100000003ull,{&s,interaction,tooltip});check(reused.initialize(e),e);check(s.reads==reads&&effects.warm_reuses()==4,"Actual marker pool reused all four resources without reread");check(reused.update(0x100000001ull,0,e),e);check(effects.scene_frame(4100,33,e)&&effects.manager_frame(33,e),e);}
 for(const auto& r:effects.views())check(r.pooled&&!r.state.visible&&!r.state.anchor,"Destructor releases while provider lease is live");
 std::cout<<"PASS "<<checks<<" checks; actual marker resources=4; target movement=37 once; native GPU packet closure; provider fixtures=true; live gameplay=false"<<std::endl;return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}}
