#include "swf_frame_schedule.hpp"
#include <cmath>
namespace {using namespace dh2::ui;
int call(SwfRootFrame32*s,const SwfFrameServices16*v,SwfFrameOp op,float delta=0,std::uintptr_t receiver=0){SwfFrameRequest24 q{op,delta,receiver,0,0};return v->invoke&&v->invoke(v->context,s,&q)==1?0:-2;}
}
namespace {
bool actions(const dh2::ui::SwfFrameActions16&b){return b.count<=b.capacity&&b.capacity<=4096&&(!b.capacity||(b.values&&reinterpret_cast<std::uintptr_t>(b.values)%alignof(std::uintptr_t)==0));}
bool separated(const void*a,std::size_t an,const void*b,std::size_t bn){if(!an||!bn)return true;auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=UINTPTR_MAX-an&&y<=UINTPTR_MAX-bn&&(x+an<=y||y+bn<=x);}
int sprite_call(dh2::ui::SwfSpriteFrame64*s,const dh2::ui::SwfSpriteServices16*v,dh2::ui::SwfSpriteOp op,float delta=0,int frame=0,int value=0,const std::uintptr_t*a=nullptr,unsigned count=0,int*result=nullptr){dh2::ui::SwfSpriteRequest32 q{op,delta,frame,value,a,count,0};int out=0;if(!v->invoke||v->invoke(v->context,s,&q,&out)!=1)return -2;if(result)*result=out;return 0;}
int signed_frame(std::uint16_t v){return v<32768?v:int(v)-65536;}
}
extern "C" int dh2_ui_swf_sprite_frame(dh2::ui::SwfSpriteFrame64*s,float delta,const dh2::ui::SwfSpriteServices16*v) noexcept {
 using namespace dh2::ui;
 if(!s||!v||reinterpret_cast<std::uintptr_t>(s)%alignof(SwfSpriteFrame64)||reinterpret_cast<std::uintptr_t>(v)%alignof(SwfSpriteServices16)||s->reserved||!s->sprite||!s->definition||s->current_frame<-32768||s->current_frame>32767||!actions(s->goto_actions)||!actions(s->scratch)||!separated(s,sizeof(*s),s->goto_actions.values,s->goto_actions.capacity*sizeof(std::uintptr_t))||!separated(s,sizeof(*s),s->scratch.values,s->scratch.capacity*sizeof(std::uintptr_t))||!separated(s->goto_actions.values,s->goto_actions.capacity*sizeof(std::uintptr_t),s->scratch.values,s->scratch.capacity*sizeof(std::uintptr_t)))return -1;
 try {
  int rc=0;
  if(!s->loaded){rc=sprite_call(s,v,SwfSpriteOp::construct);if(rc)return rc;rc=sprite_call(s,v,SwfSpriteOp::event,0,0,10);if(rc)return rc;}
  if(!s->visible&&s->loaded)return 0;
  s->need=s->goto_actions.count>0;rc=sprite_call(s,v,SwfSpriteOp::drag);if(rc)return rc;
  for(unsigned loop=0;s->goto_actions.count>0;++loop){
   if(!actions(s->goto_actions)||s->goto_actions.count>s->scratch.capacity)return -2;
   const unsigned count=s->goto_actions.count;
   for(unsigned i=0;i<count;++i)s->scratch.values[i]=s->goto_actions.values[i];
   s->scratch.count=count;s->goto_actions.count=0;
   rc=sprite_call(s,v,SwfSpriteOp::execute_actions,0,0,0,s->scratch.values,count);if(rc)return rc;
   if(loop==11){rc=sprite_call(s,v,SwfSpriteOp::warning);if(rc)return rc;s->scratch.count=0;break;}
   s->scratch.count=0;
  }
  if(static_cast<std::int8_t>(std::uint8_t(s->play_state))==0){
   const auto old_need=s->need;int count=0;rc=sprite_call(s,v,SwfSpriteOp::frame_count,0,0,0,nullptr,0,&count);if(rc)return rc;
   s->need=count>1?std::uint8_t(old_need|1):old_need;
   if(s->loaded){const auto previous=std::uint16_t(s->current_frame);s->current_frame=signed_frame(std::uint16_t(previous+1));
    rc=sprite_call(s,v,SwfSpriteOp::frame_count,0,0,0,nullptr,0,&count);if(rc)return rc;
    if(s->current_frame>=count)s->current_frame=0;
    if(std::uint16_t(s->current_frame)!=previous){
     if(s->current_frame==0){rc=sprite_call(s,v,SwfSpriteOp::frame_count,0,0,0,nullptr,0,&count);if(rc)return rc;if(count>1){rc=sprite_call(s,v,SwfSpriteOp::wrap_display_list);if(rc)return rc;}}
     rc=sprite_call(s,v,SwfSpriteOp::frame_tags,0,s->current_frame);if(rc)return rc;s->need=1;
    }
   }
  }
  if(s->enter){if(s->loaded){rc=sprite_call(s,v,SwfSpriteOp::event,0,0,12);if(rc)return rc;}s->need=1;}
  rc=sprite_call(s,v,SwfSpriteOp::do_actions);if(rc)return rc;
  int needed=0;rc=sprite_call(s,v,SwfSpriteOp::children_advance,delta,0,0,nullptr,0,&needed);if(rc)return rc;if(needed)s->need=1;
  s->loaded=1;return 0;
 }catch(...){return -2;}
}
extern "C" int dh2_ui_swf_root_frame(dh2::ui::SwfRootFrame32*s,float delta,std::uint32_t catch_up,const dh2::ui::SwfFrameServices16*v) noexcept {
 using namespace dh2::ui;
 if(!s||!v||reinterpret_cast<std::uintptr_t>(s)%alignof(SwfRootFrame32)||reinterpret_cast<std::uintptr_t>(v)%alignof(SwfFrameServices16)||s->pad[0]||s->pad[1]||s->pad[2]||!std::isfinite(s->frame_time)||s->frame_time<=0||(catch_up&&((std::isinf(s->remainder))||std::isinf(delta))))return -1;
 // A source catch-up loop cannot progress when overflow or float spacing
 // makes subtracting frame_time leave the remainder unchanged.
 const float initial_remainder=delta+s->remainder;
 if(catch_up&&(std::isinf(initial_remainder)||(initial_remainder>=s->frame_time&&initial_remainder-s->frame_time==initial_remainder)))return -1;
 try {
  int rc=call(s,v,SwfFrameOp::engine_mutex);if(rc)return rc;
  rc=call(s,v,SwfFrameOp::listeners_advance,delta);if(rc)return rc;
  s->remainder=delta+s->remainder;s->gc_remaining=s->gc_remaining-delta;
  if(!(s->remainder>=s->frame_time))return call(s,v,SwfFrameOp::engine_mutex);
  rc=call(s,v,SwfFrameOp::random);if(rc)return rc;
  if(!s->loaded){rc=call(s,v,SwfFrameOp::flash_vars,0,s->player);if(rc)return rc;}
  while(s->frame_time<=s->remainder){
   if(catch_up&&(!std::isfinite(s->frame_time)||s->frame_time<=0||std::isinf(s->remainder)||s->remainder-s->frame_time==s->remainder))return -2;
   if(!s->loaded){rc=call(s,v,SwfFrameOp::init_actions,0,s->movie);if(rc)return rc;rc=call(s,v,SwfFrameOp::construct,0,s->movie);if(rc)return rc;}
   rc=call(s,v,SwfFrameOp::movie_advance,catch_up?s->frame_time:delta,s->movie);if(rc)return rc;
   if(!s->loaded){s->loaded=1;rc=call(s,v,SwfFrameOp::event_load,0,s->movie);if(rc)return rc;}
   if(catch_up&&(!std::isfinite(s->frame_time)||s->frame_time<=0||std::isinf(s->remainder)||s->remainder-s->frame_time==s->remainder))return -2;
   s->remainder=s->remainder-s->frame_time;
   if(!catch_up)break;
  }
  if(s->gc_remaining<=0){
   rc=call(s,v,SwfFrameOp::mark_garbage,0,s->player);if(rc)return rc;
   rc=call(s,v,SwfFrameOp::listeners_alive);if(rc)return rc;
   rc=call(s,v,SwfFrameOp::movie_alive,0,s->movie);if(rc)return rc;
   rc=call(s,v,SwfFrameOp::clear_garbage,0,s->player);if(rc)return rc;
   s->gc_remaining=2.f;
  }
  // Actual ELF PLT 0x30e7f0 resolves fmodf; keep single-precision operands.
  s->remainder=std::fmod(s->remainder,s->frame_time);
  return call(s,v,SwfFrameOp::engine_mutex);
 }catch(...){return -2;}
}
