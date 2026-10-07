#include "../gameplay_camera_load_v19.hpp"
#include "../gameplay_camera_picking_v20.hpp"
#include "../authored_camera_basis_v22.hpp"
#include "../gameplay_camera_application_v23.hpp"
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
 auto actual_dictionary_owner=std::make_shared<data::Dictionary>();auto& dict=*actual_dictionary_owner;assert(data::load_dictionary(bytes("animations_dictionary_pyarraynames.bin"),bytes("animations_dictionary_pyarray.bin"),dict,e));data::AnimationTables tables;
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

 camera::CameraPickingViewV20 pv;pv.camera=view;pv.viewport_width=960;pv.viewport_height=540;pv.render_width=960;pv.render_height=540;pv.scene_manager_present=true;pv.camera_present=true;
 camera::GameplayCameraPickingV20 picking;if(!picking.bind(pv,e))throw std::runtime_error(e);
 std::array<float,2> ndc;assert(picking.screen_coord(anchor,ndc,e));
 std::array<std::int32_t,2> pixels;assert(picking.screen_pixels(anchor,pixels,e));
 assert(pixels[0]>=0&&pixels[0]<=960&&pixels[1]>=0&&pixels[1]<=540);
 camera::CameraRayV20 ray;assert(picking.ray(pixels,ray,e));assert(ray.start==view.eye);
 camera::PointV2 hit{999,999,999};const float plane[4]{0,0,1,-anchor[2]};
 assert(dh2_camera_limited_plane_v20(plane,ray.start.data(),ray.end.data(),hit.data()));
 // Integer source pixel rounding makes a finite quantization footprint.
 // Actual geometry round-trip must stay within ten world units here.
 assert(std::fabs(hit[0]-anchor[0])<10&&std::fabs(hit[1]-anchor[1])<10&&std::fabs(hit[2]-anchor[2])<.001f);
 bool intersects=false;assert(picking.world_coord({ndc[0],-ndc[1]},anchor[2],hit,intersects,e)&&intersects);
 pv.camera_present=false;assert(picking.bind(pv,e)&&picking.ray(pixels,ray,e)&&ray.start==camera::PointV2{}&&ray.end==camera::PointV2{});
 assert(picking.screen_pixels(anchor,pixels,e)&&pixels[0]==-1000&&pixels[1]==-1000);
 std::uint32_t actual_index;assert(camera.level()->scene()->select(input.camera.node,actual_index,e));
 camera::PointV2 source_up{0,0,1};camera::CameraBasisServicesV21 basis;
 unsigned fov_reads=0;auto read_fov=[&](float&f,std::string&error){++fov_reads;camera::CameraViewV11 actual;if(!camera.view(actual,error))return false;f=actual.fov;return true;};
 assert(camera::borrow_authored_camera_basis_v22(camera.level()->scene(),actual_index,read_fov,&origin,&source_up,basis,e));
 camera::PointV2 look,up,offset;bool available=false;
 assert(camera::source_camera_basis_v21(basis,look,up,e));
 const auto& authored_parent=camera.level()->scene()->graph().graph[camera.level()->scene()->cameras()[actual_index].node].world;
 for(unsigned i=0;i<3;++i){assert(look[i]==authored_parent[4+i]&&up[i]==authored_parent[8+i]);}
 assert(camera::source_camera_center_offset_v21(basis,anchor[2],offset,available,e)&&available&&fov_reads==2&&std::isfinite(offset[0])&&std::isfinite(offset[1]));
 basis.camera_present=false;offset={321,654,987};assert(camera::source_camera_center_offset_v21(basis,anchor[2],offset,available,e)&&!available&&offset[0]==321);
 assert(camera::source_camera_basis_v21(basis,look,up,e)&&look==origin&&up==origin);
 std::cout<<"Actual CameraTests view -> source HUD pixel -> same limited picking ray PASS\n";
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
 // Application services survive World replacement. Source metadata has a
 // genuine shared owner; filesystem, actor/dt/device/Debug/PM are explicit
 // fixture boundaries over actual bytes, not claimed full App PostInit.
 auto actual_app=std::make_shared<application::ApplicationServicesOwnerV5>();assert(actual_app->post_init_events_v5(e));
 auto context=std::make_shared<camera::GameplayCameraApplicationV23>();assert(actual_app->publish_native_camera_services_v20(context,e));
 camera::CameraApplicationBindingsV23 application_services;application_services.application=actual_app;application_services.actual_roots=registry;application_services.actual_dictionary_lease=actual_dictionary_owner;application_services.actual_dictionary=&dict;application_services.actual_file_system=world;application_services.backend=backend;application_services.actual_lg_devices=ms.actual_lg_devices;application_services.read=resource;application_services.debug_after_add=ms.debug_after_add;
 assert(context->initialize(std::move(application_services),e));auto same_manager=context->animation_manager();auto same_zoom=context->zoom_handler50();
 camera::CameraWorldBindingsV23 wb;wb.world_lease=world;wb.actual_tables=&tables;wb.design=design.borrow();wb.actors=s.runtime.actor_services;wb.local_character0=s.local_character0;
 wb.layout.world=world;wb.layout.source_party_count6c4=[](auto&n,auto&){n=1;return true;};wb.layout.has_current_level=[](auto&yes,auto&){yes=true;return true;};wb.layout.num_local_players=[](auto&n,auto&){n=1;return true;};
 std::shared_ptr<camera::CameraWorldSessionV23> actual_world_slot;input.skybox234.clear();
 for(unsigned replacement=0;replacement<2;++replacement){
  if(!context->load(input,wb,actual_world_slot,e))throw std::runtime_error(e);
  assert(context->loaded()&&actual_world_slot->camera->level()->set_id()==-2);
  assert(context->animation_manager()==same_manager&&context->zoom_handler50()==same_zoom);
  assert(context->update(e)&&context->scene_phase(0,e)&&context->scene_phase(17,e)&&context->view(view,e));
  assert(!context->close_application(e));
  assert(context->begin_world_release(e));context->flush_animation_sets();assert(context->release_world_owners(e));actual_world_slot.reset();assert(!context->loaded());
 }
 std::weak_ptr<application::ApplicationServicesOwnerV5> app_lifetime=actual_app;
 actual_app.reset();assert(app_lifetime.expired()); // App slot/context has no ownership cycle.
 assert(!context->load(input,wb,actual_world_slot,e));assert(context->close_application(e));
 std::cout<<"Persistent SAME Application camera services/two World loads/ordered release/no App cycle PASS\n";

 std::cout<<"Actual-cache outer Camera Load/scene/view/source teardown PASS; World actor, backend viewport and Debug fixtures explicit\n";
 }catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
