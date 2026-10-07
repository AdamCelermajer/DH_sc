#include "../gameplay_camera_load_v19.hpp"
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

 // Actual Application, event manager, process active slot, factory, registry,
 // node membership and animation owners. World/driver/Debug boundaries are
 // declared fixtures; actual decoded resource/table bytes are never fixtures.
 auto app=std::make_shared<application::ApplicationServicesOwnerV5>();
 assert(!app->events14());assert(app->post_init_events_v5(e));
 auto registry=std::make_shared<world::GameObjectSceneRootRegistryV1>();
 camera::CameraFactoryBackendV16 backend;backend.configured_backend=world;
 backend.viewport=[](auto& w,auto& h,auto&){w=960;h=540;return true;};
 auto factory=std::make_shared<camera::CameraDefaultFactoryV16>(registry,world,0,backend);
 camera::CameraZoomServicesV17 zs;zs.backend=backend;
 auto zoom=std::make_shared<camera::GameplayCameraZoomV17>(zs);
 if(!zoom->initialize(app->events14(),e))throw std::runtime_error(e);
 camera::PointV2 origin{},anchor{100,200,300};
 camera::CameraLoadServicesV19 s;s.application=app;s.manager=registry;
 s.actual_first_factory=factory;s.actual_zoom_handler50=zoom;s.backend=backend;
 s.runtime.world_lease=world;s.runtime.tables=&tables;s.runtime.design=design.borrow();s.runtime.manager=manager;s.runtime.read=resource;
 s.runtime.actor_services.target.source_origin=&origin;
 s.runtime.actor_services.target.application_dt=[](auto& dt,auto&){dt=17;return true;};
 s.runtime.actor_services.target.actor_anchor=[&](auto id,auto& point,auto&){assert(id==42);point=anchor;return true;};
 s.runtime.actor_services.actor_position=s.runtime.actor_services.target.actor_anchor;
 // One actual local player makes source HandleCentering a genuine early leaf.
 s.runtime.actor_services.handle_centering=[](auto&,auto&){return true;};
 s.runtime.actor_services.debug_infinite_zoom=[](auto& yes,auto&){yes=false;return true;};
 s.runtime.actor_services.actor_disabled81=[](auto id,auto& yes,auto&){assert(id==42);yes=false;return true;};
 s.local_character0=[](auto& id,auto&){id=42;return true;};
 camera::GameplayCameraLoadV19 camera(s);
 camera::CameraLevelConfigV19 input;input.camera={"cameratests.bdae","Default","PlayerCamera_Default",600,10000};
 if(!camera.load(input,e))throw std::runtime_error(e);
 assert(camera.loaded()&&camera.level()->set_id()==-2);
 assert(app->active_camera().source_active()!=0&&zoom->source_camera_identity20()!=0);
 assert(camera.update(e)&&camera.scene_phase(0,e)&&camera.scene_phase(17,e)&&camera.scene_phase(34,e));
 camera::CameraViewV11 view;if(!camera.view(view,e))throw std::runtime_error(e);
 assert(std::isfinite(view.eye[2])&&view.near_plane==600&&view.far_plane==10000);
 assert(camera.source_unbind_zoom_v19(e)&&zoom->source_camera_identity20()==0);
 if(!camera.source_clear_camera_roots_v19(e)||!camera.source_clear_scene_active_v19(e))throw std::runtime_error(e);
 camera.source_flush_animation_sets_v19();
 if(!camera.release_native_owners_v19(e))throw std::runtime_error(e);
 assert(!camera.loaded()&&app->active_camera().source_active()==0);
 // A genuine nonempty skybox reaches the required resource owner after
 // camera/target/Zoom publication. Failure preserves that prefix until the
 // same ordered discard, rather than inventing an empty successful skybox.
 camera::GameplayCameraLoadV19 failed(s);input.skybox234="actual-config-required.bdae";
 assert(!failed.load(input,e)&&e.find("skybox")!=std::string::npos);
 assert(!failed.loaded()&&app->active_camera().source_active()!=0);
 assert(failed.source_unbind_zoom_v19(e)&&failed.source_clear_camera_roots_v19(e)&&failed.source_clear_scene_active_v19(e));
 failed.source_flush_animation_sets_v19();
 if(!failed.release_native_owners_v19(e))throw std::runtime_error(e);
 assert(app->active_camera().source_active()==0&&zoom->source_camera_identity20()==0);
 assert(zoom->close(e));
 std::cout<<"Actual-cache outer Camera Load/scene/view/source teardown PASS; World actor, backend viewport and Debug fixtures explicit\n";
 }catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
