#include "authored_shader_program.hpp"
#include "../asset-payloads/sha256.hpp"
#include "../scene-materials/shader_sources.hpp"
#include <android/log.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>

namespace dh2::android_ui {
namespace {
std::string asset_bytes(AAssetManager* assets,const char* name,const char* expected){
    if(!assets)throw std::runtime_error("Shader asset manager unavailable");
    const std::string path=std::string("shaders/")+name;
    auto* asset=AAssetManager_open(assets,path.c_str(),AASSET_MODE_BUFFER);
    if(!asset)throw std::runtime_error("Authored shader missing: "+path);
    const auto size=AAsset_getLength64(asset);
    if(size<=0||size>65536){AAsset_close(asset);throw std::runtime_error("Authored shader size invalid");}
    std::string raw(static_cast<std::size_t>(size),'\0');std::size_t done=0;
    while(done<raw.size()){
        const auto n=AAsset_read(asset,&raw[done],raw.size()-done);
        if(n<=0){AAsset_close(asset);throw std::runtime_error("Short authored shader read");}
        done+=static_cast<std::size_t>(n);
    }
    AAsset_close(asset);
    dh2::assets::Sha256Digest digest{};
    if(!dh2::assets::sha256(reinterpret_cast<const std::uint8_t*>(raw.data()),raw.size(),digest))
        throw std::runtime_error("Shader hash failed");
    std::string hex;for(auto byte:digest){char value[3];std::snprintf(value,sizeof(value),"%02x",byte);hex+=value;}
    if(hex!=expected)throw std::runtime_error("Authored shader bytes changed: "+path);
    return raw;
}
dh2::scene::ShaderSourcePlan plan(const dh2::scene::ShaderSourcePack& pack,const char* name,unsigned type){
    const auto* member=pack.find(name);
    if(!member)throw std::runtime_error(std::string("Authored shader pack member missing: ")+name);
    dh2::scene::ShaderSourcePlan out;std::string error;
    const std::string_view body(reinterpret_cast<const char*>(member->bytes.data()),member->bytes.size());
    // GameSWF's authored passes supply an empty caller configuration. Precision
    // and driver capability flags remain zero until the real driver supplies them.
    if(!dh2::scene::shader_source_plan(0,type,name,{},nullptr,body,out,error))
        throw std::runtime_error("Authored shader source plan failed: "+error);
    return out;
}
GLuint compile(const dh2::scene::ShaderSourcePlan& source){
    auto shader=glCreateShader(source.gl_type);
    if(!shader)throw std::runtime_error("Cannot allocate authored shader");
    std::array<const char*,8> text{};std::array<GLint,8> sizes{};
    for(std::size_t i=0;i<text.size();++i){text[i]=source.chunks[i].data();sizes[i]=static_cast<GLint>(source.chunks[i].size());}
    glShaderSource(shader,static_cast<GLsizei>(text.size()),text.data(),sizes.data());glCompileShader(shader);
    GLint ok=0;glGetShaderiv(shader,GL_COMPILE_STATUS,&ok);
    if(!ok){char log[4096]{};glGetShaderInfoLog(shader,sizeof(log),nullptr,log);glDeleteShader(shader);
        throw std::runtime_error(std::string("Authored shader compile failed: ")+log);}
    return shader;
}
void check(const char* step){
    const auto code=glGetError();if(code!=GL_NO_ERROR){char value[160];std::snprintf(value,sizeof(value),"Authored UI %s GL error 0x%04x",step,code);throw std::runtime_error(value);}
}
}
Program create(AAssetManager* assets,bool premultiplied){
    const auto bytes=asset_bytes(assets,"shaders.pak","365a4d3c432454c44208ebb484c7a472a3a4534a0c5c9e77a7a90f3b87b1b5c0");
    dh2::scene::ShaderSourcePack pack;std::string error;
    if(!pack.load(reinterpret_cast<const std::uint8_t*>(bytes.data()),bytes.size(),error))
        throw std::runtime_error("Authored shader pack invalid: "+error);
    const auto vs=plan(pack,"GameSWFVS.glsl",4);
    const auto fs=plan(pack,premultiplied?"GameSWFFS_blend.glsl":"GameSWFFS.glsl",5);
    GLuint vertex=0,fragment=0;Program out;
    try{
        vertex=compile(vs);fragment=compile(fs);
        out.name=glCreateProgram();if(!out.name)throw std::runtime_error("Cannot allocate authored shader program");
        glAttachShader(out.name,vertex);glAttachShader(out.name,fragment);glLinkProgram(out.name);
        GLint ok=0;glGetProgramiv(out.name,GL_LINK_STATUS,&ok);
        if(!ok){char log[4096]{};glGetProgramInfoLog(out.name,sizeof(log),nullptr,log);throw std::runtime_error(std::string("Authored shader link failed: ")+log);}
        out.position=glGetAttribLocation(out.name,"Position");out.uv=glGetAttribLocation(out.name,"TexCoord0");
        out.color=glGetAttribLocation(out.name,"Color0");out.matrix=glGetUniformLocation(out.name,"WorldViewProjectionMatrix");
        out.diffuse=glGetUniformLocation(out.name,"DiffuseColor");out.sampler=glGetUniformLocation(out.name,"TextureSampler");
        if(out.position<0||out.uv<0||out.color<0||out.matrix<0||out.diffuse<0||out.sampler<0)
            throw std::runtime_error("Authored UI shader interface incomplete");
        glDeleteShader(vertex);glDeleteShader(fragment);vertex=fragment=0;check("program creation");return out;
    }catch(...){if(vertex)glDeleteShader(vertex);if(fragment)glDeleteShader(fragment);release(out);throw;}
}
void release(Program& value) noexcept{if(value.name)glDeleteProgram(value.name);value={};}
void draw_quad(const Program& value,GLuint texture,const std::array<float,16>& matrix,
               const std::array<float,4>& color,const std::array<float,4>& diffuse){
    if(!value.name||!texture)throw std::runtime_error("Authored UI draw owner unavailable");
    const GLfloat vertices[]={-1,1,0,0,-1,-1,0,1,1,1,1,0,1,-1,1,1};
    glUseProgram(value.name);glUniformMatrix4fv(value.matrix,1,GL_FALSE,matrix.data());
    glUniform4fv(value.diffuse,1,diffuse.data());glUniform1i(value.sampler,0);
    glActiveTexture(GL_TEXTURE0);glBindTexture(GL_TEXTURE_2D,texture);glBindBuffer(GL_ARRAY_BUFFER,0);
    glDisableVertexAttribArray(value.color);glVertexAttrib4fv(value.color,color.data());
    glEnableVertexAttribArray(value.position);glEnableVertexAttribArray(value.uv);
    glVertexAttribPointer(value.position,2,GL_FLOAT,GL_FALSE,4*sizeof(GLfloat),vertices);
    glVertexAttribPointer(value.uv,2,GL_FLOAT,GL_FALSE,4*sizeof(GLfloat),vertices+2);
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    glDisableVertexAttribArray(value.position);glDisableVertexAttribArray(value.uv);
}
void validate_pixels(const Program& normal,const Program& premultiplied){
    GLint prior_framebuffer=0,viewport[4]{};glGetIntegerv(GL_FRAMEBUFFER_BINDING,&prior_framebuffer);glGetIntegerv(GL_VIEWPORT,viewport);
    GLuint textures[2]{},framebuffer=0;unsigned max_error=0,cases=0;
    auto cleanup=[&](){glBindFramebuffer(GL_FRAMEBUFFER,prior_framebuffer);glDeleteFramebuffers(1,&framebuffer);
        glBindTexture(GL_TEXTURE_2D,0);glDeleteTextures(2,textures);glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);glUseProgram(0);};
    try{
        glGenTextures(2,textures);const std::array<GLubyte,4> pixel{64,96,160,128};
        for(unsigned i=0;i<2;++i){glBindTexture(GL_TEXTURE_2D,textures[i]);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,1,1,0,GL_RGBA,GL_UNSIGNED_BYTE,i?nullptr:pixel.data());}
        glGenFramebuffers(1,&framebuffer);glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,textures[1],0);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)throw std::runtime_error("Authored UI probe framebuffer incomplete");
        glViewport(0,0,1,1);glDisable(GL_DEPTH_TEST);glDisable(GL_CULL_FACE);glDisable(GL_BLEND);glDisable(GL_SCISSOR_TEST);
        glDisable(GL_STENCIL_TEST);glDisable(GL_DITHER);glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);glPixelStorei(GL_PACK_ALIGNMENT,1);
        const std::array<float,16> identity{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
        const std::array<std::array<float,4>,4> colors{{{1,1,1,1},{.75f,.5f,1,.5f},{.25f,1,.5f,0},{1,.5f,.25f,1}}};
        const std::array<std::array<float,4>,4> adds{{{0,0,0,0},{.1f,.2f,0,.1f},{.1f,0,.2f,0},{.05f,.1f,.15f,.1f}}};
        for(unsigned mode=0;mode<2;++mode)for(unsigned row=0;row<colors.size();++row){
            draw_quad(mode?premultiplied:normal,textures[0],identity,colors[row],adds[row]);
            std::array<GLubyte,4> observed{};glReadPixels(0,0,1,1,GL_RGBA,GL_UNSIGNED_BYTE,observed.data());
            std::array<float,4> expected{};for(unsigned channel=0;channel<4;++channel)expected[channel]=(pixel[channel]/255.f+adds[row][channel])*colors[row][channel];
            if(mode)for(unsigned channel=0;channel<3;++channel)expected[channel]*=expected[3];
            for(unsigned channel=0;channel<4;++channel){const int gold=static_cast<int>(std::lround(std::clamp(expected[channel],0.f,1.f)*255.f));
                const auto difference=static_cast<unsigned>(std::abs(gold-int(observed[channel])));max_error=std::max(max_error,difference);
                if(difference>2)throw std::runtime_error("Authored UI shader GPU color/alpha mismatch");}
            ++cases;
        }
        check("pixel probe");cleanup();glEnable(GL_DITHER);
        __android_log_print(ANDROID_LOG_INFO,"DH2Native","Authored UI shader GPU contract PASS | programs 2 | cases %u | max byte error %u",cases,max_error);
    }catch(...){cleanup();glEnable(GL_DITHER);throw;}
}
}
