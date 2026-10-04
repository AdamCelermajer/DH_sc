#include "character_design_services.hpp"
#include "character_game_design.hpp"
#include "character_host_context.hpp"
#include "character_property_bindings.hpp"
#include "character_script_lifecycle.hpp"
#include "character_timers.hpp"
#include "../../script-runtime/script_function_alias.h"
#include "../../script-runtime/script_scalar_bindings.h"
#include <array>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace {
unsigned checks=0;void check(bool value){++checks;if(!value)throw std::runtime_error("design services check "+std::to_string(checks));}
unsigned word(std::istream& f){unsigned v=0;f.read(reinterpret_cast<char*>(&v),4);check(bool(f));return v;}
std::string text(std::istream& f,unsigned size){std::string s(size,'\0');f.read(s.data(),size);check(bool(f));return s;}
using Raw=std::vector<std::uint8_t>;
Raw blob(std::istream& f){Raw raw(word(f));f.read(reinterpret_cast<char*>(raw.data()),raw.size());check(bool(f));return raw;}
struct FileProvider {
 unsigned opens=0,closes=0;int mode=0;std::string directory;bool actual=false;
 static int open(void* p,const char* name,std::uintptr_t* handle){auto& f=*static_cast<FileProvider*>(p);check(!std::strcmp(name,"DebugSwitches.savegame"));++f.opens;
  if(f.actual){auto path=f.directory+"/"+name;*handle=reinterpret_cast<std::uintptr_t>(std::fopen(path.c_str(),"rb"));return *handle||errno==ENOENT?0:1;}
  if(f.mode==2)return 1;
  *handle=f.mode?0xabcdef0123456789ull:0;return 0;
 }
 static int close(void* p,std::uintptr_t handle){auto& f=*static_cast<FileProvider*>(p);++f.closes;if(f.actual)return std::fclose(reinterpret_cast<FILE*>(handle))?1:0;check(handle==0xabcdef0123456789ull);return f.mode==3;}
 DebugFileServices24 services(){return {this,open,close};}
};
using Map=std::vector<std::pair<std::string,unsigned>>;
Map snapshot(DebugSwitches* owner,unsigned& loaded){unsigned count=0;check(dh2_character_debug_snapshot(owner,&loaded,&count)==1);Map result;
 for(unsigned i=0;i<count;++i){const char* name=nullptr;unsigned value=17;check(dh2_character_debug_entry(owner,i,&name,&value)==1&&name);result.emplace_back(name,value);}return result;
}
struct Inputs {
 std::array<std::array<Raw,3>,5> tables;std::vector<Raw> constants;std::vector<dh2::data::Bytes> views;GameDesignInputs256 input{};
 explicit Inputs(const char* path){std::ifstream f(path,std::ios::binary);check(word(f)==0x314f4447);for(auto& table:tables)for(auto& raw:table)raw=blob(f);auto count=word(f);check(count==26);for(unsigned i=0;i<count;++i){blob(f);constants.push_back(blob(f));}count=word(f);for(unsigned i=0;i<count;++i)blob(f);check(f.peek()==EOF);
  GameDesignTableInput48* dest[]={&input.characters,&input.classes,&input.ai,&input.factions,&input.levels};for(unsigned i=0;i<5;++i)*dest[i]={{tables[i][0].data(),tables[i][0].size()},{tables[i][1].data(),tables[i][1].size()},{tables[i][2].data(),tables[i][2].size()}};
  for(auto& raw:constants)views.push_back({raw.data(),raw.size()});
  input.constants=views.data();input.constant_count=views.size();
 }
};
struct HostReceiver {
 HostPlayer8 player{17,0};HostLevel8 level{};std::vector<LevelRangeRow24> rows;
 static int invoke(void* p,const HostContextRequest16* request,HostContextResponse16* response){auto& h=*static_cast<HostReceiver*>(p);if(request->service==host_get_player)response->data=&h.player;else if(request->service==host_get_current_level)response->data=&h.level;else if(request->service==host_get_range_rows){response->data=h.rows.data();response->count=h.rows.size();}else check(false);return 0;}
};
struct DebugCount {DebugLevelBinding16 binding;unsigned loads=0,queries=0;std::vector<unsigned> order;static int invoke(void* p,LevelModel32* model,const LevelRequest24* request){auto& d=*static_cast<DebugCount*>(p);d.order.push_back(request->property*10+request->service);if(request->service==level_debug_load)++d.loads;else if(request->service==level_debug_query)++d.queries;else check(false);return dh2_character_debug_level_service(&d.binding,model,request);}};
int position(void*,const dh2_script_value*,unsigned,dh2_script_value* out,unsigned capacity,unsigned* returned,char*,std::size_t){check(capacity>=2);out[0]={};out[1]={};out[0].type=out[1].type=3;out[0].number=-1234.43f;out[1].number=497.979f;*returned=2;return 0;} // Explicit borrowed authored position fixture; no fake gameplay operation.
std::string origin(void* pointer){Dl_info info{};check(dladdr(pointer,&info));return info.dli_fname;}
Raw file(const char* path){std::ifstream in(path,std::ios::binary);check(bool(in));return {std::istreambuf_iterator<char>(in),{}};}
struct TimerComposition {
 const dh2_script_design_bindings* design=nullptr;std::array<Timer32,4> timers{};TimerStore32 store{timers.data(),0,4,0xabcdef0123456789ull,0,0};TimerServices32 services{};unsigned ticks=0;
 static void invoke(void* p,ScriptLifecycleState64*,const ScriptLifecycleRequest32* r,ScriptLifecycleResponse16* out){auto& t=*static_cast<TimerComposition*>(p);
  if(r->service==script_owner_is_dead)out->word=0; // Explicit alive Character virtual fact.
  else if(r->service==script_design_tick){check(dh2_character_design_tick(&out->word,t.design,r->argument0)==1);++t.ticks;}
  else if(r->service==script_timer_stop)check(dh2_character_timer_stop(&t.store,r->argument0)==1);
  else if(r->service==script_timer_start){check(r->subject==t.store.owner&&!r->payload);const auto id=dh2_character_timer_start(&t.store,r->argument0,-1,std::int32_t(r->argument1),0,&t.services);check(id>=0);out->word=id;}
  else check(false); // Pending absent: no fabricated AIS Init acceptance.
 }
};
}
int main(int argc,char** argv){try{
 check(argc==6);std::ifstream gold(argv[1],std::ios::binary);check(word(gold)==0x31565344);const auto operations=word(gold);DebugSwitches* owner=nullptr;FileProvider provider;auto files=provider.services();unsigned sessions=0;
 for(unsigned i=0;i<operations;++i){auto reset=word(gold),load=word(gold),length=word(gold),expected_value=word(gold),expected_loaded=word(gold),count=word(gold);auto name=text(gold,length);Map expected;for(unsigned j=0;j<count;++j){auto n=word(gold),value=word(gold);expected.emplace_back(text(gold,n),value);}auto file_calls=word(gold);
  if(reset){dh2_character_debug_destroy(owner);owner=dh2_character_debug_create();check(owner);provider.opens=provider.closes=0;++sessions;}const auto before=provider.opens;unsigned value=0x12345678;
  check(load?dh2_character_debug_load(owner,&files)==1:dh2_character_debug_get(&value,owner,name.c_str(),&files)==1&&value==expected_value);unsigned loaded=0;check(snapshot(owner,loaded)==expected&&loaded==expected_loaded&&provider.opens-before==file_calls&&!provider.closes);
 }check(gold.peek()==EOF);dh2_character_debug_destroy(owner);owner=nullptr;
 unsigned failures=0,guards=0;
 for(int mode:{1,2,3}){FileProvider f;f.mode=mode;auto service=f.services();auto* o=dh2_character_debug_create();check(o);const auto status=mode==1?-3:-2;check(dh2_character_debug_load(o,&service)==status);unsigned loaded=0,count=0;check(dh2_character_debug_snapshot(o,&loaded,&count)==1&&loaded==1&&!count&&f.opens==1&&f.closes==unsigned(mode!=2));unsigned value=99;check(dh2_character_debug_get(&value,o,"isTracingChar_Stats",&service)==status&&value==99&&f.opens==1);dh2_character_debug_destroy(o);++failures;}
 owner=dh2_character_debug_create();check(owner);unsigned sentinel=99,loaded=99,count=99;
 check(dh2_character_debug_load(nullptr,&files)==-1);++guards;check(dh2_character_debug_load(owner,nullptr)==-1);++guards;check(dh2_character_debug_get(nullptr,owner,"x",&files)==-1);++guards;check(dh2_character_debug_get(&sentinel,owner,nullptr,&files)==-1&&sentinel==99);++guards;check(dh2_character_debug_snapshot(owner,&sentinel,&sentinel)==-1&&sentinel==99);++guards;const char* name=nullptr;check(dh2_character_debug_entry(owner,0,&name,&sentinel)==-1&&!name&&sentinel==99);++guards;check(dh2_character_debug_snapshot(owner,&loaded,&count)==1&&!loaded&&!count&&!provider.closes);dh2_character_debug_destroy(owner);
 // Actual caller-owned filesystem probe, using only workspace scratch.
 FileProvider real;real.actual=true;real.directory=argv[3];const auto path=real.directory+"/DebugSwitches.savegame";check(std::remove(path.c_str())==0||errno==ENOENT);auto real_files=real.services();owner=dh2_character_debug_create();check(owner&&dh2_character_debug_load(owner,&real_files)==1&&real.opens==1&&!real.closes);check(dh2_character_debug_get(&sentinel,owner,"isTracingChar_Stats",&real_files)==1&&!sentinel&&real.opens==1);check(dh2_character_debug_snapshot(owner,&loaded,&count)==1&&loaded==1&&count==7);dh2_character_debug_destroy(owner);
 {auto* f=std::fopen(path.c_str(),"wb");check(f);const char* text="existing file must not be silently ignored";check(std::fwrite(text,1,std::strlen(text),f)==std::strlen(text)&&!std::fclose(f));owner=dh2_character_debug_create();check(owner&&dh2_character_debug_load(owner,&real_files)==-3&&real.closes==1);dh2_character_debug_destroy(owner);check(!std::remove(path.c_str()));}
 auto input=Inputs(argv[2]);CharacterGameDesign design_owner;std::string error;check(design_owner.initialize(input.input,error));auto design=design_owner.borrow();std::uint32_t tick33=0,tick34=0;check(dh2_character_design_tick(&tick33,design.design(),0x33)==1&&tick33==3000);check(dh2_character_design_tick(&tick34,design.design(),0x34)==1&&tick34==1000);
 check(dh2_character_design_tick(&sentinel,design.design(),0x35)==-1&&!sentinel);++guards;
 TimerComposition timer;timer.design=design.design();ScriptLifecycleState64 lifecycle{};lifecycle.owner=timer.store.owner;lifecycle.timer33=lifecycle.timer34=-1;ScriptLifecycleServices16 lifecycle_services{&timer,&TimerComposition::invoke};
 for(unsigned pass=0;pass<2;++pass){check(dh2_character_script_lifecycle(&lifecycle,script_on_init,0,&lifecycle_services)==1&&lifecycle.timer33==0&&lifecycle.timer34==1&&timer.store.count==2&&timer.ticks==(pass+1)*2);check(timer.timers[0].duration_ms==3000&&timer.timers[1].duration_ms==1000&&timer.timers[0].repeat==-1&&timer.timers[1].repeat==-1&&timer.timers[0].event==0x33&&timer.timers[1].event==0x34&&timer.timers[0].active&&timer.timers[1].active&&!timer.timers[0].user_ref&&!timer.timers[1].user_ref);}
 // Actual monster and commons files; only its genuine existing Init
 // dependencies are bound, with explicit position/session receiver facts.
 auto common=file(argv[4]),monster=file(argv[5]);HostReceiver host;auto* levels=design.levels();host.rows.resize(levels->levels.size());for(unsigned i=0;i<host.rows.size();++i)std::memcpy(&host.rows[i],levels->levels[i].scalar.words+12,24);auto crypt=std::find(levels->level_names.begin(),levels->level_names.end(),"GOTHICUS_CRYPT_01");check(crypt!=levels->level_names.end());host.level.row_index=crypt-levels->level_names.begin();HostContextBindings16 host_bindings{{&host,&HostReceiver::invoke}};unsigned monster_cases=0,debug_queries=0,control_queries=0;
 for(const char* prototype:{"Crypt_Skeleton","Crypt_Ghost"})for(unsigned difficulty:{0u,1u,2u}){
  auto* characters=design.characters();auto found=std::find(characters->names.begin(),characters->names.end(),prototype);check(found!=characters->names.end());auto index=found-characters->names.begin();dh2::data::PropertyState props;dh2::data::reset_properties(*design.rules(),props,&characters->rows[index]);auto view=dh2::data::property_view(*design.rules(),props);check(!dh2_class_recalc_base(design.class_rows()->data(),design.class_rows()->size(),props.base.data(),&view));auto before_init=props;LevelModel32 model{};check(design.level_model(model,view,error));owner=dh2_character_debug_create();DebugCount observed{{owner,&real_files},0,0,{}};LevelBindings48 binding{&model,{&observed,&DebugCount::invoke},{0,0,0}};dh2::data::PropertySheet scratch{};PropertyBindings48 property{{props.resolved.data(),224,0},{scratch.data(),224,0},nullptr,nullptr};
  auto* vm=dh2_script_vm_create_empty(8*1024*1024);auto* aliases=dh2_script_alias_create();check(vm&&aliases&&!dh2_script_vm_open_source_libraries(vm)&&!dh2_script_alias_bind(vm,aliases)&&!design.bind(vm)&&!dh2_character_property_bind(vm,&property)&&!dh2_character_level_bind(vm,&binding)&&!dh2_script_scalar_bind(vm,nullptr)&&!dh2_character_host_context_bind(vm,&host_bindings)&&!dh2_script_vm_bind_source_values(vm,"GetPosition",&position,nullptr));host.level.difficulty=difficulty;
  check(!dh2_script_vm_load_source_file(vm,common.data(),common.size())&&!dh2_script_vm_load_source_file(vm,monster.data(),monster.size()));const auto* actual_init=dh2_script_alias_resolve(aliases,"OnInit");check(actual_init&&!std::strcmp(actual_init,"monster_OnInit"));check(!dh2_script_vm_call_discard_source(vm,actual_init,nullptr,0));std::int32_t actual_level=-1;check(!dh2_character_get_level(&actual_level,&view));auto& range=host.rows[host.level.row_index];const auto expected=std::max(range.minimum[difficulty],std::min(host.player.cached_level,range.maximum[difficulty]));check(actual_level==expected&&observed.loads==observed.queries&&observed.loads>0&&observed.loads<=2);check(observed.order==std::vector<unsigned>({361,362,411,412}));debug_queries+=observed.queries;const auto queries_before_control=observed.queries;auto control=before_init;auto control_view=dh2::data::property_view(*design.rules(),control);LevelModel32 control_model{};check(design.level_model(control_model,control_view,error));LevelServices16 control_services{&observed,&DebugCount::invoke};check(!dh2_character_set_level(&control_model,float(expected*256),&control_services)&&props.base==control.base&&props.saved==control.saved&&props.gear==control.gear&&props.resolved==control.resolved);check(dh2_character_debug_snapshot(owner,&loaded,&count)==1&&loaded==1&&count==7);control_queries+=observed.queries-queries_before_control;
  dh2_script_alias_clear_contents(aliases);dh2_script_vm_destroy(vm);dh2_script_alias_destroy(aliases);dh2_character_debug_destroy(owner);++monster_cases;
 }
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"original_debug_operations\":"<<operations<<",\"fresh_singleton_sessions\":"<<sessions<<",\"native_failure_prefixes\":"<<failures<<",\"native_guards\":"<<guards<<",\"actual_filesystem_missing_and_existing_probes\":2,\"genuine_constant_ticks\":[3000,1000],\"genuine_timer_initializations\":2,\"actual_monster_Init_cases\":"<<monster_cases<<",\"genuine_refill_debug_queries\":"<<debug_queries<<",\"direct_Level_control_queries\":"<<control_queries<<",\"source_HP_MP_debug_order_verified\":true,\"module_library\":\""<<origin(reinterpret_cast<void*>(&dh2_character_debug_load))<<"\",\"world_library\":\""<<origin(reinterpret_cast<void*>(&dh2_character_set_level))<<"\",\"runtime_library\":\""<<origin(reinterpret_cast<void*>(&dh2_script_vm_load_source_file))<<"\",\"constants_library\":\""<<origin(reinterpret_cast<void*>(&dh2_script_constants_lookup))<<"\",\"data_library\":\""<<origin(reinterpret_cast<void*>(&dh2_class_recalc_base))<<"\",\"source_debug_file_parser_supported\":false,\"Application_and_world_receivers_are_borrowed\":true}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
