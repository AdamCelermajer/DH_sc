#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include "textures.hpp"
#include "model_renderer.hpp"
#include "original_ui_session.hpp"
#include "original_menu_viewport_v1.hpp"
#include "menu_start_commit_gate_v1.hpp"
#include "player_gameplay_audio.hpp"
#include <cmath>
#include <climits>
#include <android/asset_manager_jni.h>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include <exception>
#include "gameplay_scenario_v1.hpp"

namespace {
constexpr const char* tag="DH2Native";
GLuint program=0,texture=0;
GLint position=-1,uv=-1,scale=-1;
int surface_width=1,surface_height=1,texture_width=1,texture_height=1;
bool report_model_frame=true;
dh2::android_ui::OriginalUiSession original_ui;
AAssetManager* menu_assets=nullptr;
bool ui_frame_failed=false;
std::string menu_directory;
bool difficulty_count(void*,std::uint32_t* count,std::string& error){return count&&original_ui.game_difficulty_count(*count,error);}
void bind_menu_runtime(){
 dh2::android_ui::FrontRuntimeServices services{};
 services.create_save_slot=[](void*,const std::string& name,const std::string& character,std::int32_t& slot,bool& published,std::string& error){return model_renderer::create_menu_save_slot(menu_assets,name,character,slot,published,error,nullptr,difficulty_count);};
 services.assign_save_slot=[](void*,std::int32_t slot,std::int32_t ordinal,std::string& error){return model_renderer::assign_menu_save_slot(slot,ordinal,error);};
 services.eabi_integer=[](void*,double number,std::int32_t& value,std::string& error){if(!std::isfinite(number)||number<double(INT32_MIN)||number>double(INT32_MAX)){error="Menu integer outside native conversion boundary";return false;}value=static_cast<std::int32_t>(number);return true;};
 services.request_start_game=[](void*,bool numeric,std::int32_t requested,std::int32_t& slot,std::string& error){return model_renderer::request_menu_start(numeric,requested,slot,error);};
 services.request_continue_game=[](void*,std::int32_t& slot,std::string& error){return model_renderer::request_menu_continue(slot,error);};
 services.change_preview_slot=[](void*,std::int32_t slot,bool force,std::string& error){return model_renderer::select_menu_preview_slot(slot,force,error);};
 services.debug_load=[](void*,std::string& error){return model_renderer::menu_debug_load(menu_assets,error);};
 services.debug_query=[](void*,const char* key,std::string& error){return model_renderer::menu_debug_query(menu_assets,key,error);};
 services.save_game=[](void*,std::uintptr_t character,std::string& error){return model_renderer::ui_player_save_game(character,error);};
 services.revive_local_players=[](void*,std::string& error){return model_renderer::ui_revive_local_players(error);};
 services.camera_zoom_lock=[](void*,bool locked){model_renderer::camera_zoom_lock(locked);};
 original_ui.bind_front_runtime(services);
 const auto design=original_ui.design_settings_table();
 model_renderer::bind_xp_design_settings(design.data,design.size);
 model_renderer::bind_item_text_services(&original_ui,
  [](void* context,const dh2::data::ItemTable& items,const dh2::data::CharacterTable& characters){
   return static_cast<dh2::android_ui::OriginalUiSession*>(context)->item_text_services(items,characters);
  });
 model_renderer::bind_source_dialog_services({&original_ui,
  [](void* context,std::int32_t actor,std::int32_t style,std::int32_t text){
   std::string error;
   const bool ok=static_cast<dh2::android_ui::OriginalUiSession*>(context)
       ->enqueue_source_dialog(actor,style,text,error);
   if(!ok)__android_log_print(ANDROID_LOG_ERROR,tag,
       "Source StartDialog dispatch failed: %s",error.c_str());
   return ok?1:-1;
  },
  [](void* context){
   bool active=false;std::string error;
   const bool ok=static_cast<dh2::android_ui::OriginalUiSession*>(context)
       ->source_dialog_active(active,error);
   if(!ok)__android_log_print(ANDROID_LOG_ERROR,tag,
       "Source WaitDialog predicate failed: %s",error.c_str());
   return ok?(active?1:0):-1;
  },
  [](void* context,std::int32_t dialog_id){
   std::string error;
   const bool ok=static_cast<dh2::android_ui::OriginalUiSession*>(context)
       ->enqueue_source_dialog_id(dialog_id,error);
   if(!ok)__android_log_print(ANDROID_LOG_ERROR,tag,
       "Source StartDialogID dispatch failed: %s",error.c_str());
   return ok?1:-1;
  }});
 model_renderer::bind_source_char_menu_tutorial_services({&original_ui,
  [](void* context,std::int32_t string_id,const char* frame,const char* menu){
   if(!context||!frame||!menu)return -1;
   std::string error;
   const bool ok=static_cast<dh2::android_ui::OriginalUiSession*>(context)
       ->enqueue_source_char_menu_tutorial(string_id,frame,menu,error);
   if(!ok)__android_log_print(ANDROID_LOG_ERROR,tag,
       "Source EnqueueCharMenuTutorialMessage failed: %s",error.c_str());
   return ok?1:-1;
  },
  [](void* context,bool skip_all){
   if(!context)return -1;
   std::string error;
   const bool ok=static_cast<dh2::android_ui::OriginalUiSession*>(context)
       ->skip_source_char_menu_tutorial(skip_all,error);
   if(!ok)__android_log_print(ANDROID_LOG_ERROR,tag,
       "Source CharacterMenu tutorial skip failed: %s",error.c_str());
   return ok?1:-1;
  }});
}
const char* vs_source=R"(attribute vec2 position;attribute vec2 uv;uniform vec2 scale;varying vec2 texcoord;
void main(){texcoord=uv;gl_Position=vec4(position*scale,0.0,1.0);})";
const char* fs_source=R"(precision mediump float;varying vec2 texcoord;uniform sampler2D image;
void main(){vec4 t=texture2D(image,texcoord);float tile=mod(floor(gl_FragCoord.x/16.0)+floor(gl_FragCoord.y/16.0),2.0);
vec3 bg=mix(vec3(0.22),vec3(0.38),tile);gl_FragColor=vec4(mix(bg,t.rgb,t.a),1.0);})";
GLuint compile(GLenum kind,const char* source){
  const GLuint shader=glCreateShader(kind);glShaderSource(shader,1,&source,nullptr);glCompileShader(shader);
  GLint ok=0;glGetShaderiv(shader,GL_COMPILE_STATUS,&ok);
  if(!ok){char log[2048]{};glGetShaderInfoLog(shader,sizeof(log),nullptr,log);
    __android_log_print(ANDROID_LOG_ERROR,tag,"Shader failed: %s",log);glDeleteShader(shader);return 0;}
  return shader;
}
std::string errors(const char* operation){
  std::string text;for(GLenum e;(e=glGetError())!=GL_NO_ERROR;){char s[96];std::snprintf(s,sizeof(s),"%s GL error 0x%04x; ",operation,e);
    text+=s;__android_log_print(ANDROID_LOG_ERROR,tag,"%s",s);}return text;
}
jstring result(JNIEnv* env,const std::string& text){return env->NewStringUTF(text.c_str());}
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_buildInfo(JNIEnv* env,jclass) {
  return result(env,"Native source reconstruction: animated scene nodes");
}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_modDirectory(JNIEnv* env,jclass,jstring directory){
  if(!directory){model_renderer::mod_directory("");return;}
  const char* raw=env->GetStringUTFChars(directory,nullptr);if(!raw)return;
  const std::string copy(raw);env->ReleaseStringUTFChars(directory,raw);model_renderer::mod_directory(copy);
}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_runtimeDirectory(JNIEnv* env,jclass,jstring directory){
  if(!directory){model_renderer::runtime_directory("");return;}
  const char* raw=env->GetStringUTFChars(directory,nullptr);if(!raw)return;
  const std::string copy(raw);env->ReleaseStringUTFChars(directory,raw);model_renderer::runtime_directory(copy);
  menu_directory=copy;
}
extern "C" JNIEXPORT jbyteArray JNICALL Java_com_example_dh2_NativeBridge_readAsset(JNIEnv* env,jclass,jstring name,jobject assets){
  if(!name||!assets)return nullptr;
  const char* raw=env->GetStringUTFChars(name,nullptr);if(!raw)return nullptr;
  const std::string copy(raw);env->ReleaseStringUTFChars(name,raw);
  try{
    auto bytes=model_renderer::read_asset(AAssetManager_fromJava(env,assets),copy);
    auto out=env->NewByteArray(static_cast<jsize>(bytes.size()));
    if(out)env->SetByteArrayRegion(out,0,bytes.size(),reinterpret_cast<const jbyte*>(bytes.data()));return out;
  }catch(const std::exception& error){
    auto type=env->FindClass("java/io/IOException");if(type)env->ThrowNew(type,error.what());return nullptr;
  }
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_profileSlot(JNIEnv* env,jclass,jint slot){
  return result(env,model_renderer::profile_slot(slot));
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_initialize(JNIEnv* env,jclass,jobject assets){
  program=0;texture=0;
  try{model_renderer::reset_context();}
  catch(const std::exception& error){return result(env,std::string("Renderer recreation failed: ")+error.what());}
  const GLuint vs=compile(GL_VERTEX_SHADER,vs_source),fs=compile(GL_FRAGMENT_SHADER,fs_source);
  if(!vs||!fs){if(vs)glDeleteShader(vs);if(fs)glDeleteShader(fs);return result(env,"Shader initialization failed; see DH2Native Logcat");}
  program=glCreateProgram();glAttachShader(program,vs);glAttachShader(program,fs);glLinkProgram(program);
  glDeleteShader(vs);glDeleteShader(fs);GLint linked=0;glGetProgramiv(program,GL_LINK_STATUS,&linked);
  if(!linked){char log[2048]{};glGetProgramInfoLog(program,sizeof(log),nullptr,log);glDeleteProgram(program);program=0;
    __android_log_print(ANDROID_LOG_ERROR,tag,"Link failed: %s",log);return result(env,log);}
  if(!assets)return result(env,"UI asset manager missing");
  menu_assets=AAssetManager_fromJava(env,assets);std::string ui_error;
  if(!original_ui.initialize(menu_assets,ui_error))return result(env,"Menu renderer initialization failed: "+ui_error);
  bind_menu_runtime();ui_frame_failed=false;
  position=glGetAttribLocation(program,"position");uv=glGetAttribLocation(program,"uv");scale=glGetUniformLocation(program,"scale");
  std::string report="Renderer: ";const auto* r=glGetString(GL_RENDERER);report+=r?reinterpret_cast<const char*>(r):"unknown";
  report+="\nGLES: ";const auto* v=glGetString(GL_VERSION);report+=v?reinterpret_cast<const char*>(v):"unknown";
  return result(env,report+errors("initialize"));
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_loadTexture(JNIEnv* env,jclass,jbyteArray input){
  if(!input)return result(env,"Null texture input");
  if(!program)return result(env,"Renderer is unavailable; see DH2Native Logcat");
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
  model_renderer::deactivate();
  char report[256];std::snprintf(report,sizeof(report),"%u x %u | format %u | alpha %u | RGBA upload OK\nAspect ratio preserved",
    view.width,view.height,static_cast<unsigned>(view.format),view.alpha);
  return result(env,report);
}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_resize(JNIEnv*,jclass,jint w,jint h){
  surface_width=std::max(1,int(w));surface_height=std::max(1,int(h));
  const auto viewport=dh2::ui::original_menu_viewport_v1::full_surface(surface_width,surface_height);
  glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);
  model_renderer::drawable_target_changed(surface_width,surface_height);
  report_model_frame=true;
  __android_log_print(ANDROID_LOG_INFO,tag,"Surface resized to %d x %d",surface_width,surface_height);
}
dh2::native::gameplay_scenario_v1::Driver debug_scenario;
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_debugScenario(JNIEnv* env,jclass,jstring name){
 if(!name)return;
 const char* raw=env->GetStringUTFChars(name,nullptr);if(!raw)return;
 debug_scenario.mode=dh2::native::gameplay_scenario_v1::Driver::parse(raw);
 __android_log_print(ANDROID_LOG_INFO,"DH2Scenario","Debug scenario armed | %s | mode %d",raw,int(debug_scenario.mode));
 env->ReleaseStringUTFChars(name,raw);
}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_draw(JNIEnv*,jclass){
  glClearColor(0.08f,0.09f,0.11f,1);glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  if(original_ui.active()&&!original_ui.overlays_player()){
    std::string error;if(!original_ui.render(surface_width,surface_height,error)){
      if(!ui_frame_failed)__android_log_print(ANDROID_LOG_ERROR,tag,"Original menu frame failed: %s",error.c_str());ui_frame_failed=true;
    }return;
  }
  if(model_renderer::active()){
    try {
      model_renderer::draw(surface_width,surface_height);
      if(original_ui.overlays_player()){
        std::string error;
        const bool ok=model_renderer::with_player_status_sheet(nullptr,[](void*,const std::int32_t* sheet,std::size_t count,std::uintptr_t character,std::string& failure){
          return original_ui.render_player(surface_width,surface_height,sheet,count,character,failure);
        },error);
        if(!ok&&!ui_frame_failed){__android_log_print(ANDROID_LOG_ERROR,tag,"Player HUD frame failed: %s",error.c_str());ui_frame_failed=true;}
      }
      if(original_ui.overlays_player())debug_scenario.step(surface_width,surface_height,[](float x,float y,int action){
        std::string touch_error;
        if(!original_ui.touch(x,y,action,touch_error))
          __android_log_print(ANDROID_LOG_WARN,"DH2Scenario","Scenario tap failed | %s",touch_error.c_str());
      });
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
    return;
  }
  if(!program||!texture)return;
  const GLfloat vertices[]={-1,1,0,0,-1,-1,0,1,1,1,1,0,1,-1,1,1};
  const float image_aspect=float(texture_width)/texture_height,viewport_aspect=float(surface_width)/surface_height;
  glUseProgram(program);glUniform2f(scale,std::min(1.0f,image_aspect/viewport_aspect),std::min(1.0f,viewport_aspect/image_aspect));
  glActiveTexture(GL_TEXTURE0);glBindTexture(GL_TEXTURE_2D,texture);glUniform1i(glGetUniformLocation(program,"image"),0);
  glBindBuffer(GL_ARRAY_BUFFER,0);glEnableVertexAttribArray(position);glEnableVertexAttribArray(uv);
  glVertexAttribPointer(position,2,GL_FLOAT,GL_FALSE,4*sizeof(GLfloat),vertices);
  glVertexAttribPointer(uv,2,GL_FLOAT,GL_FALSE,4*sizeof(GLfloat),vertices+2);glDrawArrays(GL_TRIANGLE_STRIP,0,4);
  glDisableVertexAttribArray(position);glDisableVertexAttribArray(uv);errors("draw");
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_loadModel(JNIEnv* env,jclass,jbyteArray input,jobject assets){
  if(!input||!assets)return result(env,"Null model input");
  const auto n=env->GetArrayLength(input);if(n<=0||n>32*1024*1024)return result(env,"Model size outside limit");
  std::vector<std::uint8_t> bytes(n);env->GetByteArrayRegion(input,0,n,reinterpret_cast<jbyte*>(bytes.data()));if(env->ExceptionCheck())return nullptr;
  report_model_frame=true;
  return result(env,model_renderer::load(bytes.data(),bytes.size(),AAssetManager_fromJava(env,assets)));
}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_orbit(JNIEnv*,jclass,jfloat dx,jfloat dy,jfloat zoom){model_renderer::orbit(dx,dy,zoom);}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_animationTime(JNIEnv*,jclass,jint milliseconds){model_renderer::set_time(milliseconds);}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_moveAxis(JNIEnv*,jclass,jfloat x,jfloat y){model_renderer::move_axis(x,y);}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_stopMovement(JNIEnv*,jclass){model_renderer::stop_movement();}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_cameraPinchZoom(JNIEnv*,jclass,jfloat previous,jfloat current){model_renderer::camera_pinch_zoom(previous,current);}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_cameraTouchPan(JNIEnv*,jclass,jint dx,jint dy){if(original_ui.camera_pan_allowed())model_renderer::camera_touch_pan(dx,dy);}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_focusObject(JNIEnv*,jclass,jint index){model_renderer::focus_object(index);}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_objectState(JNIEnv* env,jclass,jint index,jstring state){
 if(!state)return env->NewStringUTF("Actor state is absent");
 const char* raw=env->GetStringUTFChars(state,nullptr);if(!raw)return nullptr;
 const std::string name(raw);env->ReleaseStringUTFChars(state,raw);
 return env->NewStringUTF(model_renderer::set_object_state(index,name).c_str());
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_loadWorld(JNIEnv* env,jclass,jbyteArray input,jobject assets){
// SpawnCharacter's exact-name script boundary is exposed separately below.
  if(!input||!assets)return result(env,"Null world input");const auto n=env->GetArrayLength(input);
  if(n<=0||n>65560)return result(env,"World descriptor outside limit");
  std::vector<std::uint8_t> bytes(n);env->GetByteArrayRegion(input,0,n,reinterpret_cast<jbyte*>(bytes.data()));if(env->ExceptionCheck())return nullptr;
  original_ui.deactivate();
  report_model_frame=true;return result(env,model_renderer::load_world(bytes.data(),bytes.size(),AAssetManager_fromJava(env,assets)));
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_spawnCharacter(JNIEnv* env,jclass,jstring character){
 if(!character)return env->NewStringUTF("SpawnCharacter name is absent");
 const char* raw=env->GetStringUTFChars(character,nullptr);if(!raw)return nullptr;
 const std::string name(raw);env->ReleaseStringUTFChars(character,raw);
 return env->NewStringUTF(model_renderer::spawn_character(name).c_str());
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_combatTarget(JNIEnv* env,jclass,jint index,jint target){return env->NewStringUTF(model_renderer::set_combat_target(index,target).c_str());}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_debugCharacterHit(JNIEnv* env,jclass,jstring character,jint raw_damage){
 if(!character||raw_damage<=0)return env->NewStringUTF("Debug Character hit arguments rejected");
 const char* raw=env->GetStringUTFChars(character,nullptr);if(!raw)return nullptr;
 const std::string name(raw);env->ReleaseStringUTFChars(character,raw);
 return result(env,model_renderer::debug_character_hit(name,std::uint32_t(raw_damage)));
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_playerAttack(JNIEnv* env,jclass,jint target){return env->NewStringUTF(model_renderer::player_attack(target).c_str());}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_debugPlayerSkillCooldown(JNIEnv* env,jclass,jint delay){return env->NewStringUTF(model_renderer::debug_player_skill_cooldown(std::uint32_t(delay)).c_str());}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_debugPlayerSkillCheck(JNIEnv* env,jclass,jint slot){return env->NewStringUTF(model_renderer::debug_player_skill_check(std::uint32_t(slot)).c_str());}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_debugPlayerMana(JNIEnv* env,jclass,jint amount){return env->NewStringUTF(model_renderer::debug_player_mana(std::uint32_t(amount)).c_str());}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_debugPlayerScalar(JNIEnv* env,jclass,jint value,jboolean write){return env->NewStringUTF(model_renderer::debug_player_scalar(std::int32_t(value),write==JNI_TRUE).c_str());}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_debugPlayerDeath(JNIEnv* env,jclass){return env->NewStringUTF(model_renderer::debug_player_death().c_str());}

extern "C" JNIEXPORT jintArray JNICALL Java_com_example_dh2_NativeBridge_playerVitals(JNIEnv* env,jclass){auto values=model_renderer::player_vitals();auto out=env->NewIntArray(values.size());if(out)env->SetIntArrayRegion(out,0,values.size(),values.data());return out;}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_enemyAi(JNIEnv*,jclass,jboolean enabled){model_renderer::set_enemy_ai(enabled);}

extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_loadFrontScreen(JNIEnv* env,jclass,jstring directory,jobject assets){
 if(!directory||!assets)return result(env,"Menu launch input unavailable");
 const char* raw=env->GetStringUTFChars(directory,nullptr);if(!raw)return nullptr;const std::string path(raw);env->ReleaseStringUTFChars(directory,raw);
 menu_assets=AAssetManager_fromJava(env,assets);model_renderer::deactivate();
 std::string error;ui_frame_failed=false;
 if(!original_ui.load_front_screen(path,"main",error))return result(env,"Menu load failed: "+error);
 return result(env,"Original menu ready");
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_loadOpeningSplash(JNIEnv* env,jclass,jstring directory,jobject assets){
 if(!directory||!assets)return result(env,"Opening splash input unavailable");
 const char* raw=env->GetStringUTFChars(directory,nullptr);if(!raw)return nullptr;const std::string path(raw);env->ReleaseStringUTFChars(directory,raw);
 menu_assets=AAssetManager_fromJava(env,assets);model_renderer::deactivate();
 std::string error;ui_frame_failed=false;
 if(!original_ui.load_front_screen(path,"loading",error))return result(env,"Opening splash load failed: "+error);
 return result(env,"Original opening splash ready");
}
extern "C" JNIEXPORT jint JNICALL Java_com_example_dh2_NativeBridge_frontLanguage(JNIEnv*,jclass){
 return static_cast<jint>(original_ui.language());
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_returnToMainMenu(JNIEnv* env,jclass,jstring directory,jobject assets){
 if(!directory||!assets)return result(env,"Menu launch input unavailable");
 const char* raw=env->GetStringUTFChars(directory,nullptr);if(!raw)return nullptr;const std::string path(raw);env->ReleaseStringUTFChars(directory,raw);
 menu_assets=AAssetManager_fromJava(env,assets);
 std::string unload_error;
 if(!model_renderer::unload_game_to_menu(unload_error))
  return result(env,"Menu return deferred: "+unload_error);
 std::string error;ui_frame_failed=false;
 if(!original_ui.load_front_screen(path,"main",error))return result(env,"Menu load failed: "+error);
 return result(env,"Original menu ready");
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_menuTouch(JNIEnv* env,jclass,jfloat x,jfloat y,jint action){
 std::string error;if(!original_ui.touch(x,y,action,error))return result(env,"Menu touch failed: "+error);
 if(original_ui.consume_gameplay_main_menu_request())return result(env,"GAMEPLAY_MAIN_MENU_REQUESTED");
 return result(env,"");
}
extern "C" JNIEXPORT jboolean JNICALL Java_com_example_dh2_NativeBridge_menuTextInputActive(JNIEnv*,jclass){
 return original_ui.menu_text_input_active()?JNI_TRUE:JNI_FALSE;
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_menuKey(JNIEnv* env,jclass,jint code,jboolean down){
 std::string error;if(code<0||!original_ui.menu_key_event(static_cast<std::uint32_t>(code),down==JNI_TRUE,error))
  return result(env,"Menu key failed: "+(error.empty()?std::string("invalid key code"):error));
 return result(env,"");
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_backToHud(JNIEnv* env,jclass){
 std::string error;if(!original_ui.back_to_hud(error))return result(env,"Gameplay Back failed: "+error);return result(env,"");
}
extern "C" JNIEXPORT jobject JNICALL Java_com_example_dh2_NativeBridge_consumeMenuLaunch(JNIEnv* env,jclass){
 dh2::android_ui::NativeStartGameIntentV1 request;
 if(!original_ui.consume_launch_request(request))return nullptr;
 jclass cls=env->FindClass("com/example/dh2/NativeLaunchRequest");
 if(!cls)return nullptr;
 const jmethodID constructor=env->GetMethodID(cls,"<init>","(IIZILjava/lang/String;D)V");
 if(!constructor){env->DeleteLocalRef(cls);return nullptr;}
 jstring destination=request.destination_name.empty()?nullptr:
     env->NewStringUTF(request.destination_name.c_str());
 if(!request.destination_name.empty()&&!destination){env->DeleteLocalRef(cls);return nullptr;}
 const jobject result=env->NewObject(cls,constructor,
     static_cast<jint>(request.kind),static_cast<jint>(request.slot),
     request.has_numeric_difficulty?JNI_TRUE:JNI_FALSE,
     static_cast<jint>(request.requested_difficulty),destination,
     static_cast<jdouble>(request.raw_entry_point));
 if(destination)env->DeleteLocalRef(destination);
 env->DeleteLocalRef(cls);
 return result;
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_startMenuGame(JNIEnv* env,jclass,jint slot,jobject assets,jint debugLevelRow,jboolean hasNumericDifficulty,jint requestedDifficulty){
 if(!assets||slot<0)return result(env,"Start Game input unavailable");
 original_ui.deactivate();ui_frame_failed=false;report_model_frame=true;
 auto startup=model_renderer::start_menu_game(slot,AAssetManager_fromJava(env,assets),debugLevelRow,
     hasNumericDifficulty==JNI_TRUE,requestedDifficulty);
 const bool world_loaded=model_renderer::gameplay_startup_loaded(startup);
 std::string attach_error;
 const bool hud_attached=world_loaded&&original_ui.attach_player(menu_directory,attach_error);
 model_renderer::bind_gameplay_ui_session(hud_attached?&original_ui:nullptr);
 if(dh2::android_ui::menu_start_commit_gate_v1(world_loaded,hud_attached)){
  // Java may switch from the menu to gameplay only after the canonical player
  // HUD has attached to the loaded source Player.
 }else if(world_loaded){
  __android_log_print(ANDROID_LOG_ERROR,tag,"Player HUD attach failed; rejecting NativeStartGame commit: %s",attach_error.c_str());
  model_renderer::deactivate();
  std::string recovery_error;
  if(!original_ui.resume_front_after_start_failure(recovery_error)){
   if(!original_ui.load_front_screen(menu_directory,"main",recovery_error))
    __android_log_print(ANDROID_LOG_ERROR,tag,"Front menu recovery after HUD attach failure failed: %s",recovery_error.c_str());
  }
  startup="Start Game failed: player HUD attach failed: "+(attach_error.empty()?std::string("no diagnostic"):attach_error);
 }else{
  // The source menu movie and its stack are retained across the deferred
  // launch. Re-enable that owner on failure so profile/class selection stays
  // where the user left it; do not rebuild a second menu renderer.
  std::string error;if(!original_ui.resume_front_after_start_failure(error))
   __android_log_print(ANDROID_LOG_ERROR,tag,"Front menu recovery after failed start failed: %s",error.c_str());
 }
 return result(env,startup);
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_transitionLevel(JNIEnv* env,jclass,jstring destination,jdouble rawEntryPoint,jobject assets){
 if(!destination||!assets)return result(env,"NativeGoToZone input unavailable");
 const char* raw=env->GetStringUTFChars(destination,nullptr);if(!raw)return nullptr;
 const std::string name(raw);env->ReleaseStringUTFChars(destination,raw);
 if(name.empty())return result(env,"NativeGoToZone destination is empty");
 report_model_frame=true;
 return result(env,model_renderer::transition_level(name,rawEntryPoint,
     AAssetManager_fromJava(env,assets),&original_ui,
     [](void* raw,bool sourceFlag,const std::string& first,
        const std::string& second,std::int32_t entry,std::string& error){
      auto* ui=static_cast<dh2::android_ui::OriginalUiSession*>(raw);
      return ui&&ui->display_fast_travel(sourceFlag,first,second,entry,error);
     }));
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_consumeMenuAudio(JNIEnv* env,jclass){return result(env,original_ui.consume_menu_audio());}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_consumeMenuSound(JNIEnv* env,jclass){
 auto cue=original_ui.consume_menu_sound();
 if(cue.empty())dh2::player_gameplay_audio::consume(cue);
 return result(env,cue);
}
