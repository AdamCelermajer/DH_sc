#include "edit_text_display_v1.hpp"
#include <cmath>
#include <limits>
namespace dh2::ui::edit_text_display_v1 {
namespace {
bool need(bool b,const char* n,std::string& e){if(b)return true;e=std::string("source outer text display requires ")+n;return false;}
std::int32_t i32(float f){if(std::isnan(f))return 0;if(f>=2147483648.0f)return INT32_MAX;if(f<=-2147483648.0f)return INT32_MIN;return std::int32_t(f);}
std::uint32_t u32(float f){if(std::isnan(f)||f<=0)return 0;if(f>=4294967296.0f)return UINT32_MAX;return std::uint32_t(f);}
float neg(std::int32_t i){return float(std::uint32_t(0)-std::uint32_t(i)>INT32_MAX?std::int64_t(std::uint32_t(0)-std::uint32_t(i))-4294967296ll:std::int64_t(std::uint32_t(0)-std::uint32_t(i)));}
float finite(float f){return std::isfinite(f)?f:0.0f;}
text_display_v2::Matrix translated(const text_display_v2::Matrix& base,float x,float y){auto m=base;m[2]=finite((x*base[0]+y*base[1])+base[2]);m[5]=finite((x*base[3]+y*base[4])+base[5]);return m;}
std::uint32_t rgba(const text_filter_v1::Filter& f){return std::uint32_t(f.bytes[6])|(std::uint32_t(f.bytes[5])<<8)|(std::uint32_t(f.bytes[4])<<16)|(std::uint32_t(f.bytes[7])<<24);}
bool send(const Services&s,const Command&c,std::string&e){bool present{};return need(bool(s.renderer_present),"renderer presence",e)&&s.renderer_present(present,e)&&(!present||(need(bool(s.renderer),"renderer operation",e)&&s.renderer(c,e)));}
bool world(const Services&s,text_display_v2::Matrix&m,std::string&e){return need(bool(s.world_matrix),"world matrix",e)&&s.world_matrix(m,e);}
bool policy(const Services&s,Policy&p,std::string&e){return need(bool(s.policy),"live player/root policy",e)&&s.policy(p,e);}
bool records(const Services&s,const text_display_v2::Context&c,std::string&e){return need(bool(s.records),"actual glyph records",e)&&s.records(c,bool(c.override_rgba)||c.argument0||c.argument1||c.argument2,e);}
}
bool cursor(State& q,const Services&s,std::string&e){e.clear();Command c;c.kind=Command::matrix;if(!world(s,c.transform,e)||!send(s,c,e))return false;c.kind=Command::line_color;c.rgba=0xff0000ff;if(!send(s,c,e))return false;c.kind=Command::line_width;c.width=40;if(!send(s,c,e))return false;c.kind=Command::line;c.xy={q.xcursor,q.ycursor,q.xcursor,q.ycursor+q.text_height};return send(s,c,e);}
bool display(State&q,const Services&s,std::string&e){
 e.clear();Policy p;if(!policy(s,p,e))return false;
 if(p.buffering&&!p.flushing)return need(bool(s.enqueue),"root text queue",e)&&s.enqueue(e);
 Command c;
 if(q.border){c.kind=Command::matrix;if(!world(s,c.transform,e)||!send(s,c,e))return false;
  const auto r=q.rectangle;c.kind=Command::fill_color;c.rgba=q.background;if(!send(s,c,e))return false;
  c.kind=Command::mesh;c.xy={r[0],r[2],r[1],r[2],r[0],r[3],r[1],r[3]};if(!send(s,c,e))return false;
  c.kind=Command::line_color;c.rgba=0xff000000;if(!send(s,c,e))return false;c.kind=Command::line_width;c.width=0;if(!send(s,c,e))return false;
  c.kind=Command::line;c.xy={r[0],r[2],r[1],r[2],r[1],r[3],r[0],r[3],r[0],r[2]};if(!send(s,c,e))return false;
 }
 if(!policy(s,p,e))return false;c={};c.kind=Command::grid_fit;c.enabled=p.provider_scale==1&&q.grid_fit;if(!send(s,c,e))return false;
 if(!policy(s,p,e))return false;bool cached=false;
 if(p.render_cache){if(!need(bool(s.cache_validate),"actual render cache validator",e)||!s.cache_validate(cached,e))return false;
  if(cached){c.kind=Command::cache_draw;if(!send(s,c,e))return false;}}
 if(!cached){if(!policy(s,p,e))return false;if(p.render_cache){c.kind=Command::cache_set;c.enabled=true;if(!send(s,c,e))return false;}
  text_display_v2::Context ctx;ctx.provider_scale=p.provider_scale;
  if(!policy(s,p,e))return false;
  if(!p.filter_engine){auto current=q.owner;while(current&&(!current->effect||current->effect->filters.empty()))current=current->parent.lock();
   if(current){const auto count=current->effect->filters.size();bool base=true;
    for(std::size_t n=count;n;--n){auto effects=current->effect;if(!need(bool(effects)&&effects->filters.size()>=n,"stable selected filter storage",e))return false;
     const auto f=effects->filters[n-1];ctx={};ctx.provider_scale=p.provider_scale;
     if(f.word(0)==0){const auto x=i32(f.real(32)),y=i32(f.real(36));float cosine{},sine{};
      if(!need(bool(s.cosine)&&bool(s.sine),"shadow trig imports",e)||!s.cosine(f.real(8),cosine,e))return false;
      const float dx=neg(x)+cosine*f.real(12);if(!s.sine(f.real(8),sine,e))return false;const float dy=neg(y)+sine*f.real(12);
      text_display_v2::Matrix m{1,0,0,0,1,0};ctx.matrix=translated(m,dx*20,dy*20);ctx.override_rgba=rgba(f);ctx.argument1=std::uint8_t(x);ctx.argument2=std::uint8_t(y);
     }else if(f.word(0)==1){const auto x=std::uint8_t(u32(f.real(32))),y=std::uint8_t(u32(f.real(36)));if(!(x|y))continue;
      text_display_v2::Matrix m{1,0,0,0,1,0};ctx.matrix=translated(m,-float(x)*20,-float(y)*20);ctx.argument1=x;ctx.argument2=y;base=false;
     }else if(f.word(0)==2){auto color=rgba(f);const auto alpha=i32(float(f.bytes[7])*(float(std::int32_t(f.word(16)))/10.0f));const auto a=alpha>254?std::uint8_t(255):std::uint8_t(alpha);if(!a)continue;color=(color&0xffffff)|(std::uint32_t(a)<<24);
      text_display_v2::Matrix m{1,0,0,0,1,0};ctx.matrix=translated(m,neg(i32(f.real(32)))*20,neg(i32(f.real(36)))*20);ctx.override_rgba=color;
      const auto selected=f.real(32)<f.real(36)?f.real(36):f.real(32);ctx.argument0=std::uint8_t(u32(selected));
     }else continue;
     if(!records(s,ctx,e))return false;
    }
    if(base){ctx={};ctx.provider_scale=p.provider_scale;if(!records(s,ctx,e))return false;}
   }else if(!records(s,ctx,e))return false;
  }else if(!records(s,ctx,e))return false;
  if(!policy(s,p,e))return false;if(p.render_cache){c={};c.kind=Command::cache_set;c.enabled=false;if(!send(s,c,e))return false;}
 }
 if(q.focus&&!cursor(q,s,e))return false;
 if(q.callback_present)return need(bool(s.display_callback),"actual display callback",e)&&s.display_callback(e);
 return true;
}
}
