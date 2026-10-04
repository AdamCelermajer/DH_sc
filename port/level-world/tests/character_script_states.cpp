#include "character_script_states.hpp"
#include "character_script_update.hpp"
#include <array>
#include <cstring>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
using namespace dh2::character;
namespace {
unsigned checks=0;
void check(bool value){++checks;if(!value)throw std::runtime_error("states check "+std::to_string(checks));}
std::int32_t word(std::istream& f){std::int32_t v;f.read(reinterpret_cast<char*>(&v),4);check(bool(f));return v;}
struct Event {int kind,index;std::string text;bool operator==(const Event& other)const{return kind==other.kind&&index==other.index&&text==other.text;}};
struct Replay {
 ScriptStatesRuntime16 runtime{};std::array<ScriptStateRecord32,3> records;std::array<std::array<std::string,4>,3> names;
 std::vector<Event> events;int mutation=-1,destination=-1;
 Replay(){for(unsigned i=0;i<3;++i){for(unsigned j=0;j<4;++j)names[i][j]=std::string(1,char('A'+i))+"_"+std::to_string(j);records[i]={names[i][0].c_str(),names[i][1].c_str(),names[i][2].c_str(),names[i][3].c_str()};}}
 int current(){for(int i=0;i<3;++i)if(runtime.current==&records[i])return i;return -1;}
 static int call(void* p,ScriptStatesRuntime16*,const ScriptStatesRequest32* r,ScriptStateRecord32** out){auto& t=*static_cast<Replay*>(p);
  if(r->service==states_lookup_insert){t.events.push_back({0,0,r->text});*out=&t.records[0];}
  else if(r->service==states_assign)t.events.push_back({1,int(r->index),r->text});
  else {t.events.push_back({r->service==states_lua_call?2:3,t.current(),r->text?r->text:""});if(int(t.events.size())-1==t.mutation)t.runtime.current=t.destination<0?nullptr:&t.records[t.destination];}
  return 0;
 }
};
struct VM {
 dh2_script_vm* vm=dh2_script_vm_create_empty(16*1024*1024);dh2_script_aliases* aliases=dh2_script_alias_create();ScriptStates states;
 ScriptUpdateState48 base{1,2,3,4,0,0,0,0};unsigned pauses=0,stops=0;int vm_status=0;
 VM(){check(vm&&aliases);check(!dh2_script_vm_open_source_libraries(vm));check(!dh2_script_alias_bind(vm,aliases));check(!states.bind_source(vm,aliases));}
 ~VM(){dh2_script_vm_destroy(vm);dh2_script_alias_destroy(aliases);}
 void load(const char* source){check(!dh2_script_vm_load_source_file(vm,source,std::strlen(source)));check(dh2_script_vm_stack_size(vm)==5);}
 std::string text(const char* name){dh2_script_value v{};check(!dh2_script_vm_get_global(vm,name,&v)&&v.type==DH2_SCRIPT_STRING);return v.text;}
 static int lua_call(void* pointer,const char* name){auto& t=*static_cast<VM*>(pointer);t.vm_status=dh2_script_alias_call_discard_source(t.vm,t.aliases,name,nullptr,0);return t.vm_status&&t.vm_status!=-2;}
 static void base_service(void* pointer,ScriptUpdateState48*,const ScriptUpdateRequest32* r){auto& t=*static_cast<VM*>(pointer);if(r->service==script_update_timer_start){check(r->duration_ms==1000&&r->event==0x31);++t.pauses;}else{check(r->subject==4);++t.stops;}}
 static int base_call(void* pointer){auto& t=*static_cast<VM*>(pointer);const ScriptUpdateServices16 services{&t,&base_service};return dh2_character_script_update(&t.base,0,&services)!=1;}
 ScriptStatesCalls24 services(){return {this,&lua_call,&base_call};}
};
}
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream gold(argv[1],std::ios::binary);char magic[4];gold.read(magic,4);check(!std::memcmp(magic,"SST1",4));auto count=word(gold);unsigned services=0;
 for(int n=0;n<count;++n){auto op=word(gold),argc=word(gold),invalid=word(gold),prior=word(gold),flags=word(gold),mutation=word(gold),destination=word(gold),final=word(gold),events=word(gold);std::vector<Event> expected;
  for(int e=0;e<events;++e){auto kind=word(gold),index=word(gold),size=word(gold);std::string text(size,'\0');gold.read(text.data(),size);check(bool(gold));expected.push_back({kind,index,text});}
  Replay r;r.runtime={prior<0?nullptr:&r.records[prior],uint32_t(flags),0};r.mutation=mutation;r.destination=destination;const ScriptStatesServices16 c{&r,&Replay::call};int result;
  if(op==0){std::array<dh2_script_value,8> values{};std::array<std::string,8> strings;for(int i=0;i<argc;++i){strings[i]="arg"+std::to_string(i);values[i].type=i==invalid?0:4;values[i].text=strings[i].c_str();}result=dh2_character_script_states_register(&r.runtime,values.data(),argc,&c);}
  else if(op==1)result=dh2_character_script_states_change(&r.runtime,destination<0?nullptr:&r.records[destination],&c);
  else if(op==2)result=dh2_character_script_states_update(&r.runtime,&c);
  else result=dh2_character_script_states_call(&r.runtime,op-3,&c);
  check(result==1&&r.events==expected&&r.current()==final);services+=events;
 }
 check(gold.peek()==EOF);
 VM v;v.load("log=''; function AU()log=log..'aU';ChangeAIState('B')end;function AC()log=log..'aC'end;function AI()log=log..'aI'end;function AP()log=log..'aP'end;function BU()log=log..'bU'end;function BC()log=log..'bC'end;function BI()log=log..'bI'end;function BP()log=log..'bP'end;RegisterAIState('A','AU','AC','AI','AP');RegisterAIState('B','BU','BC','BI','BP');ChangeAIState('A')");
 check(v.states.size()==2&&std::string(v.states.current_name())=="A"&&v.text("log")=="aI");
 v.load("log='';function OnUpdate()log=log..'G'end");v.states.runtime().flags=1;v.base.collision_ms=200;
 check(v.states.external_update(v.services())==1&&v.pauses==1&&v.stops==1&&v.base.paused==1&&v.text("log")=="GaUaPbIbC"&&std::string(v.states.current_name())=="B");
 // Post reentry captures outer target B, even when Post changes current to C.
 v.load("RegisterAIState('C','BU','BC','BI','BP');function RePost()log=log..'R';RegisterAIState('A','AU','AC','AI','AP');ChangeAIState('C')end;ChangeAIState('A');RegisterAIState('A','AU','AC','AI','RePost');log='';ChangeAIState('B')");
 check(std::string(v.states.current_name())=="B"&&v.text("log")=="RaPbIbI");
 v.load("RegisterAIState('B','Fresh');function Fresh()log=log..'F'end;log='';ChangeAIState('B');RegisterAIState('B',17);RegisterAIState('D');ChangeAIState('missing')");
 ScriptStateRecord32 b{};check(v.states.find("B",b)&&std::string(b.update)=="Fresh"&&std::string(b.conditions)=="BC"&&std::string(b.init)=="BI"&&std::string(b.post)=="BP"&&v.states.size()==3);
 v.load("function BI()error('delivered-state-error')end;log='';ChangeAIState('B');after=1");check(v.states.last_vm_status()>0&&std::string(v.states.current_name())=="B");
 // Unbounded returned-table projections happen before the next state call.
 v.load("function CI()log=log..'cI'end;RegisterAIState('C','BU','BC','CI','BP');function BI()return setmetatable({}, {__index=function(t,k)if k=='_this'then log=log..'T';ChangeAIState('C')end end})end;log='';ChangeAIState('B')");check(std::string(v.states.current_name())=="C"&&v.text("log")=="bPTbPcI");
 const auto size=v.states.size();const ScriptStatesCalls24 missing{};check(v.states.external_update(missing)==-1&&v.states.change_name("A",missing)==-1&&v.states.size()==size);
 v.load("RegisterAIState('nil','BU','BC','CI','BP');RegisterAIState('true','BU','BC','CI','BP');RegisterAIState('false','BU','BC','CI','BP');ChangeAIState(nil)");check(std::string(v.states.current_name())=="nil");v.load("ChangeAIState(true)");check(std::string(v.states.current_name())=="true");v.load("ChangeAIState(false);ChangeAIState();ChangeAIState('A','B')");check(std::string(v.states.current_name())=="false");
 const char* unsupported="ChangeAIState(17)";check(dh2_script_vm_load_source_file(v.vm,unsupported,std::strlen(unsupported))>0&&std::string(v.states.current_name())=="false");
 Dl_info module{},runtime{};check(dladdr(reinterpret_cast<void*>(&dh2_character_script_states_update),&module));check(dladdr(reinterpret_cast<void*>(&dh2_script_vm_bind_source_scoped),&runtime));
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"original_cases\":"<<count<<",\"ordered_services\":"<<services<<",\"genuine_scoped_VM\":true,\"actual_default_kernel\":true,\"owner_library\":\""<<module.dli_fname<<"\",\"runtime_library\":\""<<runtime.dli_fname<<"\"}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
