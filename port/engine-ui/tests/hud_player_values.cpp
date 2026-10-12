#include "hud_player_values.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
using namespace dh2::ui;
namespace {
std::uint32_t word(const unsigned char* p){std::uint32_t v;std::memcpy(&v,p,4);return v;}
struct Context {std::uint32_t mask=31,types=31,mutation=0,zero=0;std::vector<HudValueRequest32> calls;int fail=0;bool nested=false;int nested_result=-99;HudValueServices16* services=nullptr;};
int invoke(void* ptr,HudValuesState24* s,const HudValueRequest32* r,HudValueResponse16* out){auto& c=*static_cast<Context*>(ptr);c.calls.push_back(*r);if(c.fail==int(r->operation))return 0;const unsigned index=unsigned(r->index);
 switch(r->operation){case HudValueOperation::resolve_clip:
  out->clip=(c.mask&(1u<<index))?index+1:0;
  if(c.mutation&(1u<<index)){++s->render_fx;const_cast<std::int32_t*>(s->resolved)[36]=987654321;}
  if(c.nested){c.nested=false;c.nested_result=dh2_ui_hud_goto_frame(s,HudValueClip::hurt,77,5,42,1,c.services);}break;
 case HudValueOperation::is_sprite:out->value=(c.types&(1u<<index))?1:0;break;
 case HudValueOperation::divide_zero:std::memcpy(&out->value,&c.zero,4);break;
 default:break;}
 return 1;
}
}
int main(int argc,char** argv){if(argc!=2)return 2;std::ifstream f(argv[1],std::ios::binary);std::vector<unsigned char> data((std::istreambuf_iterator<char>(f)),{});if(data.size()<8||word(data.data())!=0x31564448)return 2;const auto count=word(data.data()+4);std::size_t at=8;unsigned calls=0;
 for(unsigned i=0;i<count;++i){if(at+12>data.size())return 2;auto input=word(data.data()+at),output=word(data.data()+at+4),events=word(data.data()+at+8);at+=12;if(input!=216||output!=184||at+input+output+events*32>data.size())return 2;auto raw=data.data()+at;at+=input;auto expected=data.data()+at;at+=output;auto event_data=data.data()+at;at+=events*32;
  std::array<std::int32_t,44> sheet{};std::memcpy(sheet.data(),raw+4,176);Context c{};c.mask=word(raw+184);c.types=word(raw+188);c.mutation=word(raw+192);c.zero=word(raw+196);HudValuesState24 state{sheet.data(),44,0,word(raw+180)};HudValueServices16 svc{&c,invoke};c.services=&svc;
  int rc=word(raw)==0?dh2_ui_hud_player_values(&state,&svc):dh2_ui_hud_goto_frame(&state,HudValueClip(word(raw+208)),state.render_fx,word(raw+212)?word(raw+208)+1:0,std::int32_t(word(raw+200)),word(raw+204),&svc);
  std::array<unsigned char,184> actual{};std::memcpy(actual.data(),sheet.data(),176);std::memcpy(actual.data()+176,&state.render_fx,8);
  if(rc||std::memcmp(actual.data(),expected,184)||c.calls.size()!=events){std::cerr<<"HUD values record "<<i<<" mismatch\n";return 1;}
  for(unsigned k=0;k<events;++k)if(std::memcmp(&c.calls[k],event_data+k*32,32)){std::cerr<<"HUD values service "<<i<<'/'<<k<<" mismatch\n";return 1;}
  calls+=events;
 }
 if(at!=data.size())return 2;
 unsigned guards=0;auto check=[&](bool condition){if(!condition)throw guards;++guards;};
 try {
  std::array<std::int32_t,44> sheet{};sheet[36]=sheet[38]=sheet[41]=sheet[43]=sheet[33]=sheet[34]=100;Context c;HudValuesState24 state{sheet.data(),44,0,7};HudValueServices16 svc{&c,invoke};c.services=&svc;
  check(dh2_ui_hud_player_values(nullptr,&svc)==-1);
  state.count=43;check(dh2_ui_hud_player_values(&state,&svc)==-1&&c.calls.empty());state.count=44;
  state.reserved=1;check(dh2_ui_hud_player_values(&state,&svc)==-1&&c.calls.empty());state.reserved=0;
  alignas(8) unsigned char unaligned[sizeof(HudValuesState24)+8]{};check(dh2_ui_hud_player_values(reinterpret_cast<HudValuesState24*>(unaligned+1),&svc)==-1);
  state.resolved=reinterpret_cast<const std::int32_t*>(unaligned+1);check(dh2_ui_hud_player_values(&state,&svc)==-1);state.resolved=sheet.data();
  check(dh2_ui_hud_goto_frame(&state,HudValueClip(5),7,1,3,0,&svc)==-1);
  for(unsigned op=1;op<=4;++op){c.calls.clear();c.fail=int(op);check(dh2_ui_hud_player_values(&state,&svc)==-2&&unsigned(c.calls.back().operation)==op&&c.calls.size()==op);}
  c.calls.clear();c.fail=5;sheet[38]=0;check(dh2_ui_hud_player_values(&state,&svc)==-2&&c.calls.size()==1&&c.calls[0].operation==HudValueOperation::divide_zero);sheet[38]=100;
  c.calls.clear();c.fail=0;c.mutation=1;c.nested=true;check(dh2_ui_hud_player_values(&state,&svc)==0&&c.nested_result==0&&c.calls.size()==23&&state.render_fx==8);
  // Nested caller runs synchronously. Outer captured FX remains7 after lookup;
  // later clips re-read liveFX8. Source frame arithmetic was already cached.
  check(c.calls[1].render_fx==77&&c.calls[2].value==42&&c.calls[3].value==0&&c.calls[4].render_fx==7&&c.calls[5].value==99&&c.calls[7].render_fx==8);
  c.calls.clear();HudValueServices16 missing{};check(dh2_ui_hud_player_values(&state,&missing)==-2&&c.calls.empty());
  check(dh2_ui_hud_goto_frame(&state,HudValueClip::hp,0,0,-1,0,&missing)==0);
 }catch(unsigned n){std::cerr<<"HUD guard "<<n<<" mismatch\n";return 1;}
 std::cout<<"{\"comparisons\":"<<count<<",\"ordered_services\":"<<calls<<",\"failure_reentry_guards\":"<<guards<<",\"mismatches\":0}\n";
}
