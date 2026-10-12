#include "hud_sprite_timeline.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
using namespace dh2::ui;
namespace {
std::uint32_t word(const unsigned char* p){std::uint32_t v;std::memcpy(&v,p,4);return v;}
std::array<unsigned char,148> snapshot(const HudSpriteState64& s){std::array<unsigned char,148> raw{};std::uint32_t head[5]={std::uint32_t(s.current_frame),std::uint32_t(s.play_state),std::uint32_t(s.stream_sound_id),s.pending.count,s.goto_actions.count};std::memcpy(raw.data(),head,20);for(unsigned i=0;i<16;++i){auto a=i<s.pending.count?std::uint32_t(s.pending.values[i]):0,b=i<s.goto_actions.count?std::uint32_t(s.goto_actions.values[i]):0;std::memcpy(raw.data()+20+i*4,&a,4);std::memcpy(raw.data()+84+i*4,&b,4);}return raw;}
struct Context {int frame_count=103,flags=0,has_sound=0,fail=0;bool nested=false;HudSpriteServices16* services=nullptr;std::vector<std::array<unsigned char,180>> events;};
int invoke(void* ptr,HudSpriteState64* s,const HudSpriteRequest32* r,HudSpriteResponse16* out){auto& c=*static_cast<Context*>(ptr);std::array<unsigned char,180> event{};std::memcpy(event.data(),r,32);auto snap=snapshot(*s);std::memcpy(event.data()+32,snap.data(),148);c.events.push_back(event);if(c.fail==int(r->operation))return 0;
 if(r->operation==HudSpriteOperation::frame_count){out->value=c.frame_count;if(c.flags&16){s->current_frame=-1;s->play_state=2;}}
 else if(r->operation==HudSpriteOperation::sound_handler)out->identity=c.has_sound?2:0;
 else if(r->operation==HudSpriteOperation::forward_tags||r->operation==HudSpriteOperation::reverse_tags){const unsigned flag=r->operation==HudSpriteOperation::reverse_tags?4:r->state_only?1:2;
  if(c.flags&flag&&s->pending.count<8){s->pending.values[s->pending.count++]=0x10000u+(std::uint32_t(r->frame)&65535u);}
  if(c.flags&8){s->current_frame=-2;s->play_state=0;}
  if(r->operation==HudSpriteOperation::forward_tags&&(c.flags&32)&&!c.nested){c.nested=true;const int result=dh2_ui_hud_sprite_goto_v1(s,1,c.services);if(result<0)return 0;}
 }
 return 1;
}
}
int main(int argc,char** argv){if(argc!=2)return 2;std::ifstream f(argv[1],std::ios::binary);std::vector<unsigned char> bytes((std::istreambuf_iterator<char>(f)),{});if(bytes.size()<8||word(bytes.data())!=0x31545348)return 2;const auto count=word(bytes.data()+4);std::size_t at=8;unsigned calls=0,nested=0;
 for(unsigned i=0;i<count;++i){if(at+12>bytes.size())return 2;auto input=word(bytes.data()+at),output=word(bytes.data()+at+4),events=word(bytes.data()+at+8);at+=12;if(input!=168||output!=152||at+input+output+events*180>bytes.size())return 2;auto raw=bytes.data()+at;at+=input;auto expected=bytes.data()+at;at+=output;auto event_data=bytes.data()+at;at+=events*180;
  std::array<std::uintptr_t,16> pending{},queued{};for(unsigned k=0;k<16;++k){pending[k]=word(raw+40+k*4);queued[k]=word(raw+104+k*4);}HudSpriteState64 s{std::int32_t(word(raw+4)),std::int32_t(word(raw+8)),std::int32_t(word(raw+12)),0,1,3,{pending.data(),word(raw+32),16},{queued.data(),word(raw+36),16}};Context c;c.frame_count=int(word(raw+20));c.has_sound=int(word(raw+24));c.flags=int(word(raw+28));HudSpriteServices16 svc{&c,invoke};c.services=&svc;const int rc=word(raw)==0?dh2_ui_hud_sprite_goto_v1(&s,std::int32_t(word(raw+16)),&svc):dh2_ui_hud_sprite_play_v1(&s,std::int32_t(word(raw+16)),&svc);auto actual=snapshot(s);
  if(std::uint32_t(rc)!=word(expected)||std::memcmp(actual.data(),expected+4,148)||c.events.size()!=events){std::cerr<<"Timeline record "<<i<<" mismatch\n";return 1;}
  for(unsigned k=0;k<events;++k)if(std::memcmp(c.events[k].data(),event_data+k*180,180)){std::cerr<<"Timeline service "<<i<<'/'<<k<<" mismatch\n";return 1;}
  calls+=events;nested+=c.nested;
 }
 if(at!=bytes.size())return 2;unsigned guards=0;auto check=[&](bool ok){if(!ok)throw guards;++guards;};
 try{
  std::array<std::uintptr_t,16> pending{},queued{};pending[0]=100;pending[1]=101;HudSpriteState64 s{0,0,7,0,1,3,{pending.data(),2,16},{queued.data(),3,16}};Context c;HudSpriteServices16 svc{&c,invoke};c.services=&svc;
  check(dh2_ui_hud_sprite_goto_v1(nullptr,4,&svc)==-1);s.reserved=1;check(dh2_ui_hud_sprite_goto_v1(&s,4,&svc)==-1&&c.events.empty());s.reserved=0;
  s.goto_actions.values=pending.data();check(dh2_ui_hud_sprite_goto_v1(&s,4,&svc)==-1);s.goto_actions.values=queued.data();
  s.pending.count=17;check(dh2_ui_hud_sprite_goto_v1(&s,4,&svc)==-1);s.pending.count=2;
  alignas(8) unsigned char storage[80]{};check(dh2_ui_hud_sprite_goto_v1(reinterpret_cast<HudSpriteState64*>(storage+1),4,&svc)==-1);
  c.fail=1;check(dh2_ui_hud_sprite_goto_v1(&s,4,&svc)==-2&&s.play_state==0&&s.goto_actions.count==3);c.fail=0;c.events.clear();s.goto_actions.capacity=1;check(dh2_ui_hud_sprite_goto_v1(&s,4,&svc)==-1&&s.pending.count==2&&s.goto_actions.count==3);s.goto_actions.capacity=16;
  // The preceding capacity1/count3 projection is malformed, not a source
  // allocation failure. Re-run capacity failure with valid count0.
  s.goto_actions.count=0;s.goto_actions.capacity=1;check(dh2_ui_hud_sprite_goto_v1(&s,4,&svc)==-2&&s.pending.count==2);s.goto_actions.capacity=16;
  c.fail=3;check(dh2_ui_hud_sprite_goto_v1(&s,4,&svc)==-2&&s.pending.count==0&&s.goto_actions.count==2&&s.current_frame==0&&s.play_state==0);c.fail=0;s.current_frame=4;s.pending.count=2;
  c.fail=2;check(dh2_ui_hud_sprite_goto_v1(&s,0,&svc)==-2&&s.goto_actions.count==2&&s.current_frame==4);c.fail=0;
  c.fail=4;s.current_frame=0;check(dh2_ui_hud_sprite_goto_v1(&s,1,&svc)==-2&&s.current_frame==1&&s.play_state==1);c.fail=0;
  c.fail=5;s.play_state=0;check(dh2_ui_hud_sprite_play_v1(&s,1,&svc)==-2&&s.play_state==0);c.fail=6;c.has_sound=1;check(dh2_ui_hud_sprite_play_v1(&s,1,&svc)==-2&&s.play_state==0);c.fail=0;
  c.fail=4;check(dh2_ui_hud_sprite_play_v1(&s,1,&svc)==-2&&s.play_state==1);c.fail=0;
  c.events.clear();HudSpriteServices16 missing{};check(dh2_ui_hud_sprite_play_v1(&s,0,&missing)==-2);
  c.frame_count=70000;check(dh2_ui_hud_sprite_goto_v1(&s,2,&svc)==-2);
 }catch(unsigned n){std::cerr<<"Timeline guard "<<n<<" mismatch\n";return 1;}
 std::cout<<"{\"comparisons\":"<<count<<",\"ordered_services\":"<<calls<<",\"actual_nested_gold_cases\":"<<nested<<",\"failure_capacity_guards\":"<<guards<<",\"mismatches\":0}\n";
}
