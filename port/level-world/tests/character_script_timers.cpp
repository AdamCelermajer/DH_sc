#include "character_script_timers.hpp"
#include "character_ai_events.hpp"
#include <array>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

using namespace dh2::character;
struct Fixture {
 std::vector<Timer32> slots=std::vector<Timer32>(20);
 TimerStore32 timers{slots.data(),0,20,0x100000001ull,0,0};
 dh2_script_vm* vm=nullptr;
 unsigned expiries=0;int callback_status=0;
 TimerServices32 services{};
 ScriptTimerBridge bridge{};
 AIEventOwner48 owner{};
 AIEventState64 ai{};
 std::array<std::uintptr_t,51> ai_virtuals{},ais_virtuals{};
 AIEventServices24 event_services{};
 unsigned get_id_calls=0,ais_calls=0;
};
std::uint32_t timer_id(const Timer32* timer){return timer->id;}
std::int32_t event_service(void* context,AIEventState64* state,const AIEventRequest40* request,std::uint32_t* value){
 auto& f=*static_cast<Fixture*>(context);assert(state==&f.ai&&request->event==0x35);
 if(request->service==ai_event_timer_id){
  assert(request->callee==reinterpret_cast<std::uintptr_t>(&timer_id));
  ++f.get_id_calls;*value=timer_id(reinterpret_cast<const Timer32*>(request->subject));return 0;
 }
 if(request->service==ai_event_virtual){
  assert(request->operation==0x90&&request->subject==f.ai.ai&&request->callee==reinterpret_cast<std::uintptr_t>(&dh2_character_ai_event_script_timer));
  AIEventResult16 result{};return dh2_character_ai_event_script_timer(&result,state,request->argument,&f.event_services);
 }
 if(request->service==ai_event_ais_virtual){
  assert(request->operation==0x90&&request->subject==reinterpret_cast<std::uintptr_t>(&f.vm)&&request->callee==reinterpret_cast<std::uintptr_t>(&dh2_script_game_on_timer));
  ++f.ais_calls;f.callback_status=dh2_script_game_on_timer(f.vm,request->argument);return f.callback_status;
 }
 return -1;
}
void expired(void* context,std::uintptr_t owner,std::int32_t event,Timer32* timer){
 auto& f=*static_cast<Fixture*>(context);
 assert(owner==f.timers.owner&&event==0x35&&timer&&timer->user_ref==0);
 ++f.expiries;f.callback_status=0;
 const AIEventPayload24 payload{reinterpret_cast<std::uintptr_t>(timer),reinterpret_cast<std::uintptr_t>(&timer_id),0,0};
 AIEventResult16 result{};const auto status=dh2_character_ai_event(&result,&f.ai,std::uint32_t(event),&payload,&f.event_services);
 assert(status==(f.callback_status?3:0));
}
int grow(void* context,TimerStore32* timers,std::uint32_t minimum){
 auto& f=*static_cast<Fixture*>(context);assert(timers==&f.timers&&!timers->update_depth);
 f.slots.resize(minimum+20);timers->slots=f.slots.data();timers->capacity=std::uint32_t(f.slots.size());return 1;
}
void load(dh2_script_vm* vm,const char* text){assert(dh2_script_vm_load(vm,text,std::strlen(text),"@native-timer-bridge-fixture")==0);}
float number(dh2_script_vm* vm,const char* name){dh2_script_value value{};assert(dh2_script_vm_get_global(vm,name,&value)==0&&value.type==DH2_SCRIPT_NUMBER);return value.number;}
int main(int argc,char** argv){
 assert(argc==2);Fixture f;f.services={&f,expired,grow,0};f.bridge.timers=&f.timers;f.bridge.services=&f.services;
 f.vm=dh2_script_vm_create(4*1024*1024);assert(f.vm);assert(dh2_character_script_bind_timers(f.vm,&f.bridge)==0);
 f.owner={f.timers.owner,reinterpret_cast<std::uintptr_t>(&f.bridge),reinterpret_cast<std::uintptr_t>(&f.timers),reinterpret_cast<std::uintptr_t>(&f.slots),0,0,0,0};
 f.ai_virtuals[0x90/4]=reinterpret_cast<std::uintptr_t>(&dh2_character_ai_event_script_timer);
 f.ais_virtuals[0x90/4]=reinterpret_cast<std::uintptr_t>(&dh2_script_game_on_timer);
 f.ai={reinterpret_cast<std::uintptr_t>(&f.ai),&f.owner,f.ai_virtuals.data(),reinterpret_cast<std::uintptr_t>(&f.vm),f.ais_virtuals.data(),0,0,0,0,0,0};
 f.event_services={&f,event_service,(1u<<ai_event_timer_id)|(1u<<ai_event_virtual)|(1u<<ai_event_ais_virtual),0};
 std::ifstream input(argv[1],std::ios::binary);std::vector<char> common((std::istreambuf_iterator<char>(input)),{});assert(input&&common.size()==13535);
 assert(dh2_script_vm_load(f.vm,common.data(),common.size(),"@ai-commons")==0);
 assert(dh2_script_game_on_timer(f.vm,0)==0); // Actual authored empty OnTimer.
 load(f.vm,"hits=0; last=-9; function OnTimer(id) hits=hits+1;last=id end; one=StartTimer(10); loop=StartTimer(7,true)");
 assert(number(f.vm,"one")==0&&number(f.vm,"loop")==1);
 assert(f.timers.count==2&&f.timers.slots[0].event==0x35&&f.timers.slots[0].duration_ms==10&&f.timers.slots[0].repeat==0);
 assert(f.timers.slots[1].duration_ms==7&&f.timers.slots[1].repeat==-1);
 assert(dh2_character_timers_update(&f.timers,7,0,&f.services)==1&&f.callback_status==0&&number(f.vm,"hits")==1&&number(f.vm,"last")==1);
 assert(dh2_character_timers_update(&f.timers,3,0,&f.services)==1&&number(f.vm,"hits")==2&&number(f.vm,"last")==0);
 load(f.vm,"StopTimer(loop); n=select('#',StartTimer('17')); trace=select('#',Trace({})); outside=select('#',StopTimer(4294967295))");
 assert(number(f.vm,"n")==0&&number(f.vm,"trace")==0&&number(f.vm,"outside")==0&&!f.timers.slots[1].active);
 assert(dh2_character_timers_update(&f.timers,100,0,&f.services)==1&&number(f.vm,"hits")==2);
 // The source clears a one-shot before invoking its expiry callback. A timer
 // started by that callback can reuse and reactivate the same native slot.
 load(f.vm,"replacement=-9; function OnTimer(id) replacement=StartTimer(9) end; reuse=StartTimer(1)");
 assert(dh2_character_timers_update(&f.timers,1,0,&f.services)==1&&f.callback_status==0);
 assert(number(f.vm,"reuse")==0&&number(f.vm,"replacement")==0&&f.timers.slots[0].active&&f.timers.slots[0].duration_ms==9);
 assert(dh2_character_timers_stop_all(&f.timers)==1);
 // Timer expiry still reads its ID when no AIS is active, but invokes no VM.
 load(f.vm,"gated=0;function OnTimer(id) gated=gated+1 end;inactive=StartTimer(1)");
 const auto prior_calls=f.ais_calls;f.ai.active=0;
 assert(dh2_character_timers_update(&f.timers,1,0,&f.services)==1&&f.ais_calls==prior_calls&&number(f.vm,"gated")==0);
 f.ai.active=reinterpret_cast<std::uintptr_t>(&f.vm);f.ai.global_blocked=255;f.owner.locked=255;
 load(f.vm,"active=StartTimer(1)");
 assert(dh2_character_timers_update(&f.timers,1,0,&f.services)==1&&number(f.vm,"gated")==1);
 f.ai.global_blocked=0;f.owner.locked=0;
 // Returned table projection must run even though selected OnTimer discards it.
 load(f.vm,"projected=0; function OnTimer(id) return setmetatable({}, {__index=function(t,k) projected=projected+1; StartTimer(4);return nil end}) end; projection=StartTimer(1)");
 assert(dh2_character_timers_update(&f.timers,1,0,&f.services)==1&&f.callback_status==0&&number(f.vm,"projected")==1);
 assert(f.timers.slots[0].active&&f.timers.slots[0].duration_ms==4);
 assert(dh2_character_timers_stop_all(&f.timers)==1);
 // A real protected callback error is recorded, without stopping native cleanup.
 load(f.vm,"function OnTimer(id) error('authored-fixture-error') end; broken=StartTimer(1)");
 assert(dh2_character_timers_update(&f.timers,1,0,&f.services)==1&&f.callback_status==-2);
 assert(std::strstr(dh2_script_vm_error(f.vm),"authored-fixture-error")&&!f.timers.slots[0].active);
 assert(dh2_character_timers_stop_all(&f.timers)==1);
 // Exhaust actual native storage with growth disabled. Its diagnostic -2 must
 // become a protected port failure, never a Lua timer ID or silent source -1.
 f.services.grow=nullptr;f.timers.count=f.timers.capacity;
 for(std::uint32_t i=0;i<f.timers.count;++i){f.slots[i].id=i;f.slots[i].active=1;}
 const char* full="bad=StartTimer(1)";
 assert(dh2_script_vm_load(f.vm,full,std::strlen(full),"@full-store")==-2&&f.bridge.diagnostic==-2);
 assert(std::strstr(dh2_script_vm_error(f.vm),"native character timer start failed (-2)"));
 dh2_script_value bad{};assert(dh2_script_vm_get_global(f.vm,"bad",&bad)==0&&bad.type==DH2_SCRIPT_NIL);
 // Owner drift is diagnosed on the actual callback; rejected bindings have no
 // published timer names in a different VM.
 f.timers.owner++;load(f.vm,"ok=pcall(function() StartTimer(1) end)");
 dh2_script_value ok{};assert(dh2_script_vm_get_global(f.vm,"ok",&ok)==0&&ok.type==DH2_SCRIPT_BOOLEAN&&!ok.boolean);
 auto* empty=dh2_script_vm_create(1024*1024);ScriptTimerBridge missing{};
 assert(dh2_character_script_bind_timers(empty,&missing)==-1);
 assert(dh2_script_vm_get_global(empty,"StartTimer",&bad)==0&&bad.type==DH2_SCRIPT_NIL);
 dh2_script_vm_destroy(empty);dh2_script_vm_destroy(f.vm);
 std::cout<<"{\"validation\":\"PASS\",\"actual_common_loaded\":true,\"timer_expiries\":"<<f.expiries<<",\"actual_timer_id_reads\":"<<f.get_id_calls<<",\"selected_ais_vm_calls\":"<<f.ais_calls<<",\"source_ai_event35_composed\":true,\"inactive_ais_gate_verified\":true,\"native_storage_failures_protected\":true,\"owner_drift_rejected\":true,\"return_projection_timer_start\":true,\"whole_ai_ownership_claimed\":false}\n";
}
