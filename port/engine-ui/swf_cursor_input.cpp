#include "swf_cursor_input.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
namespace {
using namespace dh2::ui;
struct Missing {};
float mul(float a,float b){volatile float x=a*b;return x;}
float add(float a,float b){volatile float x=a+b;return x;}
float sub(float a,float b){volatile float x=a-b;return x;}
float finite(float a){return a>=-std::numeric_limits<float>::max()&&a<=std::numeric_limits<float>::max()?a:0.f;}
std::int32_t trunc32(float a){if(std::isnan(a))return 0;if(a>=2147483648.f)return INT32_MAX;if(a<=-2147483648.f)return INT32_MIN;return static_cast<std::int32_t>(a);}
// Preserve frozen SendEvent ABI word order. Its historical field labels do
// not denote the original cursor's local XY (source Event40 offsets12/16).
void local(SwfEvent48&e,const float xy[2]){std::memcpy(&e.value0,xy,sizeof(float));e.x=xy[1];}
struct Run {
 SwfInputState288& s;std::uint32_t& selection;const SwfInputServices16& services;int cleanup_error{};
 SwfInputResponse56 call(SwfInputOperation op,std::uintptr_t ch=0,const char*name=nullptr,SwfEvent48*event=nullptr,const float*values=nullptr,std::int32_t integer=0,std::uint32_t index=0){
  SwfInputRequest64 q{};q.operation=op;q.index=index;q.character=ch;q.name=name;q.event=event;q.integer=integer;
  if(values)std::memcpy(q.values,values,sizeof(q.values));SwfInputResponse56 r{};
  if(!services.invoke(services.context,&s,&q,&r))throw Missing{};return r;
 }
 const SwfInputCharacter32& view(std::uintptr_t ch,std::int32_t fields=1){auto r=call(SwfInputOperation::character,ch,nullptr,nullptr,nullptr,fields);if(!r.character||!r.character->name)throw Missing{};return *r.character;}
 void retain(std::uintptr_t ch){if(ch)call(SwfInputOperation::retain,ch);}
 void drop(std::uintptr_t ch){if(ch)call(SwfInputOperation::drop,ch);}
 void assign(std::uintptr_t&field,std::uintptr_t next){if(field==next)return;auto old=field;drop(old);field=next;retain(next);}
 bool mouse(std::uintptr_t ch){if(!ch)return false;const auto&name=view(ch);if(!std::strstr(name.name,"btn"))return view(ch,4).mouse9c!=0;const auto&v=view(ch,2);return v.is_sprite?v.sprite_ea!=0:true;}
 SwfEvent48 event(std::uint32_t kind,std::uintptr_t ch,std::uint32_t index){SwfEvent48 e{};e.character=ch;e.name=view(ch).name;e.kind=kind;e.cursor=static_cast<std::int32_t>(index);return e;}
 static int native(void*p,SwfEvent48*e){auto&r=*static_cast<Run*>(p);r.call(SwfInputOperation::native_event,r.s.native_receiver,nullptr,e);return 1;}
 static int method(void*p,std::uintptr_t ch,const char*n){static_cast<Run*>(p)->call(SwfInputOperation::as_method,ch,n);return 1;}
 void send(SwfEvent48&e){SwfEventServices24 v{this,native,method};if(dh2_ui_swf_send_event(&e,&selection,&v))throw Missing{};}
 bool accepts(SwfEvent48&e){return call(SwfInputOperation::can_handle_event,s.native_receiver,nullptr,&e).result!=0;}
 bool play(std::uintptr_t ch,const char*n){return call(SwfInputOperation::play_animation,ch,n).result!=0;}
 void position(std::uintptr_t ch,const float point[6],float xy[2]){auto r=call(SwfInputOperation::local_position,ch,nullptr,nullptr,point);xy[0]=r.values[0];xy[1]=r.values[1];}
 void focus(std::uintptr_t next,std::uint32_t i){auto&slot=s.slots[i];const auto old=slot.focus;if(old==next)return;
  if(!(s.flags&0x40)&&old){const auto&v=view(old,2);if(v.is_sprite&&v.sprite_ea){play(old,"focus_out");auto e=event(1,old,i);send(e);}}
  assign(slot.focus,next);
  if(!(s.flags&0x40)&&next){auto e=event(0,next,i);if(!accepts(e))assign(slot.focus,0);else{play(next,"focus_in");send(e);}}
 }
 void reset(std::uint32_t i){focus(0,i);assign(s.slots[i].pressed,0);}
};
struct Strong {
 Run&r;std::uintptr_t ch;
 Strong(Run&v,std::uintptr_t p):r(v),ch(p){r.retain(ch);}
 ~Strong(){try{r.drop(ch);}catch(...){r.cleanup_error=-2;}}
 Strong(const Strong&)=delete;Strong&operator=(const Strong&)=delete;
};
bool overlap(const void*a,std::size_t n,const void*b,std::size_t m){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<n:x-y<m;}
bool valid(SwfInputState288*s,std::uint32_t*g,const SwfInputServices16*v){return s&&g&&v&&v->invoke&&!overlap(s,sizeof(*s),g,sizeof(*g))&&!overlap(s,sizeof(*s),v,sizeof(*v))&&!overlap(g,sizeof(*g),v,sizeof(*v));}
template<class F>int execute(SwfInputState288*s,std::uint32_t*g,const SwfInputServices16*v,F f){if(!valid(s,g,v))return -1;Run r{*s,*g,*v};try{f(r);}catch(...){return -2;}return r.cleanup_error;}
void input(Run&r,std::int32_t mask,std::uint32_t i){auto&slot=r.s.slots[i];Strong old(r,slot.focus);if(!old.ch||!mask||slot.pending)return;
 auto e=r.event(3,old.ch,i);e.buttons=mask;r.send(e);if(e.consumed)return;
 auto m=r.call(SwfInputOperation::world_matrix,old.ch);const float wx=(mask&12)?1.f:10.f,wy=(mask&3)?1.f:10.f;
 auto list=r.call(SwfInputOperation::collect_buttons,r.s.context,"btn",nullptr,nullptr,3);
 if(list.count<0||(list.count&&!list.characters))throw Missing{};
 float best[4]{2147483648.f,2147483648.f,2147483648.f,2147483648.f};std::uintptr_t chosen[4]{};
 for(int j=0;j<list.count;++j){auto ch=list.characters[j];auto c=r.call(SwfInputOperation::world_matrix,ch);const float dx=mul(sub(c.values[2],m.values[2]),wx),dy=mul(sub(c.values[5],m.values[5]),wy),d=add(mul(dx,dx),mul(dy,dy));
  if(dy<0.f&&std::fabs(dy)>0.f&&d<best[0]){best[0]=d;chosen[0]=ch;}
  if(dy>0.f&&std::fabs(dy)>0.f&&d<best[1]){best[1]=d;chosen[1]=ch;}
  if(dx<0.f&&std::fabs(dx)>0.f&&d<best[2]){best[2]=d;chosen[2]=ch;}
  if(dx>0.f&&std::fabs(dx)>0.f&&d<best[3]){best[3]=d;chosen[3]=ch;}
 }
 for(unsigned j=0;j<4;++j)if((mask&(1u<<j))&&chosen[j]){r.focus(chosen[j],i);return;}
 if((mask&16)&&r.s.native_receiver&&!(r.s.flags&64)){r.play(old.ch,"clicked");r.assign(slot.pending,old.ch);}
}
void cursor(Run&r,const SwfCursor16&next,std::uint32_t i){auto&slot=r.s.slots[i];const auto old=slot.cursor;slot.cursor=next;
 float point[6]{next.x,next.y};r.call(SwfInputOperation::publish_raw_cursor,r.s.root,nullptr,nullptr,point,0,i);
 auto mapped=r.call(SwfInputOperation::screen_to_logical,r.s.root,nullptr,nullptr,point);point[0]=mapped.values[0];point[1]=mapped.values[1];
 if(slot.graphic){float m[6]{1.f,0.f,0.f,0.f,1.f,0.f};const float x=mul(point[0],20.f),y=mul(point[1],20.f);m[2]=finite(add(add(x,mul(y,0.f)),0.f));m[5]=finite(add(add(mul(x,0.f),y),0.f));
  const float c=std::cos(next.rotation),sn=std::sin(next.rotation);m[0]=finite(mul(1.f,c));m[1]=finite(mul(-1.f,sn));m[3]=finite(mul(1.f,sn));m[4]=finite(mul(1.f,c));r.call(SwfInputOperation::set_matrix,slot.graphic,nullptr,nullptr,m);
 }
 if(!slot.enabled||!r.s.context||((r.s.flags&32)&&slot.pending))return;
 float mouse[6]{static_cast<float>(trunc32(point[0])),static_cast<float>(trunc32(point[1]))};r.call(SwfInputOperation::notify_mouse_state,r.s.root,nullptr,nullptr,mouse,0,i);
 const bool down=next.buttons!=0;const bool down_edge=down&&old.buttons==0,up_edge=!down&&old.buttons!=0,moved=down&&!(next.x==old.x&&next.y==old.y);
 auto hit_root=(r.s.flags&4)?r.call(SwfInputOperation::root_movie,r.s.root).identity:r.s.context;
 Strong root(r,hit_root);float hit_point[6]{mul(point[0],20.f),mul(point[1],20.f)};
 Strong hit(r,hit_root?r.call(SwfInputOperation::topmost,hit_root,nullptr,nullptr,hit_point).identity:0);
 Strong old_focus(r,slot.focus);
 if(slot.pressed){if((r.s.flags&128)&&hit.ch&&moved)r.focus(hit.ch,i);}
 else if(down_edge||!(r.s.flags&16))r.focus(hit.ch,i);
 else if((r.s.flags&128)&&moved&&hit.ch)r.focus(hit.ch,i);
 if((down_edge||up_edge)&&slot.pending!=slot.focus)r.assign(slot.pending,0);
 if(slot.pressed&&!r.mouse(slot.pressed))r.assign(slot.pressed,0);
 if(old_focus.ch!=slot.focus){
  if(slot.hover&&r.mouse(slot.hover)){float xy[2];r.position(slot.hover,point,xy);auto e=r.event(9,slot.hover,i);local(e,xy);e.value1=next.buttons;if(r.accepts(e))r.send(e);}
  if(hit.ch&&r.mouse(hit.ch)){float xy[2];r.position(hit.ch,point,xy);auto e=r.event(8,hit.ch,i);local(e,xy);e.value1=next.buttons;if(r.accepts(e))r.send(e);}
 }
 if(moved&&slot.focus){
  if(slot.focus==slot.hover&&hit.ch!=slot.focus){if(r.mouse(slot.focus)){auto e=r.event(11,slot.focus,i);float xy[2];r.position(slot.focus,point,xy);local(e,xy);e.value1=next.buttons;if(r.accepts(e))r.send(e);}}
  // Source reloads focus/hover after the drag-out callback.
  if(slot.focus!=slot.hover&&hit.ch==slot.focus){if(r.mouse(hit.ch)){auto e=r.event(10,slot.focus,i);float xy[2];r.position(slot.focus,point,xy);local(e,xy);e.value1=next.buttons;if(r.accepts(e))r.send(e);}}
 }
 r.assign(slot.hover,hit.ch);
 Strong active(r,slot.focus);if(!active.ch||!r.mouse(active.ch))return;
 float xy[2];r.position(active.ch,point,xy);
 auto make=[&](std::uint32_t kind){auto e=r.event(kind,active.ch,i);local(e,xy);e.value1=next.buttons;return e;};
 if(down_edge){if(!(r.s.flags&1)&&!hit.ch){r.reset(i);return;}if(!(r.s.flags&64))r.play(active.ch,"pressed");auto e=make(4);r.send(e);r.assign(slot.pressed,active.ch);}
 else if(up_edge){if((r.s.flags&1)||hit.ch==active.ch){auto e=make(6);if(r.accepts(e)){
   if(r.s.flags&64){r.send(e);auto clicked=make(2);r.send(clicked);}
   else if(r.play(active.ch,"released")){r.send(e);r.assign(slot.pending,active.ch);}
   else{const bool playing=r.play(active.ch,"clicked");r.send(e);if(playing)r.assign(slot.pending,active.ch);else{auto clicked=make(2);r.send(clicked);}}
  }r.assign(slot.pressed,0);
 }else{auto e=make(7);r.send(e);r.reset(i);r.assign(slot.pressed,0);}}
 else if(moved){if(!(r.s.flags&64)){auto e=make(5);r.send(e);}r.assign(slot.pressed,active.ch);}
 else if(!(r.s.flags&1)&&!hit.ch&&!slot.pressed)r.reset(i);
}
}
extern "C" int dh2_ui_swf_set_focus(dh2::ui::SwfInputState288*s,std::uintptr_t ch,std::uint32_t i,std::uint32_t*g,const dh2::ui::SwfInputServices16*v){if(i>=4)return -1;return execute(s,g,v,[&](Run&r){r.focus(ch,i);});}
extern "C" int dh2_ui_swf_reset_focus(dh2::ui::SwfInputState288*s,std::uint32_t i,std::uint32_t*g,const dh2::ui::SwfInputServices16*v){if(i>=4)return -1;return execute(s,g,v,[&](Run&r){r.reset(i);});}
extern "C" int dh2_ui_swf_update_input(dh2::ui::SwfInputState288*s,std::int32_t mask,std::uint32_t i,std::uint32_t*g,const dh2::ui::SwfInputServices16*v){if(i>=4)return -1;return execute(s,g,v,[&](Run&r){input(r,mask,i);});}
extern "C" int dh2_ui_swf_update_cursor(dh2::ui::SwfInputState288*s,const dh2::ui::SwfCursor16*p,std::uint32_t i,std::uint32_t*g,const dh2::ui::SwfInputServices16*v){if(i>=4||!p||!s||overlap(s,sizeof(*s),p,sizeof(*p))||(g&&overlap(g,sizeof(*g),p,sizeof(*p)))||(v&&overlap(v,sizeof(*v),p,sizeof(*p))))return -1;return execute(s,g,v,[&](Run&r){cursor(r,*p,i);});}
extern "C" int dh2_ui_swf_update_pending(dh2::ui::SwfInputState288*s,std::int32_t ms,std::uint32_t advance_flag,std::uint32_t*g,const dh2::ui::SwfInputServices16*v){return execute(s,g,v,[&](Run&r){Strong root(r,r.s.root);float values[6]{static_cast<float>(ms)/1000.f};r.call(SwfInputOperation::advance,root.ch,nullptr,nullptr,values,static_cast<std::int32_t>(advance_flag));if(r.s.flags&64)return;for(std::uint32_t i=0;i<4;++i){auto&slot=r.s.slots[i];if(slot.pending&&r.call(SwfInputOperation::play_state,slot.pending).result==1){auto e=r.event(2,slot.pending,i);r.send(e);r.assign(slot.pending,0);}}});}
