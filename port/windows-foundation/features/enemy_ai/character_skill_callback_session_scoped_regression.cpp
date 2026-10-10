#include "../../../level-world/character_skill_callback_session_v3.cpp"
#include "../../../level-world/character_skill_callbacks_v3.hpp"
#include "../../../script-runtime/script_runtime.h"
#include "../../../script-runtime/script_function_alias.h"
#include <cassert>
#include <cstring>
#include <initializer_list>
#include <string>
#include <vector>

using namespace dh2::character;
using namespace dh2::character::skills;

namespace {
struct FakeOwner {
 ScriptSessionView view{};
 bool find(std::uintptr_t id,ScriptSessionView& out)const noexcept{if(id!=view.identity)return false;out=view;return true;}
 bool active(ScriptSessionView& out)const noexcept{out=view;return view.identity!=0;}
};
struct FakeTimers {std::uintptr_t owner=0x1234;};
struct FakeSession {
 FakeOwner owner_state;FakeTimers timer_state;const dh2_script_callback_scope* scope=nullptr;
 FakeOwner& owner()noexcept{return owner_state;}
 const FakeOwner& owner()const noexcept{return owner_state;}
 FakeTimers& timers()noexcept{return timer_state;}
 const dh2_script_callback_scope* current_skill_callback_scope()const noexcept{return scope;}
};
using TestDelivery=Delivery<FakeSession>;
struct Harness {
 FakeSession session;Instance32 instance{};std::string error;std::vector<std::uint32_t> operations;std::uint32_t operation=0;
 TestDelivery delivery;
 Harness():delivery{session,0,0,0,0,error}{}
};
static int service(void* p,const SkillCallbackRequest48V3* q,SkillCallbackResponse32V3* r){
 auto& h=*static_cast<Harness*>(p);h.operations.push_back(q?q->operation:0);
 h.delivery.operation=h.operation;return TestDelivery::invoke(&h.delivery,q,r);
}
static int kernel(Harness& h,std::uint32_t op,std::uint32_t& out){
 h.operation=op;const SkillCallbackServices16V3 services{&h,service};return dh2_character_skill_callback_v3(&out,&h.instance,op,&services);
}
static void require(bool ok,const char* why){if(!ok){std::fprintf(stderr,"FAIL: %s\n",why);std::abort();}}
static void require_operations(const Harness& h,const std::vector<std::uint32_t>& expected,const char* why){
 require(h.operations==expected,why);
}
static void load(dh2_script_vm* vm,const char* source){require(dh2_script_vm_load_source_file(vm,source,std::strlen(source))==0,"Lua load");}
static void assert_string(dh2_script_vm* vm,const char* name,const char* expected){dh2_script_value v{};require(!dh2_script_vm_get_global(vm,name,&v),"get global");if(v.type!=DH2_SCRIPT_STRING||std::string(v.text,v.text_bytes)!=expected){std::fprintf(stderr,"trace expected='%s' got type=%u '%.*s'\n",expected,v.type,static_cast<int>(v.text_bytes),v.text?v.text:"");require(false,"Lua trace");}}
struct NestedCall {Harness* harness;int result=-99;};
static int invoke_kernel_scoped(void* opaque,const dh2_script_callback_scope* scope,const dh2_script_value*,std::uint32_t,char*,std::size_t){
 auto& n=*static_cast<NestedCall*>(opaque);n.harness->session.scope=scope;std::uint32_t out=0;
 n.result=kernel(*n.harness,skill_check_active_v3,out);n.harness->session.scope=nullptr;
 require_operations(*n.harness,{callback_active_v3,callback_set_v3,callback_erase_v3,callback_active_v3,callback_call_v3,callback_bool_v3,callback_release_v3},"same-VM CheckActive service order");
 require(n.result==0&&out==1,"same-VM nested CheckActive through common Session delivery");
 return 0;
}
static int invoke_foreign(void* opaque,const dh2_script_callback_scope* scope,const dh2_script_value*,std::uint32_t,char*,std::size_t){
 auto& n=*static_cast<NestedCall*>(opaque);n.harness->session.scope=scope;std::uint32_t out=0;
 n.result=kernel(*n.harness,skill_check_active_v3,out);n.harness->session.scope=nullptr;
 require_operations(*n.harness,{callback_active_v3,callback_set_v3},"foreign-VM failure prefix");
 require(n.result==-2,"foreign-VM scope rejected by common Session delivery");return 0;
}
static int invoke_hidden_scope(void* opaque,const dh2_script_callback_scope*,const dh2_script_value*,std::uint32_t,char*,std::size_t){
 auto& n=*static_cast<NestedCall*>(opaque);n.harness->session.scope=nullptr;std::uint32_t out=0;
 n.result=kernel(*n.harness,skill_check_active_v3,out);
 require_operations(*n.harness,{callback_active_v3,callback_set_v3},"hidden stale scope fails before unscoped dispatch");
 require(n.result==-2,"busy Session with hidden scope rejected");return 0;
}
void setup(Harness& h,dh2_script_vm* vm,dh2_script_aliases* aliases){
 h.instance={h.session.timer_state.owner,"fixture.skill",0,0,0.0f,0};
 h.session.owner_state.view={h.session.timer_state.owner,vm,aliases,nullptr,"fixture",0,0,0};
}
}

int main(){
 auto* aliases=dh2_script_alias_create();auto* vm=dh2_script_vm_create_empty(8*1024*1024);auto* other=dh2_script_vm_create_empty(8*1024*1024);
 require(aliases&&vm&&other,"allocate aliases and actual Lua VMs");
 require(!dh2_script_vm_open_source_libraries(vm)&&!dh2_script_vm_open_source_libraries(other),"open actual source Lua libraries");
 Harness h;setup(h,vm,aliases);
 load(vm,"trace=''; SetSkill=function(script,arg) trace=trace..'S'; return 7,8 end; "
         "OnPreSkill=function() trace=trace..'P' end; OnSkill=function() trace=trace..'U' end; "
         "OnPostSkill=function() trace=trace..'O' end; "
         "OnSkillCheck=function() trace=trace..'C'; return false,true end; "
         "Trigger=function() return NativeSkill() end");
 std::uint32_t out=99;
 const std::vector<std::uint32_t> ordered_call={callback_active_v3,callback_set_v3,callback_erase_v3,callback_active_v3,callback_call_v3,callback_release_v3};
 h.operations.clear();require(!kernel(h,skill_pre_v3,out)&&out==1,"normal unscoped Pre callback");require_operations(h,ordered_call,"Pre service order");assert_string(vm,"trace","SP");
 h.operations.clear();require(!kernel(h,skill_use_v3,out)&&out==1,"normal unscoped Use callback");require_operations(h,ordered_call,"Use service order");assert_string(vm,"trace","SPSU");
 h.operations.clear();require(!kernel(h,skill_post_v3,out)&&out==0,"normal unscoped Post callback");require_operations(h,ordered_call,"Post service order");assert_string(vm,"trace","SPSUSO");
 h.operations.clear();require(!kernel(h,skill_check_usable_v3,out)&&out==0,"normal unscoped CheckUsable selected return");require_operations(h,{callback_active_v3,callback_set_v3,callback_erase_v3,callback_active_v3,callback_call_v3,callback_bool_v3,callback_release_v3},"CheckUsable service order");assert_string(vm,"trace","SPSUSOSC");
 h.operations.clear();require(!kernel(h,skill_check_active_v3,out)&&out==1,"normal unscoped CheckActive selected return");require_operations(h,{callback_active_v3,callback_set_v3,callback_erase_v3,callback_active_v3,callback_call_v3,callback_bool_v3,callback_release_v3},"CheckActive service order");assert_string(vm,"trace","SPSUSOSCSC");
 // A stale scope must fail closed even while the VM is idle: the unscoped
 // fallback would otherwise execute SetSkill and OnSkillCheck successfully.
 dh2_script_callback_scope stale{vm,0};h.session.scope=&stale;const auto stale_trace=std::string("SPSUSOSCSC");
 h.operations.clear();require(kernel(h,skill_check_active_v3,out)==-2,"stale scope rejected");require_operations(h,{callback_active_v3,callback_set_v3},"stale-scope failure prefix");assert_string(vm,"trace",stale_trace.c_str());h.session.scope=nullptr;
 // The same common Session path is entered from a genuine scoped Lua native
 // callback, where nested SetSkill and OnSkillCheck must use the active VM token.
 NestedCall nested{&h};h.operations.clear();
 require(!dh2_script_vm_bind_source_scoped(vm,"NestedNative",invoke_kernel_scoped,&nested),"bind nested scoped callback");
 load(vm,"Trigger=function() return NestedNative() end; Trigger()");
 assert_string(vm,"trace","SPSUSOSCSCSC");
 // Session getters return null for expired or shadowed scopes. A null scope
 // while the VM is busy must fail before calling the unscoped API.
 NestedCall hidden{&h};h.operations.clear();require(!dh2_script_vm_bind_source_scoped(vm,"HiddenScopeNative",invoke_hidden_scope,&hidden),"bind hidden-scope callback");
 load(vm,"HiddenScopeNative()");assert_string(vm,"trace","SPSUSOSCSCSC");
 // A live capability from a different VM is rejected before any target-VM
 // source callback. Use another genuine Lua callback to obtain that token.
 NestedCall foreign{&h};h.operations.clear();require(!dh2_script_vm_bind_source_scoped(other,"ForeignNative",invoke_foreign,&foreign),"bind foreign-VM scoped callback");
 load(other,"ForeignNative()");
 assert_string(vm,"trace","SPSUSOSCSCSC");
 require(!dh2_script_callback_scope_valid(nullptr),"null capability invalid");
 dh2_script_vm_destroy(other);dh2_script_vm_destroy(vm);dh2_script_alias_destroy(aliases);
 std::puts("{\"validation\":\"PASS\",\"real_lua\":true,\"normal_unscoped_operations\":[\"Pre\",\"Use\",\"Post\",\"CheckUsable\",\"CheckActive\"],\"same_vm_nested\":true,\"stale_scope_fail_closed\":true,\"hidden_stale_scope_fail_closed\":true,\"foreign_vm_fail_closed\":true}");
}
