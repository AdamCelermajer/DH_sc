#include "../hud_manager.hpp"
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include <stdexcept>
using namespace dh2::ui;
namespace {
void check(bool b,const char*s){if(!b)throw std::runtime_error(s);}
template<class T> void append(std::vector<unsigned char>&v,T n){auto*p=reinterpret_cast<const unsigned char*>(&n);v.insert(v.end(),p,p+sizeof n);}
void string(std::vector<unsigned char>&v,const std::string&s){append(v,std::uint32_t(s.size()));v.insert(v.end(),s.begin(),s.end());}
float floating(std::uint32_t v){float f;std::memcpy(&f,&v,4);return f;}
struct Reader {std::ifstream f;explicit Reader(const char*p):f(p,std::ios::binary){check(bool(f),"gold open");}template<class T>T get(){T v;f.read(reinterpret_cast<char*>(&v),sizeof v);check(bool(f),"gold truncated");return v;}std::vector<unsigned char> bytes(unsigned n){std::vector<unsigned char>v(n);f.read(reinterpret_cast<char*>(v.data()),n);check(bool(f),"gold bytes");return v;}};
struct Fixture {
 std::array<std::uint32_t,103> cfg{};HudManagerState state{};HudManagerActor actors[2]{};HudManagerPlayer players[4]{};std::int32_t sheet[44]{};std::uintptr_t skills[3]{},spells[1]{704};
 std::vector<std::vector<unsigned char>> calls;int fail_at=-1;
 void initialize(){for(unsigned i=0;i<44;++i)sheet[i]=cfg[58+i];for(unsigned i=0;i<3;++i)skills[i]=cfg[23+i]?701+i:0;
  for(unsigned i=0;i<2;++i){auto&a=actors[i];a.resolved=sheet;a.resolved_count=44;a.target=cfg[14]?actors+1:nullptr;a.skills=skills;a.skill_count=3;a.spells=spells;a.spell_count=1;a.name_symbol=88;a.debug_name="EnemyDebug";a.network_id=7;a.position[0]=1;a.position[1]=2;a.position[2]=3;a.identity=i+1;}
  for(unsigned i=0;i<4;++i)players[i]={cfg[26+i]?actors:nullptr,static_cast<std::int32_t>(cfg[30]),22,1,{0,0,0,0,0,0,0},"Ally",10+i};
  state={cfg[3]?actors+1:nullptr,cfg[1],static_cast<std::int32_t>(cfg[2]),cfg[4]};calls.clear();
 }
 std::uint64_t id(std::uintptr_t p){for(auto&a:actors)if(p==reinterpret_cast<std::uintptr_t>(&a))return a.identity;for(auto&x:players)if(p==reinterpret_cast<std::uintptr_t>(&x))return x.identity;return p;}
 std::vector<unsigned char> event(const HudManagerRequest&q){std::vector<unsigned char> v;append(v,std::uint32_t(q.operation));append(v,q.index);append(v,q.value);append(v,q.other);append(v,id(q.subject));append(v,std::uint64_t(q.render_fx));string(v,q.text?q.text:"");
  if(q.operation==HudManagerOperation::project_position){append(v,1u);for(float f:q.xyz)append(v,f);}
  else if(q.operation==HudManagerOperation::allies_callback){append(v,2u);auto&a=*static_cast<const HudManagerAllies*>(q.payload);append(v,std::uint32_t(a.present));string(v,a.name?a.name:"");append(v,a.level);append(v,a.hp_frame);append(v,a.index);append(v,a.death_seconds);}
  else append(v,0u);return v;
 }
 static int invoke(void*ctx,HudManagerState*s,const HudManagerRequest*q,HudManagerResponse*out){auto&f=*static_cast<Fixture*>(ctx);f.calls.push_back(f.event(*q));if(int(f.calls.size())-1==f.fail_at)return 0;auto&c=f.cfg;auto op=static_cast<unsigned>(q->operation);unsigned i=q->index;
  if(op==6&& (c[102]&(1u<<i)))++s->render_fx;
  switch(op){case 1:out->identity=c[5]?1:0;out->value=c[6];break;case 2:out->value=c[7];break;case 3:out->value=std::string(q->text)=="HUDStyle"?c[8]:c[9];break;case 4:out->identity=500;break;case 6:out->identity=(c[56]&(1u<<i))?100+i:0;break;case 7:out->identity=reinterpret_cast<std::uintptr_t>(f.players);break;case 8:out->value=c[12];break;case 12:out->value=c[11];break;case 13:out->value=c[31+i];break;case 14:out->value=c[34+i];break;case 15:out->value=c[37];break;case 16:out->fraction=floating(c[38+q->subject-701]);break;case 17:out->value=c[42+i];break;case 18:out->value=c[19];break;case 19:out->value=c[13];break;case 20:out->identity=c[15]?reinterpret_cast<std::uintptr_t>(f.actors+1):0;break;case 21:out->value=c[17];break;case 22:out->value=c[16];break;case 23:out->text="Enemy";break;case 24:out->value=c[18];break;case 25:out->value=c[21];break;case 27:out->value=c[22];break;case 28:out->fraction=floating(c[45+int(q->subject==reinterpret_cast<std::uintptr_t>(f.actors+1))]);break;case 29:out->value=c[20];break;case 30:check(i<4,"player fixture index");out->identity=reinterpret_cast<std::uintptr_t>(f.players+i);break;case 31:{auto*p=reinterpret_cast<HudManagerPlayer*>(q->subject);out->value=c[47+(p-f.players)];break;}case 32:out->text=q->text;break;case 33:out->xy[0]=c[51];out->xy[1]=c[52];break;case 34:case 35:out->fraction=floating(c[53+op-34]);break;case 37:out->identity=500;break;case 40:out->value=c[55];break;case 41:out->value=c[10];break;case 42:out->value=bool(c[57]&(1u<<i));break;default:break;}return 1;
 }
 int run(){HudManagerServices svc{this,invoke};return dh2_ui_hud_manager_v1(&state,cfg[0],&svc);}
};
}
int main(int argc,char**argv){try{check(argc==2,"gold argument");Reader r(argv[1]);check(r.get<unsigned>()==0x314d5548,"magic");auto count=r.get<unsigned>();unsigned calls=0,guards=0;for(unsigned n=0;n<count;++n){Fixture f;for(auto&v:f.cfg)v=r.get<unsigned>();std::array<unsigned,4> after;for(auto&v:after)v=r.get<unsigned>();auto num=r.get<unsigned>();std::vector<std::vector<unsigned char>> expected;for(unsigned i=0;i<num;++i)expected.push_back(r.bytes(r.get<unsigned>()));f.initialize();check(f.run()==0,"kernel returned failure");check(f.calls==expected,"ordered source calls mismatch");check(f.id(reinterpret_cast<std::uintptr_t>(f.state.cached_target))==after[0]&&f.state.initialized==after[1]&&static_cast<unsigned>(f.state.slow_ms)==after[2]&&f.state.render_fx==after[3],"after mismatch");calls+=num;
 if(n<100&&num){for(unsigned i=0;i<num;++i){f.initialize();f.fail_at=i;check(f.run()==-2,"required delivery failure");check(f.calls.size()==i+1&&std::equal(f.calls.begin(),f.calls.end(),expected.begin()),"failed prefix mismatch");++guards;}}
 }
 Fixture f;f.cfg[4]=1;f.cfg[26]=1;f.cfg[56]=(1u<<29)-1;f.cfg[57]=(1u<<29)-1;f.cfg[31]=f.cfg[32]=f.cfg[33]=UINT32_MAX;f.initialize();HudManagerServices s{&f,Fixture::invoke};check(dh2_ui_hud_manager_v1(nullptr,0,&s)==-1,"null guard");++guards;check(dh2_ui_hud_manager_v1(&f.state,5,&s)==-1,"entry guard");++guards;
 std::cout<<"{\"validation\":\"PASS\",\"cases\":"<<count<<",\"ordered_services\":"<<calls<<",\"failure_guards\":"<<guards<<"}\n";
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
