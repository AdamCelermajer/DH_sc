#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include "textures.hpp"
#include "model_renderer.hpp"
#include "authored_shader_program.hpp"
#include "original_ui_session.hpp"
#include <android/asset_manager_jni.h>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include <exception>
#include <stdexcept>

namespace {
constexpr const char* tag="DH2Native";
dh2::android_ui::Program ui_program{};
dh2::android_ui::OriginalUiSession original_ui;
std::string original_ui_error;
GLuint texture=0;
int surface_width=1,surface_height=1,texture_width=1,texture_height=1;
bool report_model_frame=true;
bool report_texture_frame=true;
std::string errors(const char* operation){
  std::string text;for(GLenum e;(e=glGetError())!=GL_NO_ERROR;){char s[96];std::snprintf(s,sizeof(s),"%s GL error 0x%04x; ",operation,e);
    text+=s;__android_log_print(ANDROID_LOG_ERROR,tag,"%s",s);}return text;
}
jstring result(JNIEnv* env,const std::string& text){return env->NewStringUTF(text.c_str());}
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_buildInfo(JNIEnv* env,jclass) {
  return result(env,"Native source reconstruction: animated scene nodes");
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_initialize(JNIEnv* env,jclass,jobject assets){
  // Android has created a new context: previous GL names belong to the old
  // context and must not be deleted against this context's reused names.
  model_renderer::reset_context();ui_program={};texture=0;report_texture_frame=true;
  dh2::android_ui::Program premultiplied;
  try{
    auto* manager=assets?AAssetManager_fromJava(env,assets):nullptr;
    ui_program=dh2::android_ui::create(manager,false);
    premultiplied=dh2::android_ui::create(manager,true);
    dh2::android_ui::validate_pixels(ui_program,premultiplied);
    dh2::android_ui::release(premultiplied);
    std::string ui_error;
    if(!original_ui.initialize(manager,ui_error))throw std::runtime_error(ui_error);
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
  glClearColor(0.08f,0.09f,0.11f,1);glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  if(original_ui.active()&&!original_ui.overlays_player()){
    std::string error;
    if(!original_ui.render(surface_width,surface_height,error)){
      original_ui_error=error;
      __android_log_print(ANDROID_LOG_ERROR,tag,"Original health panel failed: %s",error.c_str());
    }
    return;
  }
  if(model_renderer::active()){
    try {
      model_renderer::draw(surface_width,surface_height);
    } catch(const std::exception& e) {
      __android_log_print(ANDROID_LOG_ERROR,tag,"Native frame failed: %s",e.what());
      model_renderer::deactivate();
      return;
    } catch(...) {
      __android_log_print(ANDROID_LOG_ERROR,tag,"Native frame failed: unknown exception");
      model_renderer::deactivate();
      return;
    }
    if(report_model_frame){__android_log_print(ANDROID_LOG_INFO,tag,"Model frame submitted at %d x %d",surface_width,surface_height);report_model_frame=false;}
    if(original_ui.overlays_player()){
      const auto player=model_renderer::player_hud_view();std::string error;
      if(!original_ui.render_player(surface_width,surface_height,player.resolved,player.count,player.character,error)){
        original_ui_error=error;
        __android_log_print(ANDROID_LOG_ERROR,tag,"Connected player HUD failed: %s",error.c_str());
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
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_moveAxis(JNIEnv*,jclass,jfloat x,jfloat y){model_renderer::move_axis(x,y);}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_focusObject(JNIEnv*,jclass,jint index){model_renderer::focus_object(index);}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_objectState(JNIEnv* env,jclass,jint index,jstring state){
 if(!state)return env->NewStringUTF("Actor state is absent");
 const char* raw=env->GetStringUTFChars(state,nullptr);if(!raw)return nullptr;
 const std::string name(raw);env->ReleaseStringUTFChars(state,raw);
 return env->NewStringUTF(model_renderer::set_object_state(index,name).c_str());
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_loadWorld(JNIEnv* env,jclass,jbyteArray input,jobject assets,jstring files_directory){
  if(!input||!assets||!files_directory)return result(env,"Null world input");const auto n=env->GetArrayLength(input);
  if(n<=0||n>65560)return result(env,"World descriptor outside limit");
  std::vector<std::uint8_t> bytes(n);env->GetByteArrayRegion(input,0,n,reinterpret_cast<jbyte*>(bytes.data()));if(env->ExceptionCheck())return nullptr;
  const char* directory=env->GetStringUTFChars(files_directory,nullptr);if(!directory)return nullptr;
  const std::string directory_path(directory);env->ReleaseStringUTFChars(files_directory,directory);
  original_ui.deactivate();
  original_ui_error.clear();report_model_frame=true;
  auto report=model_renderer::load_world(bytes.data(),bytes.size(),AAssetManager_fromJava(env,assets),directory_path);
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
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_combatTarget(JNIEnv* env,jclass,jint index,jint target){return env->NewStringUTF(model_renderer::set_combat_target(index,target).c_str());}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_playerAttack(JNIEnv* env,jclass,jint target){return env->NewStringUTF(model_renderer::player_attack(target).c_str());}

extern "C" JNIEXPORT jintArray JNICALL Java_com_example_dh2_NativeBridge_playerVitals(JNIEnv* env,jclass){auto values=model_renderer::player_vitals();auto out=env->NewIntArray(values.size());if(out)env->SetIntArrayRegion(out,0,values.size(),values.data());return out;}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_enemyAi(JNIEnv*,jclass,jboolean enabled){model_renderer::set_enemy_ai(enabled);}
