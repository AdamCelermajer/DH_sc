// Host GL call/state contract only, not a GLES driver or pixel proof. Production
// SwfGpu/cache/color conversion execute unchanged; the shader/GL transport is
// explicit so this fixture requires no emulator or GPU resource allocation.
#include "swf_gpu.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
using dh2::android_ui::SwfGpu;using dh2::ui::SwfDraw;
namespace {unsigned checks{},calls{},draws{},composites{},clears{},capabilities{};GLenum error_code{};GLuint next_name=1;GLint framebuffer{},viewport[]{0,0,64,64};GLfloat line_width=1;
void check(bool value,const char* message){++checks;if(!value)throw std::runtime_error(message);}
void draw(SwfGpu& gpu,const SwfDraw& d){std::string error;check(gpu.draw(d,error),error.c_str());}
SwfDraw begin(){SwfDraw d;d.kind=SwfDraw::begin;d.bounds[1]=d.bounds[3]=1280;d.viewport[2]=d.viewport[3]=64;return d;}
SwfDraw end(){SwfDraw d;d.kind=SwfDraw::end;return d;}
SwfDraw square(unsigned char alpha=255){SwfDraw d;d.kind=SwfDraw::triangle_strip;d.fill.kind=dh2::ui::SwfFill::color;d.fill.rgba[0]=255;d.fill.rgba[3]=alpha;d.xy={320,320,960,320,320,960,960,960};return d;}
void fail(SwfGpu& gpu,SwfDraw d,const char* text){std::string error;check(!gpu.draw(d,error)&&error.find(text)!=std::string::npos,text);}
}
extern "C" {
GLenum glGetError(){++calls;auto code=error_code;error_code=0;return code;}
void glGetIntegerv(GLenum name,GLint* out){++calls;if(name==GL_MAX_TEXTURE_SIZE){++capabilities;*out=4096;}else if(name==GL_STENCIL_BITS){++capabilities;*out=8;}else if(name==GL_FRAMEBUFFER_BINDING)*out=framebuffer;else if(name==GL_VIEWPORT)for(unsigned i=0;i<4;++i)out[i]=viewport[i];else throw std::runtime_error("unexpected integer capability");}
void glGetFloatv(GLenum name,GLfloat* out){++calls;++capabilities;check(name==GL_ALIASED_LINE_WIDTH_RANGE,"unexpected float capability");out[0]=1;out[1]=8;}
void glGenBuffers(GLsizei n,GLuint* out){++calls;while(n--)*out++=next_name++;}
void glGenTextures(GLsizei n,GLuint* out){glGenBuffers(n,out);}
void glGenFramebuffers(GLsizei n,GLuint* out){glGenBuffers(n,out);}
void glGenRenderbuffers(GLsizei n,GLuint* out){glGenBuffers(n,out);}
void glDeleteBuffers(GLsizei,const GLuint*){++calls;}void glDeleteTextures(GLsizei,const GLuint*){++calls;}
void glDeleteFramebuffers(GLsizei,const GLuint*){++calls;}void glDeleteRenderbuffers(GLsizei,const GLuint*){++calls;}
void glBindBuffer(GLenum,GLuint){++calls;}void glBufferData(GLenum,GLsizeiptr,const void*,GLenum){++calls;}
void glBindTexture(GLenum,GLuint){++calls;}void glPixelStorei(GLenum,GLint){++calls;}
void glTexParameteri(GLenum,GLenum,GLint){++calls;}void glTexImage2D(GLenum,GLint,GLint,GLsizei,GLsizei,GLint,GLenum,GLenum,const void*){++calls;}
void glBindFramebuffer(GLenum,GLuint value){++calls;framebuffer=value;}void glBindRenderbuffer(GLenum,GLuint){++calls;}
void glRenderbufferStorage(GLenum,GLenum,GLsizei,GLsizei){++calls;}
void glFramebufferTexture2D(GLenum,GLenum,GLenum,GLuint,GLint){++calls;}void glFramebufferRenderbuffer(GLenum,GLenum,GLenum,GLuint){++calls;}
GLenum glCheckFramebufferStatus(GLenum){++calls;return GL_FRAMEBUFFER_COMPLETE;}
void glViewport(GLint x,GLint y,GLsizei w,GLsizei h){++calls;viewport[0]=x;viewport[1]=y;viewport[2]=w;viewport[3]=h;}
void glEnable(GLenum code){++calls;if(code==0xdead)error_code=GL_INVALID_ENUM;}void glDisable(GLenum){++calls;}
void glDepthMask(GLboolean){++calls;}void glColorMask(GLboolean,GLboolean,GLboolean,GLboolean){++calls;}
void glStencilMask(GLuint){++calls;}void glClearColor(GLfloat,GLfloat,GLfloat,GLfloat){++calls;}
void glClear(GLbitfield){++calls;++clears;}void glBlendEquation(GLenum){++calls;}
void glBlendFunc(GLenum,GLenum){++calls;}void glBlendFuncSeparate(GLenum,GLenum,GLenum,GLenum){++calls;}
void glUseProgram(GLuint){++calls;}void glUniformMatrix4fv(GLint,GLsizei,GLboolean,const GLfloat*){++calls;}
void glUniform4fv(GLint,GLsizei,const GLfloat*){++calls;}void glUniform1i(GLint,GLint){++calls;}
void glActiveTexture(GLenum){++calls;}void glDisableVertexAttribArray(GLuint){++calls;}
void glEnableVertexAttribArray(GLuint){++calls;}void glVertexAttrib4fv(GLuint,const GLfloat*){++calls;}
void glVertexAttribPointer(GLuint,GLint,GLenum,GLboolean,GLsizei,const void*){++calls;}
void glDrawArrays(GLenum,GLint,GLsizei){++calls;++draws;}void glLineWidth(GLfloat width){++calls;line_width=width;}
void glClearStencil(GLint){++calls;}void glStencilFunc(GLenum,GLint,GLuint){++calls;}void glStencilOp(GLenum,GLenum,GLenum){++calls;}
void glScissor(GLint,GLint,GLsizei,GLsizei){++calls;}void glReadPixels(GLint,GLint,GLsizei w,GLsizei h,GLenum,GLenum,void* out){++calls;auto* bytes=static_cast<unsigned char*>(out);for(int i=0;i<w*h*4;++i)bytes[i]=0;}
}
namespace dh2::android_ui {
Program create(AAssetManager*,bool){return {next_name++,0,1,2,3,4,5};}
void draw_quad(const Program&,GLuint,const std::array<float,16>&,const std::array<float,4>&,const std::array<float,4>&){++calls;++composites;}
}
int main(){try{
 SwfGpu gpu;gpu.initialize(nullptr);check(capabilities==2,"initial capability cache");
 auto start_calls=calls;draw(gpu,begin());draw(gpu,square(0));auto after=gpu.stats_v37();check(after.transparent_noops==1&&gpu.stats_v36().materializations==0,"normal alpha-zero materialized");
 check(calls==start_calls+2,"transparent primitive made GL calls");draw(gpu,end());check(composites==0&&clears==0,"transparent display GPU output touched");
 draw(gpu,begin());auto line=square(0);line.kind=SwfDraw::line_strip;line.line=line.fill;line.line_width=40;const auto old_width=line_width;start_calls=calls;draw(gpu,line);check(calls==start_calls&&line_width==old_width,"transparent line mutated GPU state");draw(gpu,end());
 draw(gpu,begin());draw(gpu,square());draw(gpu,end());check(draws==1&&composites==1&&gpu.stats_v36().materializations==1,"visible source draw absent");check(capabilities==3,"owned stencil capability cache");
 draw(gpu,begin());const auto queries=gpu.stats_v37().error_queries;for(unsigned i=0;i<1000;++i)draw(gpu,square());check(gpu.stats_v37().error_queries==queries,"warm primitive queried GL error");draw(gpu,end());check(capabilities==3,"capabilities re-queried per display");
 draw(gpu,begin());SwfDraw mask;mask.kind=SwfDraw::mask_begin;draw(gpu,mask);auto old_draws=draws;draw(gpu,square(0));check(draws==old_draws+1,"transparent stencil writer suppressed");mask.kind=SwfDraw::mask_end;draw(gpu,mask);draw(gpu,square(0));check(draws==old_draws+2,"masked color pass improperly suppressed");
 bool hit=true;std::string error;const float bounds[]{0,64,0,64};check(gpu.stencil(bounds,1,hit,error)&&!hit,"stencil query provider");mask.kind=SwfDraw::mask_disable;draw(gpu,mask);draw(gpu,end());
 draw(gpu,begin());auto bad=square(0);bad.xy[0]=std::numeric_limits<float>::quiet_NaN();fail(gpu,bad,"Nonfinite");
 draw(gpu,begin());bad=square(0);bad.fill.kind=dh2::ui::SwfFill::bitmap;bad.fill.texture={999999,1,1};fail(gpu,bad,"texture owner missing");
 const unsigned char texel[]{255,255,255,255};dh2::ui::SwfTexture texture;check(gpu.image(1,1,4,texel,4,texture,error),"bitmap resource");
 for(unsigned blend:{3u,4u,13u}){draw(gpu,begin());auto d=square();d.fill.kind=dh2::ui::SwfFill::bitmap;d.fill.texture=texture;d.fill.blend=blend;d.color_transform.value[6]=0;old_draws=draws;draw(gpu,d);check(draws==old_draws+1,"non-normal alpha-zero omitted");draw(gpu,end());}
 draw(gpu,begin());bad=square(0);bad.fill.kind=dh2::ui::SwfFill::bitmap;bad.fill.texture=texture;bad.fill.blend=99;bad.color_transform.value[6]=0;fail(gpu,bad,"blend mode unsupported");
 draw(gpu,begin());draw(gpu,square());glEnable(0xdead);fail(gpu,end(),"commands [");check(framebuffer==0,"deferred error did not abort target");
 for(auto barrier:{SwfDraw::mask_begin,SwfDraw::mask_end,SwfDraw::mask_disable}){draw(gpu,begin());draw(gpu,square());glEnable(0xdead);mask.kind=barrier;fail(gpu,mask,"GL error");}
 draw(gpu,begin());draw(gpu,square());glEnable(0xdead);check(!gpu.stencil(bounds,1,hit,error)&&error.find("commands [")!=std::string::npos,"stencil missed pending error");
 draw(gpu,begin());draw(gpu,square());glEnable(0xdead);check(!gpu.scene_pane(bounds,nullptr,[](void*,int,int,std::string&){return true;},error)&&error.find("commands [")!=std::string::npos,"scene missed pending error");
 draw(gpu,begin());draw(gpu,square());check(!gpu.scene_pane(bounds,nullptr,[](void*,int,int,std::string&){glEnable(0xdead);return true;},error)&&error.find("scene pane exit")!=std::string::npos,"scene callback error swallowed");
 glEnable(0xdead);check(!gpu.image(1,1,4,texel,4,texture,error)&&error.find("bitmap resource entry")!=std::string::npos,"resource missed pending error");
 const auto uploads=gpu.stats_v36().cache.context_reuploads;const auto caps=capabilities;gpu.initialize(nullptr);check(capabilities==caps+2,"new context capabilities not reset");draw(gpu,begin());draw(gpu,square());draw(gpu,end());check(gpu.stats_v36().cache.context_reuploads>uploads&&capabilities==caps+3,"new context retained cache not reuploaded");
 draw(gpu,begin());line=square();line.kind=SwfDraw::line_strip;line.line=line.fill;line.line_width=1000;draw(gpu,line);check(line_width==8,"line width capability clamp");draw(gpu,end());check(capabilities==caps+3,"line repeated capability query");
 check(glGetError()==GL_NO_ERROR,"host final error");std::cout<<"PASS host GL call/state contract (not pixels): normal-zero, line-zero,1000 warm deferred draws, transparent masks, non-normal passes, required validation/errors, scene/stencil/resource boundaries, context cache reset; checks="<<checks<<"\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
