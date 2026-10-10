#include "scene.hpp"
#include "native_batch_material_values_v113.hpp"
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>
#include "menu_material_under_test.inc"

std::string text(const std::filesystem::path& file){std::ifstream f(file);if(!f)throw std::runtime_error(file.string());return {std::istreambuf_iterator<char>(f),{}};}
GLuint compile(GLenum kind,const std::string& source){auto s=glCreateShader(kind);const auto* p=source.c_str();glShaderSource(s,1,&p,nullptr);glCompileShader(s);GLint good{};glGetShaderiv(s,GL_COMPILE_STATUS,&good);if(!good){char log[4096];glGetShaderInfoLog(s,sizeof(log),nullptr,log);throw std::runtime_error(log);}return s;}
GLuint program(const std::string& vs,const std::string& fs){auto v=compile(GL_VERTEX_SHADER,vs),f=compile(GL_FRAGMENT_SHADER,fs),p=glCreateProgram();glAttachShader(p,v);glAttachShader(p,f);glLinkProgram(p);GLint good{};glGetProgramiv(p,GL_LINK_STATUS,&good);glDeleteShader(v);glDeleteShader(f);if(!good){char log[4096];glGetProgramInfoLog(p,sizeof(log),nullptr,log);throw std::runtime_error(log);}return p;}
const float identity[]{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
void matrix(GLuint p,const char* n,const float* v){auto l=glGetUniformLocation(p,n);if(l>=0)glUniformMatrix4fv(l,1,GL_FALSE,v);}
void scalar(GLuint p,const char* n,float v){auto l=glGetUniformLocation(p,n);if(l>=0)glUniform1f(l,v);}
void sampler(GLuint p,const char* n,int v){auto l=glGetUniformLocation(p,n);if(l>=0)glUniform1i(l,v);}
void vector(GLuint p,const char* n,const float* v){auto l=glGetUniformLocation(p,n);if(l>=0)glUniform4fv(l,1,v);}
void attribute(GLuint p,const char* n,unsigned width,const float* v){auto l=glGetAttribLocation(p,n);if(l>=0){glEnableVertexAttribArray(l);glVertexAttribPointer(l,width,GL_FLOAT,GL_FALSE,0,v);}}
struct Context {
 EGLDisplay display{};EGLSurface surface{};EGLContext context{};
 Context(){display=eglGetDisplay(EGL_DEFAULT_DISPLAY);EGLint major,minor;assert(eglInitialize(display,&major,&minor));assert(eglBindAPI(EGL_OPENGL_ES_API));EGLint attrs[]{EGL_SURFACE_TYPE,EGL_PBUFFER_BIT,EGL_RENDERABLE_TYPE,EGL_OPENGL_ES2_BIT,EGL_RED_SIZE,8,EGL_GREEN_SIZE,8,EGL_BLUE_SIZE,8,EGL_ALPHA_SIZE,8,EGL_NONE};EGLConfig cfg;EGLint n;assert(eglChooseConfig(display,attrs,&cfg,1,&n)&&n);EGLint size[]{EGL_WIDTH,32,EGL_HEIGHT,32,EGL_NONE};surface=eglCreatePbufferSurface(display,cfg,size);EGLint version[]{EGL_CONTEXT_CLIENT_VERSION,2,EGL_NONE};context=eglCreateContext(display,cfg,EGL_NO_CONTEXT,version);assert(eglMakeCurrent(display,surface,surface,context));}
 ~Context(){eglMakeCurrent(display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);eglDestroyContext(display,context);eglDestroySurface(display,surface);eglTerminate(display);}
};
int main(int argc,char** argv){try{
 assert(argc==3);const std::filesystem::path assets=argv[1],shaders=argv[2];
 std::ifstream file(assets/"models/main_menu_charactere_swamp.bdae",std::ios::binary);std::vector<std::uint8_t> bytes{std::istreambuf_iterator<char>(file),{}};
 dh2::resources::BresView image;assert(dh2_bres_open(&image,bytes.data(),bytes.size())==dh2::resources::BresError::ok);
 dh2::scene::Scene scene;std::string error;assert(dh2::scene::load(image,scene,error));assert(scene.materials.size()==3&&scene.instances.size()==3);
 // The asset's one light does exist. It must not be silently borrowed for
 // material light0='#', which original URL resolution deliberately leaves NULL.
 assert(scene.lights_v113.size()==1&&scene.lights_v113[0].id=="Omni01-light");
 Context context;
 const auto generic=program(text(shaders/"generic-vs.glsl"),text(shaders/"generic-fs.glsl"));
 GLuint diffuse,alpha;glGenTextures(1,&diffuse);glGenTextures(1,&alpha);
 unsigned draws{};bool silhouette{},authored_tint{};
 for(const auto& instance:scene.instances){dh2::assets::Mesh mesh;assert(dh2_mesh_open(&mesh,&image,instance.geometry)==dh2::assets::Error::ok);
  for(unsigned primitive=0;primitive<mesh.primitives;++primitive){
   dh2::assets::Primitive part;assert(dh2_mesh_primitive(&mesh,primitive,&part)==dh2::assets::Error::ok);
   const auto& material=scene.materials.at(instance.materials.at(primitive));assert(material.id==part.material);
   const bool lit=!material.effect_file.empty();std::array<float,4> corrected{};
   assert(menu_background_material_color_v124(image,material,corrected,error));
   if(lit){assert((corrected==std::array<float,4>{0,0,0,1}));silhouette=true;}
   else{for(unsigned c=0;c<3;++c)assert(corrected[c]==149.f/255.f);assert(corrected[3]==1);authored_tint=true;}
   // Original positions supply the first three actual mesh vertices' colors
   // and normals. Clip-space positions/UV and one-pixel textures below are
   // declared fixtures so comparison isolates shader/material transport.
   dh2::assets::Attribute normal{},color{};
   const bool have_normal=dh2_mesh_attribute(&mesh,part.attributes[1],&normal)==dh2::assets::Error::ok;
   const bool have_color=dh2_mesh_attribute(&mesh,part.attributes[2],&color)==dh2::assets::Error::ok;
   if(lit){assert(have_normal&&normal.components>=3);
    for(unsigned v=0;v<mesh.vertices;++v){float raw[4]{1,1,1,1};if(have_color)assert(dh2_attribute_read(&color,v,raw));
     if(have_color&&color.components==4)assert(raw[3]/(color.type==1?255.f:1.f)==1.f);
     assert(dh2_attribute_read(&normal,v,raw));for(unsigned c=0;c<3;++c)assert(std::isfinite(raw[c]));
    }
   }
   float colors[12],normals[9];
   for(unsigned v=0;v<3;++v){std::uint32_t index;assert(dh2_index_read(&part,v,&index));
    float raw[4]{1,1,1,1};if(have_color)assert(dh2_attribute_read(&color,index,raw));
    for(unsigned c=0;c<4;++c)colors[v*4+c]=c<color.components?raw[c]/(color.type==1?255.f:1.f):1.f;
    // The original L1 shader forces vertex alpha to one. Verify the exact
    // shipping foliage stream makes the existing generic shader equivalent.
    if(lit)assert(colors[v*4+3]==1.f);
    if(have_normal)assert(dh2_attribute_read(&normal,index,raw));else std::fill(raw,raw+3,0);
    std::copy_n(raw,3,normals+v*3);
   }
   const auto original=program((lit?"#define AL\n":"#define TEXTURED\n")+text(assets/(lit?"shaders/GL_Diffuse_L1_iPhone_VS.glsl":"shaders/ProfileCOMMON_emul_VS.glsl")),(lit?"#define AL\n":"#define TEXTURED\n")+text(assets/(lit?"shaders/GL_Diffuse_L1_iPhone_FS.glsl":"shaders/ProfileCOMMON_emul_FS.glsl")));
   const float positions[]{-1,-1,0,1,-1,0,0,1,0},uv[]{.5,.5,.5,.5,.5,.5};
   for(unsigned char opacity:{static_cast<unsigned char>(0),static_cast<unsigned char>(64),static_cast<unsigned char>(204),static_cast<unsigned char>(255)}){
    for(unsigned unit=0;unit<2;++unit){glActiveTexture(GL_TEXTURE0+unit);glBindTexture(GL_TEXTURE_2D,unit?alpha:diffuse);const unsigned char sample[]{unit?opacity:static_cast<unsigned char>(180),unit?opacity:static_cast<unsigned char>(130),unit?opacity:static_cast<unsigned char>(80),255};glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,1,1,0,GL_RGBA,GL_UNSIGNED_BYTE,sample);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);}
    auto render=[&](GLuint p,bool source){glViewport(0,0,32,32);glDisable(GL_BLEND);glDisable(GL_DEPTH_TEST);glDisable(GL_CULL_FACE);glClearColor(0,0,0,0);glClear(GL_COLOR_BUFFER_BIT);glUseProgram(p);
     if(source){attribute(p,lit?"Vertex":"Position",3,positions);attribute(p,lit?"Color":"Color0",4,colors);attribute(p,"Normal",3,normals);attribute(p,lit?"texcoord0":"TexCoord0",2,uv);matrix(p,lit?"matWorldViewProjection":"WorldViewProjectionMatrix",identity);matrix(p,"matWorld",instance.world.data());matrix(p,"matWorldIT",identity);matrix(p,"TextureMatrix0",material.texture_matrix);vector(p,"DiffuseColor",corrected.data());sampler(p,lit?"DiffuseSampler":"Sampler0",0);sampler(p,"AlphaSampler",1);
      // Light uniforms remain at the fresh linked program's real values;
      // source commitLightParameter(NULL) makes no GL call.
     }else{attribute(p,"position",3,positions);attribute(p,"color",4,colors);attribute(p,"texcoord",2,uv);matrix(p,"mvp",identity);matrix(p,"texture_matrix",material.texture_matrix);vector(p,"material_color",corrected.data());scalar(p,"has_alpha",lit?1:0);scalar(p,"original_alpha_blue",lit?1:0);scalar(p,"alpha_ref",material.alpha_ref);sampler(p,"diffuse",0);sampler(p,"alpha_map",1);}
     glDrawArrays(GL_TRIANGLES,0,3);std::array<unsigned char,4> pixel;glReadPixels(16,12,1,1,GL_RGBA,GL_UNSIGNED_BYTE,pixel.data());assert(glGetError()==GL_NO_ERROR);for(unsigned i=0;i<16;++i)glDisableVertexAttribArray(i);return pixel;};
    const auto a=render(original,true),b=render(generic,false);
    for(unsigned c=0;c<4;++c)if(std::abs(int(a[c])-int(b[c]))>1)throw std::runtime_error("Original GLSL versus menu adapter pixel differs: "+material.id+" channel "+std::to_string(c)+" original "+std::to_string(a[c])+" adapter "+std::to_string(b[c]));
    if(lit)assert(a[0]==0&&a[1]==0&&a[2]==0);
   }
   glDeleteProgram(original);++draws;
  }
 }
 assert(draws==4&&silhouette&&authored_tint);
 auto unsupported=scene.materials.front();unsupported.gles2_technique="unknown";std::array<float,4> color;assert(!menu_background_material_color_v124(image,unsupported,color,error));
 // A future authored positive light binding must not be guessed or confused
 // with the shipping NULL branch. Point the real material URL cell at the
 // existing '#Omni01-light' instance URL and require an explicit failure.
 auto changed=bytes;const std::string positive="#Omni01-light";
 const auto uri=std::search(changed.begin(),changed.end(),positive.begin(),positive.end());assert(uri!=changed.end());
 auto word=[](const std::uint8_t* p){std::uint32_t v;std::memcpy(&v,p,4);return v;};
 auto row=dh2_bres_library_item(&image,dh2::resources::Library::material,0);assert(row);
 const auto table=word(row+20),parameters=word(row+16);bool mutated{};
 for(unsigned i=0;i<parameters;++i){const auto q=table+i*24;const auto name=word(image.bytes+q);
  if(std::strcmp(reinterpret_cast<const char*>(image.bytes+name),"light0"))continue;
  const auto value=word(image.bytes+q+20),offset=std::uint32_t(uri-changed.begin());std::memcpy(changed.data()+value,&offset,4);mutated=true;
 }
 assert(mutated);dh2::resources::BresView changed_image;assert(dh2_bres_open(&changed_image,changed.data(),changed.size())==dh2::resources::BresError::ok);
 assert(!menu_background_material_color_v124(changed_image,scene.materials.front(),color,error));assert(error.find("positive live light transport")!=std::string::npos);
 glDeleteTextures(1,&alpha);glDeleteTextures(1,&diffuse);glDeleteProgram(generic);
 std::cout<<"PASS actual authored 4 menu batches, 149/255 ProfileCOMMON defaults, real normal/color streams, NULL-light L1 foliage, original-GLES pixel equality at 4 alpha values; Character/Gear source untouched\n";
 return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
