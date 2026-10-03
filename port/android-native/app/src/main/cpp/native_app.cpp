#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include "textures.hpp"
#include "model_renderer.hpp"
#include <android/asset_manager_jni.h>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include <exception>

namespace {
constexpr const char* tag="DH2Native";
GLuint program=0,texture=0;
GLint position=-1,uv=-1,scale=-1;
int surface_width=1,surface_height=1,texture_width=1,texture_height=1;
bool report_model_frame=true;
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
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_initialize(JNIEnv* env,jclass){
  model_renderer::reset_context();program=0;texture=0;const GLuint vs=compile(GL_VERTEX_SHADER,vs_source),fs=compile(GL_FRAGMENT_SHADER,fs_source);
  if(!vs||!fs){if(vs)glDeleteShader(vs);if(fs)glDeleteShader(fs);return result(env,"Shader initialization failed; see DH2Native Logcat");}
  program=glCreateProgram();glAttachShader(program,vs);glAttachShader(program,fs);glLinkProgram(program);
  glDeleteShader(vs);glDeleteShader(fs);GLint linked=0;glGetProgramiv(program,GL_LINK_STATUS,&linked);
  if(!linked){char log[2048]{};glGetProgramInfoLog(program,sizeof(log),nullptr,log);glDeleteProgram(program);program=0;
    __android_log_print(ANDROID_LOG_ERROR,tag,"Link failed: %s",log);return result(env,log);}
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
  surface_width=std::max(1,int(w));surface_height=std::max(1,int(h));glViewport(0,0,surface_width,surface_height);
  report_model_frame=true;
  __android_log_print(ANDROID_LOG_INFO,tag,"Surface resized to %d x %d",surface_width,surface_height);
}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_draw(JNIEnv*,jclass){
  glClearColor(0.08f,0.09f,0.11f,1);glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
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
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_focusObject(JNIEnv*,jclass,jint index){model_renderer::focus_object(index);}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_objectState(JNIEnv* env,jclass,jint index,jstring state){
 if(!state)return env->NewStringUTF("Actor state is absent");
 const char* raw=env->GetStringUTFChars(state,nullptr);if(!raw)return nullptr;
 const std::string name(raw);env->ReleaseStringUTFChars(state,raw);
 return env->NewStringUTF(model_renderer::set_object_state(index,name).c_str());
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_loadWorld(JNIEnv* env,jclass,jbyteArray input,jobject assets){
  if(!input||!assets)return result(env,"Null world input");const auto n=env->GetArrayLength(input);
  if(n<=0||n>65560)return result(env,"World descriptor outside limit");
  std::vector<std::uint8_t> bytes(n);env->GetByteArrayRegion(input,0,n,reinterpret_cast<jbyte*>(bytes.data()));if(env->ExceptionCheck())return nullptr;
  report_model_frame=true;return result(env,model_renderer::load_world(bytes.data(),bytes.size(),AAssetManager_fromJava(env,assets)));
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_combatTarget(JNIEnv* env,jclass,jint index,jint target){return env->NewStringUTF(model_renderer::set_combat_target(index,target).c_str());}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_playerAttack(JNIEnv* env,jclass,jint target){return env->NewStringUTF(model_renderer::player_attack(target).c_str());}

extern "C" JNIEXPORT jintArray JNICALL Java_com_example_dh2_NativeBridge_playerVitals(JNIEnv* env,jclass){auto values=model_renderer::player_vitals();auto out=env->NewIntArray(values.size());if(out)env->SetIntArrayRegion(out,0,values.size(),values.data());return out;}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_enemyAi(JNIEnv*,jclass,jboolean enabled){model_renderer::set_enemy_ai(enabled);}
