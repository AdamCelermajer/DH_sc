#include <jni.h>
#include "frame_perf_v35.hpp"
#include "native_resource_budget_v38.hpp"
#include <android/log.h>
#include <GLES2/gl2.h>
#include "textures.hpp"
#include "model_renderer.hpp"
#include "renderer_menu_preview_domain_v121.hpp"
#include "source_campaign_script_tutorial_v118.hpp"
#include "owned_hud_settings_v1.hpp"
#include "source_input_manager_v60.hpp"
#include "application_save_files_owner_v61.hpp"
#include "process_resource_prefix_source_v95.hpp"
#include "gameplay_camera_application_v23.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "application_player_manager_bootstrap_v59.hpp"
#include "renderer_native_menu_prefix_v62.hpp"
#include "native_menu_preview_v121.hpp"
#include "captured_menu_lease_v101.hpp"
#include "script_manager_owner_v52.hpp"
#include "native_source_script_ui_v98.hpp"
#include "source_campaign_admission_v104.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_retirement_v88.hpp"
#include "source_campaign_spawn_groups_v108.hpp"
#include "native_source_campaign_retirement_v104.hpp"
#include "native_source_area_transition_v114.hpp"
#include "native_source_main_menu_v114.hpp"
#include "source_process_trophies_v100.hpp"
#include "native_process_startup_v119.hpp"
#include "source_campaign_script_execution_v96.hpp"
#include "source_campaign_script_actor_v96.hpp"
#include "source_campaign_script_runtime_v99.hpp"
#include "port/engine-audio/integration-v42/application/native_audio_application_v42.hpp"
#include "authored_shader_program.hpp"
#include "original_ui_session.hpp"
#include "front_ui_session_v87.hpp"
#include "front_loading_render_policy_v1.hpp"
#include "character_panel_session_v1.hpp"
#include "gameplay_hud.hpp"
#include "gameplay_icons.hpp"
#include "authored_character_panel_platform_v4.hpp"
#include "swf_menu_unload_v91.hpp"
#include "flash_anim_manager_v92.hpp"
#include "authored_shared_menu_roster_v27.hpp"
#include "authored_menu_character_projection_v4.hpp"
#include "authored_menu_native_drm_v4.hpp"
#include "authored_character_application_v1.hpp"
#include "character_panel_runtime_v3.hpp"
#include "character_design_services.hpp"
#include "menu_rollover_input_v1.hpp"
#include "character_menu_application_v4.hpp"
#include "character_menu_potions_v4.hpp"
#include <chrono>
#include <cmath>
#include <cstring>
#include <android/asset_manager_jni.h>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include <exception>
#include <stdexcept>
#include "gameplay_camera_device_v9.hpp"

namespace {
constexpr const char* tag="DH2Native";
dh2::android_ui::Program ui_program{};
dh2::android_ui::OriginalUiSession original_ui;
dh2::android_ui::FrontUiSessionV87 front_ui;
std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> application_services_v5;
 std::uint32_t application_frame_time_v93{};
 std::uint64_t application_draw_epoch_v69{};
 bool application_frame_time_started_v93{};
std::string front_directory,front_launch_report;
std::string android_manufacturer_v20;
bool android_device_configured_v20{};
std::unique_ptr<dh2::android_ui::CharacterPanelSessionV1> character_panel;
AAssetManager* gameplay_assets=nullptr;
std::string original_ui_error;
GLuint texture=0;
int surface_width=1,surface_height=1,texture_width=1,texture_height=1;
bool report_model_frame=true;
bool report_texture_frame=true;
struct MusicPlatformV1 {
 JavaVM* vm{};jclass receiver{};jmethodID method{};
 ~MusicPlatformV1(){JNIEnv* env{};if(receiver&&vm&&vm->GetEnv(reinterpret_cast<void**>(&env),JNI_VERSION_1_6)==JNI_OK)env->DeleteGlobalRef(receiver);}
 bool query(std::int32_t& value,std::string& error){
  JNIEnv* env{};
  if(!vm||!receiver||!method||vm->GetEnv(reinterpret_cast<void**>(&env),JNI_VERSION_1_6)!=JNI_OK){error="Required attached Android music JNI owner";return false;}
  value=env->CallStaticIntMethod(receiver,method);
  if(env->ExceptionCheck()){env->ExceptionClear();error="Original Android isSupportMM JNI call failed";return false;}
  __android_log_print(ANDROID_LOG_INFO,tag,"Original Android music support | JNI integer %d",value);return true;
 }
};
#include "native_character_menu_v4.inc"
std::string errors(const char* operation){
  std::string text;for(GLenum e;(e=glGetError())!=GL_NO_ERROR;){char s[96];std::snprintf(s,sizeof(s),"%s GL error 0x%04x; ",operation,e);
    text+=s;__android_log_print(ANDROID_LOG_ERROR,tag,"%s",s);}return text;
}
jstring result(JNIEnv* env,const std::string& text){return env->NewStringUTF(text.c_str());}
}
#include "native_application_audio_v45.inc"
namespace dh2::android_ui {
bool dispatch_process_menu_rollover_v1(const char* name,const gameswf::fn_call& call,std::string& error){
 if(!name||std::strcmp(name,"NativeChangeRolloverInputBehavior")){
  error="Unexpected process-menu rollover callback";return false;
 }
 if(!authored_character_menu||!character_panel){
  error="Required live Level12 MenuManager/primary1 owner for HUD rollover";return false;
 }
 auto* panel=character_panel->authored_native_v4();
 if(!panel||panel->source_stack_v4()!=authored_character_menu->shared_stack_v27.get()){
  error="Gameplay HUD rollover lost the same primary1 MenuManager";return false;
 }
 return NativeCharacterMenuV4::native(authored_character_menu.get(),name,call,error);
}
}
bool model_renderer::borrow_actual_menu_device_v1(dh2::ui::MenuDeviceFactsV1& facts,std::string& error){
 return front_ui.menu_device_borrow_v4(facts,error);
}
bool model_renderer::borrow_actual_application_services_v5(std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& out,std::string& error){
 if(!application_services_v5||!application_services_v5->events14()){
  error="Required actual Application PostInit EventManager publication";return false;
 }
 out=application_services_v5;error.clear();return true;
}
namespace model_renderer {
#include "renderer_resource_prefix_source_v95.inc"
}
bool model_renderer::borrow_actual_input_manager_v60(std::shared_ptr<dh2::input::SourceInputManagerV60>& out,std::string& error)try{
 out.reset();
 std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 if(!borrow_actual_application_services_v5(application,error))return false;
 if(application->source_input_manager_v60())out=application->source_input_manager_v60();
 else{
  auto produced=std::make_shared<dh2::input::SourceInputManagerV60>();
  if(!application->publish_source_input_manager_v60(produced,error))return false;
  out=std::move(produced);
 }
 error.clear();return true;
}catch(const std::exception& failure){out.reset();error=failure.what();return false;}
bool model_renderer::borrow_actual_camera_viewport_v20(std::int32_t& width,std::int32_t& height,std::string& error){
 if(surface_width<=1||surface_height<=1){error="Required resized Android camera surface";return false;}
 width=surface_width;height=surface_height;error.clear();return true;
}
bool model_renderer::borrow_actual_camera_lg_device_v20(std::uint8_t& lg,std::string& error){
 if(!android_device_configured_v20){error="Required Android Build.MANUFACTURER camera device input";return false;}
 return dh2::camera::source_lg_devices_v9(android_manufacturer_v20.c_str(),lg,error);
}
bool model_renderer::borrow_actual_font_palette_v4(const dh2::ui::CharacterMenuFontPaletteV1*& palette,std::string& error){
 if(!character_panel){palette=nullptr;error="Required actual character panel font palette owner";return false;}
 return character_panel->font_palette_borrow_v4(palette,error);
}
bool model_renderer::borrow_actual_status_messages_v26(std::shared_ptr<dh2::ui::MenuStatusMessagesV26>& out,std::string& error){
 return original_ui.status_messages_v26(out,error);
}
bool model_renderer::refresh_actual_message_caches_stage26_v66(std::string& error){
 return original_ui.refresh_message_caches_stage26_v66(error);
}
bool model_renderer::complete_actual_hud_refresh_stage26_v66(std::string& error){
 return original_ui.complete_refresh_stage26_v66(error);
}
#include "native_menu_update_prefix_v62.inc"
#include "native_menu_postmovie_v62.inc"
#include "native_process_primary1_binding_v98.inc"
#include "character_panel_source_loading_v98.inc"
#include "native_menu_campaign_retire_v104.inc"
#include "native_menu_singletons_v67.inc"
#include "native_gslevel_menu_v27.inc"
namespace {bool ensure_native_main_menu_load_v114(std::string&);}
#include "native_menu_resources_v93.inc"
#include "native_captured_menu_v104.inc"
#include "native_zone_presentation_v83.inc"
#include "native_death_presentation_v84.inc"
#include "native_spawn_groups_v108.inc"
#include "native_source_script_ui_v98.inc"
#include "native_process_text_startup_v101.inc"
#include "native_gameplay_presentation_v67.inc"
#include "native_source_scene_v67.inc"
#include "native_campaign_delivery_v104.inc"
#include "native_source_campaign_retirement_v104.inc"
#include "native_source_area_transition_v114.inc"
#include "native_main_menu_front_v114.inc"
#include "native_main_menu_load_v114.inc"
#include "native_source_main_menu_v114.inc"
#include "native_menu_application_event_v120.inc"
#include "native_selected_menu_hide_v119.inc"
#include "native_process_startup_v119.inc"
#include "native_menu_preview_backend_v121.inc"
bool model_renderer::bind_native_level_ui_release_v107(dh2::loader::LevelDestroyServicesV1& services,std::string& error){
 return original_ui.bind_level_ui_release_v107(services,error);
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_buildInfo(JNIEnv* env,jclass) {
  return result(env,"Native source reconstruction: animated scene nodes");
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_initialize(JNIEnv* env,jclass bridge,jobject assets){
  const bool retain_campaign=model_renderer::source_campaign_active_v55();
  if(retain_campaign){
   std::string admission_error;
   if(!model_renderer::rebind_source_campaign_admission_after_context_loss_v104(admission_error))
    return result(env,"Source GL context handoff: "+admission_error);
  }
  if(retain_campaign)native_source_scene_v67.context_lost();
  else native_source_scene_v67.clear();
  // Android has created a new context: previous GL names belong to the old
  // context and must not be deleted against this context's reused names.
  model_renderer::invalidate_actual_device_context_v54();
  dh2::android_resources::context_lost_v38();
  model_renderer::reset_context();ui_program={};texture=0;report_texture_frame=true;
  dh2::android_ui::Program premultiplied;
  try{
     dh2::android_resources::begin_context_v38();
     std::string device_error;
     if(!model_renderer::bind_actual_device_context_v54(device_error))throw std::runtime_error(device_error);
     // Service lifetime is the native Application process, not a GL context.
     // This source-backed prefix reconstructs only pointer14 publication;
     // complete Application PostInit/Shutdown remain separate work.
     if(!application_services_v5)application_services_v5=std::make_shared<dh2::application::ApplicationServicesOwnerV5>();
     std::string application_error;
     if(!application_services_v5->events14()&&!application_services_v5->post_init_events_v5(application_error))throw std::runtime_error(application_error);
     if(!model_renderer::initialize_source_resource_prefix_v95(application_services_v5,application_error))throw std::runtime_error(application_error);
     auto* manager=assets?AAssetManager_fromJava(env,assets):nullptr;
      gameplay_assets=manager;
      // Reached GSInit phase0 audio constructor; full phase9 settings remain
      // separate from exact soundpack/nullable Application owner publication.
      std::string audio_error;
      if(!dh2::android_audio::ensure_application_audio_v42(env,assets,audio_error))throw std::runtime_error(audio_error);
      if(!retain_campaign&&!native_process_startup_v119){
        if(authored_character_menu&&!authored_character_menu->release_flash_for_teardown_v93(application_error))
          throw std::runtime_error(application_error);
        character_panel.reset();authored_character_menu.reset();
      }else if(authored_character_menu){
        authored_character_menu->width=authored_character_menu->height=0;
      }
    ui_program=dh2::android_ui::create(manager,false);
    premultiplied=dh2::android_ui::create(manager,true);
    dh2::android_ui::validate_pixels(ui_program,premultiplied);
    dh2::android_ui::release(premultiplied);
    std::string ui_error;
     if(!original_ui.initialize(manager,ui_error))throw std::runtime_error(ui_error);
     if(!front_ui.initialize(manager,ui_error))throw std::runtime_error(ui_error);
     if(!dh2::android_ui::bind_android_intro_cache_v119([](const auto& uri,bool& found,auto& bytes,auto maximum,auto& e){
       return original_ui.process_cache_read_v119(uri,found,bytes,maximum,e);
     },ui_error))throw std::runtime_error(ui_error);
     auto music=std::make_shared<MusicPlatformV1>();
     if(env->GetJavaVM(&music->vm)!=JNI_OK)throw std::runtime_error("Required Android music JavaVM");
     music->receiver=static_cast<jclass>(env->NewGlobalRef(bridge));
     music->method=env->GetStaticMethodID(bridge,"isSupportMM","()I");
     if(!music->receiver||!music->method||env->ExceptionCheck()){env->ExceptionClear();throw std::runtime_error("Required original Android isSupportMM receiver");}
     original_ui.bind_platform_music([music](std::int32_t& value,std::string& error){return music->query(value,error);});
      if(!character_panel)character_panel=std::make_unique<dh2::android_ui::CharacterPanelSessionV1>(manager);
  }catch(const std::exception& e){
    model_renderer::invalidate_actual_device_context_v54();
    dh2::android_ui::release(premultiplied);dh2::android_ui::release(ui_program);
    __android_log_print(ANDROID_LOG_ERROR,tag,"Authored UI initialization failed: %s",e.what());
    return result(env,std::string("Authored UI initialization failed: ")+e.what());
  }
  std::string report="Renderer: ";const auto* r=glGetString(GL_RENDERER);report+=r?reinterpret_cast<const char*>(r):"unknown";
  report+="\nGLES: ";const auto* v=glGetString(GL_VERSION);report+=v?reinterpret_cast<const char*>(v):"unknown";
  return result(env,report+errors("initialize"));
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_loadTexture(JNIEnv* env,jclass,jbyteArray input){
  if(!input)return result(env,"Null texture input");
  if(!ui_program.name)return result(env,"Renderer is unavailable; see DH2Native Logcat");
  const auto length=env->GetArrayLength(input);
  if(length<=0||length>32*1024*1024)return result(env,"Texture input outside size limit");
  std::vector<std::uint8_t> encoded(static_cast<std::size_t>(length));
  env->GetByteArrayRegion(input,0,length,reinterpret_cast<jbyte*>(encoded.data()));if(env->ExceptionCheck())return nullptr;
  dh2::textures::View view{};auto error=dh2_texture_open(encoded.data(),encoded.size(),&view);
  if(error!=dh2::textures::Error::ok)return result(env,dh2_texture_error(error));
  std::vector<std::uint8_t> rgba(std::size_t(view.width)*view.height*4);error=dh2_texture_decode(&view,rgba.data(),rgba.size());
  if(error!=dh2::textures::Error::ok)return result(env,dh2_texture_error(error));
  GLint limit=0;glGetIntegerv(GL_MAX_TEXTURE_SIZE,&limit);
  if(view.width>static_cast<unsigned>(limit)||view.height>static_cast<unsigned>(limit))return result(env,"Texture exceeds GPU limit");
  errors("before upload");GLuint candidate=0;glGenTextures(1,&candidate);glBindTexture(GL_TEXTURE_2D,candidate);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,view.width,view.height,0,GL_RGBA,GL_UNSIGNED_BYTE,rgba.data());
  const auto fault=errors("RGBA upload");if(!fault.empty()){glDeleteTextures(1,&candidate);return result(env,fault);}
  if(texture)glDeleteTextures(1,&texture);
  texture=candidate;texture_width=view.width;texture_height=view.height;
  report_texture_frame=true;
  model_renderer::deactivate();
  original_ui.deactivate();
  char report[256];std::snprintf(report,sizeof(report),"%u x %u | format %u | alpha %u | RGBA upload OK\nAspect ratio preserved",
    view.width,view.height,static_cast<unsigned>(view.format),view.alpha);
  return result(env,report);
}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_resize(JNIEnv*,jclass,jint w,jint h){
  surface_width=std::max(1,int(w));surface_height=std::max(1,int(h));glViewport(0,0,surface_width,surface_height);
  report_model_frame=true;
  report_texture_frame=true;
  __android_log_print(ANDROID_LOG_INFO,tag,"Surface resized to %d x %d",surface_width,surface_height);
}
bool model_renderer::borrow_application_time_v68(std::uint32_t& out,std::string& error){
 if(!application_services_v5||!application_frame_time_started_v93){
  error="Required actual NativeBridge frame timer publication";return false;
 }
 out=application_frame_time_v93;error.clear();return true;
}
bool model_renderer::borrow_native_surface_dimensions_v114(std::int32_t& width,std::int32_t& height,std::string& error){
 if(surface_width<=1||surface_height<=1){error="Required actual resized native render surface";return false;}
 width=surface_width;height=surface_height;error.clear();return true;
}
bool model_renderer::borrow_application_frame_v69(std::uint64_t& out,std::string& error){
 if(!application_services_v5||!application_frame_time_started_v93){error="Required actual NativeBridge draw publication";return false;}
 out=application_draw_epoch_v69;error.clear();return true;
}
bool model_renderer::borrow_application_frame_count_v68(std::uint32_t& out,std::string& error){
 if(!application_services_v5){error="Required actual Application frame74 owner";return false;}
 out=application_services_v5->source_loading_v55().frame74;error.clear();return true;
}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_draw(JNIEnv*,jclass){
  dh2::perf::Frame perf_frame;
  const auto frame_time=std::chrono::steady_clock::now().time_since_epoch();
  dh2::audio::AudioAuthoredEventScopeV46 authored_audio_time(
    std::chrono::duration_cast<std::chrono::nanoseconds>(frame_time).count());
  // ONE Application frame clock, including main/loading/character menus.
  // Preserve raw modulo32 milliseconds; source callers own long-gap policy.
  if(application_services_v5){
    const auto now=std::uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(frame_time).count());
    const auto elapsed=application_frame_time_started_v93?now-application_frame_time_v93:0u;
    application_frame_time_v93=now;application_frame_time_started_v93=true;
    ++application_draw_epoch_v69; // Native delivery provenance, not source frame74/Scene440.
    auto& fields=application_services_v5->source_loading_v55();
    fields.dt8c=static_cast<std::uint32_t>(elapsed);fields.native_dt_produced_v93=true;
  }
  glClearColor(0.08f,0.09f,0.11f,1);glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  if(native_process_startup_v119&&!native_process_ready_v119()){
    std::string error;
    if(!tick_native_process_startup_v119(error)){
      const auto failure="Source process startup: "+error;
      if(original_ui_error!=failure)__android_log_print(ANDROID_LOG_ERROR,tag,"%s",failure.c_str());
      original_ui_error=failure;
    }
    //GSInit draws its real retained texture; it never advances Front/World or
    //admits Play while startup/intro still owns the frame.
    if(!front_ui.render_process_splash_v119(surface_width,surface_height,error)&&original_ui_error.empty())original_ui_error=error;
    return;
  }
  // ONE actual Application queue update per native frame, including menus.
  // The process owner survives individual Worlds and GL resource generations.
  if(application_services_v5&&application_services_v5->source_save_files_v61()){
    std::string failure;bool progressed=false;
    try{
      if(!application_services_v5->source_save_files_v61()->update(progressed,failure)&&failure.empty())
        failure="Actual Application save-job update failed";
    }catch(const std::exception& error){failure=error.what();}
    catch(...){failure="Actual Application save-job update threw";}
    if(!failure.empty()){
      original_ui_error="Save jobs: "+failure;
      __android_log_print(ANDROID_LOG_ERROR,tag,"%s",original_ui_error.c_str());return;
    }
  }
  // Drain after the previous dispatch returned, never inside its delivery
  // scope. Pending cancellation/receipts keep the SAME retirement request.
  if(drain_native_source_main_menu_frame_v114())return;
  if(drain_native_area_transition_frame_v114())return;
  if(model_renderer::source_campaign_retirement_requested_v88()){
    std::string error;
    const auto status=model_renderer::drain_source_campaign_retirement_v88(error);
    if(status==model_renderer::SourceCampaignRetirementResultV88::failed){
      const auto failure="Source campaign retirement: "+error;
      if(original_ui_error!=failure)__android_log_print(ANDROID_LOG_ERROR,tag,"%s",failure.c_str());
      original_ui_error=failure;
    }
    return;
  }
  if(model_renderer::source_campaign_active_v55()){
    std::string error;
    if(!model_renderer::tick_source_campaign_v55(error)){
      const auto failure=std::string(model_renderer::source_campaign_scene_active_v67()?"Source campaign update: ":"Source campaign loading: ")+error;
      if(original_ui_error!=failure)__android_log_print(ANDROID_LOG_ERROR,tag,"%s",failure.c_str());
      original_ui_error=failure; // Retain the reached source prefix and loading UI.
    }else{
      // Original _Update increments SAME App74 after gameplay, before its
      // common return. Collision callbacks in this tick observe the old word;
      // unsigned wrap remains original. Failed native prefixes do not complete
      // that tail, and neither Scene440 nor animation milliseconds substitute.
      ++application_services_v5->source_loading_v55().frame74;
    }
    if(model_renderer::source_campaign_loading_complete_v64()&&!model_renderer::source_campaign_scene_active_v67()){
      NativeCampaignDeliveryV104 activation_delivery(model_renderer::SourceCampaignDeliveryKindV104::draw);
      if(!activation_delivery)return;
      if(!model_renderer::activate_source_campaign_scene_v67(gameplay_assets,error)){
        const auto failure="Source scene activation: "+error;
        if(original_ui_error!=failure)__android_log_print(ANDROID_LOG_ERROR,tag,"%s",failure.c_str());
        original_ui_error=failure;
      }else{
        original_ui_error.clear();front_launch_report="Native campaign | loaded scene, selected player, camera and HUD connected";
      }
    }
  }
  // Actual returned GSFlashMenu only. Gameplay/loading retain their existing
  // source MenuManager update, while Front drawing never advances timelines.
  if(!model_renderer::source_campaign_active_v55()&&front_ui.main_menu_state_active_v114()){
    std::string error;
    if(!front_ui.update_main_menu_request_v114(error)||!authored_character_menu||
       !authored_character_menu->source_update_v58||
       !authored_character_menu->source_update_v58->update(false,error)){
      if(error.empty())error="Required same process GSFlashMenu/MenuManager frame owner";
      original_ui_error="Source main-menu frame: "+error;return;
    }
  }
  // Restore after the source update so its scene phase samples this frame's
  // player/body/camera state, rather than consuming the epoch before gameplay.
  NativeCampaignDeliveryV104 draw_delivery(model_renderer::SourceCampaignDeliveryKindV104::draw);
  if(!draw_delivery)return;
  if(native_source_scene_v67.gpu_restore_pending){
    std::string error;
    if(!restore_native_source_gpu_v68(gameplay_assets,error)){
      const auto failure="Source GPU restoration: "+error;
      if(original_ui_error!=failure)__android_log_print(ANDROID_LOG_ERROR,tag,"%s",failure.c_str());
      original_ui_error=failure;return;
    }
  }
  const auto render_phase=dh2::android_ui::front_loading_render_phase_v1(
    model_renderer::source_campaign_active_v55(),model_renderer::source_campaign_scene_active_v67());
  if(render_phase==dh2::android_ui::FrontLoadingRenderPhaseV1::source_loading){
    std::string error;
    if(!front_ui.render_game_loading(surface_width,surface_height,error)){
      const auto failure="Source loading menu render: "+error;
      __android_log_print(ANDROID_LOG_ERROR,tag,"%s",failure.c_str());
      // Preserve the first reached loader error, such as Stage 10, as the
      // user-visible status while still reporting renderer failures in logcat.
      if(original_ui_error.empty())original_ui_error=failure;
    }
    return;
  }
  if(render_phase==dh2::android_ui::FrontLoadingRenderPhaseV1::gameplay){
    front_ui.deactivate(); // Only the actual scene handoff retires front drawing.
    std::string error;
    if(authored_character_menu&&authored_character_menu->opened){
      if(!authored_character_menu->frame(error))original_ui_error="Character menu: "+error;
      return;
    }
    if(!model_renderer::draw_source_campaign_geometry_v64(surface_width,surface_height,error)||
       !original_ui.render_source_player_v67(surface_width,surface_height,error)){
      const auto failure="Source gameplay draw: "+error;
      if(original_ui_error!=failure)__android_log_print(ANDROID_LOG_ERROR,tag,"%s",failure.c_str());
      original_ui_error=failure;
    }
    return;
  }
  if(front_ui.active()){
    dh2::perf::Scope perf_front(dh2::perf::Phase::front);
    std::string error;
    if(!front_ui.render(surface_width,surface_height,error)){
      original_ui_error="Main menu: "+error;
      __android_log_print(ANDROID_LOG_ERROR,tag,"Merged main menu failed: %s",error.c_str());
      return;
    }
    dh2::android_ui::FrontUiSessionV87::LaunchRequest request;
    if(front_ui.consume_launch_request(request)){
      if(!native_process_ready_v119()){
        original_ui_error="Play requires completed original first-boot process initialization";
        front_launch_report=original_ui_error;return;
      }
      // Source MainMenu::Hide removes its preview scene and avatar, then
      // flushes the SAME process ObjectManager/AnimSetManager before campaign
      // World construction replaces the service receiver.
      if(!model_renderer::native_menu_preview_main_hide_v121(application_services_v5,error)){
        original_ui_error="Source campaign menu teardown: "+error;
        front_launch_report=original_ui_error;
        __android_log_print(ANDROID_LOG_ERROR,tag,"%s",original_ui_error.c_str());return;
      }
      // Consume after AS dispatch returns. Source GS constructs a fresh
      // candidate from the exact selected profile; it never loads the Crypt
      // descriptor or creates a development player/gear authority.
      if(!model_renderer::start_source_campaign_v55(gameplay_assets,request.profile,error)){
        original_ui_error="Source campaign start: "+error;
        front_launch_report=original_ui_error;
        __android_log_print(ANDROID_LOG_ERROR,tag,"%s",original_ui_error.c_str());return;
      }
      original_ui.deactivate();original_ui_error.clear();
      front_launch_report="Source campaign | selected slot "+std::to_string(request.slot)+" | actual GS/C1 loading";
      __android_log_print(ANDROID_LOG_INFO,tag,"Main menu Play -> actual selected-profile GS | slot %d | difficulty %d | source stages pending",request.slot,request.difficulty);
    }
    return;
  }
  if(original_ui.active()&&!original_ui.overlays_player()){
    std::string error;
    if(!original_ui.render(surface_width,surface_height,error)){
      original_ui_error=error;
      __android_log_print(ANDROID_LOG_ERROR,tag,"Original health panel failed: %s",error.c_str());
    }
    return;
  }
  if(authored_character_menu&&authored_character_menu->opened){
    std::string error;if(!authored_character_menu->frame(error)){
      const auto budget=dh2::android_resources::budget_lease_v39();const auto usage=budget->snapshot();
      __android_log_print(ANDROID_LOG_ERROR,tag,"Character render budget | liveGPU %llu pendingGPU %llu liveTexture %llu pendingTexture %llu peakGPU %llu CPU %llu textureLimit %llu GPUlimit %llu | admitted storage only",
        static_cast<unsigned long long>(usage.live.gpu_bytes),static_cast<unsigned long long>(usage.pending.gpu_bytes),
        static_cast<unsigned long long>(usage.live.texture_gpu_bytes),static_cast<unsigned long long>(usage.pending.texture_gpu_bytes),
        static_cast<unsigned long long>(usage.peak.gpu_bytes),static_cast<unsigned long long>(usage.live.cpu_bytes),
        static_cast<unsigned long long>(budget->limits().texture_gpu_bytes),static_cast<unsigned long long>(budget->limits().gpu_bytes));
      original_ui_error="Character menu: "+error;
      __android_log_print(ANDROID_LOG_ERROR,tag,"Original character menu frame failed: %s",error.c_str());
      authored_character_menu->close();
    }
    return;
  }
  // The menu preview renderer is still retained while GS builds its loading
  // scene. Main.Hide has already retired its preview camera, so do not submit
  // that stale menu renderer during the campaign loading interval.
  if(model_renderer::active()&&!model_renderer::source_campaign_active_v55()){
    try {
      if(original_ui.overlays_player()){
        dh2::perf::Scope perf_prepare(dh2::perf::Phase::hud_prepare);
        std::string error;if(!original_ui.prepare_player_frame(surface_width,surface_height,error))throw std::runtime_error("Combat HUD preparation: "+error);
        model_renderer::connect_combat_text(original_ui.combat_text_sink());
      }
      model_renderer::draw(surface_width,surface_height);
    } catch(const std::exception& e) {
      __android_log_print(ANDROID_LOG_ERROR,tag,"Native frame failed: %s",e.what());
      original_ui_error=std::string("Gameplay failed: ")+e.what();
      original_ui.deactivate();
      model_renderer::deactivate();
      return;
    } catch(...) {
      __android_log_print(ANDROID_LOG_ERROR,tag,"Native frame failed: unknown exception");
      original_ui_error="Gameplay failed: unknown native exception";
      original_ui.deactivate();
      model_renderer::deactivate();
      return;
    }
    if(report_model_frame){__android_log_print(ANDROID_LOG_INFO,tag,"Model frame submitted at %d x %d",surface_width,surface_height);report_model_frame=false;}
    if(original_ui.overlays_player()){
      dh2::perf::Scope perf_hud(dh2::perf::Phase::hud);
      const auto player=model_renderer::player_hud_view();std::string error;
      const auto enemy=model_renderer::enemy_hud_world_borrow(error);
      if(!enemy.player||!original_ui.render_player(surface_width,surface_height,player.resolved,player.count,player.character,error,&enemy)){
        original_ui_error=error;
        __android_log_print(ANDROID_LOG_ERROR,tag,"Connected player HUD failed: %s",error.c_str());
        return; // preserve the first failure; HUD resources were released
      }
      model_renderer::CombatTextFrameV1 frame;
      dh2::perf::Scope perf_combat_text(dh2::perf::Phase::combat_text);
      if(!model_renderer::combat_text_frame(frame,error)||!original_ui.render_combat_text(frame,error)){
        original_ui_error=error;__android_log_print(ANDROID_LOG_ERROR,tag,"Source combat text failed: %s",error.c_str());
      }
    }
    return;
  }
  if(!ui_program.name||!texture)return;
  const float image_aspect=float(texture_width)/texture_height,viewport_aspect=float(surface_width)/surface_height;
  const float sx=std::min(1.0f,image_aspect/viewport_aspect),sy=std::min(1.0f,viewport_aspect/image_aspect);
  const std::array<float,16> matrix{sx,0,0,0,0,sy,0,0,0,0,1,0,0,0,0,1};
  glDisable(GL_DEPTH_TEST);glDisable(GL_CULL_FACE);glDepthMask(GL_FALSE);
  glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  try{dh2::android_ui::draw_quad(ui_program,texture,matrix,{1,1,1,1},{0,0,0,0});}
  catch(const std::exception& e){__android_log_print(ANDROID_LOG_ERROR,tag,"Authored UI draw failed: %s",e.what());return;}
  glDepthMask(GL_TRUE);const auto fault=errors("authored UI draw");
  if(report_texture_frame&&fault.empty()){
    __android_log_print(ANDROID_LOG_INFO,tag,"Authored UI texture frame submitted | viewport %d %d | texture %d %d | scale %.7f %.7f | GameSWF normal",surface_width,surface_height,texture_width,texture_height,sx,sy);
    report_texture_frame=false;
  }
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_loadModel(JNIEnv* env,jclass,jbyteArray input,jobject assets){
  if(!input||!assets)return result(env,"Null model input");
  const auto n=env->GetArrayLength(input);if(n<=0||n>32*1024*1024)return result(env,"Model size outside limit");
  std::vector<std::uint8_t> bytes(n);env->GetByteArrayRegion(input,0,n,reinterpret_cast<jbyte*>(bytes.data()));if(env->ExceptionCheck())return nullptr;
  report_model_frame=true;
  original_ui.deactivate();
  return result(env,model_renderer::load(bytes.data(),bytes.size(),AAssetManager_fromJava(env,assets)));
}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_orbit(JNIEnv*,jclass,jfloat dx,jfloat dy,jfloat zoom){model_renderer::orbit(dx,dy,zoom);}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_animationTime(JNIEnv*,jclass,jint milliseconds){model_renderer::set_time(milliseconds);}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_characterPanelOpen(JNIEnv*,jclass,jboolean open){
 NativeCampaignDeliveryV104 delivery(model_renderer::SourceCampaignDeliveryKindV104::input);
 if(delivery)model_renderer::set_character_panel_open(open==JNI_TRUE);
}
extern "C" JNIEXPORT jboolean JNICALL Java_com_example_dh2_NativeBridge_authoredCharacterMenuIsOpen(JNIEnv*,jclass){
 return authored_character_menu&&authored_character_menu->opened?JNI_TRUE:JNI_FALSE;
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_resourceBudgetReport(JNIEnv* env,jclass){
 try{return result(env,dh2::android_resources::budget_report_v46());}
 catch(const std::exception& failure){__android_log_print(ANDROID_LOG_ERROR,tag,"Resource report failed: %s",failure.what());return nullptr;}
}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_debugCastProbe(JNIEnv*,jclass,jboolean enabled){model_renderer::debug_cast_probe_v46(enabled==JNI_TRUE);}
extern "C" JNIEXPORT jboolean JNICALL Java_com_example_dh2_NativeBridge_authoredCharacterMenuBack(JNIEnv*,jclass){
 NativeCampaignDeliveryV104 delivery(model_renderer::SourceCampaignDeliveryKindV104::input);
 if(!delivery)return JNI_TRUE;
 if(!authored_character_menu||!authored_character_menu->opened)return JNI_FALSE;
 std::string error;
 if(!character_panel->authored_back(model_renderer::player_gameplay_binding(),error))__android_log_print(ANDROID_LOG_ERROR,tag,"Original character menu Back failed: %s",error.c_str());
 return JNI_TRUE;
}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_moveAxis(JNIEnv*,jclass,jfloat x,jfloat y){
 NativeCampaignDeliveryV104 delivery(model_renderer::SourceCampaignDeliveryKindV104::input);
 if(delivery)model_renderer::move_axis(x,y);
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_authoredHudTouch(JNIEnv* env,jclass,jint phase,jint pointer,jfloat x,jfloat y){
  if(native_process_startup_v119&&!native_process_ready_v119())return nullptr;
  NativeCampaignDeliveryV104 delivery(model_renderer::SourceCampaignDeliveryKindV104::input);
  if(!delivery)return delivery.error.empty()?nullptr:result(env,delivery.error);
  if(native_process_startup_v119){std::string error;if(!dispatch_source_app_touch_v121(phase,pointer,x,y,error))return result(env,error);return nullptr;}
  // Direct authored input has its own measured dispatch time. Animation
  // event receivers may nest a scope carrying their actual authored lag.
  dh2::audio::AudioAuthoredEventScopeV46 authored_audio_time(std::chrono::duration_cast<std::chrono::nanoseconds>(
    std::chrono::steady_clock::now().time_since_epoch()).count());
  if(authored_character_menu&&authored_character_menu->opened){
    std::string error;if(!authored_character_menu->pointer(phase,pointer,x,y,error)){
      __android_log_print(ANDROID_LOG_ERROR,tag,"Original character menu input failed: %s",error.c_str());return result(env,"Character menu input failed: "+error);
    }
    return nullptr;
  }
  std::string command,error;if(!original_ui.hud_pointer(phase,pointer,x,y,command,error)){
    __android_log_print(ANDROID_LOG_ERROR,tag,"Authored HUD input failed: %s",error.c_str());return result(env,"HUD input failed: "+error);
  }
  if(command=="character"){
    std::string cancelled;
    if(!original_ui.hud_pointer(3,-1,0,0,cancelled,error))return result(env,"HUD cancellation failed: "+error);
    const auto live=model_renderer::player_gameplay_binding();
    if(authored_character_menu&&authored_character_menu->leases->world!=live.world_owner){
      if(authored_character_menu->process_directory_ready_v104)return result(env,"Profile icon requires the current admitted campaign on the retained process menu");
      if(authored_character_menu&&!authored_character_menu->release_flash_for_teardown_v93(error))return result(env,error);
      character_panel->reset_authored_v4();authored_character_menu.reset();
    }
    if(!authored_character_menu)authored_character_menu=std::make_unique<NativeCharacterMenuV4>();
    if(!authored_character_menu->open(error)){
      __android_log_print(ANDROID_LOG_ERROR,tag,"Original character menu open failed: %s",error.c_str());
      return result(env,"Character menu failed: "+error);
    }
    return nullptr;
  }
  return command.empty()?nullptr:result(env,command);
}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_focusObject(JNIEnv*,jclass,jint index){
 NativeCampaignDeliveryV104 delivery(model_renderer::SourceCampaignDeliveryKindV104::input);
 if(delivery)model_renderer::focus_object(index);
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_objectState(JNIEnv* env,jclass,jint index,jstring state){
 NativeCampaignDeliveryV104 delivery(model_renderer::SourceCampaignDeliveryKindV104::input);
 if(!delivery)return delivery.error.empty()?nullptr:result(env,delivery.error);
 if(!state)return env->NewStringUTF("Actor state is absent");
 const char* raw=env->GetStringUTFChars(state,nullptr);if(!raw)return nullptr;
 const std::string name(raw);env->ReleaseStringUTFChars(state,raw);
 return env->NewStringUTF(model_renderer::set_object_state(index,name).c_str());
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_loadWorld(JNIEnv* env,jclass,jbyteArray input,jobject assets,jstring files_directory,jstring selected_mlx){
  if(!input||!assets||!files_directory||!selected_mlx)return result(env,"Null world input");const auto n=env->GetArrayLength(input);
  if(n<=0||n>65560)return result(env,"World descriptor outside limit");
  std::vector<std::uint8_t> bytes(n);env->GetByteArrayRegion(input,0,n,reinterpret_cast<jbyte*>(bytes.data()));if(env->ExceptionCheck())return nullptr;
  const char* directory=env->GetStringUTFChars(files_directory,nullptr);if(!directory)return nullptr;
  const std::string directory_path(directory);env->ReleaseStringUTFChars(files_directory,directory);
  const char* layout=env->GetStringUTFChars(selected_mlx,nullptr);if(!layout)return nullptr;
  const std::string layout_path(layout);env->ReleaseStringUTFChars(selected_mlx,layout);
  original_ui.deactivate();
  original_ui_error.clear();report_model_frame=true;
  auto report=model_renderer::load_world(bytes.data(),bytes.size(),AAssetManager_fromJava(env,assets),directory_path,layout_path);
  if(model_renderer::active()&&report.find("failed")==std::string::npos&&report.find("error")==std::string::npos){
    std::string error;
    if(!original_ui.attach_player(directory_path,error))report+="\nConnected HUD failed: "+error;
    else report+="\nOriginal player status HUD connected to world";
  }
  return result(env,report);
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_loadOriginalHealthPanel(JNIEnv* env,jclass,jstring files_directory){
  original_ui_error.clear();
  if(!files_directory)return result(env,"Original HUD failed: private directory unavailable");
  const char* raw=env->GetStringUTFChars(files_directory,nullptr);if(!raw)return nullptr;
  const std::string directory(raw);env->ReleaseStringUTFChars(files_directory,raw);
  std::string error;
  if(!original_ui.load_health_panel(directory,error))return result(env,"Original HUD failed: "+error);
  model_renderer::deactivate();
  return result(env,"Original health/mana panel | authored initial state\nNative SWF, original textures, fonts and text\nGame updates and original viewport/input still pending");
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_consumeOriginalUiError(JNIEnv* env,jclass){
  if(original_ui_error.empty())return nullptr;
  const auto error=std::move(original_ui_error);original_ui_error.clear();return result(env,error);
}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_configureFrontDevice(JNIEnv* env,jclass,jstring manufacturer,jstring model){
 auto copy=[&](jstring input){if(!input)return std::string();const char* raw=env->GetStringUTFChars(input,nullptr);if(!raw)return std::string();std::string out(raw);env->ReleaseStringUTFChars(input,raw);return out;};
 android_manufacturer_v20=copy(manufacturer);android_device_configured_v20=true;
 front_ui.bind_menu_device(android_manufacturer_v20,copy(model),8);
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_loadOriginalFrontScreen(JNIEnv* env,jclass,jstring directory,jstring screen){
 if(!directory||!screen)return result(env,"Main menu failed: private directory or screen absent");
 const char* d=env->GetStringUTFChars(directory,nullptr);if(!d)return nullptr;
 front_directory=d;env->ReleaseStringUTFChars(directory,d);
 const char* s=env->GetStringUTFChars(screen,nullptr);if(!s)return nullptr;
 const std::string selected(s);env->ReleaseStringUTFChars(screen,s);
 if(selected=="main"){
  std::string error;
  if(native_process_startup_v119&&native_process_ready_v119())return result(env,"Original process main menu already initialized");
  original_ui.deactivate();model_renderer::deactivate();front_launch_report.clear();original_ui_error.clear();
  if(!begin_native_process_startup_v119(error))return result(env,"Source first-boot startup failed: "+error);
  return result(env,"Original process startup | data, intro, audio, trophies and menus initializing");
 }
 original_ui.deactivate();model_renderer::deactivate();front_launch_report.clear();original_ui_error.clear();
 // Explicit preview mode until canonical SG_Load4 and saved equipment are integrated.
 front_ui.demo_persona_mode();std::string error;
 if(!front_ui.load_front_screen(front_directory,selected,error))return result(env,"Main menu failed: "+error);
 if(!front_ui.prepare_front_resources_v114(surface_width,surface_height,error))
  return result(env,"Main menu resource loading failed: "+error);
 std::shared_ptr<dh2::ui::OwnedHudSettingsV1> actual_settings;
 if(!front_ui.gameplay_settings_borrow_v67(actual_settings,error)||!application_services_v5||
    !application_services_v5->publish_source_settings4c_v67(actual_settings,error))
  return result(env,"Main menu settings publication failed: "+error);
 std::shared_ptr<dh2::application::ApplicationSaveFilesOwnerV61> files;
 if(!dh2::application::ApplicationSaveFilesOwnerV61::acquire(application_services_v5,front_directory,files,error)||
    !front_ui.bind_profile_application_v114(application_services_v5,error))
  return result(env,"Main menu profile launch binding failed: "+error);
 return result(env,"Original main menu ready");
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_originalMenuTouch(JNIEnv* env,jclass,jfloat x,jfloat y,jint action){
 if(native_process_startup_v119&&!native_process_ready_v119())return nullptr;
 NativeCampaignDeliveryV104 delivery(model_renderer::SourceCampaignDeliveryKindV104::input);
 if(!delivery)return delivery.error.empty()?nullptr:result(env,delivery.error);
 dh2::audio::AudioAuthoredEventScopeV46 authored_audio_time(std::chrono::duration_cast<std::chrono::nanoseconds>(
   std::chrono::steady_clock::now().time_since_epoch()).count());
 std::string error;
 if(native_process_startup_v119){if(!dispatch_source_app_touch_v121(action==1?2:action==2?1:action==3?3:0,0,x,y,error))return result(env,error);return nullptr;}
 if(!front_ui.touch(x,y,action,error))return result(env,error);return nullptr;
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_originalMenuPointer(JNIEnv* env,jclass cls,jint phase,jint pointer,jfloat x,jfloat y){
 if(native_process_startup_v119)return Java_com_example_dh2_NativeBridge_authoredHudTouch(env,cls,phase,pointer,x,y);
 // Inspection uses its existing direct movie adapter, not App source events.
 if(pointer>0)return nullptr;
 return Java_com_example_dh2_NativeBridge_originalMenuTouch(env,cls,x,y,phase==1?2:phase==2?1:phase);
}
#if defined(DH2_NATIVE_HOST_BUILD) && DH2_NATIVE_HOST_BUILD
//Observation only: the desktop harness executes the SAME JNI init/draw/input
//entrypoints. Exposing retained phase counters never bypasses a source stage.
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_HostRunner_snapshot(JNIEnv* env,jclass){
 const auto startup=native_process_startup_v119;
 const auto stage=startup&&startup->source?startup->source->stage():-1;
 const auto menu=startup?std::int64_t(startup->menu_stage_bc):-1;
 std::string active="[";bool first=true;
 if(authored_character_menu&&authored_character_menu->shared_stack_v27){
  const auto* stack=authored_character_menu->shared_stack_v27->view();
  if(stack&&stack->renders&&stack->count<=stack->capacity)for(std::uint32_t i=0;i<stack->count;++i){
   const auto* render=stack->renders[i];if(!render||!render->states||render->count>render->capacity)continue;
   for(std::uint32_t j=0;j<render->count;++j){const auto* state=render->states[j];if(!state||!state->name)continue;
    if(!first)active+=",";first=false;active+='"';
    for(const char* p=state->name;*p;++p){if(*p=='"'||*p=='\\')active+='\\';active+=*p;}active+='"';
   }
  }
 }
 active+="]";
 return result(env,"{\"gsinit_stage\":"+std::to_string(stage)+",\"menu_init_phase\":"+std::to_string(menu)+
  ",\"startup_ready\":"+(native_process_ready_v119()?"true":"false")+
  ",\"main_state_active\":"+(front_ui.main_menu_state_active_v114()?"true":"false")+",\"active_menus\":"+active+"}");
}
#endif
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_consumeOriginalMenuAudio(JNIEnv* env,jclass){auto value=front_ui.consume_menu_audio();return value.empty()?nullptr:result(env,value);}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_consumeOriginalMenuSound(JNIEnv* env,jclass){auto value=front_ui.consume_menu_sound();return value.empty()?nullptr:result(env,value);}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_consumeFrontLaunch(JNIEnv* env,jclass){if(front_launch_report.empty())return nullptr;const auto value=std::move(front_launch_report);front_launch_report.clear();return result(env,value);}
extern "C" JNIEXPORT jboolean JNICALL Java_com_example_dh2_NativeBridge_sourceCampaignSceneActive(JNIEnv*,jclass){return model_renderer::source_campaign_scene_active_v67()?JNI_TRUE:JNI_FALSE;}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_combatTarget(JNIEnv* env,jclass,jint index,jint target){
 NativeCampaignDeliveryV104 delivery(model_renderer::SourceCampaignDeliveryKindV104::input);
 if(!delivery)return delivery.error.empty()?nullptr:result(env,delivery.error);
 return result(env,model_renderer::set_combat_target(index,target));
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_playerAttack(JNIEnv* env,jclass,jint target){
 NativeCampaignDeliveryV104 delivery(model_renderer::SourceCampaignDeliveryKindV104::input);
 if(!delivery)return delivery.error.empty()?nullptr:result(env,delivery.error);
 return result(env,model_renderer::player_attack(target));
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_playerEquipmentAction(JNIEnv* env,jclass,jint operation,jint index,jint slot){
 NativeCampaignDeliveryV104 delivery(model_renderer::SourceCampaignDeliveryKindV104::input);
 if(!delivery)return delivery.error.empty()?nullptr:result(env,delivery.error);
 return result(env,model_renderer::player_equipment_action(operation,index,slot));
}

extern "C" JNIEXPORT jintArray JNICALL Java_com_example_dh2_NativeBridge_playerVitals(JNIEnv* env,jclass){
 NativeCampaignDeliveryV104 delivery(model_renderer::SourceCampaignDeliveryKindV104::input);
 if(!delivery)return nullptr;
 auto values=model_renderer::player_vitals();auto out=env->NewIntArray(values.size());if(out)env->SetIntArrayRegion(out,0,values.size(),values.data());return out;
}
extern "C" JNIEXPORT jintArray JNICALL Java_com_example_dh2_NativeBridge_playerGameplayHud(JNIEnv* env,jclass){
  NativeCampaignDeliveryV104 delivery(model_renderer::SourceCampaignDeliveryKindV104::input);
  if(!delivery)return nullptr;
  auto values=model_renderer::gameplay_hud_snapshot(model_renderer::player_gameplay_binding());
  auto out=env->NewIntArray(static_cast<jsize>(values.size()));if(out&&!values.empty())env->SetIntArrayRegion(out,0,static_cast<jsize>(values.size()),values.data());return out;
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_playerGameplayAction(JNIEnv* env,jclass,jint operation,jint index){
 NativeCampaignDeliveryV104 delivery(model_renderer::SourceCampaignDeliveryKindV104::input);
 if(!delivery)return delivery.error.empty()?nullptr:result(env,delivery.error);
 return result(env,model_renderer::player_gameplay_action(operation,index));
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_playerCharacterSnapshot(JNIEnv* env,jclass){
 NativeCampaignDeliveryV104 delivery(model_renderer::SourceCampaignDeliveryKindV104::input);
 if(!delivery)return nullptr;
 return result(env,character_panel?character_panel->snapshot(model_renderer::player_gameplay_binding()):"{\"ready\":false,\"error\":\"Character menu unavailable\"}");
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_playerCharacterAction(JNIEnv* env,jclass,jint operation,jint index,jint slot){
 NativeCampaignDeliveryV104 delivery(model_renderer::SourceCampaignDeliveryKindV104::input);
 if(!delivery)return delivery.error.empty()?nullptr:result(env,delivery.error);
 return result(env,character_panel?character_panel->action(model_renderer::player_gameplay_binding(),operation,index,slot):"Character menu unavailable");
}
extern "C" JNIEXPORT jintArray JNICALL Java_com_example_dh2_NativeBridge_menuIcon(JNIEnv* env,jclass,jstring name){
  if(!name||!gameplay_assets)return nullptr;const char* raw=env->GetStringUTFChars(name,nullptr);if(!raw)return nullptr;
  const std::string value(raw);env->ReleaseStringUTFChars(name,raw);std::string error;
  auto pixels=dh2::android_ui::menu_icon_pixels(gameplay_assets,value,error);
  if(pixels.empty()){if(!error.empty())__android_log_print(ANDROID_LOG_WARN,tag,"Original icon unavailable | %s | %s",value.c_str(),error.c_str());return nullptr;}
  auto out=env->NewIntArray(static_cast<jsize>(pixels.size()));if(out)env->SetIntArrayRegion(out,0,static_cast<jsize>(pixels.size()),pixels.data());return out;
}
extern "C" JNIEXPORT jobjectArray JNICALL Java_com_example_dh2_NativeBridge_playerGameplayIcons(JNIEnv* env,jclass){
  NativeCampaignDeliveryV104 delivery(model_renderer::SourceCampaignDeliveryKindV104::input);
  if(!delivery)return nullptr;
  auto names=model_renderer::gameplay_hud_icon_names(model_renderer::player_gameplay_binding());
  auto string_class=env->FindClass("java/lang/String");if(!string_class)return nullptr;
  auto out=env->NewObjectArray(static_cast<jsize>(names.size()),string_class,nullptr);
  if(out)for(std::size_t i=0;i<names.size();++i){auto item=env->NewStringUTF(names[i].c_str());if(!item)return nullptr;env->SetObjectArrayElement(out,static_cast<jsize>(i),item);env->DeleteLocalRef(item);}env->DeleteLocalRef(string_class);return out;
}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_enemyAi(JNIEnv*,jclass,jboolean enabled){
 NativeCampaignDeliveryV104 delivery(model_renderer::SourceCampaignDeliveryKindV104::input);
 if(delivery)model_renderer::set_enemy_ai(enabled);
}
