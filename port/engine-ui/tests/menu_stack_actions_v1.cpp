#include "menu_stack_actions_fixture_v1.hpp"
#include "../menu_stack_actions_v1.hpp"
#include "../character_menu_queries_owner_v1.hpp"
#include <limits>
struct ActionFixture {
 Fixture f;unsigned op,count,tag,mode;bool nested,done=false;
 MenuStackActionValueV1 value{0xf123456789abcdefull,0,0};
 int boundary(unsigned operation){
  MenuStackRequestV1 q{static_cast<MenuStackOperationV1>(operation),operation==21?tag:0,nullptr,nullptr,nullptr,nullptr,0};
  if(Fixture::service(&f,&f.s,&q))return 1;
  if(mode==operation-19)f.r[0].flags=0x41;
  if(nested&&!done){done=true;MenuStackServicesV1 life{&f,Fixture::service};if(dh2_menu_stack_manager_push_v1(&f.s,&f.m[4],&life))return 1;}
  return 0;
 }
 static int instance(void*p,MenuStackV1**out){auto&a=*static_cast<ActionFixture*>(p);if(a.boundary(20))return 1;*out=&a.f.s;return 0;}
 static int text(void*p,const MenuStackActionValueV1*v,const char**out){auto&a=*static_cast<ActionFixture*>(p);assert(v->identity==a.value.identity&&v->source_type==a.tag);if(a.boundary(21))return 1;*out=a.f.m[a.f.arg].name;return 0;}
 void setup(const Bytes&raw){Reader r{raw};op=r.u();count=r.u();tag=r.u();mode=r.u();nested=r.u()!=0;value.source_type=tag;f.setup(r.take(r.b.size()-r.p));}
 int run(){MenuStackActionCallV1 call{&value,count,0};MenuStackActionServicesV1 service{this,instance,text};MenuStackServicesV1 life{&f,Fixture::service};return dh2_menu_stack_action_v1(op,&call,&service,&life);}
};
int main(int argc,char**argv){
 assert(argc==2);std::ifstream in(argv[1],std::ios::binary);Reader a{Bytes(std::istreambuf_iterator<char>(in),{})};assert(a.u()==0x3141534d);const auto cases=a.u();std::size_t services=0,prefixes=0;
 for(unsigned i=0;i<cases;++i){const auto ins=a.u(),outs=a.u(),n=a.u();const auto raw=a.take(ins),expected=a.take(outs);std::vector<Bytes>gold;
  for(unsigned j=0;j<n;++j){Bytes record;for(int k=0;k<5;++k)put(record,a.u());const auto ts=a.u(),ss=a.u();put(record,ts);put(record,ss);const auto payload=a.take(ts+ss);record.insert(record.end(),payload.begin(),payload.end());gold.push_back(std::move(record));}
  ActionFixture f;f.setup(raw);assert(f.run()==0);assert(f.f.snapshot()==expected);assert(f.f.logs==gold);services+=n;
  for(unsigned j=0;j<n;++j){ActionFixture failed;failed.setup(raw);failed.f.fail=j+1;assert(failed.run()==-2);assert(failed.f.logs.size()==j+1);assert(failed.f.logs.back()==gold[j]);++prefixes;}
 }
 assert(a.p==a.b.size());
 std::string error;MenuStackGlobalsV1 globals{};MenuStackCharacterV1 character{0xf123456789ull,1,1,1,1};MenuStackOwnerV1 owner(16);
 assert(owner.add_render(0x100000001ull,0x40,&character,nullptr,error));assert(owner.add_menu(0x200000001ull,0x100000001ull,"menu_InventorySheetMain",&character,1,1,error));assert(owner.add_menu(0x200000002ull,0x100000001ull,"menu_SkillTreeSheetNew",&character,1,1,error));assert(owner.seal(0x100000001ull,0x100000001ull,globals,error));
 OwnerServices os;os.owner=&owner;os.services={&os,OwnerServices::invoke};unsigned instance_calls=0,conversions=0;auto lifetime=std::make_shared<int>(42);
 MenuStackActionsV1 actions({lifetime,[&](MenuStackOwnerV1*&out,std::string&){++instance_calls;out=&owner;return true;},os.services});
 CharacterMenuCallV1 call;CharacterMenuValueV1 first;first.kind=4;first.text="menu_InventorySheetMain";call.arguments.push_back(first);call.result.kind=6;call.result.object=0xfffedddccccbbbbaull;call.result.number=std::numeric_limits<double>::quiet_NaN();const auto previous=call.result;
 bool nested=true;call.debug_text=[&](const CharacterMenuValueV1&v,std::string&out,std::string&failure){++conversions;assert(&v==&call.arguments[0]);out=v.text;
  if(nested){nested=false;MenuStackActionValueV1 token{77,4,0};MenuStackActionCallV1 inner{&token,1,0};assert(actions.dispatch("NativePushMenu",inner,[](const MenuStackActionValueV1&v,std::string&name,std::string&){assert(v.identity==77);name="menu_SkillTreeSheetNew";return true;},failure));}return true;};
 assert(actions.dispatch("NativePushMenu",call,error));assert(owner.view()->count==2);assert(owner.view()->renders[1]->states[1]==owner.menu("menu_InventorySheetMain"));assert(conversions==1&&instance_calls==2);assert(call.result.kind==previous.kind&&call.result.object==previous.object&&std::memcmp(&call.result.number,&previous.number,sizeof(double))==0);
 call.arguments[0].kind=2;call.debug_text={};auto ic=instance_calls;assert(actions.dispatch("NativePopAllAbove",call,error));assert(instance_calls==ic&&owner.view()->count==2);call.arguments.clear();assert(actions.dispatch("NativePopMenu",call,error));assert(owner.view()->count==1);assert(actions.dispatch("NativePopAllMenus",call,error));assert(owner.view()->count==0);assert(call.result.kind==6&&call.result.object==previous.object);
 unsigned guards=0;const MenuStackActionCallV1 zero{nullptr,0,0};assert(dh2_menu_stack_action_v1(0,&zero,nullptr,nullptr)==-1);++guards;assert(dh2_menu_stack_action_v1(2,&zero,nullptr,nullptr)==0);++guards;assert(dh2_menu_stack_action_v1(4,&zero,nullptr,nullptr)==-1);++guards;
 MenuStackActionValueV1 v{1,4,1};MenuStackActionCallV1 malformed{&v,1,0};assert(dh2_menu_stack_action_v1(0,&malformed,nullptr,nullptr)==-1);++guards;
 assert(!actions.dispatch("NativePushMenu",call,error));assert(instance_calls==ic+2);++guards;assert(!actions.dispatch("NativeOther",call,error));++guards;
 call.arguments.push_back(first);call.debug_text=[](const CharacterMenuValueV1&,std::string&,std::string&e){e="required conversion rejected";return false;};ic=instance_calls;assert(!actions.dispatch("NativePushMenu",call,error));assert(instance_calls==ic&&error=="required conversion rejected");++guards;
 assert(!actions.dispatch("NativePopMenu",call,error));assert(instance_calls==ic+1&&error=="required conversion rejected");++guards;
 std::cout<<"{\"validation\":\"PASS\",\"gold_cases\":"<<cases<<",\"ordered_services\":"<<services<<",\"failure_prefixes\":"<<prefixes<<",\"facade_checks\":14,\"guards\":"<<guards<<",\"sanitizer_findings\":0}\n";
}
