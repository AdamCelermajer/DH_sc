#include "character_script_owner_v2.hpp"
#include "character_timers.hpp"
#include "character_script_call_timer.hpp"
#include "../script-runtime/script_game_bindings.h"
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <iterator>
#include <cstring>
#include <dlfcn.h>
#include <stdexcept>
using namespace dh2::character;
namespace {
unsigned checks=0,registrations=0,timer_events=0,failures=0,reentries=0,gc_callbacks=0;
void checked(bool yes,int line){++checks;if(!yes)throw std::runtime_error("owner check "+std::to_string(checks)+" line "+std::to_string(line));}
#define check(yes) checked(yes,__LINE__)
std::uint32_t word(std::istream& f){std::uint32_t x=0;f.read(reinterpret_cast<char*>(&x),4);check(bool(f));return x;}
struct Fixture {
 std::string common;std::array<Timer32,32> slots{};
 TimerStore32 store{slots.data(),0,32,0xabcdef0123456789ULL,0,0};
 TimerServices32 timer_services{};
 std::map<std::uintptr_t,dh2_script_game_bindings> bindings;
 std::vector<std::pair<unsigned,unsigned>> delivered;
 int fail_at=-1;bool missing=false,bad_lua=false,reenter=false;unsigned files=0;
 std::unique_ptr<ScriptOwnerV2> owner;
 explicit Fixture(const std::string& input):common(input){
  timer_services={this,&expire,nullptr,0};owner.reset(new ScriptOwnerV2(store.owner));
 }
 static int32_t start(void* p,uintptr_t subject,uint32_t duration,int32_t repeat,int32_t event,uintptr_t ref){
  auto& t=*static_cast<Fixture*>(p);check(subject==t.store.owner&&event==0x35&&ref==0);
  const auto id=dh2_character_timer_start(&t.store,duration,repeat,event,ref,&t.timer_services);check(id>=0);return id;
 }
 static void stop(void* p,uintptr_t subject,uint32_t id){auto& t=*static_cast<Fixture*>(p);check(subject==t.store.owner);check(dh2_character_timer_stop(&t.store,id)>=0);}
 static void expire(void* p,uintptr_t subject,int32_t event,Timer32* timer){
  auto& t=*static_cast<Fixture*>(p);check(subject==t.store.owner);if(event!=0x35)return;
  ScriptSessionView active{};check(t.owner->active(active));const ScriptTimerCall16 call{active.vm,active.aliases};
  check(dh2_character_script_call_timer(&call,timer->id)==0);++timer_events;
 }
 static int service(void* p,ScriptOwnerV2& owner,const ScriptOwnerRequest& r,ScriptOwnerResponse& out){
  auto& t=*static_cast<Fixture*>(p);
  switch(r.service){
   case owner_register_binding:{
    check(r.session&&r.binding&&!r.binding->method);check(dh2_script_vm_stack_size(r.session->vm)>=5);
    if(int(t.delivered.size())==t.fail_at){++failures;return 1;}
    t.delivered.emplace_back(r.argument0,r.argument1);++registrations;
    if(t.reenter){
     t.reenter=false;const auto step=owner.lifecycle().load_step;owner.lifecycle().load_step=7;
     // A nested source completed-loader call has no effects. It must not erase
     // the outer call's borrowed services/facts before its next registration.
     check(owner.advance({0,0,nullptr,"Other"},t.services())==0);owner.lifecycle().load_step=step;++reentries;
    }
    // Registration-delivery fixture: no fake implementations of the other
    // captured gameplay globals. Genuine trace/alias/timer bindings only.
    if(std::strcmp(r.binding->name,"Trace")==0)check(dh2_script_vm_bind_source_values(r.session->vm,"Trace",dh2_script_game_trace,nullptr)==0);
    if(std::strcmp(r.binding->name,"AddToVFTable")==0)check(dh2_script_alias_bind(r.session->vm,r.session->aliases)==0);
    if(std::strcmp(r.binding->name,"StartTimer")==0){
     auto& b=t.bindings[r.session->identity];b={&t,r.subject,&start,&stop,0};check(dh2_script_game_bind(r.session->vm,&b)==0);
    }
    break;
   }
   case owner_cached_file:
    ++t.files;check(std::string(r.filename)=="data/scripts/ai/_commons.luac");
    out.word=t.missing?0:1;out.bytes=t.bad_lua?"broken ! lua":t.common.data();out.size=t.bad_lua?12:t.common.size();break;
   case owner_is_character:out.word=1;break;
   case owner_budget:out.word=1;break;
   case owner_is_dead:out.word=0;break;
   case owner_design_tick:out.word=1000;break;
   case owner_timer_stop:check(dh2_character_timer_stop(&t.store,r.argument0)>=0);break;
   case owner_timer_start:{
    check(r.subject==t.store.owner&&(r.argument1==0x33||r.argument1==0x34));
    auto id=dh2_character_timer_start(&t.store,r.argument0,-1,int32_t(r.argument1),0,&t.timer_services);check(id>=0);out.word=uint32_t(id);break;
   }
   case owner_ai_terminate:owner.lifecycle().active=0;break;
   default:return 1;
  }
  return 0;
 }
 ScriptOwnerServicesV2 services(){return {this,&service};}
 int advance(){return owner->advance({10,0,"__player__","Player"},services());}
};
dh2_script_value get(dh2_script_vm* vm,const char* name){dh2_script_value v{};check(dh2_script_vm_get_global(vm,name,&v)==0);return v;}
void load(dh2_script_vm* vm,const char* code){check(dh2_script_vm_load(vm,code,std::strlen(code),"@owner-test")==0);}
int finalizer(void* context,const dh2_script_value*,uint32_t,dh2_script_value*,uint32_t,uint32_t* count,char*,size_t){
 *count=0;auto* aliases=static_cast<dh2_script_aliases*>(context);
 if(dh2_script_alias_contains(aliases,"OnTimer"))return 1;
 ++gc_callbacks;return 0;
}
}

#include "../character_ais_death_vm_v56.hpp"
namespace {
struct DeathFixture:Fixture {
 std::string script;std::uintptr_t selected{};unsigned nested_count{};
 explicit DeathFixture(const std::string& common):Fixture(common){}
 static int provider(void* p,ScriptOwnerV2& o,const ScriptOwnerRequest& r,ScriptOwnerResponse& out){auto& f=*static_cast<DeathFixture*>(p);if(r.service==owner_cached_file&&std::strcmp(r.filename,"data/scripts/ai/_commons.luac")){out.word=1;out.bytes=f.script.data();out.size=f.script.size();return 0;}return Fixture::service(&f,o,r,out);}
 static int type(void*,uintptr_t,const char** n){*n="Character";return 0;}
 static int methods(void*,uintptr_t,const dh2_script_object_method** m,uint32_t* count){*m=nullptr;*count=0;return 0;}
 static int method(void*,const dh2_script_callback_scope*,uintptr_t,uint32_t,const dh2_script_value*,uint32_t,dh2_script_value*,uint32_t,uint32_t*,char*,size_t){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
 static int nested(void* p,const dh2_script_callback_scope* scope,const dh2_script_value*,uint32_t,char*,size_t){auto& f=*static_cast<DeathFixture*>(p);++f.nested_count;std::string e;return character_ais_death_vm_v56(*f.owner,f.selected,0x3dd440,202,scope,e);}
};
}
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream input(argv[1],std::ios::binary);std::string common((std::istreambuf_iterator<char>(input)),{});check(common.size()==13535);
 DeathFixture f(common);check(!f.owner->advance({7,0,"fixture","Other"},{&f,DeathFixture::provider}));ScriptSessionView v{};check(f.owner->active(v)&&v.kind==script_external);f.selected=v.identity;
 uint32_t source{};check(f.owner->source_death_callback(source)&&source==0x3dd440);
 dh2_script_object_services objects{&f,DeathFixture::type,DeathFixture::methods,DeathFixture::method};check(!dh2_script_vm_set_source_objects(v.vm,&objects));
 const auto top=dh2_script_vm_stack_size(v.vm);std::string e;
 check(!character_ais_death_vm_v56(*f.owner,v.identity,source,202,nullptr,e)); // Real commons empty endpoint.
 load(v.vm,"calls=0;depth=0;function Death(killer) assert(killer._this~=nil);calls=calls+1;if depth==0 then depth=1;Nested();depth=0 end end;AddToVFTable('OnDied','Death')");
 check(!dh2_script_vm_bind_source_scoped(v.vm,"Nested",DeathFixture::nested,&f));
 check(!character_ais_death_vm_v56(*f.owner,v.identity,source,202,nullptr,e));check(get(v.vm,"calls").number==2&&f.nested_count==1&&dh2_script_vm_stack_size(v.vm)==top);
 load(v.vm,"function Death(killer) calls=calls+100;error('death-mutation-prefix')end");
 check(!character_ais_death_vm_v56(*f.owner,v.identity,source,202,nullptr,e));check(get(v.vm,"calls").number==102&&e.find("death-mutation-prefix")!=std::string::npos);
 dh2_script_callback_scope expired{v.vm,0};check(character_ais_death_vm_v56(*f.owner,v.identity,source,202,&expired,e)==-1);
 check(character_ais_death_vm_v56(*f.owner,v.identity,0x1234,202,nullptr,e)==DH2_SCRIPT_REQUIRED_FAILURE_STATUS);
 check(character_ais_death_vm_v56(*f.owner,UINT64_MAX,source,202,nullptr,e)==-1);
 f.owner->lifecycle().active=0;check(!f.owner->source_death_callback(source));
 // Captured receiver survives source active reload; original callback receiver
 // does not change midway through the void virtual dispatch.
 check(!character_ais_death_vm_v56(*f.owner,v.identity,0x3dd440,202,nullptr,e));check(get(v.vm,"calls").number==202);
 const char* selectors[]={"__npc__","__monster__","__player__","__faery__"};
 const unsigned kinds[]={script_default,script_monster,script_player_iphone,script_faery};
 for(unsigned i=0;i<4;++i){
  Fixture branch(common);
  const int code=branch.owner->advance({unsigned(std::strlen(selectors[i])),0,selectors[i],"Other"},branch.services());
  if(kinds[i]==script_monster||kinds[i]==script_faery){check(code==-2);check(!branch.owner->lifecycle().active);check(!branch.owner->source_death_callback(source)&&source==0);continue;}
  if(code)throw std::runtime_error(std::string("actual constructor fixture ")+selectors[i]+": "+branch.owner->error());
  ScriptSessionView actual{};
  check(branch.owner->active(actual));
  check(actual.kind==kinds[i]);
  check(branch.owner->source_death_callback(source));
  check(source==0x3dbe90);
  check(!character_ais_death_vm_v56(*branch.owner,actual.identity,source,202,nullptr,e));
 }
 Fixture source_player(common);check(!source_player.owner->advance({0,0,nullptr,"Player"},source_player.services()));ScriptSessionView actual_player{};check(source_player.owner->active(actual_player)&&actual_player.kind==script_player);check(source_player.owner->source_death_callback(source)&&source==0x3dbe90);
 Fixture inherited(common);check(!inherited.owner->advance({0,0,nullptr,"Other"},{&inherited,Fixture::service}));ScriptSessionView d{};check(inherited.owner->active(d));check(inherited.owner->source_death_callback(source)&&source==0x3dbe90);check(!character_ais_death_vm_v56(*inherited.owner,d.identity,source,202,nullptr,e));
 std::cout<<"{\"status\":\"PASS\",\"four_supported_V2_constructors\":true,\"nested_real_VM_scope\":true,\"source_error_mutation_prefix\":true,\"expired_scope_rejected\":true,\"checks\":"<<checks<<"}\n";std::cout<<"PASS four supported V2 constructor AIS+24 branches and two rejected unimplemented factories; actual commons/private VM/source-object attacker; fresh alias/global, protected source error prefix, nested scope, expired scope and unknown source rejection. External cache/register/type-method fixture boundaries explicit.\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
