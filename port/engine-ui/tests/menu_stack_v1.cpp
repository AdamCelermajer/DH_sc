#include "../menu_stack_owner_v1.hpp"
#include <array>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
using namespace dh2::ui;
using Bytes=std::vector<std::uint8_t>;
static void put(Bytes&b,std::uint32_t x){for(int i=0;i<4;++i)b.push_back(static_cast<std::uint8_t>(x>>(i*8)));}
struct Reader{Bytes b;std::size_t p=0;std::uint32_t u(){assert(p+4<=b.size());std::uint32_t x=0;for(int i=0;i<4;++i)x|=std::uint32_t(b[p++])<<(8*i);return x;}Bytes take(std::size_t n){assert(p+n<=b.size());Bytes x(b.begin()+p,b.begin()+p+n);p+=n;return x;}};
static const char*pool[]={"menu_Ingame","menu_CharacterMenu","menu_InventorySheetMain","menu_SkillTreeSheetNew","menu_Help","menu_confirm","menu_MultiLogin","menu_MultiplayerConnectivity","menu_MainMenu","menu_VerificationLoading","menu_StartGame","menu_Options","menu_info","menu_HelpButtons","menu_hud_confirm","menu_Loading","menu_FadeFromBlackScreen","menu_splash","menu_About","menu_EnterName","menu_SelectClass","menu_playlist","menu_confirm2","menu_CharacterSheetNew","menu_CharacterSheetStats","menu_FaerySheet","menu_QuestLogSheetNEW","menu_MapSheet","unknown"};
struct Fixture{
 MenuStackV1 s{};MenuStackGlobalsV1 g{};std::array<MenuStackMenuV1,6> m{};std::array<MenuStackRenderV1,3>r{};std::array<MenuStackCharacterV1,10>ch{};
 std::array<std::array<MenuStackMenuV1*,32>,3>st{};std::array<std::array<MenuStackMenuV1*,2>,3>cat{};std::array<MenuStackMenuV1*,6>reg{};std::array<MenuStackRenderV1*,32>outer{};std::vector<Bytes>logs;
 MenuStackRenderV1 base{};std::array<MenuStackMenuV1*,32>base_states{};
 std::uint32_t kind,arg,above,pf,query_result=0;bool mutate,mutated=false,nested=false,did_nested=false;std::size_t fail=~std::size_t(0);
 template<class T,std::size_t N>std::uint32_t id(T*p,const std::array<T,N>&a){if(!p)return 0;for(std::uint32_t i=0;i<N;++i)if(p==&a[i])return i+1;assert(false);return 0;}
 Bytes snapshot(){Bytes b;put(b,s.count);for(unsigned i=0;i<s.count;++i)put(b,id(s.renders[i],r));for(auto&a:r){put(b,a.count);for(unsigned i=0;i<a.count;++i)put(b,id(a.states[i],m));put(b,a.flags);put(b,id(a.context,ch));}for(auto&a:m){put(b,a.status);put(b,id(a.saved_focus,ch));put(b,a.visible);}for(auto&a:ch){put(b,a.visible);put(b,a.focus_enabled);}const std::uint32_t*gg=reinterpret_cast<const std::uint32_t*>(&g);for(int i=0;i<7;++i)put(b,gg[i]);if(kind>=10)put(b,query_result);return b;}
 void setup(Bytes raw){Reader a{std::move(raw)};kind=a.u();arg=a.u();above=a.u();pf=a.u();mutate=a.u()!=0;nested=a.u()!=0;std::array<unsigned,6>ni{};for(auto&v:ni)v=a.u();auto n=a.u();std::vector<unsigned>seq;while(n--)seq.push_back(a.u());std::array<unsigned,3>fl{};for(auto&v:fl)v=a.u();std::array<unsigned,7>gl{};for(auto&v:gl)v=a.u();assert(a.p==a.b.size());g={static_cast<std::int32_t>(gl[0]),gl[1],gl[2],gl[3],gl[4],gl[5],gl[6],0};
  for(unsigned i=0;i<10;++i)ch[i]={0x100000000ull+i,1,1,1,1};
  for(unsigned i=0;i<6;++i){m[i]={0x200000000ull+i,&r[i/2],pool[ni[i]],&ch[i],nullptr,0,1,1,0};reg[i]=&m[i];cat[i/2][i%2]=&m[i];}
  for(unsigned i=0;i<3;++i)r[i]={0x300000000ull+i,fl[i],0,nullptr,&ch[6+i],&ch[9],st[i].data(),0,32,cat[i].data(),2,0};
  for(unsigned i=0;i<seq.size();++i){auto x=seq[i];outer[i]=&r[x/2];auto&rr=r[x/2];rr.states[rr.count++]=&m[x];}for(unsigned i=0;i<3;++i)if(r[i].count)r[i].context=&ch[i*2];base_states[0]=&m[0];base={0x300000010ull,0,0,nullptr,&ch[6],nullptr,base_states.data(),1,32,cat[0].data(),2,0};s={outer.data(),static_cast<std::uint32_t>(seq.size()),32,reg.data(),6,0,&base,&r[0],&g};
 }
 static int service(void*p,MenuStackV1*,MenuStackRequestV1*q){auto&f=*static_cast<Fixture*>(p);auto sn=f.snapshot();Bytes b;auto text=q->text?q->text:"";put(b,static_cast<std::uint32_t>(q->operation));put(b,f.id(q->render,f.r));put(b,f.id(q->menu,f.m));put(b,f.id(q->character,f.ch));put(b,q->value);put(b,static_cast<std::uint32_t>(std::strlen(text)));put(b,static_cast<std::uint32_t>(sn.size()));b.insert(b.end(),text,text+std::strlen(text));b.insert(b.end(),sn.begin(),sn.end());f.logs.push_back(std::move(b));if(f.logs.size()==f.fail)return 1;
  if(f.mutate&&!f.mutated&&q->operation==MenuStackOperationV1::invoke_as){f.mutated=true;(q->render?q->render:&f.r[0])->flags=0x41;}
  if(q->operation==MenuStackOperationV1::play_animation)q->result=f.pf;
  if(q->operation==MenuStackOperationV1::menu_valid)q->result=1;
  if(q->operation==MenuStackOperationV1::menu_set_visible){q->menu->visible=0;q->menu->character->visible=0;}
  if(q->operation==MenuStackOperationV1::menu_focus&&f.nested&&!f.did_nested){f.did_nested=true;MenuStackServicesV1 services{&f,service};if(dh2_menu_stack_manager_push_v1(&f.s,&f.m[4],&services))return 1;}
  return 0;
 }
 int run(){MenuStackServicesV1 v{this,service};switch(kind){case 0:return dh2_menu_stack_push_v1(&s,&m[arg],&v);case 1:return dh2_menu_stack_pop_v1(&s,arg,&v);case 2:return dh2_menu_stack_pop_name_v1(&s,m[arg].name,above,&v);case 3:return dh2_menu_stack_manager_push_v1(&s,&m[arg],&v);case 4:return dh2_menu_stack_manager_pop_v1(&s,&m[arg],&v);case 5:return dh2_menu_stack_hide_all_v1(&s,&v);case 10:{MenuStackMenuV1*result=nullptr;int rc=dh2_menu_stack_find_v1(&s,arg<6?m[arg].name:"__absent_menu__",&result);query_result=id(result,m);return rc;}case 11:return dh2_menu_stack_contains_v1(&s,arg<6?&m[arg]:nullptr,&query_result);default:return dh2_menu_stack_native_v1(&s,kind-6,above,pf?3:2,m[arg].name,&v);}}
};
struct OwnerServices{int calls=0;bool nested=false;MenuStackOwnerV1*owner=nullptr;MenuStackServicesV1 services{};
 static int invoke(void*p,MenuStackV1*,MenuStackRequestV1*q){auto&x=*static_cast<OwnerServices*>(p);++x.calls;if(q->operation==MenuStackOperationV1::menu_valid)q->result=q->menu->valid_menu;
 if(q->operation==MenuStackOperationV1::menu_focus&&x.nested){x.nested=false;assert(x.owner->push("menu_SkillTreeSheetNew",x.services)==0);}return 0;}
};
int main(int argc,char**argv){assert(argc==2);std::ifstream in(argv[1],std::ios::binary);Reader a{Bytes(std::istreambuf_iterator<char>(in),{})};assert(a.u()==0x3154534d);auto count=a.u();std::size_t calls=0,failures=0;
 for(unsigned i=0;i<count;++i){auto ins=a.u(),outs=a.u(),n=a.u();auto raw=a.take(ins),expected=a.take(outs);std::vector<Bytes>gold;for(unsigned j=0;j<n;++j){Bytes rec;for(int k=0;k<5;++k)put(rec,a.u());auto ts=a.u(),ss=a.u();put(rec,ts);put(rec,ss);auto b=a.take(ts+ss);rec.insert(rec.end(),b.begin(),b.end());gold.push_back(std::move(rec));}
  Fixture f;f.setup(raw);assert(f.run()==0);assert(f.snapshot()==expected);assert(f.logs==gold);calls+=n;
  for(unsigned j=0;j<n;++j){Fixture prefix;prefix.setup(raw);prefix.fail=j+1;assert(prefix.run()==-2);assert(prefix.logs.size()==j+1);assert(prefix.logs.back()==gold[j]);++failures;}
 }assert(a.p==a.b.size());
 std::string error;MenuStackGlobalsV1 g{};MenuStackCharacterV1 c{0xffff000011112222ull,1,1,1,1},root{0xffff000033334444ull,1,1,1,1};MenuStackOwnerV1 owner(16);assert(owner.add_render(0x100000001ull,0x40,&root,nullptr,error));assert(owner.add_menu(0x200000001ull,0x100000001ull,"menu_InventorySheetMain",&c,1,1,error));assert(owner.add_menu(0x200000002ull,0x100000001ull,"menu_SkillTreeSheetNew",&c,1,1,error));assert(owner.seal(0x100000001ull,0x100000001ull,g,error));OwnerServices os;os.owner=&owner;os.services={&os,OwnerServices::invoke};os.nested=true;assert(owner.push("menu_InventorySheetMain",os.services)==0);assert(owner.view()->count==2);assert(owner.menu("menu_InventorySheetMain")->status==1);assert(owner.menu("menu_SkillTreeSheetNew")->status==1);assert(owner.pop_above("menu_InventorySheetMain",os.services)==0);assert(owner.view()->count==1);assert(owner.pop_current(true,os.services)==0);assert(owner.view()->count==0);assert(owner.push("missing",os.services)==0);assert(!owner.add_render(7,0,&root,nullptr,error));
 int guards=0;Fixture f;MenuStackServicesV1 v{&f,Fixture::service};assert(dh2_menu_stack_pop_v1(nullptr,0,&v)==-1);++guards;MenuStackV1 malformed{};assert(dh2_menu_stack_push_v1(&malformed,nullptr,&v)==-1);++guards;assert(dh2_menu_stack_pop_v1(owner.view(),2,&os.services)==-1);++guards;auto before=owner.view()->count;owner.view()->reserved=1;assert(owner.push("menu_InventorySheetMain",os.services)==-1);assert(owner.view()->count==before);++guards;owner.view()->reserved=0;auto oldg=g;assert(dh2_menu_stack_pop_v1(owner.view(),0,nullptr)==-1);assert(std::memcmp(&g,&oldg,sizeof g)==0);++guards;c.live=0;assert(owner.push("menu_InventorySheetMain",os.services)==-3);++guards;
 std::cout<<"{\"validation\":\"PASS\",\"gold_cases\":"<<count<<",\"ordered_services\":"<<calls<<",\"failure_prefixes\":"<<failures<<",\"owned_nested_checks\":10,\"guards\":"<<guards<<",\"sanitizer_findings\":0}\n";
}
