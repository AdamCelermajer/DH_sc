#include "hud_player_infos.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
using namespace dh2::ui;
namespace {
void check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
std::uint32_t u32(const unsigned char* p){std::uint32_t v;std::memcpy(&v,p,4);return v;}
std::int32_t i32(std::uint32_t v){std::int32_t i;std::memcpy(&i,&v,4);return i;}
struct Test {std::uint32_t raw[71]{};HudInfosActor32 actor{};std::vector<unsigned char> events;int fail=0;
 static int invoke(void* p,const HudInfosRequest32* r,HudInfosResponse16* out){auto& t=*static_cast<Test*>(p);HudInfosRequest32 record=*r;record.actor=r->operation==HudInfosOperation::player?0:t.raw[3]?1:0;record.output_object=1;const auto* bytes=reinterpret_cast<const unsigned char*>(&record);t.events.insert(t.events.end(),bytes,bytes+32);if(t.fail==int(r->operation))return 0;
  switch(r->operation){case HudInfosOperation::player:out->identity=t.raw[3]?reinterpret_cast<std::uintptr_t>(&t.actor):0;break;
  case HudInfosOperation::skill_slot:out->value=i32(t.raw[6+r->index]);break;
  case HudInfosOperation::skill_usable:case HudInfosOperation::skill_level:case HudInfosOperation::skill_info:{unsigned k=0;while(k<3&&t.raw[6+k]!=r->index)++k;check(k<3,"Uncaptured skill ID");if(r->operation==HudInfosOperation::skill_info)std::memcpy(&out->fraction,t.raw+15+k,4);else out->value=i32(t.raw[(r->operation==HudInfosOperation::skill_usable?9:12)+k]);break;}
  case HudInfosOperation::spell_info:std::memcpy(&out->fraction,t.raw+18,4);break;
  case HudInfosOperation::property_int:out->value=i32(t.raw[r->index==19?19:23]);break;
  case HudInfosOperation::low_health_constant:out->value=i32(t.raw[20]);break;
  case HudInfosOperation::spell_usable:out->value=i32(t.raw[21]);break;
  case HudInfosOperation::potions:out->value=i32(t.raw[22]);break;
  case HudInfosOperation::saved_dpad:out->value=i32(t.raw[24]);break;
  case HudInfosOperation::divide_zero:out->value=i32(t.raw[25]);break;
  case HudInfosOperation::write_member:if(t.raw[26]&(1u<<r->index))for(auto index:{33,36,41})t.raw[27+index]^=0x98765432;break;
  }return 1;
 }
 void setup(const unsigned char* p){std::memcpy(raw,p,sizeof(raw));actor={reinterpret_cast<std::int32_t*>(raw+27),44,static_cast<unsigned char>(raw[4]),static_cast<unsigned char>(raw[5]),0,1,0};events.clear();fail=0;}
 int run(){HudInfosInput24 in{1,i32(raw[1]),raw[2],raw[0],0};HudInfosServices16 s{this,invoke};return dh2_ui_hud_player_infos_v1(&in,&s);}
};
}
int main(int argc,char** argv){try{check(argc==2,"Expected original gold path");std::ifstream f(argv[1],std::ios::binary);std::vector<unsigned char> b((std::istreambuf_iterator<char>(f)),{});check(b.size()>8&&u32(b.data())==0x31494850,"Invalid gold header");const auto count=u32(b.data()+4);std::size_t at=8;unsigned calls=0;Test t;
 for(unsigned n=0;n<count;++n){check(at+12<=b.size(),"Truncated record");const auto input=u32(b.data()+at),expected=u32(b.data()+at+4),events=u32(b.data()+at+8);at+=12;check(input==284&&expected==176&&at+input+expected+events*32<=b.size(),"Invalid record bounds");t.setup(b.data()+at);at+=input;check(t.run()==0,"Producer failed gold");check(!std::memcmp(t.raw+27,b.data()+at,176),"Live cache mutation mismatch");at+=expected;check(t.events.size()==events*32&&!std::memcmp(t.events.data(),b.data()+at,t.events.size()),"Ordered source requests mismatch");at+=events*32;calls+=events;}
 check(at==b.size(),"Trailing gold bytes");unsigned guards=0;std::uint32_t base[71]{};base[0]=2;base[3]=base[4]=1;base[6]=base[7]=base[8]=0xffffffff;for(auto i:{34,38,43})base[27+i]=100;auto reset=[&](){t.setup(reinterpret_cast<unsigned char*>(base));};
 for(int op:{1,2,6,7,8,9,10,11,12}){reset();t.fail=op;check(t.run()==-2,"Missing service accepted");++guards;}
 for(int op:{3,4,5}){reset();t.raw[6]=7;t.fail=op;check(t.run()==-2,"Skill failure accepted");++guards;}
 reset();t.raw[27+38]=0;t.fail=13;check(t.run()==-2,"Zero division missing service accepted");++guards;
 reset();t.actor.resolved=nullptr;check(t.run()==-1,"Null active sheet accepted");++guards;
 reset();t.actor.count=43;check(t.run()==-1,"Short active sheet accepted");++guards;
 reset();t.actor.removed=1;t.actor.resolved=nullptr;check(t.run()==0&&t.events.size()==64,"Inactive source path overvalidated");++guards;
 reset();HudInfosInput24 bad{0,0,0,2,0};HudInfosServices16 s{&t,Test::invoke};check(dh2_ui_hud_player_infos_v1(&bad,&s)==-1,"Null object accepted");++guards;
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<count<<",\"ordered_services\":"<<calls<<",\"failure_guards\":"<<guards<<"}\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
