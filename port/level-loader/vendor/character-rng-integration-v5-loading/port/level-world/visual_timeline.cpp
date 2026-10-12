#include "visual_timeline.hpp"
#include <cmath>
#include <cstring>
namespace {
float add(float a,float b){volatile float r=a+b;return r;}
float sub(float a,float b){volatile float r=a-b;return r;}
float mul(float a,float b){volatile float r=a*b;return r;}
float seconds(std::int32_t ms){volatile float f=static_cast<float>(ms);volatile float r=f/1000.f;return r;}
std::int32_t wrap_sub(std::int32_t a,std::int32_t b){auto u=std::uint32_t(a)-std::uint32_t(b);std::int32_t r;std::memcpy(&r,&u,4);return r;}
float neg(float f){std::uint32_t u;std::memcpy(&u,&f,4);u^=0x80000000u;std::memcpy(&f,&u,4);return f;}
std::int32_t integer(float f){if(std::isnan(f))return 0;if(f>=2147483648.f)return INT32_MAX;if(f<=-2147483648.f)return INT32_MIN;return static_cast<std::int32_t>(f);}
bool valid(const dh2::timeline::State* s){return s&&s->loop<=255&&s->ended<=255&&s->initialized<=255&&s->library_present<=1;}
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return a&&b&&(x<=y?y-x<an:x-y<bn);}
void jump(dh2::timeline::State& s,std::int32_t ms){s.current_ms=ms;s.current_seconds=seconds(ms);s.initialized=s.ended=0;}
std::int32_t extra(const dh2::timeline::State& s){auto frame=integer(mul(s.frame_seconds,1000.f));auto delta=wrap_sub(integer(mul(s.current_seconds,1000.f)),s.current_ms);return delta>=0&&delta<frame?wrap_sub(frame,delta):0;}
}
extern "C" int dh2_timeline_update(dh2::timeline::State* p,std::int32_t absolute_ms,const dh2::timeline::Services* callbacks){
 if(!valid(p)||overlap(p,56,callbacks,16))return -1;
 auto& s=*p;const float now=seconds(absolute_ms);float elapsed=0.f;
 if(!s.initialized){s.initialized=1;s.current_seconds=add(s.current_seconds,0.f);s.last_seconds=now;}
 else {elapsed=mul(sub(now,s.last_seconds),s.scale);s.current_seconds=add(elapsed,s.current_seconds);s.last_seconds=now;}
 float limit;float base;bool crossed;
 if(elapsed<0.f){limit=seconds(s.start_ms);base=add(s.start_seconds,s.length_seconds);s.frame_seconds=neg(elapsed);crossed=s.current_seconds<limit;}
 else {limit=seconds(s.end_ms);base=s.start_seconds;s.frame_seconds=elapsed;crossed=s.current_seconds>limit;}
 if(crossed){
  if(s.loop){float remainder=s.length_seconds==0.f?0.f:std::fmod(sub(s.current_seconds,limit),s.length_seconds);s.current_seconds=add(base,remainder);if(callbacks&&callbacks->invoke)callbacks->invoke(callbacks->context,p);}
  else {s.current_seconds=limit;if(!s.ended){s.ended=1;if(callbacks&&callbacks->invoke)callbacks->invoke(callbacks->context,p);}}
 }
 s.current_ms=integer(mul(s.current_seconds,1000.f));return 0;
}
extern "C" int dh2_timeline_jump(dh2::timeline::State* s,std::int32_t ms){if(!valid(s))return -1;jump(*s,ms);return 0;}
extern "C" int dh2_timeline_init(dh2::timeline::State* s,std::int32_t start,std::int32_t end){if(!valid(s))return -1;s->start_ms=start;s->end_ms=end;return 0;}
extern "C" int dh2_timeline_range(dh2::timeline::State* s,std::int32_t start,std::int32_t end,std::uint32_t reset){if(!valid(s)||reset>255)return -1;if(!s->library_present){s->start_ms=start;s->end_ms=end;s->start_seconds=seconds(start);s->length_seconds=seconds(wrap_sub(end,start));}if(reset)jump(*s,s->start_ms);return 0;}
extern "C" int dh2_timeline_clip(dh2::timeline::State* s,std::int32_t index,std::int32_t start,std::int32_t end){if(!valid(s))return -1;s->clip_index=index;s->initialized=s->ended=0;s->start_ms=start;s->end_ms=end;s->start_seconds=seconds(start);s->length_seconds=seconds(wrap_sub(end,start));s->current_ms=start;s->current_seconds=s->start_seconds;return 0;}
extern "C" int dh2_timeline_loop(dh2::timeline::State* s,std::uint32_t loop){if(!valid(s)||loop>255)return -1;s->loop=loop;return 0;}
extern "C" int dh2_timeline_scale(dh2::timeline::State* s,float scale){if(!valid(s))return -1;s->scale=scale;return 0;}
extern "C" int dh2_timeline_notify(dh2::timeline::Completion* c,const dh2::timeline::State* s){if(!c||c->pending>255||(s&&!valid(s))||overlap(c,8,s,56))return -1;if(s)c->extra_ms=extra(*s);c->pending=1;return 0;}
extern "C" int dh2_timeline_extra(std::int32_t* out,const dh2::timeline::State* s){if(!out||(s&&!valid(s))||overlap(out,4,s,56))return -1;if(s)*out=extra(*s);return 0;}
extern "C" int dh2_timeline_replay(dh2::timeline::ReplayResult* out,dh2::timeline::State* s,const dh2::timeline::ReplayFacts* f,const dh2::timeline::ReplayServices* services){
 if(!out||!valid(s)||!f||f->reserved||f->requested_loop>255||f->displacement>255||f->root_present>1)return -1;
 const void* pointers[]={out,s,f,services};const std::size_t sizes[]={16,56,32,16};
 for(unsigned i=0;i<4;++i)for(unsigned j=i+1;j<4;++j)if(overlap(pointers[i],sizes[i],pointers[j],sizes[j]))return -1;
 if(f->mapped_clip==-1)return 0;
 if(f->previous_clip==f->mapped_clip&&!s->loop){auto u=std::uint32_t(s->start_ms)+std::uint32_t(f->applicator_extra_ms);std::int32_t ms;std::memcpy(&ms,&u,4);jump(*s,ms);}
 s->loop=f->requested_loop;s->scale=1.f;
 dh2::timeline::ReplayResult plan{s->current_ms,f->root_timestamp,std::uint32_t(f->displacement&&f->root_present&&f->root_timestamp),f->displacement};
 if(services&&services->invoke){services->invoke(services->context,dh2::timeline::new_animation,s,&plan);plan.current_ms=s->current_ms;services->invoke(services->context,dh2::timeline::blend_post,s,&plan);}
 plan.current_ms=s->current_ms;*out=plan;return 1;
}
