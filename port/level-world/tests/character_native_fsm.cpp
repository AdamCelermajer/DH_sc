#include "character_native_fsm.hpp"
#include <array>
#include <cstring>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace dh2::character;
namespace {
unsigned checks=0;
void check(bool v){++checks;if(!v)throw std::runtime_error("native FSM check "+std::to_string(checks));}
std::uint32_t word(std::istream& f){std::uint32_t v=0;f.read(reinterpret_cast<char*>(&v),4);check(bool(f));return v;}
using Event=std::array<std::uint32_t,6>;
struct Replay {
 State state{};NativeFsm24 view{&state,0xabcdef0123456789ull,1,0};std::vector<Event> events;
 std::uint32_t dt=0;int stun=-1,scare=-1,fail=-1,mutate=0;
 Facts facts{};std::vector<Request> state_requests;bool tail=false,route=false;
 std::uint32_t current()const{return view.current_present?std::uint32_t(state.current):0xffffffffu;}
 static void state_service(void* p,State* s,const Request* r){auto& t=*static_cast<Replay*>(p);t.state_requests.push_back(*r);
  if(t.route&&r->service==raise_event&&r->argument[0]==0x3f){const Services c{p,&state_service};check(dh2_character_state_event(s,&t.facts,0x3f,0,&c)==1);}
 }
 static int service(void* p,NativeFsm24* f,const NativeFsmRequest32* r,std::uint32_t* out){auto& t=*static_cast<Replay*>(p);
  check(f==&t.view&&!r->force&&!r->payload);Event e{r->service,0,0,0,0,0};
  if(r->service==fsm_engine_dt)e={r->service,0,0,t.state.elapsed_ms,0,t.dt};
  else if(r->service==fsm_set_stun||r->service==fsm_set_scare){check(r->subject==t.view.character);e={r->service,r->argument0,r->argument1,t.state.elapsed_ms,t.current(),0};}
  else if(r->service==fsm_current_update){check(r->subject==t.view.character&&r->argument0==t.current()&&!r->argument1);e={r->service,r->argument0,0,t.state.elapsed_ms,0,0};}
  else check(!r->subject&&!r->argument0&&!r->argument1);
  t.events.push_back(e);if(int(r->service)==t.fail)return 1;
  if(r->service==fsm_engine_dt){*out=t.dt;if(t.mutate==2)t.state.elapsed_ms=0x12345678u;}
  else if(r->service==fsm_set_stun||r->service==fsm_set_scare){int result=r->service==fsm_set_stun?t.stun:t.scare;if(result>=0){t.state.current=result;t.view.current_present=1;}
   if(t.mutate==1&&r->service==fsm_set_stun){t.state.attack_gate&=~4u;t.view.character=0xfedcba9876543210ull;}
   if(t.mutate==3&&r->service==fsm_set_scare)t.view.current_present=0;
  }else if(r->service==fsm_current_update&&t.tail){const Services c{p,&state_service};const int result=dh2_character_native_fsm_bounded_tail(f,&t.facts,&c);if(result<0)return 1;}
  return 0;
 }
 NativeFsmServices16 services(){return {this,&service};}
};
float integer_float(std::uint32_t bits){std::int32_t value=0;std::memcpy(&value,&bits,4);return static_cast<float>(value);}
}
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream gold(argv[1],std::ios::binary);char magic[4];gold.read(magic,4);check(!std::memcmp(magic,"NFM1",4));auto cases=word(gold);unsigned ordered=0;
 for(unsigned i=0;i<cases;++i){Replay t;auto initial=word(gold),mask=word(gold),policy=word(gold),elapsed=word(gold);t.dt=word(gold);check(word(gold)==0);t.stun=std::int32_t(word(gold));t.scare=std::int32_t(word(gold));auto final=word(gold),final_elapsed=word(gold),count=word(gold);std::vector<Event> expected;
  for(unsigned j=0;j<count;++j){Event e{};for(auto& v:e)v=word(gold);expected.push_back(e);}
  t.state.current=std::int32_t(initial);t.view.current_present=initial!=0xffffffffu;t.state.attack_gate=mask;t.state.flags=policy;t.state.elapsed_ms=elapsed;auto c=t.services();check(dh2_character_native_fsm_update(&t.view,&c)==1);check(t.current()==final&&t.state.elapsed_ms==final_elapsed&&t.events==expected);ordered+=count;
 }check(gold.peek()==EOF);
 unsigned getters=0;
 for(auto id:{-1,0,3,4,5,8,9,12,17,2147483647})for(auto bits:{0u,1u,500u,0x7fffffffu,0x80000000u,0xffffffffu}){
  Replay t;t.state.current=id;t.view.current_present=id!=-1;t.state.elapsed_ms=bits;std::int32_t value=42;check(dh2_character_native_fsm_get_integer(&value,&t.view,0)==1&&value==id);check(dh2_character_native_fsm_get_integer(&value,&t.view,1)==1&&std::uint32_t(value)==bits);
  for(auto fn:{&dh2_character_native_fsm_script_get_state,&dh2_character_native_fsm_script_get_time})for(auto arity:{0u,1u,65u}){dh2_script_value output{};std::uint32_t count=0;char error[64]{};check(!fn(&t.view,nullptr,arity,&output,1,&count,error,sizeof error)&&count==1&&output.type==DH2_SCRIPT_NUMBER&&output.number==(fn==&dh2_character_native_fsm_script_get_state?static_cast<float>(id):integer_float(bits)));++getters;}
 }
 for(auto name:{"","Limbus","PreSpawn","Idle","limbus","PreSpawnX"}){std::int32_t result=-9;check(dh2_character_native_fsm_preset_state(&result,name)==1);check(result==(!std::strcmp(name,"Limbus")?0:!std::strcmp(name,"PreSpawn")?17:3));}
 unsigned prefixes=0;
 for(int fail=0;fail<6;++fail){Replay t;t.state.current=3;t.state.attack_gate=6;t.state.flags=0xc00;t.state.elapsed_ms=0xfffffff8u;t.dt=16;t.stun=9;t.scare=8;t.fail=fail;auto c=t.services();check(dh2_character_native_fsm_update(&t.view,&c)==-2);check(t.events.size()==unsigned(fail+1));check(t.state.elapsed_ms==(fail<=1?0xfffffff8u:8));check(t.current()==std::uint32_t(fail<=2?3:fail==3?9:8));++prefixes;}
 {Replay t;t.state.current=3;t.state.attack_gate=6;t.stun=9;t.mutate=1;auto c=t.services();check(dh2_character_native_fsm_update(&t.view,&c)==1&&t.events.size()==5&&t.events[3][0]==fsm_current_update&&t.events[3][1]==9);}
 {Replay t;t.state.elapsed_ms=0xfffffffdu;t.dt=9;t.mutate=2;auto c=t.services();check(dh2_character_native_fsm_update(&t.view,&c)==1&&t.state.elapsed_ms==6);}
 {Replay t;t.state.current=3;t.state.attack_gate=4;t.mutate=3;auto c=t.services();check(dh2_character_native_fsm_update(&t.view,&c)==1&&t.events.size()==4&&!t.view.current_present);}
 // Genuine existing source-built state bodies receive dt0 after the outer dt.
 unsigned compositions=0;
 for(int id:{3,4,5,12})for(unsigned dt:{0u,16u,0xffffffffu}){Replay t;t.state.current=id;t.state.elapsed_ms=7;t.state.heading_active=1;t.state.move_type=1;t.state.cached_speed=1;t.facts.is_player=1;t.facts.target=0x123456789abcdef0ull;t.dt=dt;t.tail=true;auto c=t.services();check(dh2_character_native_fsm_update(&t.view,&c)==1&&t.state.elapsed_ms==7u+dt);
  check(t.state_requests.size()==unsigned(id==3||id==5));if(id==3)check(t.state_requests[0].service==idle_common_update);if(id==5)check(t.state_requests[0].service==look_at&&t.state_requests[0].identity==t.facts.target);++compositions;
 }
 {Replay t;t.state.current=4;t.state.elapsed_ms=100;t.dt=25;t.tail=true;t.route=true;t.facts.is_player=1;t.facts.idle=1040;auto c=t.services();check(dh2_character_native_fsm_update(&t.view,&c)==1&&t.state.current==3&&t.state.elapsed_ms==0&&t.state.flags==0x2380);check(t.state_requests.size()==4&&t.state_requests[0].service==raise_event&&t.state_requests[1].service==stop&&t.state_requests[2].service==set_animation&&t.state_requests[3].service==raise_event);++compositions;}
 {Replay t;t.state.current=9;t.tail=true;t.dt=20;auto c=t.services();check(dh2_character_native_fsm_update(&t.view,&c)==-2&&t.state.elapsed_ms==20&&t.state.current==9&&t.state_requests.empty()&&t.events.back()[0]==fsm_current_update);++compositions;}
 unsigned guards=0;Replay t;auto c=t.services();const State before=t.state;std::int32_t value=0x12345678;
 check(dh2_character_native_fsm_get_integer(nullptr,&t.view,0)==-1);++guards;
 check(dh2_character_native_fsm_get_integer(&value,nullptr,0)==-1&&value==0x12345678);++guards;
 check(dh2_character_native_fsm_get_integer(&value,&t.view,2)==-1&&value==0x12345678);++guards;
 check(dh2_character_native_fsm_preset_state(&value,nullptr)==-1&&value==0x12345678);++guards;
 check(dh2_character_native_fsm_update(nullptr,&c)==-1);++guards;
 check(dh2_character_native_fsm_update(&t.view,nullptr)==-1);++guards;
 t.view.reserved=1;check(dh2_character_native_fsm_update(&t.view,&c)==-1);++guards;t.view.reserved=0;
 t.view.current_present=2;check(dh2_character_native_fsm_update(&t.view,&c)==-1);++guards;t.view.current_present=1;
 {std::array<unsigned char,sizeof(dh2_script_value)> bytes;bytes.fill(0xa5);dh2_script_value output;std::memcpy(&output,bytes.data(),bytes.size());std::uint32_t count=17;char error[64]{};check(dh2_character_native_fsm_script_get_state(&t.view,nullptr,0,&output,0,&count,error,sizeof error)!=0&&count==17&&!std::memcmp(&output,bytes.data(),bytes.size()));++guards;}
 check(!std::memcmp(&before,&t.state,sizeof before)&&t.events.empty());
 auto* vm=dh2_script_vm_create_empty(4*1024*1024);check(vm&&!dh2_script_vm_open_source_libraries(vm));check(!dh2_script_vm_bind_source_values(vm,"GetState",&dh2_character_native_fsm_script_get_state,&t.view));check(!dh2_script_vm_bind_source_values(vm,"GetStateTime",&dh2_character_native_fsm_script_get_time,&t.view));unsigned vm_cases=0;
 for(auto bits:{0u,1u,0x7fffffffu,0x80000000u,0xffffffffu})for(auto id:{-1,3,8,9,17}){t.state.current=id;t.view.current_present=id!=-1;t.state.elapsed_ms=bits;const char* program="sid=GetState('ignored',false,17);elapsed=GetStateTime(nil);function Read()return sid,elapsed end";check(!dh2_script_vm_load_source_file(vm,program,std::strlen(program)));std::array<dh2_script_value,2> output{};unsigned count=0;check(!dh2_script_vm_call(vm,"Read",nullptr,0,output.data(),2,&count)&&count==2&&output[0].type==DH2_SCRIPT_NUMBER&&output[0].number==float(id)&&output[1].type==DH2_SCRIPT_NUMBER&&output[1].number==integer_float(bits)&&dh2_script_vm_stack_size(vm)==5);++vm_cases;}
 dh2_script_vm_destroy(vm);
 Dl_info module{},world{},runtime{};check(dladdr(reinterpret_cast<void*>(&dh2_character_native_fsm_update),&module));check(dladdr(reinterpret_cast<void*>(&dh2_character_state_update),&world));check(dladdr(reinterpret_cast<void*>(&dh2_script_vm_bind_source_values),&runtime));
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"original_update_cases\":"<<cases<<",\"ordered_services\":"<<ordered<<",\"direct_script_getter_cases\":"<<getters<<",\"native_failure_prefixes\":"<<prefixes<<",\"linked_state_compositions\":"<<compositions<<",\"atomic_guards\":"<<guards<<",\"genuine_VM_getters\":"<<vm_cases<<",\"module_library\":\""<<module.dli_fname<<"\",\"world_library\":\""<<world.dli_fname<<"\",\"runtime_library\":\""<<runtime.dli_fname<<"\"}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
