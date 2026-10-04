#include "hud_manager_backends.hpp"
#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>
#include <stdexcept>
using namespace dh2::ui;
namespace {
void check(bool b,const char*s){if(!b)throw std::runtime_error(s);}
std::uint32_t bits(float f){std::uint32_t v;std::memcpy(&v,&f,4);return v;}
float floating(std::uint32_t v){float f;std::memcpy(&f,&v,4);return f;}
bool same(std::uint32_t a,std::uint32_t b){return a==b||((a&0x7f800000)==0x7f800000&&(a&0x7fffff)&&(b&0x7f800000)==0x7f800000&&(b&0x7fffff));}
struct Fixture {
 std::vector<std::uint32_t> cfg;std::vector<std::vector<std::uint32_t>> calls;std::size_t failure=SIZE_MAX;
 static int invoke(void*p,HudSkillAI*,const HudSkillRequest*q,HudSkillResponse*r){auto&t=*static_cast<Fixture*>(p);t.calls.push_back({unsigned(q->operation),q->index,unsigned(q->level)});if(t.calls.size()-1==t.failure)return 0;auto&c=t.cfg;
 switch(q->operation){case HudSkillOperation::current_state:r->value=c[t.calls.size()==1?2:3];break;case HudSkillOperation::selected_spell:r->value=c[4];break;case HudSkillOperation::script_usable:r->value=c[6];break;case HudSkillOperation::script_info:r->fraction=floating(c[7]);break;}return 1;}
 int query(std::uint32_t&out){HudSkillOwner owner{1,cfg[8],0};std::uintptr_t rows[4]{cfg[9]?7u:0u,cfg[9]?7u:0u,cfg[9]?7u:0u,cfg[9]?7u:0u};HudSkillAI ai{&owner,std::int32_t(cfg[5]),0,rows,4,0,rows,4,0};HudSkillResponse response{std::int32_t(0x778899aa),floating(0x7fc12345)};HudSkillServices services{this,invoke};int rc=dh2_ui_hud_skill_query(&ai,cfg[0],cfg[1],123,&response,&services);out=cfg[0]<2?unsigned(response.value):bits(response.fraction);return rc;}
};
std::uint32_t read(std::ifstream&f){std::uint32_t v=0;f.read(reinterpret_cast<char*>(&v),4);check(bool(f),"Truncated backend gold");return v;}
}
int main(int argc,char**argv){try{if(argc!=2)return 2;std::ifstream f(argv[1],std::ios::binary);check(read(f)==0x31425548,"Backend gold magic");auto count=read(f);std::size_t services=0,failures=0,guards=0;
 for(unsigned n=0;n<count;++n){auto kind=read(f),length=read(f);std::vector<unsigned>cfg(length);for(auto&v:cfg)v=read(f);auto expected=read(f),nc=read(f);std::vector<std::vector<unsigned>>calls(nc,std::vector<unsigned>(3));for(auto&c:calls)for(auto&v:c)v=read(f);unsigned out=0;int rc=0;
  if(kind==0){Fixture t;t.cfg=cfg;rc=t.query(out);check(t.calls==calls,"Backend callback order");services+=calls.size();for(unsigned i=0;i<nc;++i){Fixture fail;fail.cfg=cfg;fail.failure=i;unsigned ignored;check(fail.query(ignored)==-2,"Required script provider failure swallowed");check(fail.calls==std::vector<std::vector<unsigned>>(calls.begin(),calls.begin()+i+1),"Required failure prefix mismatch");++failures;}}
  else if(kind==1){std::int16_t value=cfg[1];std::int32_t v;rc=dh2_ui_hud_num_potions(cfg[0]?&value:nullptr,&v);out=v;}
  else if(kind==2){HudSkillLevel8 rows[8];for(auto&v:rows)v={0,std::uint16_t(cfg[2]),0};HudSavedSkillLevels saved{cfg[1]?rows:nullptr,8,0};std::int32_t v;rc=dh2_ui_hud_skill_level(&saved,cfg[0],&v);out=v;}
  else if(kind==3){HudSkillSlotNode nodes[7]{};for(int i=0;i<7;++i){nodes[i].key=cfg[1+i];nodes[i].value=cfg[8+i];}nodes[3].left=&nodes[1];nodes[3].right=&nodes[5];nodes[1].left=&nodes[0];nodes[1].right=&nodes[2];nodes[5].left=&nodes[4];nodes[5].right=&nodes[6];HudSkillSlotTree tree{&nodes[3],7,0};std::int32_t v;rc=dh2_ui_hud_skill_slot(&tree,cfg[0],&v);out=v;}
  else {dh2::character::Timer32 timer{};timer.elapsed_ms=cfg[1];timer.duration_ms=cfg[2];timer.active=cfg[3];dh2::character::TimerStore32 store{&timer,1,1,1,0,0};HudCooldown cd{&store,std::int32_t(cfg[0]),0};float v;rc=dh2_ui_hud_cooldown(&cd,&v);out=bits(v);}
  check(rc==0,"Backend native replay rejected");check(kind==4?same(out,expected):out==expected,"Backend replay value mismatch");
 }
 std::int32_t out=123;HudSkillSlotNode loop{};loop.left=&loop;HudSkillSlotTree tree{&loop,2,0};check(dh2_ui_hud_skill_slot(&tree,0,&out)==-1&&out==123,"Cyclic tree not atomic");++guards;
 HudSkillLevel8 row{};HudSavedSkillLevels levels{&row,1,0};check(dh2_ui_hud_skill_level(&levels,UINT32_MAX,&out)==-1&&out==123,"Unsigned level bounds not atomic");++guards;
 check(dh2_ui_hud_skill_slot(nullptr,3,&out)==0&&out==-1,"Missing savegame slot");++guards;check(dh2_ui_hud_skill_level(nullptr,3,&out)==0&&out==-1,"Missing savegame level");++guards;
 alignas(8) unsigned char raw[64]{};check(dh2_ui_hud_skill_slot(reinterpret_cast<HudSkillSlotTree*>(raw+1),0,&out)==-1,"Unaligned tree accepted");++guards;
 check(dh2_ui_hud_num_potions(nullptr,nullptr)==-1,"Missing potion output accepted");++guards;
 HudSkillOwner owner{1,0,0};std::uintptr_t script=7;HudSkillAI ai{&owner,8,0,&script,1,0,&script,1,0};HudSkillResponse retained{42,floating(0x7fc12345)};HudSkillServices no_write{nullptr,[](void*,HudSkillAI*,const HudSkillRequest*q,HudSkillResponse*){return q->operation==HudSkillOperation::script_info?1:0;}};
 check(dh2_ui_hud_skill_query(&ai,2,0,123,&retained,&no_write)==0&&bits(retained.fraction)==0x7fc12345,"Script info no-write prefix changed");++guards;
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<count<<",\"ordered_services\":"<<services<<",\"required_failure_prefixes\":"<<failures<<",\"atomic_guards\":"<<guards<<"}\n";return 0;
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
