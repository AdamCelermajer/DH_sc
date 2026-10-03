#include "script_game_bindings.h"
#include "character_timers.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <stdexcept>
#include <cstring>
using namespace dh2::character;
namespace {
unsigned checks=0,expiries=0,start_calls=0,stop_calls=0;
void check(bool yes){++checks;if(!yes)throw std::runtime_error("binding check "+std::to_string(checks));}
struct Fixture {
  dh2_script_vm* vm=nullptr;
  std::array<Timer32,64> slots{};
  TimerStore32 store{slots.data(),0,64,0xabcdef0123456789ULL,0,0};
  TimerServices32 services{};
  dh2_script_game_bindings bindings{};
};
int load(Fixture& f,const std::string& code){return dh2_script_vm_load(f.vm,code.data(),code.size(),"@binding-audit");}
dh2_script_value get(Fixture& f,const char* name){dh2_script_value value{};check(dh2_script_vm_get_global(f.vm,name,&value)==0);return value;}
int32_t start(void* context,uintptr_t owner,uint32_t duration,int32_t repeat,int32_t event,uintptr_t ref){
  auto& f=*static_cast<Fixture*>(context);check(owner==f.store.owner&&event==0x35&&ref==0);
  ++start_calls;auto id=dh2_character_timer_start(&f.store,duration,repeat,event,ref,&f.services);
  check(id>=0);return id;
}
void stop(void* context,uintptr_t owner,uint32_t id){
  auto& f=*static_cast<Fixture*>(context);check(owner==f.store.owner);
  ++stop_calls;check(dh2_character_timer_stop(&f.store,id)>=0);
}
void expired(void* context,uintptr_t owner,int32_t event,Timer32* timer){
  auto& f=*static_cast<Fixture*>(context);check(owner==f.store.owner&&event==0x35&&timer->user_ref==0);
  ++expiries;check(dh2_script_game_on_timer(f.vm,timer->id)==0);
}
int identity(void*,const dh2_script_value*,uint32_t,dh2_script_value* out,
  uint32_t,uint32_t* returned,char*,size_t){out[0]={};out[0].type=2;out[0].identity=0x123456789abcdef0ULL;*returned=1;return 0;}
int projected(void*,const dh2_script_value* args,uint32_t count,dh2_script_value* out,
  uint32_t,uint32_t* returned,char*,size_t){
  if(count!=1)return 1;out[0]={};out[0].type=3;out[0].number=float(args[0].type);
  out[1]={};out[1].type=3;out[1].number=float(args[0].text_bytes);*returned=2;return 0;
}
}
int main(int argc,char** argv){try{
  check(argc==2);std::ifstream file(argv[1],std::ios::binary);
  std::string commons((std::istreambuf_iterator<char>(file)),{});check(commons.size()==13535);
  Fixture f;f.vm=dh2_script_vm_create(8*1024*1024);check(f.vm!=nullptr);
  f.services={&f,expired,nullptr,0};f.bindings={&f,f.store.owner,start,stop,0};
  check(dh2_script_vm_load(f.vm,commons.data(),commons.size(),"@ai/_commons.luac")==0);
  auto bad=f.bindings;bad.start=nullptr;check(dh2_script_game_bind(f.vm,&bad)==-1);
  check(get(f,"StartTimer").type==0&&get(f,"StopTimer").type==0);
  check(load(f,"StartTimerCB(20,false,function() end)")==-2);
  check(dh2_script_game_bind(f.vm,&f.bindings)==0);
  check(load(f,"arity0=select('#',StartTimer()); arity1=select('#',StartTimer('20')); arity2=select('#',StartTimer(false)); Trace('ignored',123)")==0);
  check(get(f,"arity0").number==0&&get(f,"arity1").number==0&&get(f,"arity2").number==0&&start_calls==0);
  check(load(f,"one=StartTimer(12.75); loop=StartTimer(9,'false'); numerical=StartTimer(9,0)")==0);
  check(f.slots[0].duration_ms==12&&f.slots[0].repeat==0&&f.slots[1].repeat==-1&&f.slots[2].repeat==0);
  check(load(f,"StopTimer(one); StopTimer(loop); StopTimer(numerical); StopTimer('1'); StopTimer()")==0);
  check(stop_calls==3&&!f.slots[0].active&&!f.slots[1].active&&!f.slots[2].active);
  check(dh2_script_vm_bind(f.vm,"NativeIdentity",identity,nullptr)==0);
  check(dh2_script_vm_bind_source_values(f.vm,"InspectSource",projected,nullptr)==0);
  check(load(f,"index_count=0; local object=setmetatable({}, {__index=function(t,k) assert(k=='_this'); index_count=index_count+1;return NativeIdentity() end}); obj=StartTimer(10,object); plain=StartTimer(10,{}); raw=StartTimer(10,newproxy()); fn=StartTimer(10,function() end); thread=StartTimer(10,coroutine.create(function() end)); light=StartTimer(10,NativeIdentity()); str_type,str_size=InspectSource('a\\000b'); Trace(object)")==0);
  check(f.slots[0].repeat==-1&&f.slots[1].repeat==0&&f.slots[2].repeat==0&&f.slots[3].repeat==0&&f.slots[4].repeat==0&&f.slots[5].repeat==-1);
  check(get(f,"index_count").number==2&&get(f,"str_type").number==4&&get(f,"str_size").number==1);
  check(dh2_character_timers_stop_all(&f.store)==1);
  check(load(f,"seen=0; saved=''; timer=StartTimerCB(10,false,function(id,r) seen=seen+1;saved=r.name;assert(id==timer) end,{name='owner-reference'})")==0);
  check(dh2_character_timers_update(&f.store,9,0,&f.services)==1&&get(f,"seen").number==0);
  check(dh2_character_timers_update(&f.store,1,0,&f.services)==1&&get(f,"seen").number==1);
  auto saved=get(f,"saved");check(saved.text_bytes==15&&std::memcmp(saved.text,"owner-reference",15)==0);
  check(dh2_character_timers_update(&f.store,100,0,&f.services)==1&&get(f,"seen").number==1);
  check(load(f,"loop_seen=0; looping=StartTimerCB(10,true,function(id) loop_seen=loop_seen+1;if loop_seen==3 then StopTimerCB(id) end end)")==0);
  check(dh2_character_timers_update(&f.store,35,0,&f.services)==1&&get(f,"loop_seen").number==3&&!f.slots[0].active);
  check(load(f,"old_seen=0;new_seen=0; local id; id=StartTimerCB(10,false,function() old_seen=old_seen+1;replacement=StartTimerCB(10,false,function()new_seen=new_seen+1 end) end)")==0);
  check(dh2_character_timers_update(&f.store,10,0,&f.services)==1&&get(f,"old_seen").number==1&&f.slots[0].active);
  check(get(f,"replacement").number==0);
  check(dh2_character_timers_update(&f.store,10,0,&f.services)==1&&get(f,"new_seen").number==0);
  check(load(f,"zero=StartTimer(-1); infinity=StartTimer(math.huge); invalid=StartTimer(0/0)")==0);
  check(f.slots[0].duration_ms==0&&f.slots[1].duration_ms==0xffffffffu&&f.slots[2].duration_ms==0);
  check(dh2_character_timers_stop_all(&f.store)==1);
  check(load(f,"missing=select('#',StopTimer(123456)); silent=select('#',Trace({}))")==0);
  check(get(f,"missing").number==0&&get(f,"silent").number==0);
  check(load(f,"original_timer=OnTimer; function OnTimer(id) observed_id=id;return 1,{},'a\\000b',true,function()end,coroutine.create(function()end),newproxy() end")==0);
  check(dh2_script_game_on_timer(f.vm,0xffffffffu)==0&&get(f,"observed_id").number==-1.f);
  check(load(f,"return_lookups=0;return_order=''; function OnTimer(id) local t={};for i=1,256 do local index=i;t[i]=setmetatable({}, {__index=function(_,key)assert(key=='_this');return_lookups=return_lookups+1;return_order=return_order..index..',';if index==1 then from_return=StartTimer(30) end;return NativeIdentity()end})end;return unpack(t) end")==0);
  check(dh2_script_game_on_timer(f.vm,17)==0);
  check(get(f,"return_lookups").number==256.f&&f.slots[0].active&&f.slots[0].duration_ms==30);
  auto order=get(f,"return_order");std::string expected_order;for(unsigned i=1;i<=256;++i)expected_order+=std::to_string(i)+",";
  check(order.text_bytes==expected_order.size()&&std::memcmp(order.text,expected_order.data(),expected_order.size())==0);
  check(load(f,"function OnTimer() return setmetatable({}, {__index=function()error('projection-failure')end}) end")==0);
  check(dh2_script_game_on_timer(f.vm,17)==-2&&std::strstr(dh2_script_vm_error(f.vm),"projection-failure"));
  check(load(f,"OnTimer=original_timer")==0);check(dh2_script_game_on_timer(f.vm,99)==0);
  dh2_script_vm_destroy(f.vm);
  std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"genuine_timer_start_calls\":"<<start_calls<<",\"genuine_timer_stop_calls\":"<<stop_calls<<",\"actual_commons_timer_dispatches\":"<<expiries<<",\"source_one_shot_replacement_cleanup_preserved\":true,\"source_table_this_lookup_preserved\":true,\"discarded_return_arity\":256,\"discarded_return_metatable_order_exact\":true,\"game_timer_service_stubs\":false,\"full_CharAI_lifecycle_integration\":false,\"mismatches\":0}\n";
  return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
