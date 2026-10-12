#include "viewport.hpp"
#include <cmath>
#include <cstring>
#include <limits>
using namespace dh2::ui;
namespace {
bool aligned(const void* p,std::uintptr_t a){return p && reinterpret_cast<std::uintptr_t>(p)%a==0;}
bool overlaps(const void* a,std::size_t an,const void* b,std::size_t bn){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<an:x-y<bn;}
bool valid(ViewportState64* s,const ViewportServices16* v){return aligned(s,8)&&aligned(v,8);}
std::int32_t add(std::int32_t a,std::int32_t b){std::uint32_t u=static_cast<std::uint32_t>(a)+static_cast<std::uint32_t>(b);std::int32_t r;std::memcpy(&r,&u,4);return r;}
std::int32_t sub(std::int32_t a,std::int32_t b){return add(a,static_cast<std::int32_t>(0u-static_cast<std::uint32_t>(b)));}
std::int32_t trunc32(float f){if(std::isnan(f))return 0;if(f>=2147483648.f)return INT32_MAX;if(f<=-2147483648.f)return INT32_MIN;return static_cast<std::int32_t>(f);}
int call(ViewportState64* s,const ViewportServices16* v,ViewportOperation op,const std::int32_t* values,const float* rect,ViewportResponse16& out){if(!v->invoke)return -2;ViewportRequest40 r{};r.operation=op;if(values)std::memcpy(r.values,values,16);if(rect)std::memcpy(r.rectangle,rect,16);return v->invoke(v->context,s,&r,&out)?0:-2;}
int orientation(ViewportState64* s,const ViewportServices16* v,std::int32_t& out){ViewportResponse16 r{};int e=call(s,v,ViewportOperation::orientation,nullptr,nullptr,r);out=r.values[0];return e;}
int point(ViewportState64* s,float* p,const ViewportServices16* v,bool to_screen){
 if(!valid(s,v)||!aligned(p,4)||overlaps(s,64,p,8))return -1;
 std::int32_t o;int e=orientation(s,v,o);if(e)return e;const bool straight=o==0||o==2;
 const float mw=s->movie_rect[1]-s->movie_rect[0],mh=s->movie_rect[3]-s->movie_rect[2];
 if(!to_screen){
  if(straight){const float sy=float(s->bounds[3])/(mh/20.f);const float dx=p[0]-float(s->bounds[0]);const float sx=float(s->bounds[2])/(mw/20.f);p[0]=dx/sx;p[1]=(p[1]-float(s->bounds[1]))/sy;}
  else{const float sy=float(s->bounds[2])/(mh/20.f);const float dx=p[0]-float(s->bounds[1]);const float sx=float(s->bounds[3])/(mw/20.f);p[0]=dx/sx;p[1]=(p[1]-float(s->bounds[0]))/sy;}
 }else{
  const float ex=straight?mw:mh,ey=straight?mh:mw;
  const float sx=float(s->viewport[2])/float(s->bounds[2]),sy=float(s->viewport[3])/float(s->bounds[3]);
  const float ox=(float(s->bounds[0])*20.f)/(float(s->bounds[2])/(ex/20.f));
  const float oy=(float(s->bounds[1])*20.f)/(float(s->bounds[3])/(ey/20.f));
  p[0]=p[0]*(straight?sx:sy)-(straight?ox:oy);
  p[1]=p[1]*(straight?sy:sx)-(straight?oy:ox);
 }
 return 0;
}
std::int32_t ease(std::int32_t a,std::int32_t b){if(a<b)return add(add(a,1),sub(b,a)/10);if(a>b)return add(a,sub(-1,sub(a,b)/10));return a;}
}
extern "C" int dh2_ui_screen_to_logical(ViewportState64* s,float* p,const ViewportServices16* v){return point(s,p,v,false);}
extern "C" int dh2_ui_logical_to_screen(ViewportState64* s,float* p,const ViewportServices16* v){return point(s,p,v,true);}
extern "C" int dh2_ui_display_rectangle(ViewportState64* s,float* out,const ViewportServices16* v){
 if(!valid(s,v)||!aligned(out,4)||overlaps(s,64,out,16))return -1;
 float lo[2]={s->movie_rect[0],s->movie_rect[2]},hi[2]={s->movie_rect[1],s->movie_rect[3]};
 int e=point(s,lo,v,true);if(e)return e;e=point(s,hi,v,true);if(e)return e;
 const float r[4]={lo[0],hi[0],lo[1],hi[1]};std::memcpy(out,r,16);return 0;
}
extern "C" int dh2_ui_set_bounds(ViewportState64* s,const std::int32_t* input,std::int32_t mode,const ViewportServices16* v){
 if(!valid(s,v)||!aligned(input,4)||overlaps(s,64,input,16))return -1;
 std::int32_t x=input[0],y=input[1],w=input[2],h=input[3],o;int e=orientation(s,v,o);if(e)return e;
 bool straight=o==0;if(!straight){e=orientation(s,v,o);if(e)return e;straight=o==2;}
 const float mw=s->movie_rect[1]-s->movie_rect[0],mh=s->movie_rect[3]-s->movie_rect[2];
 const float sx=float(w)/((straight?mw:mh)/20.f),sy=float(h)/((straight?mh:mw)/20.f);const float ratio=sy/sx;
 if(mode==1||mode==2){
  if((ratio>=1.f)==(mode==2)){const auto nh=trunc32(float(h)/ratio);const auto d=sub(nh,h);h=add(h,d);y=sub(y,d/2);}
  else{const auto nw=trunc32(float(w)*ratio);const auto d=sub(nw,w);w=add(w,d);x=sub(x,d/2);}
 }
 if(s->bounds[0]==x&&s->bounds[1]==y&&s->bounds[2]==w&&s->bounds[3]==h)return 0;
 s->bounds[0]=x;s->bounds[1]=y;s->bounds[2]=w;s->bounds[3]=h;
 const float px=float(w)/((straight?mw:mh)/20.f),py=float(h)/((straight?mh:mw)/20.f);
 s->pixel_scale=px<py?py:px;
 if(!s->player_receiver)return 0;
 float lo[2]={0.f,0.f},hi[2]={float(s->viewport[0])+float(s->viewport[2]),float(s->viewport[1])+float(s->viewport[3])};
 e=point(s,lo,v,false);if(e)return e;e=point(s,hi,v,false);if(e)return e;
 const float rect[4]={lo[0],lo[1],hi[0],hi[1]};ViewportResponse16 r{};
 return call(s,v,ViewportOperation::publish_viewport,nullptr,rect,r);
}
extern "C" int dh2_ui_set_viewport(ViewportState64* s,const std::int32_t* input,const ViewportServices16* v){
 if(!valid(s,v)||!aligned(input,4)||overlaps(s,64,input,16))return -1;
 if(std::memcmp(s->viewport,input,16)==0)return 0;
 std::int32_t copy[4];std::memcpy(copy,input,16);std::memcpy(s->viewport,input,16);return dh2_ui_set_bounds(s,copy,0,v);
}
extern "C" int dh2_ui_flash_camera_update(FlashCamera40* c,ViewportState64* s,const ViewportServices16* v){
 if(!aligned(c,8)||!valid(s,v)||overlaps(c,40,s,64))return -1;
 c->current[0]=ease(c->current[0],c->desired[0]);c->current[1]=ease(c->current[1],c->desired[1]);
 ViewportResponse16 r{};int e=call(s,v,ViewportOperation::driver_dimensions,nullptr,nullptr,r);if(e)return e;const auto w=r.values[0],h=r.values[1];
 if(c->limit_identity){
  auto lo=trunc32(c->limit_rect[0]);auto x=c->current[0];if(add(lo,x)>0){x=sub(0,lo);c->current[0]=x;}
  lo=trunc32(c->limit_rect[2]);auto y=c->current[1];if(add(lo,y)>0){y=sub(0,lo);c->current[1]=y;}
  auto hi=trunc32(c->limit_rect[1]);if(w>add(hi,x))c->current[0]=sub(w,hi);
  hi=trunc32(c->limit_rect[3]);if(h>add(hi,y))c->current[1]=sub(h,hi);
 }
 const std::int32_t vp[4]={0,0,w,h};e=call(s,v,ViewportOperation::camera_set_viewport,vp,nullptr,r);if(e)return e;
 const std::int32_t b[4]={c->current[0],c->current[1],w,h};return call(s,v,ViewportOperation::camera_set_bounds,b,nullptr,r);
}
