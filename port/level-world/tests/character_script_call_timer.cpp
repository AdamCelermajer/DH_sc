#include "../character_script_call_timer.hpp"
#include "../character_script_timers.hpp"
#include "../character_ai_events.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
using namespace dh2::character;
namespace {
unsigned checks=0;
void check(bool value,const char* reason){++checks;if(!value)throw std::runtime_error(reason);}
std::uint32_t bits(float value){std::uint32_t out;std::memcpy(&out,&value,4);return out;}
struct Fixture {
 std::unique_ptr<dh2_script_vm,decltype(&dh2_script_vm_destroy)> vm{dh2_script_vm_create(4*1024*1024),dh2_script_vm_destroy};
 std::unique_ptr<dh2_script_aliases,decltype(&dh2_script_alias_destroy)> aliases{dh2_script_alias_create(),dh2_script_alias_destroy};
 ScriptTimerCall16 call{vm.get(),aliases.get()};
 std::vector<Timer32> slots=std::vector<Timer32>(20);
 TimerStore32 timers{slots.data(),0,20,0x100000001ull,0,0};
 TimerServices32 timer_services{};ScriptTimerBridge bridge{};
 AIEventOwner48 owner{};AIEventState64 ai{};
 std::array<std::uintptr_t,51> ai_virtuals{},ais_virtuals{};
 AIEventServices24 services{};
 unsigned timer_reads=0,selected_calls=0,expiries=0;int last_status=0;
 ~Fixture(){vm.reset();aliases.reset();} // Borrowed alias callbacks stay alive through VM close.
};
void load(Fixture& f,const char* text){check(!dh2_script_vm_load(f.vm.get(),text,std::strlen(text),"@selected-timer-call-fixture"),"Lua load failed");}
float number(Fixture& f,const char* key){dh2_script_value v{};check(!dh2_script_vm_get_global(f.vm.get(),key,&v)&&v.type==DH2_SCRIPT_NUMBER,"numeric global absent");return v.number;}
std::uint32_t timer_id(const Timer32* timer){return timer->id;}
std::int32_t service(void* opaque,AIEventState64* ai,const AIEventRequest40* r,std::uint32_t* result){
 auto& f=*static_cast<Fixture*>(opaque);check(ai==&f.ai&&r->event==0x35,"wrong source timer dispatch");
 if(r->service==ai_event_timer_id){
  check(r->callee==reinterpret_cast<std::uintptr_t>(&timer_id),"GetID identity differs");
  ++f.timer_reads;*result=timer_id(reinterpret_cast<const Timer32*>(r->subject));return 0;
 }
 if(r->service==ai_event_virtual){
  check(r->operation==0x90&&r->subject==f.ai.ai&&r->callee==reinterpret_cast<std::uintptr_t>(&dh2_character_ai_event_script_timer),"CharAI relay differs");
  AIEventResult16 out{};return dh2_character_ai_event_script_timer(&out,ai,r->argument,&f.services);
 }
 if(r->service==ai_event_ais_virtual){
  check(r->operation==0x90&&r->subject==reinterpret_cast<std::uintptr_t>(&f.call)&&r->callee==reinterpret_cast<std::uintptr_t>(&dh2_character_script_call_timer),"selected script service differs");
  ++f.selected_calls;f.last_status=dh2_character_script_call_timer(&f.call,r->argument);return f.last_status;
 }
 return -1;
}
int dispatch(Fixture& f,Timer32* timer){
 AIEventPayload24 payload{reinterpret_cast<std::uintptr_t>(timer),reinterpret_cast<std::uintptr_t>(&timer_id),0,0};
 AIEventResult16 out{};f.last_status=0;const int result=dh2_character_ai_event(&out,&f.ai,0x35,&payload,&f.services);
 check(result==(f.last_status?3:0),"AI protected error propagation differs");return result;
}
void expired(void* opaque,std::uintptr_t owner,std::int32_t event,Timer32* timer){
 auto& f=*static_cast<Fixture*>(opaque);check(owner==f.timers.owner&&event==0x35&&timer->user_ref==0,"expiry owner/event differs");
 ++f.expiries;dispatch(f,timer);
}
void setup(Fixture& f){
 check(f.vm&&f.aliases,"allocation failed");check(!dh2_script_alias_bind(f.vm.get(),f.aliases.get()),"alias bind failed");
 f.timer_services={&f,expired,nullptr,0};f.bridge.timers=&f.timers;f.bridge.services=&f.timer_services;
 check(!dh2_character_script_bind_timers(f.vm.get(),&f.bridge),"genuine timer bind failed");
 f.owner={f.timers.owner,reinterpret_cast<std::uintptr_t>(&f.bridge),reinterpret_cast<std::uintptr_t>(&f.timers),reinterpret_cast<std::uintptr_t>(&f.slots),0,0,0,0};
 f.ai_virtuals[0x90/4]=reinterpret_cast<std::uintptr_t>(&dh2_character_ai_event_script_timer);
 f.ais_virtuals[0x90/4]=reinterpret_cast<std::uintptr_t>(&dh2_character_script_call_timer);
 f.ai={reinterpret_cast<std::uintptr_t>(&f.ai),&f.owner,f.ai_virtuals.data(),reinterpret_cast<std::uintptr_t>(&f.call),f.ais_virtuals.data(),0,0,0,0,0,0};
 f.services={&f,service,(1u<<ai_event_timer_id)|(1u<<ai_event_virtual)|(1u<<ai_event_ais_virtual),0};
}
}
int main(int argc,char** argv){try{
 check(argc==3,"expected gold and commons");Fixture f;setup(f);
 std::ifstream gold(argv[1],std::ios::binary);std::vector<char> bytes((std::istreambuf_iterator<char>(gold)),{});
 check(gold&&bytes.size()>=8&&!std::memcmp(bytes.data(),"STC1",4),"gold absent");
 std::uint32_t count;std::memcpy(&count,bytes.data()+4,4);check(bytes.size()==8+16*count&&count==576,"gold extent differs");
 int prior_mode=-1;unsigned active=0;
 for(unsigned i=0;i<count;++i){
  std::array<std::uint32_t,4> row;std::memcpy(row.data(),bytes.data()+8+16*i,16);
  if(int(row[2])!=prior_mode){
   f.aliases.reset(dh2_script_alias_create());f.call.aliases=f.aliases.get();check(f.aliases&&!dh2_script_alias_bind(f.vm.get(),f.aliases.get()),"alias session reset failed");
   if(row[2])check(!dh2_script_alias_add(f.aliases.get(),"OnTimer","Renamed"),"timer alias failed");
   if(row[2]==2)check(!dh2_script_alias_add(f.aliases.get(),"Renamed","Other"),"alias chain failed");
   prior_mode=int(row[2]);
  }
  load(f,"hits=0;which=-1;last=0;function OnTimer(id) hits=hits+1;which=0;last=id end;function Renamed(id) hits=hits+1;which=1;last=id end;function Other(id) hits=hits+1;which=2;last=id end");
  f.ai.active=row[1]?reinterpret_cast<std::uintptr_t>(&f.call):0;Timer32 timer{};timer.id=row[0];const auto before=f.selected_calls;
  check(!dispatch(f,&timer),"gold dispatch failed");check(f.selected_calls-before==row[1],"inactive gate differs");
  check(number(f,"hits")==float(row[1]),"gold hit count differs");
  if(row[1]){++active;check(bits(number(f,"last"))==row[3],"original signed timer Value differs");check(number(f,"which")==float(row[2]?1:0),"original one-pass lookup differs");}
 }
 const auto corpus_checks=checks;
 f.ai.active=reinterpret_cast<std::uintptr_t>(&f.call);f.ai.global_blocked=255;f.owner.locked=255;
 std::ifstream source(argv[2],std::ios::binary);std::vector<char> commons((std::istreambuf_iterator<char>(source)),{});check(source&&commons.size()==13535,"commons absent");
 check(!dh2_script_vm_load(f.vm.get(),commons.data(),commons.size(),"@actual-ai-commons"),"actual commons load failed");
 check(!dh2_script_alias_add(f.aliases.get(),"OnTimer","OnTimer"),"restore default alias");check(!dispatch(f,nullptr),"null payload UINT_MAX dispatch failed");
 load(f,"hits=0;last=0;function Renamed(id) hits=hits+1;last=id end;AddToVFTable('OnTimer','Renamed');t=StartTimer(1)");
 check(dh2_character_timers_update(&f.timers,1,0,&f.timer_services)==1&&f.last_status==0&&number(f,"hits")==1&&number(f,"last")==0,"real timer selected alias expiry differs");
 load(f,"function Renamed(id) hits=hits+10;last=id;AddToVFTable('OnTimer','Other') end;function Other(id) hits=hits+100;last=id end;t=StartTimer(1)");
 check(dh2_character_timers_update(&f.timers,1,0,&f.timer_services)==1&&number(f,"hits")==11,"live global replacement ignored");
 Timer32 timer{};timer.id=0xffffffff;check(!dispatch(f,&timer)&&number(f,"hits")==111&&number(f,"last")==-1,"next live alias or signed ID differs");
 // Original discard projects ALL results in order. Projection can invoke real
 // Lua __index and genuine StartTimer even above the old fixed arity of 16.
 load(f,"effects=0;order='';function Other(id) local r={};for i=1,40 do r[i]=setmetatable({},{__index=function(t,k) effects=effects+1;order=order..i..','; if i==40 then replacement=StartTimer(9) end;return nil end}) end;return unpack(r) end;t=StartTimer(1)");
 check(dh2_character_timers_update(&f.timers,1,0,&f.timer_services)==1&&f.last_status==0&&number(f,"effects")==40,"all discarded result projections absent");
 check(number(f,"replacement")==0&&f.slots[0].active&&f.slots[0].duration_ms==9,"projection did not reuse genuine cleared timer slot");
 dh2_script_value order{};std::string expected_order;for(unsigned i=1;i<=40;++i)expected_order+=std::to_string(i)+",";
 check(!dh2_script_vm_get_global(f.vm.get(),"order",&order)&&order.type==DH2_SCRIPT_STRING&&std::string(order.text,order.text_bytes)==expected_order,"discarded results projected out of order");
 check(dh2_character_timers_stop_all(&f.timers)==1,"timer cleanup failed");
 load(f,"effects=0;function Other(id) return setmetatable({},{__index=function(t,k) effects=effects+1;error('projection-error') end}) end;t=StartTimer(1)");
 check(dh2_character_timers_update(&f.timers,1,0,&f.timer_services)==1&&f.last_status==-2&&!f.slots[0].active,"projection protected error/cleanup differs");
 check(std::strstr(dh2_script_vm_error(f.vm.get()),"projection-error"),"projection error missing");check(number(f,"effects")==1,"projection mutation lost");
 load(f,"changed=0;function Other(id) changed=changed+1;error('timer-body-error') end");
 check(dispatch(f,&timer)==3&&f.last_status==-2,"body error discarded");
 check(std::strstr(dh2_script_vm_error(f.vm.get()),"timer-body-error"),"body error missing");
 check(number(f,"changed")==1,"body mutation discarded");
 check(!dh2_script_alias_add(f.aliases.get(),"OnTimer","Missing"),"missing alias setup");check(dispatch(f,&timer)==3&&f.last_status==-2,"missing function accepted");
 check(!dh2_script_alias_add(f.aliases.get(),"OnTimer",""),"empty alias setup");check(dispatch(f,&timer)==3&&f.last_status==-2,"empty alias accepted");
 f.ai.active=0;const auto before=f.selected_calls;check(!dispatch(f,&timer)&&f.selected_calls==before,"inactive alias error not gated");
 ScriptTimerCall16 invalid{nullptr,f.aliases.get()};check(dh2_character_script_call_timer(nullptr,0)==-1&&dh2_character_script_call_timer(&invalid,0)==-1,"malformed VM pair accepted");invalid={f.vm.get(),nullptr};check(dh2_character_script_call_timer(&invalid,0)==-1,"malformed alias pair accepted");
 std::cout<<"{\"validation\":\"PASS\",\"original_cases\":"<<count<<",\"original_active_calls\":"<<active<<",\"corpus_checks\":"<<corpus_checks<<",\"integration_checks\":"<<checks-corpus_checks<<",\"actual_timer_expiries\":"<<f.expiries<<",\"discarded_projection_count\":40,\"inactive_gate_verified\":true,\"source_event35_composed\":true,\"real_alias_and_vm_calls\":true,\"protected_errors_verified\":true,\"full_ais_ownership_claimed\":false,\"mismatches\":0}\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 3;}}
