#include "swf_gpu.hpp"
#include "../scene-materials/swf_texture.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <stdexcept>

namespace dh2::android_ui {
namespace {
void check_gl(const char* action){
    const auto code=glGetError();
    if(code!=GL_NO_ERROR)throw std::runtime_error(std::string(action)+" GL error "+std::to_string(code));
}
void finite(float x){if(!std::isfinite(x))throw std::runtime_error("Nonfinite SWF render value");}
}
void SwfGpu::initialize(AAssetManager* assets){
    // Retained CPU texture identities survive GL recreation; old names must not
    // be deleted in a new context whose names may have already been recycled.
    normal_={};premultiplied_={};buffer_=0;frame_=false;mask_level_=0;submitting_mask_=false;begin_pending_=false;
    target_=query_target_=target_color_=query_color_=stencil_buffer_=0;target_width_=target_height_=0;
    for(auto& pair:textures_)pair.second.name=0;
    normal_=create(assets,false);premultiplied_=create(assets,true);
    glGenBuffers(1,&buffer_);if(!buffer_)throw std::runtime_error("SWF vertex buffer unavailable");
    if(!white_){ui::SwfTexture out;std::string error;const std::uint8_t white[4]{255,255,255,255};
        if(!image(1,1,4,white,4,out,error))throw std::runtime_error(error);white_=out.identity;}
    for(auto& pair:textures_)if(!pair.second.name)upload(pair.second);
    check_gl("SWF context initialization");
}
void SwfGpu::upload(Texture& texture){
    GLint limit=0;glGetIntegerv(GL_MAX_TEXTURE_SIZE,&limit);
    if(texture.width>limit||texture.height>limit)throw std::runtime_error("SWF bitmap exceeds GPU dimensions");
    const GLenum format=texture.channels==1?GL_ALPHA:texture.channels==3?GL_RGB:GL_RGBA;
    GLuint candidate=0;glGenTextures(1,&candidate);glBindTexture(GL_TEXTURE_2D,candidate);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D,0,format,texture.width,texture.height,0,format,GL_UNSIGNED_BYTE,texture.pixels.data());
    try{check_gl("SWF bitmap upload");}catch(...){if(candidate)glDeleteTextures(1,&candidate);throw;}
    texture.name=candidate;
}
void SwfGpu::target(int width,int height){
    if(target_width_==width&&target_height_==height)return;
    GLint limit=0;glGetIntegerv(GL_MAX_TEXTURE_SIZE,&limit);
    if(width<=0||height<=0||width>limit||height>limit)throw std::runtime_error("Invalid SWF framebuffer size");
    if(target_)glDeleteFramebuffers(1,&target_);
    if(query_target_)glDeleteFramebuffers(1,&query_target_);
    if(target_color_)glDeleteTextures(1,&target_color_);
    if(query_color_)glDeleteTextures(1,&query_color_);
    if(stencil_buffer_)glDeleteRenderbuffers(1,&stencil_buffer_);
    target_=query_target_=target_color_=query_color_=stencil_buffer_=0;target_width_=target_height_=0;
    glGenRenderbuffers(1,&stencil_buffer_);glBindRenderbuffer(GL_RENDERBUFFER,stencil_buffer_);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_STENCIL_INDEX8,width,height);
    auto make=[&](GLuint& texture,GLuint& framebuffer){
        glGenTextures(1,&texture);glBindTexture(GL_TEXTURE_2D,texture);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,nullptr);
        glGenFramebuffers(1,&framebuffer);glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texture,0);
        // Both color targets share the actual stencil owner. Queries never need
        // unsupported GLES2 stencil readback or modifications to displayed color.
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_STENCIL_ATTACHMENT,GL_RENDERBUFFER,stencil_buffer_);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
            throw std::runtime_error("SWF shared-stencil framebuffer incomplete");
    };
    make(target_color_,target_);make(query_color_,query_target_);
    target_width_=width;target_height_=height;check_gl("SWF framebuffer allocation");
}
bool SwfGpu::image(std::int32_t width,std::int32_t height,unsigned channels,const std::uint8_t* pixels,
                   std::size_t pitch,ui::SwfTexture& out,std::string& error){
    try{
        if(!pixels||width<=0||height<=0||width>16384||height>16384||(channels!=1&&channels!=3&&channels!=4))
            throw std::runtime_error("Invalid SWF bitmap request");
        const auto row=std::size_t(width)*channels;
        if(pitch<row||pitch>64*1024*1024||std::size_t(height)>64*1024*1024/row)
            throw std::runtime_error("SWF bitmap storage exceeds bounds");
        if(next_identity_==std::numeric_limits<std::uintptr_t>::max())throw std::runtime_error("SWF texture identity exhausted");
        Texture candidate;candidate.width=width;candidate.height=height;candidate.channels=channels;
        candidate.pixels.resize(row*height);
        for(int y=0;y<height;++y)std::memcpy(candidate.pixels.data()+row*y,pixels+pitch*y,row);
        if(normal_.name)upload(candidate);
        const auto id=next_identity_++;
        textures_.emplace(id,std::move(candidate));out={id,width,height};error.clear();return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
void SwfGpu::primitive(const ui::SwfDraw& command,const ui::SwfFill& style,GLenum mode){
    if(style.kind==ui::SwfFill::disabled&&!submitting_mask_)return;
    if(!normal_.name||!buffer_)throw std::runtime_error("SWF GPU context unavailable");
    auto xy=command.xy;
    const bool quad=command.kind==ui::SwfDraw::bitmap_quad;
    if(quad)xy={command.rect[0],command.rect[2],command.rect[1],command.rect[2],
                command.rect[0],command.rect[3],command.rect[1],command.rect[3]};
    if(xy.empty())return;
    if(xy.size()%2||xy.size()>2*1024*1024)throw std::runtime_error("Invalid SWF vertex span");
    const auto identity=style.kind==ui::SwfFill::bitmap?style.texture.identity:white_;
    auto found=textures_.find(identity);
    if(found==textures_.end())throw std::runtime_error("Required SWF texture owner missing");
    const auto& texture=found->second;
    if(!texture.name)throw std::runtime_error("Required SWF texture upload missing");
    if(style.kind==ui::SwfFill::bitmap&&(style.texture.width!=texture.width||style.texture.height!=texture.height))
        throw std::runtime_error("SWF texture dimensions disagree with owner");
    for(auto value:command.matrix.value)finite(value);
    for(auto value:style.uv.value)finite(value);
    const float sx=2.f/(bounds_[1]-bounds_[0]),sy=-2.f/(bounds_[3]-bounds_[2]);
    const auto& m=command.matrix.value;
    const std::array<float,16> matrix{
        sx*m[0],sy*m[3],0,0,sx*m[1],sy*m[4],0,0,0,0,1,0,
        sx*(m[2]-bounds_[0])-1.f,sy*(m[5]-bounds_[2])+1.f,0,1};
    std::vector<float> vertices;vertices.reserve(xy.size()*2);
    for(std::size_t i=0;i<xy.size();i+=2){
        const float x=xy[i],y=xy[i+1];finite(x);finite(y);
        float u=0,v=0;
        if(style.kind==ui::SwfFill::bitmap){
            if(quad){const unsigned vertex=static_cast<unsigned>(i/2);u=command.uv_rect[vertex%2];v=command.uv_rect[2+vertex/2];}
            else{const auto& uv=style.uv.value;u=(uv[0]*x+uv[1]*y+uv[2])/texture.width;v=(uv[3]*x+uv[4]*y+uv[5])/texture.height;}
        }
        finite(u);finite(v);vertices.insert(vertices.end(),{x,y,u,v});
    }
    // These four pass selections and GL blend factors were executed against the
    // original GameSWF material. Unknown authored modes must be implemented.
    const Program* program=&normal_;GLenum source=GL_SRC_ALPHA,destination=GL_ONE_MINUS_SRC_ALPHA;
    const auto blend=style.kind==ui::SwfFill::bitmap&&!quad?style.blend:0;
    switch(blend){
        case 0:case 1:case 15:case 16:break;
        case 3:program=&premultiplied_;source=GL_DST_COLOR;break;
        case 4:program=&premultiplied_;source=GL_ONE;destination=GL_ONE_MINUS_SRC_COLOR;break;
        case 13:program=&premultiplied_;source=GL_DST_COLOR;destination=GL_ONE;break;
        default:throw std::runtime_error("Required SWF blend mode unsupported: "+std::to_string(blend));
    }
    // Recovered immediate draw: solid color applies full cxform and truncates;
    // bitmap fills use multiplicative terms only; glyphs use caller byte RGBA.
    // Cached render-list playback is a separate source path, not this adapter.
    std::array<float,4> color{};
    scene::SwfCxform32 cx{};
    for(unsigned i=0;i<8;++i){finite(command.color_transform.value[i]);cx.terms[i]=command.color_transform.value[i];}
    std::array<std::uint8_t,4> bytes{};
    if(quad)std::copy(std::begin(style.rgba),std::end(style.rgba),bytes.begin());
    else if(style.kind==ui::SwfFill::bitmap)bytes=scene::swf_bitmap_color(cx).rgba;
    else {std::array<std::uint8_t,4> raw{};std::copy(std::begin(style.rgba),std::end(style.rgba),raw.begin());
          bytes=scene::swf_solid_color(cx,raw);}
    for(unsigned i=0;i<4;++i){
        color[i]=bytes[i]/255.f;
    }
    // Modern GL_ALPHA sampling supplies zero RGB; this offset yields white RGB
    // with the authored coverage alpha. It is an adapter representation, not a
    // claim that original font image format12 was texture format2.
    const std::array<float,4> diffuse=texture.channels==1?std::array<float,4>{1,1,1,0}:std::array<float,4>{0,0,0,0};
    glEnable(GL_BLEND);glBlendEquation(GL_FUNC_ADD);
    // The private color target accumulates premultiplied color for compositing;
    // preserve the recovered source RGB factors while retaining coverage alpha.
    glBlendFuncSeparate(source,destination,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(program->name);glUniformMatrix4fv(program->matrix,1,GL_FALSE,matrix.data());
    glUniform4fv(program->diffuse,1,diffuse.data());glUniform1i(program->sampler,0);
    glActiveTexture(GL_TEXTURE0);glBindTexture(GL_TEXTURE_2D,texture.name);
    std::uint32_t wrap=GL_CLAMP_TO_EDGE;
    if(!scene::swf_texture_gl_wrap(style.kind==ui::SwfFill::bitmap&&!quad?(style.wrap==0?0:2):1,wrap))
        throw std::runtime_error("Invalid SWF texture wrap producer");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrap);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrap);
    glDisableVertexAttribArray(program->color);glVertexAttrib4fv(program->color,color.data());
    glBindBuffer(GL_ARRAY_BUFFER,buffer_);
    glBufferData(GL_ARRAY_BUFFER,static_cast<GLsizeiptr>(vertices.size()*sizeof(float)),vertices.data(),GL_STREAM_DRAW);
    glEnableVertexAttribArray(program->position);glEnableVertexAttribArray(program->uv);
    glVertexAttribPointer(program->position,2,GL_FLOAT,GL_FALSE,4*sizeof(float),nullptr);
    glVertexAttribPointer(program->uv,2,GL_FLOAT,GL_FALSE,4*sizeof(float),reinterpret_cast<void*>(2*sizeof(float)));
    glDrawArrays(mode,0,static_cast<GLsizei>(xy.size()/2));
    glDisableVertexAttribArray(program->position);glDisableVertexAttribArray(program->uv);glBindBuffer(GL_ARRAY_BUFFER,0);
}
void SwfGpu::mask_rectangle(){
    ui::SwfDraw command;command.kind=ui::SwfDraw::triangle_strip;
    command.xy={bounds_[0],bounds_[2],bounds_[1],bounds_[2],bounds_[0],bounds_[3],bounds_[1],bounds_[3]};
    command.fill.kind=ui::SwfFill::color;
    primitive(command,command.fill,GL_TRIANGLE_STRIP);
}
void SwfGpu::abort() noexcept {
    if(frame_||begin_pending_){glBindFramebuffer(GL_FRAMEBUFFER,destination_);glViewport(viewport_[0],viewport_[1],viewport_[2],viewport_[3]);}
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);glDepthMask(GL_TRUE);glDisable(GL_STENCIL_TEST);
    frame_=false;begin_pending_=false;mask_level_=0;submitting_mask_=false;
}
bool SwfGpu::reset_images(std::string& error){
    abort();
    for(auto& pair:textures_)if(pair.second.name)glDeleteTextures(1,&pair.second.name);
    textures_.clear();white_=0;next_identity_=1;
    ui::SwfTexture out;const std::uint8_t white[4]{255,255,255,255};
    if(!image(1,1,4,white,4,out,error))return false;
    white_=out.identity;return true;
}
bool SwfGpu::draw(const ui::SwfDraw& command,std::string& error){
    try{
        using Draw=ui::SwfDraw;
        if(command.kind!=Draw::begin&&!frame_)throw std::runtime_error("SWF command outside display lifetime");
        switch(command.kind){
        case Draw::begin:{
            if(frame_)throw std::runtime_error("Nested SWF display begin");
            for(unsigned i=0;i<4;++i){finite(command.bounds[i]);bounds_[i]=command.bounds[i];viewport_[i]=command.viewport[i];}
            if(bounds_[1]<=bounds_[0]||bounds_[3]<=bounds_[2]||viewport_[2]<=0||viewport_[3]<=0)
                throw std::runtime_error("Invalid SWF display extent");
            glGetIntegerv(GL_FRAMEBUFFER_BINDING,&destination_);begin_pending_=true;target(viewport_[2],viewport_[3]);
            glBindFramebuffer(GL_FRAMEBUFFER,target_);
            GLint bits=0;glGetIntegerv(GL_STENCIL_BITS,&bits);
            if(bits<8)throw std::runtime_error("SWF masks require an eight-bit stencil framebuffer");
            glViewport(0,0,viewport_[2],viewport_[3]);
            glDisable(GL_DEPTH_TEST);glDisable(GL_CULL_FACE);glDisable(GL_SCISSOR_TEST);glDisable(GL_STENCIL_TEST);
            glDepthMask(GL_FALSE);glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);glStencilMask(0xff);
            // The original glitch backend ignores begin_display's background
            // color and preserves the scene underneath. This private coverage
            // target starts transparent and composites over that same scene.
            glClearColor(0,0,0,0);glClear(GL_COLOR_BUFFER_BIT);
            frame_=true;begin_pending_=false;mask_level_=0;submitting_mask_=false;break;}
        case Draw::end:
            if(mask_level_||submitting_mask_)throw std::runtime_error("Unbalanced SWF mask lifetime");
            glDisable(GL_STENCIL_TEST);glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
            glBindFramebuffer(GL_FRAMEBUFFER,destination_);glViewport(viewport_[0],viewport_[1],viewport_[2],viewport_[3]);
            glEnable(GL_BLEND);glBlendEquation(GL_FUNC_ADD);glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
            {const std::array<float,16> flipped{1,0,0,0,0,-1,0,0,0,0,1,0,0,0,0,1};
             draw_quad(normal_,target_color_,flipped,{1,1,1,1},{0,0,0,0});}
            glDepthMask(GL_TRUE);frame_=false;break;
        case Draw::triangles:primitive(command,command.fill,GL_TRIANGLES);break;
        case Draw::triangle_strip:primitive(command,command.fill,GL_TRIANGLE_STRIP);break;
        case Draw::bitmap_quad:primitive(command,command.fill,GL_TRIANGLE_STRIP);break;
        case Draw::line_strip:
            finite(command.line_width);if(command.line_width<=0)break;
            // Source widths are twips; actual affine scale and viewport turn them
            // into pixel widths. GLES2 hardware clamps to its supported range.
            {const float x=std::hypot(command.matrix.value[0],command.matrix.value[3]);
             const float y=std::hypot(command.matrix.value[1],command.matrix.value[4]);
             const float scale=(viewport_[2]/(bounds_[1]-bounds_[0])+viewport_[3]/(bounds_[3]-bounds_[2]))*.5f;
             GLfloat range[2];glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE,range);
             glLineWidth(std::clamp(command.line_width*(x+y)*.5f*scale,range[0],range[1]));}
            primitive(command,command.line,GL_LINE_STRIP);break;
        case Draw::mask_begin:
            if(mask_level_>=255)throw std::runtime_error("Invalid SWF mask begin");
            if(!mask_level_){glEnable(GL_STENCIL_TEST);glClearStencil(0);glClear(GL_STENCIL_BUFFER_BIT);}
            glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
            glStencilFunc(GL_EQUAL,mask_level_++,0xff);glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);submitting_mask_=true;break;
        case Draw::mask_end:
            if(!submitting_mask_||!mask_level_)throw std::runtime_error("Invalid SWF mask end");
            glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);glStencilFunc(GL_EQUAL,mask_level_,0xff);
            glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);submitting_mask_=false;break;
        case Draw::mask_disable:
            if(!mask_level_)throw std::runtime_error("Invalid SWF mask disable");
            submitting_mask_=false;
            if(!--mask_level_)glDisable(GL_STENCIL_TEST);
            else{glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);glStencilFunc(GL_EQUAL,mask_level_+1,0xff);
                 glStencilOp(GL_KEEP,GL_KEEP,GL_DECR);mask_rectangle();
                 glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);glStencilFunc(GL_EQUAL,mask_level_,0xff);glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);}
            break;
        case Draw::antialias:break; // frame multisampling belongs to the EGL config
        }
        check_gl("SWF drawing command");error.clear();return true;
    }catch(const std::exception& e){
        abort();
        error=e.what();return false;
    }
}
bool SwfGpu::stencil(const float bounds[4],std::uint8_t pattern,bool& out,std::string& error){
    try{
        if(!frame_||!query_target_)throw std::runtime_error("SWF stencil query outside display lifetime");
        for(unsigned i=0;i<4;++i)finite(bounds[i]);
        // Upstream sprite hitTest supplies world pixels after twips_to_pixels.
        // Convert them back through the same authored stage/viewport projection.
        const float sx=target_width_/(bounds_[1]-bounds_[0]),sy=target_height_/(bounds_[3]-bounds_[2]);
        auto coordinate=[](float value,int size,bool upper){
            return static_cast<int>(std::clamp(upper?std::ceil(value):std::floor(value),0.f,float(size)));};
        const int x0=coordinate((bounds[0]*20.f-bounds_[0])*sx,target_width_,false);
        const int x1=coordinate((bounds[1]*20.f-bounds_[0])*sx,target_width_,true);
        const int y0=coordinate((bounds[2]*20.f-bounds_[2])*sy,target_height_,false);
        const int y1=coordinate((bounds[3]*20.f-bounds_[2])*sy,target_height_,true);
        bool result=false;
        if(x1>x0&&y1>y0){
            glBindFramebuffer(GL_FRAMEBUFFER,query_target_);glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
            glClearColor(0,0,0,0);glClear(GL_COLOR_BUFFER_BIT);
            glEnable(GL_STENCIL_TEST);glStencilFunc(GL_EQUAL,pattern,0xff);glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
            mask_rectangle();
            std::vector<std::uint8_t> pixels(std::size_t(x1-x0)*(y1-y0)*4);
            glPixelStorei(GL_PACK_ALIGNMENT,1);
            glReadPixels(x0,target_height_-y1,x1-x0,y1-y0,GL_RGBA,GL_UNSIGNED_BYTE,pixels.data());
            for(std::size_t i=3;i<pixels.size();i+=4)if(pixels[i]){result=true;break;}
            glBindFramebuffer(GL_FRAMEBUFFER,target_);
            if(mask_level_){glEnable(GL_STENCIL_TEST);
                glStencilFunc(GL_EQUAL,submitting_mask_?mask_level_-1:mask_level_,0xff);
                glStencilOp(GL_KEEP,GL_KEEP,submitting_mask_?GL_INCR:GL_KEEP);}
            else glDisable(GL_STENCIL_TEST);
            glColorMask(!submitting_mask_,!submitting_mask_,!submitting_mask_,!submitting_mask_);
        }
        check_gl("SWF shared-stencil query");out=result;error.clear();return true;
    }catch(const std::exception& e){
        if(frame_)glBindFramebuffer(GL_FRAMEBUFFER,target_);
        error=e.what();return false;
    }
}
}
