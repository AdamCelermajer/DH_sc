#include "native_menu_preview_v121.hpp"
#include "model_renderer.hpp"
#include "source_process_objects_v121.hpp"
#include "source_campaign_character_fsm_v101.hpp"
#include "native_menu_preview_physics_guard_v124.hpp"
#include "math.hpp"
#include <android/log.h>
#include <cstring>
#include <exception>
namespace model_renderer {namespace {
constexpr const char* menu_preview_log_tag_v121="DH2Native";
float source_float(std::uint32_t bits){float value;std::memcpy(&value,&bits,4);return value;}
std::shared_ptr<NativeMenuPreviewV121> process_preview_v121;
}
class NativeMenuPreviewV121:public std::enable_shared_from_this<NativeMenuPreviewV121> {
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application_;
 NativeMenuPreviewServicesV121 services_;
 MenuPreviewSceneV121 plane_;
 bool plane_constructor_reference_{};
 bool plane_ready_v121_{};
 MenuPreviewCharacterV121 character_;
 std::shared_ptr<dh2::camera::CameraProceduralNodeV16> camera_;
 bool camera_external_reference_{},camera_constructor_reference_{};
 bool camera_ready_v121_{};
 bool destroyed_{true}; //Original DATA99980c byte1, not BSS0.
 bool busy_{};
 bool current(std::string& e)const{
  if(application_.expired()||!services_.owner||!services_.scene||!services_.current||!services_.current(e)){
   if(e.empty())e="Required SAME actual process menu-preview owners";return false;
  }
  e.clear();return true;
 }
 bool effect(const std::function<bool(std::string&)>& f,const char* name,std::string& e){
  if(!f){e=std::string("Required actual menu-preview ")+name;return false;}return f(e);
 }
 bool destroy_character(std::string& e){
  if(!effect(services_.flush_objects,"ObjectManager.Flush",e)||
     !effect(services_.flush_animation_sets,"AnimSetManager.Flush",e))return false;
  character_={}; //Original mCharacterToRender storeNULL AFTER both Flushes.
  return true;
 }
 bool destroy_scene(std::string& e){
  if(plane_.identity){
   if(!plane_.owner||!plane_.root.owner||plane_.identity!=plane_.root.identity){e="Required same retained main-scene root";return false;}
   if(!services_.scene->release_visual_root(plane_.identity,e))return false;
   //Only a failed native AddChild/constructor-drop prefix still owns the
   //caller's original reference. Normal source SetupScene already dropped it.
   if(plane_constructor_reference_){
    if(!plane_.drop_constructor_reference||!plane_.drop_constructor_reference(e))return false;
    plane_constructor_reference_=false;
   }
   plane_={};plane_ready_v121_=false; //Parent-reference release owns its D1.
  }
  if(!effect(services_.remove_all_scene_nodes,"SceneManager.RemoveAll",e)||!destroy_character(e)||
     !effect(services_.free_textures,"C2DDriver.freeTextures",e)||!effect(services_.clean_glitch,"Application.CleanGlitch",e))return false;
  return true;
 }
 bool destroy_avatar_camera(std::string& e){
  if(!camera_){e.clear();return true;} //Genuine process BSSNULL branch.
  if(!camera_->remove_from_root(e))return false;
  //Only this source static's explicit reference is dropped here. The SAME
  //SceneManager active camera may still retain its independent reference.
  if(camera_external_reference_){if(!camera_->drop(e))return false;camera_external_reference_=false;}
  if(camera_constructor_reference_){if(!camera_->drop(e))return false;camera_constructor_reference_=false;}
  camera_.reset();camera_ready_v121_=false;e.clear();return true;
 }
 bool create_avatar_camera(std::string& e){
  if(camera_){
   if(!camera_ready_v121_){e="Retained incomplete native avatar-camera constructor prefix";return false;}
   e.clear();return true;
  }
  if(!services_.camera_backend.configured_backend||!services_.camera_backend.viewport){e="Required actual menu-preview camera backend";return false;}
  //Actual CCameraSceneNode(id=-1, exact42bf3c position/target words,
  //input=false). Same existing procedural source class, not a camera no-op.
  camera_=std::make_shared<dh2::camera::CameraProceduralNodeV16>(services_.scene,services_.camera_backend);
  camera_constructor_reference_=true;
  const dh2::camera::PointV2 position{0,source_float(std::uint32_t(-1000275968)),source_float(1125515264)};
  const dh2::camera::PointV2 target{0,0,source_float(1130430464)};
  if(!camera_->set_position(position,e)||!camera_->set_target(target,e)||!camera_->add_to_root(e))return false;
  if(!camera_->drop(e))return false;camera_constructor_reference_=false;
  if(!services_.scene->set_active_camera_v13(camera_->camera_borrow(),e)||!camera_->set_up({0,0,1},e))return false;
  // The concrete CCameraSceneNode address point is _ZTV+0x1c (5837dc).
  // 0x42bf3c calls slots 0x138/0x13c: setAspectRatio(0x3FD578E9),
  // then setFOV(0x3F3579C8). set_data takes FOV before aspect.
  if(!camera_->set_data(source_float(1060469192),source_float(1070954729),1.f,3000.f,e)||!camera_->grab(e))return false;
  camera_external_reference_=true;
  if(!camera_->set_planes(source_float(1092616192),source_float(1157234688),e))return false;
  camera_ready_v121_=true;return true;
 }
 bool setup_scene(std::string& e){
  if(plane_.identity){
   if(!plane_ready_v121_){e="Retained incomplete native main-scene constructor prefix";return false;}
   e.clear();return true;
  }
  if(!services_.load_physics||!services_.construct_scene){e="Required native main-scene physics/resource constructor";return false;}
  if(!services_.load_physics(0,0,1,1,e))return false;
  //Hold any genuine constructed prefix even when source construction fails.
  const bool constructed=services_.construct_scene("data/3d/menu/main_menu_charactere_swamp.bdae",plane_,e);
  if(plane_.owner&&plane_.drop_constructor_reference)plane_constructor_reference_=true;
  if(!constructed)return false;
  if(!plane_.owner||!plane_.identity||plane_.root.identity!=plane_.identity||!plane_.root.owner||!plane_.drop_constructor_reference){
   e="Required actual constructed main-menu authored scene root";return false;
  }
  if(!services_.scene->add_child(plane_.root,e)||!plane_.drop_constructor_reference(e))return false;
  plane_constructor_reference_=false;plane_ready_v121_=true;
  return true;
 }
 bool setup_character(std::string& e){
  if(!services_.avatar_state_owner||!services_.avatar_state){e="Required SAME MainMenu save-slot storage";return false;}
  const auto slot=services_.avatar_state->slot;
  if(slot==-1||!plane_.identity){
   // Normal ChangeCharacterToDisplay flushes the old Character before this
   // callback. Also enforce that invariant when an interrupted/replayed slot
   // callback reaches an unoccupied selection directly, so an Empty slot can
   // never leave the previous actor retained for the menu renderer.
   if(character_.identity||character_.owner){
    if(!destroy_character(e))return false;
   }
   __android_log_print(ANDROID_LOG_INFO,menu_preview_log_tag_v121,
    "Menu preview slot setup | requested_slot %d | scene %s | actor none",slot,plane_.identity?"ready":"absent");
   e.clear();return true; //Original guarded return.
  }
  if(!services_.save_exists||!services_.create_player){e="Required actual source SG_Exists/CreatePlayer menu-preview provider";return false;}
  const bool fresh_slot_intent=services_.avatar_state->fresh_slot_intent;
  bool exists{};if(!services_.save_exists(slot,exists,e))return false;
  // The authored character picker shows the swamp scene for an empty slot,
  // but there is no saved avatar to preview. Keep its scene/camera alive and
  // defer CreatePlayer until the later name/class creation flow. The source
  // CreatePlayer fresh-character branch belongs to that creation flow; using
  // it here made every EMPTY slot display a default Warrior.
  if(!exists&&!fresh_slot_intent){
   if(character_.identity||character_.owner){
    if(!destroy_character(e))return false;
   }
   __android_log_print(ANDROID_LOG_INFO,menu_preview_log_tag_v121,
    "Menu preview slot setup | requested_slot %d | save_exists 0 | Character none | visual_root none",slot);
   e.clear();return true;
  }
  const bool fresh_profile=fresh_slot_intent||!exists;
  if(!services_.create_player(slot,fresh_profile,character_,e))return false;
  __android_log_print(ANDROID_LOG_INFO,menu_preview_log_tag_v121,
   "Menu preview slot setup | requested_slot %d | save_exists %d | fresh_intent %d | fresh %d | Character %zu | visual_root %zu",
   slot,exists,fresh_slot_intent,fresh_profile,static_cast<std::size_t>(character_.identity),static_cast<std::size_t>(character_.visual_root));
  if(!character_.owner||!character_.identity||!character_.visual_root||!character_.attach_visual||
     !character_.visual_position||!character_.visual_rotation||!character_.visual_visible){e="Required SAME source preview Character/visual2d8 owner";return false;}
  if(exists&&!fresh_profile&&(!character_.sg_load||!character_.sg_load(4,e)))return false;
  if(!camera_){e="Original positive SetupCharacter requires published main-menu camera";return false;}
  if(!services_.scene->set_active_camera_v13(camera_->camera_borrow(),e)||!character_.attach_visual(e))return false;
  const std::array<float,3> position{0,source_float(std::uint32_t(-1018691584)),source_float(std::uint32_t(-1046478848))};
  dh2::math::Quaternion rotation{};
  //Original expression is float(3.1416 * -0.125), then the existing source
  //quaternion Euler setter. No screenshot-specific alignment correction.
  dh2_quat_from_euler(&rotation,0,0,static_cast<float>(3.1416*-0.125));
  return character_.visual_position(position,e)&&character_.visual_rotation({rotation.x,rotation.y,rotation.z,rotation.w},e)&&character_.visual_visible(true,e);
 }
 template<class F> bool run(F&& f,std::string& e){
  if(busy_){e="Source menu-preview lifecycle synchronously reentered";return false;}
  if(!current(e))return false;
  busy_=true;struct Guard{bool& b;~Guard(){b=false;}} guard{busy_};
  try{return f();}catch(const std::exception& x){e=x.what();return false;}
 }
public:
 NativeMenuPreviewV121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,NativeMenuPreviewServicesV121 services):application_(app),services_(std::move(services)){}
 bool belongs_to(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app)const noexcept{
  auto actual=application_.lock();return actual&&app&&actual.get()==app.get()&&!actual.owner_before(app)&&!app.owner_before(actual);
 }
 bool main_hide(std::string& e){return run([&]{
  if(!destroy_scene(e)||!destroy_avatar_camera(e))return false;
  destroyed_=true;
  if(!services_.avatar_state||!services_.avatar_state_owner){e="Required actual MainMenu.mSavegameSlot";return false;}
  services_.avatar_state->slot=-1;
  if(!services_.stop_music){e="Required actual Vox.StopMusic1000";return false;}
  return services_.stop_music(1000,e);
 },e);}
 bool select_show(std::int32_t& saved_slot,std::string& e){return run([&]{
  // IDA MenuCharacterSelect::Show (0x428f38) calls only these two
  // MenuMainMenu teardown methods before it transfers mSavegameSlot, marks
  // m_isDestroyed, and clears the MainMenu slot. Unlike MainMenu::Hide this
  // path does not call VoxSoundManager::StopMusic.
  if(!destroy_scene(e)||!destroy_avatar_camera(e))return false;
  if(!services_.avatar_state||!services_.avatar_state_owner){e="Required actual MainMenu.mSavegameSlot";return false;}
  saved_slot=services_.avatar_state->slot;destroyed_=true;services_.avatar_state->slot=-1;
  e.clear();return true;
 },e);}
 bool select_hide(std::int32_t slot,std::uintptr_t captured_root,std::string& e){return run([&]{
  auto root=captured_root;
  if(!root){if(!services_.selected_scene_root||!services_.selected_scene_root(root,e)){if(e.empty())e="Required actual selection-scene resource query";return false;}}
  if(root&&(!services_.remove_selected_root||!services_.remove_selected_root(root,e)))return false;
  if(!effect(services_.remove_all_scene_nodes,"SceneManager.RemoveAll",e)||
     !effect(services_.flush_objects,"ObjectManager.Flush",e)||
     !effect(services_.flush_animation_sets,"AnimSetManager.Flush",e)||!create_avatar_camera(e))return false;
  destroyed_=false;
  if(!services_.avatar_state||!services_.avatar_state_owner){e="Required actual MainMenu.mSavegameSlot";return false;}
  services_.avatar_state->slot=slot;
  return setup_scene(e)&&setup_character(e);
 },e);}
 bool prepare_main(std::string& e){return run([&]{
  //42c62c always invokes these methods; their actual positive static slots
  //own the individual early returns. m_isDestroyed is not a Show gate.
  if(!create_avatar_camera(e)||!setup_scene(e))return false;
  if(!character_.identity&&!setup_character(e))return false;
  destroyed_=false;return true;
 },e);}
 bool update_main(std::string& e){return run([&]{
  // MenuMainMenu::Update (ARM 0x42c1c8) calls CharAnimator::Update on the
  // same static mCharacterToRender when non-NULL, then calls this same
  // SceneManager's CSceneManager::update(0,false). In the original application
  // the process scene clock is already advanced by its outer scene frame. This
  // native front-menu path has no outer SceneManager update, so sample the SAME
  // published Application Timer through CSceneManager's source sentinel before
  // traversing roots; adding zero here otherwise freezes the character pose.
  // The caller already applied the original MenuBase::IsValidMenu byte+0x7c gate.
  if(character_.identity&&
     !source_campaign_character_animator_update_v102(services_.owner,character_.identity,e))return false;
  dh2::world::SceneUpdateTransportV102 transport;transport.provider=services_.owner;
  transport.timer=[](std::uint32_t& time,std::string& error){return model_renderer::borrow_application_time_v68(time,error);};
  return services_.scene->source_update_v102(-123456.f,false,transport,e);
 },e);}
 bool avatar_destroy(std::string& e){return run([&]{return destroy_character(e);},e);}
 bool scene_destroy(std::string& e){return run([&]{return destroy_scene(e);},e);}
 bool camera_destroy(std::string& e){return run([&]{return destroy_avatar_camera(e);},e);}
 bool avatar_setup(std::int32_t slot,std::string& e){return run([&]{
  if(!services_.avatar_state||services_.avatar_state->slot!=slot){e="Native avatar slot differs from SAME MainMenu producer";return false;}
  // ChangeCharacterToDisplay calls SetupCharacter before CreateAvatarCamera,
  // and does not call SetupScene. Main.Show normally already established the
  // source PhysicalWorld. A renderer teardown can retire that backend while
  // the independently retained menu-scene root remains; restore it only for
  // that stale-domain prefix, and never reload a live Character's world.
  if(slot>=0&&!ensure_menu_preview_physics_v124(bool(plane_.identity),bool(character_.identity),
     services_.physical_world_ready,services_.load_physics,e))return false;
  return setup_character(e);
 },e);}
 bool avatar_camera(std::string& e){return run([&]{return create_avatar_camera(e);},e);}
 bool camera(std::shared_ptr<dh2::camera::CameraProceduralNodeV16>& out,std::string& e)const{
  if(!current(e))return false;out=camera_;e.clear();return true;
 }
 bool camera_view(dh2::camera::CameraViewV11& out,bool& present,std::string& e)const{
  present=false;if(!current(e))return false;
  if(!camera_){e.clear();return true;} //Observed process static NULL.
  const auto& active=services_.scene->active_camera_v13();
  if(!camera_ready_v121_||!camera_->alive()||active.identity!=camera_->identity()||!active.owner||
     active.owner.owner_before(camera_)||camera_.owner_before(active.owner)){
   e="Menu preview camera is not the SAME actual active SceneManager camera";return false;
  }
  if(!camera_->view(out,e))return false;present=true;e.clear();return true;
 }
};
bool borrow_native_menu_preview_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 std::shared_ptr<NativeMenuPreviewV121>& out,std::string& e){
 out.reset();if(!app){e="Required actual process Application for menu preview";return false;}
 if(process_preview_v121){
  if(!process_preview_v121->belongs_to(app)){e="Different App already owns process menu preview";return false;}
  out=process_preview_v121;e.clear();return true;
 }
 NativeMenuPreviewServicesV121 services;if(!build_native_menu_preview_services_v121(app,services,e))return false;
 if(!services.owner||!services.scene||!services.current){e="Required actual native menu-preview core owners";return false;}
 if(!services.owner.owner_before(app)&&!app.owner_before(services.owner)){e="Preview resource provider must not cycle into App";return false;}
 process_preview_v121=std::make_shared<NativeMenuPreviewV121>(app,std::move(services));out=process_preview_v121;e.clear();return true;
}
bool native_menu_preview_main_hide_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,std::string& e){
 std::shared_ptr<NativeMenuPreviewV121> p;return borrow_native_menu_preview_v121(app,p,e)&&p->main_hide(e);
}
bool native_menu_preview_select_show_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 std::int32_t& saved_slot,std::string& e){
 saved_slot=-1;std::shared_ptr<NativeMenuPreviewV121> p;if(!borrow_native_menu_preview_v121(app,p,e))return false;return p->select_show(saved_slot,e);
}
bool native_menu_preview_select_hide_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 std::int32_t slot,std::uintptr_t root,std::string& e){
 std::shared_ptr<NativeMenuPreviewV121> p;return borrow_native_menu_preview_v121(app,p,e)&&p->select_hide(slot,root,e);
}
bool native_menu_preview_prepare_main_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,std::string& e){
 std::shared_ptr<NativeMenuPreviewV121> p;return borrow_native_menu_preview_v121(app,p,e)&&p->prepare_main(e);
}
bool native_menu_preview_update_main_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,std::string& e){
 std::shared_ptr<NativeMenuPreviewV121> p;return borrow_native_menu_preview_v121(app,p,e)&&p->update_main(e);
}
bool native_menu_preview_destroy_scene_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,std::string& e){
 std::shared_ptr<NativeMenuPreviewV121> p;return borrow_native_menu_preview_v121(app,p,e)&&p->scene_destroy(e);
}
bool native_menu_preview_destroy_avatar_camera_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,std::string& e){
 std::shared_ptr<NativeMenuPreviewV121> p;return borrow_native_menu_preview_v121(app,p,e)&&p->camera_destroy(e);
}
bool native_menu_preview_camera_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 std::shared_ptr<dh2::camera::CameraProceduralNodeV16>& out,std::string& e){
 std::shared_ptr<NativeMenuPreviewV121> p;return borrow_native_menu_preview_v121(app,p,e)&&p->camera(out,e);
}
bool native_menu_preview_camera_view_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 dh2::camera::CameraViewV11& out,bool& present,std::string& e){
 present=false;std::shared_ptr<NativeMenuPreviewV121> p;
 return borrow_native_menu_preview_v121(app,p,e)&&p->camera_view(out,present,e);
}
bool native_menu_avatar_services_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 dh2::ui::MenuAvatarPreviewServicesV1& out,std::shared_ptr<void>& pin,std::string& e){
 std::shared_ptr<NativeMenuPreviewV121> p;if(!borrow_native_menu_preview_v121(app,p,e))return false;
 struct Adapter{std::weak_ptr<NativeMenuPreviewV121> preview;};auto a=std::make_shared<Adapter>();a->preview=p;
 out.context=a.get();
 out.destroy_character=[](void* raw,std::string& e){auto p=static_cast<Adapter*>(raw)->preview.lock();if(!p){e="Retired actual preview avatar provider";return false;}return p->avatar_destroy(e);};
 out.setup_character=[](void* raw,std::int32_t slot,std::string& e){auto p=static_cast<Adapter*>(raw)->preview.lock();if(!p){e="Retired actual preview avatar provider";return false;}return p->avatar_setup(slot,e);};
 out.create_avatar_camera=[](void* raw,std::string& e){auto p=static_cast<Adapter*>(raw)->preview.lock();if(!p){e="Retired actual preview camera provider";return false;}return p->avatar_camera(e);};
 pin=a;e.clear();return true;
}
}
