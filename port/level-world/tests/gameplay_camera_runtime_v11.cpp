#include "../gameplay_camera_runtime_v11.hpp"
#include <fstream>
#include <iterator>
#include <iostream>
#include <cassert>
#include <cmath>
using namespace dh2;
static std::vector<std::uint8_t> read(const std::string& path){std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error("Required real cache "+path);return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char** argv){try{
 if(argc!=2)return 2;const std::string path=argv[1];std::string e;std::vector<std::vector<std::uint8_t>> data;data.reserve(8);
 auto bytes=[&](const char* name){data.push_back(read(path+"/"+name));const auto& b=data.back();return data::Bytes{b.data(),b.size()};};
 data::Dictionary dict;assert(data::load_dictionary(bytes("animations_dictionary_pyarraynames.bin"),bytes("animations_dictionary_pyarray.bin"),dict,e));data::AnimationTables tables;
 assert(data::load_animation_tables(bytes("animations_pyarray.bin"),bytes("animations_pyarraynames.bin"),bytes("animations_pystructnames.bin"),dict,tables,e));
 data::DesignSettingsOwner design;assert(design.load(bytes("design_pyarray.bin"),bytes("design_pyarraynames.bin"),bytes("design_pystructnames.bin"),e));
 auto world=std::make_shared<int>(1);auto resource=[&](const std::string& name,std::vector<std::uint8_t>& out,std::string&){auto slash=name.find_last_of("/\\");out=read(path+"/"+name.substr(slash+1));return true;};
 camera::CameraAnimationManagerServicesV10 ms;ms.application_lease=world;ms.dictionary=&dict;ms.read=resource;ms.actual_lg_devices=[](auto& lg,auto&){lg=0;return true;};ms.debug_after_add=[](auto&){return true;};
 auto manager=std::make_shared<camera::GameplayCameraAnimationManagerV10>(ms);
 // Explicit fixtures: World actor lookup, Application dt/device/Debug, active
 // slot and SceneManager membership. Actual cache, source scene, manager,
 // timeline, CameraLevel target/zoom/damping and matrix owners are composed.
 std::uintptr_t active=0;unsigned roots=0,animators=0;bool reject_detach=false;std::vector<std::string> order;
 camera::PointV2 origin{},anchor{100,200,300};camera::CameraRuntimeServicesV11 s;s.world_lease=world;s.tables=&tables;s.design=design.borrow();s.manager=manager;s.read=resource;
 s.attach_graph=[&](auto graph,auto&){assert(graph&&!graph->graph().graph.empty());++roots;order.push_back("root");return true;};
 s.attach_animator=[&](auto graph,auto animator,auto&){assert(graph&&animator&&roots==1);++animators;order.push_back("animator");return true;};
 s.detach=[&](auto graph,auto animator,auto& error){assert(graph);if(reject_detach){error="fixture detach failure";return false;}if(animator&&animators)--animators;--roots;order.push_back("detach");return true;};
 s.activate=[&](auto identity,auto graph,auto selected,auto&){assert(graph&&selected<graph->cameras().size());active=identity;order.push_back("active");return true;};s.is_active=[&](auto id,auto& yes,auto&){yes=id==active;return true;};s.release_active=[&](auto id,auto&){if(active==id)active=0;order.push_back("release-active");return true;};
 s.actor_services.target.source_origin=&origin;s.actor_services.target.application_dt=[](auto& dt,auto&){dt=17;return true;};s.actor_services.target.actor_anchor=[&](auto id,auto& point,auto&){assert(id==42);point=anchor;return true;};
 s.actor_services.actor_position=s.actor_services.target.actor_anchor;s.actor_services.handle_centering=[](auto&,auto&){return true;};s.actor_services.debug_infinite_zoom=[](auto& result,auto&){result=false;return true;};s.actor_services.actor_disabled81=[](auto id,auto& disabled,auto&){assert(id==42);disabled=false;return true;};
 camera::GameplayCameraRuntimeV11 camera(s);camera::CameraLoadV11 input{"cameratests.bdae","Default","PlayerCamera_Default",600,10000};
 if(!camera.load(input,e))throw std::runtime_error(e);assert(camera.loaded()&&roots==1&&animators==1&&camera.set_id()==-2);assert(order==std::vector<std::string>({"root","animator"}));
 assert(camera.activate(e)&&camera.play_idle(e)&&camera.set_target(42,0,e));assert(camera.update(e));
 camera::PointV2 root;assert(camera.scene()->root_position(root.data(),e)&&root==anchor);
 assert(camera.scene_phase(0,e)&&camera.scene_phase(17,e)&&camera.scene_phase(34,e));assert(camera.update(e));
 camera::CameraViewV11 view;assert(camera.view(view,e));assert(std::isfinite(view.eye[2])&&view.up[2]==1&&view.near_plane==600&&view.far_plane==10000);
 assert(view.projection.values[11]==1&&view.projection.identity_flag==0&&view.view.identity_flag==0);
 assert(std::fabs(view.aspect-1.667752385f)<1e-6f);std::cout<<"Source camera eye "<<view.eye[0]<<' '<<view.eye[1]<<' '<<view.eye[2]<<" distance "<<camera.level()->fields().default_distance94<<'\n';
 reject_detach=true;assert(!camera.close(e)&&camera.loaded()&&camera.animator()->ready()&&roots==1&&active!=0);reject_detach=false;assert(camera.close(e)&&!camera.loaded()&&roots==0&&animators==0&&active==0);assert(order[order.size()-2]=="detach"&&order.back()=="release-active");assert(camera.close(e));
 // Actual published root remains a reachable failure prefix until discard.
 auto failure_services=s;failure_services.attach_animator={};camera::GameplayCameraRuntimeV11 failure(failure_services);assert(!failure.load(input,e)&&roots==1&&failure.animator());assert(failure.close(e)&&roots==0&&animators==0);
 camera::GameplayCameraRuntimeV11 unknown(s);input.animation_set="not-a-source-member";assert(!unknown.load(input,e)&&roots==0&&unknown.set_id()==-1);
 std::cout<<"Camera actual-cache full load/frame/view/release and failure-prefix PASS; World/SceneManager services declared fixtures\n";
 }catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
