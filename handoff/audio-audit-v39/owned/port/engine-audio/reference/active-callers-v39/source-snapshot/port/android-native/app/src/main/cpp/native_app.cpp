#include <jni.h>
#include "frame_perf_v35.hpp"
#include "native_resource_budget_v38.hpp"
#include <android/log.h>
#include <GLES2/gl2.h>
#include "textures.hpp"
#include "model_renderer.hpp"
#include "authored_shader_program.hpp"
#include "original_ui_session.hpp"
#include "front_ui_session_v87.hpp"
#include "character_panel_session_v1.hpp"
#include "gameplay_hud.hpp"
#include "gameplay_icons.hpp"
#include "authored_character_panel_platform_v4.hpp"
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
bool model_renderer::borrow_actual_menu_device_v1(dh2::ui::MenuDeviceFactsV1& facts,std::string& error){
 return front_ui.menu_device_borrow_v4(facts,error);
}
bool model_renderer::borrow_actual_application_services_v5(std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& out,std::string& error){
 if(!application_services_v5||!application_services_v5->events14()){
  error="Required actual Application PostInit EventManager publication";return false;
 }
 out=application_services_v5;error.clear();return true;
}
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
#include "native_gslevel_menu_v27.inc"
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_buildInfo(JNIEnv* env,jclass) {
  return result(env,"Native source reconstruction: animated scene nodes");
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_initialize(JNIEnv* env,jclass bridge,jobject assets){
  // Android has created a new context: previous GL names belong to the old
  // context and must not be deleted against this context's reused names.
  dh2::android_resources::context_lost_v38();
  model_renderer::reset_context();ui_program={};texture=0;report_texture_frame=true;
  dh2::android_ui::Program premultiplied;
  try{
     dh2::android_resources::begin_context_v38();
     // Service lifetime is the native Application process, not a GL context.
     // This source-backed prefix reconstructs only pointer14 publication;
     // complete Application PostInit/Shutdown remain separate work.
     if(!application_services_v5)application_services_v5=std::make_shared<dh2::application::ApplicationServicesOwnerV5>();
     std::string application_error;
     if(!application_services_v5->events14()&&!application_services_v5->post_init_events_v5(application_error))throw std::runtime_error(application_error);
     auto* manager=assets?AAssetManager_fromJava(env,assets):nullptr;
      gameplay_assets=manager;
      character_panel.reset();authored_character_menu.reset();
    ui_program=dh2::android_ui::create(manager,false);
    premultiplied=dh2::android_ui::create(manager,true);
    dh2::android_ui::validate_pixels(ui_program,premultiplied);
    dh2::android_ui::release(premultiplied);
    std::string ui_error;
     if(!original_ui.initialize(manager,ui_error))throw std::runtime_error(ui_error);
     if(!front_ui.initialize(manager,ui_error))throw std::runtime_error(ui_error);
     auto music=std::make_shared<MusicPlatformV1>();
     if(env->GetJavaVM(&music->vm)!=JNI_OK)throw std::runtime_error("Required Android music JavaVM");
     music->receiver=static_cast<jclass>(env->NewGlobalRef(bridge));
     music->method=env->GetStaticMethodID(bridge,"isSupportMM","()I");
     if(!music->receiver||!music->method||env->ExceptionCheck()){env->ExceptionClear();throw std::runtime_error("Required original Android isSupportMM receiver");}
     original_ui.bind_platform_music([music](std::int32_t& value,std::string& error){return music->query(value,error);});
      character_panel=std::make_unique<dh2::android_ui::CharacterPanelSessionV1>(manager);
  }catch(const std::exception& e){
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
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_draw(JNIEnv*,jclass){
  dh2::perf::Frame perf_frame;
  glClearColor(0.08f,0.09f,0.11f,1);glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
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
      // This explicit development destination is the current Crypt demo.
      // Consume after AS dispatch returns; no movie is destroyed in callback.
      AAsset* descriptor=AAssetManager_open(gameplay_assets,"worlds/crypt01.dwld",AASSET_MODE_BUFFER);
      if(!descriptor){original_ui_error="Crypt demo descriptor unavailable";return;}
      const auto length=AAsset_getLength64(descriptor);
      std::vector<std::uint8_t> bytes;
      if(length>0&&length<=65560)bytes.resize(static_cast<std::size_t>(length));
      const bool read=!bytes.empty()&&AAsset_read(descriptor,bytes.data(),bytes.size())==static_cast<int>(bytes.size());
      AAsset_close(descriptor);
      if(!read){original_ui_error="Crypt demo descriptor read failed";return;}
      front_ui.deactivate();model_renderer::deactivate();
      original_ui.deactivate();original_ui_error.clear();report_model_frame=true;
      // Same authored layout recorded by crypt01-provenance.json. The main
      // menu's current development destination is explicitly this Crypt.
      front_launch_report=model_renderer::load_world(bytes.data(),bytes.size(),gameplay_assets,front_directory,"data/scene/x07_crypt_backup.mlx");
      if(!model_renderer::active()||front_launch_report.find("failed")!=std::string::npos){original_ui_error=front_launch_report;return;}
      if(!original_ui.attach_player(front_directory,error)){original_ui_error=error;return;}
      front_launch_report="Crypt demo | selected menu slot "+std::to_string(request.slot)+" | campaign restore pending\n"+front_launch_report;
      __android_log_print(ANDROID_LOG_INFO,tag,"Main menu Play -> Crypt demo | slot %d | difficulty %d | live HUD attached",request.slot,request.difficulty);
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
      original_ui_error="Character menu: "+error;
      __android_log_print(ANDROID_LOG_ERROR,tag,"Original character menu frame failed: %s",error.c_str());
      authored_character_menu->close();
    }
    return;
  }
  if(model_renderer::active()){
    try {
      if(original_ui.overlays_player()){
        dh2::perf::Scope perf_prepare(dh2::perf::Phase::hud_prepare);
        std::string error;if(!original_ui.prepare_player_frame(surface_width,surface_height,error))throw std::runtime_error("Combat HUD preparation: "+error);
        model_renderer::connect_combat_text(original_ui.combat_text_sink());
      }
      model_renderer::draw(surface_width,surface_height);
    } catch(const std::exception& e) {
      __android_log_print(ANDROID_LOG_ERROR,tag,"Native frame failed: %s",e.what());
      original_ui.deactivate();
      model_renderer::deactivate();
      return;
    } catch(...) {
      __android_log_print(ANDROID_LOG_ERROR,tag,"Native frame failed: unknown exception");
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
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_characterPanelOpen(JNIEnv*,jclass,jboolean open){model_renderer::set_character_panel_open(open==JNI_TRUE);}
extern "C" JNIEXPORT jboolean JNICALL Java_com_example_dh2_NativeBridge_authoredCharacterMenuIsOpen(JNIEnv*,jclass){
 return authored_character_menu&&authored_character_menu->opened?JNI_TRUE:JNI_FALSE;
}
extern "C" JNIEXPORT jboolean JNICALL Java_com_example_dh2_NativeBridge_authoredCharacterMenuBack(JNIEnv*,jclass){
 if(!authored_character_menu||!authored_character_menu->opened)return JNI_FALSE;
 std::string error;
 if(!character_panel->authored_back(model_renderer::player_gameplay_binding(),error))__android_log_print(ANDROID_LOG_ERROR,tag,"Original character menu Back failed: %s",error.c_str());
 return JNI_TRUE;
}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_moveAxis(JNIEnv*,jclass,jfloat x,jfloat y){model_renderer::move_axis(x,y);}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_authoredHudTouch(JNIEnv* env,jclass,jint phase,jint pointer,jfloat x,jfloat y){
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
    const auto live=model_renderer::player_gameplay_binding();
    if(authored_character_menu&&authored_character_menu->leases->world!=live.world_owner){
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
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_focusObject(JNIEnv*,jclass,jint index){model_renderer::focus_object(index);}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_objectState(JNIEnv* env,jclass,jint index,jstring state){
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
 original_ui.deactivate();model_renderer::deactivate();front_launch_report.clear();original_ui_error.clear();
 // Explicit preview mode until canonical SG_Load4 and saved equipment are integrated.
 front_ui.demo_persona_mode();std::string error;
 if(!front_ui.load_front_screen(front_directory,selected,error))return result(env,"Main menu failed: "+error);
 return result(env,"Original main menu | character selection | Play launches the Knight Crypt demo");
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_originalMenuTouch(JNIEnv* env,jclass,jfloat x,jfloat y,jint action){
 std::string error;if(!front_ui.touch(x,y,action,error))return result(env,error);return nullptr;
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_consumeOriginalMenuAudio(JNIEnv* env,jclass){auto value=front_ui.consume_menu_audio();return value.empty()?nullptr:result(env,value);}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_consumeOriginalMenuSound(JNIEnv* env,jclass){auto value=front_ui.consume_menu_sound();return value.empty()?nullptr:result(env,value);}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_consumeFrontLaunch(JNIEnv* env,jclass){if(front_launch_report.empty())return nullptr;const auto value=std::move(front_launch_report);front_launch_report.clear();return result(env,value);}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_combatTarget(JNIEnv* env,jclass,jint index,jint target){return env->NewStringUTF(model_renderer::set_combat_target(index,target).c_str());}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_playerAttack(JNIEnv* env,jclass,jint target){return env->NewStringUTF(model_renderer::player_attack(target).c_str());}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_playerEquipmentAction(JNIEnv* env,jclass,jint operation,jint index,jint slot){return result(env,model_renderer::player_equipment_action(operation,index,slot));}

extern "C" JNIEXPORT jintArray JNICALL Java_com_example_dh2_NativeBridge_playerVitals(JNIEnv* env,jclass){auto values=model_renderer::player_vitals();auto out=env->NewIntArray(values.size());if(out)env->SetIntArrayRegion(out,0,values.size(),values.data());return out;}
extern "C" JNIEXPORT jintArray JNICALL Java_com_example_dh2_NativeBridge_playerGameplayHud(JNIEnv* env,jclass){
  auto values=model_renderer::gameplay_hud_snapshot(model_renderer::player_gameplay_binding());
  auto out=env->NewIntArray(static_cast<jsize>(values.size()));if(out&&!values.empty())env->SetIntArrayRegion(out,0,static_cast<jsize>(values.size()),values.data());return out;
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_playerGameplayAction(JNIEnv* env,jclass,jint operation,jint index){return result(env,model_renderer::player_gameplay_action(operation,index));}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_playerCharacterSnapshot(JNIEnv* env,jclass){return result(env,character_panel?character_panel->snapshot(model_renderer::player_gameplay_binding()):"{\"ready\":false,\"error\":\"Character menu unavailable\"}");}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_playerCharacterAction(JNIEnv* env,jclass,jint operation,jint index,jint slot){return result(env,character_panel?character_panel->action(model_renderer::player_gameplay_binding(),operation,index,slot):"Character menu unavailable");}
extern "C" JNIEXPORT jintArray JNICALL Java_com_example_dh2_NativeBridge_menuIcon(JNIEnv* env,jclass,jstring name){
  if(!name||!gameplay_assets)return nullptr;const char* raw=env->GetStringUTFChars(name,nullptr);if(!raw)return nullptr;
  const std::string value(raw);env->ReleaseStringUTFChars(name,raw);std::string error;
  auto pixels=dh2::android_ui::menu_icon_pixels(gameplay_assets,value,error);
  if(pixels.empty()){if(!error.empty())__android_log_print(ANDROID_LOG_WARN,tag,"Original icon unavailable | %s | %s",value.c_str(),error.c_str());return nullptr;}
  auto out=env->NewIntArray(static_cast<jsize>(pixels.size()));if(out)env->SetIntArrayRegion(out,0,static_cast<jsize>(pixels.size()),pixels.data());return out;
}
extern "C" JNIEXPORT jobjectArray JNICALL Java_com_example_dh2_NativeBridge_playerGameplayIcons(JNIEnv* env,jclass){
  auto names=model_renderer::gameplay_hud_icon_names(model_renderer::player_gameplay_binding());
  auto string_class=env->FindClass("java/lang/String");if(!string_class)return nullptr;
  auto out=env->NewObjectArray(static_cast<jsize>(names.size()),string_class,nullptr);
  if(out)for(std::size_t i=0;i<names.size();++i){auto item=env->NewStringUTF(names[i].c_str());if(!item)return nullptr;env->SetObjectArrayElement(out,static_cast<jsize>(i),item);env->DeleteLocalRef(item);}env->DeleteLocalRef(string_class);return out;
}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_enemyAi(JNIEnv*,jclass,jboolean enabled){model_renderer::set_enemy_ai(enabled);}
