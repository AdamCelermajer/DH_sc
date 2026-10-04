#include "character_script_owner.hpp"
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
void check(bool yes){++checks;if(!yes)throw std::runtime_error("owner check "+std::to_string(checks));}
std::uint32_t word(std::istream& f){std::uint32_t x=0;f.read(reinterpret_cast<char*>(&x),4);check(bool(f));return x;}
struct Fixture {
 std::string common;std::array<Timer32,32> slots{};
 TimerStore32 store{slots.data(),0,32,0xabcdef0123456789ULL,0,0};
 TimerServices32 timer_services{};
 std::map<std::uintptr_t,dh2_script_game_bindings> bindings;
 std::vector<std::pair<unsigned,unsigned>> delivered;
 int fail_at=-1;bool missing=false,bad_lua=false,reenter=false;unsigned files=0;
 std::unique_ptr<ScriptOwner> owner;
 explicit Fixture(const std::string& input):common(input){
  timer_services={this,&expire,nullptr,0};owner.reset(new ScriptOwner(store.owner));
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
 static int service(void* p,ScriptOwner& owner,const ScriptOwnerRequest& r,ScriptOwnerResponse& out){
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
 ScriptOwnerServices services(){return {this,&service};}
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
int main(int argc,char** argv){try{
 check(argc==3);std::ifstream gold(argv[1],std::ios::binary);char magic[4];gold.read(magic,4);check(std::memcmp(magic,"SOW1",4)==0);check(word(gold)==300);
 for(unsigned n=0;n<300;++n){auto phase=word(gold),index=word(gold),callback=word(gold),method=word(gold),size=word(gold);std::string name(size,'\0');gold.read(name.data(),size);check(bool(gold));ScriptBinding24 b{};check(dh2_character_script_binding(&b,phase,index)==0);check(name==b.name&&callback==b.original_callback&&method==b.method&&b.character_receiver==unsigned(phase==2));}
 std::ifstream input(argv[2],std::ios::binary);std::string common((std::istreambuf_iterator<char>(input)),{});check(common.size()==13535);
 Fixture first(common),second(common);check(first.advance()==0&&second.advance()==0);
 ScriptSessionView a{},b{};check(first.owner->active(a)&&second.owner->active(b));check(a.identity!=b.identity&&a.vm!=b.vm&&a.aliases!=b.aliases);
 check(first.delivered.size()==170&&second.delivered.size()==170);check(first.owner->lifecycle().load_step==7&&first.owner->lifecycle().pending==a.identity&&first.owner->lifecycle().scripted==1);
 check(std::string(a.path)=="data/scripts/ai/"&&a.loaded_files==1&&a.constructor_fields->character==first.store.owner&&a.constructor_fields->derived_present==1);
 check(dh2_script_vm_stack_size(a.vm)==5&&dh2_script_vm_stack_size(b.vm)==5);
 check(get(a.vm,"SetInt").type==0); // Explicit fixture boundary, no stub global installed.
 check(first.files==1);first.owner->lifecycle().load_step=3;check(first.advance()==0&&first.files==1);
 check(dh2_script_alias_add(a.aliases,"OnTimer","PrivateTimer")==0);check(!dh2_script_alias_contains(b.aliases,"OnTimer"));
 load(a.vm,"hits=0;function PrivateTimer(id) hits=hits+1; seen=id end");
 const ScriptTimerCall16 call{a.vm,a.aliases};check(dh2_character_script_call_timer(&call,0xffffffff)==0);check(get(a.vm,"seen").number==-1&&get(a.vm,"hits").number==1);
 load(b.vm,"hits=0;local old=OnTimer;function OnTimer(id)hits=hits+1;old(id)end; tid=StartTimerCB(1,false,function(id) fired=id end)");
 check(dh2_character_timers_update(&second.store,2,0,&second.timer_services)==1);check(timer_events==1&&get(b.vm,"fired").number>=0);
 first.owner->lifecycle().load_step=1;check(first.advance()==0&&dh2_script_vm_stack_size(a.vm)==10);check(first.delivered.size()==340);
 for(int at:{0,1,4,34,35,70,169}){Fixture failed(common);failed.fail_at=at;check(failed.advance()==-2);check(failed.owner->lifecycle().active==0&&failed.owner->lifecycle().pending!=0);ScriptSessionView p{};check(failed.owner->pending(p));check(dh2_script_vm_stack_size(p.vm)==5);check(failed.delivered.size()==unsigned(at));}
 Fixture absent(common);absent.missing=true;check(absent.advance()==0);check(absent.owner->active(a)&&a.loaded_files==0);
 Fixture bad(common);bad.bad_lua=true;check(bad.advance()==0&&bad.owner->last_vm_status()==-2);check(bad.owner->active(a)&&a.loaded_files==0);
 Fixture nested(common);nested.reenter=true;check(nested.advance()==0&&nested.delivered.size()==170&&reentries==1);
 for(const char* name:{"Player","Other"}){
  Fixture plain(common);check(plain.owner->advance({0,0,nullptr,name},plain.services())==0);check(plain.owner->active(a));
  check(!plain.owner->lifecycle().scripted&&plain.files==0&&a.loaded_files==0&&dh2_script_vm_stack_size(a.vm)==5);
  check(a.kind==(std::strcmp(name,"Player")==0?script_player:script_default));
  check(a.constructor_fields->derived_present==unsigned(a.kind!=script_default));
 }
 {
  Fixture close(common);check(close.advance()==0&&close.owner->active(a));
  check(dh2_script_vm_bind_source_values(a.vm,"InspectCleared",&finalizer,a.aliases)==0);
  load(a.vm,"AddToVFTable('OnTimer','old');PushVFTable();AddToVFTable('OnTimer','during');proxy=newproxy(true);getmetatable(proxy).__gc=function() InspectCleared();PopVFTable();AddToVFTable('GC','survived') end");
  close.owner.reset();check(gc_callbacks==1);
 }
 Fixture malformed(common);auto before=malformed.owner->lifecycle();ScriptOwnerServices empty{};check(malformed.owner->advance({10,0,"__player__","Player"},empty)==-1);check(malformed.owner->lifecycle().pending==before.pending&&malformed.delivered.empty());
 malformed.owner->lifecycle().load_step=-1;check(malformed.advance()==-1&&malformed.delivered.empty());
 Dl_info module{},runtime{},lifecycle{},timer{};check(dladdr(reinterpret_cast<void*>(&dh2_character_script_binding),&module));check(dladdr(reinterpret_cast<void*>(&dh2_script_vm_create_empty),&runtime));
 check(dladdr(reinterpret_cast<void*>(&dh2_character_script_lifecycle),&lifecycle));check(dladdr(reinterpret_cast<void*>(&dh2_character_script_call_timer),&timer));
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"original_binding_descriptors\":300,\"source_null_method_calls_skipped\":130,\"native_registration_deliveries\":"<<registrations<<",\"private_sessions\":2,\"provider_failure_prefixes\":"<<failures<<",\"genuine_timer_expiries\":"<<timer_events<<",\"loader_reentries\":"<<reentries<<",\"actual_close_finalizers\":"<<gc_callbacks<<",\"genuine_trace_alias_timer_bindings\":true,\"all_gameplay_globals_implemented\":false,\"owner_library\":\""<<module.dli_fname<<"\",\"runtime_library\":\""<<runtime.dli_fname<<"\",\"lifecycle_library\":\""<<lifecycle.dli_fname<<"\",\"timer_library\":\""<<timer.dli_fname<<"\"}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
