#include "swf_frame_schedule.hpp"
#include "swf_drag_values.hpp"
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
using namespace dh2::ui;using Bytes=std::vector<unsigned char>;
namespace {
void append(Bytes&b,const void*p,unsigned n){auto*r=static_cast<const unsigned char*>(p);b.insert(b.end(),r,r+n);}void word(Bytes&b,unsigned v){append(b,&v,4);}
struct Reader{Bytes b;std::size_t at{};Bytes take(unsigned n){if(n>b.size()-at)throw std::runtime_error("Truncated original gold");Bytes v(b.begin()+at,b.begin()+at+n);at+=n;return v;}template<class T>T value(){auto v=take(sizeof(T));T x;std::memcpy(&x,v.data(),sizeof(T));return x;}};
bool floats_equal(const Bytes&a,const Bytes&b,unsigned n){if(a.size()!=b.size())return false;for(unsigned i=0;i<n;i+=4){float x,y;std::memcpy(&x,a.data()+i,4);std::memcpy(&y,b.data()+i,4);if(std::memcmp(a.data()+i,b.data()+i,4)&&!(std::isnan(x)&&std::isnan(y)))return false;}return std::equal(a.begin()+n,a.end(),b.begin()+n);}
struct Fixture{
 unsigned mode{},mutation{},frames{},requeue{},children{},executions{},fail_op{};std::uintptr_t go[32]{},scratch[32]{};SwfRootFrame32 root{};SwfSpriteFrame64 sprite{};std::vector<Bytes>trace;
 void note(unsigned op,unsigned v=0,const void*extra=nullptr,unsigned size=0){Bytes b;word(b,op);word(b,v);if(size)append(b,extra,size);trace.push_back(b);}
 static int root_call(void*p,SwfRootFrame32*s,const SwfFrameRequest24*q){auto&f=*static_cast<Fixture*>(p);auto op=unsigned(q->operation);f.note(op,0,&q->delta,(op==2||op==7)?4:0);if(op==f.fail_op)return 0;if(f.mutation==1&&op==2)s->loaded=1;if(f.mutation==2&&op==7)s->loaded=0;if(f.mutation==3&&op==8)s->frame_time=.2f;if(f.mutation==4&&op==7)s->frame_time=0;return 1;}
 static int sprite_call(void*p,SwfSpriteFrame64*s,const SwfSpriteRequest32*q,int*out){auto&f=*static_cast<Fixture*>(p);auto op=unsigned(q->operation);
  if(op==2){f.note(op,q->value);if(f.mutation==1&&q->value==10){s->loaded=1;s->visible=0;}if(f.mutation==2&&q->value==12)s->enter=0;}
  else if(op==4){Bytes b;word(b,op);word(b,q->count);for(unsigned i=0;i<q->count;++i)word(b,q->actions[i]);f.trace.push_back(b);if(++f.executions<f.requeue){s->goto_actions.values[0]=700+f.executions;s->goto_actions.count=1;}}
  else if(op==5){f.note(op);*out=f.frames;}
  else if(op==7)f.note(op,q->frame);
  else if(op==9){f.note(op,0,&q->delta,4);*out=f.children;}
  else f.note(op);return op!=f.fail_op;
 }
};
}
int main(int argc,char**argv){if(argc!=2)return 2;try{std::ifstream file(argv[1],std::ios::binary);Reader r{Bytes(std::istreambuf_iterator<char>(file),{})};if(r.value<unsigned>()!=0x31474653)return 2;unsigned count=r.value<unsigned>(),drag=r.value<unsigned>(),services=0;
 for(unsigned k=0;k<count;++k){Fixture f;f.mode=r.value<unsigned>();f.mutation=r.value<unsigned>();float dt=0;unsigned catchup=0;
  if(!f.mode){f.root.remainder=r.value<float>();f.root.frame_time=r.value<float>();f.root.gc_remaining=r.value<float>();f.root.loaded=r.value<unsigned>();dt=r.value<float>();catchup=r.value<unsigned>();f.root.movie=1;f.root.player=2;}
  else{f.sprite.sprite=1;f.sprite.definition=2;f.sprite.current_frame=r.value<int>();f.sprite.play_state=r.value<int>();f.sprite.loaded=r.value<int>();f.sprite.visible=r.value<int>();f.sprite.need=r.value<int>();f.sprite.enter=r.value<int>();f.frames=r.value<unsigned>();f.requeue=r.value<unsigned>();f.children=r.value<unsigned>();dt=r.value<float>();auto n=r.value<unsigned>();if(n>32)return 2;for(unsigned i=0;i<n;++i)f.go[i]=r.value<unsigned>();f.sprite.goto_actions={f.go,n,32};f.sprite.scratch={f.scratch,0,32};}
  Bytes expected=r.take(r.value<unsigned>());unsigned n=r.value<unsigned>();std::vector<Bytes>trace;for(unsigned i=0;i<n;++i)trace.push_back(r.take(r.value<unsigned>()));Bytes actual;int rc;
  if(!f.mode){SwfFrameServices16 svc{&f,Fixture::root_call};rc=dh2_ui_swf_root_frame(&f.root,dt,catchup,&svc);append(actual,&f.root,13);}
  else{SwfSpriteServices16 svc{&f,Fixture::sprite_call};rc=dh2_ui_swf_sprite_frame(&f.sprite,dt,&svc);for(unsigned v:{unsigned(f.sprite.current_frame),unsigned(f.sprite.play_state),unsigned(f.sprite.loaded),unsigned(f.sprite.visible),unsigned(f.sprite.need),unsigned(f.sprite.enter),f.sprite.goto_actions.count})word(actual,v);for(unsigned i=0;i<f.sprite.goto_actions.count;++i)word(actual,f.go[i]);}
  if(rc||!(f.mode?actual==expected:floats_equal(expected,actual,12))||trace!=f.trace)throw std::runtime_error("Frame original replay mismatch "+std::to_string(k));services+=n;
 }
 for(unsigned k=0;k<drag;++k){auto raw=r.take(108);SwfDragValues108 input;std::memcpy(&input,raw.data(),108);auto expected=r.take(44);SwfDragResult44 output{};if(dh2_ui_swf_drag_values(&output,&input))throw std::runtime_error("Drag source caller rejected");Bytes actual;append(actual,&output,44);if(!floats_equal(expected,actual,40))throw std::runtime_error("Drag original replay mismatch "+std::to_string(k));}
 if(r.at!=r.b.size())return 2;
 SwfRootFrame32 bad{};SwfFrameServices16 empty{};if(dh2_ui_swf_root_frame(&bad,0,0,&empty)!=-1)return 1;
 SwfSpriteFrame64 invalid{};SwfSpriteServices16 absent{};if(dh2_ui_swf_sprite_frame(&invalid,0,&absent)!=-1)return 1;
 unsigned guards=2;Fixture atomic;atomic.root={3.4e38f,.1f,0,0,{0,0,0},1,2};SwfFrameServices16 root_services{&atomic,Fixture::root_call};auto before=atomic.root;if(dh2_ui_swf_root_frame(&atomic.root,3.4e38f,1,&root_services)!=-1||!atomic.trace.empty()||std::memcmp(&before,&atomic.root,sizeof(before)))return 1;++guards;
 for(unsigned op=1;op<=12;++op){Fixture f;f.fail_op=op;f.root={1,.1f,0,0,{0,0,0},1,2};SwfFrameServices16 svc{&f,Fixture::root_call};if(dh2_ui_swf_root_frame(&f.root,0,0,&svc)!=-2||f.trace.empty())return 1;unsigned last;std::memcpy(&last,f.trace.back().data(),4);if(last!=op)return 1;++guards;}
 Fixture live;live.mutation=4;live.root={1,.1f,0,0,{0,0,0},1,2};SwfFrameServices16 live_services{&live,Fixture::root_call};if(dh2_ui_swf_root_frame(&live.root,0,1,&live_services)!=-2||live.root.frame_time!=0||live.trace.empty())return 1;++guards;
 for(unsigned op=1;op<=10;++op){Fixture f;f.fail_op=op;f.frames=2;f.requeue=13;f.sprite={1,2,1,0,static_cast<unsigned char>(op==6||op==7),1,1,1,0,{f.go,1,32},{f.scratch,0,32}};f.go[0]=100;SwfSpriteServices16 svc{&f,Fixture::sprite_call};if(dh2_ui_swf_sprite_frame(&f.sprite,0,&svc)!=-2||f.trace.empty())return 1;unsigned last;std::memcpy(&last,f.trace.back().data(),4);if(last!=op)return 1;++guards;}
 std::cout<<"{\"validation\":\"PASS\",\"original_frame_gold_comparisons\":"<<count<<",\"original_drag_gold_comparisons\":"<<drag<<",\"ordered_services\":"<<services<<",\"atomic_and_provider_prefix_guards\":"<<guards<<",\"mismatches\":0}\n";
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
